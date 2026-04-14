/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */

/*
 * Unit test stub versions of EFA data path operations
 * 
 * This file provides stub implementations of the data path operations
 * specifically for unit testing. These functions are needed because
 * static inline functions cannot have their addresses taken for
 * function pointers in the unit test mock structure.
 */

#include <errno.h>
#include <infiniband/verbs.h>
#include "efa_cq.h"
#include "efa_base_ep.h"

/* QP wrapper functions - unit test stubs */
int efa_qp_post_recv(struct efa_qp *qp, struct ibv_recv_wr *wr, struct ibv_recv_wr **bad)
{
	return 0;
}

int efa_qp_wr_complete(struct efa_qp *efaqp)
{
	return 0;
}

void efa_qp_wr_rdma_read(struct efa_qp *efaqp, uint32_t rkey, uint64_t remote_addr)
{
}

void efa_qp_wr_rdma_write(struct efa_qp *efaqp, uint32_t rkey, uint64_t remote_addr)
{
}

void efa_qp_wr_rdma_write_imm(struct efa_qp *efaqp, uint32_t rkey, uint64_t remote_addr, __be32 imm_data)
{
}

void efa_qp_wr_send(struct efa_qp *efaqp)
{
}

void efa_qp_wr_send_imm(struct efa_qp *efaqp, __be32 imm_data)
{
}

void efa_qp_wr_set_inline_data_list(struct efa_qp *efaqp, size_t num_buf, const struct ibv_data_buf *buf_list)
{
}

void efa_qp_wr_set_sge_list(struct efa_qp *efaqp, size_t num_sge, const struct ibv_sge *sg_list)
{
}

void efa_qp_wr_set_ud_addr(struct efa_qp *efaqp, struct efa_ah *ah, uint32_t remote_qpn, uint32_t remote_qkey)
{
}

void efa_qp_wr_start(struct efa_qp *efaqp)
{
}

/* CQ wrapper functions - unit test stubs */
int efa_ibv_cq_start_poll(struct efa_ibv_cq *ibv_cq, struct ibv_poll_cq_attr *attr)
{
	return ENOENT;
}

int efa_ibv_cq_next_poll(struct efa_ibv_cq *ibv_cq)
{
	return ENOENT;
}

enum ibv_wc_opcode efa_ibv_cq_wc_read_opcode(struct efa_ibv_cq *ibv_cq)
{
	return 0;
}

void efa_ibv_cq_end_poll(struct efa_ibv_cq *ibv_cq)
{
}

uint32_t efa_ibv_cq_wc_read_qp_num(struct efa_ibv_cq *ibv_cq)
{
	return 0;
}

uint32_t efa_ibv_cq_wc_read_vendor_err(struct efa_ibv_cq *ibv_cq)
{
	return 0;
}

uint32_t efa_ibv_cq_wc_read_src_qp(struct efa_ibv_cq *ibv_cq)
{
	return 0;
}

uint32_t efa_ibv_cq_wc_read_slid(struct efa_ibv_cq *ibv_cq)
{
	return 0;
}

uint32_t efa_ibv_cq_wc_read_byte_len(struct efa_ibv_cq *ibv_cq)
{
	return 0;
}

unsigned int efa_ibv_cq_wc_read_wc_flags(struct efa_ibv_cq *ibv_cq)
{
	return 0;
}

__be32 efa_ibv_cq_wc_read_imm_data(struct efa_ibv_cq *ibv_cq)
{
	return 0;
}

bool efa_ibv_cq_wc_is_unsolicited(struct efa_ibv_cq *ibv_cq)
{
	return false;
}


int efa_ibv_cq_wc_read_sgid(struct efa_ibv_cq *ibv_cq, union ibv_gid *sgid)
{
	return ENOSYS;
}

int efa_ibv_get_cq_event(struct efa_ibv_cq *ibv_cq, void **cq_context)
{
	return 0;
}

int efa_ibv_req_notify_cq(struct efa_ibv_cq *ibv_cq, int solicited_only)
{
	return 0;
}
