#ifdef PSM_SOCKETS
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

#include "sockets_hal.h"


static inline struct _hfp_sockets *get_psm_sockets_hi(void)
{
	return (struct _hfp_sockets*) psm3_hal_current_hal_instance;
}

static PSMI_HAL_INLINE int psm3_hfp_sockets_close_context(psm2_ep_t ep)
{
	 psm3_ep_free_sockets(ep);

	return PSM_HAL_ERROR_OK;
}

/* Check NIC and context status, returns one of
 *
 * PSM2_OK: Port status is ok (or context not initialized yet but still "ok")
 * PSM2_OK_NO_PROGRESS: Cable pulled
 * PSM2_EP_NO_NETWORK: No network, no lid, ...
 * PSM2_EP_DEVICE_FAILURE: Chip failures, rxe/txe parity, etc.
 */
static PSMI_HAL_INLINE psm2_error_t psm3_hfp_sockets_context_check_status(struct ptl_ips *ips)
{
	// TBD - we need to check NIC status (ptl->ep), especially link status
	// and call psm3_handle_error
	return PSM2_OK;
}

#ifdef PSM_FI
static PSMI_HAL_INLINE int psm3_hfp_sockets_faultinj_allowed(const char *name,
			psm2_ep_t ep)
{
	// The revclost fault injection in ips_proto_help.h is N/A to
	// TCP mode since we assume a reliable TCP transport
	if (strcmp(name, "recvlost") == 0
		&& (! ep || ep->sockets_ep.sockets_mode == PSM3_SOCKETS_TCP))
		return 0;
	return 1;
}
#endif

static PSMI_HAL_INLINE int psm3_hfp_sockets_context_open(int unit,
				 int port, int addr_index,
				 uint64_t open_timeout,
				 psm2_ep_t ep,
				 psm2_uuid_t const job_key,
				 unsigned retryCnt)
{
	psm2_error_t err = PSM2_OK;

	psmi_assert_always(psm3_epid_zero_internal(ep->epid));
	// open udp 1st so psm3_context_open can get pkey, lid, etc
	if ((err = psm3_ep_open_sockets(ep, unit, port, addr_index, job_key)) != PSM2_OK) {
		const char* unit_path = psm3_sysfs_unit_path(unit);
		_HFI_ERROR( "Unable to initialize sockets NIC %s (unit %d:%d)\n",
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
// which HFI is closest to and set affinity.  Need a way to do that for sockets
// we would also wash jkey through driver and stash it in _hfi_ctrl
// but because we disable this we won't have an _hfi_ctrl structure
	return PSM_HAL_ERROR_OK;

bail:
	psm3_ep_free_sockets(ep);
	ep->epid = psm3_epid_zeroed_internal();

	return -PSM_HAL_ERROR_GENERAL_ERROR;
}

static PSMI_HAL_INLINE int psm3_hfp_sockets_get_port_index2pkey(psm2_ep_t ep, int index)
{
	return 0x8001;
}

/* Tell the driver to change the way packets can generate interrupts.

 HFI1_POLL_TYPE_URGENT: Generate interrupt only when send with
			IPS_SEND_FLAG_INTR (HFI_KPF_INTR)
 HFI1_POLL_TYPE_ANYRCV: wakeup on any rcv packet (when polled on). [not used]

 PSM: Uses TYPE_URGENT in ips protocol
*/
static PSMI_HAL_INLINE int psm3_hfp_sockets_poll_type(uint16_t poll_type, psm2_ep_t ep)
{
	// TBD - UDP does not yet implement urgent interrupt wakeup for
	// rcvThread.  TBD if we can do that with sockets
	//return psm3_sockets_poll_type(poll_type, ep);
	return 0;
}

// initialize HAL specific parts of ptl_ips
// This is called after most of the generic aspects have been initialized
// so we can use ptl->ep, ptl->ctl, etc as needed
// However it is called prior to ips_proto_init.  ips_proto_init requires some
// ips_ptl items such as ptl->spioc
static PSMI_HAL_INLINE psm2_error_t psm3_hfp_sockets_ips_ptl_init_pre_proto_init(struct ptl_ips *ptl)
{
	return psm3_sockets_ips_ptl_init_pre_proto_init(ptl);
}

// initialize HAL specific parts of ptl_ips
// This is called after after ips_proto_init and after most of the generic
// aspects of ips_ptl have been initialized
// so we can use ptl->ep and ptl->proto as needed
static PSMI_HAL_INLINE psm2_error_t psm3_hfp_sockets_ips_ptl_init_post_proto_init(struct ptl_ips *ptl)
{
	return psm3_sockets_ips_ptl_init_post_proto_init(ptl);
}

// finalize HAL specific parts of ptl_ips
// This is called before the generic aspects have been finalized
// but after ips_proto has been finalized
// so we can use ptl->ep as needed
static PSMI_HAL_INLINE psm2_error_t psm3_hfp_sockets_ips_ptl_fini(struct ptl_ips *ptl)
{
	return psm3_sockets_ips_ptl_fini(ptl);
}

// initialize HAL specific details in ips_proto.
// called after many of ips_proto parameters parsed and initialized
static PSMI_HAL_INLINE psm2_error_t psm3_hfp_sockets_ips_proto_init(
				struct ips_proto *proto, uint32_t cksum_sz)
{
	return psm3_sockets_ips_proto_init(proto, cksum_sz);
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
static PSMI_HAL_INLINE psm2_error_t psm3_hfp_sockets_ips_proto_update_linkinfo(
				struct ips_proto *proto)
{
	return psm3_sockets_ips_proto_update_linkinfo(proto);
}

// Indicate if all underlying connections are now established
// (eg. RV connections)
// return:
//	0 - not yet connected
//	1 - connected (or nothing extra needed)
//	-1 - failure to check or connect (errno is status)
//		EIO is connection error other values are more serious
//		(invalid call, etc)
static PSMI_HAL_INLINE int psm3_hfp_sockets_ips_fully_connected(ips_epaddr_t *ipsaddr)
{
	return 1;
}

/* handle HAL specific connection processing as part of processing an
 * inbound PSM connect Request or Reply when connection not yet established
 * save the negotiated parameters
 */
static PSMI_HAL_INLINE psm2_error_t psm3_hfp_sockets_ips_ipsaddr_set_req_params(
				struct ips_proto *proto,
				ips_epaddr_t *ipsaddr,
				const struct ips_connect_reqrep *req)
{
	if (proto->ep->sockets_ep.sockets_mode == PSM3_SOCKETS_TCP) {
		// only maintain one socket between 2 peers, and we pick the one has
		// "lower" ip addr
		if (!ipsaddr->sockets.connected ||
			(proto->ep->sockets_ep.tcp_incoming_fd &&
			ipsaddr->sockets.tcp_fd != proto->ep->sockets_ep.tcp_incoming_fd &&
			psm3_epid_cmp_internal(ipsaddr->epaddr.epid, proto->ep->epid) == -1)) {
			psm3_sockets_tcp_close_fd(proto->ep, ipsaddr->sockets.tcp_fd, -1,
				&ipsaddr->flows[EP_FLOW_GO_BACK_N_PIO]);
			_HFI_VDBG("Replace fd=%d with %d\n", ipsaddr->sockets.tcp_fd,
				proto->ep->sockets_ep.tcp_incoming_fd);
			ipsaddr->sockets.tcp_fd = proto->ep->sockets_ep.tcp_incoming_fd;
			ipsaddr->sockets.connected = 1;
		}
	}
	return PSM2_OK;
}

/* handle HAL specific connection processing as part of processing an
 * inbound PSM connect Reply which completes establishment of on outgoing
 * connection.
 */
static PSMI_HAL_INLINE psm2_error_t psm3_hfp_sockets_ips_ipsaddr_process_connect_reply(
				struct ips_proto *proto,
				ips_epaddr_t *ipsaddr,
				const struct ips_connect_reqrep *req)
{
	return PSM2_OK;
}

/* build HAL specific portion of an outbound PSM connect message
 * for PSM Connect or Disconnect Request or Reply
 */
static PSMI_HAL_INLINE void psm3_hfp_sockets_ips_proto_build_connect_message(
			struct ips_proto *proto,
			ips_epaddr_t *ipsaddr, uint8_t opcode,
			struct ips_connect_reqrep *req)
{
	switch (opcode) {
	case OPCODE_CONNECT_REPLY:
	case OPCODE_CONNECT_REQUEST:
		memset(req->hal_pad, 0, sizeof(req->hal_pad));
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
static PSMI_HAL_INLINE void psm3_hfp_sockets_ips_ipsaddr_init_addressing(
			struct ips_proto *proto, psm2_epid_t epid,
			ips_epaddr_t *ipsaddr, uint16_t *lidp
			, psmi_gid128_t *gidp
			)
{
	psm3_epid_build_sockaddr(&ipsaddr->sockets.remote_pri_addr, epid,
					proto->ep->sockets_ep.if_index);
	ipsaddr->hash = ipsaddr->sockets.remote_pri_addr.sin6_port;
	psm3_epid_get_av(epid, lidp, gidp);
	if (proto->ep->sockets_ep.sockets_mode == PSM3_SOCKETS_TCP) {
		psm3_epid_build_aux_sockaddr(&ipsaddr->sockets.remote_aux_addr, epid,
					proto->ep->sockets_ep.if_index);
		_HFI_CONNDBG("TCP=%s UDP=%s\n",
			psm3_sockaddr_fmt((struct sockaddr *)&ipsaddr->sockets.remote_pri_addr, 0),
			psm3_sockaddr_fmt((struct sockaddr *)&ipsaddr->sockets.remote_aux_addr, 1));
	} else {
		_HFI_CONNDBG("UDP=%s\n",
			psm3_sockaddr_fmt((struct sockaddr *)&ipsaddr->sockets.remote_pri_addr, 0));
	}
}

/* handle HAL specific ipsaddr initialization for any HAL specific connections
 * underlying the ipsaddr (RC QPs, TCP sockets, etc)
 * This is not called for an ipsaddr created just for a disconnect.  In which
 * case ips_ipsaddr_init_addressing and ips_flow_init need to do what is
 * needed to allow spio_transfer_frame to send the disconnect control packet.
 */
static PSMI_HAL_INLINE psm2_error_t psm3_hfp_sockets_ips_ipsaddr_init_connections(
			struct ips_proto *proto, psm2_epid_t epid,
			ips_epaddr_t *ipsaddr)
{
	psm2_error_t err = PSM2_OK;

	if (proto->ep->sockets_ep.sockets_mode == PSM3_SOCKETS_TCP) {
		// TBD - can use ipsaddr->pathgrp->pg_path[0][IPS_PATH_LOW_PRIORITY]->pr_static_rate to set SO_MAX_PACING_RATE
		if (proto->ep->sockets_ep.tcp_incoming_fd > 0) {
			// reuse the incoming fd for data transmission
			ipsaddr->sockets.tcp_fd = proto->ep->sockets_ep.tcp_incoming_fd;
			ipsaddr->sockets.connected = 1;
			proto->ep->sockets_ep.tcp_incoming_fd = 0;
			PSM2_LOG_MSG("connected to %s fd=%d", psm3_epid_fmt_internal(epid, 0), ipsaddr->sockets.tcp_fd);
		} else {
			ipsaddr->sockets.tcp_fd = socket(AF_INET6, SOCK_STREAM | SOCK_NONBLOCK, 0);
			// 0 is a valid fd, but we treat 0 as an uninitialized value, so make
			// sure we didn't end up with 0 (stdin should have 0)
			if (ipsaddr->sockets.tcp_fd == 0) {
				// create a copy of the file descriptor using the unused one
				ipsaddr->sockets.tcp_fd = dup(ipsaddr->sockets.tcp_fd);
				close(0);
			}
			if (ipsaddr->sockets.tcp_fd == -1) {
				_HFI_ERROR( "Unable to create TCP tx socket for %s: %s\n", proto->ep->dev_name, strerror(errno));
				err = PSM2_INTERNAL_ERR;
				goto fail;
			}

			if (psm3_tune_tcp_socket("socket", proto->ep, ipsaddr->sockets.tcp_fd)) {
				_HFI_ERROR("unable to tune socket for connection to %s for %s: %s\n",
					psm3_sockaddr_fmt((struct sockaddr *)&ipsaddr->sockets.remote_pri_addr, 0),
					proto->ep->dev_name, strerror(errno));
				err = PSM2_INTERNAL_ERR;
				goto fail;
			}
		}
	}
	return err;

fail:
	if (ipsaddr->sockets.tcp_fd > 0) {
		close(ipsaddr->sockets.tcp_fd);
		ipsaddr->sockets.tcp_fd = 0;
	}
	return err;
}

/* handle HAL specific ipsaddr free for any HAL specific information
 * in ipsaddr (from ipsaddr_init_*, set_req_params, etc
 */
static PSMI_HAL_INLINE void psm3_hfp_sockets_ips_ipsaddr_free(
			ips_epaddr_t *ipsaddr, struct ips_proto *proto)
{
	if (proto->ep->sockets_ep.sockets_mode == PSM3_SOCKETS_TCP) {
		if (ipsaddr->sockets.tcp_fd > 0) {
			psm3_sockets_tcp_close_fd(proto->ep, ipsaddr->sockets.tcp_fd, -1, &ipsaddr->flows[EP_FLOW_GO_BACK_N_PIO]);
			ipsaddr->sockets.tcp_fd = 0;
		}
	}
}

/* handle HAL specific ips_flow initialization
 */
static PSMI_HAL_INLINE void psm3_hfp_sockets_ips_flow_init(
			struct ips_flow *flow, struct ips_proto *proto)
{
	psmi_assert(flow->transfer == PSM_TRANSFER_PIO);
#ifndef PSM_TCP_SINGLE_SND
	if (proto->ep->sockets_ep.sockets_mode == PSM3_SOCKETS_TCP) {
		flow->flush = psm3_tcp_proto_flow_flush_pio;
	} else
#endif
	{
		flow->flush = psm3_ips_proto_flow_flush_pio;
	}

	flow->send_remaining = 0;
	flow->used_snd_buff = 0;

	_HFI_CONNDBG("[ipsaddr=%p] %s flow->frag_size: %u = min("
		"proto->epinfo.ep_mtu(%u), flow->path->pr_mtu(%u))\n",
		flow->ipsaddr, 
		(proto->ep->sockets_ep.sockets_mode == PSM3_SOCKETS_TCP)?"TCP":"UDP",
		flow->frag_size, proto->epinfo.ep_mtu, flow->path->pr_mtu);
}

/* handle HAL specific connection processing as part of processing an
 * outbound PSM disconnect Request or Reply or an inbound disconnect request
 */
static PSMI_HAL_INLINE void psm3_hfp_sockets_ips_ipsaddr_disconnect(
			struct ips_proto *proto, ips_epaddr_t *ipsaddr)
{
}

/* Handle HAL specific initialization of ibta path record query, CCA
 * and dispersive routing
 */
static PSMI_HAL_INLINE psm2_error_t psm3_hfp_sockets_ips_ibta_init(
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
static PSMI_HAL_INLINE psm2_error_t psm3_hfp_sockets_ips_path_rec_init(
				struct ips_proto *proto,
				struct ips_path_rec *path_rec,
				struct _ibta_path_rec *response)
{
	psm2_error_t err = PSM2_OK;
	psmi_assert(! response);	/* path_rec query N/A */
	return err;
}

static PSMI_HAL_INLINE psm2_error_t psm3_hfp_sockets_ips_ptl_pollintr(
		psm2_ep_t ep, struct ips_recvhdrq *recvq,
		int fd_pipe, int next_timeout,
		uint64_t *pollok, uint64_t *pollcyc)
{
	return psm3_sockets_ips_ptl_pollintr(ep, recvq, fd_pipe,
					 next_timeout, pollok, pollcyc);
}

#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
static PSMI_HAL_INLINE void psm3_hfp_sockets_gdr_close(void)
{
}
static PSMI_HAL_INLINE void* psm3_hfp_sockets_gdr_convert_gpu_to_host_addr(unsigned long buf,
                                size_t size, int flags,
                                psm2_ep_t ep)
{
	return psm3_sockets_gdr_convert_gpu_to_host_addr(buf, size, flags,
                                ep);
}
#ifdef PSM_ONEAPI
static PSMI_HAL_INLINE void psm3_hfp_sockets_gdr_munmap_gpu_to_host_addr(unsigned long buf,
                                size_t size, int flags,
                                psm2_ep_t ep)
{
	return psm3_sockets_gdr_munmap_gpu_to_host_addr(buf, size, flags,
                                ep);
}
#endif
#endif /* PSM_CUDA || PSM_ONEAPI */

#include "sockets_spio.c"

static PSMI_HAL_INLINE psm2_error_t psm3_hfp_sockets_spio_transfer_frame(struct ips_proto *proto,
					struct ips_flow *flow, struct ips_scb *scb,
					uint32_t *payload, uint32_t length,
					uint32_t isCtrlMsg, uint32_t cksum_valid,
					uint32_t cksum
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
				, uint32_t is_gpu_payload
#endif
	)
{
#ifdef USE_UDP
	// This is a trade-off.  We could avoid this if-test if we limit
	// all sockets NICs in a job to the same PSM3_SOCKETS setting and
	// then we could set our HAL function table to a TCP or UDP
	// specific spio_transfer_frame function based on PSM3_SOCKETS at the time
	// of HAL initialize.  If built with in-line HAL functions
	// would need to save off these few functions (or adjust psm2_hal.h
	// to have the function table entries) and then use them in our
	// sockets_inline_i.h when built with 1 HAL.
	// It is assumed that for sockets message rate, the extra if will not
	// make a noticible difference, so this approach is chosen as easier to
	// understand and maintain.
	if_pf (proto->ep->sockets_ep.sockets_mode == PSM3_SOCKETS_UDP)
		return psm3_sockets_udp_spio_transfer_frame(proto, flow, scb,
					payload, length, isCtrlMsg,
					cksum_valid, cksum
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
					, is_gpu_payload
#endif
					);
	else
#endif /* USE_UDP */
		return psm3_sockets_tcp_spio_transfer_frame(proto, flow, scb,
					payload, length, isCtrlMsg,
					cksum_valid, cksum
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
					, is_gpu_payload
#endif
					);
}

static PSMI_HAL_INLINE psm2_error_t psm3_hfp_sockets_transfer_frame(struct ips_proto *proto,
					struct ips_flow *flow, struct ips_scb *scb,
					uint32_t *payload, uint32_t length,
					uint32_t isCtrlMsg, uint32_t cksum_valid,
					uint32_t cksum
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
				, uint32_t is_gpu_payload
#endif
	)
{
	return psm3_hfp_sockets_spio_transfer_frame(proto, flow, scb,
					payload, length, isCtrlMsg,
					cksum_valid, cksum
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
					, is_gpu_payload
#endif
					);
}

static PSMI_HAL_INLINE psm2_error_t psm3_hfp_sockets_drain_sdma_completions(struct ips_proto *proto)
{
	// should never be called because never start a sockets SDMA
	// But, may be applicable in future if implement zerocopy sockets
	psmi_assert_always(0);
	return PSM2_INTERNAL_ERR;
}

static PSMI_HAL_INLINE int psm3_hfp_sockets_get_node_id(int unit, int *nodep)
{
	int64_t node_id = psm3_sysfs_unit_read_node_s64(unit);
	*nodep = (int)node_id;
	if (node_id != -1)
		return PSM_HAL_ERROR_OK;
	else
		return -PSM_HAL_ERROR_GENERAL_ERROR;
}

#endif /* PSM_SOCKETS */
