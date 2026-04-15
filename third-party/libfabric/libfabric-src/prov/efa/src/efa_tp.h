/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */

#ifndef _EFA_TP_H
#define _EFA_TP_H

#include <config.h>

#if HAVE_LTTNG

#include "efa_tp_def.h"
#include "rdm/efa_rdm_ope.h"

#include <lttng/tracef.h>
#include <lttng/tracelog.h>

#define efa_tracepoint(...)	lttng_ust_tracepoint(EFA_TP_PROV, __VA_ARGS__)

/*
 * Simple printf()-style tracepoints
 * Tracing events will be labeled `lttng_ust_tracef:*`
 */
#define efa_tracef	lttng_ust_tracef

/* tracelog() is similar to tracef(), but with a log level param */
#define efa_tracelog	lttng_ust_tracelog

static inline void efa_rdm_tracepoint_wr_id_post_send(const void *wr_id)
{
	struct efa_rdm_pke *pkt_entry = (struct efa_rdm_pke *) wr_id;
	struct efa_rdm_ope *ope = pkt_entry->ope;
	if (!ope)
		return;
	efa_tracepoint(post_send, (size_t) wr_id, (size_t) ope->cq_entry.op_context);
}

static inline void efa_rdm_tracepoint_wr_id_post_recv(const void *wr_id)
{
	struct efa_rdm_pke *pkt_entry = (struct efa_rdm_pke *) wr_id;
	struct efa_rdm_ope *ope = pkt_entry->ope;
	if (!ope)
		return;
	efa_tracepoint(post_recv, (size_t) wr_id, (size_t) ope->cq_entry.op_context);
}

static inline void efa_rdm_tracepoint_wr_id_post_read(const void *wr_id)
{
	struct efa_rdm_pke *pkt_entry = (struct efa_rdm_pke *) wr_id;
	struct efa_rdm_ope *ope = pkt_entry->ope;
	if (!ope)
		return;
	efa_tracepoint(post_read, (size_t) wr_id, (size_t) ope->cq_entry.op_context);
}

static inline void efa_rdm_tracepoint_wr_id_post_write(const void *wr_id)
{
	struct efa_rdm_pke *pkt_entry = (struct efa_rdm_pke *) wr_id;
	struct efa_rdm_ope *ope = pkt_entry->ope;
	if (!ope)
		return;
	efa_tracepoint(post_write, (size_t) wr_id, (size_t) ope->cq_entry.op_context);
}

#if HAVE_EFA_DATA_PATH_DIRECT

static inline void efa_data_path_direct_tracepoint_post_send(
		const struct efa_qp *qp,
		const struct efa_data_path_direct_sq *sq,
		const struct efa_io_tx_meta_desc *meta)
{
	efa_tracepoint(data_path_direct_post_send,
		       qp->base_ep->domain->device->ibv_ctx->device->name,
		       sq->wq.wrid[meta->req_id],
		       EFA_GET(&meta->ctrl1, EFA_IO_TX_META_DESC_OP_TYPE),
		       qp->ibv_qp->qp_num,
		       meta->dest_qp_num,
		       meta->ah,
		       meta->length);
}

static inline void efa_data_path_direct_tracepoint_post_recv(
		const struct efa_qp *qp,
		const struct ibv_recv_wr *wr)
{
	efa_tracepoint(data_path_direct_post_recv,
		qp->base_ep->domain->device->ibv_ctx->device->name,
		wr->wr_id,
		qp->ibv_qp->qp_num,
		wr->num_sge);
}

static inline void efa_data_path_direct_tracepoint_process_completion(
		const struct efa_qp *qp,
		const struct ibv_cq_ex *ibvcqx,
		const struct efa_io_cdesc_common *cqe)
{
	/* Extract opcode inline - similar to efa_data_path_direct_wc_read_opcode */
	enum efa_io_send_op_type op_type = EFA_GET(&cqe->flags, EFA_IO_CDESC_COMMON_OP_TYPE);
	int opcode;
	uint32_t src_qp_num;
	uint16_t ah_num;
	uint32_t length = 0;

	if (EFA_GET(&cqe->flags, EFA_IO_CDESC_COMMON_Q_TYPE) == EFA_IO_SEND_QUEUE) {
		opcode = (op_type == EFA_IO_RDMA_WRITE) ? IBV_WC_RDMA_WRITE : IBV_WC_SEND;
		src_qp_num = qp->ibv_qp->qp_num; /* For TX, src is our QP */
		ah_num = UINT16_MAX; /* Not applicable for TX */
	} else {
		/* RX completion */
		opcode = (op_type == EFA_IO_RDMA_WRITE) ? IBV_WC_RECV_RDMA_WITH_IMM : IBV_WC_RECV;
		/* Extract src_qp and ah from RX completion - inline version of helper functions */
		const struct efa_io_rx_cdesc *rcqe = container_of(cqe, struct efa_io_rx_cdesc, common);
		src_qp_num = rcqe->src_qp_num;
		ah_num = rcqe->ah;

		/* Extract length for RX - inline version of efa_data_path_direct_wc_read_byte_len */
		const struct efa_io_rx_cdesc_ex *rcqe_ex = container_of(cqe, struct efa_io_rx_cdesc_ex, base.common);
		length = rcqe_ex->base.length;
		if (op_type == EFA_IO_RDMA_WRITE)
			length |= ((uint32_t)rcqe_ex->u.rdma_write.length_hi << 16);
	}

	efa_tracepoint(data_path_direct_process_completion,
		qp->base_ep->domain->device->ibv_ctx->device->name,
		ibvcqx,
		opcode,
		src_qp_num,
		qp->ibv_qp->qp_num,
		ah_num,
		length);
}

#endif /* HAVE_EFA_DATA_PATH_DIRECT */

#else

#define efa_tracepoint(...)	do {} while(0)
#define efa_tracef(...)	do {} while(0)
#define efa_tracelog(...)	do {} while(0)

#endif /* HAVE_LTTNG */

#endif /* _EFA_TP_H */
