/*
 * Copyright (c) 2019 Amazon.com, Inc. or its affiliates.
 * All rights reserved.
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

#if HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#ifndef _RXR_H_
#define _RXR_H_

#include <pthread.h>
#include <rdma/fabric.h>
#include <rdma/fi_atomic.h>
#include <rdma/fi_cm.h>
#include <rdma/fi_domain.h>
#include <rdma/fi_endpoint.h>
#include <rdma/fi_eq.h>
#include <rdma/fi_errno.h>
#include <rdma/fi_rma.h>
#include <rdma/fi_tagged.h>
#include <rdma/fi_trigger.h>

#include <ofi.h>
#include <ofi_iov.h>
#include <ofi_proto.h>
#include <ofi_enosys.h>
#include <ofi_rbuf.h>
#include <ofi_list.h>
#include <ofi_util.h>
#include <ofi_tree.h>
#include <uthash.h>
#include <ofi_recvwin.h>
#include <ofi_perf.h>
#include <ofi_hmem.h>

#include "rxr_pkt_entry.h"
#include "rxr_pkt_type.h"

/*
 * EFA support interoperability between protocol version 4 and above,
 * and version 4 is considered the base version.
 */
#define RXR_BASE_PROTOCOL_VERSION	(4)
#define RXR_CUR_PROTOCOL_VERSION	(4)
#define RXR_NUM_PROTOCOL_VERSION	(RXR_CUR_PROTOCOL_VERSION - RXR_BASE_PROTOCOL_VERSION + 1)
#define RXR_MAX_PROTOCOL_VERSION	(100)

#define RXR_FI_VERSION		OFI_VERSION_LATEST

#define RXR_IOV_LIMIT		(4)

#ifdef ENABLE_EFA_POISONING
extern const uint32_t rxr_poison_value;
static inline void rxr_poison_mem_region(uint32_t *ptr, size_t size)
{
	int i;

	for (i = 0; i < size / sizeof(rxr_poison_value); i++)
		memcpy(ptr + i, &rxr_poison_value, sizeof(rxr_poison_value));
}
#endif

/*
 * Set alignment to x86 cache line size.
 */
#define RXR_BUF_POOL_ALIGNMENT	(64)

/*
 * will add following parameters to env variable for tuning
 */
#define RXR_RECVWIN_SIZE		(16384)
#define RXR_DEF_CQ_SIZE			(8192)
#define RXR_REMOTE_CQ_DATA_LEN		(8)

/* the default value for rxr_env.rnr_backoff_wait_time_cap */
#define RXR_DEFAULT_RNR_BACKOFF_WAIT_TIME_CAP	(1000000)

/*
 * the maximum value for rxr_env.rnr_backoff_wait_time_cap
 * Because the backoff wait time is multiplied by 2 when
 * RNR is encountered, its value must be < INT_MAX/2.
 * Therefore, its cap must be < INT_MAX/2 too.
 */
#define RXR_MAX_RNR_BACKOFF_WAIT_TIME_CAP	(INT_MAX/2 - 1)

/* bounds for random RNR backoff timeout */
#define RXR_RAND_MIN_TIMEOUT		(40)
#define RXR_RAND_MAX_TIMEOUT		(120)

/* bounds for flow control */
#define RXR_DEF_MAX_RX_WINDOW		(128)
#define RXR_DEF_MAX_TX_CREDITS		(64)
#define RXR_DEF_MIN_TX_CREDITS		(32)

/*
 * maximum time (microseconds) we will allow available_bufs for large msgs to
 * be exhausted
 */
#define RXR_AVAILABLE_DATA_BUFS_TIMEOUT	(5000000)

#if ENABLE_DEBUG
#define RXR_TX_PKT_DBG_SIZE	(16384)
#define RXR_RX_PKT_DBG_SIZE	(16384)
#endif

/*
 * Based on size of tx_id and rx_id in headers, can be arbitrary once those are
 * removed.
 */
#define RXR_MAX_RX_QUEUE_SIZE (UINT32_MAX)
#define RXR_MAX_TX_QUEUE_SIZE (UINT32_MAX)

/*
 * The maximum supported source address length in bytes
 */
#define RXR_MAX_NAME_LENGTH	(32)

/*
 * RxR specific flags that are sent over the wire.
 */
#define RXR_TAGGED		BIT_ULL(0)
#define RXR_REMOTE_CQ_DATA	BIT_ULL(1)
#define RXR_REMOTE_SRC_ADDR	BIT_ULL(2)

/*
 * TODO: In future we will send RECV_CANCEL signal to sender,
 * to stop transmitting large message, this flag is also
 * used for fi_discard which has similar behavior.
 */
#define RXR_RECV_CANCEL		BIT_ULL(3)

/*
 * Flags to tell if the rx_entry is tracking FI_MULTI_RECV buffers
 */
#define RXR_MULTI_RECV_POSTED	BIT_ULL(4)
#define RXR_MULTI_RECV_CONSUMER	BIT_ULL(5)

/*
 * Flag to tell if the transmission is using FI_DELIVERY_COMPLETE
 * protocols
 */

#define RXR_DELIVERY_COMPLETE_REQUESTED	BIT_ULL(6)

/*
 * Flag to tell if the sender
 * receives the receipt packet for the tx_entry.
 */
#define RXR_RECEIPT_RECEIVED BIT_ULL(7)

/*
 * Flag to tell that
 * long message protocol is used
 */
#define RXR_LONGCTS_PROTOCOL BIT_ULL(8)

#define RXR_TX_ENTRY_QUEUED_RNR BIT_ULL(9)

#define RXR_RX_ENTRY_QUEUED_RNR BIT_ULL(9)

/*
 * OFI flags
 * The 64-bit flag field is used as follows:
 * 1-grow up    common (usable with multiple operations)
 * 59-grow down operation specific (used for single call/class)
 * 60 - 63      provider specific
 */
#define RXR_NO_COMPLETION	BIT_ULL(60)
#define RXR_NO_COUNTER		BIT_ULL(61)

/*
 * RM flags
 */
#define RXR_RM_TX_CQ_FULL	BIT_ULL(0)
#define RXR_RM_RX_CQ_FULL	BIT_ULL(1)

#define RXR_MTU_MAX_LIMIT	BIT_ULL(15)



extern struct fi_info *shm_info;

extern struct fi_provider *lower_efa_prov;
extern struct fi_provider rxr_prov;
extern struct fi_info rxr_info;
extern struct rxr_env rxr_env;
extern struct fi_fabric_attr rxr_fabric_attr;
extern struct util_prov rxr_util_prov;
extern struct efa_ep_addr *local_efa_addr;

struct rxr_env {
	int rx_window_size;
	int tx_min_credits;
	int tx_max_credits;
	int tx_queue_size;
	int use_device_rdma;
	int use_zcpy_rx;
	int zcpy_rx_seed;
	int enable_shm_transfer;
	int shm_av_size;
	int shm_max_medium_size;
	int recvwin_size;
	int ooo_pool_chunk_size;
	int unexp_pool_chunk_size;
	int readcopy_pool_size;
	int atomrsp_pool_size;
	int cq_size;
	size_t max_memcpy_size;
	size_t mtu_size;
	size_t tx_size;
	size_t rx_size;
	size_t tx_iov_limit;
	size_t rx_iov_limit;
	int rx_copy_unexp;
	int rx_copy_ooo;
	int rnr_backoff_wait_time_cap; /* unit is us */
	int rnr_backoff_initial_wait_time; /* unit is us */
	size_t efa_cq_read_size;
	size_t shm_cq_read_size;
	size_t efa_max_medium_msg_size;
	size_t efa_min_read_msg_size;
	size_t efa_min_read_write_size;
	size_t efa_read_segment_size;
	/* If first attempt to send a packet failed,
	 * this value controls how many times firmware
	 * retries the send before it report an RNR error
	 * (via rdma-core error cq entry).
	 *
	 * The valid number is from
	 *      0 (no retry)
	 * to
	 *      EFA_RNR_INFINITY_RETRY (retry infinitely)
	 */
	int rnr_retry;
};

enum rxr_lower_ep_type {
	EFA_EP = 1,
	SHM_EP,
};

enum rxr_x_entry_type {
	RXR_TX_ENTRY = 1,
	RXR_RX_ENTRY,
	RXR_READ_ENTRY,
};

enum rxr_tx_comm_type {
	RXR_TX_FREE = 0,	/* tx_entry free state */
	RXR_TX_REQ,		/* tx_entry sending REQ packet */
	RXR_TX_SEND,		/* tx_entry sending data in progress */
	RXR_TX_QUEUED_SHM_RMA,	/* tx_entry was unable to send RMA operations over shm provider */
	RXR_TX_QUEUED_CTRL,	/* tx_entry was unable to send ctrl packet */
};

enum rxr_rx_comm_type {
	RXR_RX_FREE = 0,	/* rx_entry free state */
	RXR_RX_INIT,		/* rx_entry ready to recv RTM */
	RXR_RX_UNEXP,		/* rx_entry unexp msg waiting for post recv */
	RXR_RX_MATCHED,		/* rx_entry matched with RTM */
	RXR_RX_RECV,		/* rx_entry large msg recv data pkts */
	RXR_RX_QUEUED_CTRL,	/* rx_entry encountered error when sending control
				   it is in rxr_ep->rx_queued_entry_list, progress
				   engine will resend the ctrl packet */
	RXR_RX_WAIT_READ_FINISH, /* rx_entry wait for send to finish, FI_READ */
	RXR_RX_WAIT_ATOMRSP_SENT, /* rx_entry wait for atomrsp packet sent completion */
};

#define RXR_PEER_REQ_SENT BIT_ULL(0) /* sent a REQ to the peer, peer should send a handshake back */
#define RXR_PEER_HANDSHAKE_SENT BIT_ULL(1) /* a handshake packet has been sent to a peer */
#define RXR_PEER_HANDSHAKE_RECEIVED BIT_ULL(2)
#define RXR_PEER_IN_BACKOFF BIT_ULL(3) /* peer is in backoff, not allowed to send */
/*
 * FI_EAGAIN error was encountered when sending handsahke to this peer,
 * the peer was put in rxr_ep->handshake_queued_peer_list.
 * Progress engine will retry sending handshake.
 */
#define RXR_PEER_HANDSHAKE_QUEUED      BIT_ULL(5)

struct rxr_fabric {
	struct util_fabric util_fabric;
	struct fid_fabric *lower_fabric;
	struct fid_fabric *shm_fabric;
#ifdef RXR_PERF_ENABLED
	struct ofi_perfset perf_set;
#endif
};

#define RXR_MAX_NUM_PROTOCOLS (RXR_MAX_PROTOCOL_VERSION - RXR_BASE_PROTOCOL_VERSION + 1)

struct rdm_peer {
	bool is_self;			/* self flag */
	bool is_local;			/* local/remote peer flag */
	fi_addr_t efa_fiaddr;		/* fi_addr_t addr from efa provider */
	fi_addr_t shm_fiaddr;		/* fi_addr_t addr from shm provider */
	struct rxr_robuf robuf;		/* tracks expected msg_id on rx */
	uint32_t prev_qkey;		/* each peer has unique gid+qpn. the qkey can change */
	uint32_t next_msg_id;		/* sender's view of msg_id */
	uint32_t flags;
	uint32_t maxproto;		/* maximum supported protocol version by this peer */
	uint64_t features[RXR_MAX_NUM_PROTOCOLS]; /* the feature flag for each version */
	size_t efa_outstanding_tx_ops;	/* tracks outstanding tx ops to this peer on EFA device */
	size_t shm_outstanding_tx_ops;  /* tracks outstanding tx ops to this peer on SHM */
	struct dlist_entry outstanding_tx_pkts; /* a list of outstanding tx pkts to the peer */
	uint16_t tx_credits;		/* available send credits */
	uint16_t rx_credits;		/* available credits to allocate */
	uint64_t rnr_backoff_begin_ts;	/* timestamp for RNR backoff period begin */
	uint64_t rnr_backoff_wait_time;	/* how long the RNR backoff period last */
	int rnr_queued_pkt_cnt;		/* queued RNR packet count */
	struct dlist_entry rnr_backoff_entry;	/* linked to rxr_ep peer_backoff_list */
	struct dlist_entry handshake_queued_entry; /* linked with rxr_ep->handshake_queued_peer_list */
	struct dlist_entry rx_unexp_list; /* a list of unexpected untagged rx_entry for this peer */
	struct dlist_entry rx_unexp_tagged_list; /* a list of unexpected tagged rx_entry for this peer */
	struct dlist_entry tx_entry_list; /* a list of tx_entry related to this peer */
	struct dlist_entry rx_entry_list; /* a list of rx_entry relased to this peer */
};

struct rxr_queued_ctrl_info {
	int type;
	int inject;
};

struct rxr_atomic_hdr {
	/* atomic_op is different from tx_op */
	uint32_t atomic_op;
	uint32_t datatype;
};

/* extra information that is not included in fi_msg_atomic
 * used by fetch atomic and compare atomic.
 *     resp stands for response
 *     comp stands for compare
 */
struct rxr_atomic_ex {
	struct iovec resp_iov[RXR_IOV_LIMIT];
	int resp_iov_count;
	struct iovec comp_iov[RXR_IOV_LIMIT];
	int comp_iov_count;
};

struct rxr_rx_entry {
	/* Must remain at the top */
	enum rxr_x_entry_type type;

	fi_addr_t addr;
	struct rdm_peer *peer;

	/*
	 * freestack ids used to lookup rx_entry during pkt recv
	 */
	uint32_t tx_id;
	uint32_t rx_id;
	uint32_t op;

	/*
	 * The following two varibales are for emulated RMA fi_read only
	 */
	uint32_t rma_loc_tx_id;
	uint32_t rma_initiator_rx_id;

	struct rxr_atomic_hdr atomic_hdr;

	uint32_t msg_id;

	uint64_t tag;
	uint64_t ignore;

	uint64_t bytes_received;
	uint64_t bytes_copied;
	int64_t window;
	uint16_t credit_request;
	int credit_cts;

	uint64_t total_len;

	enum rxr_rx_comm_type state;
	struct rxr_queued_ctrl_info queued_ctrl;

	uint64_t fi_flags;
	uint16_t rxr_flags;

	size_t iov_count;
	struct iovec iov[RXR_IOV_LIMIT];

	/* App-provided buffers and descriptors */
	void *desc[RXR_IOV_LIMIT];
	struct fi_msg *posted_recv;

	/* iov_count on sender side, used for large message READ over shm */
	size_t rma_iov_count;
	struct fi_rma_iov rma_iov[RXR_IOV_LIMIT];

	struct fi_cq_tagged_entry cq_entry;

	/* entry is linked with rx entry lists in rxr_ep */
	struct dlist_entry entry;

	struct dlist_entry peer_unexp_entry; /* linked to peer->rx_unexp_list or peer->rx_unexp_tagged_list */

	/* queued_ctrl_entry is linked with rx_queued_ctrl_list in rxr_ep */
	struct dlist_entry queued_ctrl_entry;

	/* queued_rnr_entry is linked with rx_queued_rnr_list in rxr_ep */
	struct dlist_entry queued_rnr_entry;

	/* Queued packets due to TX queue full or RNR backoff */
	struct dlist_entry queued_pkts;

	/*
	 * A list of rx_entries tracking FI_MULTI_RECV buffers. An rx_entry of
	 * type RXR_MULTI_RECV_POSTED that was created when the multi-recv
	 * buffer was posted is the list head, and the rx_entries of type
	 * RXR_MULTI_RECV_CONSUMER get added to the list as they consume the
	 * buffer.
	 */
	struct dlist_entry multi_recv_consumers;
	struct dlist_entry multi_recv_entry;
	struct rxr_rx_entry *master_entry;

	struct rxr_pkt_entry *unexp_pkt;
	char *atomrsp_data;

	/* linked with rx_entry_list in rdm_peer */
	struct dlist_entry peer_entry;

	/* linked with rx_entry_list in rxr_ep */
	struct dlist_entry ep_entry;
#if ENABLE_DEBUG
	/* linked with rx_pending_list in rxr_ep */
	struct dlist_entry rx_pending_entry;
#endif
};

struct rxr_tx_entry {
	/* Must remain at the top */
	enum rxr_x_entry_type type;

	uint32_t op;
	fi_addr_t addr;
	struct rdm_peer *peer;

	/*
	 * freestack ids used to lookup tx_entry during ctrl pkt recv
	 */
	uint32_t tx_id;
	uint32_t rx_id;

	uint32_t msg_id;

	uint64_t tag;

	uint64_t bytes_acked;
	uint64_t bytes_sent;
	int64_t window;
	uint16_t credit_request;
	uint16_t credit_allocated;

	uint64_t total_len;

	enum rxr_tx_comm_type state;
	struct rxr_queued_ctrl_info queued_ctrl;

	uint64_t fi_flags;
	uint64_t rxr_flags;

	uint64_t send_flags;
	size_t iov_count;
	size_t iov_index;
	size_t iov_offset;
	struct iovec iov[RXR_IOV_LIMIT];

	uint64_t rma_loc_rx_id;
	uint64_t rma_window;
	size_t rma_iov_count;
	struct fi_rma_iov rma_iov[RXR_IOV_LIMIT];

	/* App-provided reg descriptor */
	void *desc[RXR_IOV_LIMIT];

	/* atomic related variables */
	struct rxr_atomic_hdr atomic_hdr;
	struct rxr_atomic_ex atomic_ex;

	/* Only used with mr threshold switch from memcpy */
	size_t iov_mr_start;
	struct fid_mr *mr[RXR_IOV_LIMIT];

	struct fi_cq_tagged_entry cq_entry;

	/* entry is linked with tx_pending_list in rxr_ep */
	struct dlist_entry entry;

	/* queued_ctrl_entry is linked with tx_queued_ctrl_list in rxr_ep */
	struct dlist_entry queued_ctrl_entry;

	/* queued_rnr_entry is linked with tx_queued_rnr_list in rxr_ep */
	struct dlist_entry queued_rnr_entry;

	/* Queued packets due to TX queue full or RNR backoff */
	struct dlist_entry queued_pkts;

	/* peer_entry is linked with tx_entry_list in rdm_peer */
	struct dlist_entry peer_entry;

	/* linked with tx_entry_list in rxr_ep */
	struct dlist_entry ep_entry;
};

#define RXR_GET_X_ENTRY_TYPE(pkt_entry)	\
	(*((enum rxr_x_entry_type *)	\
	 ((unsigned char *)((pkt_entry)->x_entry))))

enum efa_domain_type {
	EFA_DOMAIN_DGRAM = 0,
	EFA_DOMAIN_RDM,
};

struct rxr_domain {
	struct util_domain util_domain;
	enum efa_domain_type type;
	struct fid_domain *rdm_domain;
	size_t mtu_size;
	size_t addrlen;
	uint8_t rxr_mr_local;
	uint64_t rdm_mode;
	int do_progress;
	size_t cq_size;
};

struct rxr_ep {
	struct util_ep util_ep;

	uint8_t core_addr[RXR_MAX_NAME_LENGTH];
	size_t core_addrlen;

	/* per-version feature flag */
	uint64_t features[RXR_NUM_PROTOCOL_VERSION];

	/* core provider fid */
	struct fid_ep *rdm_ep;
	struct fid_cq *rdm_cq;

	/* shm provider fid */
	bool use_shm;
	struct fid_ep *shm_ep;
	struct fid_cq *shm_cq;

	/*
	 * RxR rx/tx queue sizes. These may be different from the core
	 * provider's rx/tx size and will either limit the number of possible
	 * receives/sends or allow queueing.
	 */
	size_t rx_size;
	size_t tx_size;
	size_t mtu_size;
	size_t rx_iov_limit;
	size_t tx_iov_limit;
	size_t inject_size;

	/* core's capabilities */
	uint64_t core_caps;

	/* Endpoint's capability to support zero-copy rx */
	bool use_zcpy_rx;

	/* Application requested resource management support */
	int handle_resource_management;

	/* rx/tx queue size of core provider */
	size_t core_rx_size;
	size_t efa_max_outstanding_tx_ops;
	size_t core_inject_size;
	size_t max_data_payload_size;

	/* Resource management flag */
	uint64_t rm_full;

	/* application's ordering requirements */
	uint64_t msg_order;
	/* core's supported tx/rx msg_order */
	uint64_t core_msg_order;

	/* Application's maximum msg size hint */
	size_t max_msg_size;

	/* Applicaiton's message prefix size. */
	size_t msg_prefix_size;

	/* RxR protocol's max header size */
	size_t max_proto_hdr_size;

	/* tx iov limit of core provider */
	size_t core_iov_limit;

	/* threshold to release multi_recv buffer */
	size_t min_multi_recv_size;

	/* buffer pool for send & recv */
	struct ofi_bufpool *efa_tx_pkt_pool;
	struct ofi_bufpool *efa_rx_pkt_pool;

	/*
	 * buffer pool for rxr_pkt_sendv struct, which is used
	 * to store iovec related information
	 */
	struct ofi_bufpool *pkt_sendv_pool;

	/*
	 * buffer pool for send & recv for shm as mtu size is different from
	 * the one of efa, and do not require local memory registration
	 */
	struct ofi_bufpool *shm_tx_pkt_pool;
	struct ofi_bufpool *shm_rx_pkt_pool;

	/* staging area for unexpected and out-of-order packets */
	struct ofi_bufpool *rx_unexp_pkt_pool;
	struct ofi_bufpool *rx_ooo_pkt_pool;

	/* staging area for read copy */
	struct ofi_bufpool *rx_readcopy_pkt_pool;
	int rx_readcopy_pkt_pool_used;
	int rx_readcopy_pkt_pool_max_used;

#ifdef ENABLE_EFA_POISONING
	size_t tx_pkt_pool_entry_sz;
	size_t rx_pkt_pool_entry_sz;
#endif

	/* datastructure to maintain rxr send/recv states */
	struct ofi_bufpool *tx_entry_pool;
	struct ofi_bufpool *rx_entry_pool;
	/* datastructure to maintain read response */
	struct ofi_bufpool *readrsp_tx_entry_pool;
	/* data structure to maintain read */
	struct ofi_bufpool *read_entry_pool;
	/* data structure to maintain pkt rx map */
	struct ofi_bufpool *map_entry_pool;
	/* rxr medium message pkt_entry to rx_entry map */
	struct rxr_pkt_rx_map *pkt_rx_map;
	/*
	 * buffer pool for atomic response data, used by
	 * emulated fetch and compare atomic.
	 */
	struct ofi_bufpool *rx_atomrsp_pool;
	/* rx_entries with recv buf */
	struct dlist_entry rx_list;
	/* rx_entries without recv buf (unexpected message) */
	struct dlist_entry rx_unexp_list;
	/* rx_entries with tagged recv buf */
	struct dlist_entry rx_tagged_list;
	/* rx_entries without tagged recv buf (unexpected message) */
	struct dlist_entry rx_unexp_tagged_list;
	/* list of pre-posted recv buffers */
	struct dlist_entry rx_posted_buf_list;
	/* list of pre-posted recv buffers for shm */
	struct dlist_entry rx_posted_buf_shm_list;
	/* tx entries with queued ctrl packets */
	struct dlist_entry tx_entry_queued_ctrl_list;
	/* tx entries with queued rnr packets */
	struct dlist_entry tx_entry_queued_rnr_list;
	/* rx entries with queued ctrl packets */
	struct dlist_entry rx_entry_queued_ctrl_list;
	/* rx entries with queued rnr packets */
	struct dlist_entry rx_entry_queued_rnr_list;
	/* tx_entries with data to be sent (large messages) */
	struct dlist_entry tx_pending_list;
	/* read entries with data to be read */
	struct dlist_entry read_pending_list;
	/* rxr_peer entries that are in backoff due to RNR */
	struct dlist_entry peer_backoff_list;
	/* rxr_peer entries that will retry posting handshake pkt */
	struct dlist_entry handshake_queued_peer_list;

#if ENABLE_DEBUG
	/* rx_entries waiting for data to arrive (large messages) */
	struct dlist_entry rx_pending_list;
	/* count of rx_pending_list */
	size_t rx_pending;

	/* rx packets being processed or waiting to be processed */
	struct dlist_entry rx_pkt_list;

	/* tx packets waiting for send completion */
	struct dlist_entry tx_pkt_list;

	size_t efa_total_posted_tx_ops;
	size_t shm_total_posted_tx_ops;
	size_t send_comps;
	size_t failed_send_comps;
	size_t recv_comps;
#endif
	/* track allocated rx_entries and tx_entries for endpoint cleanup */
	struct dlist_entry rx_entry_list;
	struct dlist_entry tx_entry_list;

	/* number of posted buffer for shm */
	size_t posted_bufs_shm;
	size_t rx_bufs_shm_to_post;

	/* number of posted buffers */
	size_t posted_bufs_efa;
	size_t rx_bufs_efa_to_post;
	/* number of buffers available for large messages */
	size_t available_data_bufs;
	/* Timestamp of when available_data_bufs was exhausted. */
	uint64_t available_data_bufs_ts;

	/* number of outstanding tx ops on efa device */
	size_t efa_outstanding_tx_ops;
	/* number of outstanding tx ops on shm */
	size_t shm_outstanding_tx_ops;
};

#define rxr_rx_flags(rxr_ep) ((rxr_ep)->util_ep.rx_op_flags)
#define rxr_tx_flags(rxr_ep) ((rxr_ep)->util_ep.tx_op_flags)

/*
 * Control header with completion data. CQ data length is static.
 */
#define RXR_CQ_DATA_SIZE (8)

static inline void rxr_copy_shm_cq_entry(struct fi_cq_tagged_entry *cq_tagged_entry,
					 struct fi_cq_data_entry *shm_cq_entry)
{
	cq_tagged_entry->op_context = shm_cq_entry->op_context;
	cq_tagged_entry->flags = shm_cq_entry->flags;
	cq_tagged_entry->len = shm_cq_entry->len;
	cq_tagged_entry->buf = shm_cq_entry->buf;
	cq_tagged_entry->data = shm_cq_entry->data;
	cq_tagged_entry->tag = 0; // No tag for RMA;

}

static inline void rxr_setup_msg(struct fi_msg *msg, const struct iovec *iov, void **desc,
				 size_t count, fi_addr_t addr, void *context, uint32_t data)
{
	msg->msg_iov = iov;
	msg->desc = desc;
	msg->iov_count = count;
	msg->addr = addr;
	msg->context = context;
	msg->data = data;
}

struct efa_ep_addr *rxr_ep_raw_addr(struct rxr_ep *ep);

const char *rxr_ep_raw_addr_str(struct rxr_ep *ep, char *buf, size_t *buflen);

struct efa_ep_addr *rxr_peer_raw_addr(struct rxr_ep *ep, fi_addr_t addr);

const char *rxr_peer_raw_addr_str(struct rxr_ep *ep, fi_addr_t addr, char *buf, size_t *buflen);

struct rxr_rx_entry *rxr_ep_get_rx_entry(struct rxr_ep *ep,
					 const struct fi_msg *msg,
					 uint64_t tag,
					 uint64_t ignore,
					 uint32_t op,
					 uint64_t flags);

struct rxr_rx_entry *rxr_ep_rx_entry_init(struct rxr_ep *ep,
					  struct rxr_rx_entry *rx_entry,
					  const struct fi_msg *msg,
					  uint64_t tag,
					  uint64_t ignore,
					  uint32_t op,
					  uint64_t flags);

void rxr_tx_entry_init(struct rxr_ep *rxr_ep, struct rxr_tx_entry *tx_entry,
		       const struct fi_msg *msg, uint32_t op, uint64_t flags);

struct rxr_tx_entry *rxr_ep_alloc_tx_entry(struct rxr_ep *rxr_ep,
					   const struct fi_msg *msg,
					   uint32_t op,
					   uint64_t tag,
					   uint64_t flags);

void rxr_release_tx_entry(struct rxr_ep *ep, struct rxr_tx_entry *tx_entry);

struct rxr_rx_entry *rxr_ep_alloc_rx_entry(struct rxr_ep *ep,
					   fi_addr_t addr, uint32_t op);

static inline void rxr_release_rx_entry(struct rxr_ep *ep,
					struct rxr_rx_entry *rx_entry)
{
	struct rxr_pkt_entry *pkt_entry;
	struct dlist_entry *tmp;

	if (rx_entry->peer)
		dlist_remove(&rx_entry->peer_entry);

	dlist_remove(&rx_entry->ep_entry);

	if (!dlist_empty(&rx_entry->queued_pkts)) {
		dlist_foreach_container_safe(&rx_entry->queued_pkts,
					     struct rxr_pkt_entry,
					     pkt_entry, entry, tmp) {
			rxr_pkt_entry_release_tx(ep, pkt_entry);
		}
		dlist_remove(&rx_entry->queued_rnr_entry);
	} else if (rx_entry->state == RXR_RX_QUEUED_CTRL) {
		dlist_remove(&rx_entry->queued_ctrl_entry);
	}

#ifdef ENABLE_EFA_POISONING
	rxr_poison_mem_region((uint32_t *)rx_entry,
			      sizeof(struct rxr_rx_entry));
#endif
	rx_entry->state = RXR_RX_FREE;
	ofi_buf_free(rx_entry);
}

static inline int rxr_match_addr(fi_addr_t addr, fi_addr_t match_addr)
{
	return (addr == FI_ADDR_UNSPEC || addr == match_addr);
}

static inline int rxr_match_tag(uint64_t tag, uint64_t ignore,
				uint64_t match_tag)
{
	return ((tag | ignore) == (match_tag | ignore));
}

void rxr_ep_record_tx_op_submitted(struct rxr_ep *ep, struct rxr_pkt_entry *pkt_entry);

void rxr_ep_record_tx_op_completed(struct rxr_ep *ep, struct rxr_pkt_entry *pkt_entry);

static inline size_t rxr_get_rx_pool_chunk_cnt(struct rxr_ep *ep)
{
	return MIN(ep->core_rx_size, ep->rx_size);
}

static inline size_t rxr_get_tx_pool_chunk_cnt(struct rxr_ep *ep)
{
	return MIN(ep->efa_max_outstanding_tx_ops, ep->tx_size);
}

static inline int rxr_need_sas_ordering(struct rxr_ep *ep)
{
	return ep->msg_order & FI_ORDER_SAS;
}

static inline int rxr_ep_use_zcpy_rx(struct rxr_ep *ep, struct fi_info *info)
{
	return !(ep->util_ep.caps & FI_DIRECTED_RECV) &&
		!(ep->util_ep.caps & FI_TAGGED) &&
		!(ep->util_ep.caps & FI_ATOMIC) &&
		(ep->max_msg_size <= ep->mtu_size - ep->max_proto_hdr_size) &&
		!rxr_need_sas_ordering(ep) &&
		info->mode & FI_MSG_PREFIX &&
		rxr_env.use_zcpy_rx;
}

/* Initialization functions */
void rxr_reset_rx_tx_to_core(const struct fi_info *user_info,
			     struct fi_info *core_info);
int rxr_get_lower_rdm_info(uint32_t version, const char *node, const char *service,
			   uint64_t flags, const struct util_prov *util_prov,
			   const struct fi_info *util_hints,
			   struct fi_info **core_info);
int rxr_fabric(struct fi_fabric_attr *attr,
	       struct fid_fabric **fabric, void *context);
int rxr_domain_open(struct fid_fabric *fabric, struct fi_info *info,
		    struct fid_domain **dom, void *context);
int rxr_cq_open(struct fid_domain *domain, struct fi_cq_attr *attr,
		struct fid_cq **cq_fid, void *context);
int rxr_endpoint(struct fid_domain *domain, struct fi_info *info,
		 struct fid_ep **ep, void *context);

/* EP sub-functions */
void rxr_ep_progress(struct util_ep *util_ep);
void rxr_ep_progress_internal(struct rxr_ep *rxr_ep);

int rxr_ep_post_user_buf(struct rxr_ep *ep, struct rxr_rx_entry *rx_entry,
			 uint64_t flags);

int rxr_ep_set_tx_credit_request(struct rxr_ep *rxr_ep,
				 struct rxr_tx_entry *tx_entry);

int rxr_ep_tx_init_mr_desc(struct rxr_domain *rxr_domain,
			   struct rxr_tx_entry *tx_entry,
			   int mr_iov_start, uint64_t access);

void rxr_convert_desc_for_shm(int numdesc, void **desc);

void rxr_prepare_desc_send(struct rxr_domain *rxr_domain,
			   struct rxr_tx_entry *tx_entry);

struct rxr_rx_entry *rxr_ep_lookup_mediumrtm_rx_entry(struct rxr_ep *ep,
						      struct rxr_pkt_entry *pkt_entry);

void rxr_ep_record_mediumrtm_rx_entry(struct rxr_ep *ep,
				      struct rxr_pkt_entry *pkt_entry,
				      struct rxr_rx_entry *rx_entry);

struct rxr_rx_entry *rxr_ep_alloc_unexp_rx_entry_for_msgrtm(struct rxr_ep *ep,
							    struct rxr_pkt_entry **pkt_entry);

struct rxr_rx_entry *rxr_ep_alloc_unexp_rx_entry_for_tagrtm(struct rxr_ep *ep,
							    struct rxr_pkt_entry **pkt_entry);

struct rxr_rx_entry *rxr_ep_split_rx_entry(struct rxr_ep *ep,
					   struct rxr_rx_entry *posted_entry,
					   struct rxr_rx_entry *consumer_entry,
					   struct rxr_pkt_entry *pkt_entry);
int rxr_raw_addr_to_smr_name(void *addr, char *smr_name);

/* CQ sub-functions */
void rxr_cq_write_rx_error(struct rxr_ep *ep, struct rxr_rx_entry *rx_entry,
			   int err, int prov_errno);

void rxr_cq_write_tx_error(struct rxr_ep *ep, struct rxr_tx_entry *tx_entry,
			   int err, int prov_errno);

void rxr_cq_queue_rnr_pkt(struct rxr_ep *ep,
			  struct dlist_entry *list,
			  struct rxr_pkt_entry *pkt_entry);

void rxr_cq_write_rx_completion(struct rxr_ep *ep,
				struct rxr_rx_entry *rx_entry);

void rxr_cq_handle_rx_completion(struct rxr_ep *ep,
				 struct rxr_pkt_entry *pkt_entry,
				 struct rxr_rx_entry *rx_entry);

void rxr_cq_write_tx_completion(struct rxr_ep *ep,
				struct rxr_tx_entry *tx_entry);

void rxr_cq_handle_tx_completion(struct rxr_ep *ep,
				 struct rxr_tx_entry *tx_entry);

void rxr_cq_handle_shm_completion(struct rxr_ep *ep,
				  struct fi_cq_data_entry *cq_entry,
				  fi_addr_t src_addr);

int rxr_cq_reorder_msg(struct rxr_ep *ep,
		       struct rdm_peer *peer,
		       struct rxr_pkt_entry *pkt_entry);

void rxr_cq_proc_pending_items_in_recvwin(struct rxr_ep *ep,
					  struct rdm_peer *peer);

void rxr_cq_handle_shm_rma_write_data(struct rxr_ep *ep,
				      struct fi_cq_data_entry *shm_comp,
				      fi_addr_t src_addr);

/* Aborts if unable to write to the eq */
static inline void efa_eq_write_error(struct util_ep *ep, ssize_t err,
				      ssize_t prov_errno)
{
	struct fi_eq_err_entry err_entry;
	int ret = -FI_ENOEQ;

	FI_WARN(&rxr_prov, FI_LOG_EQ, "Writing error %s to EQ.\n",
		fi_strerror(err));
	if (ep->eq) {
		memset(&err_entry, 0, sizeof(err_entry));
		err_entry.err = err;
		err_entry.prov_errno = prov_errno;
		ret = fi_eq_write(&ep->eq->eq_fid, FI_NOTIFY,
				  &err_entry, sizeof(err_entry),
				  UTIL_FLAG_ERROR);

		if (ret == sizeof(err_entry))
			return;
	}

	FI_WARN(&rxr_prov, FI_LOG_EQ,
		"Unable to write to EQ: %s. err: %s (%zd) prov_errno: %s (%zd)\n",
		fi_strerror(-ret), fi_strerror(err), err,
		fi_strerror(prov_errno), prov_errno);
	fprintf(stderr,
		"Unable to write to EQ: %s. err: %s (%zd) prov_errno: %s (%zd) %s:%d\n",
		fi_strerror(-ret), fi_strerror(err), err,
		fi_strerror(prov_errno), prov_errno, __FILE__, __LINE__);
	abort();
}

static inline struct rxr_domain *rxr_ep_domain(struct rxr_ep *ep)
{
	return container_of(ep->util_ep.domain, struct rxr_domain, util_domain);
}

/*
 * today we have only cq res check, in future we will have ctx, and other
 * resource check as well.
 */
static inline uint64_t is_tx_res_full(struct rxr_ep *ep)
{
	return ep->rm_full & RXR_RM_TX_CQ_FULL;
}

static inline uint64_t is_rx_res_full(struct rxr_ep *ep)
{
	return ep->rm_full & RXR_RM_RX_CQ_FULL;
}

static inline void rxr_rm_rx_cq_check(struct rxr_ep *ep, struct util_cq *rx_cq)
{
	fastlock_acquire(&rx_cq->cq_lock);
	if (ofi_cirque_isfull(rx_cq->cirq))
		ep->rm_full |= RXR_RM_RX_CQ_FULL;
	else
		ep->rm_full &= ~RXR_RM_RX_CQ_FULL;
	fastlock_release(&rx_cq->cq_lock);
}

static inline void rxr_rm_tx_cq_check(struct rxr_ep *ep, struct util_cq *tx_cq)
{
	fastlock_acquire(&tx_cq->cq_lock);
	if (ofi_cirque_isfull(tx_cq->cirq))
		ep->rm_full |= RXR_RM_TX_CQ_FULL;
	else
		ep->rm_full &= ~RXR_RM_TX_CQ_FULL;
	fastlock_release(&tx_cq->cq_lock);
}

/* Performance counter declarations */
#ifdef RXR_PERF_ENABLED
#define RXR_PERF_FOREACH(DECL)	\
	DECL(perf_rxr_tx),	\
	DECL(perf_rxr_recv),	\
	DECL(rxr_perf_size)	\

enum rxr_perf_counters {
	RXR_PERF_FOREACH(OFI_ENUM_VAL)
};

extern const char *rxr_perf_counters_str[];

static inline void rxr_perfset_start(struct rxr_ep *ep, size_t index)
{
	struct rxr_domain *domain = rxr_ep_domain(ep);
	struct rxr_fabric *fabric = container_of(domain->util_domain.fabric,
						 struct rxr_fabric,
						 util_fabric);
	ofi_perfset_start(&fabric->perf_set, index);
}

static inline void rxr_perfset_end(struct rxr_ep *ep, size_t index)
{
	struct rxr_domain *domain = rxr_ep_domain(ep);
	struct rxr_fabric *fabric = container_of(domain->util_domain.fabric,
						 struct rxr_fabric,
						 util_fabric);
	ofi_perfset_end(&fabric->perf_set, index);
}
#else
#define rxr_perfset_start(ep, index) do {} while (0)
#define rxr_perfset_end(ep, index) do {} while (0)
#endif
#endif
