/*
 * Copyright (C) 2016 by Argonne National Laboratory.
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
#include "rdma/bgq/fi_bgq.h"
#include <ofi.h>
#include <ofi_enosys.h>
#include <stdlib.h>

static int fi_bgq_close_cntr(struct fid *fid)
{
	int ret;
	struct fi_bgq_cntr *bgq_cntr =
		container_of(fid, struct fi_bgq_cntr, cntr_fid);

	ret = fi_bgq_fid_check(fid, FI_CLASS_CNTR, "counter");
	if (ret)
		return ret;

	struct l2atomic_lock * lock = &bgq_cntr->domain->mu.lock;
	struct fi_bgq_node * node = &bgq_cntr->domain->fabric->node;
	fi_bgq_node_bat_free(node, lock, bgq_cntr->std.batid);
	fi_bgq_node_bat_free(node, lock, bgq_cntr->err.batid);

	ret = fi_bgq_ref_dec(&bgq_cntr->domain->ref_cnt, "domain");
	if (ret)
		return ret;

	free(bgq_cntr->attr);
	free(bgq_cntr);
	return 0;
}

static int fi_bgq_bind_cntr(struct fid *fid, struct fid *bfid, uint64_t flags)
{
	errno = FI_ENOSYS;
	return -errno;
}

static uint64_t fi_bgq_cntr_read(struct fid_cntr *cntr)
{
	struct fi_bgq_cntr *bgq_cntr =
		container_of(cntr, struct fi_bgq_cntr, cntr_fid);

	const uint64_t value = L2_AtomicLoad(bgq_cntr->std.l2_vaddr);

	if (FI_BGQ_FABRIC_DIRECT_PROGRESS == FI_PROGRESS_MANUAL) {
		const uint64_t count = bgq_cntr->progress.ep_count;
		uint64_t i;
		for (i=0; i<count; ++i) {
			fi_bgq_ep_progress_manual(bgq_cntr->progress.ep[i]);
		}
	}

	return value;
}

static uint64_t fi_bgq_cntr_readerr(struct fid_cntr *cntr)
{
	struct fi_bgq_cntr *bgq_cntr =
		container_of(cntr, struct fi_bgq_cntr, cntr_fid);

	return L2_AtomicLoad(bgq_cntr->err.l2_vaddr);
}

static int fi_bgq_cntr_add(struct fid_cntr *cntr, uint64_t value)
{
	struct fi_bgq_cntr *bgq_cntr =
		container_of(cntr, struct fi_bgq_cntr, cntr_fid);

	L2_AtomicStoreAdd(bgq_cntr->std.l2_vaddr, value);

	return 0;
}

static int fi_bgq_cntr_set(struct fid_cntr *cntr, uint64_t value)
{
	struct fi_bgq_cntr *bgq_cntr =
		container_of(cntr, struct fi_bgq_cntr, cntr_fid);

	L2_AtomicStore(bgq_cntr->std.l2_vaddr, value);

	return 0;
}

static int
fi_bgq_cntr_wait(struct fid_cntr *cntr, uint64_t threshold, int timeout)
{
	struct fi_bgq_cntr *bgq_cntr =
		container_of(cntr, struct fi_bgq_cntr, cntr_fid);

	uint64_t timeout_cycles = (timeout < 0) ?
		ULLONG_MAX :
		GetTimeBase() + (1600UL * 1000 * timeout);

	uint64_t current_value = 0;
	do {
		current_value = L2_AtomicLoad(bgq_cntr->std.l2_vaddr);

		if (FI_BGQ_FABRIC_DIRECT_PROGRESS == FI_PROGRESS_MANUAL) {
			const uint64_t count = bgq_cntr->progress.ep_count;
			uint64_t i;
			for (i=0; i<count; ++i) {
				fi_bgq_ep_progress_manual(bgq_cntr->progress.ep[i]);
			}
		}

		if (threshold <= current_value) return 0;
	} while (GetTimeBase() < timeout_cycles);

	errno = FI_ETIMEDOUT;
	return -errno;
}

static struct fi_ops fi_bgq_fi_ops = {
	.size		= sizeof(struct fi_ops),
	.close		= fi_bgq_close_cntr,
	.bind		= fi_bgq_bind_cntr,
	.control	= fi_no_control,
	.ops_open	= fi_no_ops_open
};

int fi_bgq_bind_ep_cntr(struct fi_bgq_ep *bgq_ep,
		struct fi_bgq_cntr *bgq_cntr, uint64_t flags)
{
	if (!(flags & (FI_WRITE |
			FI_READ |
			FI_SEND |
			FI_RECV |
			FI_REMOTE_READ |
			FI_REMOTE_WRITE))) {
		FI_LOG(fi_bgq_global.prov, FI_LOG_DEBUG, FI_LOG_CQ,
				"unclear flags while binding counter\n");
		goto err;
	}

	if (flags & FI_WRITE)
		bgq_ep->write_cntr = bgq_cntr;
	if (flags & FI_READ)
		bgq_ep->read_cntr = bgq_cntr;
	if (flags & FI_SEND)
		bgq_ep->send_cntr = bgq_cntr;
	if (flags & FI_RECV)
		bgq_ep->recv_cntr = bgq_cntr;

	bgq_cntr->ep[(bgq_cntr->ep_bind_count)++] = bgq_ep;

	if (ofi_recv_allowed(bgq_ep->rx.caps) || ofi_rma_target_allowed(bgq_ep->rx.caps)) {
		bgq_cntr->progress.ep[(bgq_cntr->progress.ep_count)++] = bgq_ep;
	}

	return 0;
err:
	errno = FI_EINVAL;
	return -errno;
}

static struct fi_ops_cntr fi_bgq_ops_cntr = {
	.size		= sizeof(struct fi_ops_cntr),
	.read		= fi_bgq_cntr_read,
	.readerr	= fi_bgq_cntr_readerr,
	.add		= fi_bgq_cntr_add,
	.set		= fi_bgq_cntr_set,
	.wait		= fi_bgq_cntr_wait
};

int fi_bgq_cntr_open(struct fid_domain *domain,
		struct fi_cntr_attr *attr,
		struct fid_cntr **cntr, void *context)
{
	int ret;
	struct fi_bgq_cntr *bgq_cntr;

	if (!attr) {
		FI_LOG(fi_bgq_global.prov, FI_LOG_DEBUG, FI_LOG_CQ,
				"no attr supplied\n");
		errno = FI_EINVAL;
		return -errno;
	}
	ret = fi_bgq_fid_check(&domain->fid, FI_CLASS_DOMAIN, "domain");
	if (ret)
		return ret;

	bgq_cntr = calloc(1, sizeof(*bgq_cntr));
	if (!bgq_cntr) {
		errno = FI_ENOMEM;
		goto err;
	}

	bgq_cntr->cntr_fid.fid.fclass	= FI_CLASS_CNTR;
	bgq_cntr->cntr_fid.fid.context	= context;
	bgq_cntr->cntr_fid.fid.ops	= &fi_bgq_fi_ops;
	bgq_cntr->cntr_fid.ops		= &fi_bgq_ops_cntr;

	bgq_cntr->domain = (struct fi_bgq_domain *) domain;

	/* ---- allocate and initialize the "std" and "err" mu/l2 counters ---- */
	{
		uint32_t cnk_rc __attribute__ ((unused));
		struct l2atomic_lock * lock = &bgq_cntr->domain->mu.lock;
		struct fi_bgq_node * node = &bgq_cntr->domain->fabric->node;

		/* ---- initialize the "std" counter ---- */
		bgq_cntr->std.batid =
			fi_bgq_node_bat_allocate(node, lock);

		bgq_cntr->std.paddr =
			node->bat.l2_cntr_paddr[bgq_cntr->std.batid];

		cnk_rc = Kernel_Physical2Virtual(
			(void *)bgq_cntr->std.paddr,
			(void**)&bgq_cntr->std.l2_vaddr);
		assert(cnk_rc == 0);

		L2_AtomicStore(bgq_cntr->std.l2_vaddr, 0);

		fi_bgq_node_bat_write(node, lock,
			bgq_cntr->std.batid,
			bgq_cntr->std.paddr);

		/* ---- initialize the "err" counter ---- */
		bgq_cntr->err.batid =
			fi_bgq_node_bat_allocate(node, lock);

		bgq_cntr->err.paddr =
			node->bat.l2_cntr_paddr[bgq_cntr->err.batid];

		cnk_rc = Kernel_Physical2Virtual(
			(void *)bgq_cntr->err.paddr,
			(void**)&bgq_cntr->err.l2_vaddr);
		assert(cnk_rc == 0);

		L2_AtomicStore(bgq_cntr->err.l2_vaddr, 0);

		fi_bgq_node_bat_write(node, lock,
			bgq_cntr->err.batid,
			bgq_cntr->err.paddr);
	}

	bgq_cntr->ep_bind_count = 0;
	bgq_cntr->progress.ep_count = 0;
	unsigned i;
	for (i=0; i<64; ++i) {			/* TODO - check this array size */
		bgq_cntr->ep[i] = NULL;
		bgq_cntr->progress.ep[i] = NULL;
	}

	fi_bgq_ref_inc(&bgq_cntr->domain->ref_cnt, "domain");

	*cntr = &bgq_cntr->cntr_fid;
	return 0;
err:
	if (bgq_cntr)
		free(bgq_cntr);
	return -errno;
}
