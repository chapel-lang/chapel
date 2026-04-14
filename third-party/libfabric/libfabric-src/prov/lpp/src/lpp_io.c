/*
 * Copyright (c) 2018-2024 GigaIO, Inc. All Rights Reserved.
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

#include <stdatomic.h>

#include "lpp.h"

static int lpp_try_pio(struct lpp_domain *lpp_domainp, struct lpp_ep *lpp_epp,
		       const struct iovec *local_iovp, size_t local_iov_count,
		       const struct fi_rma_iov *remote_iovp,
		       size_t remote_iov_count, fi_addr_t addr, void *context,
		       struct lpp_stx *lpp_stxp, uint64_t flags)
{

	int			status = 0;
	void			*baseaddr = NULL;
	void			*rmr_buf = NULL;
	size_t			write_length = local_iovp->iov_len;
	uint64_t		offset_in_region = 0;
	uint64_t		legal_offset;
	uint64_t		dst_addr = remote_iovp->addr;
	uint64_t		dst_key = remote_iovp->key;
	void			*src_buf = local_iovp->iov_base;
	struct klpp_rmr_meta	*rmr_meta = NULL;
	struct lpp_fi_addr	*fiaddr = (struct lpp_fi_addr*)&addr;
	size_t			meta_count;

	// Only attempt PIO if enabled.
	if (lpp_disable_osbypass)
		return -1;

	// Only 1 IOV on local and remote sides allowed.
	if ((local_iov_count != 1) || (remote_iov_count != 1))
		return -1;

	if (((flags & FI_WRITE) && local_iovp->iov_len > lpp_max_wr_osbypass_size) ||
	    ((flags & FI_READ) &&
	    ((local_iovp->iov_len > lpp_max_rd_osbypass_size) || lpp_write_only)))
		return -1;

	if (fiaddr->node_id > lpp_domainp->devinfo.host_count) {
		FI_WARN(&lpp_prov, FI_LOG_EP_DATA, "node_id %d is larger than host_count %d\n",
		    fiaddr->node_id, lpp_domainp->devinfo.host_count);
		return -1;
	}

	/* If there are RMAs in progress from this endpoint to the target
	 * endpoint, we must go to the kernel and get in line to avoid
	 * violating ordering constraints. */
	if (lpp_epp->io_stat[fiaddr->node_id].rma_in_prog_cnt != 0) {
		FI_DBG(&lpp_prov, FI_LOG_EP_DATA, "RMAs in progress, can't start PIO\n");
		return -1;
	}

	/* Check that we have a remote RMR to receive the data.  Also, check
	 * that the RMR is big enough to contain all of the data. */
	meta_count = klpp_rmr_meta_count(&lpp_domainp->devinfo.rmr_region_info,
					 lpp_domainp->devinfo.host_count);
	rmr_meta = klpp_rmr_meta_find(lpp_domainp->rmr_meta_arr, meta_count,
				      dst_key, fiaddr->node_id);
	if (rmr_meta == NULL) {
		FI_DBG(&lpp_prov, FI_LOG_EP_DATA, "rmr_meta not found, not doing PIO\n");
		return -1;
	}

	if (dst_addr < rmr_meta->legal_remote_start_uaddr) {
		FI_WARN(&lpp_prov, FI_LOG_EP_DATA,
			"PIO start out of bounds %lx %lx\n", dst_addr,
			rmr_meta->legal_remote_start_uaddr);
		return -1;
	}

	offset_in_region = dst_addr - rmr_meta->remote_start_uaddr;
	legal_offset = dst_addr - rmr_meta->legal_remote_start_uaddr;

	if (rmr_meta->legal_length < legal_offset + write_length) {
		FI_WARN(&lpp_prov, FI_LOG_EP_DATA,
			"PIO write out of range (region len: %ld offset: %ld len:%ld\n",
			rmr_meta->legal_length, legal_offset, write_length);
		return -1;
	}

	baseaddr = (rmr_meta->region == KLPP_RMR_RW) ? lpp_domainp->rmr_rw : lpp_domainp->rmr_ro;
	rmr_buf = (void *)((uintptr_t)baseaddr + rmr_meta->offset + offset_in_region);

	if (flags & FI_WRITE) {
		memcpy(rmr_buf, src_buf, write_length);
	} else {
		memcpy(src_buf, rmr_buf, write_length);
	}

	// Ensure all writes are complete before generating the completion.
	atomic_thread_fence(memory_order_seq_cst);

	lpp_completion_transmit(lpp_epp, lpp_epp->cntr_write,
				flags | FI_OS_BYPASS, 0, context, status);
	FI_DBG(&lpp_prov, FI_LOG_EP_DATA, "PIO write done successfully\n");

	return status;
}

ssize_t lpp_rma_common(struct fid_ep *ep, const struct iovec *local_iovp,
		       size_t local_iov_count,
		       const struct fi_rma_iov *remote_iovp,
		       size_t remote_iov_count,
		       fi_addr_t addr, void *context, uint64_t flags)
{

	struct lpp_ep		*lpp_epp = NULL;
	struct lpp_stx		*lpp_stxp = NULL;
	struct lpp_domain	*lpp_domainp = NULL;
	bool			cq_slot_reserved = false;
	int			ret;

	if (remote_iov_count == 1) {
		FI_DBG(&lpp_prov, FI_LOG_MR,
		       "RMA op on remote key: 0x%lx, flags: 0x%lx, ctx: %p\n",
			remote_iovp->key, flags, context);
	} else {
		FI_DBG(&lpp_prov, FI_LOG_MR, "RMA op iov_count: %ld\n",
		       remote_iov_count);
	}

	// If we don't have an EP, error out.
	if (ep == NULL) {
		FI_WARN(&lpp_prov, FI_LOG_EP_DATA, "endpoint is NULL\n");
		return -FI_EINVAL;
	}

	lpp_epp = container_of(ep, struct lpp_ep, ep_fid);
	if (lpp_epp->is_enabled == 0) {
		FI_WARN(&lpp_prov, FI_LOG_EP_DATA, "endpoint is not enabled\n");
		return -FI_EINVAL;
	}

	lpp_domainp = lpp_epp->domain;

	// Get operation specific values.
	if (lpp_stxp = lpp_epp->stx, lpp_stxp == NULL) {
		FI_WARN(&lpp_prov, FI_LOG_EP_DATA, "STX ctx is NULL\n");
		return -FI_EINVAL;
	}

	if ((lpp_stxp->attr.caps & flags & LPP_CAPS_OPS) != (flags & LPP_CAPS_OPS)) {
		FI_WARN(&lpp_prov, FI_LOG_EP_DATA, "TX attr caps (%lx) incorrect for requested op (%lx)\n",
			lpp_stxp->attr.caps, flags);
		return -FI_EINVAL;
	}

	// Make sure that we won't overrun our IOVs in the descriptors.
	if (local_iov_count > lpp_stxp->attr.rma_iov_limit) {
		FI_WARN(&lpp_prov, FI_LOG_EP_DATA, "local iov count out of range, (%lu/%lu)\n",
		    local_iov_count, lpp_stxp->attr.rma_iov_limit);
		return -FI_EINVAL;
	}
	if (remote_iov_count > lpp_stxp->attr.rma_iov_limit) {
		FI_WARN(&lpp_prov, FI_LOG_EP_DATA, "remote iov count out of range, (%lu/%lu)\n",
		    remote_iov_count, lpp_stxp->attr.rma_iov_limit);
		return -FI_EINVAL;
	}

	// Make sure that the inject data isn't too big.
	if (((flags & FI_INJECT) != 0) && (local_iovp->iov_len > lpp_stxp->attr.inject_size)) {
		FI_WARN(&lpp_prov, FI_LOG_EP_DATA, "inject size %ld exceeds maximum %ld\n",
		    local_iovp->iov_len, lpp_stxp->attr.inject_size);
		return -FI_EINVAL;
	}

	lpp_ep_op_lock(lpp_epp);

	ret = lpp_cq_reserve_slot(lpp_epp->cq_transmit, flags);
	if (ret > 0) {
		cq_slot_reserved = true;
	} else if (ret < 0) {
		FI_INFO(&lpp_prov, FI_LOG_CQ, "cannot start IO as it may overrun the CQ\n");
		ret = -FI_EAGAIN;
		goto unlock;
	}

	if (lpp_try_pio(lpp_domainp, lpp_epp, local_iovp, local_iov_count,
			remote_iovp, remote_iov_count, addr, context,
			lpp_stxp, flags) == 0) {
		// If the PIO succeeded, we're done. The data is on the other side by now.
		if (cq_slot_reserved)
			lpp_cq_unreserve_slot(lpp_epp->cq_transmit);

		ret = 0;
		goto unlock;
	}

	if (local_iov_count > lpp_stxp->attr.rma_iov_limit) {
		FI_WARN(&lpp_prov, FI_LOG_EP_DATA, "local iov_count %ld exceeds max %ld\n",
		    local_iov_count, lpp_stxp->attr.rma_iov_limit);
	}
	if (remote_iov_count > lpp_stxp->attr.rma_iov_limit) {
		FI_WARN(&lpp_prov, FI_LOG_EP_DATA, "remote iov_count %ld exceeds max %ld\n",
		    remote_iov_count, lpp_stxp->attr.rma_iov_limit);
	}

	/* TODO: FI_INJECT isn't handleled properly. The user can modify
	 * local_iovp->iov_base before the DMA tx finishes. Should copy the
	 * contents to an object that gets freed when the completion is posted. */
	ret = klpp_readwrite(lpp_epp, addr, local_iovp->iov_len, flags,
			     context, local_iovp->iov_base, remote_iovp->addr,
			     remote_iovp->key);

unlock:
	if (ret != 0 && cq_slot_reserved)
		lpp_cq_unreserve_slot(lpp_epp->cq_transmit);

	lpp_ep_op_unlock(lpp_epp);

	return ret;
}
