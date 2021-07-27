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

static int
smr_write_err_comp(struct util_cq *cq, void *context,
		   uint64_t flags, uint64_t tag, uint64_t err)
{
	struct fi_cq_err_entry err_entry;

	memset(&err_entry, 0, sizeof err_entry);
	err_entry.op_context = context;
	err_entry.flags = flags;
	err_entry.tag = tag;
	err_entry.err = err;
	err_entry.prov_errno = -err;
	return ofi_cq_insert_error(cq, &err_entry);
}

static int
smr_write_comp(struct util_cq *cq, void *context,
	       uint64_t flags, size_t len, void *buf,
	       uint64_t tag, uint64_t data, uint64_t err)
{
	if (err)
		return smr_write_err_comp(cq, context, flags, tag, err);

	if (ofi_cirque_freecnt(cq->cirq) > 1) {
		ofi_cq_write_entry(cq, context, flags, len,
				   buf, data, tag);
		return 0;
	} else {
		return ofi_cq_write_overflow(cq, context, flags,
					     len, buf, data, tag,
					     FI_ADDR_NOTAVAIL);
	}
}

static int
smr_write_src_comp(struct util_cq *cq, void *context,
		   uint64_t flags, size_t len, void *buf, fi_addr_t addr,
		   uint64_t tag, uint64_t data, uint64_t err)
{
	if (err)
		return smr_write_err_comp(cq, context, flags, tag, err);

	if (ofi_cirque_freecnt(cq->cirq) > 1) {
		ofi_cq_write_src_entry(cq, context, flags, len,
				       buf, data, tag, addr);
		return 0;
	} else {
		return ofi_cq_write_overflow(cq, context, flags,
					     len, buf, data, tag, addr);
	}
}

int smr_tx_comp(struct smr_ep *ep, void *context, uint32_t op,
		uint16_t flags, uint64_t err)
{
	return smr_write_comp(ep->util_ep.tx_cq, context,
			      ofi_tx_cq_flags(op), 0, NULL, 0, 0, err);
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

int smr_complete_rx(struct smr_ep *ep, void *context, uint32_t op,
		    uint16_t flags, size_t len, void *buf, int64_t id,
		    uint64_t tag, uint64_t data, uint64_t err)
{
	fi_addr_t fiaddr = FI_ADDR_UNSPEC;

	ofi_ep_rx_cntr_inc_func(&ep->util_ep, op);

	if (!err && !(flags & (SMR_REMOTE_CQ_DATA | SMR_RX_COMPLETION)))
		return 0;

	if (ep->util_ep.domain->info_domain_caps & FI_SOURCE)
		fiaddr = ep->region->map->peers[id].fiaddr;

	return ep->rx_comp(ep, context, op, flags, len, buf,
			   fiaddr, tag, data, err);
}

int smr_rx_comp(struct smr_ep *ep, void *context, uint32_t op,
		uint16_t flags, size_t len, void *buf, fi_addr_t addr,
		uint64_t tag, uint64_t data, uint64_t err)
{
	return smr_write_comp(ep->util_ep.rx_cq, context,
			      smr_rx_cq_flags(op, flags), len, buf,
			      tag, data, err);
}

int smr_rx_src_comp(struct smr_ep *ep, void *context, uint32_t op,
		    uint16_t flags, size_t len, void *buf, fi_addr_t addr,
		    uint64_t tag, uint64_t data, uint64_t err)
{
	return smr_write_src_comp(ep->util_ep.rx_cq, context,
				  smr_rx_cq_flags(op, flags), len, buf, addr,
				  tag, data, err);
}

int smr_rx_comp_signal(struct smr_ep *ep, void *context, uint32_t op,
		       uint16_t flags, size_t len, void *buf, fi_addr_t addr,
		       uint64_t tag, uint64_t data, uint64_t err)
{
	int ret;

	ret = smr_rx_comp(ep, context, op, flags, len, buf, addr, tag,
			  data, err);
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
