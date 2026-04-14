/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */

#ifndef _EFA_RDM_OPE_H
#define _EFA_RDM_OPE_H

#include "efa_rdm_pke.h"

#define EFA_RDM_IOV_LIMIT		(4)

/**
 * @brief EFA RDM operation entry (ope) type
 */
enum efa_rdm_ope_type {
	EFA_RDM_TXE = 1, /**< this ope is for an TX operation */
	EFA_RDM_RXE,     /**< this ope is for an RX operation */
};

/**
 * @brief EFA RDM operation entry (ope)'s state
 */
enum efa_rdm_ope_state {
	EFA_RDM_TXE_REQ = 1,	/**< txe sending REQ packet */
	EFA_RDM_OPE_SEND,	/**< ope sending data in progress */
	EFA_RDM_RXE_INIT,	/**< rxe ready to recv RTM */
	EFA_RDM_RXE_UNEXP,	/**< rxe unexp msg waiting for post recv */
	EFA_RDM_RXE_MATCHED,	/**< rxe matched with RTM */
	EFA_RDM_RXE_RECV,	/**< rxe large msg recv data pkts */
	EFA_RDM_OPE_ERR, /*< ope is in error state */
};

/**
 * @brief basic information of an atomic operation
 * used by all 3 types of atomic operations: fetch, compare and write
 */
struct efa_rdm_atomic_hdr {
	/* atomic_op is different from tx_op */
	uint32_t atomic_op;
	uint32_t datatype;
};

/**
 * @brief extra information that is not included in fi_msg_atomic
 * used by fetch atomic and compare atomic.
 *     resp stands for response
 *     comp stands for compare
 */
struct efa_rdm_atomic_ex {
	struct iovec resp_iov[EFA_RDM_IOV_LIMIT];
	int resp_iov_count;
	struct iovec comp_iov[EFA_RDM_IOV_LIMIT];
	int comp_iov_count;
	void *result_desc[EFA_RDM_IOV_LIMIT];
	/* compare_desc does not require persistence b/c it is only used to send the RTA */
	void **compare_desc;
};

/**
 * @brief how to copy data from bounce buffer to CUDA receive buffer
 */
enum efa_rdm_cuda_copy_method {
	EFA_RDM_CUDA_COPY_UNSPEC = 0,
	EFA_RDM_CUDA_COPY_BLOCKING,   /** gdrcopy or cudaMemcpy */
	EFA_RDM_CUDA_COPY_LOCALREAD   /** device driven copy by using local RDMA read */
};

/**
 * @brief EFA RDM operation entry (ope)
 *
 */
struct efa_rdm_ope {
	enum efa_rdm_ope_type type;

	struct efa_rdm_ep *ep;
	struct efa_rdm_peer *peer;

	uint32_t tx_id;
	uint32_t rx_id;
	uint32_t op;

	struct efa_rdm_atomic_hdr atomic_hdr;
	struct efa_rdm_atomic_ex atomic_ex;

	uint32_t msg_id;

	uint64_t tag;
	uint64_t ignore;

	int64_t window;

	uint64_t total_len;

	enum efa_rdm_ope_state state;
	int queued_ctrl_type;

	uint64_t fi_flags;

	/**
	 * @brief used by EFA provider to check status of an operation entry
	 * @details
	 * flags whose name started with EFA_RDM_TXE or EFA_RDM_RXE are
	 * applied (such as #EFA_RDM_TXE_DELIVERY_COMPLETE_REQUESTED)
	 * on internal_flags, and is not visible from user.
	 * This flag is different from #cq_entry.flags, which is
	 * applied to CQ entry's returned to user.
	 */
	uint16_t internal_flags;

	size_t iov_count;
	struct iovec iov[EFA_RDM_IOV_LIMIT];
	void *desc[EFA_RDM_IOV_LIMIT];
	struct fid_mr *mr[EFA_RDM_IOV_LIMIT];

	size_t rma_iov_count;
	struct fi_rma_iov rma_iov[EFA_RDM_IOV_LIMIT];

	struct fi_cq_tagged_entry cq_entry;

	/* entry is linked with ope_longcts_send_list in efa_domain */
	struct dlist_entry entry;

	/* ep_entry is linked to tx/rxe_list in efa_rdm_ep */
	struct dlist_entry ep_entry;

	/* queued_entry is linked with ope_queued_list in efa_domain */
	struct dlist_entry queued_entry;

	/* Queued packets due to TX queue full or RNR backoff */
	struct dlist_entry queued_pkts;


	/* linked with tx/rxe_list in rdm_peer */
	struct dlist_entry peer_entry;

	uint64_t bytes_runt;

	/* the following variables are for RX operation only */
	uint64_t bytes_received;
	uint64_t bytes_received_via_mulreq;
	uint64_t bytes_copied;
	uint64_t bytes_queued_blocking_copy;

#if ENABLE_DEBUG
	/* linked with ope_recv_list in efa_rdm_ep */
	struct dlist_entry pending_recv_entry;
#endif

	size_t efa_outstanding_tx_ops;

	struct efa_rdm_pke *unexp_pkt;
	char *atomrsp_data;
	enum efa_rdm_cuda_copy_method cuda_copy_method;
	/* the rxe_map that the rxe is ever inserted */
	struct efa_rdm_rxe_map *rxe_map;
	/* end of RX related variables */
	/* the following variables are for TX operation only */
	uint64_t bytes_acked;
	uint64_t bytes_sent;
	/* end of TX only variables */

	uint64_t bytes_read_completed;
	uint64_t bytes_read_submitted;
	uint64_t bytes_read_total_len;
	uint64_t bytes_read_offset;

	/* counters for rma writes */
	uint64_t bytes_write_completed;
	uint64_t bytes_write_submitted;
	uint64_t bytes_write_total_len;

	/* used by peer SRX ops */
	struct fi_peer_rx_entry *peer_rxe;

	/** the source packet entry of a local read operation */
	struct efa_rdm_pke *local_read_pkt_entry;
};

void efa_rdm_txe_construct(struct efa_rdm_ope *txe,
			   struct efa_rdm_ep *ep,
		      	   struct efa_rdm_peer *peer,
			   const struct fi_msg *msg,
			   uint32_t op, uint64_t flags);

void efa_rdm_txe_release(struct efa_rdm_ope *txe);

void efa_rdm_rxe_release(struct efa_rdm_ope *rxe);

void efa_rdm_rxe_release_internal(struct efa_rdm_ope *rxe);

/* The follow flags are applied to the internal_flags field
 * of an efa_rdm_ope*/

/**
 * @brief indicate an ope's receive has been cancel
 *
 * @todo: In future we will send RECV_CANCEL signal to sender,
 * to stop transmitting large message, this flag is also
 * used for fi_discard which has similar behavior.
 */
#define EFA_RDM_RXE_RECV_CANCEL		BIT_ULL(3)

/**
 * @brief Flag to tell if the transmission is using FI_DELIVERY_COMPLETE
 * protocols
 */
#define EFA_RDM_TXE_DELIVERY_COMPLETE_REQUESTED	BIT_ULL(6)

/**
 * @brief flag to tell if an ope encouter RNR when sending packets
 *
 * If an ope has this flag, it is on the ope_queued_list
 * of the endpoint.
 */
#define EFA_RDM_OPE_QUEUED_RNR BIT_ULL(9)

/**
 * @brief Flag to indicate an rxe has an EOR in flight
 *
 * In flag means the EOR has been sent or queued, and has not got send completion.
 * hence the rxe cannot be released
 */
#define EFA_RDM_RXE_EOR_IN_FLIGHT BIT_ULL(10)

/**
 * @brief flag to indicate a txe has already written an cq error entry for RNR
 *
 * This flag is used to prevent writing multiple cq error entries
 * for the same txe
 */
#define EFA_RDM_TXE_WRITTEN_RNR_CQ_ERR_ENTRY BIT_ULL(10)

/**
 * @brief flag to indicate an ope has queued ctrl packet,
 *
 * If this flag is on, the op_entyr is on the ope_queued_list
 * of the endpoint
 */
#define EFA_RDM_OPE_QUEUED_CTRL BIT_ULL(11)

/**
 * @brief flag to indicate an ope does not need to report completion to user
 *
 * This flag is used to by emulated injection and #efa_rdm_ep_trigger_handshake
 */
#define EFA_RDM_TXE_NO_COMPLETION	BIT_ULL(60)
/**
 * @brief flag to indicate an ope does not need to increase counter
 *
 * This flag is used to implement #efa_rdm_ep_trigger_handshake
 *
 */
#define EFA_RDM_TXE_NO_COUNTER		BIT_ULL(61)

/**
 * @brief flag to indicate an ope has queued read requests
 *
 * When this flag is on, the ope is on ope_queued_list
 * of the endpoint
 */
#define EFA_RDM_OPE_QUEUED_READ 	BIT_ULL(12)

/**
 * @brief flag to indicate that the ope corresponds to a long CTS transfer
 * that's used as a fallback for long read protocol
 *
 */
#define EFA_RDM_OPE_READ_NACK 	BIT_ULL(13)

/**
 * @brief flag to indicate that the ope was queued because it hasn't
 * made a handshake with the peer. Because this happens before
 * EFA provider makes any protocol selection, the progress engine
 * needs to determine the protocol from the peer status when
 * progressing the queued opes.
 */
#define EFA_RDM_OPE_QUEUED_BEFORE_HANDSHAKE	BIT_ULL(14)

/**
 * @brief flag to indicate that the ope was created
 * for internal operations, so it should not generate
 * any cq entry or err entry.
 * NOTICE: the ope->internal_flags is uint16_t, so
 * to introduce more bits for internal flags, the
 * internal_flags needs to be changed to uint32_t
 * or larger.
 */
#define EFA_RDM_OPE_INTERNAL			BIT_ULL(15)

#define EFA_RDM_OPE_QUEUED_FLAGS (EFA_RDM_OPE_QUEUED_RNR | EFA_RDM_OPE_QUEUED_CTRL | EFA_RDM_OPE_QUEUED_READ | EFA_RDM_OPE_QUEUED_BEFORE_HANDSHAKE)

void efa_rdm_ope_try_fill_desc(struct efa_rdm_ope *ope, int mr_iov_start, uint64_t access);

int efa_rdm_txe_prepare_to_be_read(struct efa_rdm_ope *txe,
				    struct fi_rma_iov *read_iov);

size_t efa_rdm_ope_mulreq_total_data_size(struct efa_rdm_ope *ope, int pkt_type);

size_t efa_rdm_txe_max_req_data_capacity(struct efa_rdm_ep *ep, struct efa_rdm_ope *txe, int pkt_type);

void efa_rdm_txe_handle_error(struct efa_rdm_ope *txe, int err, int prov_errno);

void efa_rdm_rxe_handle_error(struct efa_rdm_ope *rxe, int err, int prov_errno);

void efa_rdm_txe_report_completion(struct efa_rdm_ope *txe);

void efa_rdm_rxe_report_completion(struct efa_rdm_ope *rxe);

void efa_rdm_ope_handle_recv_completed(struct efa_rdm_ope *ope);

void efa_rdm_ope_handle_send_completed(struct efa_rdm_ope *ope);

int efa_rdm_ope_prepare_to_post_read(struct efa_rdm_ope *ope);

void efa_rdm_ope_prepare_to_post_write(struct efa_rdm_ope *ope);

int efa_rdm_ope_post_read(struct efa_rdm_ope *ope);

int efa_rdm_ope_post_remote_write(struct efa_rdm_ope *ope);

int efa_rdm_ope_post_remote_read_or_queue(struct efa_rdm_ope *ope);

int efa_rdm_rxe_post_local_read_or_queue(struct efa_rdm_ope *rxe,
					  size_t rx_data_offset,
					  struct efa_rdm_pke *pkt_entry,
					  char *pkt_data, size_t data_size);

ssize_t efa_rdm_ope_prepare_to_post_send(struct efa_rdm_ope *ope,
					 int pkt_type,
					 int *pkt_entry_cnt,
					 int *pkt_entry_data_size_vec);

ssize_t efa_rdm_ope_post_send(struct efa_rdm_ope *ope, int pkt_type);

ssize_t efa_rdm_ope_post_send_fallback(struct efa_rdm_ope *ope,
					   int pkt_type, ssize_t err);

ssize_t efa_rdm_ope_post_send_or_queue(struct efa_rdm_ope *ope, int pkt_type);

ssize_t efa_rdm_ope_repost_ope_queued_before_handshake(struct efa_rdm_ope *ope);

ssize_t efa_rdm_txe_prepare_local_read_pkt_entry(struct efa_rdm_ope *txe);

int efa_rdm_ope_process_queued_ope(struct efa_rdm_ope *ope, uint16_t flag);

#endif
