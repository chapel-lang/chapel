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

static ssize_t tcpx_read_from_buffer(struct stage_buf *stage_buf,
				     uint8_t *buf, size_t len)
{
	size_t rem_size;
	ssize_t ret;

	assert(stage_buf->cur_pos < stage_buf->bytes_avail);
	rem_size = stage_buf->bytes_avail - stage_buf->cur_pos;
	ret = (rem_size >= len) ? len : rem_size;
	memcpy(buf, &stage_buf->buf[stage_buf->cur_pos], ret);
	stage_buf->cur_pos += ret;
	return ret;
}

ssize_t tcpx_recv_hdr(SOCKET sock, struct stage_buf *stage_buf,
		      struct tcpx_cur_rx_msg *cur_rx_msg)
{
	ssize_t bytes_recvd, bytes_read;
	size_t rem_len;
	void *rem_buf;

	rem_buf = (uint8_t *) &cur_rx_msg->hdr + cur_rx_msg->done_len;
	rem_len = cur_rx_msg->hdr_len - cur_rx_msg->done_len;

	if (stage_buf->cur_pos < stage_buf->bytes_avail) {
		bytes_read = tcpx_read_from_buffer(stage_buf, rem_buf, rem_len);
		rem_len -= bytes_read;
		if (!rem_len)
			return bytes_read;

		rem_buf = (char *) rem_buf + bytes_read;
	} else {
		bytes_read = 0;
	}

	bytes_recvd = ofi_recv_socket(sock, rem_buf, rem_len, 0);
	if (bytes_recvd < 0)
		return bytes_read ? bytes_read : -ofi_sockerr();
	else if (bytes_recvd == 0)
		return -FI_ENOTCONN;

	return bytes_read + bytes_recvd;
}

static ssize_t tcpx_readv_from_buffer(struct stage_buf *stage_buf,
				      struct iovec *iov,
				      int iov_cnt)
{
	ssize_t ret = 0;
	size_t bytes_read;
	int i;

	if (iov_cnt == 1)
		return tcpx_read_from_buffer(stage_buf, iov[0].iov_base,
					     iov[0].iov_len);

	for (i = 0; i < iov_cnt; i++) {
		bytes_read = tcpx_read_from_buffer(stage_buf, iov[i].iov_base,
						   iov[i].iov_len);
		ret += bytes_read;
		if ((bytes_read < iov[i].iov_len) ||
		    !(stage_buf->bytes_avail - stage_buf->cur_pos))
			break;
	}
	return ret;
}

int tcpx_recv_msg_data(struct tcpx_xfer_entry *rx_entry)
{
	struct stage_buf *stage_buf;
	ssize_t bytes_recvd, bytes_read;

	if (!rx_entry->iov_cnt || !rx_entry->iov[0].iov_len)
		return FI_SUCCESS;

	stage_buf = &rx_entry->ep->stage_buf;
	if (stage_buf->cur_pos < stage_buf->bytes_avail) {
		bytes_read = tcpx_readv_from_buffer(stage_buf,
						    rx_entry->iov,
						    rx_entry->iov_cnt);
		ofi_consume_iov(rx_entry->iov, &rx_entry->iov_cnt, bytes_read);
		if (!rx_entry->iov_cnt || !rx_entry->iov[0].iov_len)
			return FI_SUCCESS;
	} else {
		bytes_read = 0;
	}

	bytes_recvd = ofi_readv_socket(rx_entry->ep->sock, rx_entry->iov,
				       rx_entry->iov_cnt);
	if (bytes_recvd < 0)
		return bytes_read ? -FI_EAGAIN : -ofi_sockerr();
	else if (bytes_recvd == 0)
		return -FI_ENOTCONN;

	ofi_consume_iov(rx_entry->iov, &rx_entry->iov_cnt, bytes_recvd);
	return (!rx_entry->iov_cnt || !rx_entry->iov[0].iov_len) ?
		FI_SUCCESS : -FI_EAGAIN;
}

int tcpx_read_to_buffer(SOCKET sock, struct stage_buf *stage_buf)
{
	int bytes_recvd;

	bytes_recvd = ofi_recv_socket(sock, stage_buf->buf,
				      sizeof(stage_buf->buf), 0);
	if (bytes_recvd <= 0)
		return (bytes_recvd) ? -ofi_sockerr(): -FI_ENOTCONN;

	stage_buf->bytes_avail = bytes_recvd;
	stage_buf->cur_pos = 0;
	return FI_SUCCESS;
}
