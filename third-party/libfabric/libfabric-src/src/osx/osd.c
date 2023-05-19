/*
 * Copyright (C) 2020 by Argonne National Laboratory.
 * Copyright (c) 2022 DataDirect Networks, Inc. All rights reserved.
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

#include "ofi.h"
#include "ofi_osd.h"

ssize_t
ofi_sendv_socket(SOCKET fd, const struct iovec *iovec, size_t iov_cnt, int flags)
{
	ssize_t size = 0;
	int ret, i;

	if (iov_cnt == 1) {
		return ofi_send_socket(fd, iovec[0].iov_base,
				       iovec[0].iov_len, flags);
	}

	for (i = 0; i < iov_cnt; i++) {
		ret = ofi_send_socket(fd, iovec[i].iov_base,
				      iovec[i].iov_len, flags);
		if (ret >= 0) {
			size += ret;
			if (ret != iovec[i].iov_len)
				return size;
		} else {
			return size ? size : ret;
		}
	}
	return size;
}

ssize_t
ofi_recvv_socket(SOCKET fd, const struct iovec *iovec, size_t iov_cnt, int flags)
{
	ssize_t size = 0;
	int ret, i;

	if (iov_cnt == 1) {
		return ofi_recv_socket(fd, iovec[0].iov_base,
				       iovec[0].iov_len, flags);
	}

	for (i = 0; i < iov_cnt; i++) {
		ret = ofi_recv_socket(fd, iovec[i].iov_base,
				      iovec[i].iov_len, flags);
		if (ret >= 0) {
			size += ret;
			if (ret != iovec[i].iov_len)
				return size;
		} else {
			return size ? size : ret;
		}
	}
	return size;
}

ssize_t ofi_writev_socket(SOCKET fd, const struct iovec *iovec, size_t iov_cnt)
{
	return ofi_sendv_socket(fd, iovec, iov_cnt, 0);
}

ssize_t ofi_readv_socket(SOCKET fd, const struct iovec *iovec, size_t iov_cnt)
{
	return ofi_recvv_socket(fd, iovec, iov_cnt, 0);
}

ssize_t ofi_sendmsg_tcp(SOCKET fd, const struct msghdr *msg, int flags)
{
	return ofi_sendv_socket(fd, msg->msg_iov, msg->msg_iovlen, flags);
}

ssize_t ofi_recvmsg_tcp(SOCKET fd, struct msghdr *msg, int flags)
{
	return ofi_recvv_socket(fd, msg->msg_iov, msg->msg_iovlen, flags);
}
