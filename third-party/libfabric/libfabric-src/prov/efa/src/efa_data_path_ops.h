/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */

/*
 * EFA Data Path Operations
 *
 * This file contains wrapper functions for EFA device operations that are used
 * in the data transfer path. These operations provide a unified interface for
 * both regular IBV operations and direct CQ operations, allowing the EFA provider
 * to seamlessly switch between different hardware acceleration modes based on
 * device capabilities and configuration.
 *
 * The wrapper functions handle:
 * - Queue Pair (QP) operations: post_recv, work request operations (send, RDMA read/write)
 * - Completion Queue (CQ) operations: polling, reading completion data
 * - Automatic selection between IBV and direct CQ implementations
 */

#ifndef EFA_DATA_PATH_OPS_H
#define EFA_DATA_PATH_OPS_H

#include <infiniband/verbs.h>
#include <infiniband/efadv.h>

/* Forward declarations to avoid cyclic dependencies */
#include "efa_base_ep.h"
#include "efa_cq.h"

#if HAVE_EFA_DATA_PATH_DIRECT
#include "efa_data_path_direct_entry.h"
#endif

#if EFA_UNIT_TEST
/* For unit tests, declare functions that are defined in efa_unit_test_data_path_ops.c */
int efa_qp_post_recv(struct efa_qp *qp, struct ibv_recv_wr *wr, struct ibv_recv_wr **bad);
int efa_qp_post_send(struct efa_qp *qp, const struct ibv_sge *sge_list,
		      const struct ibv_data_buf *inline_data_list,
		      size_t iov_count, bool use_inline, uintptr_t wr_id,
		      uint64_t data, uint64_t flags, struct efa_ah *ah,
		      uint32_t qpn, uint32_t qkey);
int efa_qp_post_read(struct efa_qp *qp, const struct ibv_sge *sge_list,
		      size_t sge_count, uint32_t remote_key,
		      uint64_t remote_addr, uintptr_t wr_id, uint64_t flags,
		      struct efa_ah *ah, uint32_t qpn, uint32_t qkey);
int efa_qp_post_write(struct efa_qp *qp, const struct ibv_sge *sge_list,
		       size_t sge_count, uint32_t remote_key,
		       uint64_t remote_addr, uintptr_t wr_id, uint64_t data,
		       uint64_t flags, struct efa_ah *ah, uint32_t qpn,
		       uint32_t qkey);
int efa_ibv_cq_start_poll(struct efa_ibv_cq *ibv_cq, struct ibv_poll_cq_attr *attr);
int efa_ibv_cq_next_poll(struct efa_ibv_cq *ibv_cq);
enum ibv_wc_opcode efa_ibv_cq_wc_read_opcode(struct efa_ibv_cq *ibv_cq);
void efa_ibv_cq_end_poll(struct efa_ibv_cq *ibv_cq);
uint32_t efa_ibv_cq_wc_read_qp_num(struct efa_ibv_cq *ibv_cq);
uint32_t efa_ibv_cq_wc_read_vendor_err(struct efa_ibv_cq *ibv_cq);
uint32_t efa_ibv_cq_wc_read_src_qp(struct efa_ibv_cq *ibv_cq);
uint32_t efa_ibv_cq_wc_read_slid(struct efa_ibv_cq *ibv_cq);
uint32_t efa_ibv_cq_wc_read_byte_len(struct efa_ibv_cq *ibv_cq);
unsigned int efa_ibv_cq_wc_read_wc_flags(struct efa_ibv_cq *ibv_cq);
__be32 efa_ibv_cq_wc_read_imm_data(struct efa_ibv_cq *ibv_cq);
bool efa_ibv_cq_wc_is_unsolicited(struct efa_ibv_cq *ibv_cq);

int efa_ibv_cq_wc_read_sgid(struct efa_ibv_cq *ibv_cq, union ibv_gid *sgid);

int efa_ibv_get_cq_event(struct efa_ibv_cq *ibv_cq, void **cq_context);
int efa_ibv_req_notify_cq(struct efa_ibv_cq *ibv_cq, int solicited_only);

#else
/* For production, define static inline functions */

/* QP wrapper functions */
static inline int efa_qp_post_recv(struct efa_qp *qp, struct ibv_recv_wr *wr, struct ibv_recv_wr **bad)
{
#if HAVE_EFA_DATA_PATH_DIRECT
	if (qp->data_path_direct_enabled)
		return efa_data_path_direct_post_recv(qp, wr, bad);
#endif
	return ibv_post_recv(qp->ibv_qp, wr, bad);
}

/**
 * @brief RDMA-core version of send operation using ibv_* APIs
 */
static inline int
efa_ibv_post_send(
		struct efa_qp *qp,
		const struct ibv_sge *sge_list,
		const struct ibv_data_buf *inline_data_list,
		size_t data_count,
		bool use_inline,
		uintptr_t wr_id,
		uint64_t data,
		uint64_t flags,
		struct efa_ah *ah,
		uint32_t qpn,
		uint32_t qkey)
{
	struct efa_base_ep *base_ep = qp->base_ep;
	int ret;

	if (!base_ep->is_wr_started) {
		ibv_wr_start(qp->ibv_qp_ex);
		base_ep->is_wr_started = true;
	}

	qp->ibv_qp_ex->wr_id = wr_id;

	if (flags & FI_REMOTE_CQ_DATA) {
		ibv_wr_send_imm(qp->ibv_qp_ex, data);
	} else {
		ibv_wr_send(qp->ibv_qp_ex);
	}

	if (use_inline) {
		ibv_wr_set_inline_data_list(qp->ibv_qp_ex, data_count, inline_data_list);
	} else {
		ibv_wr_set_sge_list(qp->ibv_qp_ex, data_count, sge_list);
	}

	ibv_wr_set_ud_addr(qp->ibv_qp_ex, ah->ibv_ah, qpn, qkey);

	if (!(flags & FI_MORE)) {
		ret = ibv_wr_complete(qp->ibv_qp_ex);
		base_ep->is_wr_started = false;
		return ret;
	}

	return 0;
}

/**
 * @brief RDMA-core version of RDMA read operation using ibv_* APIs
 */
static inline int
efa_ibv_post_read(
		struct efa_qp *qp,
		const struct ibv_sge *sge_list,
		size_t sge_count,
		uint32_t remote_key,
		uint64_t remote_addr,
		uintptr_t wr_id,
		uint64_t flags,
		struct efa_ah *ah,
		uint32_t qpn,
		uint32_t qkey)
{
	struct efa_base_ep *base_ep = qp->base_ep;
	int ret;

	if (!base_ep->is_wr_started) {
		ibv_wr_start(qp->ibv_qp_ex);
		base_ep->is_wr_started = true;
	}

	qp->ibv_qp_ex->wr_id = wr_id;
	ibv_wr_rdma_read(qp->ibv_qp_ex, remote_key, remote_addr);
	ibv_wr_set_sge_list(qp->ibv_qp_ex, sge_count, sge_list);
	ibv_wr_set_ud_addr(qp->ibv_qp_ex, ah->ibv_ah, qpn, qkey);

	if (!(flags & FI_MORE)) {
		ret = ibv_wr_complete(qp->ibv_qp_ex);
		base_ep->is_wr_started = false;
		return ret;
	}

	return 0;
}

/**
 * @brief RDMA-core version of RDMA write operation using ibv_* APIs
 */
static inline int
efa_ibv_post_write(
		struct efa_qp *qp,
		const struct ibv_sge *sge_list,
		size_t sge_count,
		uint32_t remote_key,
		uint64_t remote_addr,
		uintptr_t wr_id,
		uint64_t data,
		uint64_t flags,
		struct efa_ah *ah,
		uint32_t qpn,
		uint32_t qkey)
{
	struct efa_base_ep *base_ep = qp->base_ep;
	int ret;

	if (!base_ep->is_wr_started) {
		ibv_wr_start(qp->ibv_qp_ex);
		base_ep->is_wr_started = true;
	}

	qp->ibv_qp_ex->wr_id = wr_id;

	if (flags & FI_REMOTE_CQ_DATA) {
		ibv_wr_rdma_write_imm(qp->ibv_qp_ex, remote_key, remote_addr, data);
	} else {
		ibv_wr_rdma_write(qp->ibv_qp_ex, remote_key, remote_addr);
	}

	ibv_wr_set_sge_list(qp->ibv_qp_ex, sge_count, sge_list);
	ibv_wr_set_ud_addr(qp->ibv_qp_ex, ah->ibv_ah, qpn, qkey);

	if (!(flags & FI_MORE)) {
		ret = ibv_wr_complete(qp->ibv_qp_ex);
		base_ep->is_wr_started = false;
		return ret;
	}

	return 0;
}

/**
 * @brief Wrapper for send operations - chooses between direct and IBV paths
 */
static inline int
efa_qp_post_send(struct efa_qp *qp,
                 const struct ibv_sge *sge_list,
                 const struct ibv_data_buf *inline_data_list,
                 size_t data_count,
                 bool use_inline,
                 uintptr_t wr_id,
                 uint64_t data,
                 uint64_t flags,
                 struct efa_ah *ah,
                 uint32_t qpn,
                 uint32_t qkey)
{
	EFA_DBG(FI_LOG_EP_DATA, "Posting WQE: qp=%p data_count=%ld use_inline=%d wr_id=0x%lx data=0x%lx flags=0x%lx qpn=%u qkey=0x%x\n",
		qp, data_count, use_inline, wr_id, data, flags, qpn, qkey);
#if HAVE_EFA_DATA_PATH_DIRECT
	if (qp->data_path_direct_enabled)
		return efa_data_path_direct_post_send(qp, sge_list, inline_data_list, data_count,
					   use_inline, wr_id, data, flags, ah, qpn, qkey);
#endif
	return efa_ibv_post_send(qp, sge_list, inline_data_list, data_count,
				use_inline, wr_id, data, flags, ah, qpn, qkey);
}

/**
 * @brief Wrapper for RDMA read operations - chooses between direct and IBV paths
 */
static inline int
efa_qp_post_read(struct efa_qp *qp,
                 const struct ibv_sge *sge_list,
                 size_t sge_count,
                 uint32_t remote_key,
                 uint64_t remote_addr,
                 uintptr_t wr_id,
                 uint64_t flags,
                 struct efa_ah *ah,
                 uint32_t qpn,
                 uint32_t qkey)
{
	EFA_DBG(FI_LOG_EP_DATA, "Posting WQE: qp=%p sge_count=%ld remote_key=%u remote_addr=0x%lx wr_id=0x%lx flags=0x%lx qpn=%u qkey=0x%x\n",
		qp, sge_count, remote_key, remote_addr, wr_id, flags, qpn, qkey);
#if HAVE_EFA_DATA_PATH_DIRECT
	if (qp->data_path_direct_enabled)
		return efa_data_path_direct_post_read(qp, sge_list, sge_count,
						remote_key, remote_addr, wr_id, flags, ah, qpn, qkey);
#endif
	return efa_ibv_post_read(qp, sge_list, sge_count,
				 remote_key, remote_addr, wr_id, flags, ah, qpn, qkey);
}

/**
 * @brief Wrapper for RDMA write operations - chooses between direct and IBV paths
 */
static inline int
efa_qp_post_write(struct efa_qp *qp,
                  const struct ibv_sge *sge_list,
                  size_t sge_count,
                  uint32_t remote_key,
                  uint64_t remote_addr,
                  uintptr_t wr_id,
                  uint64_t data,
                  uint64_t flags,
                  struct efa_ah *ah,
                  uint32_t qpn,
                  uint32_t qkey)
{
	EFA_DBG(FI_LOG_EP_DATA, "Posting WQE: qp=%p sge_count=%ld remote_key=%u remote_addr=0x%lx wr_id=0x%lx data=0x%lx flags=0x%lx qpn=%u qkey=0x%x\n",
		qp, sge_count, remote_key, remote_addr, wr_id, data, flags, qpn, qkey);
#if HAVE_EFA_DATA_PATH_DIRECT
	if (qp->data_path_direct_enabled)
		return efa_data_path_direct_post_write(qp, sge_list, sge_count,
					 remote_key, remote_addr, wr_id, data, flags, ah, qpn, qkey);
#endif
	return efa_ibv_post_write(qp, sge_list, sge_count,
				  remote_key, remote_addr, wr_id, data, flags, ah, qpn, qkey);
}

/* CQ wrapper functions */
static inline int efa_ibv_cq_start_poll(struct efa_ibv_cq *ibv_cq, struct ibv_poll_cq_attr *attr)
{
#if HAVE_EFA_DATA_PATH_DIRECT
	if (ibv_cq->data_path_direct_enabled)
		return efa_data_path_direct_start_poll(ibv_cq, attr);
#endif
	return ibv_start_poll(ibv_cq->ibv_cq_ex, attr);
}

static inline int efa_ibv_cq_next_poll(struct efa_ibv_cq *ibv_cq)
{
#if HAVE_EFA_DATA_PATH_DIRECT
	if (ibv_cq->data_path_direct_enabled)
		return efa_data_path_direct_next_poll(ibv_cq);
#endif
	return ibv_next_poll(ibv_cq->ibv_cq_ex);
}

static inline enum ibv_wc_opcode efa_ibv_cq_wc_read_opcode(struct efa_ibv_cq *ibv_cq)
{
#if HAVE_EFA_DATA_PATH_DIRECT
	if (ibv_cq->data_path_direct_enabled)
		return efa_data_path_direct_wc_read_opcode(ibv_cq);
#endif
	return ibv_wc_read_opcode(ibv_cq->ibv_cq_ex);
}

static inline void efa_ibv_cq_end_poll(struct efa_ibv_cq *ibv_cq)
{
#if HAVE_EFA_DATA_PATH_DIRECT
	if (ibv_cq->data_path_direct_enabled) {
		efa_data_path_direct_end_poll(ibv_cq);
		return;
	}
#endif
	ibv_end_poll(ibv_cq->ibv_cq_ex);
}

static inline uint32_t efa_ibv_cq_wc_read_qp_num(struct efa_ibv_cq *ibv_cq)
{
#if HAVE_EFA_DATA_PATH_DIRECT
	if (ibv_cq->data_path_direct_enabled)
		return efa_data_path_direct_wc_read_qp_num(ibv_cq);
#endif
	return ibv_wc_read_qp_num(ibv_cq->ibv_cq_ex);
}

static inline uint32_t efa_ibv_cq_wc_read_vendor_err(struct efa_ibv_cq *ibv_cq)
{
#if HAVE_EFA_DATA_PATH_DIRECT
	if (ibv_cq->data_path_direct_enabled)
		return efa_data_path_direct_wc_read_vendor_err(ibv_cq);
#endif
	return ibv_wc_read_vendor_err(ibv_cq->ibv_cq_ex);
}

static inline uint32_t efa_ibv_cq_wc_read_src_qp(struct efa_ibv_cq *ibv_cq)
{
#if HAVE_EFA_DATA_PATH_DIRECT
	if (ibv_cq->data_path_direct_enabled)
		return efa_data_path_direct_wc_read_src_qp(ibv_cq);
#endif
	return ibv_wc_read_src_qp(ibv_cq->ibv_cq_ex);
}

static inline uint32_t efa_ibv_cq_wc_read_slid(struct efa_ibv_cq *ibv_cq)
{
#if HAVE_EFA_DATA_PATH_DIRECT
	if (ibv_cq->data_path_direct_enabled)
		return efa_data_path_direct_wc_read_slid(ibv_cq);
#endif
	return ibv_wc_read_slid(ibv_cq->ibv_cq_ex);
}

static inline uint32_t efa_ibv_cq_wc_read_byte_len(struct efa_ibv_cq *ibv_cq)
{
#if HAVE_EFA_DATA_PATH_DIRECT
	if (ibv_cq->data_path_direct_enabled)
		return efa_data_path_direct_wc_read_byte_len(ibv_cq);
#endif
	return ibv_wc_read_byte_len(ibv_cq->ibv_cq_ex);
}

static inline unsigned int efa_ibv_cq_wc_read_wc_flags(struct efa_ibv_cq *ibv_cq)
{
#if HAVE_EFA_DATA_PATH_DIRECT
	if (ibv_cq->data_path_direct_enabled)
		return efa_data_path_direct_wc_read_wc_flags(ibv_cq);
#endif
	return ibv_wc_read_wc_flags(ibv_cq->ibv_cq_ex);
}

static inline __be32 efa_ibv_cq_wc_read_imm_data(struct efa_ibv_cq *ibv_cq)
{
#if HAVE_EFA_DATA_PATH_DIRECT
	if (ibv_cq->data_path_direct_enabled)
		return efa_data_path_direct_wc_read_imm_data(ibv_cq);
#endif
	return ibv_wc_read_imm_data(ibv_cq->ibv_cq_ex);
}


static inline bool efa_ibv_cq_wc_is_unsolicited(struct efa_ibv_cq *ibv_cq)
{
#if HAVE_EFA_DATA_PATH_DIRECT
	if (ibv_cq->data_path_direct_enabled)
		return efa_data_path_direct_wc_is_unsolicited(ibv_cq);
#endif
#if HAVE_CAPS_UNSOLICITED_WRITE_RECV
	return efadv_wc_is_unsolicited(efadv_cq_from_ibv_cq_ex(ibv_cq->ibv_cq_ex));
#else
	return false;
#endif
}

static inline int efa_ibv_cq_wc_read_sgid(struct efa_ibv_cq *ibv_cq, union ibv_gid *sgid)
{
#if HAVE_EFA_DATA_PATH_DIRECT
	if (ibv_cq->data_path_direct_enabled)
		return efa_data_path_direct_wc_read_sgid(ibv_cq, sgid);
#endif

#if HAVE_EFADV_CQ_EX
	return efadv_wc_read_sgid(efadv_cq_from_ibv_cq_ex(ibv_cq->ibv_cq_ex), sgid);
#else
	return false;
#endif
}

static inline int efa_ibv_get_cq_event(struct efa_ibv_cq *ibv_cq, void **cq_context)
{
	struct ibv_cq *cq = ibv_cq_ex_to_cq(ibv_cq->ibv_cq_ex);
#if HAVE_EFA_DATA_PATH_DIRECT && HAVE_EFADV_CQ_ATTR_DB
	if (ibv_cq->data_path_direct_enabled)
		return efa_data_path_direct_get_cq_event(ibv_cq, &cq, cq_context);
#endif
#if HAVE_EFA_CQ_NOTIFICATION
	return ibv_get_cq_event(ibv_cq->channel, &cq, cq_context);
#else
	return -FI_ENOSYS;
#endif
}

static inline int efa_ibv_req_notify_cq(struct efa_ibv_cq *ibv_cq, int solicited_only)
{
#if HAVE_EFA_DATA_PATH_DIRECT && HAVE_EFADV_CQ_ATTR_DB
	if (ibv_cq->data_path_direct_enabled)
		return efa_data_path_direct_req_notify_cq(ibv_cq, solicited_only);
#endif
#if HAVE_EFA_CQ_NOTIFICATION
	return ibv_req_notify_cq(ibv_cq_ex_to_cq(ibv_cq->ibv_cq_ex), solicited_only);
#else
	return -FI_ENOSYS;
#endif
}


#endif /* EFA_UNIT_TEST */

/**
 * @brief Check whether a completion consumes recv buffer
 *
 * @param ibv_cq efa ibv cq
 * @return true the wc consumes a recv buffer
 * @return false the wc doesn't consume a recv buffer
 */
static inline bool efa_cq_wc_is_unsolicited(struct efa_ibv_cq *ibv_cq)
{
	return ibv_cq->unsolicited_write_recv_enabled && efa_ibv_cq_wc_is_unsolicited(ibv_cq);
}

static inline bool efa_cq_wc_available(struct efa_ibv_cq *cq)
{
	return cq->poll_active && !cq->poll_err;
}

static inline void efa_cq_report_poll_err(struct efa_ibv_cq *cq)
{
	int err = cq->poll_err;

	if (err && err != ENOENT)
		EFA_INFO(FI_LOG_CQ, "Ignoring CQ entries from destroyed queue pair\n");
}

static inline void efa_cq_start_poll(struct efa_ibv_cq *cq)
{
	/**
	 * It is possible that the last efa_cq_readfrom
	 * is leaving the device cq in a poll active status
	 * when polling a failed cqe and leave it for the efa_cq_readfrom, efa_cq_readerr
	 * or efa_cq_poll_ibv_cq to consume it. And efa_cq_poll_ibv_cq
	 * will call this wrapper at the beginning.
	 * We shouldn't start poll in this stuation as it will make the
	 * cqe index shifted and the entry lost.
	 */
	if (cq->poll_active)
		return;

	/* Pass an empty ibv_poll_cq_attr struct (zero-initialized) for
	 * ibv_start_poll. EFA expects .comp_mask = 0, or otherwise returns EINVAL.
	 */
	cq->poll_err = efa_ibv_cq_start_poll(cq, &(struct ibv_poll_cq_attr){0});
	if (!cq->poll_err) {
		cq->poll_active = true;
		EFA_DBG(FI_LOG_CQ, "Polled CQE: wr_id 0x%lx\n", cq->ibv_cq_ex->wr_id);
	} else {
		efa_cq_report_poll_err(cq);
	}
}

static inline void efa_cq_next_poll(struct efa_ibv_cq *cq)
{
	assert(cq->poll_active);
	cq->poll_err = efa_ibv_cq_next_poll(cq);
	if (cq->poll_err) {
		efa_cq_report_poll_err(cq);
		return;
	}
	EFA_DBG(FI_LOG_CQ, "Polled CQE: wr_id 0x%lx\n", cq->ibv_cq_ex->wr_id);
}

static inline void efa_cq_end_poll(struct efa_ibv_cq *cq)
{
	if (cq->poll_active)
		efa_ibv_cq_end_poll(cq);
	cq->poll_active = false;
	cq->poll_err = 0;
}

static inline struct efa_base_ep *efa_ibv_cq_get_base_ep_from_cur_cqe(struct efa_ibv_cq *cq, struct efa_domain *efa_domain)
{
	struct efa_qp *qp = efa_domain->device->qp_table[efa_ibv_cq_wc_read_qp_num(cq) & efa_domain->device->qp_table_sz_m1];

	return qp ? qp->base_ep : NULL;
}

#endif /* EFA_DATA_PATH_OPS_H */
