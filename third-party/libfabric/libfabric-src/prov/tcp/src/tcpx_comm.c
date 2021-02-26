/*
 * Copyright (c) 2017 Intel Corporation. All rights reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * BSD license below:
 *
 *	   Redistribution and use in source and binary forms, with or
 *	   without modification, are permitted provided that the following
 *	   conditions are met:
 *
 *		- Redistributions of source code must retain the above
 *		  copyright notice, this list of conditions and the following
 *		  disclaimer.
 *
 *		- Redistributions in binary form must reproduce the above
 *		  copyright notice, this list of conditions and the following
 *		  disclaimer in the documentation and/or other materials
 *		  provided with the distribution.
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

#include <rdma/fi_errno.h>
#include <ofi_prov.h>
#include <sys/types.h>
#include <ofi_util.h>
#include <ofi_iov.h>
#include "tcpx.h"

int tcpx_send_msg(struct tcpx_xfer_entry *tx_entry)
{
	ssize_t bytes_sent;
	struct msghdr msg = {0};

	msg.msg_iov = tx_entry->iov;
	msg.msg_iovlen = tx_entry->iov_cnt;

	bytes_sent = ofi_sendmsg_tcp(tx_entry->ep->sock, &msg, MSG_NOSIGNAL);
	if (bytes_sent < 0)
		return ofi_sockerr() == EPIPE ? -FI_ENOTCONN : -ofi_sockerr();

	tx_entry->rem_len -= bytes_sent;
	if (tx_entry->rem_len) {
		ofi_consume_iov(tx_entry->iov, &tx_entry->iov_cnt, bytes_sent);
		return -FI_EAGAIN;
	}
	return FI_SUCCESS;
}

static ssize_t tcpx_read_from_buffer(struct stage_buf *sbuf,
				     uint8_t *buf, size_t len)
{
	size_t rem_size;
	ssize_t ret;

	assert(sbuf->len >= sbuf->off);
	rem_size = sbuf->len - sbuf->off;
	assert(rem_size);
	ret = (rem_size >= len) ? len : rem_size;
	memcpy(buf, &sbuf->buf[sbuf->off], ret);
	sbuf->off += ret;
	return ret;
}

static int tcpx_recv_hdr(SOCKET sock, struct stage_buf *sbuf,
			  struct tcpx_cur_rx_msg *cur_rx_msg)
{
	void *rem_buf;
	size_t rem_len;
	ssize_t bytes_recvd;

	rem_buf = (uint8_t *) &cur_rx_msg->hdr + cur_rx_msg->done_len;
	rem_len = cur_rx_msg->hdr_len - cur_rx_msg->done_len;

	if (sbuf->len != sbuf->off)
		bytes_recvd = tcpx_read_from_buffer(sbuf, rem_buf, rem_len);
	else
		bytes_recvd = ofi_recv_socket(sock, rem_buf, rem_len, 0);
	if (bytes_recvd <= 0)
		return bytes_recvd ? -ofi_sockerr(): -FI_ENOTCONN;

	return bytes_recvd;
}

int tcpx_comm_recv_hdr(SOCKET sock, struct stage_buf *sbuf,
		        struct tcpx_cur_rx_msg *cur_rx_msg)
{
	ssize_t bytes_recvd;
	bytes_recvd = tcpx_recv_hdr(sock, sbuf, cur_rx_msg);
	if (bytes_recvd < 0)
		return bytes_recvd;
	cur_rx_msg->done_len += bytes_recvd;

	if (cur_rx_msg->done_len == sizeof(cur_rx_msg->hdr.base_hdr)) {
		cur_rx_msg->hdr_len = (size_t) cur_rx_msg->hdr.base_hdr.payload_off;

		if (cur_rx_msg->hdr_len > cur_rx_msg->done_len) {
			bytes_recvd = tcpx_recv_hdr(sock, sbuf, cur_rx_msg);
			if (bytes_recvd < 0)
				return bytes_recvd;
			cur_rx_msg->done_len += bytes_recvd;
			return (cur_rx_msg->done_len == cur_rx_msg->hdr_len) ?
				FI_SUCCESS : -FI_EAGAIN;
		}
	}

	return (cur_rx_msg->done_len == cur_rx_msg->hdr_len) ?
		FI_SUCCESS : -FI_EAGAIN;
}

static ssize_t tcpx_readv_from_buffer(struct stage_buf *sbuf,
				      struct iovec *iov,
				      int iov_cnt)
{
	ssize_t ret = 0;
	size_t bytes_read;
	int i;

	if (iov_cnt == 1)
		return tcpx_read_from_buffer(sbuf, iov[0].iov_base,
					     iov[0].iov_len);

	for (i = 0; i < iov_cnt; i++) {
		bytes_read = tcpx_read_from_buffer(sbuf, iov[i].iov_base,
						   iov[i].iov_len);
		ret += bytes_read;
		if ((bytes_read < iov[i].iov_len) ||
		    !(sbuf->len - sbuf->off))
			break;
	}
	return ret;
}

int tcpx_recv_msg_data(struct tcpx_xfer_entry *rx_entry)
{
	ssize_t bytes_recvd;

	if (!rx_entry->iov_cnt || !rx_entry->iov[0].iov_len)
		return FI_SUCCESS;

	if (rx_entry->ep->stage_buf.len != rx_entry->ep->stage_buf.off)
		bytes_recvd = tcpx_readv_from_buffer(&rx_entry->ep->stage_buf,
						     rx_entry->iov,
						     rx_entry->iov_cnt);
	else
		bytes_recvd = ofi_readv_socket(rx_entry->ep->sock,
					       rx_entry->iov,
					       rx_entry->iov_cnt);
	if (bytes_recvd <= 0)
		return (bytes_recvd) ? -ofi_sockerr(): -FI_ENOTCONN;

	ofi_consume_iov(rx_entry->iov, &rx_entry->iov_cnt, bytes_recvd);
	return (rx_entry->iov_cnt && rx_entry->iov[0].iov_len) ?
		-FI_EAGAIN: FI_SUCCESS;
}

int tcpx_read_to_buffer(SOCKET sock, struct stage_buf *stage_buf)
{
	int bytes_recvd;

	bytes_recvd = ofi_recv_socket(sock, stage_buf->buf,
				      stage_buf->size, 0);
	if (bytes_recvd <= 0)
		return (bytes_recvd) ? -ofi_sockerr(): -FI_ENOTCONN;

	stage_buf->len = bytes_recvd;
	stage_buf->off = 0;
	return FI_SUCCESS;
}
