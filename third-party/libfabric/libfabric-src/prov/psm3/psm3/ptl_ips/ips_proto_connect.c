/*

  This file is provided under a dual BSD/GPLv2 license.  When using or
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2015 Intel Corporation.

  This program is free software; you can redistribute it and/or modify
  it under the terms of version 2 of the GNU General Public License as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  Contact Information:
  Intel Corporation, www.intel.com

  BSD LICENSE

  Copyright(c) 2015 Intel Corporation.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in
      the documentation and/or other materials provided with the
      distribution.
    * Neither the name of Intel Corporation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

/* Copyright (c) 2003-2014 Intel Corporation. All rights reserved. */

#include "psm_user.h"
#include "psm2_hal.h"
#include "ips_proto.h"
#include "psm_mq_internal.h"
#include "ips_proto_internal.h"
#ifdef RNDV_MOD
#include "psm_rndv_mod.h"
#endif

/*
 * define connection version. this is the basic version, optimized
 * version will be added later for scalability.
 * version kept in 2 nibbles in this format: 0xMMmm MM=major, mm=minor version
 */
// a litle paranod as a UD or UDP connect can't reach a STL100 PSM recv context
// but we don't worry about UDP vs UD since can't reach eachother either
#define IPS_CONNECT_VERNO	  0x0100 // 1.0

struct ips_connect_hdr {
	uint16_t connect_verno;	/* should be ver IPS_CONNECT_VERNO */
	uint16_t psm_verno;	/* should be 2.0 */
	uint32_t connidx;	/* ignore if 0xffffffff */
	uint64_t epid;		/* epid of connector process */
} PACK_SUFFIX;

struct ips_connect_reqrep {
	uint16_t connect_verno;	/* should be ver IPS_CONNECT_VERNO */
	uint16_t psm_verno;	/* should be 2.0 */
	uint32_t connidx;	/* ignore if 0xffffffff */
	uint64_t epid;		/* epid of connector process */
	/* above should be same as ips_connect_hdr */

	// fields below specific to CONNECT_REQUEST/REPLY
	uint16_t connect_result;	/* error code */
	uint16_t sl;		/* service level for matching */
	uint16_t mtu;		/* receive payload */
	uint16_t job_pkey;	/* partition key for verification */

	uint32_t runid_key;	/* one-time stamp connect key */
	uint32_t initpsn;	/* initial psn for flow */

	char hostname[128];	/* sender's hostname string */
	// fields below added as part of IPS_CONNECT_VERNO 1.0
	// used for rndv and user space RC QP connection (CONNECT_REQUEST/REPLY)
	uint8_t rdmamode;	/* IPS_PROTOEXP_FLAG_RDMA_MASK portion of rdmamode */
	uint8_t static_rate;	/* ibv_rate enum */
	uint8_t reserved[6+16];	// 1st 6 bytes keep fields below 64b aligned
	// fields below can be zero depending on rdmamode

	// for rndv module connection establishment only set for RNDV_MOD
	union ibv_gid gid; /* sender's gid */	// zero if no rndv mod RDMA
	uint32_t rv_index; /* senders process index */ // zero if no rndv mod RDMA
	uint32_t resv;	// alignment
	// for user space RC QP connection establishment only set for USE_RC
	struct psm_rc_qp_attr qp_attr;	// zero if no user space RC QPs
	// 8 bytes of subnet and 8 bytes of epid may follow for each of up to
	// PSMI_MAX_QPS in case this is a multi-rail run and/or mutliple QPs 
	// are opened per NIC.
} PACK_SUFFIX;

/* Startup protocol in PSM/IPS
 *
 * Start timer.
 *
 * For all nodes to connect to:
 *   Grab connect lock
 *   Look up epid in table
 *      MATCH.
 *         assert cstate_outgoing != CONNECT_WAITING (no re-entrancy)
 *         If cstate_outgoing == CONNECT_DONE
 *            return the already connected address.
 *         else
 *            assert cstate_outgoing == CONNECT_NONE
 *            assert cstate_incoming == CONNECT_DONE
 *            cstate_outgoing := CONNECT_WAITING
 *            assert connidx_outgoing != UNKNOWN && connidx_incoming != UNKNOWN
 *            req->connidx := epaddr->connidx_incoming
 *            add to list of pending connect.
 *      NO MATCH
 *         allocate epaddr and put in table
 *         cstate_outgoing := CONNECT_WAITING
 *         cstate_incoming := CONNECT_NONE
 *         connidx_outgoing := UNKNOWN
 *         req->connidx := epaddr->connidx_incoming := NEW connidx integer
 *         add to list of pending connect
 *   Release connect lock
 *
 * expected_connect_count = ep->total_connect_count + num_to_connect
 * while (expected_connect_count != ep->total_connect_count)
 *    check for timeout
 *    progress();
 *
 * For all connection requests received (within progress loop)
 *   If uuid doesn't match, NAK the connect and skip request
 *   Grab connect lock
 *   Lock up epid in table
 *      MATCH
 *	   if cstate_incoming == CONNECT_DONE
 *	      req->connidx := epaddr->connidx_incoming
 *            compose reply and send again (this is a dupe request).
 *         else
 *            assert cstate_incoming == CONNECT_NONE
 *            assert cstate_outgoing == (CONNECT_WAITING | CONNECT_DONE)
 *            cstate_incoming := CONNECT_DONE
 *            epaddr->connidx_outgoing := req->connidx
 *            req->connidx := epaddr->connidx_incoming
 *      NO MATCH
 *         allocate epaddr and put in table
 *         cstate_incoming := CONNECT_DONE
 *         epaddr->connidx_outgoing = req->connidx;
 *         rep->connidx := epaddr->connidx_incoming := NEW connidx integer
 *         compose connect reply and send
 *   Release connect lock
 *
 * For all connection replies received:
 *    If connect_result != 0, process error and skip.
 *    assert cstate_outgoing == CONNECT_WAITING
 *    if cstate_incoming == CONNECT_DONE
 *       assert rep->connidx == epaddr->connidx_outgoing
 *    else
 *	 epaddr->connidx_outgoing := rep->connidx
 *    cstate_outgoing := CONNECT_DONE
 *    ep->total_connect_count ++
 *
 *   * Fill in a connection request:
 *      1. Set connect protocol version and PSM versions
 *      2. Set the uuid attached to current endpoint and add the job_pkey
 *         the node wishes to communicate post-connect.
 *      3. Set our mtu, bitwidth and endianess to detect inconsistencies
 *
 */

static int
ips_proto_build_connect_message(struct ips_proto *proto,
	ips_epaddr_t *ipsaddr, uint8_t opcode, void *payload,
	size_t max_paylen);

#ifdef RNDV_MOD
/* on -1 errno is status
 * EIO is connection error other values are more serious (invalid call, etc)
 */
static int is_rv_connected(ips_epaddr_t *ipsaddr)
{
	int ret;

	/* ! rv_conn means we don't need a rv connection, otherwise
	 * return status of the connection
	 */
	if (! ipsaddr->rv_conn || ipsaddr->rv_connected)
		return 1;
	ret = __psm2_rv_connected(ipsaddr->rv_conn);
	if (ret < 0 && errno != EIO) {
		int save_errno = errno;
		perror("can't query rv connection\n");
		errno = save_errno;
	}
	ipsaddr->rv_connected = (1 == ret);
	return ret;
}
#else // RNDV_MOD
static inline int is_rv_connected(ips_epaddr_t *ipsaddr) { return 1; }
#endif // RNDV_MOD
	
/**
 * Configure flows for an ipsaddr.
 *
 * @arg ipsaddr - the ipsaddr to configure the flows for
 * @arg proto - the protocol used
 *
 * @pre proto's flags must be set
 *
 * Flows should be configured:
 * - immediately upon creation of an ipsaddr
 * - whenever a connection is established and the receiver's characteristics
 *   (e.g. mtu) become known
 */
ustatic
void
ips_ipsaddr_configure_flows(struct ips_epaddr *ipsaddr, struct ips_proto *proto)
{
	/* PIO flow uses the normal priority path, to separate low
	 * priority path for bulk sdma data packets
	 */
	ips_flow_init(&ipsaddr->flows[EP_FLOW_GO_BACK_N_PIO], proto,
		      ipsaddr, PSM_TRANSFER_PIO, PSM_PROTOCOL_GO_BACK_N,
		      IPS_PATH_NORMAL_PRIORITY, EP_FLOW_GO_BACK_N_PIO);

}

/*
 * Teardown any unnecessary timers that could still be active and assign NULL
 * to pointers in flow structs. We do this mainly for PIO and DMA flows.
 * TidFlow teardowns are conducted in ips_protoexp_fini()
 */
static
void
ips_flow_fini(struct ips_epaddr *ipsaddr, struct ips_proto *proto)
{
	struct ips_flow *flow;
	int i;

	for (i = 0; i < EP_FLOW_LAST-1; i++) {
		flow = &ipsaddr->flows[i];

		/* Cancel any stale flow->timers in flight */
		if (flow->timer_ack) {
			psmi_timer_cancel(proto->timerq, flow->timer_ack);
			flow->timer_ack = NULL;
		}

		if (flow->timer_send) {
			psmi_timer_cancel(proto->timerq, flow->timer_send);
			flow->timer_send = NULL;
		}

		flow->flush = NULL;
		flow->path = NULL;
		flow->ipsaddr = NULL;
	}
}

static
psm2_epaddr_t
ips_alloc_epaddr(struct ips_proto *proto, int master, psm2_epid_t epid,
		 const char *hostname,
		 unsigned long timeout, psm2_error_t *err_out);

/*
 * Given a connection request, set mtu, communication index and hdr length
 * parameters.
 *
 * The most subtle parameter is the mtu.  When set as 'req->mtu', the mtu
 * is our connecting peer's declared mtu (which may not be the same as our
 * mtu).  The approach is to take the smaller of both mtus when communicating
 * with that peer.  Also, when using pio, the size can be further restricted by
 * the pio send buffer sizes (i.e. 4K IB MTU but only 2K PIO buffers).
 */
static
psm2_error_t
ips_ipsaddr_set_req_params(struct ips_proto *proto,
			   ips_epaddr_t *ipsaddr,
			   const struct ips_connect_reqrep *req,
			   uint32_t paylen)
{
	psm2_ep_t ep;
	psm2_epaddr_t epaddr;
	psm2_error_t err = PSM2_OK;
	int i, start, count;
	uint64_t *data;
	psmi_assert_always(req->mtu > 0);
	// common_mtu will be further reduced by pr_mtu to set frag_size and RC mtu
	uint16_t common_mtu = min(req->mtu, proto->epinfo.ep_mtu);
	psmi_assert_always(req->static_rate > 0);
	enum psm_ibv_rate common_rate = min_rate(req->static_rate,
										 proto->epinfo.ep_link_rate);
	int ptype, pidx;

	ipsaddr->window_rv = proto->mq->hfi_base_window_rv;

	/*
	 * For static routes i.e. "none" path resolution update all paths to
	 * have the same profile (mtu, sl etc.).
	 *
	 * For path record queries the epr_mtu and epr_sl are setup correctly
	 * from the path itself.
	 */
	for (ptype = IPS_PATH_LOW_PRIORITY;
	     ptype < IPS_PATH_MAX_PRIORITY; ptype++)
		for (pidx = 0;
		     pidx < ipsaddr->pathgrp->pg_num_paths[ptype]; pidx++) {
			if (proto->ep->path_res_type == PSM2_PATH_RES_NONE) {
				ipsaddr->pathgrp->pg_path[pidx][ptype]->pr_mtu =
					common_mtu;
				ipsaddr->pathgrp->pg_path[pidx][ptype]->pr_static_rate =
					common_rate;
			} else {
				ipsaddr->pathgrp->pg_path[pidx][ptype]->pr_mtu =
				    min(common_mtu,
					ipsaddr->pathgrp->pg_path[pidx][ptype]->pr_mtu);
				ipsaddr->pathgrp->pg_path[pidx][ptype]->pr_static_rate =
				    min_rate(common_rate,
					ipsaddr->pathgrp->pg_path[pidx][ptype]->pr_static_rate);
			}
		}

	/*
	 * We've got updated mtu/path records, need to re-initialize the flows to take
	 * into account _real_ (updated) remote endpoint characteristics
	 */
	ips_ipsaddr_configure_flows(ipsaddr, proto);

	/*
	 * Save peer's info.
	 */
	ipsaddr->connidx_outgoing = req->connidx;
	ipsaddr->runid_key = req->runid_key;
	/* ipsaddr->initpsn = req->initpsn; */

	err =
	    psmi_epid_set_hostname(psm2_epid_nid(((psm2_epaddr_t) ipsaddr)->epid),
				   (char *)req->hostname, 0);
	if (err)
		return err;

#ifdef RNDV_MOD
	ipsaddr->remote_gid = req->gid;
	ipsaddr->remote_rv_index = req->rv_index;
	if (ipsaddr->rv_conn) {
		psmi_assert(IPS_PROTOEXP_FLAG_KERNEL_QP(proto->ep->rdmamode));
		psmi_assert(proto->ep->verbs_ep.rv);
		if (!  __psm2_nonzero_gid(&req->gid)) {
			_HFI_ERROR("mismatched PSM3_RDMA config, remote end not in mode 1\n");
			return PSM2_INTERNAL_ERR;
			// TBD - if we wanted to allow mismatched config to run in UD mode
			//__psm2_rv_destroy_conn(ipsaddr->rv_conn);
			//ipsaddr->rv_conn = NULL;
		} else {
			// both sides are ready, so we can start rv_connect now
			if (! ipsaddr->pathgrp->pg_path[0][IPS_PATH_LOW_PRIORITY]->connecting) {
				char buf[80];
				struct ib_user_path_rec path;
				_HFI_MMDBG("rv_connect to: %s\n", __psm2_dump_gid(&ipsaddr->remote_gid, buf, sizeof(buf)));
				// pg_path has negotiated pr_mtu and pr_static_rate
				ips_path_rec_to_ib_user_path_rec(proto->ep,
					ipsaddr->pathgrp->pg_path[0][IPS_PATH_LOW_PRIORITY],
 					&ipsaddr->remote_gid, &path);
				if (__psm2_rv_connect(ipsaddr->rv_conn, &path)) {
					_HFI_ERROR("rv_connect failed: %s\n", strerror(errno));
					return PSM2_INTERNAL_ERR;
				}
				ipsaddr->pathgrp->pg_path[0][IPS_PATH_LOW_PRIORITY]->connecting = 1;
			}
		}
	// } else if (__psm2_nonzero_gid(&req->gid)) {
	//	 We could fail here, but we just let remote end decide
	//	_HFI_ERROR("mismatched PSM3_RDMA config, remote end in mode 1\n");
	//	return PSM2_INTERNAL_ERR;
	}
#endif // RNDV_MOD
	if (ipsaddr->rc_qp) {
		psmi_assert(IPS_PROTOEXP_FLAG_USER_RC_QP(proto->ep->rdmamode));
#ifdef RNDV_MOD
		psmi_assert(proto->ep->verbs_ep.rv
					|| proto->ep->mr_cache_mode != MR_CACHE_MODE_KERNEL);
#endif
		if (! req->qp_attr.qpn) {
			_HFI_ERROR("mismatched PSM3_RDMA config, remote end not in mode 2 or 3\n");
			return PSM2_INTERNAL_ERR;
			// TBD - if we wanted to allow mismatched config to run in UD mode
			//rc_qp_destroy(ipsaddr->rc_qp);
			//ipsaddr->rc_qp = NULL;
		} else {
			// we got a REQ or a REP, we can move to RTR
			// if we are only doing RDMA, we don't need any buffers, but we need a
			// pool object for RQ coallesce, so we create a pool with 0 size buffers
			if (PSM2_OK != psm_verbs_alloc_recv_pool(proto->ep, ipsaddr->rc_qp, &ipsaddr->recv_pool,
					min(proto->ep->hfi_num_recv_wqes/VERBS_RECV_QP_FRACTION, ipsaddr->rc_qp_max_recv_wr),
				  (proto->ep->rdmamode == IPS_PROTOEXP_FLAG_RDMA_USER)? 0
					// want to end up with multiple of cache line (64)
					// pr_mtu is negotiated max PSM payload, not including hdrs
					// pr_mtu+MAX_PSM_HEADERS will be power of 2 verbs MTU
					// be conservative (+BUFFER_HEADROOM)
					: ipsaddr->pathgrp->pg_path[0][IPS_PATH_LOW_PRIORITY]->pr_mtu
							+ MAX_PSM_HEADER + BUFFER_HEADROOM
			)) {
				_HFI_ERROR("failed to alloc RC recv buffers\n");
				return PSM2_INTERNAL_ERR;
			}

			if (modify_rc_qp_to_init(proto->ep, ipsaddr->rc_qp)) {
				_HFI_ERROR("qp_to_init failed\n");
				return PSM2_INTERNAL_ERR;
			}
			if (PSM2_OK != __psm2_ep_verbs_prepost_recv(&ipsaddr->recv_pool)) {
				_HFI_ERROR("prepost failed\n");
				return PSM2_INTERNAL_ERR;
			}
			// RC QP MTU will be set to min of req->qp_attr and pr_mtu
			// TBD - we already factored in req vs pr to update pr no need
			// for modify_cq_qp_to_rtr to repeat it
			if (modify_rc_qp_to_rtr(proto->ep, ipsaddr->rc_qp, &req->qp_attr,
					ipsaddr->pathgrp->pg_path[0][IPS_PATH_LOW_PRIORITY], //TBD path_rec
					req->initpsn)) {
				_HFI_ERROR("qp_to_rtr failed\n");
				return PSM2_INTERNAL_ERR;
			}
		}
	// } else if (req->qp_attr.qpn) {
	//	 We could fail here, but we just let remote end decide
	//	_HFI_ERROR("mismatched PSM3_RDMA config, remote end in mode 2 or 3\n");
	//	return PSM2_INTERNAL_ERR;
	}

	/*
	 * Check if there is other rails to setup.
	 */
	paylen -= sizeof(struct ips_connect_reqrep);
	if (paylen == 0)
		return PSM2_OK;

	/*
	 * Yes, other rail's gid/epid is attached.
	 */
	if (paylen % (sizeof(uint64_t) + sizeof(psm2_epid_t))) {
		return PSM2_INTERNAL_ERR;
	}
	count = paylen / (sizeof(uint64_t) + sizeof(psm2_epid_t));
	if (count > PSMI_MAX_QPS)
		return PSM2_INTERNAL_ERR;

	/*
	 * Both side are ordered, so just search from small to big.
	 */
	start = 0;
	data = (uint64_t *) (req + 1);
	ep = proto->ep->mctxt_next;

	struct drand48_data drand48_data;
	srand48_r((long int)(ipsaddr->epaddr.epid + proto->ep->epid), &drand48_data);

	/* Loop over all slave endpoints */
	while (ep != ep->mctxt_master) {
		for (i = start; i < count; i++) {

			/* There is a gid match, create the epaddr */
			if (data[2 * i] == ep->gid_hi
			// allow_routers only applied to ethernet epids (V4)
			|| (psmi_allow_routers && PSMI_EPID_GET_EPID_VERSION(data[2*i+1]) == PSMI_EPID_V4)
			 ) {

				epaddr =
					ips_alloc_epaddr(&((struct ptl_ips *)(ep->ptl_ips.ptl))->proto, 0,
							 data[2 * i + 1], NULL,
							  5000, &err);
				if (epaddr == NULL)
					return err;

				/* link the ipsaddr */
				IPS_MCTXT_APPEND(ipsaddr,
						 (ips_epaddr_t *) epaddr);

				/* Setup message control info to the same struct */
				((ips_epaddr_t *) epaddr)->msgctl =
				    ipsaddr->msgctl;
				ipsaddr->msgctl->ipsaddr_count++;

				/* randomize the rail to start traffic */
				long int rnum;
				lrand48_r(&drand48_data, &rnum);
				if ((rnum % count) == i) {
					ipsaddr->msgctl->ipsaddr_next =
					    (ips_epaddr_t *) epaddr;
				}

				/* update the starting point,
				 * all previous ones are not valid anymore */
				start = i + 1;
				break;
			}
		}

		ep = ep->mctxt_next;
	}

	return PSM2_OK;
}

static psm2_error_t
ips_proto_send_ctrl_message_request(struct ips_proto *proto,
				    struct ips_flow *flow, uint8_t message_type,
				    uint16_t *msg_queue_mask, uint64_t timeout)
{
	psm2_error_t err = PSM2_OK;
	ips_scb_t ctrlscb;

	/* msg header plus gid+epid for all rails plus checksum */
	char payload[sizeof(struct ips_connect_reqrep) +
		16*PSMI_MAX_QPS + PSM_CRC_SIZE_IN_BYTES];
	uint32_t paylen;

	ctrlscb.scb_flags = 0;
	paylen = ips_proto_build_connect_message(proto,
		flow->ipsaddr, message_type, payload, sizeof(payload));
	psmi_assert_always(paylen <= sizeof(payload));

	do {
		err = ips_proto_send_ctrl_message(flow, message_type,
				msg_queue_mask, &ctrlscb, payload, paylen);
		if (err == PSM2_OK) {
			break;
		}
		if ((err = psmi_err_only(psmi_poll_internal(proto->ep, 1)))) {
			break;
		}
	} while (get_cycles() < timeout);

	return err;
}

static psm2_error_t
ips_proto_send_ctrl_message_reply(struct ips_proto *proto,
				    struct ips_flow *flow, uint8_t message_type,
				    uint16_t *msg_queue_mask)
{
	/* This will try up to 100 times until the message is sent. The code
	 * is persistent because dropping replies will lead to a lack of
	 * overall progress on the connection/disconnection. We do not want
	 * to poll from here, and we cannot afford a lengthy timeout, since
	 * this is called from the receive path.
	 */
	psm2_error_t err = PSM2_OK;
	int i;
	ips_scb_t ctrlscb;
	/* msg header plus gid+epid for all rails plus checksum */
	char payload[sizeof(struct ips_connect_reqrep) +
		16*PSMI_MAX_QPS + PSM_CRC_SIZE_IN_BYTES];
	uint32_t paylen;

	ctrlscb.scb_flags = 0;
	paylen = ips_proto_build_connect_message(proto,
		flow->ipsaddr, message_type, payload, sizeof(payload));
	psmi_assert_always(paylen <= sizeof(payload));

	for (i = 0; i < 100; i++) {
		err = ips_proto_send_ctrl_message(flow, message_type,
				msg_queue_mask, &ctrlscb, payload, paylen);
		if (err == PSM2_OK) {
			break;
		}
	}

	return err;
}

static int
ips_proto_build_connect_message(struct ips_proto *proto,
	ips_epaddr_t *ipsaddr, uint8_t opcode, void *payload,
	size_t max_paylen)
{
	struct ips_connect_hdr *hdr = (struct ips_connect_hdr *)payload;
	struct ips_connect_reqrep *req = (struct ips_connect_reqrep *)payload;
	uint32_t paylen = 0;

	psmi_assert_always(proto != NULL);

	hdr->connect_verno = IPS_CONNECT_VERNO;
	hdr->psm_verno = PSMI_VERNO;
	hdr->connidx = (uint32_t) ipsaddr->connidx_incoming;
	hdr->epid = proto->ep->epid;

	switch (opcode) {
	case OPCODE_CONNECT_REPLY:
	case OPCODE_CONNECT_REQUEST:
		if (opcode == OPCODE_CONNECT_REQUEST) {
			req->connect_result = PSM2_OK;
			req->runid_key = proto->runid_key;
		} else {
			req->connect_result = ipsaddr->cerror_incoming;
			req->runid_key = ipsaddr->runid_key;
		}

		req->sl = proto->epinfo.ep_sl;
		// we keep this simple and send our local PSM payload (MTU)
		// after connection negotiation of a common_mtu, the MTU will be
		// further reduced by pr_mtu to set frag_size and RC QP mtu
		req->mtu = proto->epinfo.ep_mtu;
		req->job_pkey = proto->epinfo.ep_pkey;

		strncpy(req->hostname, psmi_gethostname(),
			sizeof(req->hostname) - 1);
		req->hostname[sizeof(req->hostname) - 1] = '\0';
		req->rdmamode = proto->ep->rdmamode & IPS_PROTOEXP_FLAG_RDMA_MASK;
		req->static_rate = proto->epinfo.ep_link_rate;
		memset(&req->reserved, 0, sizeof(req->reserved));
#ifdef RNDV_MOD
		// only supply gid if we want to use kernel rv
		if (IPS_PROTOEXP_FLAG_KERNEL_QP(proto->ep->rdmamode)
				&& proto->ep->verbs_ep.rv) {
			req->gid = proto->ep->verbs_ep.lgid;
			req->rv_index = proto->ep->verbs_ep.rv_index;
		} else
#endif
		{
			memset(&req->gid, 0, sizeof(req->gid));
			req->rv_index = 0;
		}
		if (ipsaddr->rc_qp) {
			psmi_assert(IPS_PROTOEXP_FLAG_USER_RC_QP(proto->ep->rdmamode));
			req->initpsn = proto->runid_key;// pid, not ideal, better than const
			req->qp_attr.qpn = ipsaddr->rc_qp->qp_num;
			req->qp_attr.mtu = opa_mtu_int_to_enum(req->mtu);
			req->qp_attr.srq = 0;
			req->qp_attr.resv = 0;
			req->qp_attr.target_ack_delay = 0; // TBD; - from local device
			req->qp_attr.resv2 = 0;
			req->qp_attr.responder_resources = 0;
			req->qp_attr.initiator_depth = 0;
			memset(&req->qp_attr.resv3, 0, sizeof(req->qp_attr.resv3));
		} else
			memset(&req->qp_attr, 0, sizeof(req->qp_attr));

		paylen = sizeof(struct ips_connect_reqrep);

		/* Attach all multi-context subnetids and epids. */
		if (proto->ep->mctxt_master == proto->ep) {
			psm2_ep_t ep = proto->ep->mctxt_next;
			uint64_t *data = (uint64_t *) (req + 1);
			while (ep != proto->ep) {
				*data = ep->gid_hi;
				paylen += sizeof(uint64_t);
				data++;
				*data = ep->epid;
				paylen += sizeof(uint64_t);
				data++;
				psmi_assert_always(paylen <= max_paylen);
				ep = ep->mctxt_next;
			}
		}

		break;

	case OPCODE_DISCONNECT_REQUEST:
	case OPCODE_DISCONNECT_REPLY:
		paylen = sizeof(struct ips_connect_hdr);
		// TBD - this is redundant if transfer_frame uses UD for all
		// control messages, but it also makes sure we stop using
		// RC for any non-control messages (should be none) after disconnect
		// use the UD QP's allocator and inline now and going forward
		ipsaddr->use_allocator =  &proto->ep->verbs_ep.send_allocator;
		ipsaddr->use_qp =  proto->ep->verbs_ep.qp;
		ipsaddr->use_max_inline_data = proto->ep->verbs_ep.qp_cap.max_inline_data;
		_HFI_MMDBG("RC discon\n");
		// ultimately we will free ipsaddr
		// so that will free RC QP and its buffers
		break;

	default:
		psmi_handle_error(PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,
				  "Unexpected/unhandled connection opcode 0x%x\n",
				  opcode);
		break;
	}

	return paylen;
}

void
MOCKABLE(ips_flow_init)(struct ips_flow *flow, struct ips_proto *proto,
	      ips_epaddr_t *ipsaddr, psm_transfer_type_t transfer_type,
	      psm_protocol_type_t protocol, ips_path_type_t path_type,
	      uint32_t flow_index)
{
	psmi_assert_always(protocol < PSM_PROTOCOL_LAST);
	psmi_assert_always(flow_index < EP_FLOW_LAST);

	SLIST_NEXT(flow, next) = NULL;
	psmi_assert(transfer_type == PSM_TRANSFER_PIO);
	flow->flush = ips_proto_flow_flush_pio;

	flow->path =
	    ips_select_path(proto, path_type, ipsaddr, ipsaddr->pathgrp);

	/* Select the fragment size for this flow. Flow is the common
	 * denominator between the local endpoint, the remote endpoint,
	 * the path between those and whether it's a PIO or DMA send.
	 * Hence, it "owns" the maximum transmission unit in its frag_size
	 * member.
	 */

	/* min of local MTU and path MTU */
	flow->frag_size = min(proto->epinfo.ep_mtu, flow->path->pr_mtu);
	_HFI_CONNDBG("[ipsaddr=%p] UD flow->frag_size: %u = min("
		"proto->epinfo.ep_mtu(%u), flow->path->pr_mtu(%u))\n",
		ipsaddr, flow->frag_size, proto->epinfo.ep_mtu,
		flow->path->pr_mtu);

	flow->ipsaddr = ipsaddr;
	flow->transfer = transfer_type;
	flow->protocol = protocol;
	flow->flowid = flow_index;
	flow->xmit_seq_num.psn_val = 0;
	flow->recv_seq_num.psn_val = 0;
	flow->xmit_ack_num.psn_val = 0;
	flow->flags = 0;
	flow->credits = flow->cwin = proto->flow_credits;
	flow->ack_interval = max((proto->flow_credits >> 2) - 1, 1);
	flow->ack_counter = 0;
#ifdef PSM_DEBUG
	flow->scb_num_pending = 0;
	flow->scb_num_unacked = 0;
#endif

	flow->timer_ack = NULL;
	flow->timer_send = NULL;

	STAILQ_INIT(&flow->scb_unacked);
	SLIST_INIT(&flow->scb_pend);
	return;
}
MOCK_DEF_EPILOGUE(ips_flow_init);

static
psm2_epaddr_t
ips_alloc_epaddr(struct ips_proto *proto, int master, psm2_epid_t epid,
		 const char *hostname,
		 unsigned long timeout, psm2_error_t *err_out)
{
	psm2_error_t err = PSM2_OK;
	psm2_epaddr_t epaddr;
	ips_epaddr_t *ipsaddr;
	ips_path_grp_t *pathgrp;
	uint16_t lid;
	uint16_t ip_hi;

	/* The PSM/PTL-level epaddr, ips-level epaddr, and per-peer msgctl
	 * structures are collocated in memory for performance reasons -- this is
	 * why ips allocates memory for all three together.
	 *
	 * The PSM/PTL structure data is filled in upon successfully ep connect in
	 * ips_ptl_connect().
	 */
	if (master) {
		struct ips_msgctl *msgctl;

		/* Although an ips_msgtl is allocated here, it can be safely casted to
		   both an ips_epaddr and a psm2_epaddr.  It is eventually freed as an
		   ips_epaddr. */
		msgctl =
		    (struct ips_msgctl *)psmi_calloc(proto->ep,
						     PER_PEER_ENDPOINT, 1,
						     sizeof(struct ips_msgctl));
		if (msgctl == NULL) {
			*err_out = PSM2_NO_MEMORY;
			return NULL;
		}

		ipsaddr = &msgctl->master_epaddr;
		epaddr = (psm2_epaddr_t) ipsaddr;

		_HFI_CONNDBG("ips_alloc_epaddr %p for EPID= 0x%"PRIx64" %s\n",
				epaddr, epid, hostname?hostname:"unknown");
		ipsaddr->msgctl = msgctl;

		/* initialize items in ips_msgctl_t */
		msgctl->ipsaddr_next = ipsaddr;
		msgctl->mq_send_seqnum = 0;
		msgctl->mq_recv_seqnum = 0;
		msgctl->am_send_seqnum = 0;
		msgctl->am_recv_seqnum = 0;
		msgctl->ipsaddr_count = 1;
		msgctl->outoforder_count = 0;
	} else {
		epaddr =
		    (psm2_epaddr_t) psmi_calloc(proto->ep, PER_PEER_ENDPOINT, 1,
					       sizeof(struct ips_epaddr));
		if (!epaddr) {
			*err_out = PSM2_NO_MEMORY;
			return NULL;
		}
		ipsaddr = (ips_epaddr_t *) epaddr;
	}

	epaddr->ptlctl = ((struct ptl_ips *)(proto->ptl))->ctl;
	epaddr->proto = proto;
	epaddr->epid = epid;

	/* IPS-level epaddr */
	ipsaddr->next = ipsaddr;

	ipsaddr->ctrl_msg_queued = 0;
	ipsaddr->msg_toggle = 0;

	ipsaddr->remote_qpn = PSMI_EPID_GET_CONTEXT(epid);

	/* Get path record for <service, slid, dlid> tuple */
	lid = PSMI_EPID_GET_LID(epid);
	ip_hi = PSMI_EPID_GET_LID(epid) >> 16;
	_HFI_CONNDBG("qpn=0x%x lid=0x%x ip_hi=0x%x\n", ipsaddr->remote_qpn, lid, ip_hi);

	err = proto->ibta.get_path_rec(proto, proto->epinfo.ep_base_lid,
				       __cpu_to_be16(lid),
				       __cpu_to_be16(ip_hi),
				       timeout,
				       &pathgrp);
	if (err != PSM2_OK) {
		goto fail;
	}
	ipsaddr->pathgrp = pathgrp;

	/* Setup high priority path index, control messages use the high
	 * priority CONTROL path.
	 */
	if (proto->flags & IPS_PROTO_FLAG_PPOLICY_ADAPTIVE)
		ipsaddr->hpp_index = 0;
	else if (proto->flags & IPS_PROTO_FLAG_PPOLICY_STATIC_DST)
		ipsaddr->hpp_index = ipsaddr->IPSADDR_HASH %
		    ipsaddr->pathgrp->pg_num_paths[IPS_PATH_HIGH_PRIORITY];
	else if (proto->flags & IPS_PROTO_FLAG_PPOLICY_STATIC_SRC)
		ipsaddr->hpp_index = proto->epinfo.EP_HASH %
		    ipsaddr->pathgrp->pg_num_paths[IPS_PATH_HIGH_PRIORITY];
	else			/* Base LID  */
		ipsaddr->hpp_index = 0;

	if (IPS_PROTOEXP_FLAG_USER_RC_QP(proto->ep->rdmamode)
#ifdef RNDV_MOD
		// if verbs_ep allows us to open w/o rv_open then we can't use RC QP
		&& (proto->ep->verbs_ep.rv
			|| proto->ep->mr_cache_mode != MR_CACHE_MODE_KERNEL)
#endif
		) {
		struct ibv_qp_cap qp_cap;
		ipsaddr->rc_qp = rc_qp_create(proto->ep, ipsaddr, &qp_cap);
		if (! ipsaddr->rc_qp) {
			_HFI_ERROR("unable to create RC QP\n");
			err = PSM2_INTERNAL_ERR;
			goto fail;
		}
		if ((proto->ep->rdmamode&IPS_PROTOEXP_FLAG_RDMA_MASK) == IPS_PROTOEXP_FLAG_RDMA_USER_RC) {
			// we need to make sure we can't overflow send Q
			if (qp_cap.max_send_wr < proto->ep->verbs_ep.send_pool.send_total) {
				_HFI_ERROR("RC QP Send Q too small\n");
				err = PSM2_INTERNAL_ERR;
				goto fail;
			}
		}
		ipsaddr->rc_qp_max_recv_wr = qp_cap.max_recv_wr;
		ipsaddr->rc_qp_max_inline_data = qp_cap.max_inline_data;
		if (PSM2_OK != psm_verbs_init_send_allocator(&ipsaddr->send_allocator,
							&proto->ep->verbs_ep.send_pool)) {
			_HFI_ERROR("can't init RC QP send allocator\n");
			err = PSM2_INTERNAL_ERR;
			goto fail;
		}
	}
	// until our QP is connected, use the UD QP's allocator and inline
	ipsaddr->use_allocator =  &proto->ep->verbs_ep.send_allocator;
	ipsaddr->use_qp =  proto->ep->verbs_ep.qp;
	ipsaddr->use_max_inline_data = proto->ep->verbs_ep.qp_cap.max_inline_data;

#ifdef RNDV_MOD
	if (IPS_PROTOEXP_FLAG_KERNEL_QP(proto->ep->rdmamode)
			&& proto->ep->verbs_ep.rv) {
		struct ibv_ah_attr ah_attr;

		ipsaddr->rv_connected = 0; // redundant since we calloc above
		// Right now we are not doing multi-pathing and
		// multi-priority so using path 0 in LOW PRIORITY (TID RDMA) is ok
		// we're going to share the same path with all processes.  So we
		// don't want to apply dispersive routing.  Hence we don't use
		//ips_select_path(proto, IPS_PATH_LOW_PRIORITY, ipsaddr, ipsaddr->pathgrp);
		// nor do we use the ipsaddr->flows[EP_FLOW_GO_BACK_N_PIO].path which
		// configure_flows will have setup similarly
		// we only need 1 connn per remote node, can share same conn for
		// all ipsaddr which go to same node.  so we track rv_conn at
		// path record level
		if (ipsaddr->pathgrp->pg_path[0][IPS_PATH_LOW_PRIORITY]->rv_conn) {
			ipsaddr->rv_conn = ipsaddr->pathgrp->pg_path[0][IPS_PATH_LOW_PRIORITY]->rv_conn;
		} else {
			err = ips_path_rec_to_ah_attr(proto->ep,
				ipsaddr->pathgrp->pg_path[0][IPS_PATH_LOW_PRIORITY], // ????
				&ah_attr);
			if (PSM2_OK != err) {
				_HFI_ERROR("unable to get ah from path\n");
				goto fail;
			}
			ipsaddr->rv_conn = __psm2_rv_create_conn(proto->ep->verbs_ep.rv,
							 &ah_attr, (uint32_t)PSMI_EPID_GET_LID(epid));
			ipsaddr->pathgrp->pg_path[0][IPS_PATH_LOW_PRIORITY]->rv_conn = ipsaddr->rv_conn;
			if (! ipsaddr->rv_conn) {
				_HFI_ERROR("rv_create_conn failed: %s\n", strerror(errno));
				err = PSM2_INTERNAL_ERR;
//TBD - should we make this non-fatal?  Just regress to UD mode and output ERROR
				goto fail;
			}
		}
	}
#endif // RNDV_MOD

	/*
	 * Set up the flows on this ipsaddr
	 */
	ips_ipsaddr_configure_flows(ipsaddr, proto);

	/* clear connection state. */
	ipsaddr->cstate_outgoing = CSTATE_NONE;
	ipsaddr->cstate_incoming = CSTATE_NONE;

	/* Add epaddr to PSM's epid table */
	psmi_epid_add(proto->ep, epaddr->epid, epaddr);
	psmi_assert(psmi_epid_lookup(proto->ep, epaddr->epid) == epaddr);

	*err_out = PSM2_OK;
	return epaddr;

fail:
	if (ipsaddr->rc_qp) {
		rc_qp_destroy(ipsaddr->rc_qp);
		ipsaddr->rc_qp = NULL;
	}
	psmi_free(epaddr);
	*err_out = err;
	return NULL;
}

static
void ips_free_epaddr(psm2_epaddr_t epaddr, struct ips_proto *proto)
{
	ips_epaddr_t *ipsaddr = (ips_epaddr_t *) epaddr;
	ips_flow_fini(ipsaddr, proto);

	_HFI_CONNDBG("epaddr=%p connidx_incoming=%d epid=0x%"PRIx64"\n",
			epaddr, ipsaddr->connidx_incoming, epaddr->epid);
	IPS_MCTXT_REMOVE(ipsaddr);
#ifdef RNDV_MOD
	_HFI_MMDBG("free_epaddr\n");
	if (ipsaddr->rv_conn) {
		//__psm2_rv_destroy_conn(ipsaddr->rv_conn);
		// TBD - call rv_disconnect or maybe rv_destroy_conn
		// TBD disconnect and free rv_conn
		// TBD - can we do this in a synchronous manner?
		// below we free epaddr, so we will lose track of rv_conn
		// but maybe rndv module will track it enough that we don't have to
		// here, provided we don't confuse ourselves with a discon resp
		// because the rv_conn's content we will get in that callback
		// may be pointing to a freed rv_conn or freed epaddr
		// maybe just call rndv_mod to set context to 0?  But could
		// be races for callbacks and events already queued
	}
#endif // RNDV_MOD
	if (ipsaddr->rc_qp) {
		rc_qp_destroy(ipsaddr->rc_qp);
		ipsaddr->rc_qp = NULL;
	}
	psm_verbs_free_recv_pool(&ipsaddr->recv_pool);
	psmi_epid_remove(epaddr->proto->ep, epaddr->epid);
	ips_epstate_del(epaddr->proto->epstate, ipsaddr->connidx_incoming);
	psmi_free(epaddr);
	return;
}

static
psm2_error_t
ptl_handle_connect_req(struct ips_proto *proto,
		       psm2_epaddr_t epaddr, struct ips_connect_reqrep *req,
		       uint32_t paylen);

psm2_error_t
ips_proto_process_connect(struct ips_proto *proto, uint8_t opcode,
			  struct ips_message_header *p_hdr, void *payload,
			  uint32_t paylen)
{
	struct ips_connect_hdr *hdr = (struct ips_connect_hdr *)payload;
	psm2_epaddr_t epaddr;
	ips_epaddr_t *ipsaddr;
	psm2_error_t err = PSM2_OK;

	PSMI_LOCK_ASSERT(proto->mq->progress_lock);

	epaddr = psmi_epid_lookup(proto->ep, hdr->epid);
	ipsaddr = epaddr ? (ips_epaddr_t *) epaddr : NULL;

	_HFI_CONNDBG("Conn Pkt Rcv'd: op=0x%02x from: 0x%lx to: 0x%lx\n",
		opcode, hdr->epid, proto->ep->epid);
	switch (opcode) {
	case OPCODE_CONNECT_REQUEST:
		proto->epaddr_stats.connect_req_recv++;
		err = ptl_handle_connect_req(proto, epaddr,
					     (struct ips_connect_reqrep *)hdr,
					     paylen);
		break;

	case OPCODE_CONNECT_REPLY:
		{
			struct ips_connect_reqrep *req =
			    (struct ips_connect_reqrep *)payload;

			proto->epaddr_stats.connect_rep_recv++;
			if (!ipsaddr || req->runid_key != proto->runid_key) {
				_HFI_PRDBG
				    ("Unknown connectrep (ipsaddr=%p, %d,%d) from epid 0x%"PRIx64": %s\n",
				     ipsaddr, req->runid_key, proto->runid_key,
				     hdr->epid, psmi_epaddr_fmt_addr(hdr->epid));
			} else if (ipsaddr->cstate_outgoing != CSTATE_OUTGOING_WAITING) {
				/* possible dupe */
				_HFI_CONNDBG("connect dupe, expected %d got %d\n",
					  CSTATE_OUTGOING_WAITING,
					  ipsaddr->cstate_outgoing);
			} else {
				/* Reply to our request for connection (i.e. outgoing connection) */
				if (ipsaddr->cstate_incoming != CSTATE_ESTABLISHED) {
					err =
					    ips_ipsaddr_set_req_params(proto,
								       ipsaddr,
								       req,
								       paylen);
					if (err)
						goto fail;
				}
				if (ipsaddr->rc_qp) {
					psmi_assert(IPS_PROTOEXP_FLAG_USER_RC_QP(proto->ep->rdmamode));
					psmi_assert(req->qp_attr.qpn); // checked in set_req_params
					// we got a a REP, we can move to RTS
					if (modify_rc_qp_to_rts(proto->ep, ipsaddr->rc_qp,
						&req->qp_attr, proto->runid_key)) { // initpsn we sent
						_HFI_ERROR("qp_to_rts failed\n");
						return PSM2_INTERNAL_ERR;
					}
					if ((proto->ep->rdmamode&IPS_PROTOEXP_FLAG_RDMA_MASK) == IPS_PROTOEXP_FLAG_RDMA_USER_RC) {
						// use RC QPs for eager and RDMA
						// now we can use our own send Q and send allocator
						ipsaddr->use_allocator =  &ipsaddr->send_allocator;
						ipsaddr->use_qp =  ipsaddr->rc_qp;
						ipsaddr->use_max_inline_data = ipsaddr->rc_qp_max_inline_data;
						_HFI_MMDBG("RC enabled\n");
					}
					ipsaddr->rc_connected = 1;
				}
				ipsaddr->cstate_outgoing = CSTATE_ESTABLISHED;
				ipsaddr->cerror_outgoing = req->connect_result;
			}
		}
		break;

	case OPCODE_DISCONNECT_REQUEST:
		{
			ips_epaddr_t ipsaddr_f;	/* fake a ptl addr */
			int epaddr_do_free = 0;
			psmi_assert_always(paylen ==
					   sizeof(struct ips_connect_hdr));
			_HFI_CONNDBG("Got a disconnect from %s\n",
				  psmi_epaddr_get_name(hdr->epid));
			proto->num_disconnect_requests++;
			proto->epaddr_stats.disconnect_req_recv++;
			/* It's possible to get a disconnection request on a ipsaddr that
			 * we've since removed if the request is a dupe.  Instead of
			 * silently dropping the packet, we "echo" the request in the
			 * reply. */
			if (ipsaddr == NULL) {
				ips_path_grp_t *pathgrp;
				uint16_t lid;
				uint16_t ip_hi;

				ipsaddr = &ipsaddr_f;
				memset(&ipsaddr_f, 0, sizeof(ips_epaddr_t));
				ipsaddr_f.IPSADDR_HASH =
				    PSMI_EPID_GET_CONTEXT(hdr->epid);

				/* Get path record for peer */
				lid = PSMI_EPID_GET_LID(hdr->epid);
				ip_hi = PSMI_EPID_GET_LID(hdr->epid) >> 16;
				err = proto->ibta.get_path_rec(proto,
							       proto->epinfo.
								   ep_base_lid,
							       __cpu_to_be16(lid),
							       __cpu_to_be16(ip_hi),
								   3000, &pathgrp);
				if (err != PSM2_OK)
					goto fail;

				ipsaddr_f.pathgrp = pathgrp;
				((psm2_epaddr_t) &ipsaddr_f)->ptlctl =
					((struct ptl_ips *)(proto->ptl))->ctl;
				((psm2_epaddr_t) &ipsaddr_f)->proto = proto;
				/* If the send fails because of pio_busy, don't let ips queue
				 * the request on an invalid ipsaddr, just drop the reply */
				ipsaddr_f.ctrl_msg_queued = ~0;

				psmi_assert_always(proto->msgflowid < EP_FLOW_LAST);

				ips_flow_init(&ipsaddr_f.
					      flows[proto->msgflowid], proto,
					      &ipsaddr_f, PSM_TRANSFER_PIO,
					      PSM_PROTOCOL_GO_BACK_N,
					      IPS_PATH_LOW_PRIORITY,
					      EP_FLOW_GO_BACK_N_PIO);
				_HFI_CONNDBG
				    ("Disconnect on unknown epaddr, just echo request\n");
			} else if (ipsaddr->cstate_incoming != CSTATE_NONE) {
				ipsaddr->cstate_incoming = CSTATE_NONE;
				proto->num_connected_incoming--;
				if (ipsaddr->cstate_outgoing == CSTATE_NONE) {
					epaddr_do_free = 1;
				}
			}

			psmi_assert_always(proto->msgflowid < EP_FLOW_LAST);
			// TBD - this is redundant if transfer_frame uses UD for all
			// control messages, but it also makes sure we stop using
			// RC for any non-control messages (should be none) after disconnect
			// use the UD QP's allocator and inline now and going forward
			ipsaddr->use_allocator =  &proto->ep->verbs_ep.send_allocator;
			ipsaddr->use_qp =  proto->ep->verbs_ep.qp;
			ipsaddr->use_max_inline_data = proto->ep->verbs_ep.qp_cap.max_inline_data;
			_HFI_MMDBG("RC discon\n");
			// we will free ipsaddr below for all but "fake ipsaddr" case
			// so that will free RC QP and its buffers

			ips_proto_send_ctrl_message_reply(proto, &ipsaddr->
							  flows[proto->
								msgflowid],
							  OPCODE_DISCONNECT_REPLY,
							  &ipsaddr->
							  ctrl_msg_queued);
			/* We can safely free the ipsaddr if required since disconnect
			 * messages are never enqueued so no reference to ipsaddr is kept */
			if (epaddr_do_free) {
				ips_free_epaddr(epaddr, proto);
				epaddr = NULL;
			}
		}
		break;

	case OPCODE_DISCONNECT_REPLY:
		proto->epaddr_stats.disconnect_rep_recv++;
		if (!ipsaddr) {
			_HFI_CONNDBG
			    ("Unknown disconnect reply from epid 0x%"PRIx64": %s\n",
			     hdr->epid, psmi_epaddr_fmt_addr(hdr->epid));
			break;
		} else if (ipsaddr->cstate_outgoing == CSTATE_OUTGOING_WAITING_DISC) {
			ipsaddr->cstate_outgoing = CSTATE_OUTGOING_DISCONNECTED;
			/* Freed in disconnect() if cstate_incoming == NONE */
		}		/* else dupe reply */
		break;

	default:
		psmi_handle_error(PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,
				  "Unexpected/unhandled connect opcode 0x%x\n",
				  opcode);
	}

fail:
	return err;
}

static
psm2_error_t
ptl_handle_connect_req(struct ips_proto *proto, psm2_epaddr_t epaddr,
		       struct ips_connect_reqrep *req, uint32_t paylen)
{
	ips_epaddr_t *ipsaddr;
	psm2_error_t err = PSM2_OK;
	uint16_t connect_result;
	int newconnect = 0;

	if (req->epid == proto->ep->epid) {
		psmi_handle_error(PSMI_EP_NORETURN, PSM2_EPID_NETWORK_ERROR,
				  "Network connectivity problem: Locally detected duplicate "
				  "LIDs 0x%04x on hosts %s and %s (%s port %u). (Exiting)",
				  (uint32_t) psm2_epid_nid(req->epid),
				  psmi_epaddr_get_hostname(req->epid),
				  psmi_gethostname(), proto->ep->dev_name, proto->ep->portnum);
		/* XXX no return */
		abort();
	} else if (epaddr == NULL) {	/* new ep connect before we call into connect */
		newconnect = 1;
		if ((epaddr =
		     ips_alloc_epaddr(proto, 1, req->epid, req->hostname,
					      5000, &err)) == NULL) {
			goto fail;
		}
	} else if (((ips_epaddr_t *) epaddr)->cstate_incoming == CSTATE_ESTABLISHED) {
		ipsaddr = (ips_epaddr_t *) epaddr;
		/* Duplicate lid detection.  */
		if (ipsaddr->runid_key == req->runid_key)
			goto do_reply;	/* duplicate request, not duplicate lid */
		else {		/* Some out of context message.  Just drop it */
			if (!proto->done_warning) {
				psmi_syslog(proto->ep, 1, LOG_INFO,
					    "Non-fatal connection problem: Received an out-of-context "
					    "connection message from host %s LID=0x%x context=%d. (Ignoring)",
					    req->hostname,
					    (int)psm2_epid_nid(req->epid),
					    psm2_epid_context(req->epid));
				proto->done_warning = 1;
			}
			goto no_reply;
		}
	} else if (((ips_epaddr_t *) epaddr)->cstate_outgoing == CSTATE_NONE) {
		/* pre-created epaddr in multi-rail */
		psmi_assert_always(epaddr->proto->ep !=
				   epaddr->proto->ep->mctxt_master);
		newconnect = 1;
	}

	ipsaddr = (ips_epaddr_t *) epaddr;
	psmi_assert_always(ipsaddr->cstate_incoming == CSTATE_NONE);

	/* Check connect version and psm version */
	// for now we are strict about major rev, if we add additional optional
	// features they can be minor revs and may need more sophisticated handling
	if ((req->connect_verno >>8) != (IPS_CONNECT_VERNO >>8)) {
		psmi_handle_error(PSMI_EP_NORETURN, PSM2_EPID_INVALID_VERSION,
				  "Connect protocol (%x,%x) is incompatible with %x.%x",
				  (req->connect_verno >> 8) & 0xff,
				  req->connect_verno & 0xff,
				  (IPS_CONNECT_VERNO >> 8) & 0xff,
				  IPS_CONNECT_VERNO & 0xff);
		connect_result = PSM2_EPID_INVALID_CONNECT;
	} else if (!psmi_verno_isinteroperable(req->psm_verno)) {
		connect_result = PSM2_EPID_INVALID_VERSION;
	} else if (!(proto->flags & IPS_PROTO_FLAG_QUERY_PATH_REC) &&
		   proto->epinfo.ep_pkey != HFI_DEFAULT_P_KEY &&
		   proto->epinfo.ep_pkey != req->job_pkey) {
		connect_result = PSM2_EPID_INVALID_PKEY;
	} else if (req->sl != proto->epinfo.ep_sl) {
		connect_result = PSM2_EPID_INVALID_CONNECT;
		_HFI_ERROR("Connection error: Service Level mismatch (local:%d, remote:%d)\n", proto->epinfo.ep_sl, req->sl);
	} else if (req->rdmamode != (proto->ep->rdmamode & IPS_PROTOEXP_FLAG_RDMA_MASK) ) {
		connect_result = PSM2_EPID_INVALID_CONNECT;
		_HFI_ERROR("Connection error: RDMA Mode mismatch (local:%d, remote:%d)\n",
			(proto->ep->rdmamode & IPS_PROTOEXP_FLAG_RDMA_MASK), req->rdmamode);
	} else {
		connect_result = PSM2_OK;
		if (ipsaddr->cstate_outgoing == CSTATE_NONE) {
			ips_epstate_idx idx;
			psmi_assert_always(newconnect == 1);
			err = ips_epstate_add(proto->epstate, ipsaddr, &idx);
			if (err)
				goto fail;
			ipsaddr->connidx_incoming = idx;
		}
	}

	/* Incoming connection request */
	if (ipsaddr->cstate_outgoing != CSTATE_ESTABLISHED) {
		err = ips_ipsaddr_set_req_params(proto, ipsaddr, req, paylen);
		if (err)
			goto fail;
	}
	ipsaddr->cstate_incoming = CSTATE_ESTABLISHED;
	ipsaddr->cerror_incoming = connect_result;

	ipsaddr->runid_key = req->runid_key;

	proto->num_connected_incoming++;

do_reply:
	_HFI_CONNDBG("Conn Pkt Sent: op=0x%02x from: 0x%lx to: 0x%lx\n",
		OPCODE_CONNECT_REPLY, proto->ep->epid, ipsaddr->epaddr.epid);
	psmi_assert_always(proto->msgflowid < EP_FLOW_LAST);
	ips_proto_send_ctrl_message_reply(proto,
					  &ipsaddr->flows[proto->msgflowid],
					  OPCODE_CONNECT_REPLY,
					  &ipsaddr->ctrl_msg_queued);
no_reply:
fail:
	return err;
}

psm2_error_t
ips_proto_connect(struct ips_proto *proto, int numep,
		  const psm2_epid_t *array_of_epid,
		  const int *array_of_epid_mask, psm2_error_t *array_of_errors,
		  psm2_epaddr_t *array_of_epaddr, uint64_t timeout_in)
{
	int i, n, n_first;
	psm2_error_t err = PSM2_OK;
	psm2_epaddr_t epaddr;
	ips_epaddr_t *ipsaddr;
	ips_epstate_idx idx;
	int numep_toconnect = 0, numep_left;
	union psmi_envvar_val credits_intval;
	int connect_credits;

	psmi_getenv("PSM3_CONNECT_CREDITS",
		    "End-point connect request credits.",
		    PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_UINT,
		    (union psmi_envvar_val)100, &credits_intval);

	connect_credits = credits_intval.e_uint;

	PSMI_LOCK_ASSERT(proto->mq->progress_lock);

	/* All timeout values are in cycles */
	uint64_t t_start = get_cycles();
	/* Print a timeout at the warning interval */
	union psmi_envvar_val warn_intval;
	uint64_t to_warning_interval;
	uint64_t to_warning_next;

	/* Setup warning interval */
	psmi_getenv("PSM3_CONNECT_WARN_INTERVAL",
		    "Period in seconds to warn if connections are not completed."
		    "Default is 300 seconds, 0 to disable",
		    PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_UINT,
		    (union psmi_envvar_val)300, &warn_intval);

	to_warning_interval = nanosecs_to_cycles(warn_intval.e_uint * SEC_ULL);
	to_warning_next = t_start + to_warning_interval;

	/* Some sanity checks */
	psmi_assert_always(array_of_epid_mask != NULL);

	/* First pass: make sure array of errors is at least fully defined */
	for (i = 0; i < numep; i++) {
		_HFI_CONNDBG("epid-connect=%s connect to epid 0x%"PRIx64": %s\n",
			  array_of_epid_mask[i] ? "YES" : " NO",
			  array_of_epid[i],
			  psmi_epaddr_fmt_addr(array_of_epid[i]));
		if (array_of_epid_mask[i]) {
			array_of_errors[i] = PSM2_EPID_UNKNOWN;
			array_of_epaddr[i] = NULL;
		}
	}

	/* Second pass: see what to connect and what is connectable. */
	for (i = 0, numep_toconnect = 0; i < numep; i++) {
		if (!array_of_epid_mask[i])
			continue;

		/* Can't send to epid on same lid if not loopback */
		if ((psm2_epid_nid(proto->ep->epid) ==
		    psm2_epid_nid(array_of_epid[i])) &&
		    !(proto->flags & IPS_PROTO_FLAG_LOOPBACK)) {
			array_of_errors[i] = PSM2_EPID_UNREACHABLE;
			continue;
		}

		if ((PSMI_EPID_VERSION == PSMI_EPID_V3
				|| (PSMI_EPID_VERSION == PSMI_EPID_V4 && ! psmi_allow_routers))
			 && (PSMI_GET_SUBNET_ID(proto->ep->gid_hi) !=
			 	 PSMI_EPID_GET_SUBNET_ID(array_of_epid[i]))) {
			char buf1[INET_ADDRSTRLEN];
			char buf2[INET_ADDRSTRLEN];
			if (PSMI_EPID_VERSION == PSMI_EPID_V3)
					psmi_handle_error(PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,
					  " Trying to connect from %s port %u to a node (subnet id - %"PRIx64") on a"
					  " different subnet - %"PRIx64"\n",
					  proto->ep->dev_name, proto->ep->portnum,
					  PSMI_GET_SUBNET_ID(proto->ep->gid_hi),
					  (uint64_t)PSMI_EPID_GET_SUBNET_ID(array_of_epid[i]));
			else // V4
					psmi_handle_error(PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,
					  " Trying to connect from %s to a node (subnet %s) on a"
					  " different subnet %s\n",
					  proto->ep->dev_name,
					  psmi_ipv4_ntop((uint32_t)PSMI_GET_SUBNET_ID(proto->ep->gid_hi), buf1, sizeof(buf1)),
					  psmi_ipv4_ntop((uint32_t)PSMI_EPID_GET_SUBNET_ID(array_of_epid[i]), buf2, sizeof(buf2)));
		}

		epaddr = psmi_epid_lookup(proto->ep, array_of_epid[i]);
		if (epaddr == NULL) {
			/* We're sending a connect request message before some other node
			 * has sent its connect message */
			// so we lack it's hostname, rv and qpn info
			epaddr = ips_alloc_epaddr(proto, 1, array_of_epid[i],
						  NULL,
						  (timeout_in / 1000000UL), &err);
			if (epaddr == NULL) {
				_HFI_ERROR("Unable to issue connect from %s to %s: %s\n",
						proto->ep->dev_name, psmi_epaddr_get_name(array_of_epid[i]),
						psm2_error_get_string(err));
				goto fail;
			}
			ipsaddr = (ips_epaddr_t *) epaddr;
			err = ips_epstate_add(proto->epstate, ipsaddr, &idx);
			if (err)
				goto fail;
			ipsaddr->connidx_incoming = idx;
		} else if (((ips_epaddr_t *) epaddr)->cstate_outgoing != CSTATE_NONE) {	/* already connected */
			psmi_assert_always(((ips_epaddr_t *) epaddr)->
					   cstate_outgoing == CSTATE_ESTABLISHED);
			array_of_errors[i] = PSM2_EPID_ALREADY_CONNECTED;
			array_of_epaddr[i] = epaddr;
			continue;
		} else if (((ips_epaddr_t *) epaddr)->cstate_incoming ==
			   CSTATE_NONE) {
			/* pre-created epaddr in multi-rail */
			psmi_assert_always(epaddr->proto->ep !=
					   epaddr->proto->ep->mctxt_master);
			ipsaddr = (ips_epaddr_t *) epaddr;
			err = ips_epstate_add(proto->epstate, ipsaddr, &idx);
			if (err)
				goto fail;
			ipsaddr->connidx_incoming = idx;
		} else {
			/* We've already received a connect request message from a remote
			 * peer, it's time to send our own. */
			ipsaddr = (ips_epaddr_t *) epaddr;
			/* No re-entrancy sanity check and makes sure we are not connected
			 * twice (caller's precondition) */
			psmi_assert(ipsaddr->cstate_outgoing == CSTATE_NONE);
			psmi_assert(ipsaddr->cstate_incoming != CSTATE_NONE);
		}

		ipsaddr->cstate_outgoing = CSTATE_OUTGOING_WAITING;
		ipsaddr->cerror_outgoing = PSM2_OK;
		array_of_epaddr[i] = epaddr;
		ipsaddr->s_timeout = get_cycles();
		ipsaddr->delay_in_ms = 1;
		ipsaddr->credit = 0;
		numep_toconnect++;
	}

	/* Second pass: do the actual connect.
	 * PSM2_EPID_UNKNOWN: Not connected yet.
	 * PSM2_EPID_UNREACHABLE: Not to be connected.
	 * PSM2_OK: Successfully connected.
	 * Start sending connect messages at a random index between 0 and numep-1
	 */
	numep_left = numep_toconnect;
	n_first = ((uint32_t) get_cycles()) % numep;
	while (numep_left > 0) {
		for (n = 0; n < numep; n++) {
			int keep_polling = 1;
			i = (n_first + n) % numep;
			if (!array_of_epid_mask[i])
				continue;
			switch (array_of_errors[i]) {
			case PSM2_EPID_UNREACHABLE:
			case PSM2_EPID_ALREADY_CONNECTED:
			case PSM2_OK:
				continue;
			default:
				break;
			}
			psmi_assert_always(array_of_epaddr[i] != NULL);
			ipsaddr = (ips_epaddr_t *) array_of_epaddr[i];
			if (ipsaddr->cstate_outgoing == CSTATE_ESTABLISHED) {
				if (ipsaddr->credit) {
					connect_credits++;
					ipsaddr->credit = 0;
				}
				switch (is_rv_connected(ipsaddr)) {
				case 1:
					/* This is not the real error code, we only set OK here
					 * so we know to stop polling for the reply. The actual
					 * error is in ipsaddr->cerror_outgoing */
					array_of_errors[i] = PSM2_OK;
					numep_left--;
					continue;
					break;
				case 0:
					// fall through to "keep_polling" loop below to check timers
					break;
				default:
					/* This is not the real error code, we only set OK here
					 * so we know to stop polling for the reply. The actual
					 * error is in ipsaddr->cerror_outgoing */
					array_of_errors[i] = PSM2_OK;
					numep_left--;
					if (ipsaddr->cerror_outgoing == PSM2_OK)
						ipsaddr->cerror_outgoing = PSM2_EPID_RV_CONNECT_ERROR;
					// EIO is connect error
					if (errno != EIO) {
						err = PSM2_INTERNAL_ERR;
						goto fail;	// serious error
					}
					continue;
					break;
				}
			}
			while (keep_polling) {
				if (!psmi_cycles_left(t_start, timeout_in)) {
					err = PSM2_TIMEOUT;
					goto err_timeout;
				}
				if (to_warning_interval
				    && get_cycles() >= to_warning_next) {
#if _HFI_DEBUGGING
					uint64_t waiting_time = 0;
					if (_HFI_INFO_ON) {
					    waiting_time = cycles_to_nanosecs(
								get_cycles() -
								t_start) / SEC_ULL;
					}
#endif
					const char *first_name = NULL;
					int num_waiting = 0;

					for (i = 0; i < numep; i++) {
						if (!array_of_epid_mask[i] ||
						    array_of_errors[i] !=
						    PSM2_EPID_UNKNOWN)
							continue;
						if (!first_name)
							first_name =
							    psmi_epaddr_get_name
							    (array_of_epid[i]);
						num_waiting++;
					}
					if (_HFI_INFO_ON) {
						if (first_name) {
						_HFI_INFO_ALWAYS
						    ("Couldn't connect to %s (and %d others). "
						     "Time elapsed %02i:%02i:%02i. Still trying...\n",
						     first_name, num_waiting,
						     (int)(waiting_time / 3600),
						     (int)((waiting_time / 60) -
							   ((waiting_time /
							     3600) * 60)),
						     (int)(waiting_time -
							   ((waiting_time /
							     60) * 60)));
						}
					}
					to_warning_next =
					    get_cycles() + to_warning_interval;
				}
				if (ipsaddr->cstate_outgoing == CSTATE_ESTABLISHED) {
					// just waiting for rv to be connected
					if ((err = psmi_err_only(psmi_poll_internal(proto->ep, 1))))
						goto fail;
					break;	// let outer loop start another REQ
				}

				if (get_cycles() > ipsaddr->s_timeout) {
					if (!ipsaddr->credit && connect_credits) {
						ipsaddr->credit = 1;
						connect_credits--;
					}
					if (ipsaddr->credit) {
						_HFI_CONNDBG("Conn Pkt Sent: op=0x%02x from: 0x%lx to: 0x%lx\n",
							OPCODE_CONNECT_REQUEST, proto->ep->epid, ipsaddr->epaddr.epid);
					    psmi_assert_always(proto->msgflowid < EP_FLOW_LAST);
					    if (
					    ips_proto_send_ctrl_message_request
						    (proto, &ipsaddr->
						     flows[proto->msgflowid],
						     OPCODE_CONNECT_REQUEST,
						     &ipsaddr->ctrl_msg_queued,
						     0) == PSM2_OK) {
							keep_polling = 0;
							ipsaddr->delay_in_ms =
							    min(100,
								ipsaddr->
								delay_in_ms <<
								1);
							ipsaddr->s_timeout =
							    get_cycles() +
							    nanosecs_to_cycles
							    (ipsaddr->
							     delay_in_ms *
							     MSEC_ULL);
						}
						/* If not, send got "busy", keep trying */
					} else {
						keep_polling = 0;
					}
				}

				if ((err = psmi_err_only(psmi_poll_internal(proto->ep, 1))))
					goto fail;

				if (ipsaddr->cstate_outgoing == CSTATE_ESTABLISHED) {
					connect_credits++;
					ipsaddr->credit = 0;
					switch (is_rv_connected(ipsaddr)) {
					case 1:
						/* This is not the real error code, we only set OK here
						 * so we know to stop polling for the reply. The actual
						 * error is in ipsaddr->cerror_outgoing */
						array_of_errors[i] = PSM2_OK;
						numep_left--;
						break;
					case 0:
						break;
					default:
						/* This is not the real error code, we only set OK here
						 * so we know to stop polling for the reply. The actual
						 * error is in ipsaddr->cerror_outgoing */
						array_of_errors[i] = PSM2_OK;
						numep_left--;
						if (ipsaddr->cerror_outgoing == PSM2_OK)
							ipsaddr->cerror_outgoing = PSM2_EPID_RV_CONNECT_ERROR;
						// EIO is connect error
						if (errno != EIO) {
							err = PSM2_INTERNAL_ERR;
							goto fail;	// serious error
						}
						break;
					}
					// even if ! rv_connected, let outer loop start next REQ
					break;
				}
			}
		}
	}

err_timeout:
	/* Find the worst error to report */
	for (i = 0; i < numep; i++) {
		if (!array_of_epid_mask[i])
			continue;
		switch (array_of_errors[i]) {
			/* These are benign */
		case PSM2_EPID_UNREACHABLE:
		case PSM2_EPID_ALREADY_CONNECTED:
			break;
		case PSM2_EPID_UNKNOWN:
			array_of_errors[i] = PSM2_TIMEOUT;
			err = psmi_error_cmp(err, PSM2_TIMEOUT);
			_HFI_CONNDBG("EP has timed out on connect.\n");
			break;
		case PSM2_OK:
			/* Restore the real connect error */
			ipsaddr = (ips_epaddr_t *) array_of_epaddr[i];
			array_of_errors[i] = psmi_error_cmp(ipsaddr->cerror_outgoing,
												ipsaddr->cerror_incoming);
			psmi_assert_always(ipsaddr->cstate_outgoing ==
					   CSTATE_ESTABLISHED);
			if (array_of_errors[i] != PSM2_OK) {
				err = psmi_error_cmp(err, array_of_errors[i]);
				ips_free_epaddr(array_of_epaddr[i], proto);
				array_of_epaddr[i] = NULL;
			} else {
				proto->num_connected_outgoing++;
				psmi_assert_always(ipsaddr->pathgrp->
						   pg_path[0]
						   [IPS_PATH_HIGH_PRIORITY]->
						   pr_mtu > 0);
			}
			break;
		default:
			_HFI_CONNDBG("EP has error code %d\n", array_of_errors[i]);
			break;
		}
	}

fail:
	return err;
}

/* Repercussions on MQ.
 *
 * If num_connected==0, everything that exists in the posted queue should
 * complete and the error must be marked epid_was_closed.
 *
 */

psm2_error_t
ips_proto_disconnect(struct ips_proto *proto, int force, int numep,
		     psm2_epaddr_t array_of_epaddr[],
		     const int array_of_epaddr_mask[],
		     psm2_error_t array_of_errors[], uint64_t timeout_in)
{
	ips_epaddr_t *ipsaddr;
	int numep_left, numep_todisc, i, n;
	int n_first;
	int has_pending;
	uint64_t timeout;
	psm2_error_t err = PSM2_OK;
	uint64_t reqs_sent = 0;
	union psmi_envvar_val credits_intval;
	int disconnect_credits;
	uint64_t t_warning, t_start;
	union psmi_envvar_val warn_intval;
	unsigned warning_secs;

	/* In case of a forced close, we cancel whatever timers are pending
	 * on the proto so that we don't have zombie timers coming back
	 * after the internal structures of PSM2 have been destroyed
	 */
	if (force) {
		struct psmi_timer *t_cursor;
		TAILQ_FOREACH(t_cursor, &proto->timerq->timerq, timer) {
			psmi_timer_cancel(proto->timerq, t_cursor);
		}
	}

	psmi_assert_always(numep > 0);

	psmi_getenv("PSM3_DISCONNECT_CREDITS",
		    "End-point disconnect request credits.",
		    PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_UINT,
		    (union psmi_envvar_val)100, &credits_intval);

	disconnect_credits = credits_intval.e_uint;

	/* Setup warning interval */
	psmi_getenv("PSM3_DISCONNECT_WARN_INTERVAL",
		    "Period in seconds to warn if disconnections are not completed."
		    "Default is 300 seconds, 0 to disable.",
		    PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_UINT,
		    (union psmi_envvar_val)300, &warn_intval);

	warning_secs = warn_intval.e_uint;

	PSMI_LOCK_ASSERT(proto->mq->progress_lock);

	/* First pass: see what to disconnect and what is disconnectable */
	for (i = 0, numep_todisc = 0; i < numep; i++) {
		if (!array_of_epaddr_mask[i])
			continue;
		psmi_assert_always(array_of_epaddr[i]->ptlctl->ptl ==
				   proto->ptl);
		ipsaddr = (ips_epaddr_t *) array_of_epaddr[i];
		ipsaddr->credit = 0;
		if (ipsaddr->cstate_outgoing == CSTATE_NONE) {
			array_of_errors[i] = PSM2_OK;
			continue;
		} else {
			psmi_assert_always(ipsaddr->cstate_outgoing ==
					   CSTATE_ESTABLISHED);
		}
		_HFI_CONNDBG("disconnecting %p force=%d EPID= 0x%"PRIx64" %s\n",
				ipsaddr, force, ((psm2_epaddr_t)ipsaddr)->epid,

				psmi_epaddr_get_hostname(((psm2_epaddr_t)ipsaddr)->epid));
		array_of_errors[i] = PSM2_EPID_UNKNOWN;
		numep_todisc++;
	}
	if (numep_todisc == 0)
		goto success;

	/* Wait for everyone to ack previous packets before putting */
	if (timeout_in == 0)
		timeout = ~0ULL;
	else
		timeout = get_cycles() + nanosecs_to_cycles(timeout_in);

	t_start = get_cycles();
	t_warning = t_start + nanosecs_to_cycles(warning_secs * SEC_ULL);

	n_first = ((uint32_t) get_cycles()) % numep;
	if (!force) {
		numep_left = numep_todisc;
		do {
			for (n = 0; n < numep; n++) {
				i = (n_first + n) % numep;
				if (!array_of_epaddr_mask[i]
				    || array_of_errors[i] == PSM2_OK)
					continue;
				ipsaddr = (ips_epaddr_t *) array_of_epaddr[i];
				switch (ipsaddr->cstate_outgoing) {
				case CSTATE_OUTGOING_DISCONNECTED:
					array_of_errors[i] = PSM2_OK;
					numep_left--;
					disconnect_credits++;
					ipsaddr->credit = 0;
					continue;
				case CSTATE_OUTGOING_WAITING_DISC:
					if (ipsaddr->s_timeout > get_cycles())
						continue;
					ipsaddr->delay_in_ms =
					    min(100, ipsaddr->delay_in_ms << 1);
					ipsaddr->s_timeout = get_cycles() +
					    nanosecs_to_cycles(ipsaddr->
							       delay_in_ms *
							       MSEC_ULL);
					psmi_assert_always(proto->msgflowid < EP_FLOW_LAST);
					ips_proto_send_ctrl_message_request
					    (proto,
					     &ipsaddr->flows[proto->msgflowid],
					     OPCODE_DISCONNECT_REQUEST,
					     &ipsaddr->ctrl_msg_queued,
					     timeout);
					reqs_sent++;
					break;
				case CSTATE_ESTABLISHED:
					/* Still pending acks, hold off for now */
					has_pending =
					    !STAILQ_EMPTY(&ipsaddr->flows
							  [EP_FLOW_GO_BACK_N_PIO].
							  scb_unacked)
						;
					if (has_pending)
						continue;
					if (!ipsaddr->credit
					    && disconnect_credits) {
						ipsaddr->credit = 1;
						disconnect_credits--;
					}
					if (!ipsaddr->credit)
						continue;
					ipsaddr->delay_in_ms = 1;
					ipsaddr->cstate_outgoing =
					    CSTATE_OUTGOING_WAITING_DISC;
					ipsaddr->s_timeout =
					    get_cycles() +
					    nanosecs_to_cycles(MSEC_ULL);
					psmi_assert_always(proto->msgflowid < EP_FLOW_LAST);
					ips_proto_send_ctrl_message_request
					    (proto,
					     &ipsaddr->flows[proto->msgflowid],
					     OPCODE_DISCONNECT_REQUEST,
					     &ipsaddr->ctrl_msg_queued,
					     timeout);
					reqs_sent++;
					break;
				default:
					psmi_handle_error(PSMI_EP_NORETURN,
							  PSM2_INTERNAL_ERR,
							  "Unhandled/unknown close state %d",
							  ipsaddr->cstate_outgoing);
					break;
				}
			}
			if (numep_left == 0)
				break;

			if ((err =
			     psmi_err_only(psmi_poll_internal(proto->ep, 1))))
				goto fail;

			if (warning_secs && get_cycles() > t_warning) {
				_HFI_INFO
				    ("graceful close in progress for %d/%d peers "
				     "(elapsed=%d millisecs,timeout=%d millisecs,reqs=%lld)\n",
				     numep_left, numep_todisc,
				     (int)(cycles_to_nanosecs
					   (get_cycles() - t_start) / MSEC_ULL),
				     (int)(timeout_in / MSEC_ULL),
				     (unsigned long long)reqs_sent);
				t_warning =
				    get_cycles() +
				    nanosecs_to_cycles(warning_secs * SEC_ULL);
			}
		}
		while (timeout > get_cycles());

		if (numep_left > 0) {
			err = PSM2_TIMEOUT;
			for (i = 0; i < numep; i++) {
				if (!array_of_epaddr_mask[i])
					continue;
				if (array_of_errors[i] == PSM2_EPID_UNKNOWN) {
					array_of_errors[i] = PSM2_TIMEOUT;
					_HFI_CONNDBG
					    ("disc timeout on index %d, epaddr %s\n",
					     i,
					     psmi_epaddr_get_name
					     (array_of_epaddr[i]->epid));
				}
			}
			_HFI_PRDBG("graceful close incomplete for %d/%d peers "
				   "(elapsed=%d millisecs,timeout=%d millisecs,reqs=%lld)\n",
				   numep_left, numep_todisc,
				   (int)(cycles_to_nanosecs
					 (get_cycles() - t_start) / MSEC_ULL),
				   (int)(timeout_in / MSEC_ULL),
				   (unsigned long long)reqs_sent);
		} else
			_HFI_PRDBG
			    ("graceful close complete from %d peers in %d millisecs, reqs_sent=%lld\n",
			     numep_todisc,
			     (int)(cycles_to_nanosecs(get_cycles() - t_start) /
				   MSEC_ULL), (unsigned long long)reqs_sent);
	} else {
		psmi_assert_always(proto->msgflowid < EP_FLOW_LAST);
		for (n = 0; n < numep; n++) {
			i = (n_first + n) % numep;
			if (!array_of_epaddr_mask[i])
				continue;
			ipsaddr = (ips_epaddr_t *) array_of_epaddr[i];
			psmi_assert_always(ipsaddr->cstate_outgoing ==
					   CSTATE_ESTABLISHED);
			ips_proto_send_ctrl_message_request(proto, &ipsaddr->
						    flows[proto->msgflowid],
						    OPCODE_DISCONNECT_REQUEST,
						    &ipsaddr->ctrl_msg_queued,
						    0);
			/* Force state to DISCONNECTED */
			ipsaddr->cstate_outgoing = CSTATE_OUTGOING_DISCONNECTED;
			array_of_errors[i] = PSM2_OK;
		}
		_HFI_CONNDBG("non-graceful close complete from %d peers\n", numep);
	}

	for (i = 0; i < numep; i++) {
		if (!array_of_epaddr_mask[i] || array_of_errors[i] != PSM2_OK)
			continue;
		ipsaddr = (ips_epaddr_t *) array_of_epaddr[i];
		if (ipsaddr->cstate_outgoing == CSTATE_NONE)
			continue;
		psmi_assert_always(ipsaddr->cstate_outgoing ==
				   CSTATE_OUTGOING_DISCONNECTED);
		proto->num_connected_outgoing--;
		/* Remote disconnect req arrived already, remove this epid.  If it
		 * hasn't arrived yet, that's okay, we'll pick it up later and just
		 * mark our connect-to status as being "none". */
		if (ipsaddr->cstate_incoming == CSTATE_NONE) {
			ips_free_epaddr(array_of_epaddr[i], proto);
			array_of_epaddr[i] = NULL;
		} else
			ipsaddr->cstate_outgoing = CSTATE_NONE;
	}

fail:
success:
	return err;
}

int ips_proto_isconnected(ips_epaddr_t *ipsaddr)
{
	if (ipsaddr->cstate_outgoing == CSTATE_ESTABLISHED ||
	    ipsaddr->cstate_incoming == CSTATE_ESTABLISHED)
		return 1;
	else
		return 0;
}
