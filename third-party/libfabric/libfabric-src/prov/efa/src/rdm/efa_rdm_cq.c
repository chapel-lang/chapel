/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */

#include "efa.h"
#include "efa_rdm_cq.h"
#include "efa_data_path_ops.h"
#include "ofi_util.h"
#include "efa_av.h"
#include "efa_cntr.h"
#include "efa_rdm_pke_cmd.h"
#include "efa_rdm_pke_utils.h"
#include "efa_rdm_pke_nonreq.h"
#include "efa_rdm_tracepoint.h"
#include "efa_rdm_pke_print.h"

static
const char *efa_rdm_cq_strerror(struct fid_cq *cq_fid, int prov_errno,
				const void *err_data, char *buf, size_t len)
{
	return err_data
		? (const char *) err_data
		: efa_strerror(prov_errno);
}

/**
 * @brief close a CQ of EFA RDM endpoint
 *
 * @param[in,out]	fid	fid of the CQ to be closed
 * @returns		0 on sucesss,
 * 			negative libfabric error code on error
 * @relates efa_rdm_cq
 */
static
int efa_rdm_cq_close(struct fid *fid)
{
	int ret, retv;
	struct efa_rdm_cq *cq;

	retv = 0;

	cq = container_of(fid, struct efa_rdm_cq, efa_cq.util_cq.cq_fid.fid);

	if (cq->efa_cq.ibv_cq.ibv_cq_ex) {
		ret = -ibv_destroy_cq(ibv_cq_ex_to_cq(cq->efa_cq.ibv_cq.ibv_cq_ex));
		if (ret) {
			EFA_WARN(FI_LOG_CQ, "Unable to close ibv cq: %s\n",
				fi_strerror(-ret));
			return ret;
		}
		cq->efa_cq.ibv_cq.ibv_cq_ex = NULL;
	}

	if (cq->shm_cq) {
		ret = fi_close(&cq->shm_cq->fid);
		if (ret) {
			EFA_WARN(FI_LOG_CQ, "Unable to close shm cq: %s\n", fi_strerror(-ret));
			retv = ret;
		}
	}

	ret = ofi_cq_cleanup(&cq->efa_cq.util_cq);
	if (ret)
		return ret;
	free(cq);
	return retv;
}

static struct fi_ops efa_rdm_cq_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = efa_rdm_cq_close,
	.bind = fi_no_bind,
	.control = fi_no_control,
	.ops_open = fi_no_ops_open,
};


/**
 * @brief handle rdma-core CQ completion resulted from IBV_WRITE_WITH_IMM
 *
 * This function handles hardware-assisted RDMA writes with immediate data at
 * remote endpoint.  These do not have a packet context, nor do they have a
 * connid available.
 *
 * @param[in]		ibv_cq_ex	extended ibv cq
 * @param[in]		flags		flags (such as FI_REMOTE_CQ_DATA)
 * @param[in]		ep	        efa_rdm_ep
 * @param[in]		pkt_entry	packet entry
 */
static
void efa_rdm_cq_proc_ibv_recv_rdma_with_imm_completion(
						       struct efa_ibv_cq *ibv_cq,
						       uint64_t flags,
						       struct efa_rdm_ep *ep,
						       struct efa_rdm_pke *pkt_entry
						       )
{
	struct util_cq *target_cq;
	int ret;
	fi_addr_t src_addr;
	struct efa_av *efa_av;
	uint32_t imm_data = efa_ibv_cq_wc_read_imm_data(ibv_cq);
	uint32_t len = efa_ibv_cq_wc_read_byte_len(ibv_cq);

	target_cq = ep->base_ep.util_ep.rx_cq;
	efa_av = ep->base_ep.av;

	if (ep->base_ep.util_ep.caps & FI_SOURCE) {

		/* Only check the explicit AV when writing completions */
		src_addr = efa_av_reverse_lookup_rdm(efa_av,
						efa_ibv_cq_wc_read_slid(ibv_cq),
						efa_ibv_cq_wc_read_src_qp(ibv_cq),
						NULL);
		ret = ofi_cq_write_src(target_cq, NULL, flags, len, NULL, imm_data, 0, src_addr);
	} else {
		ret = ofi_cq_write(target_cq, NULL, flags, len, NULL, imm_data, 0);
	}

	if (OFI_UNLIKELY(ret)) {
		EFA_WARN(FI_LOG_CQ,
			"Unable to write a cq entry for remote for RECV_RDMA operation: %s\n",
			fi_strerror(-ret));
		efa_base_ep_write_eq_error(&ep->base_ep, -ret, FI_EFA_ERR_WRITE_RECV_COMP);
	}

	efa_cntr_report_rx_completion(&ep->base_ep.util_ep, flags);

	/**
	 * For unsolicited wc, pkt_entry can be NULL, so we can only
	 * access it for solicited wc.
	 */
	if (!efa_cq_wc_is_unsolicited(ibv_cq)) {
		/**
		 * Recv with immediate will consume a pkt_entry, but the pkt is not
		 * filled, so free the pkt_entry and record we have one less posted
		 * packet now.
		 */
		assert(pkt_entry);
		ep->efa_rx_pkts_posted--;
		efa_rdm_pke_release_rx(pkt_entry);
	}
}

/**
 * @brief Populate the efa_ep_addr struct with the raw address, QPN and QKEY
 * This function tries to read the raw address first from the packet header
 * If the packet header does not have the raw address, it uses
 * efadv_wc_read_sgid if available.
 * @return FI_SUCCESS if the efa_ep_addr struct is populated. Error otherwise.
 */
static inline int efa_rdm_cq_populate_src_efa_ep_addr(
	struct efa_rdm_pke *pkt_entry, struct efa_ibv_cq *ibv_cq,
	struct efa_ep_addr *efa_ep_addr)
{
	struct efa_rdm_base_hdr *base_hdr;
	struct efa_rdm_ep *ep;
	void *raw_addr_ptr = NULL;
	uint32_t *connid;

	ep = pkt_entry->ep;
	assert(ep);

	efa_ep_addr->qpn = efa_ibv_cq_wc_read_src_qp(ibv_cq);

	if (pkt_entry->alloc_type == EFA_RDM_PKE_FROM_USER_RX_POOL) {
		/* Receive packet posted in the zero-copy path does not have a
		 * header. So we cannot read the connid or the raw address. */
		return FI_EADDRNOTAVAIL;
	}

	connid = efa_rdm_pke_connid_ptr(pkt_entry);
	if (!connid) {
		return FI_EADDRNOTAVAIL;
	}
	efa_ep_addr->qkey = *connid;

	base_hdr = efa_rdm_pke_get_base_hdr(pkt_entry);
	if (base_hdr->version < EFA_RDM_PROTOCOL_VERSION) {
		char self_raw_addr_str[OFI_ADDRSTRLEN];
		size_t buflen = OFI_ADDRSTRLEN;

		efa_base_ep_raw_addr_str(&ep->base_ep, self_raw_addr_str,
					 &buflen);
		EFA_WARN(FI_LOG_CQ,
			 "Host %s received a packet with invalid protocol "
			 "version %d.\n"
			 "This host can only support protocol version %d and "
			 "above.\n",
			 self_raw_addr_str, base_hdr->version,
			 EFA_RDM_PROTOCOL_VERSION);
		efa_base_ep_write_eq_error(&ep->base_ep, FI_EIO,
					   FI_EFA_ERR_INVALID_PKT_TYPE);
		fprintf(stderr,
			"Host %s received a packet with invalid protocol "
			"version %d.\n"
			"This host can only support protocol version %d and "
			"above. %s:%d\n",
			self_raw_addr_str, base_hdr->version,
			EFA_RDM_PROTOCOL_VERSION, __FILE__, __LINE__);
		abort();
	}

	/* Attempt to read raw address from packet header */
	if (base_hdr->type >= EFA_RDM_REQ_PKT_BEGIN &&
	    efa_rdm_pke_get_req_raw_addr(pkt_entry)) {
		raw_addr_ptr = efa_rdm_pke_get_req_raw_addr(pkt_entry);
		assert(raw_addr_ptr);
		goto out;
	}

#if HAVE_EFADV_CQ_EX
	union ibv_gid gid = {0};
	if (ibv_cq->ibv_cq_ex_type != EFADV_CQ) {
		/* EFA DV CQ is not supported. This could be due to old EFA
		 * kernel module versions. */
		return FI_EADDRNOTAVAIL;
	}

	/* Attempt to read sgid from EFA firmware */
	if (efa_ibv_cq_wc_read_sgid(ibv_cq, &gid) < 0) {
		/* Return code is negative if the peer AH is known */
		return FI_EADDRNOTAVAIL;
	}

	raw_addr_ptr = &gid.raw;
#endif

out:
	if (raw_addr_ptr) {
		memcpy(efa_ep_addr->raw, raw_addr_ptr,
		       sizeof(efa_ep_addr->raw));
		return FI_SUCCESS;
	}

	return FI_EADDRNOTAVAIL;
}

/**
 * @brief Read peer raw address from packet entry or the EFA device and look up
 * the peer address in explicit and implicit AVs This function should only be
 * called if the peer AH is unknown.
 * @return Pointer to peer struct, or NULL if unavailable.
 */
static inline struct efa_rdm_peer *
efa_rdm_cq_lookup_raw_addr(struct efa_rdm_pke *pke,
			   struct efa_ep_addr *efa_ep_addr)
{
	struct efa_rdm_ep *ep;
	fi_addr_t addr;
	struct efa_rdm_peer *peer = NULL;
	bool implicit = false;
	char gid_str_cdesc[INET6_ADDRSTRLEN];

	ep = pke->ep;
	assert(ep);

	/* First check the explicit AV */
	addr = ofi_av_lookup_fi_addr(&ep->base_ep.av->util_av,
				     (void *) efa_ep_addr);
	if (addr != FI_ADDR_NOTAVAIL) {
		implicit = false;
		peer = efa_rdm_ep_get_peer(ep, addr);
		assert(peer);
		goto out;
	}

	/* Next check implicit AV */
	addr = ofi_av_lookup_fi_addr(&ep->base_ep.av->util_av_implicit,
				     (void *) efa_ep_addr);
	if (addr != FI_ADDR_NOTAVAIL) {
		implicit = true;
		peer = efa_rdm_ep_get_peer_implicit(ep, addr);
		assert(peer);
		goto out;
	}

	return NULL;

out:
	inet_ntop(AF_INET6, efa_ep_addr->raw, gid_str_cdesc, INET6_ADDRSTRLEN);
	EFA_WARN(FI_LOG_AV,
		 "Recovered fi_addr for peer:[QPN]:[QKey] = "
		 "[%s]:[%" PRIu16 "]:[%" PRIu32 "] fi_addr: %" PRIu64
		 " implicit AV: %s\n",
		 gid_str_cdesc, efa_ep_addr->qpn, efa_ep_addr->qkey, addr,
		 implicit ? "true" : "false");

	return peer;
}

/**
 * @brief Determine peer struct from ibv_cq and packet entry
 * Insert the peer into the implicit AV if not found. This
 * happens when the endpoint receives the first packet from a new peer.
 * @param ep Pointer to RDM endpoint
 * @param ibv_cq Pointer to CQ
 * @param pkt_entry packet entry
 * @returns Peer address, or FI_ADDR_NOTAVAIL if unsuccessful.
 */
static inline struct efa_rdm_peer *
efa_rdm_cq_get_peer_for_pkt_entry(struct efa_rdm_ep *ep,
				  struct efa_ibv_cq *efa_ibv_cq,
				  struct efa_rdm_pke *pkt_entry)
{
	struct efa_av *efa_av = ep->base_ep.av;
	fi_addr_t explicit_fi_addr, implicit_fi_addr;
	struct efa_ep_addr efa_ep_addr = {0};
	struct efa_ep_addr_hashable *efa_ep_addr_hashable = NULL;
	struct efa_rdm_peer *peer = NULL;
	int ret;
	uint32_t gid;
	uint32_t qpn;

	gid = efa_ibv_cq_wc_read_slid(efa_ibv_cq);
	qpn = efa_ibv_cq_wc_read_src_qp(efa_ibv_cq);

	EFA_DBG(FI_LOG_CQ,
		"Attempting to retrieve peer for packet from peer with gid %d "
		"and qpn %d\n",
		gid, qpn);

	/* To determine the source peer struct, the workflow is the following
	 * 1. Get GID and QPN from rdma-core and check the explicit AV
	 * 2. If not found, check the implicit AV with GID and QPN
	 * 3 (a). If not found, retrieve raw address from the packet header
	 * 3 (b). If packet header doesn't have raw address, retrieve raw
	 * address with efadv_wc_read_sgid
	 * 4. Check explicit and implicit AVs for the raw address retrieved in
	 * (3)
	 * 5. If not found and raw address is available, insert raw address into
	 * the implicit AV
	 *
	 * TODO: Remove the usage of efadv_wc_read_sgid after EFA device's
	 * behavior is fixed
	 */
	explicit_fi_addr =
		efa_av_reverse_lookup_rdm(efa_av, gid, qpn, pkt_entry);

	if (explicit_fi_addr != FI_ADDR_NOTAVAIL) {
		EFA_DBG(FI_LOG_CQ,
			"Peer with gid %d and qpn %d found in explicit AV with "
			"fi_addr %ld\n",
			gid, qpn, explicit_fi_addr);
		peer = efa_rdm_ep_get_peer(ep, explicit_fi_addr);
		goto out;
	}

	implicit_fi_addr =
		efa_av_reverse_lookup_rdm_implicit(efa_av, gid, qpn, pkt_entry);

	if (implicit_fi_addr != FI_ADDR_NOTAVAIL) {
		EFA_DBG(FI_LOG_CQ,
			"Peer with gid %d and qpn %d found in implicit AV with "
			"fi_addr %ld\n",
			gid, qpn, implicit_fi_addr);
		peer = efa_rdm_ep_get_peer_implicit(ep, implicit_fi_addr);
		goto out;
	}

	ret = efa_rdm_cq_populate_src_efa_ep_addr(
		pkt_entry, efa_ibv_cq,
		&efa_ep_addr);
	if (ret) {
		/* Failed to read raw address from packet entry and
		 * efadv_wc_read_sgid */
		return NULL;
	}

	/* If the packet is from a peer that we evicted from the implicit AV,
	 * print a warning and ignore the packet. We do this because we lose
	 * information about previous communication from the peer when we evict
	 * the peer from the implicit AV
	 *
	 * TODO: continue communication with peer by saving the previous state
	 * and restoring it
	 */
	HASH_FIND(hh, ep->base_ep.av->evicted_peers_hashset, &efa_ep_addr,
		  sizeof(struct efa_ep_addr), efa_ep_addr_hashable);
	if (OFI_UNLIKELY(!!efa_ep_addr_hashable)) {
		EFA_WARN(FI_LOG_CQ, "Received packet from peer already evicted "
				    "from the implicit AV\n");
		return NULL;
	}

	peer = efa_rdm_cq_lookup_raw_addr(pkt_entry, &efa_ep_addr);
	if (peer)
		goto out;

	EFA_DBG(FI_LOG_CQ,
		"Peer with gid %d and qpn %d not found in explicit or implicit "
		"AV. Attempting to insert into implicit AV...\n",
		gid, qpn);
	/*
	 * The message is from a peer through efa device, which means peer is
	 * not local or shm is disabled for transmission. We shouldn't insert
	 * in to shm av in this case.
	 */
	ret = efa_av_insert_one(ep->base_ep.av, &efa_ep_addr, &implicit_fi_addr,
				0, NULL, false, true);
	if (OFI_UNLIKELY(ret != 0)) {
		efa_base_ep_write_eq_error(&ep->base_ep, ret,
					   FI_EFA_ERR_AV_INSERT);
		return NULL;
	}
	assert(implicit_fi_addr != FI_ADDR_NOTAVAIL);
	peer = efa_rdm_ep_get_peer_implicit(ep, implicit_fi_addr);

out:
	assert(peer);
	assert((peer->conn->fi_addr != FI_ADDR_NOTAVAIL &&
		peer->conn->implicit_fi_addr == FI_ADDR_NOTAVAIL) ||
	       (peer->conn->implicit_fi_addr != FI_ADDR_NOTAVAIL &&
		peer->conn->fi_addr == FI_ADDR_NOTAVAIL));
	return peer;
}

/**
 * @brief handle a received packet
 *
 * @param	ep[in,out]		endpoint
 * @param	pkt_entry[in,out]	received packet, will be released by this function
 */
static void efa_rdm_cq_handle_recv_completion(struct efa_ibv_cq *ibv_cq, struct efa_rdm_pke *pkt_entry, struct efa_rdm_ep *ep)
{
	int pkt_type;
	struct efa_rdm_base_hdr *base_hdr;
	uint32_t imm_data = 0;
	bool has_imm_data = false;

	if (pkt_entry->alloc_type == EFA_RDM_PKE_FROM_USER_RX_POOL) {
		assert(ep->user_rx_pkts_posted > 0);
		ep->user_rx_pkts_posted--;
	} else {
		assert(ep->efa_rx_pkts_posted > 0);
		ep->efa_rx_pkts_posted--;
	}

	pkt_entry->pkt_size = efa_ibv_cq_wc_read_byte_len(ibv_cq);
	if (efa_ibv_cq_wc_read_wc_flags(ibv_cq) & IBV_WC_WITH_IMM) {
		has_imm_data = true;
		imm_data = efa_ibv_cq_wc_read_imm_data(ibv_cq);
	}

	pkt_entry->peer = efa_rdm_cq_get_peer_for_pkt_entry(ep, ibv_cq, pkt_entry);

	/*
	 * Ignore packet if peer address cannot be determined. This ususally happens if
	 * we had prior communication with the peer, but
	 * application called fi_av_remove() to remove the address
	 * from address vector.
	 */
	if (!pkt_entry->peer) {
		EFA_WARN(FI_LOG_CQ,
			"Warning: ignoring a received packet from a removed address. packet type: %" PRIu8
			", packet flags: %x\n",
			efa_rdm_pke_get_base_hdr(pkt_entry)->type,
			efa_rdm_pke_get_base_hdr(pkt_entry)->flags);
		efa_rdm_pke_release_rx(pkt_entry);
		return;
	}

#if ENABLE_DEBUG
	dlist_remove(&pkt_entry->dbg_entry);
	dlist_insert_tail(&pkt_entry->dbg_entry, &ep->rx_pkt_list);
#ifdef ENABLE_EFA_RDM_PKE_DUMP
	efa_rdm_pke_print(pkt_entry, "Received");
#endif
#endif
	if (pkt_entry->peer->is_local) {
		/*
		 * This happens when the peer is on same instance, but chose to
		 * use EFA device to communicate with me. In this case, we respect
		 * that and will not use shm with the peer.
		 * TODO: decide whether to use shm through handshake packet.
		 */
		pkt_entry->peer->is_local = 0;
	}

	efa_rdm_ep_post_handshake_or_queue(ep, pkt_entry->peer);

	/**
	 * Data is already delivered to user posted pkt without pkt hdrs.
	 */
	if (pkt_entry->alloc_type == EFA_RDM_PKE_FROM_USER_RX_POOL) {
		assert(ep->base_ep.user_recv_qp);
		/* User recv pkts are only posted to the user recv qp */
		assert(efa_ibv_cq_wc_read_qp_num(ibv_cq) == ep->base_ep.user_recv_qp->qp_num);
		return efa_rdm_pke_proc_received_no_hdr(pkt_entry, has_imm_data, imm_data);
	}

	/* Proc receives with pkt hdrs (posted to ctrl QPs)*/
	base_hdr = efa_rdm_pke_get_base_hdr(pkt_entry);
	pkt_type = base_hdr->type;
	if (OFI_UNLIKELY(pkt_type >= EFA_RDM_EXTRA_REQ_PKT_END)) {
		EFA_WARN(FI_LOG_CQ,
			 "Peer fi_addr: %ld implicit fi_addr %ld is requesting "
			 "feature %d, which this EP does not support.\n",
			 pkt_entry->peer->conn->fi_addr,
			 pkt_entry->peer->conn->implicit_fi_addr,
			 base_hdr->type);

		assert(0 && "invalid REQ packet type");
		efa_base_ep_write_eq_error(&ep->base_ep, FI_EIO, FI_EFA_ERR_INVALID_PKT_TYPE);
		efa_rdm_pke_release_rx(pkt_entry);
		return;
	}

	/**
	 * When zero copy recv is turned on, the ep cannot
	 * handle rtm pkts delivered to the internal bounce buffer,
	 * because the user recv buffer has been posted to the other
	 * QP and we cannot cancel that.
	 */
	if (OFI_UNLIKELY(ep->use_zcpy_rx && efa_rdm_pkt_type_is_rtm(pkt_type))) {
		char errbuf[EFA_ERROR_MSG_BUFFER_LENGTH] = {0};
		size_t errbuf_len;

		/* local & peer host-id & ep address will be logged by efa_rdm_write_error_msg */
		if (!efa_rdm_write_error_msg(ep, pkt_entry->peer, FI_EFA_ERR_INVALID_PKT_TYPE_ZCPY_RX, errbuf, &errbuf_len))
			EFA_WARN(FI_LOG_CQ, "Error: %s\n", (const char *) errbuf);
		efa_base_ep_write_eq_error(&ep->base_ep, FI_EINVAL, FI_EFA_ERR_INVALID_PKT_TYPE_ZCPY_RX);
		efa_rdm_pke_release_rx(pkt_entry);
		return;
	}

	efa_rdm_pke_proc_received(pkt_entry);
}


/**
 * @brief Get the vendor error code for an endpoint's CQ
 *
 * This function is essentially a wrapper for `ibv_cq->read_vendor_err()`; making
 * a best-effort attempt to promote the error code to a proprietary EFA
 * provider error code.
 *
 * @param[in]	ibv_cq_ex	IBV CQ
 * @param[in]	peer	efa_rdm_peer struct of sender
 * @return	EFA-specific error code
 * @sa		#EFA_PROV_ERRNOS
 *
 * @todo Currently, this only checks for unresponsive receiver
 * (#EFA_IO_COMP_STATUS_LOCAL_ERROR_UNRESP_REMOTE) and attempts to promote it to
 * #FI_EFA_ERR_ESTABLISHED_RECV_UNRESP if a handshake was made, or
 * #FI_EFA_ERR_UNESTABLISHED_RECV_UNRESP if the handshake failed. 
 * This should be expanded to handle other
 * RDMA Core error codes (#EFA_IO_COMP_STATUSES) for the sake of more accurate
 * error reporting
 */
static int efa_rdm_cq_get_prov_errno(struct efa_ibv_cq *ibv_cq, struct efa_rdm_peer *peer) {
	uint32_t vendor_err = efa_ibv_cq_wc_read_vendor_err(ibv_cq);

	if (OFI_UNLIKELY(!peer)) {
		return vendor_err;
	}

	switch (vendor_err) {
	case EFA_IO_COMP_STATUS_LOCAL_ERROR_UNRESP_REMOTE: {
		vendor_err = (peer->flags & EFA_RDM_PEER_HANDSHAKE_RECEIVED) ?
			     FI_EFA_ERR_ESTABLISHED_RECV_UNRESP :
			     FI_EFA_ERR_UNESTABLISHED_RECV_UNRESP;
		break;
	}
	default:
		break;
	}

	return vendor_err;
}

static int efa_rdm_cq_match_ep(struct dlist_entry *item, const void *ep)
{
	return (container_of(item, struct efa_rdm_ep, entry) == ep) ;
}

static inline struct efa_rdm_ep *efa_rdm_cq_get_rdm_ep(struct efa_ibv_cq *cq, struct efa_domain *efa_domain)
{
	struct efa_base_ep *base_ep = efa_domain->qp_table[efa_ibv_cq_wc_read_qp_num(cq) & efa_domain->qp_table_sz_m1]->base_ep;
	return container_of(base_ep, struct efa_rdm_ep, base_ep);
}

/**
 * @brief Process work completions for a closing endpoint
 *
 * This is a lighter-weight counterpart to #efa_rdm_cq_process_wc(); avoiding
 * unnecessary overhead for processing completions for an endpoint that's
 * closing anyway by simply releasing the packet entries. Exceptions include
 * RECEIPT and EOR packets when a completion fails due to RNR and resource
 * management is enabled by the user (FI_RM_ENABLED). In this case, packets are
 * queued to be reposted.
 *
 * @param[in]	cq	IBV CQ
 * @param[in]	ep	EFA RDM endpoint (to be closed)
 * @return	Status code for the work completion
 */
static inline
enum ibv_wc_status efa_rdm_cq_process_wc_closing_ep(struct efa_ibv_cq *cq, struct efa_rdm_ep *ep)
{
	uint64_t wr_id = cq->ibv_cq_ex->wr_id;
	enum ibv_wc_status status = cq->ibv_cq_ex->status;
	enum ibv_wc_opcode opcode = efa_ibv_cq_wc_read_opcode(cq);
	struct efa_rdm_pke *pkt_entry = (struct efa_rdm_pke *) wr_id;
	int prov_errno;

#if HAVE_LTTNG
	efa_rdm_tracepoint(poll_cq, (size_t) wr_id);
	if (pkt_entry && pkt_entry->ope)
		efa_rdm_tracepoint(poll_cq_ope, pkt_entry->ope->msg_id,
				   (size_t) pkt_entry->ope->cq_entry.op_context,
				   pkt_entry->ope->total_len, pkt_entry->ope->cq_entry.tag,
				   pkt_entry->ope->peer ? pkt_entry->ope->peer->conn->fi_addr : FI_ADDR_NOTAVAIL);
#endif

	if (!efa_cq_wc_is_unsolicited(cq)) {
		if (OFI_UNLIKELY(status != IBV_WC_SUCCESS)) {
			prov_errno = efa_rdm_cq_get_prov_errno(cq, pkt_entry->peer);
			if (prov_errno == EFA_IO_COMP_STATUS_REMOTE_ERROR_RNR &&
				ep->handle_resource_management == FI_RM_ENABLED) {
				switch(efa_rdm_pkt_type_of(pkt_entry)) {
				case EFA_RDM_RECEIPT_PKT:
				case EFA_RDM_EOR_PKT:
					efa_rdm_ep_record_tx_op_completed(ep, pkt_entry);
					efa_rdm_ep_queue_rnr_pkt(ep, pkt_entry);
					return status;
				default:
					break;
				}
			}
		}
		switch (opcode) {
		case IBV_WC_SEND: /* fall through */
		case IBV_WC_RDMA_WRITE: /* fall through */
		case IBV_WC_RDMA_READ:
			efa_rdm_ep_record_tx_op_completed(ep, pkt_entry);
			efa_rdm_pke_release_tx(pkt_entry);
			break;
		case IBV_WC_RECV: /* fall through */
		case IBV_WC_RECV_RDMA_WITH_IMM:
			if (pkt_entry->alloc_type == EFA_RDM_PKE_FROM_USER_RX_POOL) {
				assert(ep->user_rx_pkts_posted > 0);
				ep->user_rx_pkts_posted--;
			} else {
				assert(ep->efa_rx_pkts_posted > 0);
				ep->efa_rx_pkts_posted--;
			}
			efa_rdm_pke_release_rx(pkt_entry);
			break;
		default:
			EFA_WARN(FI_LOG_EP_CTRL, "Unhandled opcode: %d\n", opcode);
			assert(0 && "Unhandled opcode");
		}
	}
	return status;
}

/**
 * @brief Process work completions
 *
 * @param[in]	cq	IBV CQ
 * @param[in]	ep	EFA RDM endpoint
 * @return	Status code for the work completion
 */
static inline
enum ibv_wc_status efa_rdm_cq_process_wc(struct efa_ibv_cq *cq, struct efa_rdm_ep *ep)
{
	uint64_t wr_id = cq->ibv_cq_ex->wr_id;
	enum ibv_wc_status status = cq->ibv_cq_ex->status;
	enum ibv_wc_opcode opcode = efa_ibv_cq_wc_read_opcode(cq);
	struct efa_rdm_pke *pkt_entry = (struct efa_rdm_pke *) wr_id;
	int prov_errno;

#if HAVE_LTTNG
	efa_rdm_tracepoint(poll_cq, (size_t) wr_id);
	if (pkt_entry && pkt_entry->ope)
		efa_rdm_tracepoint(poll_cq_ope, pkt_entry->ope->msg_id,
				   (size_t) pkt_entry->ope->cq_entry.op_context,
				   pkt_entry->ope->total_len, pkt_entry->ope->cq_entry.tag,
				   pkt_entry->ope->peer ? pkt_entry->ope->peer->conn->fi_addr : FI_ADDR_NOTAVAIL);
#endif

	if (OFI_UNLIKELY(status != IBV_WC_SUCCESS)) {
		prov_errno = efa_rdm_cq_get_prov_errno(cq, pkt_entry ? pkt_entry->peer : NULL);
		switch (opcode) {
		case IBV_WC_SEND: /* fall through */
		case IBV_WC_RDMA_WRITE: /* fall through */
		case IBV_WC_RDMA_READ:
			assert(pkt_entry);
			efa_rdm_pke_handle_tx_error(pkt_entry, prov_errno);
			break;
		case IBV_WC_RECV: /* fall through */
		case IBV_WC_RECV_RDMA_WITH_IMM:
			if (efa_cq_wc_is_unsolicited(cq)) {
				EFA_WARN(FI_LOG_CQ, "Receive error %s (%d) for unsolicited write recv",
					efa_strerror(prov_errno), prov_errno);
				efa_base_ep_write_eq_error(&ep->base_ep, to_fi_errno(prov_errno), prov_errno);
				break;
			}
			assert(pkt_entry);
			efa_rdm_pke_handle_rx_error(pkt_entry, prov_errno);
			break;
		default:
			EFA_WARN(FI_LOG_EP_CTRL, "Unhandled opcode: %d\n", opcode);
			assert(0 && "Unhandled opcode");
		}
	} else {
		switch (opcode) {
		case IBV_WC_SEND:
#if ENABLE_DEBUG
			ep->send_comps++;
#endif
			efa_rdm_pke_handle_send_completion(pkt_entry);
			break;
		case IBV_WC_RECV:
			/* efa_rdm_cq_handle_recv_completion does additional work to determine the source
			 * address and the peer struct. So do not try to identify the peer here. */
			efa_rdm_cq_handle_recv_completion(cq, pkt_entry, ep);
#if ENABLE_DEBUG
			ep->recv_comps++;
#endif
			break;
		case IBV_WC_RDMA_READ:
		case IBV_WC_RDMA_WRITE:
			efa_rdm_pke_handle_rma_completion(pkt_entry);
			break;
		case IBV_WC_RECV_RDMA_WITH_IMM:
			efa_rdm_cq_proc_ibv_recv_rdma_with_imm_completion(
				cq,
				FI_REMOTE_CQ_DATA | FI_RMA | FI_REMOTE_WRITE,
				ep, pkt_entry);
			break;
		default:
			EFA_WARN(FI_LOG_EP_CTRL,
				"Unhandled cq type\n");
			assert(0 && "Unhandled cq type");
		}
	}
	return status;
}

void efa_rdm_cq_poll_ibv_cq_closing_ep(struct efa_ibv_cq *ibv_cq, struct efa_rdm_ep *closing_ep)
{

	struct efa_rdm_ep *ep = NULL;
	struct efa_cq *efa_cq = container_of(ibv_cq, struct efa_cq, ibv_cq);
	struct efa_domain *efa_domain = container_of(efa_cq->util_cq.domain, struct efa_domain, util_domain);
	struct dlist_entry rx_progressed_ep_list, *tmp;

	dlist_init(&rx_progressed_ep_list);

	efa_cq_start_poll(ibv_cq);
	while (efa_cq_wc_available(ibv_cq)) {
		ep = efa_rdm_cq_get_rdm_ep(ibv_cq, efa_domain);
		if (ep == closing_ep) {
			if (OFI_UNLIKELY(efa_rdm_cq_process_wc_closing_ep(ibv_cq, ep) != IBV_WC_SUCCESS))
				break;
		} else {
			if (OFI_UNLIKELY(efa_rdm_cq_process_wc(ibv_cq, ep) != IBV_WC_SUCCESS))
				break;
			if (ep->efa_rx_pkts_to_post > 0 && !dlist_find_first_match(&rx_progressed_ep_list, &efa_rdm_cq_match_ep, ep))
				dlist_insert_tail(&ep->entry, &rx_progressed_ep_list);
		}
		efa_cq_next_poll(ibv_cq);
	}
	efa_cq_end_poll(ibv_cq);
	dlist_foreach_container_safe(
		&rx_progressed_ep_list, struct efa_rdm_ep, ep, entry, tmp) {
		efa_rdm_ep_post_internal_rx_pkts(ep);
		dlist_remove(&ep->entry);
	}
	assert(dlist_empty(&rx_progressed_ep_list));
}

/**
 * @brief poll rdma-core cq and process the cq entry
 *
 * @param[in]	ep_poll	the RDM endpoint that polls ibv cq. Note this polling endpoint can be different
 * from the endpoint that the completed packet entry was posted from (pkt_entry->ep).
 * @param[in]	cqe_to_process	Max number of cq entry to poll and process. A negative number means to poll until cq empty
 */
int efa_rdm_cq_poll_ibv_cq(ssize_t cqe_to_process, struct efa_ibv_cq *ibv_cq)
{
	int err;
	size_t i = 0;
	struct efa_rdm_ep *ep = NULL;
	struct efa_cq *efa_cq = container_of(ibv_cq, struct efa_cq, ibv_cq);
	struct efa_domain *efa_domain = container_of(efa_cq->util_cq.domain, struct efa_domain, util_domain);

	struct dlist_entry rx_progressed_ep_list, *tmp;

	dlist_init(&rx_progressed_ep_list);

	/* Call ibv_start_poll only once */
	efa_cq_start_poll(ibv_cq);

	while (efa_cq_wc_available(ibv_cq)) {
		ep = efa_rdm_cq_get_rdm_ep(ibv_cq, efa_domain);
		if (OFI_UNLIKELY(efa_rdm_cq_process_wc(ibv_cq, ep) != IBV_WC_SUCCESS))
			break;
		if (ep->efa_rx_pkts_to_post > 0 && !dlist_find_first_match(&rx_progressed_ep_list, &efa_rdm_cq_match_ep, ep))
			dlist_insert_tail(&ep->entry, &rx_progressed_ep_list);
		if (++i >= cqe_to_process)
			break;

		/*
		 * ibv_next_poll MUST be call after the current WC is fully processed,
		 * which prevents later calls on ibv_cq_ex from reading the wrong WC.
		 */
		efa_cq_next_poll(ibv_cq);
	}

	err = ibv_cq->poll_err;
	efa_cq_end_poll(ibv_cq);
	dlist_foreach_container_safe(
		&rx_progressed_ep_list, struct efa_rdm_ep, ep, entry, tmp) {
		efa_rdm_ep_post_internal_rx_pkts(ep);
		dlist_remove(&ep->entry);
	}
	assert(dlist_empty(&rx_progressed_ep_list));

	return err;
}

static ssize_t efa_rdm_cq_readfrom(struct fid_cq *cq_fid, void *buf, size_t count, fi_addr_t *src_addr)
{
	struct efa_rdm_cq *cq;
	ssize_t ret;
	struct efa_domain *domain;

	cq = container_of(cq_fid, struct efa_rdm_cq, efa_cq.util_cq.cq_fid.fid);

	domain = container_of(cq->efa_cq.util_cq.domain, struct efa_domain, util_domain);

	ofi_genlock_lock(&domain->srx_lock);

	if (cq->shm_cq) {
		fi_cq_read(cq->shm_cq, NULL, 0);

		/* 
		 * fi_cq_read(cq->shm_cq, NULL, 0) will progress shm ep and write
		 * completion to efa. Use ofi_cq_read_entries to get the number of
		 * shm completions without progressing efa ep again.
		 */
		ret = ofi_cq_read_entries(&cq->efa_cq.util_cq, buf, count, src_addr);

		if (ret > 0)
			goto out;
	}

	ret = ofi_cq_readfrom(&cq->efa_cq.util_cq.cq_fid, buf, count, src_addr);

out:
	ofi_genlock_unlock(&domain->srx_lock);

	return ret;
}

static struct fi_ops_cq efa_rdm_cq_ops = {
	.size = sizeof(struct fi_ops_cq),
	.read = ofi_cq_read,
	.readfrom = efa_rdm_cq_readfrom,
	.readerr = ofi_cq_readerr,
	.sread = fi_no_cq_sread,
	.sreadfrom = fi_no_cq_sreadfrom,
	.signal = fi_no_cq_signal,
	.strerror = efa_rdm_cq_strerror,
};

static void efa_rdm_cq_progress(struct util_cq *cq)
{
	struct dlist_entry *item;
	struct efa_rdm_cq *efa_rdm_cq;
	struct efa_ibv_cq_poll_list_entry *poll_list_entry;
	struct efa_domain *efa_domain;
	struct efa_rdm_ep *efa_rdm_ep;
	struct fid_list_entry *fid_entry;

	ofi_genlock_lock(&cq->ep_list_lock);
	efa_rdm_cq = container_of(cq, struct efa_rdm_cq, efa_cq.util_cq);
	efa_domain = container_of(efa_rdm_cq->efa_cq.util_cq.domain, struct efa_domain, util_domain);

	/**
	 * TODO: It's better to just post the initial batch of internal rx pkts during ep enable
	 * so we don't have to iterate cq->ep_list here.
	 * However, it is observed that doing that will hurt performance if application opens
	 * some idle endpoints and never poll completions for them. Move these initial posts to
	 * the first cq read call before having a long term fix.
	 */
	if (efa_rdm_cq->need_to_scan_ep_list) {
		dlist_foreach(&cq->ep_list, item) {
			fid_entry = container_of(item, struct fid_list_entry, entry);
			efa_rdm_ep = container_of(fid_entry->fid, struct efa_rdm_ep, base_ep.util_ep.ep_fid.fid);
			if (efa_rdm_ep->base_ep.efa_qp_enabled)
				efa_rdm_ep_post_internal_rx_pkts(efa_rdm_ep);
		}
		efa_rdm_cq->need_to_scan_ep_list = false;
	}

	dlist_foreach(&efa_rdm_cq->ibv_cq_poll_list, item) {
		poll_list_entry = container_of(item, struct efa_ibv_cq_poll_list_entry, entry);
		(void) efa_rdm_cq_poll_ibv_cq(efa_env.efa_cq_read_size, poll_list_entry->cq);
	}
	efa_domain_progress_rdm_peers_and_queues(efa_domain);
	ofi_genlock_unlock(&cq->ep_list_lock);
}

/**
 * @brief create a CQ for EFA RDM provider
 *
 * Note that EFA RDM provider used the util_cq as its CQ
 *
 * @param[in]		domain		efa domain
 * @param[in]		attr		cq attribuite
 * @param[out]		cq_fid 		fid of the created cq
 * @param[in]		context 	currently EFA provider does not accept any context
 * @returns		0 on success
 * 			negative libfabric error code on error
 * @relates efa_rdm_cq
 */
int efa_rdm_cq_open(struct fid_domain *domain, struct fi_cq_attr *attr,
		    struct fid_cq **cq_fid, void *context)
{
	int ret, retv;
	struct efa_rdm_cq *cq;
	struct efa_domain *efa_domain;
	struct fi_cq_attr shm_cq_attr = {0};
	struct fi_peer_cq_context peer_cq_context = {0};
	struct fi_efa_cq_init_attr efa_cq_init_attr = {0};

	if (attr->wait_obj != FI_WAIT_NONE)
		return -FI_ENOSYS;

	cq = calloc(1, sizeof(*cq));
	if (!cq)
		return -FI_ENOMEM;

	efa_domain = container_of(domain, struct efa_domain,
				  util_domain.domain_fid);
	/* Override user cq size if it's less than recommended cq size */
	attr->size = MAX(efa_domain->rdm_cq_size, attr->size);

	dlist_init(&cq->ibv_cq_poll_list);
	cq->need_to_scan_ep_list = false;
	ret = ofi_cq_init(&efa_prov, domain, attr, &cq->efa_cq.util_cq,
			  &efa_rdm_cq_progress, context);

	if (ret)
		goto free;

	ret = efa_cq_open_ibv_cq(
		attr, efa_domain->device->ibv_ctx, &cq->efa_cq.ibv_cq,
		&efa_cq_init_attr);
	if (ret) {
		EFA_WARN(FI_LOG_CQ, "Unable to create extended CQ: %s\n", fi_strerror(ret));
		goto close_util_cq;
	}

	cq->efa_cq.poll_ibv_cq = efa_rdm_cq_poll_ibv_cq;

	*cq_fid = &cq->efa_cq.util_cq.cq_fid;
	(*cq_fid)->fid.ops = &efa_rdm_cq_fi_ops;
	(*cq_fid)->ops = &efa_rdm_cq_ops;

	/* open shm cq as peer cq */
	if (efa_domain->shm_domain) {
		memcpy(&shm_cq_attr, attr, sizeof(*attr));
		/* Bind ep with shm provider's cq */
		shm_cq_attr.flags |= FI_PEER;
		peer_cq_context.size = sizeof(peer_cq_context);
		peer_cq_context.cq = cq->efa_cq.util_cq.peer_cq;
		ret = fi_cq_open(efa_domain->shm_domain, &shm_cq_attr,
				 &cq->shm_cq, &peer_cq_context);
		if (ret) {
			EFA_WARN(FI_LOG_CQ, "Unable to open shm cq: %s\n", fi_strerror(-ret));
			goto destroy_ibv_cq;
		}
	}

	return 0;
destroy_ibv_cq:
	retv = -ibv_destroy_cq(ibv_cq_ex_to_cq(cq->efa_cq.ibv_cq.ibv_cq_ex));
	if (retv)
		EFA_WARN(FI_LOG_CQ, "Unable to close ibv cq: %s\n",
			 fi_strerror(-retv));
close_util_cq:
	retv = ofi_cq_cleanup(&cq->efa_cq.util_cq);
	if (retv)
		EFA_WARN(FI_LOG_CQ, "Unable to close util cq: %s\n",
			 fi_strerror(-retv));
free:
	free(cq);
	return ret;
}
