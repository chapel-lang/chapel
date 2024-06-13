#ifdef PSM_SOCKETS
/*

  This file is provided under a dual BSD/GPLv2 license.  When using or
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2021 Intel Corporation.

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

  Copyright(c) 2021 Intel Corporation.

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

/* Copyright (c) 2003-2021 Intel Corporation. All rights reserved. */

#include "psm_user.h"
#include "psm2_hal.h"

#include "ips_epstate.h"
#include "ips_proto.h"
#include "ips_expected_proto.h"
#include "ips_proto_help.h"
#include "ips_proto_internal.h"
#include "sockets_hal.h"
#include <sys/poll.h>

/*
 * Receive header queue initialization.
 */
psm2_error_t
psm3_sockets_recvhdrq_init(const struct ips_epstate *epstate,
		  const struct ips_proto *proto,
		  const struct ips_recvhdrq_callbacks *callbacks,
		  struct ips_recvhdrq *recvq
		)
{
	psm2_error_t err = PSM2_OK;

	memset(recvq, 0, sizeof(*recvq));
	recvq->proto = (struct ips_proto *)proto;
	pthread_spin_init(&recvq->hdrq_lock, PTHREAD_PROCESS_SHARED);

	recvq->epstate = epstate;
	recvq->recvq_callbacks = *callbacks;	/* deep copy */
	SLIST_INIT(&recvq->pending_acks);

	return err;
}

/* receive service routine for each packet opcode starting at
 * OPCODE_RESERVED (C0)
 */
ips_packet_service_fn_t
psm3_sockets_packet_service_routines[] = {
psm3_ips_proto_process_unknown_opcode,	/* 0xC0 */
psm3_ips_proto_mq_handle_tiny,		/* OPCODE_TINY */
psm3_ips_proto_mq_handle_short,		/* OPCODE_SHORT */
psm3_ips_proto_mq_handle_eager,		/* OPCODE_EAGER */
psm3_ips_proto_mq_handle_rts,		/* OPCODE_LONG_RTS */
psm3_ips_proto_mq_handle_cts,		/* OPCODE_LONG_CTS */
psm3_ips_proto_mq_handle_data,		/* OPCODE_LONG_DATA */
psm3_ips_proto_process_unknown_opcode,	/* C7 */
psm3_ips_proto_process_unknown_opcode,	/* C8 */

/* these are control packets */
psm3_ips_proto_process_ack,		/* OPCODE_ACK */
psm3_ips_proto_process_nak,		/* OPCODE_NAK */
psm3_ips_proto_process_unknown_opcode,	/* CB */
psm3_ips_proto_process_err_chk,		/* OPCODE_ERR_CHK */
psm3_ips_proto_process_unknown_opcode,	/* CD */
psm3_ips_proto_connect_disconnect,	/* OPCODE_CONNECT_REQUEST */
psm3_ips_proto_connect_disconnect,	/* OPCODE_CONNECT_REPLY */
psm3_ips_proto_connect_disconnect,	/* OPCODE_DISCONNECT__REQUEST */
psm3_ips_proto_connect_disconnect,	/* OPCODE_DISCONNECT_REPLY */

/* rest are not control packets */
psm3_ips_proto_am,			/* OPCODE_AM_REQUEST_NOREPLY */
psm3_ips_proto_am,			/* OPCODE_AM_REQUEST */
psm3_ips_proto_am			/* OPCODE_AM_REPLY */

/* D5-DF (OPCODE_FUTURE_FROM to OPCODE_FUTURE_TO) reserved for expansion */
};

/*---------------------------------------------------------------------------*/
/* TCP specific code */
#define MSG_HDR_SIZE sizeof(struct ips_message_header)

#define if_incomplete(RECVLEN, EXPLEN)  \
	if ((RECVLEN > 0 && RECVLEN < EXPLEN) || (RECVLEN < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)))

#define if_complete(RECVLEN, EXPLEN) if (RECVLEN > 0 && RECVLEN >= EXPLEN)

static __inline__ uint8_t
psm3_hdr_sanity_check(uint8_t *buf, uint32_t size)
{
	int count = min(size, sizeof(EXP_HDR)) - 1;
	for (; count >= 0; count--) {
		if ((buf[count] ^ EXP_HDR[count]) & EXP_HDR_MASK[count]) {
			return PSMI_FALSE;
		}
	}
	return PSMI_TRUE;
}

static __inline__ ssize_t psm3_sockets_tcp_aggressive_recv(int fd, uint8_t *buf, uint32_t size, int aggressive) {
	uint32_t remainder  = size;
	uint8_t *rbuf = buf;
	int ret;
	while (remainder) {
#ifdef PSM_FI
		uint32_t part_len = remainder;
		if_pf(PSM3_FAULTINJ_ENABLED()) {
			PSM3_FAULTINJ_STATIC_DECL(fi_recvpart, "recvpart",
					"partial TCP recv",
					1, IPS_FAULTINJ_RECVPART);
			if_pf(PSM3_FAULTINJ_IS_FAULT(fi_recvpart, NULL, ""))
				part_len = min(remainder, 32);	// purposely less than min pkt size
		}
		ret = recv(fd, rbuf, part_len, MSG_DONTWAIT);
#else
		ret = recv(fd, rbuf, remainder, MSG_DONTWAIT);
#endif
		PSM2_LOG_MSG("recv fd=%d ret=%d", fd, ret);
		if (aggressive == 0) {
			return ret;
		}
		if (ret > 0) {
			remainder -= ret;
			rbuf += ret;
#ifdef PSM_FI
			if (part_len != remainder + ret)	// fault was injected
				return size - remainder;
#endif
		} else if (ret < 0 && remainder < size) {
			return size - remainder;
		} else {
			return ret;
		}
	}
	return size;
}

static __inline__ ssize_t drain_tcp_stream(int fd, uint8_t *buf, uint32_t buf_size, uint32_t drain_size) {
	uint32_t remainder  = drain_size;
	int ret;
	while (remainder) {
		ret = recv(fd, buf, buf_size < remainder ? buf_size : remainder, MSG_DONTWAIT);
		if (ret <= 0) {
			if (errno == EAGAIN || errno == EWOULDBLOCK) {
				_HFI_VDBG("No more data to read. Try to poll again.\n");
				// simple solution - try to finish the reading
				int old_errno = errno;
				struct pollfd pfd = {
					.fd = fd,
					.events = POLLIN
				};
				if (poll(&pfd, 1, TCP_POLL_TO) == 1 && (pfd.revents & POLLIN)) {
					continue;
				};
				errno = old_errno;
			}
			return ret;
		} else {
			remainder -= ret;
		}
	}
	return drain_size;
}

#define TCPRCV_OK 0
#define TCPRCV_PARTIAL -1
#define TCPRCV_DISCARD -2
#define TCPRCV_INVALID -3

static __inline__ void
psm3_sockets_tcp_preprocess_packet(psm2_ep_t ep, int fd, struct ips_recvhdrq_event *rcv_ev, struct fd_ctx* ctx)
{
	uint8_t *buf = ctx->shrt_buf + ctx->offset;
	rcv_ev->p_hdr = (struct ips_message_header *)buf;
	psmi_assert(ep->sockets_ep.rbuf_cur_fd == 0);
	psmi_assert(ips_proto_lrh2_be_to_bytes(rcv_ev->proto, rcv_ev->p_hdr->lrh[2]) > ep->sockets_ep.shrt_buf_size);

	if (!TCP_DIRECT_USR_BUF) {
		goto out;
	}

	uint8_t opcode = _get_proto_hfi_opcode(rcv_ev->p_hdr);
	if_pf (opcode != OPCODE_SHORT && opcode != OPCODE_EAGER) {
		goto out;
	}

	struct ips_epstate_entry *epstaddr =
		ips_epstate_lookup(rcv_ev->recvq->epstate, rcv_ev->p_hdr->connidx);
	if_pf((epstaddr == NULL) || (epstaddr->ipsaddr == NULL)) {
		goto out;
	}

#if !defined(PSM_CUDA) && !defined(PSM_ONEAPI)
	psm2_mq_req_t req = psm3_mq_req_match(rcv_ev->proto->mq,
		(psm2_epaddr_t) &epstaddr->ipsaddr->msgctl->master_epaddr,
		(psm2_mq_tag_t *) rcv_ev->p_hdr->tag, 0);
	if (req) {
		uint32_t offset = 0;
		if (opcode == OPCODE_EAGER) {
			offset = rcv_ev->p_hdr->hdr_data.u32w0;
		}
		// we can potentially switch from rbuf to user buffer, i.e.
		// extra_buf = rbuf. But with the current logic, it will not
		// happen because when extra_buf != NULL, we just continue read
		// data into it.
		memcpy((uint8_t*)req->req_data.buf + offset,
			ctx->extra_buf ? ctx->extra_buf : buf + MSG_HDR_SIZE,
			ctx->len - MSG_HDR_SIZE);
		ctx->extra_buf = (uint8_t*)req->req_data.buf + offset;
		// Different fds may hit the same user buffer. We need to stick on it
		// until get all data and process it. So we set rbuf_cur_fd here.
		ep->sockets_ep.rbuf_cur_fd = fd;
		_HFI_VDBG("Use user buffer - req=%p user_buffer=%p offset=%d data copied=%ld\n",
			req, req->req_data.buf, offset, ctx->len - MSG_HDR_SIZE);
	}
#endif
out:
	if (ctx->extra_buf == NULL) {
		// move payload to rbuf, will continue read rest data into rbuf
		ctx->extra_buf = ep->sockets_ep.rbuf;
		memcpy(ctx->extra_buf, ctx->shrt_buf + ctx->offset + MSG_HDR_SIZE,
			ctx->len - MSG_HDR_SIZE);
		ep->sockets_ep.rbuf_cur_fd = fd;
		_HFI_VDBG("Switch to rbuf fd=%d\n", fd);
	}

	return;
}

/**
 * Receive byte stream from TCP. We use both short buffer and global large buffer
 * to receive data. Each connection (fd) uses its own short buffer to receive data.
 * If incoming message is larger than the short buffer, it switches to use the
 * global buffer to store and read payload. Since the global buffer are shared among
 * connections (fds), once one connection (fd) uses it, other connections (fds) can
 * not use it, but they still can use their own short buffers. This hybrid buffer
 * solution provides better performance on small msgs (msg size < short buffer size)
 * because it has no head of line blocking issue, and also has control on total
 * memory consumed because it uses a global buffer for middle and large msgs. The
 * default short buffer size is TCP_SHRT_BUF_SIZE (512) bytes. A user can adjust it
 * with PSM3_TCP_SHRTBUF_SIZE. Setting it to zero will turn off short buffer feature
 * and only use global buffer to receive data. The short buffer will point to the global
 * buffer and each fd will use it exclusively. When use global buffer, we may change
 * to use user buffer if available and directly read data into it. Different fds may
 * hit the same user buffer, so when we use a user buffer we still lock other fds
 * from using the global buffer. We can potentially improve to only lock among fds
 * that use the same user buffer. Real APPs usually do not use too much large msgs
 * so the value might be limited.
 * To help manage short buffer, we introduced structure fd_ctx and maintain it on
 * fd maps. Below is the detailed read data logic:
 *
 * Normal/Fresh data read
 * 1) read data into short buffer (ctx->shrt_buf) as much as we can
 * 2) if received length is less than ips_message_header size, update ctx->len to
 *    record data read so far. The next progress call will call this function to
 *    continue the read. See "Partial data read" described below.
 * 3) if data received contains ips_message_header, get pktlen from it.
 *    3.1) if received data length is larger than pktlen, we got a whole PSM
 *         pkt, and have extra data for next pkts. Record pktlen in ctx->pkt_len.
 *         The progress call will go ahead process the data and when success it
 *         update ctx->len by decreasing it with pktlen. If ctx->len is still large
 *         than zero, it means more data available. The progress call will keep
 *         calling this function to fetch next pkt and process it until no more
 *         extra pkt or reach the last pkt partially read.
 *         See "Extra data read" described below
 *    3.2) if received data length is less than pktlen, we got partial PSM pkt,
 *         update ctx->len. If the short buffer is full, try to pre-process it
 *         that will change to use wither user buffer or global buffer (rbuf).
 *         See "Partial data pre-process" below. Since short buffer is full, it may
 *         happen that there are more data in socket. So we try to read remaining
 *         data immediately. If the short buffer is not full, we just return, and
 *         the next progress call will continue the read.
 *         See "Partial data read" described below.
 *    3.3) if received data length is pktlen, we got the whole pkt, update ctx->pkt_len
 *         and return TCPRCV_OK. The progress call will process the pkt.
 *
 *
 * Partial data pre-process
 * 1) Check pkt, and only go to next step if the pkt is SHORT or EAGER msg. Otherwise
 *    go to step 3).
 * 2) Try to get matched psm2_mq_req_t based on pkt hdr. If get psm2_mq_req_t,
 *    get user buffer and copy payload received so far into it. Update ctx->extra_buff
 *    to the user buffer, so in the next progress call, we will continue read in
 *    user buffer. We also set rbuf_cur_fd to exclusively use the user buffer.
 * 3) If ctx->extra_buff is null, we try to use rbuf. Copy payload received so
 *    far into it. Update ctx->extra_buf to rbuf, and set rbuf_cur_fd to exclusively
 *    use rbuf.
 *
 * Partial data read
 * 1) If data received so far contains msg hdr, get pktlen from the hdr.
 *    1.1) if ctx->extra_buff is not NULL, we just continue read the remaining
 *         data into ctx->extra_buff that can be either a user buffer or rbuf
 *    1.2) if pktlen larger than short buffer size, pre-process pkt to shift to
 *         using user buffer or rbuf. See above "Partial data pre-process".
 *         Then read the remaining data.
 *    1.3) if short buffer has no enough free space to store remaining pkt, move
 *         data received so far to the beginning of short buffer, then read remaining
 *         data into short buffer
 *    1.4) otherwise, there are enough space in short buffer, we just read remaining
 *         data into short buffer
 * 2) If no full msg hdr, since we only get tiny data so far (< 56 bytes), we
 *    move data received so far to the beginning of short buffer. This should not
 *    cause too much overhead. And then we read as much as we can in short buffer,
 *    i.e. short buffer size - ctx->len
 *
 * Extra data fetch (only happens on short buffer, note when we turn off short buffer,
 * it points to rbuf)
 * 1) if the length of data available, i.e. ctx->len >= ips_message_header size,
 *    get pktlen from header
 *    1.1) if ctx->len >= pktlen, we have the whole pkt in short buffer. Set
 *         ctx->pkt_len = pktlen, and recvlen = pktlen, then return TCPRCV_OK.
 *         The progress call will get the whole msg based on ctx->offset and ctx->pkt_len.
 *    1.2) if ctx->len < pktlen, we only have partial pkt. set recvlen = ctx->len,
 *         then return TCPRCV_PARTIAL. The progress call will skip this fd, and
 *         continue read data from other fds.
 * 2) if extra data length < ips_message_header size, we only have partial pkt.
 *    set recvlen = ctx->len, then return TCPRCV_PARTIAL. The progress call will
 *    skip this fd, and continue read data from other fds.
 *
 * If we turn off short buff to use rbuf, in above operations, when return
 * TCPRCV_PARTIAL we always set rbuf_cur_fd to exclusively use the rbuf.
 * In below implementation, we combined above operations together to make our code
 * to be more efficient.
 */
static __inline__ int
psm3_sockets_tcp_recv(psm2_ep_t ep, int fd, struct ips_recvhdrq_event *rcv_ev, struct fd_ctx* ctx, int fetch, int* ret_recvlen)
{
	uint8_t *buf;
	uint8_t *rbuf;
	uint32_t explen, acplen;
	uint32_t pktlen = 0;
	int recvlen, ret;

#ifdef PSM_DEBUG
	// when use rbuf only, i.e. shrt_buf_size = 0
	if (ep->sockets_ep.shrt_buf_size == 0) {
	 	if (ep->sockets_ep.rbuf_cur_fd == 0 && fetch == 0) {
	 		// a fresh read shall read data into the beginning
	 		// of rbuf, i.e. ctx->len=0, ctx->offset=0.
			psmi_assert_always(ctx->len == 0);
			psmi_assert_always(ctx->offset == 0);
		}
		// rbuf_cur_fd shall be ZERO, or point to the fd
		if (ep->sockets_ep.rbuf_cur_fd) {
			psmi_assert_always(ep->sockets_ep.rbuf_cur_fd == fd);
		}
	}
#endif

	uint32_t shrt_buf_size = ep->sockets_ep.shrt_buf_size ?
		ep->sockets_ep.shrt_buf_size /* use shrt_buf */
		: ep->sockets_ep.buf_size; /* use rbuf */
receive:
	ret = TCPRCV_OK;
	buf = ctx->shrt_buf + ctx->offset;
	if (ctx->len >= MSG_HDR_SIZE) {
		pktlen = ips_proto_lrh2_be_to_bytes(rcv_ev->proto,
			((struct ips_message_header *)buf)->lrh[2]);
		if (ctx->len >= pktlen) {
			psmi_assert(ctx->extra_buf == NULL);
			// already have a pkt
			ctx->pkt_len = pktlen;
			_HFI_VDBG("Next Pkt: ctx{offset=%d;len=%d;pkt_len=%d} fd=%d\n",
				ctx->offset, ctx->len, ctx->pkt_len, fd);
			recvlen = pktlen;
			goto out;
		}
		if (fetch) {
			psmi_assert(ctx->extra_buf == NULL);
			recvlen = ctx->len;
			ret = TCPRCV_PARTIAL;
			goto pout;
		}
		// partial pkt
		explen = pktlen - ctx->len;
		if (ctx->extra_buf) {
			// read rest data into extra_buf
			rbuf = ctx->extra_buf + ctx->len - MSG_HDR_SIZE;
			_HFI_VDBG("Continue partial read in extra_buf: remainder=%d fd=%d\n", explen, fd);
		} else if (pktlen > shrt_buf_size) {
			// do not fit in shrt_buf, need to use user buffer or rbuf
			if (ep->sockets_ep.rbuf_cur_fd) {
				// can not use extra_buf, directly return without reading
				recvlen = ctx->len;
				rcv_ev->recvq->proto->stats.rcv_hol_blocking++;
				// have partial data to read, but the fd is not expected, skip it
				_HFI_VDBG("Skip fd=%d, expected fd=%d\n", fd, ep->sockets_ep.rbuf_cur_fd);
				ret = TCPRCV_PARTIAL;
				goto pout;
			}
			psm3_sockets_tcp_preprocess_packet(ep, fd, rcv_ev, ctx);
			rbuf = ctx->extra_buf + ctx->len - MSG_HDR_SIZE;
		} else if (ctx->offset + pktlen > shrt_buf_size) {
			// no enough free space in shrt_buf
			_HFI_VDBG("Move data to head: dest=%p src=%p len=%d\n",
				ctx->shrt_buf, ctx->shrt_buf + ctx->offset, ctx->len);
			memmove(ctx->shrt_buf, ctx->shrt_buf + ctx->offset, ctx->len);
			ctx->offset = 0;
			buf = ctx->shrt_buf;
			rbuf = buf + ctx->len;
		} else {
			// read rest data into shrt_buf
			rbuf = buf + ctx->len;
			_HFI_VDBG("Continue partial read in shrt_buf: remainder=%d fd=%d\n", explen, fd);
		}
		acplen = explen;
	} else if (ctx->len > 0) {
		if (fetch) {
			psmi_assert(ctx->extra_buf == NULL);
			recvlen = ctx->len;
			ret = TCPRCV_PARTIAL;
			goto pout;
		}
		// partial pkt without hdr
		if (ctx->offset) {
			// move data to the beginning of shrt_buf so we can read in more data.
			// This shall be fine because data to move is tiny, ctex->len < 56.
			_HFI_VDBG("Move data to head: dest=%p src=%p len=%d\n", ctx->shrt_buf, buf, ctx->len);
			memmove(ctx->shrt_buf, buf, ctx->len);
			ctx->offset = 0;
			buf = ctx->shrt_buf;
		}
		rbuf = buf + ctx->len;
		// Try to read as much as we can
		explen = shrt_buf_size - ctx->len;
		acplen = MSG_HDR_SIZE - ctx->len;
	} else {
		psmi_assert(fetch == 0); // fetch shall only applies on ctx->len > 0
		// fresh read
		rbuf = buf;
		// Try to read as much as we can
		explen = shrt_buf_size;
		acplen = MSG_HDR_SIZE;
	}
	recvlen = psm3_sockets_tcp_aggressive_recv(fd, rbuf, explen, 0);
	_HFI_VDBG("Got %d ctx{offset=%d;len=%d;pkt_len=%d} explen=%d acplen=%d from fd=%d\n",
		recvlen, ctx->offset, ctx->len, ctx->pkt_len, explen, acplen, fd);
	// PSM header sanity check for the first pkt that shall be a CONN_REQ msg
	// this code is introduced for the case that a 3rd party, such as a security scanner,
	// may directly connect to an opened port and send some test data. the 3rd party may
	// send data less than PSM header size that will be treated as partial data. For this
	// case flow level shrt_buff is required because we want to block on this connection
	// only. And we also need to do sanity check on any data we received so far.
	if_pf (ctx->state == FD_STATE_READY && recvlen > 0) {
		if (psm3_hdr_sanity_check(ctx->shrt_buf, ctx->len + recvlen)) {
			if (ctx->len + recvlen >= sizeof(EXP_HDR)) {
				_HFI_VDBG("Header sanity check success. ctx->state=FD_STATE_VALID\n");
				ctx->state = FD_STATE_VALID;
			}
		} else {
			return TCPRCV_INVALID;
		}
	}

	if_incomplete (recvlen, acplen) {
		// partial data
		psmi_assert(ctx->pkt_len == 0);
		if (recvlen > 0) {
			ctx->len += recvlen;
		}
		_HFI_VDBG("Partial data: offset=%d, pktlen=%d recvlen=%d explen=%d fd=%d\n",
			ctx->offset, ctx->pkt_len, recvlen, explen, fd);
		ret = TCPRCV_PARTIAL;
		goto pout;
	}
	if_complete (recvlen, acplen) {
		// got header
		pktlen = ips_proto_lrh2_be_to_bytes(rcv_ev->proto,
			((struct ips_message_header *)buf)->lrh[2]);
		if (ctx->len < MSG_HDR_SIZE) {
			psmi_assert(ctx->offset == 0);
			psmi_assert(ctx->extra_buf == NULL);
			if_pf (pktlen > ep->sockets_ep.buf_size) {
				// shouldn't happen
				_HFI_ERROR( "unexpected large recv fd=%d: pktlen=%u buf_size=%u on %s\n",
						fd, pktlen, ep->sockets_ep.buf_size, ep->dev_name);
				psm3_ips_proto_dump_frame((uint8_t*)buf, sizeof(struct ips_message_header),
					"bad header");
				// no need to drain, we will close the socket
//				ret = drain_tcp_stream(fd, buf, ep->sockets_ep.buf_size,
//						pktlen - MSG_HDR_SIZE);
				ret = TCPRCV_DISCARD;
				goto out;
			}
			ctx->len += recvlen;
			if (ctx->len >= pktlen) {
				ctx->pkt_len = pktlen;
				_HFI_VDBG("Got %d + %d data from fd=%d on %s\n",
						pktlen, ctx->len - pktlen, fd, ep->dev_name);
				recvlen = pktlen;
			} else if (ctx->len < pktlen) {
				_HFI_VDBG("New partial data: rcvlen=%d, pktlen=%d fd=%d\n", ctx->len, ctx->pkt_len, fd);
				recvlen = ctx->len;
				if (ep->sockets_ep.shrt_buf_size && ctx->len == shrt_buf_size &&
					ep->sockets_ep.rbuf_cur_fd == 0) {
					// shrt_buf is full, may have more data in socket to read
					// move data to user buffer or rbuf and then continue read
					psm3_sockets_tcp_preprocess_packet(ep, fd, rcv_ev, ctx);
					goto receive;
				}
				ret = TCPRCV_PARTIAL;
				goto pout;
			}
		} else {
			// get all remainder data
			ctx->len += recvlen;
			ctx->pkt_len = pktlen;
			recvlen = ctx->len;
		}
	}
out:
	*ret_recvlen = recvlen;
	return ret;
pout:
	if (ep->sockets_ep.shrt_buf_size == 0) {
		ep->sockets_ep.rbuf_cur_fd = fd;
	}
	goto out;
}

static __inline__ int
psm3_sockets_tcp_process_packet(struct ips_recvhdrq_event *rcv_ev,
               psm2_ep_t ep, int fd, struct fd_ctx* ctx,
               psm3_sockaddr_in_t *rem_addr,
               struct ips_recvhdrq *recvq, int flush)
{
	int ret = IPS_RECVHDRQ_CONTINUE;
	uint8_t opcode = _get_proto_hfi_opcode(rcv_ev->p_hdr);

	_HFI_VDBG("TCP receive - opcode %x on %s\n", opcode, ep->dev_name);

	PSM2_LOG_MSG("Process PKT: opcode=%x payload_size=%d", opcode, rcv_ev->payload_size);
	PSM2_LOG_PKT_STRM(PSM2_LOG_RX,rcv_ev->p_hdr,"PKT_STRM:");
	struct ips_epstate_entry *epstaddr =
		ips_epstate_lookup(recvq->epstate, rcv_ev->p_hdr->connidx);

	if_pf((epstaddr == NULL) || (epstaddr->ipsaddr == NULL)) {
		rcv_ev->ipsaddr = NULL;
		ep->sockets_ep.tcp_incoming_fd = fd;
		recvq->recvq_callbacks.callback_packet_unknown(rcv_ev);
		ep->sockets_ep.tcp_incoming_fd = 0;
#if 0
	} else if_pf (psm3_sockaddr_cmp(&epstaddr->ipsaddr->sockets.remote_pri_addr, rem_addr)) {
		// TBD - we could also compare sin6_scope_id here or in
		// psm3_sockaddr_cmp to confirm interface used
		// TBD - we get some packets from unexpected sockets
		// occurs under higher stress
		_HFI_ERROR("mismatched IP %s got %s on %s\n",
				psm3_sockaddr_fmt((struct sockaddr *)&epstaddr->ipsaddr->sockets.remote_pri_addr, 0),
				psm3_sockaddr_fmt((struct sockaddr *)&rem_addr, 1),
				ep->dev_name);
		// TBD - we really want a variation of callback which will
		// not check for attempted connect REQ/DREQ, but do the rest
		rcv_ev->ipsaddr = NULL;
		recvq->recvq_callbacks.callback_packet_unknown(rcv_ev);
#endif
	} else {
		rcv_ev->ipsaddr = epstaddr->ipsaddr;
		ep->sockets_ep.tcp_incoming_fd = fd;
		psmi_assert(PSMI_HOWMANY(psm3_sockets_packet_service_routines)
				== OPCODE_FUTURE_FROM - OPCODE_RESERVED);
#ifndef PSM_TCP_ACK
		// TCP doesn't need ACK so simulate an ack for all packets sent
		// thus far. This essentially disables process_ack processing and
		// in rare cases involving TCP partial sends and remainder handling
		// this will delay ack processing until tcp_spio_transfer_frame has
		// reported send is done (such reports can be late due to partial send
		// handling and remainder).  The upside is it protects against
		// a very slow return path delivering a very stale PSN (> 2 billion
		// packets sent while return packet in flight).  OPA and verbs would
		// not have such an issue since lack of received acks would pace
		// them via PSM credit mechanism.
		if (opcode >= OPCODE_TINY && opcode <= OPCODE_LONG_DATA) {
			ips_epaddr_flow_t flowid = ips_proto_flowid(rcv_ev->p_hdr);
			psmi_assert(flowid < EP_NUM_FLOW_ENTRIES);
			struct ips_flow *flow = &rcv_ev->ipsaddr->flows[flowid];
			// ack_seq_num is last received+1 and xmit_ack_num
			// is last acked+1, so this simulates ack up to
			// xmit_ack_num-1 (e.g. the last packet we sent).  Net result is
			// the process_ack call after handling the inbound packet is a noop
			rcv_ev->p_hdr->ack_seq_num = flow->xmit_ack_num.psn_num;
		}
#endif
		ret = ips_proto_process_packet(rcv_ev,
			psm3_sockets_packet_service_routines);
#ifdef TCP_RCV_FLUSH
#ifndef PSM_TCP_ACK
		// When running ack-less we can miss the opportunity to immediately
		// take advantage of send buffer space now available due to the
		// receipt of TCP acks carried with the packet we just received.
		// So if caller had gotten a new packet from the wire we try to
		// make some send progress here (just like process_ack would if
		// we were running with acks).  However we only do this for
		// data packets (while connecing won't have anything queued
		// and disconnect packets could have freed rcv_ev->ipsaddr)
		if (flush && opcode >= OPCODE_TINY && opcode <= OPCODE_LONG_DATA) {
			ips_epaddr_flow_t flowid = ips_proto_flowid(rcv_ev->p_hdr);
			psmi_assert(flowid < EP_NUM_FLOW_ENTRIES);
			struct ips_flow *flow = &rcv_ev->ipsaddr->flows[flowid];
			if (!SLIST_EMPTY(&flow->scb_pend))
				flow->flush(flow, NULL);
		}
#endif
#endif
		ep->sockets_ep.tcp_incoming_fd = 0;
		if_pf (ret == IPS_RECVHDRQ_REVISIT)
		{
			// try processing on next progress call
			_HFI_VDBG( "REVISIT returned on process_packet\n");
			// normally PSM would stop processing the header Q and
			// poll the same entry again.  We can't do that with a
			// UDP socket, so we stash the buffer and payload length
			// in our ep and will revisit it next time we are called
			recvq->proto->stats.rcv_revisit++;
			ctx->revisit = 1;
			if (ep->sockets_ep.shrt_buf_size == 0) {
				ep->sockets_ep.rbuf_cur_fd = fd;
			}

			if_pf (ep->sockets_ep.nrfd == ep->sockets_ep.max_rfds) {
				ep->sockets_ep.max_rfds += TCP_INC_CONN;
				_HFI_VDBG("Max rfds = %d\n", ep->sockets_ep.max_rfds);
				ep->sockets_ep.rfds = psmi_realloc(ep, NETWORK_BUFFERS,
					ep->sockets_ep.rfds, ep->sockets_ep.max_rfds * sizeof(int));
				if (ep->sockets_ep.rfds == NULL) {
					_HFI_ERROR( "Unable to allocate memory for revisit fds\n");
					return IPS_RECVHDRQ_ERROR;
				}
			}
			ep->sockets_ep.rfds[ep->sockets_ep.nrfd] = fd;
			ep->sockets_ep.nrfd += 1;

			return ret;
		}
	}

	// when get DISCONN_REQ, we may close fd and free fd_ctx
	// we shall update fd_ctx only when it's still available
	if_pf (ep->sockets_ep.map_fds[fd] == NULL) {
		return ret;
	}

	// update fd_ctx
	ctx->len -= ctx->pkt_len;
	if_pf (ctx->len) {
		ctx->offset += ctx->pkt_len;
	} else {
		ctx->offset = 0;
	}
	if (ep->sockets_ep.rbuf_cur_fd == fd) {
		ep->sockets_ep.rbuf_cur_fd = 0;
	}
	ctx->pkt_len = 0;
	ctx->extra_buf = NULL;
	ctx->revisit = 0;

	return ret;
}

static __inline__ int
psm3_sockets_tcp_revisit(psm2_ep_t ep, int fd, struct fd_ctx* ctx, struct ips_recvhdrq_event *rcv_ev,
	struct ips_recvhdrq *recvq, struct sockaddr_storage *rem_addr, uint32_t *num_done)
{
	psmi_assert(ctx->revisit);
	int ret = IPS_RECVHDRQ_CONTINUE;
	int rcv_state;
	int recvlen;
	uint8_t *buf;
	while (ctx->len) {
		buf = ctx->shrt_buf + ctx->offset;
		rcv_ev->payload_size = ctx->pkt_len - MSG_HDR_SIZE;
		rcv_ev->p_hdr = (struct ips_message_header *)buf;
		if (ctx->extra_buf) {
			rcv_ev->payload = ctx->extra_buf;
		} else {
			rcv_ev->payload = buf + MSG_HDR_SIZE;
		}
		ret = psm3_sockets_tcp_process_packet(rcv_ev, ep, fd, ctx,
			(psm3_sockaddr_in_t *)&rem_addr,
			recvq, 0);
		_HFI_VDBG("Process revisit pkt. ctx{offset=%d;len=%d;pkt_len=%d} ret=%d rbuf_cur_fd=%d fd=%d\n",
			ctx->offset, ctx->len, ctx->pkt_len, ret, ep->sockets_ep.rbuf_cur_fd, fd);
		if_pf (ret == IPS_RECVHDRQ_REVISIT || ret == IPS_RECVHDRQ_ERROR) {
			break;
		}
		*num_done += 1;
		if (ctx->len == 0)  {
			break;
		}
		// may have more pkts to process
		rcv_state = psm3_sockets_tcp_recv(ep, fd, rcv_ev, ctx, 1, &recvlen);
		_HFI_VDBG("Get remainder revisit pkt. ctx{offset=%d;len=%d;pkt_len=%d} rcv_state=%d fd=%d\n",
			ctx->offset, ctx->len, ctx->pkt_len, rcv_state, fd);
		if (rcv_state == TCPRCV_PARTIAL) {
			recvq->proto->stats.partial_read_cnt++;
			break;
		}
		// has more pkts, continue to process them
	}
	return ret;
}

static __inline__ psm2_error_t
psm3_sockets_tcp_accept(psm2_ep_t ep, int fd,
	struct sockaddr_storage *rem_addr, socklen_t *len_addr)
{
	int incoming_fd;
	while(1) {
		incoming_fd = accept(fd, (struct sockaddr *)rem_addr, len_addr);
		if (incoming_fd < 0) {
			if (errno != EWOULDBLOCK && errno != EAGAIN) {
				_HFI_ERROR("failed accept '%s' (%d) on %s epid %s\n",
					strerror(errno), errno, ep->dev_name, psm3_epid_fmt_internal(ep->epid, 0));
				return PSM2_INTERNAL_ERR;
			}
			break;
		}
		// coverity[uninit_use_in_call] - rem_addr initialized in accept() call above
		_HFI_PRDBG("Accept connection (fd=%d) from %s on %s epid %s\n", incoming_fd,
			psm3_sockaddr_fmt((struct sockaddr *)rem_addr, 0),
			ep->dev_name, psm3_epid_fmt_internal(ep->epid, 1));
		PSM2_LOG_MSG("Accept connection (fd=%d) from %s on %s epid %s", incoming_fd,
			psm3_sockaddr_fmt((struct sockaddr *)rem_addr, 0),
			ep->dev_name, psm3_epid_fmt_internal(ep->epid, 1));
		if (psm3_sockets_tcp_add_fd(ep, incoming_fd, FD_STATE_READY) != PSM2_OK) {
			// shouldn't happen
			close(incoming_fd);
			return PSM2_INTERNAL_ERR;
		}
	}
	return PSM2_OK;
}

static __inline__ int
psm3_sockets_tcp_aux_rcv(psm2_ep_t ep, int fd, struct fd_ctx* ctx,
	struct ips_recvhdrq_event *rcv_ev, struct ips_recvhdrq *recvq,
	struct sockaddr_storage *rem_addr, socklen_t *len_addr)
{
	*len_addr = sizeof(rem_addr);
	// UDP is datagram. we shall get the whole pkt.
	psmi_assert(ctx->offset == 0 && ctx->len == 0);
	uint8_t *buf = ctx->shrt_buf;
	int recvlen = recvfrom(ep->sockets_ep.udp_rx_fd, buf, TCP_SHRT_BUF_SIZE,
				MSG_DONTWAIT|MSG_TRUNC,
				(struct sockaddr *)rem_addr, len_addr);
	_HFI_VDBG("Got %d ctx{offset=%d;len=%d;pkt_len=%d} fd=%d",
		recvlen, ctx->offset, ctx->len, ctx->pkt_len, fd);
	if (recvlen < 0) {
		if (errno == EAGAIN || errno == EWOULDBLOCK) {
			return IPS_RECVHDRQ_NO_PROGRESS;
		} else {
			// TBD - how to best handle errors
			_HFI_ERROR("failed aux recv '%s' (%d) on %s epid %s\n",
				strerror(errno), errno, ep->dev_name, psm3_epid_fmt_internal(ep->epid, 0));
			return IPS_RECVHDRQ_ERROR;
		}
	}
	if_pf (recvlen < MSG_HDR_SIZE) {
		_HFI_ERROR( "unexpected small recv: %u on %s fd=%d\n", recvlen, ep->dev_name, fd);
		return IPS_RECVHDRQ_CONTINUE;
	}
	rcv_ev->p_hdr = (struct ips_message_header *)buf;
	rcv_ev->payload = (buf + MSG_HDR_SIZE);
	rcv_ev->payload_size = recvlen - MSG_HDR_SIZE;
	if_pf (_HFI_VDBG_ON) {
		_HFI_VDBG("got recv %u bytes from IP %s payload_size=%d opcode=%x\n", recvlen,
			psm3_sockaddr_fmt((struct sockaddr *)rem_addr, 0),
			rcv_ev->payload_size,
			_get_proto_hfi_opcode((struct ips_message_header *)buf));
	}
	if_pf (_HFI_PDBG_ON) {
		_HFI_PDBG_DUMP_ALWAYS(buf, recvlen);
	}
	ctx->len = recvlen;
	ctx->pkt_len = ips_proto_lrh2_be_to_bytes(rcv_ev->proto, rcv_ev->p_hdr->lrh[2]);
	return psm3_sockets_tcp_process_packet(rcv_ev, ep, fd, ctx,
				(psm3_sockaddr_in_t *)&rem_addr,
				recvq, 1);
}

psm2_error_t psm3_sockets_tcp_recvhdrq_progress(struct ips_recvhdrq *recvq, bool force)
{
	GENERIC_PERF_BEGIN(PSM_RX_SPEEDPATH_CTR); /* perf stats */

	int ret = IPS_RECVHDRQ_CONTINUE;
	int recvlen = 0;
	psm2_ep_t ep = recvq->proto->ep;
	struct sockaddr_storage rem_addr = {0};
	socklen_t len_addr = sizeof(rem_addr);
	PSMI_CACHEALIGN struct ips_recvhdrq_event rcv_ev = {
		.proto = recvq->proto,
		.recvq = recvq,
		//.ptype = RCVHQ_RCV_TYPE_ERROR
	};
	uint32_t num_done = 0;

	int rc, i, fd;
	int new_accept, fetch = 0;
	int rcv_state = TCPRCV_OK;
	struct fd_ctx* ctx;
#ifdef PSM_TCP_POLL
	int to_adjust_fds;
	// when we have a partial read, we put the fd to the beginning of the fds after
	// the listener fd and aux fd, i.e. sw_id starts with 2.
	int sw_id = 2; // index of the fd that we will switch with
#endif

	int nrfd = ep->sockets_ep.nrfd;
	ep->sockets_ep.nrfd = 0;
	for (i = 0; i < nrfd; i++) {
		fd = ep->sockets_ep.rfds[i];

		ctx = psm3_sockets_get_fd_ctx(ep, fd);
		if_pf (ctx == NULL) {
			// when process DISCONN on a different UDP socket/fd we may
			// close fd and set its ctx to NULL.
			continue;
		}

		_HFI_VDBG("Process revisit pkt fd=%d\n", fd);
		ret = psm3_sockets_tcp_revisit(ep, fd, ctx,
			&rcv_ev, recvq, &rem_addr, &num_done);
		if_pf (ret == IPS_RECVHDRQ_REVISIT) {
			ep->sockets_ep.poll_count = ep->sockets_ep.active_skip_polls_offset;
			if (ep->sockets_ep.shrt_buf_size == 0) {
				// if use rbuf, break because we want
				// to exclusively use it. In this case
				// nrfd shall be 1
				psmi_assert(ep->sockets_ep.nrfd == 1);
				goto out;
			} else {
				// if use shrt_buf, continue on next fd
				// because each fd uses its own shrt_buf
				continue;
			}
		}
		if_pf (ret == IPS_RECVHDRQ_BREAK) {
			// continue on other FDs
			continue;
		}
		if_pf (ret == IPS_RECVHDRQ_ERROR) {
			goto fail;
		}
#ifdef PSM_TCP_POLL
		if_pf (ctx->len) {
			// partial read
			psm3_sockets_switch_fds(ep, sw_id, i);
			sw_id += 1;
		}
#endif
	}

	if (!force && ep->sockets_ep.poll_count++ < ep->sockets_ep.inactive_skip_polls) {
		GENERIC_PERF_END(PSM_RX_SPEEDPATH_CTR); /* perf stats */
		return PSM2_OK_NO_PROGRESS;
	}
poll:
	new_accept = 0;
#ifdef PSM_TCP_POLL
	to_adjust_fds = 0;
	int nfds = ep->sockets_ep.nfds;
	rc = poll(ep->sockets_ep.fds, nfds, 0);
#else
	rc = epoll_wait(ep->sockets_ep.efd, ep->sockets_ep.events, TCP_MAX_EVENTS, 0);
	if (rc > 1024) {
		_HFI_INFO("EPOLL EVENTS %d\n", rc);
	}
#endif
	if_pf (rc < 0) {
		_HFI_ERROR("failed to poll '%s' (%d) on %s epid %s\n",
			strerror(errno), errno, ep->dev_name, psm3_epid_fmt_internal(ep->epid, 0));
		goto fail;
	} else if (rc == 0) {
		// still continue because we may have revisit in individual fds
		ep->sockets_ep.poll_count = 0;
	}

#ifdef PSM_TCP_POLL
	sw_id = 2;
	for (i = 0; i < nfds; i++) {
		fd = ep->sockets_ep.fds[i].fd;
		if (fd == -1 || ep->sockets_ep.fds[i].revents == 0) {
			// when process DISCONN on a different UDP socket/fd we may
			// close fd and set fds[] to -1 before we come to process this fd.
			continue;
		}
#else
	for (i = 0; i < rc; i++) {
		fd = ep->sockets_ep.events[i].data.fd;
#endif
		ctx = psm3_sockets_get_fd_ctx(ep, fd);
		if_pf (ctx == NULL) {
			// shouldn't happen
			goto fail;
		}

		if_pf (ctx->revisit || ctx->state == FD_STATE_NONE) {
			// already revisited. do it in next progress.
			// we are doing non-blocking connect, it can happen that we poll
			// before the connection is established and shall ignore it
			_HFI_VDBG("Skip revisit=%d state=%d fd=%d\n", ctx->revisit, ctx->state, fd);
			continue;
		}

#ifdef PSM_TCP_POLL
		if_pf (! (ep->sockets_ep.fds[i].revents & POLLIN) || (ep->sockets_ep.fds[i].revents & POLLERR)) {
			// POLLNVAL is expected if remote closed the socket
			if (!(ep->sockets_ep.fds[i].revents & POLLNVAL)) {
				_HFI_VDBG("Unexpected returned events fd=%d (%d) on %s epid %s\n",
					fd, ep->sockets_ep.fds[i].revents,
					ep->dev_name, psm3_epid_fmt_internal(ep->epid, 0));
			}
#else
		if_pf (! (ep->sockets_ep.events[i].events & EPOLLIN) || (ep->sockets_ep.events[i].events & EPOLLERR)) {
			_HFI_VDBG("Unexpected returned events fd=%d (%d) on %s epid %s\n",
				fd, ep->sockets_ep.events[i].events,
				ep->dev_name, psm3_epid_fmt_internal(ep->epid, 0));
#endif
			if (ctx->state != FD_STATE_ESTABLISHED && ctx->ipsaddr) {
				// reset below fields, so will recreate socket and connect to remote
				// in next data send attempt
				ctx->ipsaddr->sockets.tcp_fd = -1;
				ctx->ipsaddr->sockets.connected = 0;
			}
#ifdef PSM_TCP_POLL
			psm3_sockets_tcp_close_fd(ep, fd, i, NULL);
			to_adjust_fds = 1;
#else
			psm3_sockets_tcp_close_fd(ep, fd, -1, NULL);
#endif
			continue;
		}

		if (fd == ep->sockets_ep.listener_fd) {
			// listening socket
			if (psm3_sockets_tcp_accept(ep, fd, &rem_addr, &len_addr)
				== PSM2_INTERNAL_ERR) {
				goto fail;
			}
			new_accept = 1;
			continue;
		} else if (fd == ep->sockets_ep.udp_rx_fd) {
			// udp socket.
			ret = psm3_sockets_tcp_aux_rcv(ep, fd, ctx, &rcv_ev, recvq, &rem_addr, &len_addr);
			if (ret == IPS_RECVHDRQ_NO_PROGRESS) {
				continue;
			}
			if_pf (ret == IPS_RECVHDRQ_ERROR) {
				goto fail;
			}
			if_pf (ret == IPS_RECVHDRQ_REVISIT)
			{
				ep->sockets_ep.poll_count = ep->sockets_ep.active_skip_polls_offset;
				if (ep->sockets_ep.shrt_buf_size == 0) {
					break;
				} else {
					continue;
				}
			}
			num_done++;
			// if we can't process this now (such as an RTS we revisited and
			// ended up queueing on unexpected queue) we're told
			// to stop processing, we'll look at the rest later
			if_pf (ret == IPS_RECVHDRQ_BREAK) {
				_HFI_VDBG("stop rcvq\n");
				break;
			}
		} else {
			if (ep->sockets_ep.shrt_buf_size == 0 && ep->sockets_ep.rbuf_cur_fd
				&& ep->sockets_ep.rbuf_cur_fd != fd) {
				recvq->proto->stats.rcv_hol_blocking++;
				// have partial data to read and the fd is not expected, skip it
				_HFI_VDBG("Skip fd=%d, expected fd=%d\n", fd, ep->sockets_ep.rbuf_cur_fd);
				continue;
			}

			fetch = 0;
process:
			rcv_state = psm3_sockets_tcp_recv(ep, fd, &rcv_ev, ctx, fetch, &recvlen);
			if_pf (rcv_state == TCPRCV_INVALID) {
				_HFI_INFO("First PKT received is not PSM pkt. Close unknown connection fd=%d\n", fd);
				goto closefd;
			}
			if (rcv_state == TCPRCV_PARTIAL) {
				recvq->proto->stats.partial_read_cnt++;
#ifdef PSM_TCP_POLL
				// switch fd
				psm3_sockets_switch_fds(ep, sw_id, i);
				sw_id += 1;
#endif
				if (ep->sockets_ep.shrt_buf_size == 0) {
					break;
				} else {
					continue;
				}
			} else if (rcv_state == TCPRCV_DISCARD) {
				goto closefd;
			}
			if_pf (recvlen < 0) {
				if (errno == EAGAIN || errno == EWOULDBLOCK) {
					continue;
				} else {
					// TBD - how to best handle errors
					_HFI_ERROR("failed recv '%s' (%d) on %s epid %s\n",
						strerror(errno), errno, ep->dev_name, psm3_epid_fmt_internal(ep->epid, 0));
					goto fail;
				}
			} else if_pf (recvlen == 0) {
				_HFI_VDBG("fd=%d recvlen=0\n", fd);
				if (ctx->state != FD_STATE_ESTABLISHED && ctx->ipsaddr) {
					// will reconnect
					ctx->ipsaddr->sockets.tcp_fd = -1;
					ctx->ipsaddr->sockets.connected = 0;
				}
#ifdef PSM_TCP_POLL
				psm3_sockets_tcp_close_fd(ep, fd, i, NULL);
				to_adjust_fds = 1;
#else
				psm3_sockets_tcp_close_fd(ep, fd, -1, NULL);
#endif
				continue;
			}
			rcv_ev.p_hdr = (struct ips_message_header *)(ctx->shrt_buf + ctx->offset);
			if (ctx->extra_buf) {
				psmi_assert(ep->sockets_ep.shrt_buf_size);
				rcv_ev.payload = ctx->extra_buf;
			} else {
				rcv_ev.payload = ctx->shrt_buf + ctx->offset + MSG_HDR_SIZE;
			}
			rcv_ev.payload_size = recvlen - MSG_HDR_SIZE;
			if_pf (_HFI_VDBG_ON) {
				len_addr = sizeof(rem_addr);
				if (getpeername(fd, (struct sockaddr *)&rem_addr, &len_addr) != 0) {
					_HFI_VDBG("got recv %u bytes from IP n/a\n", recvlen);
				} else {
					// coverity[uninit_use_in_call] - intended, need to know what exactly in rem_addr
					_HFI_VDBG("got recv %u bytes from IP %s payload_size=%d opcode=%x\n", recvlen,
						psm3_sockaddr_fmt((struct sockaddr *)&rem_addr, 0),
						rcv_ev.payload_size,
						_get_proto_hfi_opcode(rcv_ev.p_hdr));
				}
			}
			if_pf (_HFI_PDBG_ON) {
				_HFI_PDBG_DUMP_ALWAYS((uint8_t*)rcv_ev.p_hdr, MSG_HDR_SIZE);
				_HFI_PDBG_DUMP_ALWAYS(rcv_ev.payload, rcv_ev.payload_size);
			}
			if_pf (recvlen < MSG_HDR_SIZE) {
				_HFI_ERROR( "unexpected small recv: %u on %s\n", recvlen, ep->dev_name);
				psm3_ips_proto_dump_frame(rcv_ev.p_hdr, sizeof(struct ips_message_header),
					"bad header");
				goto closefd;
			}
			ret = psm3_sockets_tcp_process_packet(&rcv_ev, ep, fd, ctx,
						(psm3_sockaddr_in_t *)&rem_addr,
						recvq, 1);
			_HFI_VDBG("Pkt process - ctx{offset=%d;len=%d;pkt_len=%d} ret=%d rbuf_cur_fd=%d fd=%d\n",
				ctx->offset, ctx->len, ctx->pkt_len, ret, ep->sockets_ep.rbuf_cur_fd, fd);
			if_pf (ret == IPS_RECVHDRQ_ERROR) {
				goto fail;
			}
			if_pf (ret == IPS_RECVHDRQ_REVISIT)
			{
				ep->sockets_ep.poll_count = ep->sockets_ep.active_skip_polls_offset;
				if (ep->sockets_ep.shrt_buf_size == 0) {
					break;
				} else {
					continue;
				}
			}
//processed:
			num_done++;
			// if we can't process this now (such as an RTS we revisited and
			// ended up queueing on unexpected queue) we're told
			// to stop processing, we'll look at the rest later
			if_pf (ret == IPS_RECVHDRQ_BREAK) {
				// continue on other FDs
				continue;
			}
			// when get DISCONN_REQ, we may close fd and free fd_ctx
			// check whether fd is closed (fd=-1)
			ctx = psm3_sockets_get_fd_ctx(ep, fd);
			if (ctx && ctx->len) {
				// has more data
				fetch = 1;
				goto process;
			}
			continue;
closefd:
#ifdef PSM_TCP_POLL
			psm3_sockets_tcp_close_fd(ep, fd, i, NULL);
			to_adjust_fds = 1;
#else
			psm3_sockets_tcp_close_fd(ep, fd, -1, NULL);
#endif
		}
	}
#ifdef PSM_TCP_POLL
	if_pf (to_adjust_fds) {
		// remove -1 indexes from array
		psm3_sockets_adjust_fds(ep);
		to_adjust_fds = 0;
	}
#endif
	if_pf (new_accept) {
		// try to read data from the new accept sockets
		// this will speed up our connection establishment
		goto poll;
	}
out:
	/* Process any pending acks before exiting */
	//process_pending_acks(recvq);
	GENERIC_PERF_END(PSM_RX_SPEEDPATH_CTR); /* perf stats */
	return num_done?PSM2_OK:PSM2_OK_NO_PROGRESS;
fail:
	GENERIC_PERF_END(PSM_RX_SPEEDPATH_CTR); /* perf stats */
	return PSM2_INTERNAL_ERR;
}

/*---------------------------------------------------------------------------*/
/* UDP specific code */

static __inline__ int
psm3_sockets_udp_process_packet(struct ips_recvhdrq_event *rcv_ev,
               psm2_ep_t ep, uint8_t *buf,
               psm3_sockaddr_in_t *rem_addr,
               struct ips_recvhdrq *recvq)
{
	int ret = IPS_RECVHDRQ_CONTINUE;
	rcv_ev->p_hdr = (struct ips_message_header *)buf;
	rcv_ev->payload = buf + MSG_HDR_SIZE;

#ifdef UDP_DEBUG
	_HFI_VDBG("UDP receive - opcode %x on %s\n",
		_get_proto_hfi_opcode(rcv_ev->p_hdr), ep->dev_name);
#endif

	PSM2_LOG_MSG("Process PKT: opcode=%x payload_size=%d", _get_proto_hfi_opcode(rcv_ev->p_hdr), rcv_ev->payload_size);
	PSM2_LOG_PKT_STRM(PSM2_LOG_RX,rcv_ev->p_hdr,"PKT_STRM:");
	struct ips_epstate_entry *epstaddr =
		ips_epstate_lookup(recvq->epstate, rcv_ev->p_hdr->connidx);

	if_pf((epstaddr == NULL) || (epstaddr->ipsaddr == NULL)) {
		rcv_ev->ipsaddr = NULL;
		recvq->recvq_callbacks.callback_packet_unknown(rcv_ev);
#if 0
	} else if_pf (psm3_sockaddr_cmp(&epstaddr->ipsaddr->sockets.remote_pri_addr, rem_addr)) {
		// TBD - we could also compare sin6_scope_id here or in
		// psm3_sockaddr_cmp to confirm interface used
		// TBD - we get some packets from unexpected sockets
		// occurs under higher stress
		_HFI_ERROR("mismatched IP %s got %s on %s\n",
				psm3_sockaddr_fmt((struct sockaddr *)&epstaddr->ipsaddr->sockets.remote_pri_addr, 0),
				psm3_sockaddr_fmt((struct sockaddr *)&rem_addr, 1),
				ep->dev_name);
		// TBD - we really want a variation of callback which will
		// not check for attempted connect REQ/DREQ, but do the rest
		rcv_ev->ipsaddr = NULL;
		recvq->recvq_callbacks.callback_packet_unknown(rcv_ev);
#endif
	} else {
		rcv_ev->ipsaddr = epstaddr->ipsaddr;
		psmi_assert(PSMI_HOWMANY(psm3_sockets_packet_service_routines)
				== OPCODE_FUTURE_FROM - OPCODE_RESERVED);
		ret = ips_proto_process_packet(rcv_ev,
			psm3_sockets_packet_service_routines);
		if_pf (ret == IPS_RECVHDRQ_REVISIT)
		{
			// try processing on next progress call
			_HFI_VDBG( "REVISIT returned on process_packet\n");
			// normally PSM would stop processing the header Q and
			// poll the same entry again.  We can't do that with a
			// UDP socket, so we stash the buffer and payload length
			// in our ep and will revisit it next time we are called
			ep->sockets_ep.revisit_buf = buf;
			ep->sockets_ep.revisit_payload_size = rcv_ev->payload_size;
			recvq->proto->stats.rcv_revisit++;
		}
	}
	return ret;
}

psm2_error_t psm3_sockets_udp_recvhdrq_progress(struct ips_recvhdrq *recvq, bool force)
{
	GENERIC_PERF_BEGIN(PSM_RX_SPEEDPATH_CTR); /* perf stats */

	int ret = IPS_RECVHDRQ_CONTINUE;
	int recvlen = 0;
	psm2_ep_t ep = recvq->proto->ep;
	struct sockaddr_storage rem_addr;
	socklen_t len_addr = sizeof(rem_addr);
	PSMI_CACHEALIGN struct ips_recvhdrq_event rcv_ev = {
		.proto = recvq->proto,
		.recvq = recvq,
		//.ptype = RCVHQ_RCV_TYPE_ERROR
	};
	uint32_t num_done = 0;

	while (1) {
		uint8_t *buf;
		// TBD really only need to check this on 1st loop
		if_pf (ep->sockets_ep.revisit_buf) {
			buf = ep->sockets_ep.revisit_buf;
			ep->sockets_ep.revisit_buf = NULL;
			rcv_ev.payload_size = ep->sockets_ep.revisit_payload_size;
			ep->sockets_ep.revisit_payload_size = 0;
		} else {
			buf = ep->sockets_ep.rbuf;
			// TBD - do we need rem_addr?  if not, can use recv
			// MSG_DONTWAIT is redundant since we set O_NONBLOCK
			recvlen = recvfrom(ep->sockets_ep.udp_rx_fd, buf, ep->sockets_ep.buf_size,
								MSG_DONTWAIT|MSG_TRUNC,
								(struct sockaddr *)&rem_addr, &len_addr);
			if (recvlen < 0) {
				if (errno == EAGAIN || errno == EWOULDBLOCK) {
					break;
				} else {
					// TBD - how to best handle errors
					_HFI_ERROR("failed recv '%s' (%d) on %s epid %s\n",
						strerror(errno), errno, ep->dev_name, psm3_epid_fmt_internal(ep->epid, 0));
					GENERIC_PERF_END(PSM_RX_SPEEDPATH_CTR); /* perf stats */
					return PSM2_INTERNAL_ERR;
				}
			}
			// coverity[uninit_use] - rem_addr initialized in recvfrom() call above
			if_pf (len_addr > sizeof(rem_addr) ||
				rem_addr.ss_family != psm3_socket_domain
				) {
				// TBD - how to best handle errors
				// coverity[uninit_use_in_call] - rem_addr initialized in recvfrom() call above
				_HFI_ERROR("unexpected rem_addr type (%u) on %s epid %s\n",
					rem_addr.ss_family, ep->dev_name, psm3_epid_fmt_internal(ep->epid, 0));
				GENERIC_PERF_END(PSM_RX_SPEEDPATH_CTR); /* perf stats */
				return PSM2_INTERNAL_ERR;
			}
			if_pf (_HFI_VDBG_ON) {
				if (len_addr) {
					_HFI_VDBG("got recv %u bytes from IP %s payload_size=%d opcode=%x\n", recvlen,
						psm3_sockaddr_fmt((struct sockaddr *)&rem_addr, 0),
						rcv_ev.payload_size,
						_get_proto_hfi_opcode((struct ips_message_header *)buf));
				} else {
					_HFI_VDBG("got recv %u bytes from IP n/a\n", recvlen);
				}
			}
			if_pf (_HFI_PDBG_ON)
				_HFI_PDBG_DUMP_ALWAYS(buf, recvlen);
			if_pf (recvlen < MSG_HDR_SIZE) {
				_HFI_ERROR( "unexpected small recv: %u on %s\n", recvlen, ep->dev_name);
				goto processed;
			} else if_pf (recvlen > ep->sockets_ep.buf_size) {
				_HFI_ERROR( "unexpected large recv: %u on %s\n", recvlen, ep->dev_name);
				goto processed;
			}
			rcv_ev.payload_size = recvlen - MSG_HDR_SIZE;
		}
		ret = psm3_sockets_udp_process_packet(&rcv_ev, ep, buf,
						(psm3_sockaddr_in_t *)&rem_addr,
						recvq);
		if_pf (ret == IPS_RECVHDRQ_REVISIT)
		{
			GENERIC_PERF_END(PSM_RX_SPEEDPATH_CTR); /* perf stats */
			return PSM2_OK_NO_PROGRESS;
		}
processed:
		num_done++;
		// if we can't process this now (such as an RTS we revisited and
		// ended up queueing on unexpected queue) we're told
		// to stop processing, we'll look at the rest later
		if_pf (ret == IPS_RECVHDRQ_BREAK) {
			_HFI_VDBG("stop rcvq\n");
			break;
		}
	}

	/* Process any pending acks before exiting */
	process_pending_acks(recvq);

	GENERIC_PERF_END(PSM_RX_SPEEDPATH_CTR); /* perf stats */

	return num_done?PSM2_OK:PSM2_OK_NO_PROGRESS;
}

#endif /* PSM_SOCKETS */
