/*
 * Copyright (c) Intel Corporation. All rights reserved
 * Copyright (c) Amazon.com, Inc. or its affiliates. All rights reserved.
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
#include "sm2.h"

int sm2_complete_tx(struct sm2_ep *ep, void *context, uint32_t op,
		    uint64_t flags)
{
	ofi_ep_peer_tx_cntr_inc(&ep->util_ep, op);

	if (!(flags & FI_COMPLETION))
		return 0;

	return ofi_peer_cq_write(ep->util_ep.tx_cq, context,
				 ofi_tx_cq_flags(op), 0, NULL, 0, 0,
				 FI_ADDR_NOTAVAIL);
}

int sm2_write_err_comp(struct util_cq *cq, void *context, uint64_t flags,
		       uint64_t tag, uint64_t err)
{
	struct fi_cq_err_entry err_entry;
	memset(&err_entry, 0, sizeof err_entry);
	err_entry.op_context = context;
	err_entry.flags = flags;
	err_entry.tag = tag;
	err_entry.err = err;
	err_entry.prov_errno = -err;
	return ofi_peer_cq_write_error(cq, &err_entry);
}

int sm2_complete_rx(struct sm2_ep *ep, void *context, uint32_t op,
		    uint64_t flags, size_t len, void *buf, sm2_gid_t gid,
		    uint64_t tag, uint64_t data)
{
	struct sm2_av *sm2_av;

	ofi_ep_peer_rx_cntr_inc(&ep->util_ep, op);

	if (!(flags & (FI_REMOTE_CQ_DATA | FI_COMPLETION)))
		return 0;

	flags &= ~FI_COMPLETION;

	sm2_av = container_of(ep->util_ep.av, struct sm2_av, util_av);

	return ofi_peer_cq_write(ep->util_ep.rx_cq, context, flags, len, buf,
				 data, tag, sm2_av->reverse_lookup[gid]);
}
