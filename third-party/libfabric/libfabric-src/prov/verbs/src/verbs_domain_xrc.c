/*
 * Copyright (c) 2018 Cray Inc. All rights reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * BSD license below:
 *
 *     Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *      - Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "config.h"
#include "fi_verbs.h"
#include <sys/stat.h>


/* Domain XRC INI QP RBTree key */
struct vrb_ini_conn_key {
	struct sockaddr		*addr;
	struct vrb_cq	*tx_cq;
};

static int vrb_process_ini_conn(struct vrb_xrc_ep *ep,int reciprocal,
				   void *param, size_t paramlen);

/*
 * This routine is a work around that creates a QP for the only purpose of
 * reserving the QP number. The QP is not transitioned out of the RESET state.
 */
int vrb_reserve_qpn(struct vrb_xrc_ep *ep, struct ibv_qp **qp)
{
	struct vrb_domain *domain = vrb_ep_to_domain(&ep->base_ep);
	struct vrb_cq *cq = container_of(ep->base_ep.util_ep.tx_cq,
					    struct vrb_cq, util_cq);
	struct ibv_qp_init_attr attr = { 0 };
	int ret;

	/* Limit library allocated resources and do not INIT QP */
	attr.cap.max_send_wr = 1;
	attr.cap.max_send_sge = 1;
	attr.cap.max_recv_wr = 0;
	attr.cap.max_recv_sge = 0;
	attr.cap.max_inline_data = 0;
	attr.send_cq = cq->cq;
	attr.recv_cq = cq->cq;
	attr.qp_type = IBV_QPT_RC;

	*qp = ibv_create_qp(domain->pd, &attr);
	if (OFI_UNLIKELY(!*qp)) {
		ret = -errno;
		VERBS_WARN(FI_LOG_EP_CTRL,
			   "Reservation QP create failed %d\n", -ret);
		return ret;
	}
	return FI_SUCCESS;
}

static int vrb_create_ini_qp(struct vrb_xrc_ep *ep)
{
#if VERBS_HAVE_XRC
	struct ibv_qp_init_attr_ex attr_ex;
	struct vrb_domain *domain = vrb_ep_to_domain(&ep->base_ep);
	int ret;

	vrb_msg_ep_get_qp_attr(&ep->base_ep,
			(struct ibv_qp_init_attr *)&attr_ex);
	attr_ex.qp_type = IBV_QPT_XRC_SEND;
	attr_ex.comp_mask = IBV_QP_INIT_ATTR_PD;
	attr_ex.pd = domain->pd;
	attr_ex.qp_context = domain;

	ret = rdma_create_qp_ex(ep->base_ep.id, &attr_ex);
	if (ret) {
		ret = -errno;
		VERBS_WARN(FI_LOG_EP_CTRL,
			   "XRC INI QP rdma_create_qp_ex failed %d\n", -ret);
		return ret;
	}
	return FI_SUCCESS;
#else /* VERBS_HAVE_XRC */
	return -FI_ENOSYS;
#endif /* !VERBS_HAVE_XRC */
}

static inline void vrb_set_ini_conn_key(struct vrb_xrc_ep *ep,
					   struct vrb_ini_conn_key *key)
{
	key->addr = ep->base_ep.info->dest_addr;
	key->tx_cq = container_of(ep->base_ep.util_ep.tx_cq,
				  struct vrb_cq, util_cq);
}

/* Caller must hold domain:eq:lock */
int vrb_get_shared_ini_conn(struct vrb_xrc_ep *ep,
			       struct vrb_ini_shared_conn **ini_conn) {
	struct vrb_domain *domain = vrb_ep_to_domain(&ep->base_ep);
	struct vrb_ini_conn_key key;
	struct vrb_ini_shared_conn *conn;
	struct ofi_rbnode *node;
	int ret;

	vrb_set_ini_conn_key(ep, &key);
	node = ofi_rbmap_find(domain->xrc.ini_conn_rbmap, &key);
	if (node) {
		*ini_conn = node->data;
		ofi_atomic_inc32(&(*ini_conn)->ref_cnt);
		return FI_SUCCESS;
	}

	*ini_conn = NULL;
	conn = calloc(1, sizeof(*conn));
	if (!conn) {
		VERBS_WARN(FI_LOG_EP_CTRL,
			   "Unable to allocate INI connection memory\n");
		return -FI_ENOMEM;
	}

	conn->tgt_qpn = VRB_NO_INI_TGT_QPNUM;
	conn->peer_addr = mem_dup(key.addr, ofi_sizeofaddr(key.addr));
	if (!conn->peer_addr) {
		VERBS_WARN(FI_LOG_EP_CTRL,
			   "mem_dup of peer address failed\n");
		free(conn);
		return -FI_ENOMEM;
	}
	conn->tx_cq = container_of(ep->base_ep.util_ep.tx_cq,
				   struct vrb_cq, util_cq);
	dlist_init(&conn->pending_list);
	dlist_init(&conn->active_list);
	ofi_atomic_initialize32(&conn->ref_cnt, 1);

	ret = ofi_rbmap_insert(domain->xrc.ini_conn_rbmap,
			       (void *) &key, (void *) conn, NULL);
	assert(ret != -FI_EALREADY);
	if (ret) {
		VERBS_WARN(FI_LOG_EP_CTRL, "INI QP RBTree insert failed %d\n",
			   ret);
		goto insert_err;
	}
	*ini_conn = conn;
	return FI_SUCCESS;

insert_err:
	free(conn->peer_addr);
	free(conn);
	return ret;
}

/* Caller must hold domain:eq:lock */
void vrb_put_shared_ini_conn(struct vrb_xrc_ep *ep)
{
	struct vrb_domain *domain = vrb_ep_to_domain(&ep->base_ep);
	struct vrb_ini_shared_conn *ini_conn;
	struct vrb_ini_conn_key key;

	if (!ep->ini_conn)
		return;

	/* remove from pending or active connection list */
	dlist_remove(&ep->ini_conn_entry);
	ep->conn_state = VRB_XRC_UNCONNECTED;
	ini_conn = ep->ini_conn;
	ep->ini_conn = NULL;
	ep->base_ep.ibv_qp = NULL;
	if (ep->base_ep.id)
		ep->base_ep.id->qp = NULL;

	/* If XRC physical QP connection was not completed, make sure
	 * any pending connection to that destination will get scheduled. */
	if (ep->base_ep.id && ep->base_ep.id == ini_conn->phys_conn_id) {
		if (ini_conn->state == VRB_INI_QP_CONNECTING)
			ini_conn->state = VRB_INI_QP_UNCONNECTED;

		ini_conn->phys_conn_id = NULL;
	}

	/* Tear down physical INI/TGT when no longer being used */
	if (!ofi_atomic_dec32(&ini_conn->ref_cnt)) {
		if (ini_conn->ini_qp && ibv_destroy_qp(ini_conn->ini_qp))
			VERBS_WARN(FI_LOG_EP_CTRL,
				   "Destroy of XRC physical INI QP failed %d\n",
				   errno);

		assert(dlist_empty(&ini_conn->pending_list));
		vrb_set_ini_conn_key(ep, &key);
		ofi_rbmap_find_delete(domain->xrc.ini_conn_rbmap, &key);
		free(ini_conn->peer_addr);
		free(ini_conn);
	} else {
		vrb_sched_ini_conn(ini_conn);
	}
}

/* Caller must hold domain:eq:lock */
void vrb_add_pending_ini_conn(struct vrb_xrc_ep *ep, int reciprocal,
				 void *conn_param, size_t conn_paramlen)
{
	ep->conn_setup->pending_recip = reciprocal;
	ep->conn_setup->pending_paramlen = MIN(conn_paramlen,
				sizeof(ep->conn_setup->pending_param));
	memcpy(ep->conn_setup->pending_param, conn_param,
	       ep->conn_setup->pending_paramlen);
	dlist_insert_tail(&ep->ini_conn_entry, &ep->ini_conn->pending_list);
}

/* Caller must hold domain:eq:lock */
static void vrb_create_shutdown_event(struct vrb_xrc_ep *ep)
{
	struct fi_eq_cm_entry entry = {
		.fid = &ep->base_ep.util_ep.ep_fid.fid,
	};
	struct vrb_eq_entry *eq_entry;

	eq_entry = vrb_eq_alloc_entry(FI_SHUTDOWN, &entry, sizeof(entry));
	if (eq_entry)
		dlistfd_insert_tail(&eq_entry->item, &ep->base_ep.eq->list_head);
}

/* Caller must hold domain:eq:lock */
void vrb_sched_ini_conn(struct vrb_ini_shared_conn *ini_conn)
{
	struct vrb_xrc_ep *ep;
	enum vrb_ini_qp_state last_state;
	struct sockaddr *addr;
	int ret;

	/* Continue to schedule shared connections if the physical connection
	 * has completed and there are connection requests pending. We could
	 * implement a throttle here if it is determined that it is better to
	 * limit the number of outstanding connections. */
	while (1) {
		if (dlist_empty(&ini_conn->pending_list) ||
				ini_conn->state == VRB_INI_QP_CONNECTING)
			return;

		dlist_pop_front(&ini_conn->pending_list,
				struct vrb_xrc_ep, ep, ini_conn_entry);

		dlist_insert_tail(&ep->ini_conn_entry,
				  &ep->ini_conn->active_list);
		last_state = ep->ini_conn->state;

		ret = vrb_create_ep(ep->base_ep.info,
				       last_state == VRB_INI_QP_UNCONNECTED ?
				       RDMA_PS_TCP : RDMA_PS_UDP,
				       &ep->base_ep.id);
		if (ret) {
			VERBS_WARN(FI_LOG_EP_CTRL,
				   "Failed to create active CM ID %d\n",
				   ret);
			goto err;
		}

		if (last_state == VRB_INI_QP_UNCONNECTED) {
			assert(!ep->ini_conn->phys_conn_id && ep->base_ep.id);

			if (ep->ini_conn->ini_qp &&
			    ibv_destroy_qp(ep->ini_conn->ini_qp)) {
				VERBS_WARN(FI_LOG_EP_CTRL, "Failed to destroy "
					   "physical INI QP %d\n", errno);
			}
			ret = vrb_create_ini_qp(ep);
			if (ret) {
				VERBS_WARN(FI_LOG_EP_CTRL, "Failed to create "
					   "physical INI QP %d\n", ret);
				goto err;
			}
			ep->ini_conn->ini_qp = ep->base_ep.id->qp;
			ep->ini_conn->state = VRB_INI_QP_CONNECTING;
			ep->ini_conn->phys_conn_id = ep->base_ep.id;
		} else {
			assert(!ep->base_ep.id->qp);
			VERBS_DBG(FI_LOG_EP_CTRL, "Sharing XRC INI QPN %d\n",
				  ep->ini_conn->ini_qp->qp_num);
		}

		assert(ep->ini_conn->ini_qp);
		ep->base_ep.id->context = &ep->base_ep.util_ep.ep_fid.fid;
		ret = rdma_migrate_id(ep->base_ep.id,
				      ep->base_ep.eq->channel);
		if (ret) {
			VERBS_WARN(FI_LOG_EP_CTRL,
				   "Failed to migrate active CM ID %d\n", ret);
			goto err;
		}

		addr = rdma_get_local_addr(ep->base_ep.id);
		if (addr)
			ofi_straddr_dbg(&vrb_prov, FI_LOG_EP_CTRL,
					"XRC connect src_addr", addr);
		addr = rdma_get_peer_addr(ep->base_ep.id);
		if (addr)
			ofi_straddr_dbg(&vrb_prov, FI_LOG_EP_CTRL,
					"XRC connect dest_addr", addr);

		ep->base_ep.ibv_qp = ep->ini_conn->ini_qp;
		ret = vrb_process_ini_conn(ep, ep->conn_setup->pending_recip,
					      ep->conn_setup->pending_param,
					      ep->conn_setup->pending_paramlen);
err:
		if (ret) {
			ep->ini_conn->state = last_state;
			vrb_put_shared_ini_conn(ep);

			/* We need to let the application know that the
			 * connect request has failed. */
			vrb_create_shutdown_event(ep);
			break;
		}
	}
}

/* Caller must hold domain:xrc:eq:lock */
int vrb_process_ini_conn(struct vrb_xrc_ep *ep,int reciprocal,
			    void *param, size_t paramlen)
{
	struct vrb_xrc_cm_data *cm_data = param;
	int ret;

	assert(ep->base_ep.ibv_qp);

	vrb_set_xrc_cm_data(cm_data, reciprocal, reciprocal ?
			       ep->conn_setup->remote_conn_tag :
			       ep->conn_setup->conn_tag,
			       ep->base_ep.eq->xrc.pep_port,
			       ep->ini_conn->tgt_qpn, ep->srqn);

	ep->base_ep.conn_param.private_data = cm_data;
	ep->base_ep.conn_param.private_data_len = paramlen;
	ep->base_ep.conn_param.responder_resources = RDMA_MAX_RESP_RES;
	ep->base_ep.conn_param.initiator_depth = RDMA_MAX_INIT_DEPTH;
	ep->base_ep.conn_param.flow_control = 1;
	ep->base_ep.conn_param.retry_count = 15;
	ep->base_ep.conn_param.rnr_retry_count = 7;
	ep->base_ep.conn_param.srq = 1;

	if (!ep->base_ep.id->qp)
		ep->base_ep.conn_param.qp_num =
				ep->ini_conn->ini_qp->qp_num;

	assert(ep->conn_state == VRB_XRC_UNCONNECTED ||
	       ep->conn_state == VRB_XRC_ORIG_CONNECTED);
	vrb_next_xrc_conn_state(ep);

	ret = rdma_resolve_route(ep->base_ep.id, VERBS_RESOLVE_TIMEOUT);
	if (ret) {
		ret = -errno;
		VERBS_WARN(FI_LOG_EP_CTRL,
			   "rdma_resolve_route failed %s (%d)\n",
			   strerror(-ret), -ret);
		vrb_prev_xrc_conn_state(ep);
	}

	return ret;
}

int vrb_ep_create_tgt_qp(struct vrb_xrc_ep *ep, uint32_t tgt_qpn)
{
#if VERBS_HAVE_XRC
	struct ibv_qp_open_attr open_attr;
	struct ibv_qp_init_attr_ex attr_ex;
	struct vrb_domain *domain = vrb_ep_to_domain(&ep->base_ep);
	int ret;

	assert(ep->tgt_id && !ep->tgt_id->qp);

	/* If a target QP number was specified then open that existing
	 * QP for sharing. */
	if (tgt_qpn) {
		memset(&open_attr, 0, sizeof(open_attr));
		open_attr.qp_num = tgt_qpn;
		open_attr.comp_mask = IBV_QP_OPEN_ATTR_NUM |
			IBV_QP_OPEN_ATTR_XRCD | IBV_QP_OPEN_ATTR_TYPE |
			IBV_QP_OPEN_ATTR_CONTEXT;
		open_attr.xrcd = domain->xrc.xrcd;
		open_attr.qp_type = IBV_QPT_XRC_RECV;
		open_attr.qp_context = ep;

		ep->tgt_ibv_qp = ibv_open_qp(domain->verbs, &open_attr);
		if (!ep->tgt_ibv_qp) {
			ret = -errno;
			VERBS_WARN(FI_LOG_EP_CTRL,
				   "XRC TGT QP ibv_open_qp failed %d\n", -ret);
			return ret;
		}
		return FI_SUCCESS;
	}

	/* An existing XRC target was not specified, create XRC TGT
	 * side of new physical connection. */
	vrb_msg_ep_get_qp_attr(&ep->base_ep,
			(struct ibv_qp_init_attr *)&attr_ex);
	attr_ex.qp_type = IBV_QPT_XRC_RECV;
	attr_ex.qp_context = ep;
	attr_ex.comp_mask = IBV_QP_INIT_ATTR_PD | IBV_QP_INIT_ATTR_XRCD;
	attr_ex.pd = domain->pd;
	attr_ex.xrcd = domain->xrc.xrcd;
	if (rdma_create_qp_ex(ep->tgt_id, &attr_ex)) {
		ret = -errno;
		VERBS_WARN(FI_LOG_EP_CTRL,
			   "Physical XRC TGT QP rdma_create_qp_ex failed %d\n",
			   -ret);
		return ret;
	}
	ep->tgt_ibv_qp = ep->tgt_id->qp;

	return FI_SUCCESS;
#else /* VERBS_HAVE_XRC */
	return -FI_ENOSYS;
#endif /* !VERBS_HAVE_XRC */
}

static int vrb_put_tgt_qp(struct vrb_xrc_ep *ep)
{
	int ret;

	if (!ep->tgt_ibv_qp)
		return FI_SUCCESS;

	/* The kernel will not destroy the detached TGT QP until all
	 * shared opens have called ibv_destroy_qp. */
	ret = ibv_destroy_qp(ep->tgt_ibv_qp);
	if (ret) {
		ret = -errno;
		VERBS_WARN(FI_LOG_EP_CTRL,
			   "Close XRC TGT QP ibv_destroy_qp failed %d\n",
			   -ret);
		return ret;
	}
	ep->tgt_ibv_qp = NULL;
	if (ep->tgt_id)
		ep->tgt_id->qp = NULL;

	return FI_SUCCESS;
}

/* Caller must hold eq:lock */
int vrb_ep_destroy_xrc_qp(struct vrb_xrc_ep *ep)
{
	vrb_put_shared_ini_conn(ep);

	if (ep->base_ep.id) {
		rdma_destroy_id(ep->base_ep.id);
		ep->base_ep.id = NULL;
	}
	if (ep->tgt_ibv_qp)
		vrb_put_tgt_qp(ep);

	if (ep->tgt_id) {
		rdma_destroy_id(ep->tgt_id);
		ep->tgt_id = NULL;
	}
	return 0;
}

FI_VERBS_XRC_ONLY
static int vrb_ini_conn_compare(struct ofi_rbmap *map, void *key, void *data)
{
	struct vrb_ini_shared_conn *ini_conn = data;
	struct vrb_ini_conn_key *_key = key;
	int ret;

	assert(_key->addr->sa_family == ini_conn->peer_addr->sa_family);

	/* Only interested in the interface address and TX CQ */
	switch (_key->addr->sa_family) {
	case AF_INET:
		ret = memcmp(&ofi_sin_addr(_key->addr),
			     &ofi_sin_addr(ini_conn->peer_addr),
			     sizeof(ofi_sin_addr(_key->addr)));
		break;
	case AF_INET6:
		ret = memcmp(&ofi_sin6_addr(_key->addr),
			     &ofi_sin6_addr(ini_conn->peer_addr),
			     sizeof(ofi_sin6_addr(_key->addr)));
		break;
	default:
		VERBS_WARN(FI_LOG_FABRIC, "Unsupported address format\n");
		assert(0);
		return -FI_EINVAL;
	}
	if (ret)
		return ret;

	return _key->tx_cq < ini_conn->tx_cq ?
			-1 : _key->tx_cq > ini_conn->tx_cq;
}

FI_VERBS_XRC_ONLY
static int vrb_domain_xrc_validate_hw(struct vrb_domain *domain)
{
	struct ibv_device_attr attr;
	int ret;

	ret = ibv_query_device(domain->verbs, &attr);
	if (ret || !(attr.device_cap_flags & IBV_DEVICE_XRC)) {
		VERBS_INFO(FI_LOG_DOMAIN, "XRC is not supported");
		return -FI_EINVAL;
	}
	return FI_SUCCESS;
}

int vrb_domain_xrc_init(struct vrb_domain *domain)
{
#if VERBS_HAVE_XRC
	struct ibv_xrcd_init_attr attr;
	int ret;

	ret = vrb_domain_xrc_validate_hw(domain);
	if (ret)
		return ret;

	domain->xrc.xrcd_fd = -1;
	if (vrb_gl_data.msg.xrcd_filename) {
		domain->xrc.xrcd_fd = open(vrb_gl_data.msg.xrcd_filename,
				       O_CREAT, S_IWUSR | S_IRUSR);
		if (domain->xrc.xrcd_fd < 0) {
			VERBS_WARN(FI_LOG_DOMAIN,
				   "XRCD file open failed %d\n", errno);
			return -errno;
		}
	}

	attr.comp_mask = IBV_XRCD_INIT_ATTR_FD | IBV_XRCD_INIT_ATTR_OFLAGS;
	attr.fd = domain->xrc.xrcd_fd;
	attr.oflags = O_CREAT;
	domain->xrc.xrcd = ibv_open_xrcd(domain->verbs, &attr);
	if (!domain->xrc.xrcd) {
		ret = -errno;
		VERBS_INFO_ERRNO(FI_LOG_DOMAIN, "ibv_open_xrcd", errno);
		goto xrcd_err;
	}

	domain->xrc.ini_conn_rbmap = ofi_rbmap_create(vrb_ini_conn_compare);
	if (!domain->xrc.ini_conn_rbmap) {
		ret = -ENOMEM;
		VERBS_INFO_ERRNO(FI_LOG_DOMAIN, "XRC INI QP RB Tree", -ret);
		goto rbmap_err;
	}

	domain->flags |= VRB_USE_XRC;
	return FI_SUCCESS;

rbmap_err:
	(void)ibv_close_xrcd(domain->xrc.xrcd);
xrcd_err:
	if (domain->xrc.xrcd_fd >= 0) {
		close(domain->xrc.xrcd_fd);
		domain->xrc.xrcd_fd = -1;
	}
	return ret;
#else /* VERBS_HAVE_XRC */
	return -FI_ENOSYS;
#endif /* !VERBS_HAVE_XRC */
}

int vrb_domain_xrc_cleanup(struct vrb_domain *domain)
{
#if VERBS_HAVE_XRC
	int ret;

	assert(domain->xrc.xrcd);

	/* All endpoint and hence XRC INI QP should be closed */
	if (!ofi_rbmap_empty(domain->xrc.ini_conn_rbmap)) {
		VERBS_WARN(FI_LOG_DOMAIN, "XRC domain busy\n");
		return -FI_EBUSY;
	}

	ret = ibv_close_xrcd(domain->xrc.xrcd);
	if (ret) {
		VERBS_WARN(FI_LOG_DOMAIN, "ibv_close_xrcd failed %d\n", ret);
		return -ret;
	}
	if (domain->xrc.xrcd_fd >= 0) {
		close(domain->xrc.xrcd_fd);
		domain->xrc.xrcd_fd = -1;
	}

	ofi_rbmap_destroy(domain->xrc.ini_conn_rbmap);
#endif /* VERBS_HAVE_XRC */
	return 0;
}
