#ifdef PSM_VERBS
/*

  This file is provided under a dual BSD/GPLv2 license.  When using or
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2017 Intel Corporation.

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

  Copyright(c) 2017 Intel Corporation.

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

#include "verbs_hal.h"


static inline struct _hfp_verbs *get_psm_verbs_hi(void)
{
	return (struct _hfp_verbs*) psm3_hal_current_hal_instance;
}

static PSMI_HAL_INLINE int psm3_hfp_verbs_close_context(psm2_ep_t ep)
{
	psm3_ep_free_verbs(ep);
	return PSM_HAL_ERROR_OK;
}

/* Check NIC and context status, returns one of
 *
 * PSM2_OK: Port status is ok (or context not initialized yet but still "ok")
 * PSM2_OK_NO_PROGRESS: Cable pulled
 * PSM2_EP_NO_NETWORK: No network, no lid, ...
 * PSM2_EP_DEVICE_FAILURE: Chip failures, rxe/txe parity, etc.
 */
static PSMI_HAL_INLINE psm2_error_t psm3_hfp_verbs_context_check_status(struct ptl_ips *ptl)
{
	// TBD - we need to check NIC (ptl->ep) and QP status, especially link status
	// and call psm3_handle_error
	return PSM2_OK;
}

#ifdef PSM_FI
static PSMI_HAL_INLINE int psm3_hfp_verbs_faultinj_allowed(const char *name,
			psm2_ep_t ep)
{
	return 1;
}
#endif

static PSMI_HAL_INLINE int psm3_hfp_verbs_context_open(int unit,
				 int port, int addr_index,
				 uint64_t open_timeout,
				 psm2_ep_t ep,
				 psm2_uuid_t const job_key,
				 unsigned retryCnt)
{
	psm2_error_t err = PSM2_OK;

	psmi_assert_always(psm3_epid_zero_internal(ep->epid));
	// open verbs 1st so psm3_context_open can get pkey, lid, etc
	if ((err = psm3_ep_open_verbs(ep, unit, port, addr_index, job_key)) != PSM2_OK) {
		const char* unit_path = psm3_sysfs_unit_path(unit);
		_HFI_ERROR( "Unable to initialize verbs NIC %s (unit %d:%d)\n",
				unit_path ? unit_path : "NULL", unit, port);
		err = -PSM_HAL_ERROR_CANNOT_OPEN_CONTEXT;
		goto bail;
	}
	cpu_set_t mycpuset;
	if (psm3_sysfs_get_unit_cpumask(unit, &mycpuset)) {
		_HFI_ERROR( "Failed to get %s (unit %d) cpu set\n", ep->dev_name, unit);
		//err = -PSM_HAL_ERROR_GENERAL_ERROR;
		goto bail;
	}

	if (psm3_context_set_affinity(ep, mycpuset))
		goto bail;

// TBD - inside psm3_gen1_userinit_internal we would find CPU
// which HFI is closest to and set affinity.  Need a way to do that for UD
// we would also wash jkey through driver and stash it in _hfi_ctrl
// but because we disable this we won't have an _hfi_ctrl structure
	return PSM_HAL_ERROR_OK;

bail:
	 psm3_ep_free_verbs(ep);
	ep->epid = psm3_epid_zeroed_internal();

	return -PSM_HAL_ERROR_GENERAL_ERROR;
}

static PSMI_HAL_INLINE int psm3_hfp_verbs_get_port_index2pkey(psm2_ep_t ep, int index)
{
	return verbs_get_port_index2pkey(ep, index);
}

/* Tell the driver to change the way packets can generate interrupts.

 HFI1_POLL_TYPE_URGENT: Generate interrupt only when send with
			IPS_SEND_FLAG_INTR (HFI_KPF_INTR)
 HFI1_POLL_TYPE_ANYRCV: wakeup on any rcv packet (when polled on). [not used]

 PSM: Uses TYPE_URGENT in ips protocol
*/
static PSMI_HAL_INLINE int psm3_hfp_verbs_poll_type(uint16_t poll_type, psm2_ep_t ep)
{
	return psm3_verbs_poll_type(poll_type, ep);
}

// initialize HAL specific parts of ptl_ips
// This is called after most of the generic aspects have been initialized
// so we can use ptl->ep, ptl->ctl, etc as needed
// However it is called prior to ips_proto_init.  ips_proto_init requires some
// ips_ptl items such as ptl->spioc
static PSMI_HAL_INLINE psm2_error_t psm3_hfp_verbs_ips_ptl_init_pre_proto_init(struct ptl_ips *ptl)
{
	return psm3_verbs_ips_ptl_init_pre_proto_init(ptl);
}

// initialize HAL specific parts of ptl_ips
// This is called after after ips_proto_init and after most of the generic
// aspects of ips_ptl have been initialized
// so we can use ptl->ep and ptl->proto as needed
static PSMI_HAL_INLINE psm2_error_t psm3_hfp_verbs_ips_ptl_init_post_proto_init(struct ptl_ips *ptl)
{
	return psm3_verbs_ips_ptl_init_post_proto_init(ptl);
}

// finalize HAL specific parts of ptl_ips
// This is called before the generic aspects have been finalized
// but after ips_proto has been finalized
// so we can use ptl->ep as needed
static PSMI_HAL_INLINE psm2_error_t psm3_hfp_verbs_ips_ptl_fini(struct ptl_ips *ptl)
{
	return psm3_verbs_ips_ptl_fini(ptl);
}

// initialize HAL specific details in ips_proto.
// called after many of ips_proto parameters parsed and initialized
static PSMI_HAL_INLINE psm2_error_t psm3_hfp_verbs_ips_proto_init(
				struct ips_proto *proto, uint32_t cksum_sz)
{
	return psm3_verbs_ips_proto_init(proto, cksum_sz);
}

// Fetch current link state to update linkinfo fields in ips_proto:
// 	ep_base_lid, ep_lmc, ep_link_rate, QoS tables, CCA tables
// These are all fields which can change during a link bounce.
// Note "active" state is not adjusted as on link down PSM will wait for
// the link to become usable again so it's always a viable/active device
// afer initial PSM startup has selected devices.
// Called during initialization of ips_proto during ibta_init as well
// as during a link bounce.
// TBD - may be able to call this from HAL ips_proto_init as well as
// directly within HAL event processing, in which case this could
// be completely internal to HAL and not exposed in HAL API
static PSMI_HAL_INLINE psm2_error_t psm3_hfp_verbs_ips_proto_update_linkinfo(
				struct ips_proto *proto)
{
	return psm3_verbs_ips_proto_update_linkinfo(proto);
}

// Indicate if all underlying connections are now established
// (eg. RV connections)
// return:
//	0 - not yet connected
//	1 - connected (or nothing extra needed)
//	-1 - failure to check or connect (errno is status)
//		EIO is connection error other values are more serious
//		(invalid call, etc)
static PSMI_HAL_INLINE int psm3_hfp_verbs_ips_fully_connected(ips_epaddr_t *ipsaddr)
{
#ifdef RNDV_MOD
	int ret;

	/* ! rv_conn means we don't need a rv connection, otherwise
	 * return status of the connection
	 */
	if (! ipsaddr->verbs.rv_conn || ipsaddr->verbs.rv_connected)
		return 1;
	ret = psm3_rv_connected(ipsaddr->verbs.rv_conn);
	if (ret < 0 && errno != EIO) {
		int save_errno = errno;
		perror("can't query rv connection\n");
		errno = save_errno;
	}
	ipsaddr->verbs.rv_connected = (1 == ret);
	return ret;
#else /* RNDV_MOD */
	return 1;
#endif /* RNDV_MOD */
}

/* handle HAL specific connection processing as part of processing an
 * inbound PSM connect Request or Reply when connection not yet established
 * save the negotiated parameters
 */
static PSMI_HAL_INLINE psm2_error_t psm3_hfp_verbs_ips_ipsaddr_set_req_params(
				struct ips_proto *proto,
				ips_epaddr_t *ipsaddr,
				const struct ips_connect_reqrep *req)
{
#ifdef RNDV_MOD
	ipsaddr->verbs.remote_gid = req->verbs.gid;
	ipsaddr->verbs.remote_rv_index = req->verbs.rv_index;
	if (ipsaddr->verbs.rv_conn) {
		psmi_assert(IPS_PROTOEXP_FLAG_KERNEL_QP(proto->ep->rdmamode));
		psmi_assert(proto->ep->rv);
		if (!  psm3_nonzero_gid(&req->verbs.gid)) {
			_HFI_ERROR("mismatched PSM3_RDMA config, remote end not in mode 1\n");
			return PSM2_INTERNAL_ERR;
			// TBD - if we wanted to allow mismatched config to run in UD mode
			//psm3_rv_destroy_conn(ipsaddr->verbs.rv_conn);
			//ipsaddr->verbs.rv_conn = NULL;
		} else {
			// both sides are ready, so we can start rv_connect now
			if (! ipsaddr->pathgrp->pg_path[0][IPS_PATH_LOW_PRIORITY]->verbs.pr_connecting) {
				struct ib_user_path_rec path;
				_HFI_MMDBG("rv_connect to: %s\n",
					psm3_ibv_gid_fmt(ipsaddr->verbs.remote_gid, 0));
				// pg_path has negotiated pr_mtu and pr_static_rate
				psm3_verbs_ips_path_rec_to_ib_user_path_rec(proto->ep,
					ipsaddr->pathgrp->pg_path[0][IPS_PATH_LOW_PRIORITY],
 					&ipsaddr->verbs.remote_gid, &path);
				if (psm3_rv_connect(ipsaddr->verbs.rv_conn, &path)) {
					_HFI_ERROR("rv_connect failed: %s\n", strerror(errno));
					return PSM2_INTERNAL_ERR;
				}
				ipsaddr->pathgrp->pg_path[0][IPS_PATH_LOW_PRIORITY]->verbs.pr_connecting = 1;
			}
		}
	// } else if (psm3_nonzero_gid(&req->verbs.gid)) {
	//	 We could fail here, but we just let remote end decide
	//	_HFI_ERROR("mismatched PSM3_RDMA config, remote end in mode 1\n");
	//	return PSM2_INTERNAL_ERR;
	}
#endif // RNDV_MOD
#ifdef USE_RC
	if (ipsaddr->verbs.rc_qp) {
		psmi_assert(IPS_PROTOEXP_FLAG_USER_RC_QP(proto->ep->rdmamode));
#ifdef RNDV_MOD
		psmi_assert(proto->ep->rv
					|| proto->ep->mr_cache_mode != MR_CACHE_MODE_KERNEL);
#endif
		if (! req->verbs.qp_attr.qpn) {
			_HFI_ERROR("mismatched PSM3_RDMA config, remote end not in mode 2 or 3\n");
			return PSM2_INTERNAL_ERR;
			// TBD - if we wanted to allow mismatched config to run in UD mode
			//rc_qp_destroy(ipsaddr->verbs.rc_qp);
			//ipsaddr->verbs.rc_qp = NULL;
		} else {
			// we got a REQ or a REP, we can move to RTR
			// if we are only doing RDMA, we don't need any buffers, but we need a
			// pool object for RQ coallesce, so we create a pool with 0 size buffers
			if (PSM2_OK != psm_verbs_alloc_recv_pool(proto->ep, ipsaddr->verbs.rc_qp, &ipsaddr->verbs.recv_pool,
					min(proto->ep->verbs_ep.hfi_num_recv_wqes/VERBS_RECV_QP_FRACTION, ipsaddr->verbs.rc_qp_max_recv_wr),
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

			if (modify_rc_qp_to_init(proto->ep, ipsaddr->verbs.rc_qp)) {
				_HFI_ERROR("qp_to_init failed\n");
				return PSM2_INTERNAL_ERR;
			}
			if (PSM2_OK != psm3_ep_verbs_prepost_recv(&ipsaddr->verbs.recv_pool)) {
				_HFI_ERROR("prepost failed\n");
				return PSM2_INTERNAL_ERR;
			}
			// RC QP MTU will be set to min of req->verbs.qp_attr and pr_mtu
			// TBD - we already factored in req vs pr to update pr no need
			// for modify_cq_qp_to_rtr to repeat it
			if (modify_rc_qp_to_rtr(proto->ep, ipsaddr->verbs.rc_qp, &req->verbs.qp_attr,
					ipsaddr->pathgrp->pg_path[0][IPS_PATH_LOW_PRIORITY], //TBD path_rec
					req->initpsn)) {
				_HFI_ERROR("qp_to_rtr failed\n");
				return PSM2_INTERNAL_ERR;
			}
		}
	// } else if (req->verbs.qp_attr.qpn) {
	//	 We could fail here, but we just let remote end decide
	//	_HFI_ERROR("mismatched PSM3_RDMA config, remote end in mode 2 or 3\n");
	//	return PSM2_INTERNAL_ERR;
	}
#endif
	return PSM2_OK;
}

/* handle HAL specific connection processing as part of processing an
 * inbound PSM connect Reply which completes establishment of on outgoing
 * connection.
 */
static PSMI_HAL_INLINE psm2_error_t psm3_hfp_verbs_ips_ipsaddr_process_connect_reply(
				struct ips_proto *proto,
				ips_epaddr_t *ipsaddr,
				const struct ips_connect_reqrep *req)
{
#ifdef USE_RC
	if (ipsaddr->verbs.rc_qp) {
		psmi_assert(IPS_PROTOEXP_FLAG_USER_RC_QP(proto->ep->rdmamode));
		psmi_assert(req->verbs.qp_attr.qpn); // checked in set_req_params
		// we got a a REP, we can move to RTS
		if (modify_rc_qp_to_rts(proto->ep, ipsaddr->verbs.rc_qp,
			&req->verbs.qp_attr, proto->runid_key)) { // initpsn we sent
			_HFI_ERROR("qp_to_rts failed\n");
			return PSM2_INTERNAL_ERR;
		}
		if ((proto->ep->rdmamode&IPS_PROTOEXP_FLAG_RDMA_MASK) == IPS_PROTOEXP_FLAG_RDMA_USER_RC) {
			// use RC QPs for eager and RDMA
			// now we can use our own send Q and send allocator
			ipsaddr->verbs.use_allocator =  &ipsaddr->verbs.send_allocator;
			ipsaddr->verbs.use_qp =  ipsaddr->verbs.rc_qp;
			ipsaddr->verbs.use_max_inline_data = ipsaddr->verbs.rc_qp_max_inline_data;
			_HFI_MMDBG("RC enabled\n");
		}
		ipsaddr->verbs.rc_connected = 1;
	}
#endif
	return PSM2_OK;
}

/* build HAL specific portion of an outbound PSM connect message
 * for PSM Connect or Disconnect Request or Reply
 */
static PSMI_HAL_INLINE void psm3_hfp_verbs_ips_proto_build_connect_message(
			struct ips_proto *proto,
			ips_epaddr_t *ipsaddr, uint8_t opcode,
			struct ips_connect_reqrep *req)
{
	switch (opcode) {
	case OPCODE_CONNECT_REPLY:
	case OPCODE_CONNECT_REQUEST:
		memset(&req->verbs.reserved, 0, sizeof(req->verbs.reserved));
#ifdef RNDV_MOD
		// only supply gid if we want to use kernel rv
		if (IPS_PROTOEXP_FLAG_KERNEL_QP(proto->ep->rdmamode)
				&& proto->ep->rv) {
			req->verbs.gid = proto->ep->verbs_ep.lgid;
			req->verbs.rv_index = proto->ep->verbs_ep.rv_index;
		} else
#endif
		{
			memset(&req->verbs.gid, 0, sizeof(req->verbs.gid));
			req->verbs.rv_index = 0;
		}
#if defined(USE_RC)
		if (ipsaddr->verbs.rc_qp) {
			psmi_assert(IPS_PROTOEXP_FLAG_USER_RC_QP(proto->ep->rdmamode));
			req->initpsn = proto->runid_key;// pid, not ideal, better than const
			req->verbs.qp_attr.qpn = ipsaddr->verbs.rc_qp->qp_num;
			req->verbs.qp_attr.mtu = opa_mtu_int_to_enum(req->mtu);
			req->verbs.qp_attr.srq = 0;
			req->verbs.qp_attr.resv = 0;
			req->verbs.qp_attr.target_ack_delay = 0; // TBD; - from local device
			req->verbs.qp_attr.resv2 = 0;
			req->verbs.qp_attr.responder_resources = 0;
			req->verbs.qp_attr.initiator_depth = 0;
			memset(&req->verbs.qp_attr.resv3, 0, sizeof(req->verbs.qp_attr.resv3));
		} else
#endif // USE_RC
			memset(&req->verbs.qp_attr, 0, sizeof(req->verbs.qp_attr));
		break;
	case OPCODE_DISCONNECT_REQUEST:
	case OPCODE_DISCONNECT_REPLY:
		// placeholder, but typically nothing to be done
		// as the ips_connect_hdr is sufficient
		break;
	default:
		psmi_assert_always(0);
		break;
	}
}

/* handle HAL specific ipsaddr initialization for addressing, including
 * parts of ipsaddr needed for path record query
 * For ipsaddr created just for a disconnect, ips_ipsaddr_init_connections
 * is not called. In which case ips_ipsaddr_init_addressing and ips_flow_init
 * need to do what is needed to allow spio_transfer_frame to send the
 * disconnect control packet.
 */
static PSMI_HAL_INLINE void psm3_hfp_verbs_ips_ipsaddr_init_addressing(
			struct ips_proto *proto, psm2_epid_t epid,
			ips_epaddr_t *ipsaddr, uint16_t *lidp
			, psmi_gid128_t *gidp
			)
{
	ipsaddr->verbs.remote_qpn = psm3_epid_context(epid);
	ipsaddr->hash = ipsaddr->verbs.remote_qpn;

	psm3_epid_get_av(epid, lidp, gidp);
	_HFI_CONNDBG("qpn=0x%x lid=0x%x GID=%s\n",
			ipsaddr->verbs.remote_qpn, *lidp,
			psm3_gid128_fmt(*gidp, 0));
}

/* handle HAL specific ipsaddr initialization for any HAL specific connections
 * underlying the ipsaddr (RC QPs, TCP sockets, etc)
 * This is not called for an ipsaddr created just for a disconnect.  In which
 * case ips_ipsaddr_init_addressing and ips_flow_init need to do what is
 * needed to allow spio_transfer_frame to send the disconnect control packet.
 */
static PSMI_HAL_INLINE psm2_error_t psm3_hfp_verbs_ips_ipsaddr_init_connections(
			struct ips_proto *proto, psm2_epid_t epid,
			ips_epaddr_t *ipsaddr)
{
	psm2_error_t err = PSM2_OK;

#ifdef USE_RC
	if (IPS_PROTOEXP_FLAG_USER_RC_QP(proto->ep->rdmamode)
#ifdef RNDV_MOD
		// if verbs_ep allows us to open w/o rv_open then we can't use RC QP
		&& (proto->ep->rv
			|| proto->ep->mr_cache_mode != MR_CACHE_MODE_KERNEL)
#endif
		) {
		struct ibv_qp_cap qp_cap;
		ipsaddr->verbs.rc_qp = rc_qp_create(proto->ep, ipsaddr, &qp_cap);
		if (! ipsaddr->verbs.rc_qp) {
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
		ipsaddr->verbs.rc_qp_max_recv_wr = qp_cap.max_recv_wr;
		ipsaddr->verbs.rc_qp_max_inline_data = qp_cap.max_inline_data;
		if (PSM2_OK != psm_verbs_init_send_allocator(&ipsaddr->verbs.send_allocator,
							&proto->ep->verbs_ep.send_pool)) {
			_HFI_ERROR("can't init RC QP send allocator\n");
			err = PSM2_INTERNAL_ERR;
			goto fail;
		}
	}
	// until our QP is connected, use the UD QP's allocator and inline
	ipsaddr->verbs.use_allocator =  &proto->ep->verbs_ep.send_allocator;
	ipsaddr->verbs.use_qp =  proto->ep->verbs_ep.qp;
	ipsaddr->verbs.use_max_inline_data = proto->ep->verbs_ep.qp_cap.max_inline_data;
#endif

#ifdef RNDV_MOD
	if (IPS_PROTOEXP_FLAG_KERNEL_QP(proto->ep->rdmamode)
			&& proto->ep->rv) {
		struct ibv_ah_attr ah_attr;

		ipsaddr->verbs.rv_connected = 0; // redundant since we calloc above
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
		if (ipsaddr->pathgrp->pg_path[0][IPS_PATH_LOW_PRIORITY]->verbs.pr_rv_conn) {
			ipsaddr->verbs.rv_conn = ipsaddr->pathgrp->pg_path[0][IPS_PATH_LOW_PRIORITY]->verbs.pr_rv_conn;
		} else {
			err = psm3_verbs_ips_path_rec_to_ah_attr(proto->ep,
				ipsaddr->pathgrp->pg_path[0][IPS_PATH_LOW_PRIORITY], // ????
				&ah_attr);
			if (PSM2_OK != err) {
				_HFI_ERROR("unable to get ah from path\n");
				goto fail;
			}
			// low 64b of NID is sufficient, IPv6 will use ah_attr
			ipsaddr->verbs.rv_conn = psm3_rv_create_conn(proto->ep->rv,
							&ah_attr,
							psm3_epid_get_rem_addr(epid));
			ipsaddr->pathgrp->pg_path[0][IPS_PATH_LOW_PRIORITY]->verbs.pr_rv_conn = ipsaddr->verbs.rv_conn;
			if (! ipsaddr->verbs.rv_conn) {
				_HFI_ERROR("rv_create_conn failed: %s\n", strerror(errno));
				err = PSM2_INTERNAL_ERR;
//TBD - should we make this non-fatal?  Just regress to UD mode and output ERROR
				goto fail;
			}
		}
	}
#endif // RNDV_MOD

	return err;

#if defined(USE_RC) || defined(RNDV_MOD)
fail:
#endif
#ifdef USE_RC
        if (ipsaddr->verbs.rc_qp) {
                rc_qp_destroy(ipsaddr->verbs.rc_qp);
                ipsaddr->verbs.rc_qp = NULL;
        }
#endif
	return err;
}

/* handle HAL specific ipsaddr free for any HAL specific information
 * in ipsaddr (from ipsaddr_init_*, set_req_params, etc
 */
static PSMI_HAL_INLINE void psm3_hfp_verbs_ips_ipsaddr_free(
			ips_epaddr_t *ipsaddr, struct ips_proto *proto)
{
#ifdef RNDV_MOD
	_HFI_MMDBG("free_epaddr\n");
	if (ipsaddr->verbs.rv_conn) {
		//psm3_rv_destroy_conn(ipsaddr->verbs.rv_conn);
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
#ifdef USE_RC
	if (ipsaddr->verbs.rc_qp) {
		rc_qp_destroy(ipsaddr->verbs.rc_qp);
		ipsaddr->verbs.rc_qp = NULL;
	}
	psm_verbs_free_recv_pool(&ipsaddr->verbs.recv_pool);
#endif
}

/* handle HAL specific ips_flow initialization
 */
static PSMI_HAL_INLINE void psm3_hfp_verbs_ips_flow_init(
			struct ips_flow *flow, struct ips_proto *proto)
{
	psmi_assert(flow->transfer == PSM_TRANSFER_PIO);
	flow->flush = psm3_ips_proto_flow_flush_pio;

	_HFI_CONNDBG("[ipsaddr=%p] UD flow->frag_size: %u = min("
		"proto->epinfo.ep_mtu(%u), flow->path->pr_mtu(%u))\n",
		flow->ipsaddr, flow->frag_size, proto->epinfo.ep_mtu,
		flow->path->pr_mtu);
}

/* handle HAL specific connection processing as part of processing an
 * outbound PSM disconnect Request or Reply or an inbound disconnect request
 */
static PSMI_HAL_INLINE void psm3_hfp_verbs_ips_ipsaddr_disconnect(
			struct ips_proto *proto, ips_epaddr_t *ipsaddr)
{
#ifdef USE_RC
	// This is redundant if transfer_frame uses UD for all
	// control messages, but it also makes sure we stop using
	// RC for any non-control messages (should be none) after disconnect.
	// Use the UD QP's allocator and inline now and going forward
	ipsaddr->verbs.use_allocator =  &proto->ep->verbs_ep.send_allocator;
	ipsaddr->verbs.use_qp =  proto->ep->verbs_ep.qp;
	ipsaddr->verbs.use_max_inline_data = proto->ep->verbs_ep.qp_cap.max_inline_data;
	_HFI_MMDBG("RC discon\n");
	// we let ipsaddr free destroy the QP and it's buffers
#endif
}

/* Handle HAL specific initialization of ibta path record query, CCA
 * and dispersive routing
 */
static PSMI_HAL_INLINE psm2_error_t psm3_hfp_verbs_ips_ibta_init(
				struct ips_proto *proto)
{
	psm2_error_t err = PSM2_OK;
	proto->flags |= IPS_PROTO_FLAG_PPOLICY_ADAPTIVE;

	return err;

}

/* Handle HAL specific initialization of an ips_path_rec
 * as part of fetching or hand building a path record.
 * Responsible for all fields in the HAL specific union and any tweaks to
 * other fields which may be HAL specific (such as pr_mtu).
 * response is only provided when we are building a ips_path_rec from a
 * fetched ibta_path_rec.  Otherwise we are building it solely based on
 * our own end point and what our caller knows from the EPID.
 */
static PSMI_HAL_INLINE psm2_error_t psm3_hfp_verbs_ips_path_rec_init(
				struct ips_proto *proto,
				struct ips_path_rec *path_rec,
				struct _ibta_path_rec *response)
{
	psm2_error_t err = PSM2_OK;
	err = psm3_verbs_ips_path_rec_init(proto, path_rec, response);
	return err;
}

static PSMI_HAL_INLINE psm2_error_t psm3_hfp_verbs_ips_ptl_pollintr(
		psm2_ep_t ep, struct ips_recvhdrq *recvq,
		int fd_pipe, int next_timeout,
		uint64_t *pollok, uint64_t *pollcyc)
{
	return psm3_verbs_ips_ptl_pollintr(ep, recvq, fd_pipe,
					 next_timeout, pollok, pollcyc);
}

#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
static PSMI_HAL_INLINE void psm3_hfp_verbs_gdr_close(void)
{
}
static PSMI_HAL_INLINE void* psm3_hfp_verbs_gdr_convert_gpu_to_host_addr(unsigned long buf,
                                size_t size, int flags,
                                psm2_ep_t ep)
{
	return psm3_verbs_gdr_convert_gpu_to_host_addr(buf, size, flags,
                                ep);
}
#ifdef PSM_ONEAPI
static PSMI_HAL_INLINE void psm3_hfp_verbs_gdr_munmap_gpu_to_host_addr(unsigned long buf,
                                size_t size, int flags,
                                psm2_ep_t ep)
{
	return psm3_verbs_gdr_munmap_gpu_to_host_addr(buf, size, flags,
                                ep);
}
#endif
#endif /* PSM_CUDA || PSM_ONEAPI */

#include "verbs_spio.c"

static PSMI_HAL_INLINE psm2_error_t psm3_hfp_verbs_spio_transfer_frame(struct ips_proto *proto,
					struct ips_flow *flow, struct ips_scb *scb,
					uint32_t *payload, uint32_t length,
					uint32_t isCtrlMsg, uint32_t cksum_valid,
					uint32_t cksum
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
				, uint32_t is_gpu_payload
#endif
	)
{
	return psm3_verbs_spio_transfer_frame(proto, flow, scb,
					 payload, length, isCtrlMsg,
					 cksum_valid, cksum
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
				, is_gpu_payload
#endif
	);
}

static PSMI_HAL_INLINE psm2_error_t psm3_hfp_verbs_transfer_frame(struct ips_proto *proto,
					struct ips_flow *flow, struct ips_scb *scb,
					uint32_t *payload, uint32_t length,
					uint32_t isCtrlMsg, uint32_t cksum_valid,
					uint32_t cksum
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
				, uint32_t is_gpu_payload
#endif
	)
{
	return psm3_verbs_spio_transfer_frame(proto, flow, scb,
					 payload, length, isCtrlMsg,
					 cksum_valid, cksum
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
				, is_gpu_payload
#endif
	);
}

static PSMI_HAL_INLINE psm2_error_t psm3_hfp_verbs_drain_sdma_completions(struct ips_proto *proto)
{
	return psm3_verbs_completion_update(proto->ep, 1);
}

static PSMI_HAL_INLINE int psm3_hfp_verbs_get_node_id(int unit, int *nodep)
{
	int64_t node_id = psm3_sysfs_unit_read_node_s64(unit);
	*nodep = (int)node_id;
	if (node_id != -1)
		return PSM_HAL_ERROR_OK;
	else
		return -PSM_HAL_ERROR_GENERAL_ERROR;
}

#endif /* PSM_VERBS */
