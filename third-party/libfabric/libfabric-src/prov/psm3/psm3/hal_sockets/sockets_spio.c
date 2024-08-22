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

/* Copyright (c) 2003-2017 Intel Corporation. All rights reserved. */
#ifndef _SOCKETS_SPIO_C_
#define _SOCKETS_SPIO_C_

/* included header files  */
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <sched.h>
#include <sys/ioctl.h>
#include <linux/sockios.h>
#include "ips_proto.h"
#include "ips_proto_internal.h"
#include "ips_proto_params.h"

/*---------------------------------------------------------------------------*/
/* TCP specific code */

#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
// set iov for remaining GPU payload data. It copies device memory to sockets_ep.sbuf
// in word boundary and then set iov to use the sockets_ep.sbuf with proper offset.
#define PAYLOAD_IOV(iov, payload, payload_len, remaining, buf, is_gpu_payload)                      \
	if (is_gpu_payload) {                                                                       \
		uint8_t adjust = remaining & 0x3;                                                    \
		if (adjust) {                                                                        \
			adjust = 4 - adjust;                                                         \
			_HFI_VDBG("Copy GPU remaining device memory: remaining=%d adjustment=%d\n",  \
				remaining, adjust);                                                  \
		}                                                                                    \
		PSM3_GPU_MEMCPY_DTOH(buf,                                                            \
			(uint8_t*)payload + payload_len - remaining - adjust,                        \
			remaining + adjust);                                                         \
		iov.iov_base = buf + adjust;                                                         \
		buf += adjust + remaining;                                                           \
		iov.iov_len = remaining;                                                             \
	} else {                                                                                     \
		iov.iov_base = (uint8_t*)payload + payload_len - remaining;                          \
		iov.iov_len = remaining;                                                             \
	}
#else
// set iov for remaining payload data
#define PAYLOAD_IOV(iov, payload, payload_len, remaining) do {                     \
		iov.iov_base = (uint8_t*)payload + payload_len - remaining;        \
		iov.iov_len = remaining;                                           \
	} while(0)
#endif

// prepare msghdr for a message
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
#define MSG_IOV(msg, header, payload, payload_len, remaining, buf, is_gpu_payload)                  \
	if (likely(remaining > payload_len)) {                                                       \
		msg.msg_iov[msg.msg_iovlen].iov_len = remaining - payload_len;                       \
		msg.msg_iov[msg.msg_iovlen].iov_base =                                               \
			(uint8_t*)header + sizeof(*header) - msg.msg_iov[msg.msg_iovlen].iov_len;    \
		msg.msg_iovlen += 1;                                                                 \
		if (likely(payload_len > 0)) {                                                       \
			PAYLOAD_IOV(msg.msg_iov[msg.msg_iovlen],                                     \
				payload, payload_len, payload_len,                                   \
				buf, is_gpu_payload);                                               \
			msg.msg_iovlen += 1;                                                         \
		}                                                                                    \
	} else {                                                                                     \
		PAYLOAD_IOV(msg.msg_iov[msg.msg_iovlen], payload, payload_len,                       \
			remaining, buf, is_gpu_payload);                                            \
		msg.msg_iovlen += 1;                                                                 \
	}
#else
#define MSG_IOV(msg, header, payload, payload_len, remaining)                                        \
	if (likely(remaining > payload_len)) {                                                       \
		msg.msg_iov[msg.msg_iovlen].iov_len = remaining - payload_len;                       \
		msg.msg_iov[msg.msg_iovlen].iov_base =                                               \
			(uint8_t*)header + sizeof(*header) - msg.msg_iov[msg.msg_iovlen].iov_len;    \
		msg.msg_iovlen += 1;                                                                 \
		if (likely(payload_len > 0)) {                                                       \
			PAYLOAD_IOV(msg.msg_iov[msg.msg_iovlen],                                     \
				payload, payload_len, payload_len);                                  \
			msg.msg_iovlen += 1;                                                         \
		}                                                                                    \
	} else {                                                                                     \
		PAYLOAD_IOV(msg.msg_iov[msg.msg_iovlen], payload, payload_len, remaining);           \
		msg.msg_iovlen += 1;                                                                 \
	}
#endif

#ifdef PSM_FI
static __inline__ void
psm3_unknown_connection(struct ips_proto *proto, struct ips_flow * flow)
{
	int fd;
	uint8_t size, idx;
	uint8_t *buf;

	ips_epaddr_t *ipsaddr = flow->ipsaddr;
	fd = socket(psm3_socket_domain, SOCK_STREAM, 0);
	if (fd <= 0) {
		return;
	}
	struct linger lg;
	lg.l_onoff = 1;		/* non-zero value enables linger option in kernel */
        lg.l_linger = 0;	/* timeout interval in seconds */
        setsockopt( fd, SOL_SOCKET, SO_LINGER, (void *)&lg, sizeof(lg));

	if (psm3_parse_tcp_src_bind()) {
		psm3_sockaddr_in_t loc_addr;
		psm3_build_sockaddr(&loc_addr, 0, proto->ep->gid.hi, proto->ep->gid.lo,
					proto->ep->sockets_ep.if_index);
		bind(fd, (struct sockaddr *)&loc_addr, sizeof(loc_addr));
	}

	if (0 == connect(fd, &ipsaddr->sockets.remote_pri_addr, sizeof(ipsaddr->sockets.remote_pri_addr))) {
		size = random() % 100;
		if (size) {
			buf = psmi_malloc(proto->ep, UNDEFINED, size);
			if (buf) {
				if (size < 56) {
					// intentionally dirty data with one byte initialized
					idx = (size == 1) ? 0 : psm3_rand((long int) getpid()) % (size - 1);
					buf[idx] = idx;
				} else {
					// simulate the rare case that can pass check
					memcpy(buf, EXP_HDR, sizeof(EXP_HDR));
					if (size % 2) {
						buf[4] = 0xee;
						buf[5] = 0xee;
					} else {
						buf[4] = 0x00;
						buf[5] = 0x01;
					}
				}
				send(fd, buf, size, 0);
				psmi_free(buf);
			}
		}
	}

	close(fd);
}
#endif

static __inline__ psm2_error_t
psm3_sockets_tcp_connect(struct ips_proto *proto, struct ips_flow * flow)
{
	psm2_error_t err = PSM2_OK;
	psm3_sockaddr_in_t loc_addr;
	ips_epaddr_t *ipsaddr = flow->ipsaddr;

	if (ipsaddr->sockets.tcp_fd == -1) {
		ipsaddr->sockets.tcp_fd = socket(psm3_socket_domain, SOCK_STREAM | SOCK_NONBLOCK, 0);
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
		// new socket, reset partial send info just in case we are "reuse" a flow
		flow->send_remaining = 0;
		flow->conn_msg_remainder = 0;
	}

	if (psm3_parse_tcp_src_bind()) {
		psm3_build_sockaddr(&loc_addr, proto->ep->sockets_ep.out_socket, proto->ep->gid.hi, proto->ep->gid.lo,
					proto->ep->sockets_ep.if_index);
		if (-1 == bind(ipsaddr->sockets.tcp_fd, (struct sockaddr *)&loc_addr, sizeof(loc_addr))) {
			_HFI_ERROR("unable bind (fd=%d) to addr %s:%d %s\n",
				ipsaddr->sockets.tcp_fd, psm3_sockaddr_fmt((struct sockaddr *)&loc_addr, 0),
				proto->ep->sockets_ep.out_socket, strerror(errno));
			err = PSM2_INTERNAL_ERR;
			goto fail;
		} else {
			_HFI_PRDBG("PSM TCP bind (fd=%d) to addr %s\n", ipsaddr->sockets.tcp_fd,
				psm3_sockaddr_fmt((struct sockaddr *)&loc_addr, 0));
		}
	}

	if (-1 == connect(ipsaddr->sockets.tcp_fd, &ipsaddr->sockets.remote_pri_addr, sizeof(ipsaddr->sockets.remote_pri_addr))) {
		if ( errno == EALREADY || errno == EINPROGRESS || errno == EISCONN) {
			// connection already established or will be established in nearest future
			_HFI_PRDBG("PSM TCP connection to %s started for %s, fd=%d errno=%d\n",
				psm3_sockaddr_fmt((struct sockaddr *)&ipsaddr->sockets.remote_pri_addr, 0),
				proto->ep->dev_name, ipsaddr->sockets.tcp_fd, errno);
		} else {
			_HFI_ERROR("unable to establish connection to %s for %s: %s\n",
				psm3_sockaddr_fmt((struct sockaddr *)&ipsaddr->sockets.remote_pri_addr, 0),
				proto->ep->dev_name, strerror(errno));
			err = PSM2_INTERNAL_ERR;
			goto fail;
		}
	}
	ipsaddr->sockets.connected = 1;

	if (!proto->ep->sockets_ep.out_socket && psm3_parse_tcp_reuseport()) {
		socklen_t addr_len = sizeof(loc_addr);
		if ( -1 == getsockname(ipsaddr->sockets.tcp_fd, (struct sockaddr *)&loc_addr, &addr_len)
			|| addr_len > sizeof(loc_addr)) {
			_HFI_ERROR("Failed to query TCP socket address for %s: %s\n", proto->ep->dev_name, strerror(errno));
			goto fail;
		}

		proto->ep->sockets_ep.out_socket = __be16_to_cpu(psm3_socket_port(&loc_addr));
		_HFI_PRDBG("Local port for reuse: %d\n", proto->ep->sockets_ep.out_socket);
	}

#ifndef PSM_LOG
	if_pf (_HFI_VDBG_ON)
#endif
	{
		socklen_t addr_len = sizeof(loc_addr);
		if ( -1 == getsockname(ipsaddr->sockets.tcp_fd, (struct sockaddr *)&loc_addr, &addr_len)
			|| addr_len > sizeof(loc_addr)) {
			_HFI_ERROR("Failed to query TCP socket address for %s: %s\n", proto->ep->dev_name, strerror(errno));
			goto fail;
		}
		_HFI_VDBG("fd=%d Loc Addr:%s\n", ipsaddr->sockets.tcp_fd, psm3_sockaddr_fmt((struct sockaddr *)&loc_addr, 0));
#ifdef PSM_LOG
		PSM2_LOG_MSG("fd=%d Loc Addr:%s", ipsaddr->sockets.tcp_fd, psm3_sockaddr_fmt((struct sockaddr *)&loc_addr, 0));
#endif
	}

	_HFI_PRDBG("PSM TCP connected to %s fd=%d\n",
		psm3_sockaddr_fmt((struct sockaddr *)&ipsaddr->sockets.remote_pri_addr, 0), ipsaddr->sockets.tcp_fd);
	PSM2_LOG_MSG("connected to %s fd=%d",
		psm3_sockaddr_fmt((struct sockaddr *)&ipsaddr->sockets.remote_pri_addr, 0), ipsaddr->sockets.tcp_fd);
	return err;
fail:
	if (ipsaddr->sockets.tcp_fd > 0) {
		close(ipsaddr->sockets.tcp_fd);
		// will re-connect
		ipsaddr->sockets.tcp_fd = -1;
		ipsaddr->sockets.connected = 0;
	}
	return err;
}

static __inline__ psm2_error_t
psm3_sockets_tcp_sendpacing(struct ips_proto *proto, struct ips_flow *flow)
{
	uint32_t used;
	if (ioctl(flow->ipsaddr->sockets.tcp_fd, SIOCOUTQ, &used) == 0) {
		if (flow->used_snd_buff && used > proto->ep->sockets_ep.snd_pace_thresh
			&& used >= flow->used_snd_buff) {
			_HFI_VDBG("Pre=%d Cur=%d Delta=%d fd=%d\n",
					flow->used_snd_buff, used,
					used - flow->used_snd_buff,
					flow->ipsaddr->sockets.tcp_fd);
			return PSMI_TRUE;
		}
		flow->used_snd_buff = used;
	} else {
		_HFI_DBG("ERR: %s\n", strerror(errno));
	}
	return PSMI_FALSE;
}

static __inline__ psm2_error_t
psm3_sockets_tcp_aux_send(psm2_ep_t ep, struct ips_flow *flow,
	struct ips_message_header *header, uint32_t *payload, uint32_t payload_len
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
	, uint32_t is_gpu_payload
#endif
	)
{
	psm2_error_t ret = PSM2_OK;
#ifdef PSM_FI
	if_pf(PSM3_FAULTINJ_ENABLED_EP(ep)) {
		PSM3_FAULTINJ_STATIC_DECL(fi_sendlost, "sendlost",
				"drop packet before sending",
				1, IPS_FAULTINJ_SENDLOST);
		if_pf(PSM3_FAULTINJ_IS_FAULT(fi_sendlost, ep, " UDP DISCON"))
			return ret;
	}
#endif // PSM_FI
	// receiver expects a whole pkt in one datagram
	struct msghdr msg = ep->sockets_ep.snd_msg;
	msg.msg_iov[0].iov_base = (uint8_t*)header;
	msg.msg_iov[0].iov_len = sizeof(*header);
	if (payload_len) {
		PAYLOAD_IOV(msg.msg_iov[1], payload, payload_len, payload_len
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
			, ep->sockets_ep.sbuf, is_gpu_payload
#endif
			);
		msg.msg_iovlen = 2;
	} else {
		msg.msg_iovlen = 1;
	}
	msg.msg_name = &flow->ipsaddr->sockets.remote_aux_addr;
	msg.msg_namelen = sizeof(psm3_sockaddr_in_t);
	if_pf (sendmsg(ep->sockets_ep.udp_tx_fd, &msg, 0) == -1) {
		PSM2_LOG_MSG("sendto fd=%d ret=-1 errno=%d", ep->sockets_ep.udp_tx_fd, errno);
		if (errno != EAGAIN && errno != EWOULDBLOCK) {
			_HFI_ERROR("UDP send failed on %s: %s\n", ep->dev_name, strerror(errno));
		}
		ret = PSM2_EP_NO_RESOURCES;
#ifdef PSM_LOG
	} else {
		PSM2_LOG_MSG("sendto fd=%d ret=%d", ep->sockets_ep.udp_tx_fd, ret);
#endif
	}

	return ret;
}

//TODO: If we choose to use bulk message send for data messages, we
// can simplify this function to work for ctr msg only.
// when called:
//		scb->ips_lrh has fixed size PSM header including OPA LRH
//		payload, length is data after header
//		we don't do checksum, let verbs handle that for us
//		the above is in unregistered memory, perhaps even on stack
// for isCtrlMsg, scb is only partially initialized (see ips_scb.h)
// and payload and length may refer to buffers on stack
static inline psm2_error_t
psm3_sockets_tcp_spio_transfer_frame(struct ips_proto *proto, struct ips_flow *flow,
			struct ips_scb *scb, uint32_t *payload,
			uint32_t length, uint32_t isCtrlMsg,
			uint32_t cksum_valid, uint32_t cksum
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
			, uint32_t is_gpu_payload
#endif
			)
{
	psm2_error_t ret = PSM2_OK;
	psm2_ep_t ep = proto->ep;
	struct ips_message_header *ips_lrh = &scb->ips_lrh;
	uint8_t opcode = _get_proto_hfi_opcode(ips_lrh);
	uint32_t len;
	struct msghdr msg = ep->sockets_ep.snd_msg;

	psmi_assert(flow->transfer == PSM_TRANSFER_PIO);
	// shall be called for ctr msg only. will cleanup isCtrlMsg in HED-4082
	psmi_assert(isCtrlMsg);
	PSMI_LOCK_ASSERT(proto->mq->progress_lock);
	PSM2_LOG_MSG("entering with fd=%d pay_len=%d opcode=%x",
		flow->ipsaddr->sockets.tcp_fd, length, opcode);

	if_pf(flow->ipsaddr->cstate_outgoing == CSTATE_OUTGOING_WAITING) {
		if (flow->ipsaddr->sockets.tcp_fd == 0) {
			// closed because of connection fatal error
			return PSM2_INTERNAL_ERR;
		}
		if (!flow->ipsaddr->sockets.connected) {
			ret = psm3_sockets_tcp_connect(proto, flow);
			if (ret != PSM2_OK) {
				return ret;
			}
		}
		struct fd_ctx *ctx = psm3_sockets_get_fd_ctx(ep, flow->ipsaddr->sockets.tcp_fd);
		if (opcode == OPCODE_CONNECT_REQUEST && (!ctx || ctx->state == FD_STATE_NONE)) {
			// we are in connecting state, check whether the connection is established
			struct pollfd pfd = {
				.fd = flow->ipsaddr->sockets.tcp_fd,
				.events = POLLOUT};
			if (poll(&pfd, 1, 0) == 1 && (pfd.revents & POLLOUT)) {
				int error;
				socklen_t n = sizeof(error);
				if (getsockopt(flow->ipsaddr->sockets.tcp_fd, SOL_SOCKET, SO_ERROR, &error, &n) < 0 ||
					error) {
					_HFI_DBG("non-blocking connect failed - error=%d fd=%d\n",
						error, flow->ipsaddr->sockets.tcp_fd);
					int id = psm3_sockets_get_index_by_fd(ep, flow->ipsaddr->sockets.tcp_fd);
					if (id >= 0) {
						psm3_sockets_tcp_close_fd(ep, flow->ipsaddr->sockets.tcp_fd, id, flow);
					} else {
						close(flow->ipsaddr->sockets.tcp_fd);
					}
					// will re-connect
					flow->ipsaddr->sockets.tcp_fd = -1;
					flow->ipsaddr->sockets.connected = 0;
					return PSM2_INTERNAL_ERR;
				}
				// connection successfully established
				_HFI_VDBG("Connection established fd=%d\n", flow->ipsaddr->sockets.tcp_fd);
				if (ctx == NULL) {
					psm3_sockets_tcp_add_fd(ep, flow->ipsaddr->sockets.tcp_fd, 1);
					ctx = psm3_sockets_get_fd_ctx(ep, flow->ipsaddr->sockets.tcp_fd);
					if (ctx) {
						ctx->ipsaddr = flow->ipsaddr;
						ctx->state = FD_STATE_VALID;
					} else {
						// shouldn't happen
						close(flow->ipsaddr->sockets.tcp_fd);
						flow->ipsaddr->sockets.tcp_fd = -1;
						flow->ipsaddr->sockets.connected = 0;
						// return PSM2_OK to retry in next loop rather than retry immediately
						return PSM2_OK;
					}
				}
			} else {
				_HFI_VDBG("non-blocking connecting revents=%d fd=%d\n", pfd.revents, flow->ipsaddr->sockets.tcp_fd);
				return PSM2_EP_NO_RESOURCES;
			}
		}
	}

	if_pf (flow->conn_msg_remainder) {
		psmi_assert(flow->partial_conn_msg);
		len = flow->conn_msg_remainder;
		msg.msg_iov[0].iov_base = flow->partial_conn_msg;
		msg.msg_iov[0].iov_len = len;
		msg.msg_iovlen = 1;
	} else if_pf (flow->send_remaining && !(scb->scb_flags & IPS_SEND_FLAG_TCP_REMAINDER)) {
		// this flow has partial send, but the scb is not the one we want to continue
		// this likely will not happen, but we have ctr and data msg retry in different
		// timers. So we check here to play safe.
		if_pf (opcode == OPCODE_DISCONNECT_REQUEST || opcode == OPCODE_DISCONNECT_REPLY) {
			// send DISCONN msg via aux socket
			PSMI_LOCK_ASSERT(proto->mq->progress_lock);
			_HFI_VDBG("Send DISCONN msg opcode=%x via aux_socket\n", opcode);
			flow->send_remaining = 0;
			return psm3_sockets_tcp_aux_send(ep, flow, ips_lrh, payload, length
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
				, is_gpu_payload
#endif
			);
		} else {
			// retry later
			_HFI_VDBG("skip because of undesired scb\n");
			return PSM2_EP_NO_RESOURCES;
		}
	} else {
		len = flow->send_remaining ? flow->send_remaining : sizeof(*ips_lrh) + length;
		msg.msg_iovlen = 0;
		MSG_IOV(msg, ips_lrh, payload, length, len
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
			, ep->sockets_ep.sbuf, is_gpu_payload
#endif
			);
	}

#ifdef PSM_FI
	// This is a bit of a high stress cheat.  While TCP is loss-less
	// we know that our callers for control messages may be forced to
	// discard control messages when we have NO_RESOURCES for long enough
	// or the shallow control queue overflows.  So for control messages
	// we can discard the packet here to simulate those side effects of
	// extreme TCP backpressure and long duration NO_RESOURCES.
	// The "sendfull*" injectors have a similar effect but will rarely
	// simulate enough repeated backpressure for the caller to actually give up.
	// We do not simulate loss for data packets since we commit to their
	// reliable delivery here once we return PSM2_OK.  For data packets the
	// "sendfull" injector will create the need for callers to retry sending.
	// We omit connect packets since they are less likely to see NO_RESOURCES
	// long enough for their caller to timeout (and we want to be able to set
	// a high sendlost without causing job launch issues).  Disconnect only
	// has 100 quick retries, so we let sendlost apply to it.
	if_pf(PSM3_FAULTINJ_ENABLED_EP(ep) && isCtrlMsg
			&& opcode != OPCODE_CONNECT_REQUEST
			&& opcode != OPCODE_CONNECT_REPLY) {
		PSM3_FAULTINJ_STATIC_DECL(fi_sendlost, "sendlost",
				"drop packet before sending",
				1, IPS_FAULTINJ_SENDLOST);
		if_pf(PSM3_FAULTINJ_IS_FAULT(fi_sendlost, ep, " TCP ctrl"))
			return PSM2_OK;
	}
#endif // PSM_FI

	if (!isCtrlMsg && proto->ep->sockets_ep.snd_pace_thresh &&
		psm3_sockets_tcp_sendpacing(proto, flow) == PSMI_TRUE) {
		return PSM2_EP_NO_RESOURCES;
	}

	if (likely(flow->send_remaining == 0 && flow->conn_msg_remainder == 0)) {
		psmi_assert_always(! cksum_valid);	// no software checksum yet
		psmi_assert((len & 3) == 0);	// must be DWORD mult
#ifndef PSM_TCP_ACK
		// clear IPS_SEND_FLAG_ACKREQ in bth[2] because TCP doesn't need ack
		if (!isCtrlMsg) {
			ips_lrh->bth[2] &= __cpu_to_be32(~IPS_SEND_FLAG_ACKREQ);
		}
#endif
		_HFI_VDBG("TCP send - opcode %x len %u fd=%d\n", opcode, len, flow->ipsaddr->sockets.tcp_fd);
		// we don't support software checksum
		psmi_assert_always(! (proto->flags & IPS_PROTO_FLAG_CKSUM));

		if_pf (ips_lrh->khdr.kdeth0 & __cpu_to_le32(IPS_SEND_FLAG_INTR)) {
			_HFI_VDBG("send solicted event\n");
			// TBD - how to send so wake up rcvthread?  Separate socket?
		}

		if_pf (_HFI_PDBG_ON) {
			_HFI_PDBG_ALWAYS("len %u, remote IP %s payload %u\n",
				len,
				psm3_sockaddr_fmt((struct sockaddr *)&flow->ipsaddr->sockets.remote_pri_addr, 0),
				length);
			_HFI_PDBG_DUMP_ALWAYS((uint8_t*)ips_lrh, sizeof(*ips_lrh));
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
			if (is_gpu_payload) {
				PSM3_GPU_MEMCPY_DTOH(ep->sockets_ep.sbuf,
					payload, length);
				_HFI_PDBG_DUMP_ALWAYS(ep->sockets_ep.sbuf, length);
			} else {
				_HFI_PDBG_DUMP_ALWAYS((uint8_t*)payload, length);
			}
#else
			_HFI_PDBG_DUMP_ALWAYS((uint8_t*)payload, length);
#endif
		}
		if_pf (opcode == OPCODE_DISCONNECT_REPLY) {
			return psm3_sockets_tcp_aux_send(ep, flow, ips_lrh, payload, length
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
				, is_gpu_payload
#endif
			);
		}
	}

	// TBD - we should be able to skip sending some headers such as OPA lrh and
	// perhaps bth (does PSM use bth to hold PSNs? - yes)
	if (likely(flow->ipsaddr->sockets.tcp_fd > 0)) {
#ifdef PSM_FI
		int org_len = 0;
		if_pf(PSM3_FAULTINJ_ENABLED_EP(ep)) {
			PSM3_FAULTINJ_STATIC_DECL(fi_sendpart, "sendpart",
					"partial TCP send",
					1, IPS_FAULTINJ_SENDPART);
			if_pf(PSM3_FAULTINJ_IS_FAULT(fi_sendpart, ep, "")) {
				org_len = msg.msg_iov[0].iov_len;
				msg.msg_iov[0].iov_len = min(msg.msg_iov[0].iov_len, 32); // purposely less than min pkt size
				msg.msg_iovlen = 1;
			}
			PSM3_FAULTINJ_STATIC_DECL(fi_connunkn, "connunkn",
					"unknown connection",
					1, IPS_FAULTINJ_CONNUNKN);
			if_pf(PSM3_FAULTINJ_IS_FAULT(fi_connunkn, ep, "")) {
				psm3_unknown_connection(proto, flow);
			}
		}
#endif // PSM_FI
		int flags = MSG_DONTWAIT;
		if (flow->ipsaddr->cstate_outgoing == CSTATE_OUTGOING_WAITING_DISC) {
			// during disconnecting, SIGPIPE may happen on send because
			// we intend to close it from another side. Set MSG_NOSIGNAL
			// to ignore it, otherwise this can cause IMPI termination
			flags |= MSG_NOSIGNAL;
		}
		ssize_t res = sendmsg(flow->ipsaddr->sockets.tcp_fd, &msg, flags);
#ifdef PSM_FI
		if_pf(org_len) {
			// restore iov_len because below code needs it
			msg.msg_iov[0].iov_len = org_len;
		}
#endif
		if (res == len) {
			// sent out full pkt (or last chunk)
			// if we just sent a partial conn msg, we need return PSM2_EP_NO_RESOURCES
			// to trigger a retry of the callers intended send message
			ret = flow->conn_msg_remainder ? PSM2_EP_NO_RESOURCES : PSM2_OK;
			flow->send_remaining = 0;
			flow->conn_msg_remainder = 0;
			_HFI_VDBG("Sent successfully. opcode=%x fd=%d\n", opcode, flow->ipsaddr->sockets.tcp_fd);
		} else if (res > 0) {
			if_pf (flow->conn_msg_remainder) {
				// partial conn msg already copied to flow->partial_conn_msg
				flow->conn_msg_remainder = len - res;
				// we can avoid memmove by using offset. But this is a very very rare case
				// that multiple partial sends on a short conn msg. So no need to introduce
				// a new field for offset.
				memmove(flow->partial_conn_msg, flow->partial_conn_msg + res,
					flow->conn_msg_remainder);
				_HFI_VDBG("Partial conn msg send: opcode=%x send=%ld remainder=%d fd=%d\n",
					opcode, res, flow->conn_msg_remainder, flow->ipsaddr->sockets.tcp_fd);
			} else if (opcode == OPCODE_CONNECT_REQUEST ||
				opcode == OPCODE_CONNECT_REPLY ||
				opcode == OPCODE_DISCONNECT_REQUEST) {
			    	// first time partial send on conn msg
				flow->conn_msg_remainder = len - res;
			    	if (flow->conn_msg_remainder > flow->partial_conn_msg_size) {
			    		flow->partial_conn_msg_size = TCP_CONN_MSG_BUF_BLOCK *
			    			(1 + flow->conn_msg_remainder/TCP_CONN_MSG_BUF_BLOCK);
			    		// when we grow the buff, we do not need to retain the data
			    		// below is faster than psmi_realloc that will do memcpy
			    		if (flow->partial_conn_msg) {
						psmi_free(flow->partial_conn_msg);
			    		}
					flow->partial_conn_msg = (uint8_t *) psmi_calloc(ep,
						NETWORK_BUFFERS, flow->partial_conn_msg_size, 1);
					if (!flow->partial_conn_msg) {
						_HFI_ERROR("Unable to allocate buffer for partial connection message\n");
						return PSM2_INTERNAL_ERR;
					}
			    	}
			    	// we can potentially use ep->sbuf to store partial conn msg. But the buffer
			    	// is at EP level. When one flow blocks on partial conn msg, all other flows
			    	// have to wait for it. It's more efficient to use flow->partial_conn_msg
			    	// that is at flow level. Below memecpy uses msg.msg_iov that already has data
			    	// copied from device for GPU.
				if (res < msg.msg_iov[0].iov_len) {
					memcpy(flow->partial_conn_msg, (uint8_t *)msg.msg_iov[0].iov_base + res,
						msg.msg_iov[0].iov_len - res);
					memcpy(flow->partial_conn_msg + msg.msg_iov[0].iov_len - res,
						msg.msg_iov[1].iov_base, msg.msg_iov[1].iov_len);
				} else {
					memcpy(flow->partial_conn_msg,
						(uint8_t *)msg.msg_iov[1].iov_base + msg.msg_iov[1].iov_len - flow->conn_msg_remainder,
						flow->conn_msg_remainder);
				}
				_HFI_VDBG("New partial conn msg send: opcode=%x send=%ld remainder=%d fd=%d\n",
                                          opcode, res, flow->conn_msg_remainder, flow->ipsaddr->sockets.tcp_fd);
				proto->stats.partial_ctr_write_cnt++;
			} else {
				// this shall not happen since we are doing local ack
				// send out partial pkt
				flow->send_remaining = len - res;
				scb->scb_flags |= IPS_SEND_FLAG_TCP_REMAINDER;
				if (isCtrlMsg) {
					proto->stats.partial_ctr_write_cnt++;
				} else {
					proto->stats.partial_data_write_cnt++;
				}
			}
			_HFI_VDBG("Partial sending. opcode=%x fd=%d res=%ld len=%d remainder=%d conn_msg_remainder=%d\n",
					opcode, flow->ipsaddr->sockets.tcp_fd, res, len,
					flow->send_remaining, flow->conn_msg_remainder);
			ret = PSM2_EP_NO_RESOURCES;
		} else if (errno == EAGAIN || errno == EWOULDBLOCK || errno == ENOTCONN) {
			// socket is not ready. Either of outgoing buffer is full or not yet connected.
			_HFI_VDBG("Partial or empty sending. errno=%d remainder=%d\n",
				errno, flow->send_remaining);
			ret = PSM2_EP_NO_RESOURCES;
		} else {
			psmi_assert(errno != EMSGSIZE); // shouldn't happen on TCP
			if (flow->ipsaddr->cstate_outgoing == CSTATE_ESTABLISHED) {
				// error
				_HFI_INFO("TCP send fd=%d opcode=%x failed on %s: %s\n",
					flow->ipsaddr->sockets.tcp_fd, opcode,
					ep->dev_name, strerror(errno));
			} else {
				// sending data under undesired state, such as WAITING, WAITING_DISC, DISCONNECTED
				_HFI_DBG("TCP send fd=%d failed on %s. cstate_outgoing=%d opcode=%x error: %s\n",
					flow->ipsaddr->sockets.tcp_fd, ep->dev_name, flow->ipsaddr->cstate_outgoing,
					opcode, strerror(errno));
			}
			if (isCtrlMsg && opcode!=OPCODE_CONNECT_REQUEST && opcode!=OPCODE_CONNECT_REPLY) {
				// send the whole non-conn control message via aux socket (UDP). Note:
				// 1) it doesn't make sense to continue CONN msg with aux socket because
				//    we do not support sending data msg via aux socket (UDP)
				// 2) we resend the whole pkt because when receiver gets data via aux socket
				//    (UDP), it will fill receiver buffer from beginning
				flow->send_remaining = 0;
				ret = psm3_sockets_tcp_aux_send(ep, flow, ips_lrh, payload, length
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
					, is_gpu_payload
#endif
				);
				if (ret != PSM2_OK && flow->send_remaining) {
					// hopefully this will not happen. TBD - how we handle this case
					_HFI_ERROR("TCP send failed in the middle of msg transition.\n");
				}
			} else {
				// TCP has lost the connection.  We can't put TCP data scb's
				// on UDP since TCP's PSM3_MTU may be too large.  We can't
				// segment since caller has assigned psn's for this and scb's
				// after it already.  Also with TCP ack-less behavior we
				// may have lost some packets previously given to the sockets
				// stack and we have no way to know which and retransmit.
				// So our only recourse is an error so caller doesn't think
				// this packet was sent.  Ultimately unless we get a remote
				// disconnect, the job may hang (which is what verbs or OPA
				// would do if network connectivity was lost), however unlike
				// OPA we will not be able to resume if connectivity is restored
				ret = PSM2_EP_NO_NETWORK;
			}
			// close the TCP fd, and we will switch to use aux socket that is UDP
			psm3_sockets_tcp_close_fd(ep, flow->ipsaddr->sockets.tcp_fd, -1, flow);
			flow->ipsaddr->sockets.tcp_fd = 0;
		}
	} else {
		if (isCtrlMsg && opcode!=OPCODE_CONNECT_REQUEST && opcode!=OPCODE_CONNECT_REPLY) {
			// send control message via aux socket (UDP)
			_HFI_VDBG("Invalid tcp_fd on %s! Try to use aux socket.\n", ep->dev_name);
			flow->send_remaining = 0;
			ret = psm3_sockets_tcp_aux_send(ep, flow, ips_lrh, payload, length
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
				, is_gpu_payload
#endif
			);
		} else {
			// unable to switch to UDP for data packets, see discussion above
			_HFI_VDBG("Invalid tcp_fd on %s!\n", ep->dev_name);
			ret = PSM2_EP_NO_NETWORK;
		}
	}

#ifndef PSM_TCP_ACK
	// return PSM2_OK for ctrl msg and PSM2_TCP_DATA_SENT for data msg
	if (ret == PSM2_OK && !isCtrlMsg) {
		return PSM2_TCP_DATA_SENT;
	}
#endif
	return ret;
}

// send a bulk of messages. used for data messages only.
static __inline__ psm2_error_t
psm3_sockets_tcp_spio_transfer_frames(struct ips_proto *proto, struct ips_flow *flow,
			struct ips_scb_pendlist *scb_queue, int *send_count)
{
	psm2_error_t ret = PSM2_OK;
	psm2_ep_t ep = proto->ep;
	*send_count = 0;

	PSMI_LOCK_ASSERT(proto->mq->progress_lock);
	psmi_assert(!SLIST_EMPTY(scb_queue));
	ips_scb_t *scb = SLIST_FIRST(scb_queue);

	struct msghdr msg = ep->sockets_ep.snd_msg;
	msg.msg_iovlen = 0;
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
	// this is used for GPU support. It maintains the position in sbuf
	// to which we copy data from device
	uint8_t *buf = ep->sockets_ep.sbuf;
#endif
	// we put messages in iovs until the total length > buffer_size, or
	// number of messages in the array > TCP_IOV_SIZE
	struct iovec *iovs = ep->sockets_ep.snd_msg.msg_iov;
	// number of messages we plan to send out. This number shall >= 1 because
	// we will always put the first scb in iovs array given each pkt len <= buffer_size
	int pend_msgs = 1;
	// total length of the data we plan to send out
	uint32_t len;
	int i;

	PSM2_LOG_MSG("entering with fd=%d", flow->ipsaddr->sockets.tcp_fd);

	if_pf (flow->conn_msg_remainder) {
		// continue partial conn msg, the passed in scb is used as a dumy scb
		ret = psm3_sockets_tcp_spio_transfer_frame(proto, flow, scb, ips_scb_buffer(scb),
			scb->payload_size, PSMI_TRUE, scb->ips_lrh.flags & IPS_SEND_FLAG_PKTCKSUM,
			scb->cksum[0]
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
			, IS_TRANSFER_BUF_GPU_MEM(scb)
#endif
		);
		if (ret != PSM2_OK) {
			return ret;
		}
	}

	if_pf (flow->send_remaining && !(scb->scb_flags & IPS_SEND_FLAG_TCP_REMAINDER)) {
		// this flow has partial send, but the scb is not the one we want to continue
		// this likely will not happen, but we have ctr and data msg retry in different
		// timers. So we check here to play safe.
		_HFI_VDBG("skip because of undesired scb\n");
		return PSM2_EP_NO_RESOURCES;
	}

	if (proto->ep->sockets_ep.snd_pace_thresh &&
		psm3_sockets_tcp_sendpacing(proto, flow) == PSMI_TRUE) {
		return PSM2_EP_NO_RESOURCES;
	}

	if_pf (flow->ipsaddr->sockets.tcp_fd <= 0) {
		_HFI_VDBG("Invalid tcp_fd on %s!\n", ep->dev_name);
		return PSM2_EP_NO_NETWORK;
	}

	struct ips_message_header *ips_lrh = &scb->ips_lrh;
	psmi_assert(_get_proto_hfi_opcode(ips_lrh) >= OPCODE_TINY &&
		_get_proto_hfi_opcode(ips_lrh) <= OPCODE_LONG_DATA);

	// need to consider partial send for the first msg
	if (flow->send_remaining) {
		len = flow->send_remaining;
	} else {
		psmi_assert((scb->payload_size & 3) == 0);	// must be DWORD mult
#ifndef PSM_TCP_ACK
		// clear IPS_SEND_FLAG_ACKREQ in bth[2] because TCP doesn't need ack
		ips_lrh->bth[2] &= __cpu_to_be32(~IPS_SEND_FLAG_ACKREQ);
#endif
		len = sizeof(*ips_lrh) + scb->payload_size;
	}
	MSG_IOV(msg, ips_lrh, ips_scb_buffer(scb), scb->payload_size, len
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
			, buf, IS_TRANSFER_BUF_GPU_MEM(scb)
#endif
		);

	while ((scb = SLIST_NEXT(scb, next)) != NULL) {
		ips_lrh = &scb->ips_lrh;
		psmi_assert(_get_proto_hfi_opcode(ips_lrh) >= OPCODE_TINY &&
			_get_proto_hfi_opcode(ips_lrh) <= OPCODE_LONG_DATA);
		if (len + sizeof(*ips_lrh) + scb->payload_size > ep->sockets_ep.buf_size ||
			msg.msg_iovlen + 2 > TCP_IOV_SIZE) {
			break;
		}

		psmi_assert((scb->payload_size & 3) == 0);	// must be DWORD mult
#ifndef PSM_TCP_ACK
		// clear IPS_SEND_FLAG_ACKREQ in bth[2] because TCP doesn't need ack
		ips_lrh->bth[2] &= __cpu_to_be32(~IPS_SEND_FLAG_ACKREQ);
#endif
		iovs[msg.msg_iovlen].iov_base = (uint8_t*)ips_lrh;
		iovs[msg.msg_iovlen].iov_len = sizeof(*ips_lrh);
		msg.msg_iovlen += 1;
		if (likely(scb->payload_size > 0)) {
			PAYLOAD_IOV(iovs[msg.msg_iovlen], ips_scb_buffer(scb),
				scb->payload_size, scb->payload_size
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
				, buf, IS_TRANSFER_BUF_GPU_MEM(scb)
#endif
				);
			msg.msg_iovlen += 1;
		}
		len += sizeof(*ips_lrh) + scb->payload_size;
		pend_msgs += 1;
	}

	_HFI_VDBG("TCP send - iov_count=%ld total_len=%d num_messages=%d fd=%d\n",
		msg.msg_iovlen, len, pend_msgs, flow->ipsaddr->sockets.tcp_fd);
	if_pf (_HFI_PDBG_ON) {
		_HFI_PDBG_ALWAYS("remote IP %s iov_count=%ld total_len %d\n",
			psm3_sockaddr_fmt((struct sockaddr *)&flow->ipsaddr->sockets.remote_pri_addr, 0),
			msg.msg_iovlen, len);
		for (i = 0; i < msg.msg_iovlen; i++) {
			_HFI_PDBG_DUMP_ALWAYS((uint8_t*)iovs[i].iov_base, iovs[i].iov_len);
		}
	}
#ifdef PSM_FI
	if_pf(PSM3_FAULTINJ_ENABLED_EP(ep)) {
		PSM3_FAULTINJ_STATIC_DECL(fi_sendfull, "sendfull",
			"no more send resources pio flush",
			1, IPS_FAULTINJ_SENDFULL);
		if_pf(PSM3_FAULTINJ_IS_FAULT(fi_sendfull, proto->ep, "")) {
			msg.msg_iovlen = psm3_rand((long int) getpid()) % msg.msg_iovlen + 1;
		}
		PSM3_FAULTINJ_STATIC_DECL(fi_sendpart, "sendpart",
				"partial TCP send",
				1, IPS_FAULTINJ_SENDPART);
		if_pf(PSM3_FAULTINJ_IS_FAULT(fi_sendpart, ep, "")) {
			iovs[msg.msg_iovlen - 1].iov_len = min(32, iovs[msg.msg_iovlen - 1].iov_len); // purposely less than min pkt size
		}
		PSM3_FAULTINJ_STATIC_DECL(fi_connunkn, "connunkn",
				"unknown connection",
				1, IPS_FAULTINJ_CONNUNKN);
		if_pf(PSM3_FAULTINJ_IS_FAULT(fi_connunkn, ep, "")) {
			psm3_unknown_connection(proto, flow);
		}
	}
#endif // PSM_FI

	ssize_t res = sendmsg(flow->ipsaddr->sockets.tcp_fd, &msg, MSG_DONTWAIT);
	if (res == len) {
		// send out all pkts (include last chunk if has partial send)
		flow->send_remaining = 0;
		*send_count = pend_msgs;
		_HFI_VDBG("Sent successfully. message_sent=%d fd=%d\n", *send_count,
			flow->ipsaddr->sockets.tcp_fd);
	} else if (res > 0) {
		// find the partial pkt
		uint32_t sum = 0;
		int msg_count = 0;
		SLIST_FOREACH(scb, scb_queue, next) {
			if (sum == 0 && flow->send_remaining) {
				sum = flow->send_remaining;
			} else {
				sum += sizeof(struct ips_message_header);
				sum += scb->payload_size;
			}
			msg_count += 1;
			if (sum == res) {
				flow->send_remaining = 0;
				*send_count = msg_count;
				break;
			} else if (sum > res) {
				flow->send_remaining = sum - res;
				scb->scb_flags |= IPS_SEND_FLAG_TCP_REMAINDER;
				proto->stats.partial_data_write_cnt++;
				*send_count = msg_count - 1;
				_HFI_VDBG("Partial sending. fd=%d num_msg_sent=%d partial_msg_len=%ld partial_msg_remainder=%d\n",
					flow->ipsaddr->sockets.tcp_fd, *send_count,
					sizeof(struct ips_message_header) + scb->payload_size,
					flow->send_remaining);
				ret = PSM2_EP_NO_RESOURCES;
				break;
			}
		}
	} else if (errno == EAGAIN || errno == EWOULDBLOCK || errno == ENOTCONN) {
		// socket is not ready. Either of outgoing buffer is full or not yet connected.
		_HFI_VDBG("Partial or empty sending. errno=%d remainder=%d\n",
			errno, flow->send_remaining);
		ret = PSM2_EP_NO_RESOURCES;
	} else {
		psmi_assert(errno != EMSGSIZE); // shouldn't happen on TCP
		scb = SLIST_FIRST(scb_queue);
		if (flow->ipsaddr->cstate_outgoing == CSTATE_ESTABLISHED) {
			// error
			_HFI_INFO("TCP send fd=%d opcode=%x failed on %s: %s\n",
				flow->ipsaddr->sockets.tcp_fd,
				_get_proto_hfi_opcode(&scb->ips_lrh),
				ep->dev_name, strerror(errno));
		} else {
			// sending data under undesired state, such as WAITING, WAITING_DISC, DISCONNECTED
			_HFI_DBG("TCP send fd=%d failed on %s. cstate_outgoing=%d opcode=%x error: %s\n",
				flow->ipsaddr->sockets.tcp_fd, ep->dev_name, flow->ipsaddr->cstate_outgoing,
				_get_proto_hfi_opcode(&scb->ips_lrh), strerror(errno));
		}
		ret = PSM2_EP_NO_NETWORK;
		// close the TCP fd, and we will switch to use aux socket that is UDP
		psm3_sockets_tcp_close_fd(ep, flow->ipsaddr->sockets.tcp_fd, -1, flow);
		flow->ipsaddr->sockets.tcp_fd = 0;
	}

	return ret;
}

/*---------------------------------------------------------------------------*/
/* UDP specific code */

// segment and send a eager or long_data SCB using GSO send
// caller will wait for ack before freeing scb, so we could potentially
// send directly from the payload in future and avoid some copies
static __inline__ int
psm3_sockets_udp_gso_send(int fd, struct ips_proto *proto,
		psm3_sockaddr_in_t *addr,
		struct ips_scb *scb, uint8_t *payload, uint32_t length,
		uint32_t frag_size
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
		, uint32_t is_gpu_payload
#endif
		)
{
	char control[CMSG_SPACE(sizeof(uint16_t))] = {0};
	struct msghdr msg = {0};
	struct iovec iov = {0};
	uint8_t *sbuf_gso = proto->ep->sockets_ep.sbuf_udp_gso;
	struct ips_message_header *ips_lrh = &scb->ips_lrh;
	psm2_ep_t ep = proto->ep;
	uint32_t gso_len = 0;

	// get some fields out of header, we'll be modifying these on each
	// fragment
	uint32_t psn = __be32_to_cpu(ips_lrh->bth[2]) & proto->psn_mask;
	uint32_t bth_w2 = __be32_to_cpu(ips_lrh->bth[2]) & ~proto->psn_mask;
	uint32_t offset = ips_lrh->hdr_data.u32w0;

	psmi_assert(length > 0);
	psmi_assert(scb->nfrag > 1);
	psmi_assert(scb->nfrag_remaining <= scb->nfrag);
	psmi_assert(scb->nfrag_remaining);
	psmi_assert(scb->nfrag <= ep->chunk_max_segs);
	psmi_assert(frag_size);
	psmi_assert(frag_size <= scb->frag_size);
	psmi_assert(scb->frag_size <= ep->mtu);
	psmi_assert(0 == (frag_size & 0x3) || length <= frag_size);

	while (length) {
		uint32_t len = min(length, frag_size);
		struct ips_message_header *hdr = (struct ips_message_header*)sbuf_gso;
		_HFI_VDBG("copy lrh %p\n", ips_lrh);
		memcpy(sbuf_gso, ips_lrh, sizeof(*ips_lrh));
		// patch up hdr's psn, ACKREQ, offset and len in sbuf_gso
		hdr->bth[2] = __cpu_to_be32(bth_w2 | (psn & proto->psn_mask)
				| (length == len?IPS_SEND_FLAG_ACKREQ:0));
		hdr->hdr_data.u32w0 = offset;
		// Note length includes ICRC but pretend HW added and removed it
		// so we never actually see it
		hdr->lrh[2] = ips_proto_bytes_to_lrh2_be(proto,
                                len + sizeof(*ips_lrh) + HFI_CRC_SIZE_IN_BYTES);

		_HFI_VDBG("copy payload %p %u\n", payload, len);
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
		if (is_gpu_payload) {
			PSM3_GPU_MEMCPY_DTOH(sbuf_gso + sizeof(*ips_lrh),
					payload, len);
		} else
#endif
		{
			memcpy(sbuf_gso+sizeof(*ips_lrh), payload, len);
		}

		if_pf (_HFI_PDBG_ON) {
			_HFI_PDBG_ALWAYS("len %u, remote IP %s, payload %u\n",
				(unsigned)(len+sizeof(*ips_lrh)),
				psm3_sockaddr_fmt((struct sockaddr *)addr, 0),
				len);
			_HFI_PDBG_DUMP_ALWAYS(sbuf_gso, sizeof(*ips_lrh)+ len);
		}
		_HFI_VDBG("UDP send - opcode %x len %u\n",
					_get_proto_hfi_opcode(ips_lrh),
					(unsigned int)(sizeof(*ips_lrh) + len));

		gso_len += sizeof(*ips_lrh) + len;
		sbuf_gso += sizeof(*ips_lrh) + len;
		psn++;
		offset += len;
		payload += len;
		length -= len;
	}

	iov.iov_base = ep->sockets_ep.sbuf_udp_gso;
	iov.iov_len = gso_len;

	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;
	msg.msg_control = control;
	msg.msg_controllen = sizeof(control);
	msg.msg_name = addr;
	msg.msg_namelen = sizeof(psm3_sockaddr_in_t);
	// specify how to segment (segment size)
	struct cmsghdr* cm = CMSG_FIRSTHDR(&msg);
	cm->cmsg_level = SOL_UDP;
	cm->cmsg_type = UDP_SEGMENT;
	cm->cmsg_len = CMSG_LEN(sizeof(uint16_t));
	uint16_t *pcmd = (uint16_t*)CMSG_DATA(cm);
	*pcmd = sizeof(*ips_lrh) + frag_size;

	if_pf (-1 == sendmsg(fd, &msg, ep->sockets_ep.udp_gso_zerocopy ? MSG_ZEROCOPY : 0))
		return -1;
	if (ep->sockets_ep.udp_gso_zerocopy) {
		// flush zerocopy
		memset(&msg, 0, sizeof(msg));
		while (1) {
			if_pt (-1 == recvmsg(fd, &msg, MSG_ERRQUEUE))
				return (errno == EAGAIN) ? 0 : -1;
			msg.msg_flags = 0;
		}
	}
	return 0;
}

// when called:
//		scb->ips_lrh has fixed size PSM header including OPA LRH
//		payload, length is data after header
//		we don't do checksum, let verbs handle that for us
//		the above is in unregistered memory, perhaps even on stack
// for isCtrlMsg, scb is only partially initialized (see ips_scb.h)
// and payload and length may refer to buffers on stack
static inline psm2_error_t
psm3_sockets_udp_spio_transfer_frame(struct ips_proto *proto, struct ips_flow *flow,
			struct ips_scb *scb, uint32_t *payload,
			uint32_t length, uint32_t isCtrlMsg,
			uint32_t cksum_valid, uint32_t cksum
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
			, uint32_t is_gpu_payload
#endif
			)
{
	psm2_error_t ret = PSM2_OK;
	psm2_ep_t ep = proto->ep;
	uint8_t *sbuf = ep->sockets_ep.sbuf;
	unsigned len;
	struct ips_message_header *ips_lrh = &scb->ips_lrh;
	len = sizeof(*ips_lrh) + length;

	psmi_assert(flow->transfer == PSM_TRANSFER_PIO);
#ifdef PSM_FI
	if_pf(PSM3_FAULTINJ_ENABLED_EP(ep)) {
		PSM3_FAULTINJ_STATIC_DECL(fi_sendlost, "sendlost",
				"drop packet before sending",
				1, IPS_FAULTINJ_SENDLOST);
		if_pf(PSM3_FAULTINJ_IS_FAULT(fi_sendlost, ep, " UDP"))
			return PSM2_OK;
	}
#endif // PSM_FI
	PSMI_LOCK_ASSERT(proto->mq->progress_lock);
	psmi_assert_always(! cksum_valid);	// no software checksum yet
	// TBD - we should be able to skip sending some headers such as OPA lrh and
	// perhaps bth (does PSM use bth to hold PSNs? - yes)
	// copy scb->ips_lrh to send buffer
	// when called as part of retransmission:
	//	nfrag remains original total frags in message
	//	nfrag_remaining is fragments left to do (only valid if nfrag>1)
	// if nfrag>1 but nfrag_remaining == 1, don't need to use GSO
	// could just falthrough to sendto because payload, length and ACKREQ
	// are all properly set.  But GSO may allow zerocopy option, so use it.
	psmi_assert(isCtrlMsg || ips_scb_buffer(scb) == payload);
	if (scb->nfrag > 1 /* && scb->nfrag_remaining > 1 */) {
		// when nfrag>1, length and payload_size are just 1st pkt size
		psmi_assert(!isCtrlMsg);
		psmi_assert(length);
		psmi_assert(length <= scb->chunk_size_remaining);
		psmi_assert(scb->payload_size == length);
		if_pf (-1 == psm3_sockets_udp_gso_send(ep->sockets_ep.udp_tx_fd, proto,
					&flow->ipsaddr->sockets.remote_pri_addr,
					scb, (uint8_t*)payload, scb->chunk_size_remaining,
					scb->frag_size
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
					,is_gpu_payload
#endif
					)) {
			if (errno != EAGAIN && errno != EWOULDBLOCK) {
				_HFI_ERROR("UDP GSO send failed on %s: %s\n", ep->dev_name, strerror(errno));
				ret = PSM2_INTERNAL_ERR;
			}
		}
		return ret;
	}

	_HFI_VDBG("copy lrh %p\n", ips_lrh);
	memcpy(sbuf, ips_lrh, sizeof(*ips_lrh));
	// copy payload to send buffer, length could be zero, be safe
	_HFI_VDBG("copy payload %p %u\n",  payload, length);
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
	if (is_gpu_payload) {
		PSM3_GPU_MEMCPY_DTOH(sbuf + sizeof(*ips_lrh),
			payload, length);
	} else
#endif
	{
		memcpy(sbuf+sizeof(*ips_lrh), payload, length);
	}
	_HFI_VDBG("UDP send - opcode %x len %u\n",
				_get_proto_hfi_opcode((struct  ips_message_header*)sbuf), len);
	// we don't support software checksum
	psmi_assert_always(! (proto->flags & IPS_PROTO_FLAG_CKSUM));

	if_pf (ips_lrh->khdr.kdeth0 & __cpu_to_le32(IPS_SEND_FLAG_INTR)) {
		_HFI_VDBG("send solicted event\n");
		// TBD - how to send so wake up rcvthread?  Separate socket?
	}

	if_pf (_HFI_PDBG_ON) {
		_HFI_PDBG_ALWAYS("len %u, remote IP %s payload %u\n",
			len,
			psm3_sockaddr_fmt((struct sockaddr *)&flow->ipsaddr->sockets.remote_pri_addr, 0),
			length);
		_HFI_PDBG_DUMP_ALWAYS(sbuf, len);
	}
	// UDP is datagram oriented, each send is delivered as a single datagram
	// this is unlike TCP which is bytestream oriented.
	if_pf (sendto(ep->sockets_ep.udp_tx_fd, sbuf, len, 0,
		&flow->ipsaddr->sockets.remote_pri_addr,
		sizeof(flow->ipsaddr->sockets.remote_pri_addr)) == -1) {
		if (errno != EAGAIN && errno != EWOULDBLOCK) {
			_HFI_ERROR("UDP send failed on %s: %s\n", ep->dev_name, strerror(errno));
			ret = PSM2_EP_NO_RESOURCES;
		}
	}
	return ret;
}
#endif /* PSM_SOCKETS */
#endif /* _SOCKETS_SPIO_C_ */
