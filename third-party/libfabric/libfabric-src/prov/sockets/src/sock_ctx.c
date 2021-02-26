/*
 * Copyright (c) 2014 Intel Corporation. All rights reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * OpenFabrics.org BSD license below:
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

#include <stdlib.h>
#include <string.h>

#include "sock.h"
#include "sock_util.h"

#define SOCK_LOG_DBG(...) _SOCK_LOG_DBG(FI_LOG_EP_CTRL, __VA_ARGS__)
#define SOCK_LOG_ERROR(...) _SOCK_LOG_ERROR(FI_LOG_EP_CTRL, __VA_ARGS__)

struct sock_rx_ctx *sock_rx_ctx_alloc(const struct fi_rx_attr *attr,
				      void *context, int use_shared)
{
	struct sock_rx_ctx *rx_ctx;
	rx_ctx = calloc(1, sizeof(*rx_ctx));
	if (!rx_ctx)
		return NULL;

	dlist_init(&rx_ctx->cq_entry);
	dlist_init(&rx_ctx->pe_entry);

	dlist_init(&rx_ctx->pe_entry_list);
	dlist_init(&rx_ctx->rx_entry_list);
	dlist_init(&rx_ctx->rx_buffered_list);
	dlist_init(&rx_ctx->ep_list);

	fastlock_init(&rx_ctx->lock);

	rx_ctx->ctx.fid.fclass = FI_CLASS_RX_CTX;
	rx_ctx->ctx.fid.context = context;
	rx_ctx->num_left = sock_get_tx_size(attr->size);
	rx_ctx->attr = *attr;
	rx_ctx->use_shared = use_shared;
	return rx_ctx;
}

void sock_rx_ctx_free(struct sock_rx_ctx *rx_ctx)
{
	fastlock_destroy(&rx_ctx->lock);
	free(rx_ctx->rx_entry_pool);
	free(rx_ctx);
}

static struct sock_tx_ctx *sock_tx_context_alloc(const struct fi_tx_attr *attr,
						 void *context, int use_shared,
						 size_t fclass)
{
	struct sock_tx_ctx *tx_ctx;
	struct fi_rx_attr rx_attr = {0};

	tx_ctx = calloc(sizeof(*tx_ctx), 1);
	if (!tx_ctx)
		return NULL;

	if (!use_shared && ofi_rbinit(&tx_ctx->rb,
				 (attr->size) ? attr->size * SOCK_EP_TX_ENTRY_SZ :
				 SOCK_EP_TX_SZ * SOCK_EP_TX_ENTRY_SZ))
		goto err;

	dlist_init(&tx_ctx->cq_entry);
	dlist_init(&tx_ctx->pe_entry);

	dlist_init(&tx_ctx->pe_entry_list);
	dlist_init(&tx_ctx->ep_list);

	fastlock_init(&tx_ctx->rb_lock);
	fastlock_init(&tx_ctx->lock);

	switch (fclass) {
	case FI_CLASS_TX_CTX:
		tx_ctx->fid.ctx.fid.fclass = FI_CLASS_TX_CTX;
		tx_ctx->fid.ctx.fid.context = context;
		tx_ctx->fclass = FI_CLASS_TX_CTX;
		tx_ctx->use_shared = use_shared;
		break;
	case FI_CLASS_STX_CTX:
		tx_ctx->fid.stx.fid.fclass = FI_CLASS_STX_CTX;
		tx_ctx->fid.stx.fid.context = context;
		tx_ctx->fclass = FI_CLASS_STX_CTX;
		break;
	default:
		goto err;
	}
	tx_ctx->attr = *attr;
	tx_ctx->attr.op_flags |= FI_TRANSMIT_COMPLETE;

	if (!use_shared) {
		tx_ctx->rx_ctrl_ctx = sock_rx_ctx_alloc(&rx_attr, NULL, 0);
		if (!tx_ctx->rx_ctrl_ctx)
			goto err;
		tx_ctx->rx_ctrl_ctx->is_ctrl_ctx = 1;
	}
	return tx_ctx;

err:
	free(tx_ctx);
	return NULL;
}


struct sock_tx_ctx *sock_tx_ctx_alloc(const struct fi_tx_attr *attr,
				      void *context, int use_shared)
{
	return sock_tx_context_alloc(attr, context, use_shared, FI_CLASS_TX_CTX);
}

struct sock_tx_ctx *sock_stx_ctx_alloc(const struct fi_tx_attr *attr,
					void *context)
{
	return sock_tx_context_alloc(attr, context, 0, FI_CLASS_STX_CTX);
}

void sock_tx_ctx_free(struct sock_tx_ctx *tx_ctx)
{
	fastlock_destroy(&tx_ctx->rb_lock);
	fastlock_destroy(&tx_ctx->lock);

	if (!tx_ctx->use_shared) {
		ofi_rbfree(&tx_ctx->rb);
		sock_rx_ctx_free(tx_ctx->rx_ctrl_ctx);
	}
	free(tx_ctx);
}

void sock_tx_ctx_start(struct sock_tx_ctx *tx_ctx)
{
	fastlock_acquire(&tx_ctx->rb_lock);
}

void sock_tx_ctx_write(struct sock_tx_ctx *tx_ctx, const void *buf, size_t len)
{
	ofi_rbwrite(&tx_ctx->rb, buf, len);
}

void sock_tx_ctx_commit(struct sock_tx_ctx *tx_ctx)
{
	ofi_rbcommit(&tx_ctx->rb);
	sock_pe_signal(tx_ctx->domain->pe);
	fastlock_release(&tx_ctx->rb_lock);
}

void sock_tx_ctx_abort(struct sock_tx_ctx *tx_ctx)
{
	ofi_rbabort(&tx_ctx->rb);
	fastlock_release(&tx_ctx->rb_lock);
}

void sock_tx_ctx_write_op_send(struct sock_tx_ctx *tx_ctx,
		struct sock_op *op, uint64_t flags, uint64_t context,
		uint64_t dest_addr, uint64_t buf, struct sock_ep_attr *ep_attr,
		struct sock_conn *conn)
{
	sock_tx_ctx_write(tx_ctx, op, sizeof(*op));
	sock_tx_ctx_write(tx_ctx, &flags, sizeof(flags));
	sock_tx_ctx_write(tx_ctx, &context, sizeof(context));
	sock_tx_ctx_write(tx_ctx, &dest_addr, sizeof(dest_addr));
	sock_tx_ctx_write(tx_ctx, &buf, sizeof(buf));
	sock_tx_ctx_write(tx_ctx, &ep_attr, sizeof(ep_attr));
	sock_tx_ctx_write(tx_ctx, &conn, sizeof(conn));
}

void sock_tx_ctx_write_op_tsend(struct sock_tx_ctx *tx_ctx,
		struct sock_op *op, uint64_t flags, uint64_t context,
		uint64_t dest_addr, uint64_t buf, struct sock_ep_attr *ep_attr,
		struct sock_conn *conn, uint64_t tag)
{
	sock_tx_ctx_write_op_send(tx_ctx, op, flags, context, dest_addr,
			buf, ep_attr, conn);
	sock_tx_ctx_write(tx_ctx, &tag, sizeof(tag));
}

void sock_tx_ctx_read_op_send(struct sock_tx_ctx *tx_ctx,
		struct sock_op *op, uint64_t *flags, uint64_t *context,
		uint64_t *dest_addr, uint64_t *buf, struct sock_ep_attr **ep_attr,
		struct sock_conn **conn)
{
	ofi_rbread(&tx_ctx->rb, op, sizeof(*op));
	ofi_rbread(&tx_ctx->rb, flags, sizeof(*flags));
	ofi_rbread(&tx_ctx->rb, context, sizeof(*context));
	ofi_rbread(&tx_ctx->rb, dest_addr, sizeof(*dest_addr));
	ofi_rbread(&tx_ctx->rb, buf, sizeof(*buf));
	ofi_rbread(&tx_ctx->rb, ep_attr, sizeof(*ep_attr));
	ofi_rbread(&tx_ctx->rb, conn, sizeof(*conn));
}
