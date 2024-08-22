/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright (c) 2013-2015 Intel Corporation, Inc.  All rights reserved. */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */

#include <errno.h>
#include <string.h>
#include "config.h"
#include <ofi_mem.h>
#include "dgram/efa_dgram_ep.h"
#include "efa.h"
#include "efa_cq.h"
#include "efa_av.h"
#include "efa_dgram_cq.h"
#include <infiniband/verbs.h>

struct efa_wc {
	struct ibv_wc		ibv_wc;
	/* Source address */
	uint16_t		efa_ah;
};

struct efa_wce {
	struct slist_entry	entry;
	struct efa_wc		wc;
};

#define EFA_WCE_CNT 1024

static inline uint64_t efa_dgram_cq_opcode_to_fi_flags(enum ibv_wc_opcode	opcode) {
	switch (opcode) {
	case IBV_WC_SEND:
		return FI_SEND | FI_MSG;
	case IBV_WC_RECV:
		return FI_RECV | FI_MSG;
	default:
		assert(0);
		return 0;
	}
}

static inline uint32_t efa_dgram_cq_api_version(struct efa_dgram_cq *cq) {
	return cq->domain->fabric->util_fabric.fabric_fid.api_version;
}

ssize_t efa_dgram_cq_readerr(struct fid_cq *cq_fid, struct fi_cq_err_entry *entry,
		       uint64_t flags)
{
	struct efa_dgram_cq *cq;
	uint32_t api_version;

	cq = container_of(cq_fid, struct efa_dgram_cq, util_cq.cq_fid);

	ofi_spin_lock(&cq->lock);

	if (!cq->ibv_cq_ex->status)
		goto err;

	api_version = efa_dgram_cq_api_version(cq);

	entry->op_context = (void *)(uintptr_t)cq->ibv_cq_ex->wr_id;
	entry->flags = efa_dgram_cq_opcode_to_fi_flags(ibv_wc_read_opcode(cq->ibv_cq_ex));
	entry->err = FI_EIO;
	entry->prov_errno = ibv_wc_read_vendor_err(cq->ibv_cq_ex);
	EFA_WARN(FI_LOG_CQ, "Work completion status: %s\n", efa_strerror(entry->prov_errno));

	ofi_spin_unlock(&cq->lock);

	/* We currently don't have err_data to give back to the user. */
	if (FI_VERSION_GE(api_version, FI_VERSION(1, 5)))
		entry->err_data_size = 0;

	return sizeof(*entry);
err:
	ofi_spin_unlock(&cq->lock);
	return -FI_EAGAIN;
}

static void efa_dgram_cq_read_context_entry(struct ibv_cq_ex *ibv_cqx, int i, void *buf)
{
	struct fi_cq_entry *entry = buf;

	entry[i].op_context = (void *)ibv_cqx->wr_id;
}

static void efa_dgram_cq_read_msg_entry(struct ibv_cq_ex *ibv_cqx, int i, void *buf)
{
	struct fi_cq_msg_entry *entry = buf;

	entry[i].op_context = (void *)(uintptr_t)ibv_cqx->wr_id;
	entry[i].flags = efa_dgram_cq_opcode_to_fi_flags(ibv_wc_read_opcode(ibv_cqx));
	entry[i].len = ibv_wc_read_byte_len(ibv_cqx);
}

static void efa_dgram_cq_read_data_entry(struct ibv_cq_ex *ibv_cqx, int i, void *buf)
{
	struct fi_cq_data_entry *entry = buf;

	entry[i].op_context = (void *)ibv_cqx->wr_id;
	entry[i].flags = efa_dgram_cq_opcode_to_fi_flags(ibv_wc_read_opcode(ibv_cqx));
	entry[i].data = 0;
	entry[i].len = ibv_wc_read_byte_len(ibv_cqx);
}

/**
 * @brief Convert an error code from CQ poll API, e.g. `ibv_start_poll`, `ibv_end_poll`.
 * The returned error code must be 0 (success) or negative (error).
 * As a special case, if input error code is ENOENT (there was no item on CQ), we should return -FI_EAGAIN.
 * @param[in] err	Return value from `ibv_start_poll` or `ibv_end_poll`
 * @returns	Converted error code
 */
static inline ssize_t efa_dgram_cq_ibv_poll_error_to_fi_error(ssize_t err) {
	if (err == ENOENT) {
		return -FI_EAGAIN;
	}

	if (err > 0) {
		return -err;
	}

	return err;
}

ssize_t efa_dgram_cq_readfrom(struct fid_cq *cq_fid, void *buf, size_t count,
			fi_addr_t *src_addr)
{
	bool should_end_poll = false;
	struct efa_dgram_cq *cq;
	struct efa_av *av;
	ssize_t err = 0;
	size_t num_cqe = 0; /* Count of read entries */
	uint32_t qp_num, src_qp, slid;

	/* Initialize an empty ibv_poll_cq_attr struct for ibv_start_poll.
	 * EFA expects .comp_mask = 0, or otherwise returns EINVAL.
	 */
	struct ibv_poll_cq_attr poll_cq_attr = {.comp_mask = 0};

	cq = container_of(cq_fid, struct efa_dgram_cq, util_cq.cq_fid);

	ofi_spin_lock(&cq->lock);

	/* Call ibv_start_poll only once regardless of count == 0 */
	err = ibv_start_poll(cq->ibv_cq_ex, &poll_cq_attr);
	should_end_poll = !err;

	while (!err && num_cqe < count) {
		if (cq->ibv_cq_ex->status) {
			err = -FI_EAVAIL;
			break;
		}

		if (src_addr) {
			qp_num = ibv_wc_read_qp_num(cq->ibv_cq_ex);
			src_qp = ibv_wc_read_src_qp(cq->ibv_cq_ex);
			slid = ibv_wc_read_slid(cq->ibv_cq_ex);
			av = cq->domain->qp_table[qp_num & cq->domain->qp_table_sz_m1]->base_ep->av;

			src_addr[num_cqe] = efa_av_reverse_lookup_dgram(av, slid, src_qp);
		}

		cq->read_entry(cq->ibv_cq_ex, num_cqe, buf);
		num_cqe++;

		err = ibv_next_poll(cq->ibv_cq_ex);
	}

	err = efa_dgram_cq_ibv_poll_error_to_fi_error(err);

	if (should_end_poll)
		ibv_end_poll(cq->ibv_cq_ex);

	ofi_spin_unlock(&cq->lock);

	return num_cqe ? num_cqe : err;
}

static const char *efa_dgram_cq_strerror(struct fid_cq *cq_fid,
				   int prov_errno,
				   const void *err_data,
				   char *buf, size_t len)
{
	return err_data
		? (const char *) err_data
		: efa_strerror(prov_errno);
}

static struct fi_ops_cq efa_dgram_cq_ops = {
	.size = sizeof(struct fi_ops_cq),
	.read = ofi_cq_read,
	.readfrom = ofi_cq_readfrom,
	.readerr = ofi_cq_readerr,
	.sread = fi_no_cq_sread,
	.sreadfrom = fi_no_cq_sreadfrom,
	.signal = fi_no_cq_signal,
	.strerror = efa_dgram_cq_strerror
};

static int efa_dgram_cq_control(fid_t fid, int command, void *arg)
{
	int ret = 0;

	switch (command) {
	default:
		ret = -FI_ENOSYS;
		break;
	}

	return ret;
}

static int efa_dgram_cq_close(fid_t fid)
{
	struct efa_dgram_cq *cq;
	int ret;

	cq = container_of(fid, struct efa_dgram_cq, util_cq.cq_fid.fid);

	ofi_bufpool_destroy(cq->wce_pool);

	ofi_spin_destroy(&cq->lock);

	ret = -ibv_destroy_cq(ibv_cq_ex_to_cq(cq->ibv_cq_ex));
	if (ret)
		return ret;

	ret = ofi_cq_cleanup(&cq->util_cq);
	if (ret)
		return ret;

	free(cq);

	return 0;
}

static struct fi_ops efa_dgram_cq_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = efa_dgram_cq_close,
	.bind = fi_no_bind,
	.control = efa_dgram_cq_control,
	.ops_open = fi_no_ops_open,
};

/**
 * @brief Create and set cq->ibv_cq_ex
 *
 * @param[in] cq Pointer to the efa_dgram_cq. cq->ibv_cq_ex must be NULL.
 * @param[in] attr Pointer to fi_cq_attr.
 * @param[out] Return code = 0 if successful, or negative otherwise.
 */
static inline int efa_dgram_cq_set_ibv_cq_ex(struct efa_dgram_cq *cq, struct fi_cq_attr *attr)
{
	enum ibv_cq_ex_type ibv_cq_ex_type;

	if (cq->ibv_cq_ex) {
		EFA_WARN(FI_LOG_CQ, "CQ already has attached ibv_cq_ex\n");
		return -FI_EALREADY;
	}

	return efa_cq_ibv_cq_ex_open(attr, cq->domain->device->ibv_ctx,
				    &cq->ibv_cq_ex, &ibv_cq_ex_type);
}

int efa_dgram_cq_open(struct fid_domain *domain_fid, struct fi_cq_attr *attr,
		struct fid_cq **cq_fid, void *context)
{
	struct efa_dgram_cq *cq;
	int err;

	if (attr->wait_obj != FI_WAIT_NONE)
		return -FI_ENOSYS;

	cq = calloc(1, sizeof(*cq));
	if (!cq)
		return -FI_ENOMEM;

	err = ofi_cq_init(&efa_prov, domain_fid, attr, &cq->util_cq,
			  &ofi_cq_progress, context);
	if (err) {
		EFA_WARN(FI_LOG_CQ, "Unable to create UTIL_CQ\n");
		goto err_free_cq;
	}

	cq->domain = container_of(domain_fid, struct efa_domain,
				  util_domain.domain_fid);

	err = efa_dgram_cq_set_ibv_cq_ex(cq, attr);
	if (err) {
		EFA_WARN(FI_LOG_CQ, "Unable to create extended CQ\n");
		err = -FI_EINVAL;
		goto err_free_util_cq;
	}

	err = ofi_bufpool_create(&cq->wce_pool, sizeof(struct efa_wce), 16, 0,
				 EFA_WCE_CNT, 0);
	if (err) {
		EFA_WARN(FI_LOG_CQ, "Failed to create wce_pool\n");
		goto err_destroy_cq;
	}

	switch (attr->format) {
	case FI_CQ_FORMAT_UNSPEC:
	case FI_CQ_FORMAT_CONTEXT:
		cq->read_entry = efa_dgram_cq_read_context_entry;
		cq->entry_size = sizeof(struct fi_cq_entry);
		break;
	case FI_CQ_FORMAT_MSG:
		cq->read_entry = efa_dgram_cq_read_msg_entry;
		cq->entry_size = sizeof(struct fi_cq_msg_entry);
		break;
	case FI_CQ_FORMAT_DATA:
		cq->read_entry = efa_dgram_cq_read_data_entry;
		cq->entry_size = sizeof(struct fi_cq_data_entry);
		break;
	case FI_CQ_FORMAT_TAGGED:
	default:
		err = -FI_ENOSYS;
		goto err_destroy_pool;
	}

	ofi_spin_init(&cq->lock);

	*cq_fid = &cq->util_cq.cq_fid;
	(*cq_fid)->fid.fclass = FI_CLASS_CQ;
	(*cq_fid)->fid.context = context;
	(*cq_fid)->fid.ops = &efa_dgram_cq_fi_ops;
	(*cq_fid)->ops = &efa_dgram_cq_ops;

	return 0;

err_destroy_pool:
	ofi_bufpool_destroy(cq->wce_pool);
err_destroy_cq:
	ibv_destroy_cq(ibv_cq_ex_to_cq(cq->ibv_cq_ex));
err_free_util_cq:
	ofi_cq_cleanup(&cq->util_cq);
err_free_cq:
	free(cq);
	return err;
}
