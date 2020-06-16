/*
 * Copyright (c) 2013-2018 Intel Corporation. All rights reserved
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

#include <stdlib.h>
#include <string.h>
#include <sys/uio.h>

#include "ofi_iov.h"
#include "smr.h"

int smr_complete_tx(struct smr_ep *ep, void *context, uint32_t op,
		    uint16_t flags, uint64_t err)
{
	ofi_ep_tx_cntr_inc_func(&ep->util_ep, op);

	if (!err && !(flags & SMR_TX_COMPLETION))
		return 0;

	return ep->tx_comp(ep, context, op, flags, err);
}

int smr_tx_comp(struct smr_ep *ep, void *context, uint32_t op,
		uint16_t flags, uint64_t err)
{
	struct fi_cq_tagged_entry *comp;
	struct util_cq_oflow_err_entry *entry;

	comp = ofi_cirque_tail(ep->util_ep.tx_cq->cirq);
	if (err) {
		if (!(entry = calloc(1, sizeof(*entry))))
			return -FI_ENOMEM;
		entry->comp.op_context = context;
		entry->comp.flags = ofi_tx_cq_flags(op);
		entry->comp.err = err;
		entry->comp.prov_errno = -err;
		slist_insert_tail(&entry->list_entry,
				  &ep->util_ep.tx_cq->oflow_err_list);
		comp->flags = UTIL_FLAG_ERROR;
	} else {
		comp->op_context = context;
		comp->flags = ofi_tx_cq_flags(op);
		comp->len = 0;
		comp->buf = NULL;
		comp->data = 0;
	}
	ofi_cirque_commit(ep->util_ep.tx_cq->cirq);
	return 0;
}

int smr_tx_comp_signal(struct smr_ep *ep, void *context, uint32_t op,
		uint16_t flags, uint64_t err)
{
	int ret;

	ret = smr_tx_comp(ep, context, op, flags, err);
	if (ret)
		return ret;
	ep->util_ep.tx_cq->wait->signal(ep->util_ep.tx_cq->wait);
	return 0;
}

int smr_complete_rx(struct smr_ep *ep, void *context, uint32_t op, uint16_t flags,
		    size_t len, void *buf, fi_addr_t addr, uint64_t tag, uint64_t data,
		    uint64_t err)
{
	ofi_ep_rx_cntr_inc_func(&ep->util_ep, op);

	if (!err && !(flags & (SMR_REMOTE_CQ_DATA | SMR_RX_COMPLETION)))
		return 0;

	return ep->rx_comp(ep, context, op, flags, len, buf,
			   addr, tag, data, err);
}

int smr_rx_comp(struct smr_ep *ep, void *context, uint32_t op,
		uint16_t flags, size_t len, void *buf, fi_addr_t addr,
		uint64_t tag, uint64_t data, uint64_t err)
{
	struct fi_cq_tagged_entry *comp;
	struct util_cq_oflow_err_entry *entry;

	if (ofi_cirque_isfull(ep->util_ep.rx_cq->cirq))
		return ofi_cq_write_overflow(ep->util_ep.rx_cq, context,
					     smr_rx_cq_flags(op, flags),
					     len, buf, data, tag, addr);

	comp = ofi_cirque_tail(ep->util_ep.rx_cq->cirq);
	if (err) {
		if (!(entry = calloc(1, sizeof(*entry))))
			return -FI_ENOMEM;
		entry->comp.op_context = context;
		entry->comp.flags = smr_rx_cq_flags(op, flags);
		entry->comp.tag = tag;
		entry->comp.err = err;
		entry->comp.prov_errno = -err;
		slist_insert_tail(&entry->list_entry,
				  &ep->util_ep.rx_cq->oflow_err_list);
		comp->flags = UTIL_FLAG_ERROR;
	} else {
		comp->op_context = context;
		comp->flags = smr_rx_cq_flags(op, flags);
		comp->len = len;
		comp->buf = buf;
		comp->data = data;
		comp->tag = tag;
	}
	ofi_cirque_commit(ep->util_ep.rx_cq->cirq);
	return 0;
}

int smr_rx_src_comp(struct smr_ep *ep, void *context, uint32_t op,
		    uint16_t flags, size_t len, void *buf, fi_addr_t addr,
		    uint64_t tag, uint64_t data, uint64_t err)
{
	ep->util_ep.rx_cq->src[ofi_cirque_windex(ep->util_ep.rx_cq->cirq)] = addr;
	return smr_rx_comp(ep, context, op, flags, len, buf, addr, tag,
			   data, err);
}

int smr_rx_comp_signal(struct smr_ep *ep, void *context, uint32_t op,
		       uint16_t flags, size_t len, void *buf, fi_addr_t addr,
		       uint64_t tag, uint64_t data, uint64_t err)
{
	int ret;

	ret = smr_rx_comp(ep, context, op, flags, len, buf, addr, tag, data, err);
	if (ret)
		return ret;
	ep->util_ep.rx_cq->wait->signal(ep->util_ep.rx_cq->wait);
	return 0;
}

int smr_rx_src_comp_signal(struct smr_ep *ep, void *context, uint32_t op,
			   uint16_t flags, size_t len, void *buf, fi_addr_t addr,
			   uint64_t tag, uint64_t data, uint64_t err)
{
	int ret;

	ret = smr_rx_src_comp(ep, context, op, flags, len, buf, addr,
			      tag, data, err);
	if (ret)
		return ret;
	ep->util_ep.rx_cq->wait->signal(ep->util_ep.rx_cq->wait);
	return 0;

}

uint64_t smr_rx_cq_flags(uint32_t op, uint16_t op_flags)
{
	uint64_t flags;

	flags = ofi_rx_cq_flags(op);

	if (op_flags & SMR_REMOTE_CQ_DATA)
		flags |= FI_REMOTE_CQ_DATA;

	if (op_flags & SMR_MULTI_RECV)
		flags |= FI_MULTI_RECV;

	return flags;
}
