/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */

#ifndef EFA_BASE_EP_H
#define EFA_BASE_EP_H

#include <arpa/inet.h>
#include <infiniband/verbs.h>
#include <infiniband/efadv.h>

#include "ofi.h"
#include "ofi_util.h"
#include "efa_av.h"
#include "rdm/efa_rdm_protocol.h"
#include "efa_data_path_direct_structs.h"

#define EFA_QP_DEFAULT_SERVICE_LEVEL 0
#define EFA_QP_LOW_LATENCY_SERVICE_LEVEL 8
#define EFA_ERROR_MSG_BUFFER_LENGTH 1024

/* Default rnr_retry for efa-rdm ep.
 * If first attempt to send a packet failed,
 * this value controls how many times firmware
 * retries the send before it report an RNR error
 * (via rdma-core error cq entry).
 * The valid number is from
 *      0 (no retry)
 * to
 *      EFA_RNR_INFINITY_RETRY (retry infinitely)
 */
#define EFA_RDM_DEFAULT_RNR_RETRY	(3)
/**
 * Infinite retry.
 * NOTICE: this is the default rnr_retry
 * mode for SRD qp. So modifying qp_attr.rnr_retry
 * to this value has the same behavior as
 * not modifying qp's rnr_retry attribute
 */
#define EFA_RNR_INFINITE_RETRY		(7)

#define efa_rx_flags(efa_base_ep) ((efa_base_ep)->util_ep.rx_op_flags)
#define efa_tx_flags(efa_base_ep) ((efa_base_ep)->util_ep.tx_op_flags)

struct efa_qp {
	struct ibv_qp *ibv_qp;
	struct ibv_qp_ex *ibv_qp_ex;
	struct efa_base_ep *base_ep;
	uint32_t qp_num;
	uint32_t qkey;
	bool data_path_direct_enabled;
#if HAVE_EFA_DATA_PATH_DIRECT
	struct efa_data_path_direct_qp data_path_direct_qp;
#endif
	bool unsolicited_write_recv_enabled;
};

struct efa_av;

struct efa_recv_wr {
	/** @brief Work request struct used by rdma-core */
	struct ibv_recv_wr wr;

	/** @brief Scatter gather element array
	 *
	 * @details
	 * EFA device supports a maximum of 2 iov/SGE
	 */
	struct ibv_sge sge[2];
};

struct efa_base_ep {
	struct util_ep util_ep;
	struct efa_domain *domain;
	struct efa_qp *qp;
	struct efa_av *av;
	struct fi_info *info;
	size_t rnr_retry;
	struct efa_ep_addr src_addr;
	struct efa_ah *self_ah;

	bool util_ep_initialized;
	bool efa_qp_enabled;
	bool is_wr_started;

	struct efa_recv_wr *efa_recv_wr_vec;
	size_t recv_wr_index;

	size_t max_msg_size;		/**< #FI_OPT_MAX_MSG_SIZE */
	size_t max_rma_size;		/**< #FI_OPT_MAX_RMA_SIZE */
	size_t inject_msg_size;		/**< #FI_OPT_INJECT_MSG_SIZE */
	size_t inject_rma_size;		/**< #FI_OPT_INJECT_RMA_SIZE */

	/* Only used by RDM ep type */
	struct efa_qp *user_recv_qp; /* Separate qp to receive pkts posted by users */
	struct efa_recv_wr *user_recv_wr_vec;
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

int efa_ep_open(struct fid_domain *domain_fid, struct fi_info *user_info,
		struct fid_ep **ep_fid, void *context);

int efa_qp_create(struct efa_qp **qp, struct ibv_qp_init_attr_ex *init_attr_ex,
		   uint32_t tclass, bool enable_unsolicited_write_recv);

void efa_qp_destruct(struct efa_qp *qp);

void efa_base_ep_close_util_ep(struct efa_base_ep *base_ep);

int efa_base_ep_destruct_qp(struct efa_base_ep *base_ep);

bool efa_qp_support_op_in_order_aligned_128_bytes(struct efa_qp *qp,
						       enum ibv_wr_opcode op);

void efa_base_ep_write_eq_error(struct efa_base_ep *ep,
				ssize_t err,
				ssize_t prov_errno);

const char *efa_base_ep_raw_addr_str(struct efa_base_ep *base_ep, char *buf,
				     size_t *buflen);

struct efa_ep_addr *efa_base_ep_get_peer_raw_addr(struct efa_base_ep *base_ep,
						  fi_addr_t addr);

const char *efa_base_ep_get_peer_raw_addr_str(struct efa_base_ep *base_ep,
					      fi_addr_t addr, char *buf,
					      size_t *buflen);

struct efa_cq *efa_base_ep_get_tx_cq(struct efa_base_ep *ep);

struct efa_cq *efa_base_ep_get_rx_cq(struct efa_base_ep *ep);

int efa_base_ep_check_qp_in_order_aligned_128_bytes(struct efa_base_ep *base_ep,
						   enum ibv_wr_opcode op_code);

int efa_base_ep_insert_cntr_ibv_cq_poll_list(struct efa_base_ep *ep);

void efa_base_ep_remove_cntr_ibv_cq_poll_list(struct efa_base_ep *ep);

int efa_base_ep_create_and_enable_qp(struct efa_base_ep *ep, bool create_user_recv_qp);

#if ENABLE_DEBUG
void efa_ep_addr_print(char *prefix, struct efa_ep_addr *addr);
#endif

#endif
