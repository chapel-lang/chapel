/*
 * Copyright (C) 2016 by Argonne National Laboratory.
 * Copyright (C) 2021-2023 by Cornelis Networks.
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
#include <ofi.h>

#include "rdma/opx/fi_opx_domain.h"
#include "rdma/opx/fi_opx_endpoint.h"
#include "rdma/opx/fi_opx_eq.h"
#include "rdma/opx/fi_opx.h"

#include <ofi_enosys.h>
#include <stdlib.h>

static int fi_opx_close_cntr(struct fid *fid)
{
	int ret;
	struct fi_opx_cntr *opx_cntr =
		container_of(fid, struct fi_opx_cntr, cntr_fid);

	ret = fi_opx_fid_check(fid, FI_CLASS_CNTR, "counter");
	if (ret)
		return ret;

	ret = fi_opx_ref_dec(&opx_cntr->domain->ref_cnt, "domain");
	if (ret)
		return ret;

	free(opx_cntr->attr);
	opx_cntr->attr = NULL;
	free(opx_cntr);
	opx_cntr = NULL;
	//opx_cntr (the object passed in as fid) is now unusable
	return 0;
}

static int fi_opx_bind_cntr(struct fid *fid, struct fid *bfid, uint64_t flags)
{
	errno = FI_ENOSYS;
	return -errno;
}

static uint64_t fi_opx_cntr_read(struct fid_cntr *cntr)
{
	struct fi_opx_cntr *opx_cntr =
		container_of(cntr, struct fi_opx_cntr, cntr_fid);

	if (IS_PROGRESS_MANUAL(opx_cntr->domain)) {
		const uint64_t count = opx_cntr->progress.ep_count;
		uint64_t i;
		if (OFI_UNLIKELY(opx_cntr->lock_required)) {
			for (i=0; i<count; ++i) {
				fi_opx_lock(&opx_cntr->progress.ep[i]->lock);
				fi_opx_ep_rx_poll(&opx_cntr->progress.ep[i]->ep_fid, 0, OPX_RELIABILITY, FI_OPX_HDRQ_MASK_RUNTIME);
				fi_opx_unlock(&opx_cntr->progress.ep[i]->lock);
			}
		} else {
			for (i=0; i<count; ++i) {
				fi_opx_ep_rx_poll(&opx_cntr->progress.ep[i]->ep_fid, 0, OPX_RELIABILITY, FI_OPX_HDRQ_MASK_RUNTIME);
			}
		}
	}
	return ofi_atomic_get64(&opx_cntr->std);
}

static uint64_t fi_opx_cntr_readerr(struct fid_cntr *cntr)
{
	struct fi_opx_cntr *opx_cntr =
		container_of(cntr, struct fi_opx_cntr, cntr_fid);
    return ofi_atomic_get64(&opx_cntr->err);
}

static int fi_opx_cntr_add(struct fid_cntr *cntr, uint64_t value)
{
    struct fi_opx_cntr *opx_cntr =
            container_of(cntr, struct fi_opx_cntr, cntr_fid);

	return ofi_atomic_add64(&opx_cntr->std, value);
}

static int fi_opx_cntr_set(struct fid_cntr *cntr, uint64_t value)
{
	struct fi_opx_cntr *opx_cntr = container_of(cntr, struct fi_opx_cntr, cntr_fid);

	return ofi_atomic_set64(&opx_cntr->std, value);
}

static inline struct timespec fi_opx_cntr_timediff(struct timespec start, struct timespec end)
{
        struct timespec temp;
        if ((end.tv_nsec - start.tv_nsec) < 0)
        {
                temp.tv_sec = end.tv_sec - start.tv_sec - 1;
                temp.tv_nsec = 1000000000 + end.tv_nsec - start.tv_nsec;
        }
        else
        {
                temp.tv_sec = end.tv_sec - start.tv_sec;
                temp.tv_nsec = end.tv_nsec - start.tv_nsec;
        }
        return temp;
}

static int
fi_opx_cntr_wait(struct fid_cntr *cntr, uint64_t threshold, int timeout)
{
	struct fi_opx_cntr *opx_cntr =
		container_of(cntr, struct fi_opx_cntr, cntr_fid);

	struct timespec tpi, tpf, tpdiff;
	clock_gettime(CLOCK_MONOTONIC, &tpi);
	uint64_t timeout_ns = timeout * 1e6; // ms to ns
	uint64_t timeout_elapsed_ns = 0;
	uint64_t current_value = 0;
	ofi_atomic64_t *std = &opx_cntr->std;

	do {
		current_value = ofi_atomic_get64(std);

		if (IS_PROGRESS_MANUAL(opx_cntr->domain)) {
			const uint64_t count = opx_cntr->progress.ep_count;
			uint64_t i;
			if (OFI_UNLIKELY(opx_cntr->lock_required)) {
				for (i=0; i<count; ++i) {
					fi_opx_lock(&opx_cntr->progress.ep[i]->lock);
					fi_opx_ep_rx_poll(&opx_cntr->progress.ep[i]->ep_fid, 0,
							  OPX_RELIABILITY,
							  FI_OPX_HDRQ_MASK_RUNTIME);
					fi_opx_unlock(&opx_cntr->progress.ep[i]->lock);
				}
			} else {
				for (i=0; i<count; ++i) {
					fi_opx_ep_rx_poll(&opx_cntr->progress.ep[i]->ep_fid, 0,
							  OPX_RELIABILITY,
							  FI_OPX_HDRQ_MASK_RUNTIME);
				}
			}
		}

		if (threshold <= current_value) {
			return 0;
		}

		clock_gettime(CLOCK_MONOTONIC, &tpf);
		tpdiff = fi_opx_cntr_timediff(tpi, tpf);
		timeout_elapsed_ns = (tpdiff.tv_sec*1e9 + tpdiff.tv_nsec);

	} while (timeout_elapsed_ns < timeout_ns);

	errno = FI_ETIMEDOUT;
	return -errno;
}

static struct fi_ops fi_opx_fi_ops = {
	.size		= sizeof(struct fi_ops),
	.close		= fi_opx_close_cntr,
	.bind		= fi_opx_bind_cntr,
	.control	= fi_no_control,
	.ops_open	= fi_no_ops_open
};

int fi_opx_bind_ep_cntr(struct fid_ep *ep,
		struct fid_cntr *cntr, uint64_t flags)
{
	struct fi_opx_cntr *opx_cntr =
		container_of(cntr, struct fi_opx_cntr, cntr_fid);

	struct fi_opx_ep *opx_ep =
		container_of(ep, struct fi_opx_ep, ep_fid);

	if (!(flags & (FI_WRITE |
			FI_READ |
			FI_SEND |
			FI_RECV |
			FI_REMOTE_READ |
			FI_REMOTE_WRITE))) {
		FI_LOG(fi_opx_global.prov, FI_LOG_DEBUG, FI_LOG_CQ,
				"unclear flags while binding counter\n");
		goto err;
	}

	if (flags & FI_WRITE) {
		opx_ep->write_cntr = opx_cntr;
		opx_ep->init_write_cntr = opx_cntr;
	}
	if (flags & FI_READ) {
		opx_ep->read_cntr = opx_cntr;
		opx_ep->init_read_cntr = opx_cntr;
	}
	if (flags & FI_SEND) {
		opx_ep->send_cntr = opx_cntr;
		opx_ep->init_send_cntr = opx_cntr;
	}
	if (flags & FI_RECV) {
		opx_ep->recv_cntr = opx_cntr;
		opx_ep->init_recv_cntr = opx_cntr;
	}

	opx_cntr->ep[(opx_cntr->ep_bind_count)++] = opx_ep;

	return 0;
err:
	errno = FI_EINVAL;
	return -errno;
}

static struct fi_ops_cntr fi_opx_ops_cntr = {
	.size		= sizeof(struct fi_ops_cntr),
	.read		= fi_opx_cntr_read,
	.readerr	= fi_opx_cntr_readerr,
	.add		= fi_opx_cntr_add,
	.set		= fi_opx_cntr_set,
	.wait		= fi_opx_cntr_wait
};

int fi_opx_cntr_open(struct fid_domain *domain,
		struct fi_cntr_attr *attr,
		struct fid_cntr **cntr, void *context)
{
	int ret;
	struct fi_opx_cntr *opx_cntr;

	if (!attr) {
		FI_LOG(fi_opx_global.prov, FI_LOG_DEBUG, FI_LOG_CQ,
				"no attr supplied\n");
		errno = FI_EINVAL;
		return -errno;
	}
	ret = fi_opx_fid_check(&domain->fid, FI_CLASS_DOMAIN, "domain");
	if (ret)
		return ret;

	opx_cntr = calloc(1, sizeof(*opx_cntr));
	if (!opx_cntr) {
		errno = FI_ENOMEM;
		goto err;
	}

	opx_cntr->cntr_fid.fid.fclass		= FI_CLASS_CNTR;
	opx_cntr->cntr_fid.fid.context	= context;
	opx_cntr->cntr_fid.fid.ops		= &fi_opx_fi_ops;
	opx_cntr->cntr_fid.ops		= &fi_opx_ops_cntr;

	opx_cntr->domain = (struct fi_opx_domain *) domain;

	opx_cntr->threading = opx_cntr->domain->threading;
	opx_cntr->lock_required = fi_opx_threading_lock_required(opx_cntr->threading, fi_opx_global.progress);

	/* ---- allocate and initialize the "std" and "err" counters ---- */
	ofi_atomic_initialize64(&opx_cntr->std, 0);
	ofi_atomic_initialize64(&opx_cntr->err, 0);

	opx_cntr->ep_bind_count = 0;
	opx_cntr->progress.ep_count = 0;
	unsigned i;
	for (i=0; i<64; ++i) {			/* TODO - check this array size */
		opx_cntr->ep[i] = NULL;
		opx_cntr->progress.ep[i] = NULL;
	}

	fi_opx_ref_inc(&opx_cntr->domain->ref_cnt, "domain");

	*cntr = &opx_cntr->cntr_fid;
	return 0;
err:
	return -errno;
}
