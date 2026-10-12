/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright (c) 2013-2015 Intel Corporation, Inc.  All rights reserved. */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */

#include <errno.h>
#include <string.h>
#include "config.h"
#include <ofi_mem.h>
#include "efa.h"
#include "efa_av.h"
#include "efa_cntr.h"
#include "efa_cq.h"
#include "efa_data_path_ops.h"
#include <infiniband/verbs.h>
#include "efa_data_path_direct.h"


static inline uint64_t efa_cq_opcode_to_fi_flags(enum ibv_wc_opcode opcode) {
	switch (opcode) {
	case IBV_WC_SEND:
		return FI_SEND | FI_MSG;
	case IBV_WC_RECV:
		return FI_RECV | FI_MSG;
	case IBV_WC_RDMA_WRITE:
		return FI_RMA | FI_WRITE;
	case IBV_WC_RECV_RDMA_WITH_IMM:
		return FI_REMOTE_CQ_DATA | FI_RMA | FI_REMOTE_WRITE;
	case IBV_WC_RDMA_READ:
		return FI_RMA | FI_READ;
	default:
		assert(0 && "Unhandled op code");
		return 0;
	}
}

static inline void efa_cq_direct_ope_release(struct efa_cq *efa_cq,
					      struct efa_ibv_cq *ibv_cq)
{
	struct efa_domain *efa_domain;
	struct efa_base_ep *base_ep;

	if (efa_env.track_mr && ibv_cq->ibv_cq_ex->wr_id) {
		efa_domain = container_of(efa_cq->util_cq.domain, struct efa_domain, util_domain);
		base_ep = efa_ibv_cq_get_base_ep_from_cur_cqe(ibv_cq, efa_domain);
		efa_direct_ope_release(base_ep,
			(struct efa_direct_ope *)(uintptr_t)ibv_cq->ibv_cq_ex->wr_id);
	}
}

static void efa_cq_read_context_entry(struct efa_ibv_cq *ibv_cq, void *buf, int opcode)
{
	struct fi_cq_entry *entry = buf;

	if (efa_env.track_mr && ibv_cq->ibv_cq_ex->wr_id)
		entry->op_context = ((struct efa_direct_ope *)(uintptr_t)ibv_cq->ibv_cq_ex->wr_id)->context;
	else
		entry->op_context = (void *)(uintptr_t)ibv_cq->ibv_cq_ex->wr_id;
}

static inline
void efa_cq_read_entry_common(struct efa_ibv_cq *cq, struct fi_cq_msg_entry *entry, int opcode)
{
	struct ibv_cq_ex *ibv_cqx = cq->ibv_cq_ex;
	struct efa_direct_ope *direct_ope;

	if (!efa_cq_wc_is_unsolicited(cq) && ibv_cqx->wr_id) {
		if (efa_env.track_mr) {
			direct_ope = (struct efa_direct_ope *)(uintptr_t)ibv_cqx->wr_id;
			entry->op_context = direct_ope->context;
			entry->flags = (opcode == IBV_WC_RECV_RDMA_WITH_IMM) ? efa_cq_opcode_to_fi_flags(opcode) : direct_ope->context->completion_flags;
		} else {
			entry->op_context = (void *)ibv_cqx->wr_id;
			entry->flags = (opcode == IBV_WC_RECV_RDMA_WITH_IMM) ? efa_cq_opcode_to_fi_flags(opcode): ((struct efa_context *) ibv_cqx->wr_id)->completion_flags;
		}
	} else {
		entry->op_context = NULL;
		entry->flags = efa_cq_opcode_to_fi_flags(opcode);
	}
	entry->len = efa_ibv_cq_wc_read_byte_len(cq);
}

static void efa_cq_read_msg_entry(struct efa_ibv_cq *cq, void *buf, int opcode)
{
	efa_cq_read_entry_common(cq, (struct fi_cq_msg_entry *)buf, opcode);
}

static void efa_cq_read_data_entry(struct efa_ibv_cq *cq, void *buf, int opcode)
{
	struct fi_cq_data_entry *entry = buf;

	efa_cq_read_entry_common(cq, (struct fi_cq_msg_entry *)buf, opcode);
	entry->buf = NULL;
	entry->data = 0;
	if (efa_ibv_cq_wc_read_wc_flags(cq) & IBV_WC_WITH_IMM) {
		entry->flags |= FI_REMOTE_CQ_DATA;
		entry->data = efa_ibv_cq_wc_read_imm_data(cq);
	}
}

static inline void efa_cq_write_error_data(struct efa_cq *efa_cq, struct efa_base_ep *base_ep, fi_addr_t addr, int prov_errno, struct fi_cq_err_entry *buf)
{
	char *err_msg;
	int err = to_fi_errno(prov_errno);

	EFA_INFO(FI_LOG_CQ, "Encountered error during CQ polling. err: %s (%d), prov_errno: %s (%d)\n",
			fi_strerror(err), err, efa_strerror(prov_errno), prov_errno);
	efa_show_help(prov_errno);

	if (buf->err_data_size > 0 && FI_VERSION_GE(efa_cq->util_cq.domain->fabric->fabric_fid.api_version, FI_VERSION(1, 5))) {
		err_msg = (char *)buf->err_data;
	} else {
		err_msg = efa_cq->err_buf;
	}

	if (efa_write_error_msg(base_ep, addr, prov_errno, err_msg, &buf->err_data_size) != 0) {
		buf->err_data_size = 0;
	} else if (err_msg == efa_cq->err_buf) {
		buf->err_data = err_msg;
	}
}

static inline void efa_cq_fill_err_entry(struct efa_ibv_cq *ibv_cq, struct fi_cq_err_entry *buf)
{
	struct efa_cq *efa_cq = container_of(ibv_cq, struct efa_cq, ibv_cq);
	struct efa_domain *efa_domain = container_of(efa_cq->util_cq.domain, struct efa_domain, util_domain);
	struct efa_base_ep *base_ep = efa_ibv_cq_get_base_ep_from_cur_cqe(ibv_cq, efa_domain);
	int opcode = efa_ibv_cq_wc_read_opcode(ibv_cq);
	int prov_errno = efa_ibv_cq_wc_read_vendor_err(ibv_cq);
	fi_addr_t addr;

	assert(base_ep);
	/* Use the most informative entry that efa-direct support to construct cq entry for general usage */
	efa_cq_read_data_entry(ibv_cq, buf, opcode);
	buf->err = to_fi_errno(prov_errno);
	buf->prov_errno = prov_errno;

	switch (opcode) {
	case IBV_WC_SEND: /* fall through */
	case IBV_WC_RDMA_WRITE: /* fall through */
	case IBV_WC_RDMA_READ:
		if (!ibv_cq->ibv_cq_ex->wr_id) {
			addr = FI_ADDR_NOTAVAIL;
		} else if (efa_env.track_mr) {
			addr = ((struct efa_direct_ope *)(uintptr_t)ibv_cq->ibv_cq_ex->wr_id)->context->addr;
		} else {
			addr = ((struct efa_context *)ibv_cq->ibv_cq_ex->wr_id)->addr;
		}
		break;
	case IBV_WC_RECV: /* fall through */
	case IBV_WC_RECV_RDMA_WITH_IMM:
		addr = efa_av_reverse_lookup(base_ep->av, efa_ibv_cq_wc_read_slid(ibv_cq), efa_ibv_cq_wc_read_src_qp(ibv_cq));
		break;
	default:
		addr = FI_ADDR_NOTAVAIL;
		break;
	}

	efa_cq_write_error_data(efa_cq, base_ep, addr, prov_errno, buf);
}

/**
 * @brief handle the situation that a TX/RX operation encountered error
 *
 * This function does the following to handle error:
 *
 * 1. write an error cq entry for the operation, if writing
 *    CQ error entry failed, it will write eq entry.
 *
 * 2. increase error counter.
 *
 * 3. print warning message with self and peer's raw address
 *
 * 4. complete ope if mr tracking is enabled
 *
 * @param[in]	base_ep     efa_base_ep
 * @param[in]	efa_cq      EFA CQ containing the extended ibv cq
 * @param[in]	err         positive libfabric error code
 * @param[in]	prov_errno  positive EFA provider specific error code
 */
static void efa_cq_handle_error(struct efa_base_ep *base_ep,
				struct efa_ibv_cq *cq, int err,
				int prov_errno)
{
	struct fi_cq_err_entry err_entry;
	int write_cq_err;
	struct efa_cq *efa_cq = container_of(cq, struct efa_cq, ibv_cq);

	memset(&err_entry, 0, sizeof(err_entry));
	/* This will use efa_cq->err_buf (because we set
	 * err_entry->err_data_size as 0) to store the err_data which will be
	 * copied to util_cq in the subsequent ofi_cq_write_error
	 */
	efa_cq_fill_err_entry(cq, &err_entry);

	if (efa_env.track_mr && cq->ibv_cq_ex->wr_id)
		efa_direct_ope_release(base_ep,
			(struct efa_direct_ope *)(uintptr_t)cq->ibv_cq_ex->wr_id);

	efa_cntr_report_error(&base_ep->util_ep, err_entry.flags);
	write_cq_err = ofi_cq_write_error(&efa_cq->util_cq, &err_entry);
	if (write_cq_err) {
		EFA_WARN(
			FI_LOG_CQ,
			"Error when writing error cq entry\n");
		efa_base_ep_write_eq_error(base_ep, err, prov_errno);
	}
}

/**
 * @brief handle the event that a TX request has been completed
 *
 * @param[in]		base_ep     efa_base_ep
 * @param[in]		ibv_cq_ex   extended ibv cq
 * @param[in]		cq_entry    fi_cq_tagged_entry
 */
static void efa_cq_handle_tx_completion(struct efa_base_ep *base_ep,
					struct efa_ibv_cq *ibv_cq,
					struct fi_cq_tagged_entry *cq_entry)
{
	struct util_cq *tx_cq = base_ep->util_ep.tx_cq;
	int ret = 0;
	struct ibv_cq_ex *ibv_cq_ex = ibv_cq->ibv_cq_ex;

	/* NULL wr_id means no FI_COMPLETION flag */
	if (!ibv_cq_ex->wr_id)
		return;

	efa_tracepoint(handle_tx_completion, ibv_cq_ex->wr_id);

	if (efa_env.track_mr)
		efa_direct_ope_release(base_ep,
			(struct efa_direct_ope *)(uintptr_t)ibv_cq_ex->wr_id);

	/* TX completions should not send peer address to util_cq */
	if (base_ep->util_ep.caps & FI_SOURCE)
		ret = ofi_cq_write_src(tx_cq, cq_entry->op_context,
				       cq_entry->flags, cq_entry->len,
				       cq_entry->buf, cq_entry->data,
				       cq_entry->tag, FI_ADDR_NOTAVAIL);
	else
		ret = ofi_cq_write(tx_cq, cq_entry->op_context, cq_entry->flags,
				   cq_entry->len, cq_entry->buf, cq_entry->data,
				   cq_entry->tag);

	if (OFI_UNLIKELY(ret)) {
		EFA_WARN(FI_LOG_CQ, "Unable to write send completion: %s\n",
			 fi_strerror(-ret));
		efa_cq_handle_error(base_ep, ibv_cq, -ret,
				    FI_EFA_ERR_WRITE_SEND_COMP);
	}
}

/**
 * @brief handle the event that a RX request has been completed
 *
 * @param[in]		base_ep     efa_base_ep
 * @param[in]		ibv_cq_ex   extended ibv cq
 * @param[in]		cq_entry    fi_cq_tagged_entry
 */
static void efa_cq_handle_rx_completion(struct efa_base_ep *base_ep,
					struct efa_ibv_cq *ibv_cq,
					struct fi_cq_tagged_entry *cq_entry)
{
	struct util_cq *rx_cq = base_ep->util_ep.rx_cq;
	struct ibv_cq_ex *ibv_cq_ex = ibv_cq->ibv_cq_ex;
	fi_addr_t src_addr;
	int ret = 0;

	/* NULL wr_id means no FI_COMPLETION flag */
	if (!ibv_cq_ex->wr_id)
		return;

	efa_tracepoint(handle_rx_completion, ibv_cq_ex->wr_id);

	if (efa_env.track_mr)
		efa_direct_ope_release(base_ep,
			(struct efa_direct_ope *)(uintptr_t)ibv_cq_ex->wr_id);

	if (base_ep->util_ep.caps & FI_SOURCE) {
		src_addr = efa_av_reverse_lookup(base_ep->av,
						 efa_ibv_cq_wc_read_slid(ibv_cq),
						 efa_ibv_cq_wc_read_src_qp(ibv_cq));
		ret = ofi_cq_write_src(rx_cq, cq_entry->op_context,
				       cq_entry->flags, cq_entry->len,
				       cq_entry->buf, cq_entry->data,
				       cq_entry->tag, src_addr);
	} else {
		ret = ofi_cq_write(rx_cq, cq_entry->op_context, cq_entry->flags,
				   cq_entry->len, cq_entry->buf, cq_entry->data,
				   cq_entry->tag);
	}

	if (OFI_UNLIKELY(ret)) {
		EFA_WARN(FI_LOG_CQ, "Unable to write recv completion: %s\n",
			 fi_strerror(-ret));
		efa_cq_handle_error(base_ep, ibv_cq, -ret,
				    FI_EFA_ERR_WRITE_RECV_COMP);
	}
}

/**
 * @brief handle rdma-core CQ completion resulted from IBV_WRITE_WITH_IMM
 *
 * This function handles hardware-assisted RDMA writes with immediate data at
 * remote endpoint.  These do not have a packet context, nor do they have a
 * connid available.
 * 
 * @param[in]		base_ep     efa_base_ep
 * @param[in]		ibv_cq_ex   extended ibv cq
 */
static void
efa_cq_proc_ibv_recv_rdma_with_imm_completion(struct efa_base_ep *base_ep,
					      struct efa_ibv_cq *ibv_cq,
					      struct fi_cq_tagged_entry *cq_entry)
{
	struct util_cq *rx_cq = base_ep->util_ep.rx_cq;
	int ret;
	fi_addr_t src_addr;

	if (base_ep->util_ep.caps & FI_SOURCE) {
		src_addr = efa_av_reverse_lookup(base_ep->av,
						 efa_ibv_cq_wc_read_slid(ibv_cq),
						 efa_ibv_cq_wc_read_src_qp(ibv_cq));
		ret = ofi_cq_write_src(rx_cq, cq_entry->op_context, cq_entry->flags, cq_entry->len, NULL, cq_entry->data,
				       0, src_addr);
	} else {
		ret = ofi_cq_write(rx_cq, cq_entry->op_context, cq_entry->flags, cq_entry->len, NULL, cq_entry->data, 0);
	}

	if (OFI_UNLIKELY(ret)) {
		EFA_WARN(FI_LOG_CQ,
			 "Unable to write a cq entry for remote for RECV_RDMA "
			 "operation: %s\n",
			 fi_strerror(-ret));
		efa_base_ep_write_eq_error(base_ep, -ret,
					   FI_EFA_ERR_WRITE_RECV_COMP);
	}
}

/**
 * @brief poll rdma-core cq and process the cq entry
 *
 * @param[in]	cqe_to_process    Max number of cq entry to poll and process. 
 * A negative number means to poll until cq empty.
 * @param[in]   util_cq           util_cq
 */
int efa_cq_poll_ibv_cq(ssize_t cqe_to_process, struct efa_ibv_cq *ibv_cq)
{
	struct efa_base_ep *base_ep;
	struct efa_cq *cq;
	struct efa_domain *efa_domain;
	struct fi_cq_tagged_entry cq_entry = {0};
	int err = 0;
	size_t num_cqe = 0; /* Count of read entries */
	int prov_errno, opcode;

	cq = container_of(ibv_cq, struct efa_cq, ibv_cq);
	efa_domain = container_of(cq->util_cq.domain, struct efa_domain, util_domain);

	/* Call ibv_start_poll only once */
	efa_cq_start_poll(ibv_cq);

	while (efa_cq_wc_available(ibv_cq)) {
		base_ep = efa_ibv_cq_get_base_ep_from_cur_cqe(ibv_cq, efa_domain);
		/* This can only happen in the following steps
		 * 1. fi_cq_read returned EAVAIL
		 * 2. fi_cq_readerr was not called to read that cq err entry,
		 * and then device cq remains at an active state
		 * 3. fi_close(ep) destroy the qp -> set qp_table[qpn] to NULL
		 * 4. fi_close(ep) calls efa_cq_poll_ibv_cq to drain the cq,
		 * which skipped that efa_cq_start_poll because it is already in
		 * a poll active status, and the NULL check of qp table in
		 * efa_ibv_cq_start_poll didn't happen. Then efa_cq_wc_available is
		 * true and start to access the cqe from the destroyed qp
		 * Today this "device cq staying at active state" can only
		 * happen for the util cq bypass code path
		 */
		if (!base_ep) {
			/* Ignore the cq error from a destroyed QP */
			efa_cq_next_poll(ibv_cq);
			continue;
		}
		assert(base_ep);
		opcode = efa_ibv_cq_wc_read_opcode(ibv_cq);
		if (ibv_cq->ibv_cq_ex->status) {
			prov_errno = efa_ibv_cq_wc_read_vendor_err(ibv_cq);
			efa_cq_handle_error(base_ep, ibv_cq, to_fi_errno(prov_errno), prov_errno);
			break;
		}

		/* Use the most informative entry that efa-direct support to construct cq entry for general usage */
		efa_cq_read_data_entry(ibv_cq, &cq_entry, opcode);
		EFA_DBG(FI_LOG_CQ,
			"Write cq entry of context: %lx, flags: %lx\n",
			(size_t) cq_entry.op_context, cq_entry.flags);

		switch (opcode) {
		case IBV_WC_SEND: /* fall through */
		case IBV_WC_RDMA_WRITE: /* fall through */
		case IBV_WC_RDMA_READ:
			efa_cq_handle_tx_completion(base_ep, ibv_cq, &cq_entry);
			efa_cntr_report_tx_completion(&base_ep->util_ep, cq_entry.flags);
			break;
		case IBV_WC_RECV:
			efa_cq_handle_rx_completion(base_ep, ibv_cq, &cq_entry);
			efa_cntr_report_rx_completion(&base_ep->util_ep, cq_entry.flags);
			break;
		case IBV_WC_RECV_RDMA_WITH_IMM:
			efa_cq_proc_ibv_recv_rdma_with_imm_completion(
				base_ep, ibv_cq, &cq_entry);
			efa_cntr_report_rx_completion(&base_ep->util_ep, cq_entry.flags);
			break;
		default:
			EFA_WARN(FI_LOG_EP_CTRL,
				"Unhandled cq type\n");
			assert(0 && "Unhandled cq type");
		}

		num_cqe++;
		if (num_cqe == cqe_to_process) {
			break;
		}

		efa_cq_next_poll(ibv_cq);
	}
	err = ibv_cq->poll_err;
	efa_cq_end_poll(ibv_cq);
	return err;
}

const char *efa_cq_strerror(struct fid_cq *cq_fid, int prov_errno,
			    const void *err_data, char *buf, size_t len)
{
	return err_data
		? (const char *) err_data
		: efa_strerror(prov_errno);
}

#if HAVE_EFA_CQ_NOTIFICATION
int efa_cq_signal_init(struct efa_cq *cq)
{
	int ret;

	if (cq->wait_obj == FI_WAIT_NONE || !cq->ibv_cq.channel)
		return FI_SUCCESS;

	ret = fd_signal_init(&cq->signal);
	if (ret) {
		EFA_WARN(FI_LOG_CQ, "Failed to initialize signal FD: %s (%d)\n",
			 fi_strerror(-ret), -ret);
		return ret;
	}

	ret = efa_ibv_req_notify_cq(&cq->ibv_cq, 0);
	if (ret) {
		ret = -errno;
		EFA_WARN(FI_LOG_CQ, "ibv_req_notify_cq failed: %s (%d)\n",
			 fi_strerror(ret), errno);
		fd_signal_free(&cq->signal);
		return ret;
	}

	ofi_atomic_initialize32(&cq->nevents, 0);

	return FI_SUCCESS;
}

void efa_cq_ack_events(struct efa_cq *cq)
{
	struct ibv_cq *ibv_cq;

	if (!cq->ibv_cq.ibv_cq_ex || !cq->ibv_cq.channel)
		return;

	ibv_cq = ibv_cq_ex_to_cq(cq->ibv_cq.ibv_cq_ex);

	if (ofi_atomic_get32(&cq->nevents))
		ibv_ack_cq_events(ibv_cq, ofi_atomic_get32(&cq->nevents));
}

int efa_cq_destroy_comp_channel(struct efa_cq *cq)
{
	if (!cq->ibv_cq.channel)
		return FI_SUCCESS;

	if (ibv_destroy_comp_channel(cq->ibv_cq.channel)) {
		EFA_WARN(FI_LOG_CQ, "Unable to destroy completion channel: %s\n",
			 strerror(errno));
		return -errno;
	}

	cq->ibv_cq.channel = NULL;
	return FI_SUCCESS;
}
#else
int efa_cq_signal_init(struct efa_cq *cq)
{
	return (cq->wait_obj == FI_WAIT_NONE) ? FI_SUCCESS : -FI_ENOSYS;
}

void efa_cq_ack_events(struct efa_cq *cq)
{
}

int efa_cq_destroy_comp_channel(struct efa_cq *cq)
{
	return FI_SUCCESS;
}
#endif

void efa_cq_signal_fini(struct efa_cq *cq)
{
	if (cq->wait_obj != FI_WAIT_NONE && cq->ibv_cq.channel)
		fd_signal_free(&cq->signal);
}

/**
 * @brief Try to wait on CQ - check if ready for blocking
 *
 * @param[in] cq EFA completion queue
 * @return 0 if ready to wait, -FI_EAGAIN if completions available
 */
#if HAVE_EFA_CQ_NOTIFICATION
int efa_cq_trywait(struct efa_cq *cq)
{
	void *context;
	int ret;

	if (!cq->ibv_cq.channel) {
		EFA_WARN(FI_LOG_CQ, "No ibv_comp_channel associated with CQ\n");
		return -FI_EINVAL;
	}

	if (!ofi_cirque_isempty(cq->util_cq.cirq)) {
		EFA_INFO(FI_LOG_CQ, "efa_cq_trywait: completions available in "
				 "util_cq, return -FI_EAGAIN\n");
		return -FI_EAGAIN;
	}

	while (!efa_ibv_get_cq_event(&cq->ibv_cq, &context)) {
		ofi_atomic_inc32(&cq->nevents);
		EFA_DBG(FI_LOG_CQ,
				"efa_cq_trywait: drained pending event, nevents=%d\n",
				ofi_atomic_get32(&cq->nevents));
	}

	/* Arm the completion notification for next completion */
	ret = efa_ibv_req_notify_cq(&cq->ibv_cq, 0);
	if (ret) {
		ret = -errno;
		EFA_WARN(FI_LOG_CQ,
				 "ibv_req_notify_cq failed with errno=%d (%s)\n", errno,
				 fi_strerror(ret));
		return ret;
	}

	/* Fetch any completions that we might have missed while rearming */
	efa_cq_progress(&cq->util_cq);

	return ofi_cirque_isempty(cq->util_cq.cirq) ? FI_SUCCESS : -FI_EAGAIN;
}
#else
int efa_cq_trywait(struct efa_cq *cq) {
	return -FI_ENOSYS;
}
#endif

/**
 * @brief Poll for completion events with timeout.
 * 
 * ibv_get_cq_event() waits for the next completion event in the
 * completion event channel. Fills the arguments cq with the
 * CQ that got the event and cq_context with the CQ's context.
 *
 * @param[in] cq EFA completion queue
 * @param[in] timeout Timeout in milliseconds
 * @return 0 on success, -FI_EAGAIN on timeout, negative error code on failure
 */
#if HAVE_EFA_CQ_NOTIFICATION
int efa_poll_events(struct efa_cq *cq, int timeout)
{
	int ret, rc;
	void *context;
	struct pollfd fds[2];

	fds[0].fd = cq->ibv_cq.channel->fd;
	/* A signal FD for interrupting the wait operation */
	fds[1].fd = fd_signal_get(&cq->signal);

	fds[0].events = fds[1].events = POLLIN;

	rc = poll(fds, 2, timeout);
	if (rc == 0) {
		EFA_DBG(FI_LOG_CQ, "efa_poll_events: poll() timeout after %d ms\n", timeout);
		return -FI_EAGAIN;
	} else if (rc < 0) {
		ret = -errno;
		EFA_WARN(FI_LOG_CQ,
				 "efa_poll_events: poll() failed with errno=%d (%s)\n",
				 errno, fi_strerror(ret));
		return ret;
	}

	ret = 0;
	if (fds[0].revents & POLLIN) {
		/* Data is available for reading on completion channel */
		ret = efa_ibv_get_cq_event(&cq->ibv_cq, &context);
		if (ret) {
			EFA_WARN(FI_LOG_CQ, "ibv_get_cq_event failed with ret=%d\n", ret);
			return ret;
		}

		ofi_atomic_inc32(&cq->nevents);
		rc--;
	}
	
	if (fds[1].revents & POLLIN) {
		EFA_INFO(FI_LOG_CQ, "efa_poll_events: signal FD triggered by fi_cq_signal\n");
		fd_signal_reset(&cq->signal);
		ret = -FI_ECANCELED;
		rc--;
	}

	if (rc > 0) {
		EFA_WARN(FI_LOG_CQ,
				 "efa_poll_events: unexpected poll events remaining "
				 "(%d), comp_revents=0x%x signal_revents=0x%x\n",
				 rc, fds[0].revents, fds[1].revents);
		return -FI_EOTHER;
	}

	return ret;
}
#else
int efa_poll_events(struct efa_cq *cq, int timeout)
{
	return -FI_ENOSYS;
}
#endif

/**
 * @brief
 * A synchronous (blocking) read that waits until a specified condition has
 * been met before reading a completion from a completion queue.
 *
 * @param[in]	cq_fid		Completion queue
 * @param[out]	buf			Data buffer to write completions into
 * @param[in]	count		Number of CQ entries
 * @param[out]	src_addr	Source address of a completed receive operation
 * @param[in]	cond		Condition that must be met before a completion
 * is generated
 * @param[in]	timeout		Time in milliseconds to wait. A negative value
 * indicates infinite timeout.
 *
 * @return On success, returns the number of completions retrieved from the
 * completion queue. On error, returns a negative fabric errno, with these two
 * errors explicitly identified: If the timeout expires or the calling thread
 * is signaled and no data is available to be read from the completion queue,
 * returns -FI_EAGAIN. If the topmost completion is for a failed transfer
 * (an error entry), returns -FI_EAVAIL.
 */
static ssize_t efa_cq_sreadfrom(struct fid_cq *cq_fid, void *buf, size_t count,
								fi_addr_t *src_addr, const void *cond,
								int timeout)
{
	struct efa_cq *cq;
	ssize_t ret = 0;
	uint64_t endtime = ofi_timeout_time(timeout);
	ssize_t threshold = 1, num_completions;
	uint8_t *buffer;

	buffer = buf;
	cq = container_of(cq_fid, struct efa_cq, util_cq.cq_fid);

	if (!cq->wait_obj) {
		EFA_WARN(FI_LOG_CQ, "Cannot call fi_cq_sread with FI_WAIT_NONE\n");
		return -FI_ENOSYS;
	}

	if (!cq->ibv_cq.channel) {
		EFA_WARN(FI_LOG_CQ, "No ibv_comp_channel associated with CQ\n");
		return -FI_ENOSYS;
	}

	if (cq->wait_cond == FI_CQ_COND_THRESHOLD && cond)
		threshold = MIN(*(ssize_t *) cond, count);

	for (num_completions = 0; num_completions < threshold; ) {
		if (efa_cq_trywait(cq) == FI_SUCCESS) {
			/* CQ is empty, wait for events */
			ret = efa_poll_events(cq, timeout);
			if (ret && ret != -FI_EAGAIN)
				break;
		} 
		
		ret = cq->util_cq.cq_fid.ops->readfrom(&cq->util_cq.cq_fid, buffer, count - num_completions, src_addr ? src_addr + num_completions : NULL);
		if (ret > 0) {
			buffer += ret * cq->entry_size;
			num_completions += ret;
			if (num_completions >= threshold)
				break;
		} else if (ret != -FI_EAGAIN) {
			break;
		}

		if (ofi_adjust_timeout(endtime, &timeout))
			return num_completions ? num_completions : -FI_EAGAIN;
	}

	return num_completions ? num_completions : ret;
}

static ssize_t efa_cq_sread(struct fid_cq *cq_fid, void *buf, size_t count,
							const void *cond, int timeout)
{
	return efa_cq_sreadfrom(cq_fid, buf, count, NULL, cond, timeout);
}

/**
 * @brief Unblock any thread waiting in fi_cq_sread or fi_cq_sreadfrom.
 * This may be used to wake-up a thread that is blocked waiting to read a
 * completion operation. The fi_cq_signal operation is only available if the CQ
 * was configured with a wait object.
 * 
 * Returns 0 on success. On error, returns a negative fabric errno.
 */
int efa_cq_signal(struct fid_cq *cq_fid)
{
	struct efa_cq *cq;

	cq = container_of(cq_fid, struct efa_cq, util_cq.cq_fid);
	if (!cq->wait_obj) {
		EFA_WARN(FI_LOG_CQ, "fi_cq_signal is only available if the CQ "
				 "was configured with a wait object.");
		return -FI_EINVAL;
	}

	fd_signal_set(&cq->signal);

	return 0;
}

static inline fi_addr_t efa_cq_get_src_addr(struct efa_ibv_cq *ibv_cq, int opcode)
{
	struct efa_cq *efa_cq;
	struct efa_domain *efa_domain;
	struct efa_base_ep *base_ep;

	switch (opcode) {
	case IBV_WC_RECV:
	case IBV_WC_RECV_RDMA_WITH_IMM:
		efa_cq = container_of(ibv_cq, struct efa_cq, ibv_cq);
		efa_domain = container_of(efa_cq->util_cq.domain, struct efa_domain, util_domain);
		base_ep = efa_ibv_cq_get_base_ep_from_cur_cqe(ibv_cq, efa_domain);
		assert(base_ep);
		if (!(base_ep->util_ep.caps & FI_SOURCE))
			return FI_ADDR_NOTAVAIL;
		return efa_av_reverse_lookup(base_ep->av,
					     efa_ibv_cq_wc_read_slid(ibv_cq),
					     efa_ibv_cq_wc_read_src_qp(ibv_cq));
	default:
		return FI_ADDR_NOTAVAIL;
	}
}

static
ssize_t efa_cq_readfrom(struct fid_cq *cq_fid, void *buf, size_t count,
			fi_addr_t *src_addr)
{
	struct efa_cq *efa_cq;
	struct efa_ibv_cq *ibv_cq;
	int err = -FI_EAGAIN;
	size_t num_cqe = 0; /* Count of read entries */
	int opcode;

	if (OFI_UNLIKELY((!buf || !count)))
		return -FI_EAGAIN;

	efa_cq = container_of(cq_fid, struct efa_cq, util_cq.cq_fid);

	/* Acquire the lock to prevent race conditions when qp_table is being updated */
	ofi_genlock_lock(&efa_cq->util_cq.ep_list_lock);

	/* If there are cqes in the util cq (due to the cq flush in ep close or efa_trywait) */
	if (!ofi_cirque_isempty(efa_cq->util_cq.cirq)) {
		err = ofi_cq_read_entries(&efa_cq->util_cq, buf, count, src_addr);
		goto out;
	}

	ibv_cq = &efa_cq->ibv_cq;

	/* Call ibv_start_poll only once */
	efa_cq_start_poll(ibv_cq);

	while (efa_cq_wc_available(ibv_cq)) {
		if (ibv_cq->ibv_cq_ex->status) {
			err = -FI_EAVAIL;
			goto out;
		}

		opcode = efa_ibv_cq_wc_read_opcode(ibv_cq);
		/**
		 * Only populate the cqes when:
		 * 1. It is IBV_WC_RECV_RDMA_WITH_IMM which is the target side of the rdma write with imm
		 * 2. It is a solicited wc and having wr_id (efa_context) which means it needs a completion.
		 */
		if ((!efa_cq_wc_is_unsolicited(ibv_cq) && ibv_cq->ibv_cq_ex->wr_id ) || opcode == IBV_WC_RECV_RDMA_WITH_IMM) {
			efa_tracepoint(handle_completion, ibv_cq->ibv_cq_ex->wr_id, opcode);
			efa_cq->read_entry(ibv_cq, (void *)((uintptr_t) buf + num_cqe * efa_cq->entry_size), opcode);
			if (src_addr)
				src_addr[num_cqe] = efa_cq_get_src_addr(ibv_cq, opcode);
			efa_cq_direct_ope_release(efa_cq, ibv_cq);
			num_cqe++;
		}

		if (num_cqe == count) {
			break;
		}
		efa_cq_next_poll(ibv_cq);
	}
	efa_cq_end_poll(ibv_cq);

out:
	ofi_genlock_unlock(&efa_cq->util_cq.ep_list_lock);
	return num_cqe ? num_cqe : err;
}

ssize_t efa_cq_readerr(struct fid_cq *cq_fid, struct fi_cq_err_entry *buf,
		       uint64_t flags)
{
	struct efa_cq *efa_cq;
	struct efa_ibv_cq *ibv_cq;
	ssize_t ret;

	efa_cq = container_of(cq_fid, struct efa_cq, util_cq.cq_fid);

	ofi_genlock_lock(&efa_cq->util_cq.ep_list_lock);

	ibv_cq = &efa_cq->ibv_cq;

	/* If there are error in util cq already, just return it */
	ret = ofi_cq_readerr(cq_fid, buf, flags);
	if (ret > 0)
		goto out;

	/* No wc / wc error is available, return EAGAIN */
	if(!ibv_cq->poll_active) {
		ret = -FI_EAGAIN;
		goto out;
	}

	/**
	 * It is impossible that we can have a zero status here since
	 * the efa_cq_readfrom only leave the poll active
	 * when hitting a wc error (status != 0).
	 */
	assert(ibv_cq->ibv_cq_ex->status);
	efa_cq_fill_err_entry(ibv_cq, buf);

	efa_cq_direct_ope_release(efa_cq, ibv_cq);

	efa_cq_end_poll(ibv_cq);
	ret = 1;

out:
	ofi_genlock_unlock(&efa_cq->util_cq.ep_list_lock);
	return ret;
}

/* CQ ops with EFA-specific readfrom/readerr that bypass the util cq and counters on the data path */
struct fi_ops_cq efa_cq_bypass_util_cq_ops = {
	.size = sizeof(struct fi_ops_cq),
	.read = ofi_cq_read,
	.readfrom = efa_cq_readfrom,
	.readerr = efa_cq_readerr,
	.sread = efa_cq_sread,
	.sreadfrom = efa_cq_sreadfrom,
	.signal = efa_cq_signal,
	.strerror = efa_cq_strerror
};

/* CQ ops with util readfrom/readerr that stages cqes and increments counters during cq read */
struct fi_ops_cq efa_cq_ops = {
	.size = sizeof(struct fi_ops_cq),
	.read = ofi_cq_read,
	.readfrom = ofi_cq_readfrom,
	.readerr = ofi_cq_readerr,
	.sread = efa_cq_sread,
	.sreadfrom = efa_cq_sreadfrom,
	.signal = efa_cq_signal,
	.strerror = efa_cq_strerror
};

void efa_cq_progress(struct util_cq *cq)
{
	struct efa_cq *efa_cq = container_of(cq, struct efa_cq, util_cq);

	/* Acquire the lock to prevent race conditions when qp_table is being updated */
	ofi_genlock_lock(&cq->ep_list_lock);
	(void) efa_cq_poll_ibv_cq(efa_env.efa_cq_read_size, &efa_cq->ibv_cq);
	ofi_genlock_unlock(&cq->ep_list_lock);
}

int efa_cq_close(fid_t fid)
{
	struct efa_cq *cq;
	struct ibv_cq *ibv_cq;
	int ret;

	cq = container_of(fid, struct efa_cq, util_cq.cq_fid.fid);

	if (cq->ibv_cq.ibv_cq_ex) {
		ibv_cq = ibv_cq_ex_to_cq(cq->ibv_cq.ibv_cq_ex);

		efa_cq_ack_events(cq);

		ret = -ibv_destroy_cq(ibv_cq);
		if (ret) {
			EFA_WARN(FI_LOG_CQ, "Unable to close ibv cq: %s\n",
				fi_strerror(-ret));
			return ret;
		}
		cq->ibv_cq.ibv_cq_ex = NULL;
	}

	efa_cq_signal_fini(cq);

	ret = ofi_cq_cleanup(&cq->util_cq);
	if (ret)
		return ret;

	ret = efa_cq_destroy_comp_channel(cq);
	if (ret)
		return ret;

	if (cq->err_buf)
		free(cq->err_buf);

	free(cq);

	return 0;
}

/**
 * @brief
 * The fi_control call is used to access provider or implementation specific
 * details of the completion queue. Users may use fi_control to retrieve the 
 * underlying wait object associated with a CQ, in order to use it in other 
 * system calls. The following control commands are usable with a CQ.
 * 
 * FI_GETWAIT
 * This command allows the user to retrieve the low-level wait object associated
 * with the CQ. 
 * 
 * FI_GETWAITOBJ
 * This command allows the user to retrieve the type of wait object specified 
 * during CQ creation, through the CQ attributes.
 *
 * @param[in]	fid		Completion queue fid
 * @param[in]	command	Command of control operation to perform on CQ.
 * @param[out]	arg		Optional control argument. An address where a
 * pointer to the returned wait object will be written. This should be an 
 * ‘int *’ for FI_WAIT_FD.
 *
 * @return Returns 0 on success. On error, returns a negative fabric errno.
 */
int efa_cq_control(struct fid *fid, int command, void *arg)
{
	struct efa_cq *cq;
	int ret;

	cq = container_of(fid, struct efa_cq, util_cq.cq_fid.fid);
	switch(command) {
	case FI_GETWAIT:
		if (!cq->ibv_cq.channel || cq->wait_obj != FI_WAIT_FD) {
			ret = -FI_ENODATA;
			break;
		}
		*(int *) arg = cq->ibv_cq.channel->fd;
		ret = 0;
		break;
	case FI_GETWAITOBJ:
		*(enum fi_wait_obj *) arg = cq->wait_obj;
		ret = 0;
		break;
	default:
		ret = -FI_ENOSYS;
		break;
	}

	return ret;
}

struct fi_ops efa_cq_fi_ops = {
	.size = sizeof(struct fi_ops),
	.close = efa_cq_close,
	.bind = fi_no_bind,
	.control = efa_cq_control,
	.ops_open = fi_no_ops_open,
};


/**
 * @brief Create ibv_cq_ex by calling efadv_create_cq or ibv_create_cq_ex
 *
 * @param[in] attr Completion queue attributes
 * @param[in] ibv_ctx Pointer to ibv_context
 * @param[in,out] ibv_cq Pointer to efa_ibv_cq to be initialized
 * @param[in] efa_cq_init_attr Pointer to fi_efa_cq_init_attr containing attributes for efadv_create_cq
 * @return Return 0 on success, error code otherwise
 */
#if HAVE_EFADV_CQ_EX
int efa_cq_open_ibv_cq(struct fi_cq_attr *attr,
			struct ibv_context *ibv_ctx,
			struct efa_ibv_cq *ibv_cq,
			struct fi_efa_cq_init_attr *efa_cq_init_attr)
{
	int ret;

	ibv_cq->channel = NULL;
	if (attr->wait_obj != FI_WAIT_NONE) {
		ret = efa_cq_create_comp_channel(ibv_cq, ibv_ctx);
		if (ret)
			return ret;
	}

	struct ibv_cq_init_attr_ex init_attr_ex = {
		.cqe = attr->size ? attr->size : EFA_DEF_CQ_SIZE,
		.cq_context = NULL,
		.channel = ibv_cq->channel,
		.comp_vector = 0,
		/* EFA requires these values for wc_flags and comp_mask.
		 * See `efa_create_cq_ex` in rdma-core.
		 */
		.wc_flags = IBV_WC_STANDARD_FLAGS,
		.comp_mask = 0,
	};

	struct efadv_cq_init_attr efadv_cq_init_attr = {
		.comp_mask = 0,
		.wc_flags = EFADV_WC_EX_WITH_SGID,
	};

	ibv_cq->unsolicited_write_recv_enabled = false;
#if HAVE_CAPS_UNSOLICITED_WRITE_RECV
	if (efa_use_unsolicited_write_recv())
		efadv_cq_init_attr.wc_flags |= EFADV_WC_EX_WITH_IS_UNSOLICITED;
#endif

#if HAVE_CAPS_CQ_WITH_EXT_MEM_DMABUF
	if (efa_cq_init_attr->flags & FI_EFA_CQ_INIT_FLAGS_EXT_MEM_DMABUF) {
		efadv_cq_init_attr.flags = EFADV_CQ_INIT_FLAGS_EXT_MEM_DMABUF;
		efadv_cq_init_attr.ext_mem_dmabuf.buffer = efa_cq_init_attr->ext_mem_dmabuf.buffer;
		efadv_cq_init_attr.ext_mem_dmabuf.length = efa_cq_init_attr->ext_mem_dmabuf.length;
		efadv_cq_init_attr.ext_mem_dmabuf.offset = efa_cq_init_attr->ext_mem_dmabuf.offset;
		efadv_cq_init_attr.ext_mem_dmabuf.fd = efa_cq_init_attr->ext_mem_dmabuf.fd;
	}
#endif

	ibv_cq->data_path_direct_enabled = false;
	ibv_cq->ibv_cq_ex = efadv_create_cq(ibv_ctx, &init_attr_ex,
				     &efadv_cq_init_attr,
				     sizeof(efadv_cq_init_attr));

	if (!ibv_cq->ibv_cq_ex) {
#if HAVE_CAPS_CQ_WITH_EXT_MEM_DMABUF
		if (efa_cq_init_attr->flags & FI_EFA_CQ_INIT_FLAGS_EXT_MEM_DMABUF) {
			EFA_WARN(FI_LOG_CQ,
				 "efadv_create_cq failed on external memory. "
				 "errno: %s\n", strerror(errno));
			return (errno == EOPNOTSUPP) ? -FI_EOPNOTSUPP : -FI_EINVAL;
		}
#endif
		/* This could be due to old EFA kernel module versions */
		/* Fallback to ibv_create_cq_ex */
		return efa_cq_open_ibv_cq_with_ibv_create_cq_ex(
			&init_attr_ex, ibv_ctx, &ibv_cq->ibv_cq_ex, &ibv_cq->ibv_cq_ex_type);
	}

#if HAVE_CAPS_UNSOLICITED_WRITE_RECV
	if (efadv_cq_init_attr.wc_flags & EFADV_WC_EX_WITH_IS_UNSOLICITED)
		ibv_cq->unsolicited_write_recv_enabled = true;
#endif

	ibv_cq->ibv_cq_ex_type = EFADV_CQ;

#if HAVE_EFA_DATA_PATH_DIRECT
	#if HAVE_EFADV_CQ_ATTR_DB
		efa_data_path_direct_cq_initialize(ibv_cq);
	#else
		if (attr->wait_obj == FI_WAIT_NONE) {
			efa_data_path_direct_cq_initialize(ibv_cq);
		} else {
			ibv_cq->data_path_direct_enabled = false;
			EFA_INFO(FI_LOG_CQ, "Direct CQ data path is not "
					    "enabled with wait object.\n");
		}
	#endif
#endif

	return 0;
}
#else
int efa_cq_open_ibv_cq(struct fi_cq_attr *attr,
			struct ibv_context *ibv_ctx,
			struct efa_ibv_cq *ibv_cq,
			struct fi_efa_cq_init_attr *efa_cq_init_attr)
{
	int ret;

	ibv_cq->channel = NULL;
	if (attr->wait_obj != FI_WAIT_NONE) {
		ret = efa_cq_create_comp_channel(ibv_cq, ibv_ctx);
		if (ret)
			return ret;
	}

	struct ibv_cq_init_attr_ex init_attr_ex = {
		.cqe = attr->size ? attr->size : EFA_DEF_CQ_SIZE,
		.cq_context = NULL,
		.channel = ibv_cq->channel,
		.comp_vector = 0,
		/* EFA requires these values for wc_flags and comp_mask.
		 * See `efa_create_cq_ex` in rdma-core.
		 */
		.wc_flags = IBV_WC_STANDARD_FLAGS,
		.comp_mask = 0,
	};

	ibv_cq->data_path_direct_enabled = false;
	ibv_cq->unsolicited_write_recv_enabled = false;
	return efa_cq_open_ibv_cq_with_ibv_create_cq_ex(
		&init_attr_ex, ibv_ctx, &ibv_cq->ibv_cq_ex, &ibv_cq->ibv_cq_ex_type);
}
#endif

int efa_cq_open(struct fid_domain *domain_fid, struct fi_cq_attr *attr,
		struct fid_cq **cq_fid, void *context)
{
	struct efa_cq *cq;
	struct efa_domain *efa_domain;
	struct fi_efa_cq_init_attr efa_cq_init_attr = {0};
	struct fi_cq_attr tmp_attr;
	int err, retv;

	cq = calloc(1, sizeof(*cq));
	if (!cq) {
		EFA_WARN(FI_LOG_CQ, "Failed to allocate memory for CQ\n");
		return -FI_ENOMEM;
	}

	cq->poll_ibv_cq = efa_cq_poll_ibv_cq;

	/* efa uses its own implementation of wait objects for CQ */
	tmp_attr = *attr;
	tmp_attr.wait_obj = FI_WAIT_NONE;
	err = ofi_cq_init(&efa_prov, domain_fid, &tmp_attr, &cq->util_cq,
					  &efa_cq_progress, context);
	if (err) {
		EFA_WARN(FI_LOG_CQ, "Unable to create UTIL_CQ\n");
		goto err_free_cq;
	}

	efa_domain = container_of(cq->util_cq.domain, struct efa_domain,
				  util_domain);

	switch (attr->wait_obj) {
	case FI_WAIT_UNSPEC:
	case FI_WAIT_FD:
		cq->wait_obj = FI_WAIT_FD;
		break;
	case FI_WAIT_NONE:
		cq->wait_obj = attr->wait_obj;
		break;
	default:
		err = -FI_ENOSYS;
		goto err_free_util_cq;
	}

	switch (attr->format) {
	case FI_CQ_FORMAT_UNSPEC:
	case FI_CQ_FORMAT_CONTEXT:
		cq->entry_size = sizeof(struct fi_cq_entry);
		cq->read_entry = efa_cq_read_context_entry;
		break;
	case FI_CQ_FORMAT_MSG:
		cq->entry_size = sizeof(struct fi_cq_msg_entry);
		cq->read_entry = efa_cq_read_msg_entry;
		break;
	case FI_CQ_FORMAT_DATA:
		cq->entry_size = sizeof(struct fi_cq_data_entry);
		cq->read_entry = efa_cq_read_data_entry;
		break;
	default:
		err = -FI_ENOSYS;
		goto err_free_util_cq;
	}

	cq->wait_cond = attr->wait_cond;

	/* A persistent error data buffer that stages the cq_err_entry.err_data */
	cq->err_buf = malloc(EFA_ERROR_MSG_BUFFER_LENGTH);
	if (!cq->err_buf) {
		EFA_WARN(FI_LOG_CQ, "Failed to allocate memory for err_data buf in CQ\n");
		err = -FI_ENOMEM;
		goto err_free_util_cq;
	}

	err = efa_cq_open_ibv_cq(attr, efa_domain->device->ibv_ctx,
				 &cq->ibv_cq,
				 &efa_cq_init_attr);
	if (err) {
		EFA_WARN(FI_LOG_CQ, "Unable to create extended CQ: %s\n", fi_strerror(err));
		goto err_destroy_channel;
	}

	err = efa_cq_signal_init(cq);
	if (err)
		goto err_destroy_ibv_cq;

	*cq_fid = &cq->util_cq.cq_fid;
	(*cq_fid)->fid.fclass = FI_CLASS_CQ;
	(*cq_fid)->fid.context = context;
	(*cq_fid)->fid.ops = &efa_cq_fi_ops;
	/* Use bypass ops by default */
	(*cq_fid)->ops = &efa_cq_bypass_util_cq_ops;

	return 0;

err_destroy_ibv_cq:
	if (cq->ibv_cq.ibv_cq_ex)
		ibv_destroy_cq(ibv_cq_ex_to_cq(cq->ibv_cq.ibv_cq_ex));
err_destroy_channel:
	efa_cq_destroy_comp_channel(cq);
	free(cq->err_buf);
err_free_util_cq:
	retv = ofi_cq_cleanup(&cq->util_cq);
	if (retv)
		EFA_WARN(FI_LOG_CQ, "Unable to close util cq: %s\n",
			 fi_strerror(-retv));
err_free_cq:
	free(cq);
	return err;
}
