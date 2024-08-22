/*
 * Copyright (c) 2022 Intel Corporation. All rights reserved.
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

#include "config.h"

#include <liburing.h>

#include <ofi_net.h>

int ofi_sockapi_connect_uring(struct ofi_sockapi *sockapi, SOCKET sock,
			      const struct sockaddr *addr, socklen_t addrlen,
			      struct ofi_sockctx *ctx)
{
	struct io_uring_sqe *sqe;
	struct ofi_sockapi_uring *uring;

	uring = &sockapi->tx_uring;
	if (ctx->uring_sqe_inuse || uring->credits == 0)
		return -FI_EAGAIN;

	sqe = io_uring_get_sqe(uring->io_uring);
	if (!sqe)
		return -FI_EOVERFLOW;

	io_uring_prep_connect(sqe, sock, addr, addrlen);
	io_uring_sqe_set_data(sqe, ctx);
	ctx->uring_sqe_inuse = true;
	uring->credits--;
	return -OFI_EINPROGRESS_URING;
}

int ofi_sockapi_accept_uring(struct ofi_sockapi *sockapi, SOCKET sock,
			     struct sockaddr *addr, socklen_t *addrlen,
			     struct ofi_sockctx *ctx)
{
	struct io_uring_sqe *sqe;
	struct ofi_sockapi_uring *uring;

	uring = &sockapi->rx_uring;
	if (ctx->uring_sqe_inuse || uring->credits == 0)
		return -FI_EAGAIN;

	sqe = io_uring_get_sqe(uring->io_uring);
	if (!sqe)
		return -FI_EOVERFLOW;

	io_uring_prep_accept(sqe, sock, addr, addrlen, 0);
	io_uring_sqe_set_data(sqe, ctx);
	ctx->uring_sqe_inuse = true;
	uring->credits--;
	return -OFI_EINPROGRESS_URING;
}

ssize_t ofi_sockapi_send_uring(struct ofi_sockapi *sockapi, SOCKET sock,
			       const void *buf,  size_t len, int flags,
			       struct ofi_sockctx *ctx)
{
	struct io_uring_sqe *sqe;
	struct ofi_sockapi_uring *uring;

	uring = &sockapi->tx_uring;
	if (ctx->uring_sqe_inuse || uring->credits == 0)
		return -FI_EAGAIN;

	/* MSG_NOSIGNAL would return ENOTSUP with io_uring */
	flags &= ~MSG_NOSIGNAL;

	sqe = io_uring_get_sqe(uring->io_uring);
	if (!sqe)
		return -FI_EOVERFLOW;

	io_uring_prep_send(sqe, sock, buf, len, flags);
	io_uring_sqe_set_data(sqe, ctx);
	ctx->uring_sqe_inuse = true;
	uring->credits--;
	return -OFI_EINPROGRESS_URING;
}

ssize_t ofi_sockapi_sendv_uring(struct ofi_sockapi *sockapi, SOCKET sock,
				const struct iovec *iov, size_t cnt, int flags,
				struct ofi_sockctx *ctx)
{
	struct io_uring_sqe *sqe;
	struct ofi_sockapi_uring *uring;

	uring = &sockapi->tx_uring;
	if (ctx->uring_sqe_inuse || uring->credits == 0)
		return -FI_EAGAIN;

	/* MSG_NOSIGNAL would return ENOTSUP with io_uring */
	flags &= ~MSG_NOSIGNAL;

	sqe = io_uring_get_sqe(uring->io_uring);
	if (!sqe)
		return -FI_EOVERFLOW;

	io_uring_prep_writev(sqe, sock, iov, cnt, flags);
	io_uring_sqe_set_data(sqe, ctx);
	ctx->uring_sqe_inuse = true;
	uring->credits--;
	return -OFI_EINPROGRESS_URING;
}

ssize_t ofi_sockapi_recv_uring(struct ofi_sockapi *sockapi, SOCKET sock,
			       void *buf, size_t len, int flags,
			       struct ofi_sockctx *ctx)
{
	struct io_uring_sqe *sqe;
	struct ofi_sockapi_uring *uring;

	uring = &sockapi->rx_uring;
	if (ctx->uring_sqe_inuse || uring->credits == 0)
		return -FI_EAGAIN;

	sqe = io_uring_get_sqe(uring->io_uring);
	if (!sqe)
		return -FI_EOVERFLOW;

	io_uring_prep_recv(sqe, sock, buf, len, flags);
	io_uring_sqe_set_data(sqe, ctx);
	ctx->uring_sqe_inuse = true;
	uring->credits--;
	return -OFI_EINPROGRESS_URING;
}

ssize_t ofi_sockapi_recvv_uring(struct ofi_sockapi *sockapi, SOCKET sock,
				struct iovec *iov, size_t cnt, int flags,
				struct ofi_sockctx *ctx)
{
	struct io_uring_sqe *sqe;
	struct ofi_sockapi_uring *uring;

	uring = &sockapi->rx_uring;
	if (ctx->uring_sqe_inuse || uring->credits == 0)
		return -FI_EAGAIN;

	sqe = io_uring_get_sqe(uring->io_uring);
	if (!sqe)
		return -FI_EOVERFLOW;

	io_uring_prep_readv(sqe, sock, iov, cnt, flags);
	io_uring_sqe_set_data(sqe, ctx);
	ctx->uring_sqe_inuse = true;
	uring->credits--;
	return -OFI_EINPROGRESS_URING;
}

int ofi_sockctx_uring_cancel(struct ofi_sockapi_uring *uring,
			     struct ofi_sockctx *canceled_ctx,
			     struct ofi_sockctx *ctx)
{
	struct io_uring_sqe *sqe;

	if (!canceled_ctx->uring_sqe_inuse)
		return 0;

	if (ctx->uring_sqe_inuse || uring->credits == 0)
		return -FI_EAGAIN;

	sqe = io_uring_get_sqe(uring->io_uring);
	if (!sqe)
		return -FI_EOVERFLOW;

	io_uring_prep_cancel(sqe, canceled_ctx, 0);
	io_uring_sqe_set_data(sqe, ctx);
	ctx->uring_sqe_inuse = true;
	uring->credits--;
	return -OFI_EINPROGRESS_URING;
}

int ofi_sockctx_uring_poll_add(struct ofi_sockapi_uring *uring,
			       int fd, short poll_mask, bool multishot,
			       struct ofi_sockctx *ctx)
{
	struct io_uring_sqe *sqe;

	if (ctx->uring_sqe_inuse || uring->credits == 0)
		return -FI_EAGAIN;

	sqe = io_uring_get_sqe(uring->io_uring);
	if (!sqe)
		return -FI_EOVERFLOW;

	if (multishot)
		io_uring_prep_poll_multishot(sqe, fd, poll_mask);
	else
		io_uring_prep_poll_add(sqe, fd, poll_mask);
	io_uring_sqe_set_data(sqe, ctx);
	ctx->uring_sqe_inuse = true;
	uring->credits--;
	return -OFI_EINPROGRESS_URING;
}

int ofi_uring_init(ofi_io_uring_t *io_uring, size_t entries)
{
	struct io_uring_params params;
	int ret;

	memset(&params, 0, sizeof(params));
	ret = io_uring_queue_init_params(entries, io_uring, &params);
	if (ret)
		return -errno;

	/* FAST_POOL is required for pre-posting receive buffers */
	if (!(params.features & IORING_FEAT_FAST_POLL)) {
		io_uring_queue_exit(io_uring);
		return -FI_ENOSYS;
	}

	assert(!io_uring_sq_ready(io_uring));
	assert(!io_uring_cq_ready(io_uring));
	assert(io_uring_sq_space_left(io_uring) >= entries);
	return 0;
}

int ofi_uring_destroy(ofi_io_uring_t *io_uring)
{
	if (io_uring_sq_ready(io_uring) || io_uring_cq_ready(io_uring))
		return -FI_EBUSY;

	io_uring_queue_exit(io_uring);
	return 0;
}

