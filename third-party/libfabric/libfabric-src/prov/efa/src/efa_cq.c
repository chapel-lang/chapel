/*
 * Copyright (c) 2013-2015 Intel Corporation, Inc.  All rights reserved.
 * Copyright (c) 2017-2019 Amazon.com, Inc. or its affiliates. All rights reserved.
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

#include <ofi_mem.h>

#include "efa.h"

static uint64_t efa_cq_wc_to_fi_flags(struct efa_wc *wc)
{
	switch (wc->ibv_wc.opcode) {
	case IBV_WC_SEND:
		return FI_SEND | FI_MSG;
	case IBV_WC_RECV:
		return FI_RECV | FI_MSG;
	default:
		assert(0);
		return 0;
	}
}

ssize_t efa_cq_readerr(struct fid_cq *cq_fid, struct fi_cq_err_entry *entry,
		       uint64_t flags)
{
	struct efa_cq *cq;
	struct efa_wce *wce;
	struct slist_entry *slist_entry;
	uint32_t api_version;

	cq = container_of(cq_fid, struct efa_cq, util_cq.cq_fid);

	fastlock_acquire(&cq->lock);
	if (slist_empty(&cq->wcq))
		goto err;

	wce = container_of(cq->wcq.head, struct efa_wce, entry);
	if (!wce->wc.ibv_wc.status)
		goto err;

	api_version = cq->domain->fab->util_fabric.fabric_fid.api_version;

	slist_entry = slist_remove_head(&cq->wcq);
	fastlock_release(&cq->lock);

	wce = container_of(slist_entry, struct efa_wce, entry);

	entry->op_context = (void *)(uintptr_t)wce->wc.ibv_wc.wr_id;
	entry->flags = efa_cq_wc_to_fi_flags(&wce->wc);
	entry->err = EIO;
	entry->prov_errno = wce->wc.ibv_wc.status;

	/* We currently don't have err_data to give back to the user. */
	if (FI_VERSION_GE(api_version, FI_VERSION(1, 5)))
		entry->err_data_size = 0;

	ofi_buf_free(wce);
	return sizeof(*entry);
err:
	fastlock_release(&cq->lock);
	return -FI_EAGAIN;
}

static void efa_cq_read_context_entry(struct efa_wc *wc, int i, void *buf)
{
	struct fi_cq_entry *entry = buf;

	entry[i].op_context = (void *)(uintptr_t)wc->ibv_wc.wr_id;
}

static void efa_cq_read_msg_entry(struct efa_wc *wc, int i, void *buf)
{
	struct fi_cq_msg_entry *entry = buf;

	entry[i].op_context = (void *)(uintptr_t)wc->ibv_wc.wr_id;
	entry[i].flags = efa_cq_wc_to_fi_flags(wc);
	entry[i].len = (uint64_t)wc->ibv_wc.byte_len;
}

static void efa_cq_read_data_entry(struct efa_wc *wc, int i, void *buf)
{
	struct fi_cq_data_entry *entry = buf;

	entry[i].op_context = (void *)(uintptr_t)wc->ibv_wc.wr_id;
	entry[i].flags = efa_cq_wc_to_fi_flags(wc);
	entry[i].data = 0;
	entry[i].len = (uint64_t)wc->ibv_wc.byte_len;
}

ssize_t efa_cq_readfrom(struct fid_cq *cq_fid, void *buf, size_t count,
			fi_addr_t *src_addr)
{
	struct efa_cq *cq;
	struct efa_wce *wce;
	struct slist_entry *entry;
	struct efa_av *av;
	struct efa_wc wc;
	ssize_t ret = 0, i;

	cq = container_of(cq_fid, struct efa_cq, util_cq.cq_fid);

	fastlock_acquire(&cq->lock);

	for (i = 0; i < count; i++) {
		if (!slist_empty(&cq->wcq)) {
			wce = container_of(cq->wcq.head, struct efa_wce, entry);
			if (wce->wc.ibv_wc.status) {
				ret = -FI_EAVAIL;
				break;
			}
			entry = slist_remove_head(&cq->wcq);
			wce = container_of(entry, struct efa_wce, entry);
			cq->read_entry(&wce->wc, i, buf);
			ofi_buf_free(wce);
			continue;
		}

		ret = ibv_poll_cq(cq->ibv_cq, 1, &wc.ibv_wc);
		if (ret != 1) {
			if (!ret)
				ret = -FI_EAGAIN;
			break;
		}

		/* Insert error entry into wcq */
		if (wc.ibv_wc.status) {
			wce = ofi_buf_alloc(cq->wce_pool);
			if (!wce) {
				fastlock_release(&cq->lock);
				return -FI_ENOMEM;
			}
			memset(wce, 0, sizeof(*wce));
			memcpy(&wce->wc, &wc, sizeof(wc));
			slist_insert_tail(&wce->entry, &cq->wcq);
			ret = -FI_EAVAIL;
			break;
		}

		if (src_addr) {
			av = cq->domain->qp_table[wc.ibv_wc.qp_num &
			     cq->domain->qp_table_sz_m1]->ep->av;

			src_addr[i] = efa_ahn_qpn_to_addr(av,
							  wc.ibv_wc.slid,
							  wc.ibv_wc.src_qp);
		}
		cq->read_entry(&wc, i, buf);
	}

	fastlock_release(&cq->lock);
	return i ? i : ret;
}

static const char *efa_cq_strerror(struct fid_cq *cq_fid,
				   int prov_errno,
				   const void *err_data,
				   char *buf, size_t len)
{
	/* XXX use vendor_error */
	return "unknown error";
}

static struct fi_ops_cq efa_cq_ops = {
	.size = sizeof(struct fi_ops_cq),
	.read = ofi_cq_read,
	.readfrom = ofi_cq_readfrom,
	.readerr = ofi_cq_readerr,
	.sread = fi_no_cq_sread,
	.sreadfrom = fi_no_cq_sreadfrom,
	.signal = fi_no_cq_signal,
	.strerror = efa_cq_strerror
};

static int efa_cq_control(fid_t fid, int command, void *arg)
{
	int ret = 0;

	switch (command) {
	default:
		ret = -FI_ENOSYS;
		break;
	}

	return ret;
}

static int efa_cq_close(fid_t fid)
{
	struct efa_cq *cq;
	struct efa_wce *wce;
	struct slist_entry *entry;
	int ret;

	cq = container_of(fid, struct efa_cq, util_cq.cq_fid.fid);

	fastlock_acquire(&cq->lock);
	while (!slist_empty(&cq->wcq)) {
		entry = slist_remove_head(&cq->wcq);
		wce = container_of(entry, struct efa_wce, entry);
		ofi_buf_free(wce);
	}
	fastlock_release(&cq->lock);

	ofi_bufpool_destroy(cq->wce_pool);

	fastlock_destroy(&cq->lock);

	ret = -ibv_destroy_cq(cq->ibv_cq);
	if (ret)
		return ret;

	ret = ofi_cq_cleanup(&cq->util_cq);
	if (ret)
		return ret;

	free(cq);

	return 0;
}

static struct fi_ops efa_cq_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = efa_cq_close,
	.bind = fi_no_bind,
	.control = efa_cq_control,
	.ops_open = fi_no_ops_open,
};

int efa_cq_open(struct fid_domain *domain_fid, struct fi_cq_attr *attr,
		struct fid_cq **cq_fid, void *context)
{
	struct efa_cq *cq;
	size_t size;
	int ret;

	if (attr->wait_obj != FI_WAIT_NONE)
		return -FI_ENOSYS;

	cq = calloc(1, sizeof(*cq));
	if (!cq)
		return -FI_ENOMEM;

	ret = ofi_cq_init(&efa_prov, domain_fid, attr, &cq->util_cq,
			  &ofi_cq_progress, context);
	if (ret) {
		EFA_WARN(FI_LOG_CQ, "Unable to create UTIL_CQ\n");
		goto err_free_cq;
	}

	cq->domain = container_of(domain_fid, struct efa_domain,
				  util_domain.domain_fid);

	size = attr->size ? attr->size : EFA_DEF_CQ_SIZE;
	cq->ibv_cq = ibv_create_cq(cq->domain->ctx->ibv_ctx, size, NULL, NULL, 0);
	if (!cq->ibv_cq) {
		EFA_WARN(FI_LOG_CQ, "Unable to create CQ\n");
		ret = -FI_EINVAL;
		goto err_free_util_cq;
	}

	ret = ofi_bufpool_create(&cq->wce_pool, sizeof(struct efa_wce), 16, 0,
				 EFA_WCE_CNT, 0);
	if (ret) {
		EFA_WARN(FI_LOG_CQ, "Failed to create wce_pool\n");
		goto err_destroy_cq;
	}

	switch (attr->format) {
	case FI_CQ_FORMAT_UNSPEC:
	case FI_CQ_FORMAT_CONTEXT:
		cq->read_entry = efa_cq_read_context_entry;
		cq->entry_size = sizeof(struct fi_cq_entry);
		break;
	case FI_CQ_FORMAT_MSG:
		cq->read_entry = efa_cq_read_msg_entry;
		cq->entry_size = sizeof(struct fi_cq_msg_entry);
		break;
	case FI_CQ_FORMAT_DATA:
		cq->read_entry = efa_cq_read_data_entry;
		cq->entry_size = sizeof(struct fi_cq_data_entry);
		break;
	case FI_CQ_FORMAT_TAGGED:
	default:
		ret = -FI_ENOSYS;
		goto err_destroy_pool;
	}

	fastlock_init(&cq->lock);

	slist_init(&cq->wcq);

	*cq_fid = &cq->util_cq.cq_fid;
	(*cq_fid)->fid.fclass = FI_CLASS_CQ;
	(*cq_fid)->fid.context = context;
	(*cq_fid)->fid.ops = &efa_cq_fi_ops;
	(*cq_fid)->ops = &efa_cq_ops;

	return 0;

err_destroy_pool:
	ofi_bufpool_destroy(cq->wce_pool);
err_destroy_cq:
	ibv_destroy_cq(cq->ibv_cq);
err_free_util_cq:
	ofi_cq_cleanup(&cq->util_cq);
err_free_cq:
	free(cq);
	return ret;
}

