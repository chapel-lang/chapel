/* Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */
/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */

#ifndef _EFA_RDM_PKE_H
#define _EFA_RDM_PKE_H

#include <stdalign.h>
#include <uthash.h>
#include <ofi_mem.h>
#include <ofi_list.h>

#define EFA_RDM_PKE_IN_USE		BIT_ULL(0) /**< this packet entry is being used */
#define EFA_RDM_PKE_RNR_RETRANSMIT	BIT_ULL(1) /**< this packet entry encountered RNR and is being retransmitted*/
#define EFA_RDM_PKE_LOCAL_READ		BIT_ULL(2) /**< this packet entry is used as context of a local read operation */
#define EFA_RDM_PKE_DC_LONGCTS_DATA	BIT_ULL(3) /**< this DATA packet entry is used by a delivery complete LONGCTS send/write protocol*/
#define EFA_RDM_PKE_LOCAL_WRITE		BIT_ULL(4) /**< this packet entry is used as context of an RDMA Write to self */

#define EFA_RDM_PKE_ALIGNMENT		128

/**
 * @enum for packet entry allocation type
 */
enum efa_rdm_pke_alloc_type {
	EFA_RDM_PKE_FROM_EFA_TX_POOL = 1, /**< packet is allocated from `ep->efa_tx_pkt_pool` */
	EFA_RDM_PKE_FROM_EFA_RX_POOL,     /**< packet is allocated from `ep->efa_rx_pkt_pool` */
	EFA_RDM_PKE_FROM_UNEXP_POOL,      /**< packet is allocated from `ep->rx_unexp_pkt_pool` */
	EFA_RDM_PKE_FROM_OOO_POOL,	      /**< packet is allocated from `ep->rx_ooo_pkt_pool` */
	EFA_RDM_PKE_FROM_USER_BUFFER,     /**< packet is from user provided buffer` */
	EFA_RDM_PKE_FROM_READ_COPY_POOL,  /**< packet is allocated from `ep->rx_readcopy_pkt_pool` */
};

/**
 * @brief Packet entry
 * 
 * efa_rdm_pke (pke stands for packet entry) is used the following occassions:
 * 
 * First, it is used as the work request ID for the request EFA provider posted to EFA
 * device via rdma-core:
 * 
 * For each request EFA provider submits to EFA device, it will allocate a packet entry.
 * 
 * When the request was submitted to rdma-core, the pointer of the packet entry will be used as work
 * request ID (`wr_id`).
 * 
 * When the request was completed, rdma-core return a completion, with the "wr_id"
 * in it, so EFA provder knows which request has completed.
 * 
 * Sometimes, the completion can be a Receiver Not Ready (RNR) error completion.
 * In that case the packet entry will be queued and resubmitted. For the resubmission,
 * the packet entry must contain all the information of the request.
 * 
 * EFA device (rdma-core) supported request types are:
 *  send and receive (by all EFA device)
 *  read and write (by certain EFA device)
 * 
 * a send/read/write request uses a packet entry allocated from endpoint's efa_tx_pkt_pool.
 * a read request uses a packet entry allocated from efa_rx_pkt_pool. Both pool's memories
 * are registeredd with efa device, the registration is stored in
 * the "mr" field of the packet entry.
 * 
 * Second, packet entries can be used to store received packet entries that is
 * unexpected or out-of-order. This is because the efa_rx_pkt_pool's size is fixed,
 * therefore it cannot be used to hold unexpected/out-of-order packets. When an unexpected/out-of-order
 * packet is received, a new packet entry will be cloned from unexpected/ooo_pkt_pool.
 * The old packet will be released then reposted to EFA device or SHM. The new packet
 * (allocated from unexpected/ooo_pkt_pool)'s memory is not registered
 * 
 * Finally, packet entries can be used to support local read copy. Local read copy means
 * to copy data from a packet entry to HMEM receive buffer through EFA device's read capability.
 * Local require a packet entry's memory to be registered with device. If the packet entry's memory
 * is not registered (when it is unexpected or out-of-order). A new packet entry will be cloned
 * using endpoint's read_copy_pkt_pool, whose memory was registered.
 */
struct efa_rdm_pke {
	/**
	 * entry to the linked list of outstanding/queued packet entries
	 *
	 * `entry` is used for sending only.
	 * It is either linked to `peer->outstanding_tx_pkts` (after a packet has been successfully sent, but it get a completion),
	 * or linked to `ope->queued_pkts` (after it encountered RNR error completion).
	 */
	struct dlist_entry entry;
#if ENABLE_DEBUG
	/** @brief entry to a linked list of posted buf list */
	struct dlist_entry dbg_entry;
#endif
	/** @brief pointer to #efa_rdm_ep */
	struct efa_rdm_ep *ep;

	/** @brief pointer to #efa_rdm_ope */
	struct efa_rdm_ope *ope;

	/** @brief number of bytes sent/received over wire */
	size_t pkt_size;

	/**
	 * @brief memory registration
	 *
	 * @details
	 * If this packet is used by EFA device, `mr` the memory registration of wiredata over the EFA device.
	 * If this packet is used by SHM, `mr` is NULL because SHM does not require memory registration
	 *
	 * @todo
	 * Use type `struct ibv_mr` instead of `struct fid_mr` for this field
	 */
	struct fid_mr *mr;
	/**
	 * @brief peer address
	 *
	 * @details
	 * When sending a packet, `addr` will be provided by application and it cannot be FI_ADDR_NOTAVAIL.
	 * However, after a packet is sent, application can remove a peer by calling fi_av_remove().
	 * When removing the peering, `addr` will be set to FI_ADDR_NOTAVAIL. Later, when device report
	 * completion for such a TX packet, the TX completion will be ignored.
	 *
	 * When receiving a packet, lower device will set `addr`. If the sender's address is not in
	 * address vector (AV), `lower device will set `addr` to FI_ADDR_NOTAVAIL. This can happen in
	 * two scenarios:
	 *
	 * 1. There has been no prior communication with the peer. In this case, the packet should have
	 *    peer's raw address in the header, and progress engine will insert the raw address into
	 *    address vector, and update `addr`.
	 *
	 * 2. This packet is from a peer whose address has been removed from AV. In this case, the
	 *    recived packet will be ignored because all resources associated with peer has been released.
	 */
	fi_addr_t addr;

	/** @brief indicate where the memory of this packet entry reside */
	enum efa_rdm_pke_alloc_type alloc_type;

	/** 
	 * @brief flags indicating the status of the packet entry
	 * 
	 * @details
	 * Possible flags include  #EFA_RDM_PKE_IN_USE #EFA_RDM_PKE_RNR_RETRANSMIT,
	 * #EFA_RDM_PKE_LOCAL_READ, and #EFA_RDM_PKE_DC_LONGCTS_DATA
	 */
	uint32_t flags;

	/**
	 * @brief link multiple MEDIUM/RUNTREAD RTM with same
	 * message ID together
	 *
	 * @details
	 * used on receiver side only
	 */
	struct efa_rdm_pke *next;

	/**
	 * @brief a buffer that contains actual user data that is going over wire
	 * 
	 * @details
	 * "payload" points to either a location inside user's buffer,
	 * (when user's buffer is registered with EFA device), or
	 * a location in "wiredata" (where user data has been copied to).
	 * The EFA provider tries its best to avoid copy, but copy is not
	 * always avoidable.
	 */
	char *payload;

	/**
	 * @brief memory regstration for user buffer
	 * 
	 * @details
	 * payload_mr is same as mr, when payload is pointing to
	 * a location inside wiredata.
	 */
	struct fid_mr *payload_mr;

	/**
	 * @brief size of payload buffer
	 * 
	 */
	size_t payload_size;

	/** @brief buffer that contains data that is going over wire
	 *
	 * @details
	 * wiredata consists of 3 parts:
	 *
	 * 1. Packet header. All packet entries have a packet header,
	 *    except the packet entry allocated from readcopy_pool.
	 * 
	 * 2. User buffer infomation, which presents
	 *    only for LONGREAD and RUNTREAD RTM packets.
	 * 
	 * 3. User data, which presents when:
	 *    a) pakcet is an outging (TX) packet, and EFA device
	 *       is not able to send data directory from user's buffer
	 *       (thus data has been copied to wiredata).
	 *    b) packet is an incoming (RX) packet.
	 */
	_Alignas(EFA_RDM_PKE_ALIGNMENT) char wiredata[0];
};

#if defined(static_assert) && defined(__x86_64__)
static_assert(sizeof (struct efa_rdm_pke) % EFA_RDM_PKE_ALIGNMENT == 0, "efa_rdm_pke alignment check");
#endif

struct efa_rdm_ep;

struct efa_rdm_ope;

struct efa_rdm_pke *efa_rdm_pke_init_prefix(struct efa_rdm_ep *ep,
						const struct fi_msg *posted_buf,
						struct ofi_bufpool *pkt_pool);

struct efa_rdm_pke *efa_rdm_pke_alloc(struct efa_rdm_ep *ep,
				      struct ofi_bufpool *pkt_pool,
				      enum efa_rdm_pke_alloc_type alloc_type);

void efa_rdm_pke_release_tx(struct efa_rdm_pke *pkt_entry);

void efa_rdm_pke_release_rx(struct efa_rdm_pke *pkt_entry);

void efa_rdm_pke_release(struct efa_rdm_pke *pkt_entry);

void efa_rdm_pke_append(struct efa_rdm_pke *dst,
			struct efa_rdm_pke *src);

struct efa_rdm_pke *efa_rdm_pke_clone(struct efa_rdm_pke *src,
				      struct ofi_bufpool *pkt_pool,
				      enum efa_rdm_pke_alloc_type alloc_type
				      );

struct efa_rdm_pke *efa_rdm_pke_get_unexp(struct efa_rdm_pke **pkt_entry_ptr);

ssize_t efa_rdm_pke_sendv(struct efa_rdm_pke **pkt_entry_vec,
			  int pkt_entry_cnt);

int efa_rdm_pke_read(struct efa_rdm_pke *pkt_entry,
		     void *local_buf, size_t len, void *desc,
		     uint64_t remote_buf, size_t remote_key);

ssize_t efa_rdm_pke_recvv(struct efa_rdm_pke **pke_vec,
			  int pke_cnt);

int efa_rdm_pke_write(struct efa_rdm_pke *pkt_entry);
#endif
