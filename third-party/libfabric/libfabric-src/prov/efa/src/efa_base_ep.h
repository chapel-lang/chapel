/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */

#ifndef EFA_BASE_EP_H
#define EFA_BASE_EP_H

#include <arpa/inet.h>
#include <infiniband/verbs.h>
#include <infiniband/efadv.h>

#include "ofi.h"
#include "ofi_util.h"
#include "rdm/efa_rdm_protocol.h"

struct efa_qp {
	struct ibv_qp *ibv_qp;
	struct ibv_qp_ex *ibv_qp_ex;
	struct efa_base_ep *base_ep;
	uint32_t qp_num;
	uint32_t qkey;
};

struct efa_av;

struct efa_recv_wr {
	/** @brief Work request struct used by rdma-core */
	struct ibv_recv_wr wr;

	/** @brief Scatter gather element array
	 *
	 * @details
	 * EFA device supports a maximum of 2 iov/SGE
	 * For receive, we only use 1 SGE
	 */
	struct ibv_sge sge[1];
};

struct efa_base_ep {
	struct util_ep util_ep;
	struct efa_domain *domain;
	struct efa_qp *qp;
	struct efa_av *av;
	struct fi_info *info;
	size_t rnr_retry;
	struct efa_ep_addr src_addr;
	struct ibv_ah *self_ah;

	bool util_ep_initialized;
	bool efa_qp_enabled;

	struct ibv_send_wr xmit_more_wr_head;
	struct ibv_send_wr *xmit_more_wr_tail;
	struct ibv_recv_wr recv_more_wr_head;
	struct ibv_recv_wr *recv_more_wr_tail;
	struct efa_recv_wr *efa_recv_wr_vec;
};

int efa_base_ep_bind_av(struct efa_base_ep *base_ep, struct efa_av *av);

int efa_base_ep_destruct(struct efa_base_ep *base_ep);

int efa_base_ep_enable(struct efa_base_ep *base_ep);

int efa_base_ep_construct(struct efa_base_ep *base_ep,
			  struct fid_domain* domain_fid,
			  struct fi_info *info,
			  ofi_ep_progress_func progress,
			  void *context);

int efa_base_ep_getname(fid_t fid, void *addr, size_t *addrlen);

int efa_qp_create(struct efa_qp **qp, struct ibv_qp_init_attr_ex *init_attr_ex);

int efa_base_ep_create_qp(struct efa_base_ep *base_ep,
			  struct ibv_qp_init_attr_ex *init_attr_ex);

void efa_base_ep_close_util_ep(struct efa_base_ep *base_ep);

int efa_base_ep_destruct_qp(struct efa_base_ep *base_ep);

bool efa_qp_support_op_in_order_aligned_128_bytes(struct efa_qp *qp,
						       enum ibv_wr_opcode op);

void efa_base_ep_write_eq_error(struct efa_base_ep *ep,
				ssize_t err,
				ssize_t prov_errno);

#endif
