/*
 * Copyright (c) 2014 Intel Corporation, Inc.  All rights reserved.
 * Copyright (c) 2016 Cisco Systems, Inc. All rights reserved.
 * Copyright (c) 2017 DataDirect Networks, Inc. All rights reserved.
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

#include "config.h"

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
#include <netdb.h>
#include <netinet/in.h>

#include <ofi.h>
#include <ofi_atom.h>
#include <ofi_atomic.h>
#include <ofi_mr.h>
#include <ofi_enosys.h>
#include <ofi_indexer.h>
#include <ofi_rbuf.h>
#include <ofi_list.h>
#include <ofi_file.h>
#include <ofi_osd.h>
#include <ofi_util.h>

#ifndef _SOCK_H_
#define _SOCK_H_

/* 4k allocated for all sockets headers */
#define SOCK_EP_MAX_MSG_SZ (OFI_MAX_SOCKET_BUF_SIZE - 4096)
#define SOCK_EP_MAX_INJECT_SZ ((1<<8) - 1)
#define SOCK_EP_MAX_BUFF_RECV (1<<26)
#define SOCK_EP_MAX_ORDER_RAW_SZ SOCK_EP_MAX_MSG_SZ
#define SOCK_EP_MAX_ORDER_WAR_SZ SOCK_EP_MAX_MSG_SZ
#define SOCK_EP_MAX_ORDER_WAW_SZ SOCK_EP_MAX_MSG_SZ
#define SOCK_EP_MEM_TAG_FMT FI_TAG_GENERIC
#define SOCK_EP_MAX_EP_CNT (128)
#define SOCK_EP_MAX_CQ_CNT (32)
#define SOCK_EP_MAX_CNTR_CNT (128)
#define SOCK_EP_MAX_TX_CNT (16)
#define SOCK_EP_MAX_RX_CNT (16)
#define SOCK_EP_MAX_IOV_LIMIT (8)
#define SOCK_EP_TX_SZ (256)
#define SOCK_EP_RX_SZ (256)
#define SOCK_EP_MIN_MULTI_RECV (64)
#define SOCK_EP_MAX_ATOMIC_SZ (4096)
#define SOCK_EP_MAX_CTX_BITS (16)
#define SOCK_EP_MSG_PREFIX_SZ (0)
#define SOCK_DOMAIN_MR_CNT (65535)

#define SOCK_PE_POLL_TIMEOUT (100000)
#define SOCK_PE_MAX_ENTRIES (128)
#define SOCK_PE_WAITTIME (10)

#define SOCK_EQ_DEF_SZ (1<<8)
#define SOCK_CQ_DEF_SZ (1<<8)
#define SOCK_AV_DEF_SZ (1<<8)
#define SOCK_CMAP_DEF_SZ (1<<10)
#define SOCK_EPOLL_WAIT_EVENTS 32

#define SOCK_CQ_DATA_SIZE (sizeof(uint64_t))
#define SOCK_TAG_SIZE (sizeof(uint64_t))
#define SOCK_MAX_NETWORK_ADDR_SZ (35)

#define SOCK_PEP_LISTENER_TIMEOUT (10000)
#define SOCK_CM_COMM_TIMEOUT (2000)
#define SOCK_EP_MAX_RETRY (5)
#define SOCK_EP_MAX_CM_DATA_SZ (256)
#define SOCK_CM_DEF_BACKLOG (128)
#define SOCK_CM_DEF_TIMEOUT (15000)
#define SOCK_CM_DEF_RETRY (5)
#define SOCK_CM_CONN_IN_PROGRESS ((struct sock_conn *)(0x1L))

#define SOCK_EP_MSG_ORDER (OFI_ORDER_RAR_SET | OFI_ORDER_RAW_SET | FI_ORDER_RAS| \
			   OFI_ORDER_WAR_SET | OFI_ORDER_WAW_SET | FI_ORDER_WAS | \
			   FI_ORDER_SAR | FI_ORDER_SAW | FI_ORDER_SAS)

#define SOCK_EP_COMP_ORDER (FI_ORDER_STRICT | FI_ORDER_DATA)

#define SOCK_EP_CQ_FLAGS (FI_SEND | FI_TRANSMIT | FI_RECV | \
			FI_SELECTIVE_COMPLETION)
#define SOCK_EP_CNTR_FLAGS (FI_SEND | FI_RECV | FI_READ | \
			FI_WRITE | FI_REMOTE_READ | FI_REMOTE_WRITE)

#define SOCK_EP_SET_TX_OP_FLAGS(_flags) do {			\
		if (!((_flags) & FI_INJECT_COMPLETE))		\
			(_flags) |= FI_TRANSMIT_COMPLETE;	\
	} while (0)

#define SOCK_MODE (0)
#define SOCK_NO_COMPLETION (1ULL << 60)
#define SOCK_USE_OP_FLAGS (1ULL << 61)
#define SOCK_TRIGGERED_OP (1ULL << 62)
#define SOCK_PE_COMM_BUFF_SZ (1024)
#define SOCK_PE_OVERFLOW_COMM_BUFF_SZ (128)

/* it must be adjusted if error data size in CQ/EQ
 * will be larger than SOCK_EP_MAX_CM_DATA_SZ */
#define SOCK_MAX_ERR_CQ_EQ_DATA_SZ SOCK_EP_MAX_CM_DATA_SZ

enum {
	SOCK_SIGNAL_RD_FD = 0,
	SOCK_SIGNAL_WR_FD
};

enum {
	SOCK_OPTS_NONBLOCK  = 1<<0,
	SOCK_OPTS_KEEPALIVE = 1<<1,
	SOCK_OPTS_BUFSIZE = 1<<2,
};

#define SOCK_WIRE_PROTO_VERSION (2)

extern struct fi_info sock_dgram_info;
extern struct fi_info sock_msg_info;

extern struct util_prov sock_util_prov;
extern struct fi_domain_attr sock_domain_attr;
extern struct fi_fabric_attr sock_fabric_attr;
extern struct fi_tx_attr sock_msg_tx_attr;
extern struct fi_tx_attr sock_rdm_tx_attr;
extern struct fi_tx_attr sock_dgram_tx_attr;
extern struct fi_rx_attr sock_msg_rx_attr;
extern struct fi_rx_attr sock_rdm_rx_attr;
extern struct fi_rx_attr sock_dgram_rx_attr;
extern struct fi_ep_attr sock_msg_ep_attr;
extern struct fi_ep_attr sock_rdm_ep_attr;
extern struct fi_ep_attr sock_dgram_ep_attr;
extern struct fi_tx_attr sock_stx_attr;
extern struct fi_rx_attr sock_srx_attr;

struct sock_service_entry {
	int service;
	struct dlist_entry entry;
};

struct sock_fabric {
	struct fid_fabric fab_fid;
	ofi_atomic32_t ref;
#if ENABLE_DEBUG
	uint64_t num_send_msg;
#endif
	struct dlist_entry service_list;
	struct dlist_entry fab_list_entry;
	ofi_mutex_t lock;
};

struct sock_conn {
	int sock_fd;
	int connected;
	int address_published;
	union ofi_sock_ip addr;
	struct sock_pe_entry *rx_pe_entry;
	struct sock_pe_entry *tx_pe_entry;
	struct sock_ep_attr *ep_attr;
	fi_addr_t av_index;
	struct dlist_entry ep_entry;
};

struct sock_conn_map {
	struct sock_conn *table;
	ofi_epoll_t epoll_set;
	struct ofi_epollfds_event *epoll_events;
	int epoll_size;
	int used;
	int size;
	ofi_mutex_t lock;
};

struct sock_conn_listener {
	ofi_epoll_t epollfd;
	struct fd_signal signal;
	ofi_mutex_t signal_lock; /* acquire before map lock */
	pthread_t listener_thread;
	int do_listen;
	bool removed_from_epollfd;
};

struct sock_ep_cm_head {
	ofi_epoll_t epollfd;
	struct fd_signal signal;
	pthread_mutex_t signal_lock;
	pthread_t listener_thread;
	struct dlist_entry msg_list;
	int do_listen;
	bool removed_from_epollfd;
};

struct sock_domain {
	struct fi_info		info;
	struct fid_domain	dom_fid;
	struct sock_fabric	*fab;
	ofi_mutex_t		lock;
	ofi_atomic32_t		ref;

	struct sock_eq		*eq;
	struct sock_eq		*mr_eq;

	enum fi_progress	progress_mode;
	struct ofi_mr_map	mr_map;
	struct sock_pe		*pe;
	struct dlist_entry	dom_list_entry;
	struct fi_domain_attr	attr;
	struct sock_conn_listener conn_listener;
	struct sock_ep_cm_head cm_head;
};

/* move to fi_trigger.h when removing experimental tag from work queues */
enum {
	SOCK_DEFERRED_WORK = FI_TRIGGER_THRESHOLD + 1
};

/* move to fi_trigger.h when removing experimental tag from work queues */
/* Overlay with fi_trigger_threshold and within fi_trigger_context */
struct sock_trigger_work {
	struct fid_cntr		*triggering_cntr;
	size_t			threshold;
	struct fid_cntr		*completion_cntr;
};

/* must overlay fi_triggered_context */
struct sock_triggered_context {
	int					event_type;
	union {
		struct fi_trigger_threshold	threshold;
		struct sock_trigger_work	work;
		void				*internal[3];
	} trigger;
};

struct sock_trigger {
	enum fi_op_type op_type;
	size_t threshold;
	struct dlist_entry entry;

	struct sock_triggered_context *context;
	struct fid_ep *ep;
	uint64_t flags;

	union {
		struct {
			struct fi_msg msg;
			struct iovec msg_iov[SOCK_EP_MAX_IOV_LIMIT];
		} msg;

		struct {
			struct fi_msg_tagged msg;
			struct iovec msg_iov[SOCK_EP_MAX_IOV_LIMIT];
		} tmsg;

		struct {
			struct fi_msg_rma msg;
			struct iovec msg_iov[SOCK_EP_MAX_IOV_LIMIT];
			struct fi_rma_iov rma_iov[SOCK_EP_MAX_IOV_LIMIT];
		} rma;

		struct {
			struct fi_msg_atomic msg;
			struct fi_ioc msg_iov[SOCK_EP_MAX_IOV_LIMIT];
			struct fi_rma_ioc rma_iov[SOCK_EP_MAX_IOV_LIMIT];
			struct fi_ioc comparev[SOCK_EP_MAX_IOV_LIMIT];
			size_t compare_count;
			struct fi_ioc resultv[SOCK_EP_MAX_IOV_LIMIT];
			size_t result_count;
		} atomic;
	} op;
};

struct sock_cntr {
	struct fid_cntr		cntr_fid;
	struct sock_domain	*domain;
	ofi_atomic32_t		value;
	ofi_atomic32_t		ref;
	ofi_atomic32_t		err_cnt;
	ofi_atomic32_t		last_read_val;
	pthread_cond_t 		cond;
	pthread_mutex_t		mut;
	struct fi_cntr_attr	attr;

	struct dlist_entry	rx_list;
	struct dlist_entry	tx_list;
	ofi_mutex_t		list_lock;

	ofi_mutex_t		trigger_lock;
	struct dlist_entry	trigger_list;

	struct fid_wait		*waitset;
	int			signal;
	ofi_atomic32_t		num_waiting;
	int			err_flag;
};

struct sock_mr {
	struct fid_mr mr_fid;
	struct sock_domain *domain;
	uint64_t key;
	uint64_t flags;
	struct sock_cntr *cntr;
	struct sock_cq *cq;
};

struct sock_av_addr {
	union ofi_sock_ip addr;
	uint8_t valid;
	uint8_t reserved[7];
};

struct sock_av_table_hdr {
	uint64_t size;
	uint64_t stored;
};

struct sock_av {
	struct fid_av av_fid;
	struct sock_domain *domain;
	ofi_atomic32_t ref;
	struct fi_av_attr attr;
	uint64_t mask;
	int rx_ctx_bits;
	socklen_t addrlen;
	struct sock_eq *eq;
	struct sock_av_table_hdr *table_hdr;
	struct sock_av_addr *table;
	uint64_t *idx_arr;
	struct util_shm shm;
	int    shared;
	struct dlist_entry ep_list;
	ofi_mutex_t list_lock;
	ofi_mutex_t table_lock;
};

struct sock_fid_list {
	struct dlist_entry entry;
	struct fid *fid;
};

struct sock_poll {
	struct fid_poll poll_fid;
	struct sock_domain *domain;
	struct dlist_entry fid_list;
};

struct sock_wait {
	struct fid_wait wait_fid;
	struct sock_fabric *fab;
	struct dlist_entry fid_list;
	enum fi_wait_obj type;
	union {
		int fd[2];
		struct sock_mutex_cond {
			pthread_mutex_t	mutex;
			pthread_cond_t	cond;
		} mutex_cond;
	} wobj;
};

enum {
	/* wire protocol */
	SOCK_OP_SEND = 0,
	SOCK_OP_TSEND = 1,
	SOCK_OP_SEND_COMPLETE = 2,

	SOCK_OP_WRITE = 3,
	SOCK_OP_WRITE_COMPLETE = 4,
	SOCK_OP_WRITE_ERROR = 5,

	SOCK_OP_READ = 6,
	SOCK_OP_READ_COMPLETE = 7,
	SOCK_OP_READ_ERROR = 8,

	SOCK_OP_ATOMIC = 9,
	SOCK_OP_ATOMIC_COMPLETE = 10,
	SOCK_OP_ATOMIC_ERROR = 11,

	SOCK_OP_CONN_MSG = 12,

	/* internal */
	SOCK_OP_RECV,
	SOCK_OP_TRECV,
};

/*
 * Transmit context - ring buffer data:
 *    tx_op + flags + context + dest_addr + conn + [data] + [tag] + tx_iov
 *     8B       8B      8B         8B         8B       8B      24B+
 * data - only present if flags indicate
 * tag - only present for TSEND op
 */
struct sock_op {
	uint8_t op;
	uint8_t src_iov_len;
	uint8_t	dest_iov_len;
	struct {
		uint8_t	op;
		uint8_t	datatype;
		uint8_t	res_iov_len;
		uint8_t	cmp_iov_len;
	} atomic;
	uint8_t	reserved[1];
};

struct sock_op_send {
	struct sock_op op;
	uint64_t flags;
	uint64_t context;
	uint64_t dest_addr;
	uint64_t buf;
	struct sock_ep *ep;
	struct sock_conn *conn;
};

struct sock_op_tsend {
	struct sock_op op;
	uint64_t flags;
	uint64_t context;
	uint64_t dest_addr;
	uint64_t buf;
	struct sock_ep *ep;
	struct sock_conn *conn;
	uint64_t tag;
};

union sock_iov {
	struct fi_rma_iov iov;
	struct fi_rma_ioc ioc;
};

struct sock_eq_entry {
	uint32_t type;
	size_t len;
	uint64_t flags;
	struct dlist_entry entry;
	char event[];
};

struct sock_eq_err_data_entry {
	struct dlist_entry entry;
	int do_free;
	char err_data[];
};

struct sock_eq {
	struct fid_eq eq;
	struct fi_eq_attr attr;
	struct sock_fabric *sock_fab;

	struct dlistfd_head list;
	struct dlistfd_head err_list;
	struct dlist_entry err_data_list;
	ofi_mutex_t lock;

	struct fid_wait *waitset;
	int signal;
	int wait_fd;
	char service[NI_MAXSERV];
};

struct sock_comp {
	uint8_t send_cq_event;
	uint8_t recv_cq_event;
	char reserved[2];

	struct sock_cq	*send_cq;
	struct sock_cq	*recv_cq;

	struct sock_cntr *send_cntr;
	struct sock_cntr *recv_cntr;
	struct sock_cntr *read_cntr;
	struct sock_cntr *write_cntr;
	struct sock_cntr *rem_read_cntr;
	struct sock_cntr *rem_write_cntr;

	struct sock_eq *eq;
};

enum sock_cm_state {
	SOCK_CM_STATE_DISCONNECTED = 0,
	SOCK_CM_STATE_REQUESTED,
	SOCK_CM_STATE_CONNECTED,
};

struct sock_pep_cm_entry {
	int sock;
	int do_listen;
	int signal_fds[2];
	pthread_t listener_thread;
};

struct sock_ep_cm_entry {
	int sock;
	ofi_mutex_t lock;
	enum sock_cm_state state;
};

struct sock_conn_handle {
	int sock;
	int do_listen;
};

struct sock_ep_attr {
	size_t fclass;

	int tx_shared;
	int rx_shared;
	size_t buffered_len;
	size_t min_multi_recv;

	ofi_atomic32_t ref;
	struct sock_eq *eq;
	struct sock_av *av;
	struct sock_domain *domain;

	struct sock_rx_ctx *rx_ctx;
	struct sock_tx_ctx *tx_ctx;

	struct sock_rx_ctx **rx_array;
	struct sock_tx_ctx **tx_array;
	ofi_atomic32_t num_rx_ctx;
	ofi_atomic32_t num_tx_ctx;

	struct dlist_entry rx_ctx_entry;
	struct dlist_entry tx_ctx_entry;

	struct fi_info info;
	struct fi_ep_attr ep_attr;

	enum fi_ep_type ep_type;
	union ofi_sock_ip *src_addr;
	union ofi_sock_ip *dest_addr;
	uint16_t msg_src_port;
	uint16_t msg_dest_port;

	uint64_t peer_fid;
	uint16_t key;
	int is_enabled;
	struct sock_ep_cm_entry cm;
	struct sock_conn_handle conn_handle;
	ofi_mutex_t lock;

	struct index_map av_idm;
	struct sock_conn_map cmap;
};

struct sock_ep {
	struct fid_ep ep;
	struct fi_tx_attr tx_attr;
	struct fi_rx_attr rx_attr;
	struct sock_ep_attr *attr;
	int is_alias;
};

struct sock_pep {
	struct fid_pep	pep;
	struct sock_fabric *sock_fab;

	struct sock_ep_cm_head cm_head;
	struct sock_pep_cm_entry cm;
	union ofi_sock_ip src_addr;
	struct fi_info info;
	struct sock_eq *eq;
	int name_set;
};

struct sock_rx_entry {
	struct sock_op rx_op;
	uint8_t is_buffered;
	uint8_t is_busy;
	uint8_t is_claimed;
	uint8_t is_complete;
	uint8_t is_tagged;
	uint8_t is_pool_entry;
	uint8_t reserved[2];

	uint64_t used;
	uint64_t total_len;

	uint64_t flags;
	uint64_t context;
	uint64_t addr;
	uint64_t data;
	uint64_t tag;
	uint64_t ignore;
	struct sock_comp *comp;

	union sock_iov iov[SOCK_EP_MAX_IOV_LIMIT];
	struct dlist_entry entry;
	struct slist_entry pool_entry;
	struct sock_rx_ctx *rx_ctx;
};

struct sock_rx_ctx {
	struct fid_ep ctx;

	uint16_t rx_id;
	int enabled;
	int progress;
	int is_ctrl_ctx;
	int recv_cq_event;
	int use_shared;

	size_t num_left;
	size_t buffered_len;
	size_t min_multi_recv;
	uint64_t addr;
	struct sock_comp comp;
	struct sock_rx_ctx *srx_ctx;

	struct sock_ep_attr *ep_attr;
	struct sock_av *av;
	struct sock_eq *eq;
 	struct sock_domain *domain;

	struct dlist_entry pe_entry;
	struct dlist_entry cq_entry;

	struct dlist_entry pe_entry_list;
	struct dlist_entry rx_entry_list;
	struct dlist_entry rx_buffered_list;
	struct dlist_entry ep_list;
	ofi_mutex_t lock;

	struct dlist_entry *progress_start;

	struct fi_rx_attr attr;
	struct sock_rx_entry *rx_entry_pool;
	struct slist pool_list;
};

struct sock_tx_ctx {
	union {
		struct fid_ep ctx;
		struct fid_stx stx;
	} fid;
	size_t fclass;

	struct ofi_ringbuf rb;
	ofi_mutex_t rb_lock;

	uint16_t tx_id;
	uint8_t enabled;
	uint8_t progress;

	int use_shared;
	uint64_t addr;
	struct sock_comp comp;
	struct sock_rx_ctx *rx_ctrl_ctx;
	struct sock_tx_ctx *stx_ctx;

	struct sock_ep_attr *ep_attr;
	struct sock_av *av;
	struct sock_eq *eq;
 	struct sock_domain *domain;

	struct dlist_entry pe_entry;
	struct dlist_entry cq_entry;

	struct dlist_entry pe_entry_list;
	struct dlist_entry ep_list;

	struct fi_tx_attr attr;
	ofi_mutex_t lock;
};

struct sock_msg_hdr {
	uint8_t version;
	uint8_t op_type;
	uint8_t rx_id;
	uint8_t dest_iov_len;
	uint16_t pe_entry_id;
	uint8_t reserved[2];

	uint64_t flags;
	uint64_t msg_len;
};

struct sock_msg_send {
	struct sock_msg_hdr msg_hdr;
	/* user data */
	/* data */
};

struct sock_msg_tsend {
	struct sock_msg_hdr msg_hdr;
	uint64_t tag;
	/* user data */
	/* data */
};

struct sock_rma_write_req {
	struct sock_msg_hdr msg_hdr;
	/* user data */
	/* dst iov(s)*/
	/* data */
};

struct sock_atomic_req {
	struct sock_msg_hdr msg_hdr;
	struct sock_op op;

	/* user data */
	/* dst ioc(s)*/
	/* cmp iov(s) */
	/* data */
};

struct sock_msg_response {
	struct sock_msg_hdr msg_hdr;
	uint16_t pe_entry_id;
	int32_t err;
	uint8_t reserved[2];
};

struct sock_rma_read_req {
	struct sock_msg_hdr msg_hdr;
	/* src iov(s)*/
};

struct sock_rma_read_response {
	struct sock_msg_hdr msg_hdr;
	uint16_t pe_entry_id;
	uint8_t reserved[6];
	/* data */
};

struct sock_atomic_response {
	struct sock_msg_hdr msg_hdr;
	uint16_t pe_entry_id;
	uint8_t reserved[6];
	/* data */
};

struct sock_tx_iov {
	union sock_iov src;
	union sock_iov dst;
	union sock_iov res;
	union sock_iov cmp;
};

struct sock_tx_pe_entry {
	struct sock_op tx_op;
	struct sock_comp *comp;
	uint8_t header_sent;
	uint8_t send_done;
	uint8_t reserved[6];

	struct sock_tx_ctx *tx_ctx;
	struct sock_tx_iov tx_iov[SOCK_EP_MAX_IOV_LIMIT];
	char inject[SOCK_EP_MAX_INJECT_SZ];
};

struct sock_rx_pe_entry {
	struct sock_op rx_op;

	struct sock_comp *comp;
	uint8_t header_read;
	uint8_t pending_send;
	uint8_t reserved[6];
	struct sock_rx_entry *rx_entry;
	union sock_iov rx_iov[SOCK_EP_MAX_IOV_LIMIT];
	char *atomic_cmp;
	char *atomic_src;
};

/* PE entry type */
enum {
	SOCK_PE_RX,
	SOCK_PE_TX,
};

struct sock_pe_entry {
	union {
		struct sock_tx_pe_entry tx;
		struct sock_rx_pe_entry rx;
	} pe;

	struct sock_msg_hdr msg_hdr;
	struct sock_msg_response response;

	uint64_t flags;
	uint64_t context;
	uint64_t addr;
	uint64_t data;
	uint64_t tag;
	uint64_t buf;

	uint8_t type;
	uint8_t is_complete;
	uint8_t is_error;
	uint8_t mr_checked;
	uint8_t is_pool_entry;
	uint8_t completion_reported;
	uint8_t reserved[3];

	uint64_t done_len;
	uint64_t total_len;
	uint64_t data_len;
	uint64_t rem;
	void *comm_addr;
	struct sock_ep_attr *ep_attr;
	struct sock_conn *conn;
	struct sock_comp *comp;

	struct dlist_entry entry;
	struct dlist_entry ctx_entry;
	struct ofi_ringbuf comm_buf;
	size_t cache_sz;
};

struct sock_pe {
	struct sock_domain *domain;
	int num_free_entries;
	struct sock_pe_entry pe_table[SOCK_PE_MAX_ENTRIES];
	ofi_mutex_t lock;
	ofi_mutex_t signal_lock;
	pthread_mutex_t list_lock;
	int wcnt, rcnt;
	int signal_fds[2];
	uint64_t waittime;

	struct ofi_bufpool *pe_rx_pool;
	struct ofi_bufpool *atomic_rx_pool;
	struct dlist_entry free_list;
	struct dlist_entry busy_list;
	struct dlist_entry pool_list;

	struct dlist_entry tx_list;
	struct dlist_entry rx_list;

	pthread_t progress_thread;
	volatile int do_progress;
	struct sock_pe_entry *pe_atomic;
	ofi_epoll_t epoll_set;
};

typedef ssize_t (*sock_cq_report_fn) (struct sock_cq *cq, fi_addr_t addr,
				      struct sock_pe_entry *pe_entry);

struct sock_cq_overflow_entry_t {
	size_t len;
	fi_addr_t addr;
	struct dlist_entry entry;
	char cq_entry[];
};

struct sock_cq {
	struct fid_cq cq_fid;
	struct sock_domain *domain;
	ssize_t cq_entry_size;
	ofi_atomic32_t ref;
	struct fi_cq_attr attr;

	struct ofi_ringbuf addr_rb;
	struct ofi_ringbuffd cq_rbfd;
	struct ofi_ringbuf cqerr_rb;
	struct dlist_entry overflow_list;
	pthread_mutex_t lock;
	pthread_mutex_t list_lock;

	struct fid_wait *waitset;
	int signal;
	ofi_atomic32_t signaled;

	struct dlist_entry ep_list;
	struct dlist_entry rx_list;
	struct dlist_entry tx_list;

	sock_cq_report_fn report_completion;
};

struct sock_conn_hdr {
	uint8_t type;
	uint8_t reserved[3];
	uint16_t port;
	uint16_t cm_data_sz;
	/* cm data follows cm_data_sz */
};

struct sock_conn_req {
	struct sock_conn_hdr hdr;
	union ofi_sock_ip src_addr;
	uint64_t caps;
	char cm_data[];
};

enum {
	SOCK_CONN_REQ,
	SOCK_CONN_ACCEPT,
	SOCK_CONN_REJECT,
	SOCK_CONN_SHUTDOWN,
};

enum sock_conn_handle_state {
	SOCK_CONN_HANDLE_ACTIVE,
	SOCK_CONN_HANDLE_ACCEPTED,
	SOCK_CONN_HANDLE_REJECTED,
	SOCK_CONN_HANDLE_DELETED,
	SOCK_CONN_HANDLE_FINALIZING,
	SOCK_CONN_HANDLE_FINALIZED,
};

struct sock_conn_req_handle {
	struct fid handle;
	struct sock_conn_req *req;
	int sock_fd;
	uint8_t monitored;
	enum sock_conn_handle_state state;
	pthread_mutex_t	finalized_mutex;
	pthread_cond_t	finalized_cond;
	struct sock_pep *pep;
	struct sock_ep *ep;
	size_t paramlen;
	union ofi_sock_ip dest_addr;
	struct dlist_entry entry;
	char cm_data[SOCK_EP_MAX_CM_DATA_SZ];
};

union sock_tx_op {
	struct sock_msg {
		struct sock_op_send op;
		uint64_t cq_data;
		union {
			char inject[SOCK_EP_MAX_INJECT_SZ];
			union sock_iov msg[SOCK_EP_MAX_IOV_LIMIT];
		} data;
	} msg;

	struct sock_rma_write {
		struct sock_op_send op;
		union {
			char inject[SOCK_EP_MAX_INJECT_SZ];
			union sock_iov msg[SOCK_EP_MAX_IOV_LIMIT];
		} data;
		union sock_iov rma[SOCK_EP_MAX_IOV_LIMIT];
	} rma_write;

	struct sock_rma_read {
		struct sock_op_send op;
		union sock_iov msg[SOCK_EP_MAX_IOV_LIMIT];
		union sock_iov rma[SOCK_EP_MAX_IOV_LIMIT];
	} rma_read;

	struct sock_atomic {
		struct sock_op_send op;
		union {
			char inject[SOCK_EP_MAX_INJECT_SZ];
			union sock_iov msg[SOCK_EP_MAX_IOV_LIMIT];
		} data;
		union sock_iov rma[SOCK_EP_MAX_IOV_LIMIT];
		union sock_iov res[SOCK_EP_MAX_IOV_LIMIT];
	} atomic;
};
#define SOCK_EP_TX_ENTRY_SZ (sizeof(union sock_tx_op))

size_t sock_get_tx_size(size_t size);
int sock_get_src_addr(union ofi_sock_ip *dest_addr,
		      union ofi_sock_ip *src_addr);
int sock_get_src_addr_from_hostname(union ofi_sock_ip *src_addr,
				    const char *service, uint16_t sa_family);

struct fi_info *sock_fi_info(uint32_t version, enum fi_ep_type ep_type,
			     const struct fi_info *hints, void *src_addr,
			     void *dest_addr);
void free_fi_info(struct fi_info *info);

int sock_msg_getinfo(uint32_t version, const char *node, const char *service,
		uint64_t flags, const struct fi_info *hints,
		struct fi_info **info);

int sock_domain(struct fid_fabric *fabric, struct fi_info *info,
		struct fid_domain **dom, void *context);
void sock_dom_add_to_list(struct sock_domain *domain);
int sock_dom_check_list(struct sock_domain *domain);
void sock_dom_remove_from_list(struct sock_domain *domain);
struct sock_domain *sock_dom_list_head(void);
int sock_dom_check_manual_progress(struct sock_fabric *fabric);
int sock_query_atomic(struct fid_domain *domain,
		      enum fi_datatype datatype, enum fi_op op,
		      struct fi_atomic_attr *attr, uint64_t flags);

void sock_fab_add_to_list(struct sock_fabric *fabric);
int sock_fab_check_list(struct sock_fabric *fabric);
void sock_fab_remove_from_list(struct sock_fabric *fabric);
struct sock_fabric *sock_fab_list_head(void);

int sock_alloc_endpoint(struct fid_domain *domain, struct fi_info *info,
			struct sock_ep **ep, void *context, size_t fclass);
int sock_rdm_ep(struct fid_domain *domain, struct fi_info *info,
		struct fid_ep **ep, void *context);
int sock_rdm_sep(struct fid_domain *domain, struct fi_info *info,
		 struct fid_ep **sep, void *context);

int sock_dgram_ep(struct fid_domain *domain, struct fi_info *info,
		  struct fid_ep **ep, void *context);
int sock_dgram_sep(struct fid_domain *domain, struct fi_info *info,
		   struct fid_ep **sep, void *context);

int sock_msg_ep(struct fid_domain *domain, struct fi_info *info,
		struct fid_ep **ep, void *context);
int sock_msg_sep(struct fid_domain *domain, struct fi_info *info,
		 struct fid_ep **sep, void *context);
int sock_msg_passive_ep(struct fid_fabric *fabric, struct fi_info *info,
			struct fid_pep **pep, void *context);
int sock_ep_enable(struct fid_ep *ep);
int sock_ep_disable(struct fid_ep *ep);

int sock_stx_ctx(struct fid_domain *domain,
		 struct fi_tx_attr *attr, struct fid_stx **stx, void *context);
int sock_srx_ctx(struct fid_domain *domain,
		 struct fi_rx_attr *attr, struct fid_ep **srx, void *context);


int sock_cq_open(struct fid_domain *domain, struct fi_cq_attr *attr,
		 struct fid_cq **cq, void *context);
int sock_cq_report_error(struct sock_cq *cq, struct sock_pe_entry *entry,
			 size_t olen, int err, int prov_errno, void *err_data,
			 size_t err_data_size);
int sock_cq_progress(struct sock_cq *cq);
void sock_cq_add_tx_ctx(struct sock_cq *cq, struct sock_tx_ctx *tx_ctx);
void sock_cq_remove_tx_ctx(struct sock_cq *cq, struct sock_tx_ctx *tx_ctx);
void sock_cq_add_rx_ctx(struct sock_cq *cq, struct sock_rx_ctx *rx_ctx);
void sock_cq_remove_rx_ctx(struct sock_cq *cq, struct sock_rx_ctx *rx_ctx);


int sock_eq_open(struct fid_fabric *fabric, struct fi_eq_attr *attr,
		struct fid_eq **eq, void *context);
int sock_eq_report_event(struct sock_eq *sock_eq, uint32_t event,
			 const void *buf, size_t len, uint64_t flags);
int sock_eq_report_error(struct sock_eq *sock_eq, fid_t fid, void *context,
			 uint64_t data, int err, int prov_errno,
			 void *err_data, size_t err_data_size);
int sock_eq_openwait(struct sock_eq *eq, const char *service);

int sock_cntr_open(struct fid_domain *domain, struct fi_cntr_attr *attr,
		   struct fid_cntr **cntr, void *context);
void sock_cntr_inc(struct sock_cntr *cntr);
int sock_cntr_progress(struct sock_cntr *cntr);
void sock_cntr_add_tx_ctx(struct sock_cntr *cntr, struct sock_tx_ctx *tx_ctx);
void sock_cntr_remove_tx_ctx(struct sock_cntr *cntr, struct sock_tx_ctx *tx_ctx);
void sock_cntr_add_rx_ctx(struct sock_cntr *cntr, struct sock_rx_ctx *rx_ctx);
void sock_cntr_remove_rx_ctx(struct sock_cntr *cntr, struct sock_rx_ctx *rx_ctx);


struct sock_mr *sock_mr_verify_key(struct sock_domain *domain, uint64_t key,
				   uintptr_t *buf, size_t len, uint64_t access);
struct sock_mr *sock_mr_verify_desc(struct sock_domain *domain, void *desc,
				    void *buf, size_t len, uint64_t access);

struct sock_rx_ctx *sock_rx_ctx_alloc(const struct fi_rx_attr *attr,
				      void *context, int use_shared);
void sock_rx_ctx_free(struct sock_rx_ctx *rx_ctx);

struct sock_tx_ctx *sock_tx_ctx_alloc(const struct fi_tx_attr *attr,
				      void *context, int use_shared);
struct sock_tx_ctx *sock_stx_ctx_alloc(const struct fi_tx_attr *attr, void *context);
void sock_tx_ctx_free(struct sock_tx_ctx *tx_ctx);
void sock_tx_ctx_start(struct sock_tx_ctx *tx_ctx);
void sock_tx_ctx_write(struct sock_tx_ctx *tx_ctx, const void *buf, size_t len);
void sock_tx_ctx_commit(struct sock_tx_ctx *tx_ctx);
void sock_tx_ctx_abort(struct sock_tx_ctx *tx_ctx);
void sock_tx_ctx_write_op_send(struct sock_tx_ctx *tx_ctx,
		struct sock_op *op, uint64_t flags, uint64_t context,
		uint64_t dest_addr, uint64_t buf, struct sock_ep_attr *ep_attr,
		struct sock_conn *conn);
void sock_tx_ctx_write_op_tsend(struct sock_tx_ctx *tx_ctx,
		struct sock_op *op, uint64_t flags, uint64_t context,
		uint64_t dest_addr, uint64_t buf, struct sock_ep_attr *ep_attr,
		struct sock_conn *conn, uint64_t tag);
void sock_tx_ctx_read_op_send(struct sock_tx_ctx *tx_ctx,
		struct sock_op *op, uint64_t *flags, uint64_t *context,
		uint64_t *dest_addr, uint64_t *buf, struct sock_ep_attr **ep_attr,
		struct sock_conn **conn);

int sock_poll_open(struct fid_domain *domain, struct fi_poll_attr *attr,
		   struct fid_poll **pollset);
int sock_wait_open(struct fid_fabric *fabric, struct fi_wait_attr *attr,
		   struct fid_wait **waitset);
void sock_wait_signal(struct fid_wait *wait_fid);
int sock_wait_get_obj(struct fid_wait *fid, void *arg);
int sock_wait_close(fid_t fid);


int sock_av_open(struct fid_domain *domain, struct fi_av_attr *attr,
		 struct fid_av **av, void *context);
int sock_av_compare_addr(struct sock_av *av, fi_addr_t addr1, fi_addr_t addr2);
int sock_av_get_addr_index(struct sock_av *av, union ofi_sock_ip *addr);

struct sock_conn *sock_ep_lookup_conn(struct sock_ep_attr *attr, fi_addr_t index,
				      union ofi_sock_ip *addr);
int sock_ep_get_conn(struct sock_ep_attr *ep_attr, struct sock_tx_ctx *tx_ctx,
		     fi_addr_t index, struct sock_conn **pconn);
void sock_ep_remove_conn(struct sock_ep_attr *ep_attr, struct sock_conn *conn);
int sock_ep_connect(struct sock_ep_attr *attr, fi_addr_t index,
		    struct sock_conn **conn);
ssize_t sock_conn_send_src_addr(struct sock_ep_attr *ep_attr, struct sock_tx_ctx *tx_ctx,
				struct sock_conn *conn);
int sock_conn_listen(struct sock_ep_attr *ep_attr);
int sock_conn_start_listener_thread(struct sock_conn_listener *conn_listener);
int sock_conn_stop_listener_thread(struct sock_conn_listener *conn_listener);
void sock_conn_map_destroy(struct sock_ep_attr *ep_attr);
void sock_conn_release_entry(struct sock_conn_map *map, struct sock_conn *conn);
void sock_set_sockopts(int sock, int sock_opts);
int fd_set_nonblock(int fd);
int sock_conn_map_init(struct sock_ep *ep, int init_size);

struct sock_pe *sock_pe_init(struct sock_domain *domain);
void sock_pe_add_tx_ctx(struct sock_pe *pe, struct sock_tx_ctx *ctx);
void sock_pe_add_rx_ctx(struct sock_pe *pe, struct sock_rx_ctx *ctx);
void sock_pe_signal(struct sock_pe *pe);
void sock_pe_poll_add(struct sock_pe *pe, int fd);
void sock_pe_poll_del(struct sock_pe *pe, int fd);

int sock_pe_progress_ep_rx(struct sock_pe *pe, struct sock_ep_attr *ep_attr);
int sock_pe_progress_ep_tx(struct sock_pe *pe, struct sock_ep_attr *ep_attr);
int sock_pe_progress_rx_ctx(struct sock_pe *pe, struct sock_rx_ctx *rx_ctx);
int sock_pe_progress_tx_ctx(struct sock_pe *pe, struct sock_tx_ctx *tx_ctx);
void sock_pe_remove_tx_ctx(struct sock_tx_ctx *tx_ctx);
void sock_pe_remove_rx_ctx(struct sock_rx_ctx *rx_ctx);
void sock_pe_finalize(struct sock_pe *pe);


struct sock_rx_entry *sock_rx_new_entry(struct sock_rx_ctx *rx_ctx);
struct sock_rx_entry *sock_rx_new_buffered_entry(struct sock_rx_ctx *rx_ctx,
						 size_t len);
struct sock_rx_entry *sock_rx_get_entry(struct sock_rx_ctx *rx_ctx,
					uint64_t addr, uint64_t tag,
					uint8_t is_tagged);
struct sock_rx_entry *sock_rx_get_buffered_entry(struct sock_rx_ctx *rx_ctx,
						 uint64_t addr, uint64_t tag,
						 uint64_t ignore, uint8_t is_tagged);
ssize_t sock_rx_peek_recv(struct sock_rx_ctx *rx_ctx, fi_addr_t addr,
			  uint64_t tag, uint64_t ignore, void *context, uint64_t flags,
			  uint8_t is_tagged);
ssize_t sock_rx_claim_recv(struct sock_rx_ctx *rx_ctx, void *context,
			   uint64_t flags, uint64_t tag, uint64_t ignore,
			   uint8_t is_tagged, const struct iovec *msg_iov,
			   size_t iov_count);
void sock_rx_release_entry(struct sock_rx_entry *rx_entry);

ssize_t sock_comm_send(struct sock_pe_entry *pe_entry, const void *buf, size_t len);
ssize_t sock_comm_recv(struct sock_pe_entry *pe_entry, void *buf, size_t len);
ssize_t sock_comm_peek(struct sock_conn *conn, void *buf, size_t len);
ssize_t sock_comm_discard(struct sock_pe_entry *pe_entry, size_t len);
int sock_comm_tx_done(struct sock_pe_entry *pe_entry);
ssize_t sock_comm_flush(struct sock_pe_entry *pe_entry);
int sock_comm_is_disconnected(struct sock_pe_entry *pe_entry);

ssize_t sock_ep_recvmsg(struct fid_ep *ep, const struct fi_msg *msg,
			uint64_t flags);
ssize_t sock_ep_sendmsg(struct fid_ep *ep, const struct fi_msg *msg,
			uint64_t flags);
ssize_t sock_ep_trecvmsg(struct fid_ep *ep,
			 const struct fi_msg_tagged *msg, uint64_t flags);
ssize_t sock_ep_tsendmsg(struct fid_ep *ep,
			 const struct fi_msg_tagged *msg, uint64_t flags);
ssize_t sock_ep_rma_readmsg(struct fid_ep *ep, const struct fi_msg_rma *msg,
			    uint64_t flags);
ssize_t sock_ep_rma_writemsg(struct fid_ep *ep, const struct fi_msg_rma *msg,
			     uint64_t flags);
ssize_t sock_ep_tx_atomic(struct fid_ep *ep,
			  const struct fi_msg_atomic *msg,
			  const struct fi_ioc *comparev, void **compare_desc,
			  size_t compare_count, struct fi_ioc *resultv,
			  void **result_desc, size_t result_count, uint64_t flags);

ssize_t sock_queue_work(struct sock_domain *dom, struct fi_deferred_work *work);
ssize_t sock_queue_rma_op(struct fid_ep *ep, const struct fi_msg_rma *msg,
			  uint64_t flags, enum fi_op_type op_type);
ssize_t sock_queue_atomic_op(struct fid_ep *ep, const struct fi_msg_atomic *msg,
			     const struct fi_ioc *comparev, size_t compare_count,
			     struct fi_ioc *resultv, size_t result_count,
			     uint64_t flags, enum fi_op_type op_type);
ssize_t sock_queue_tmsg_op(struct fid_ep *ep, const struct fi_msg_tagged *msg,
			   uint64_t flags, enum fi_op_type op_type);
ssize_t sock_queue_msg_op(struct fid_ep *ep, const struct fi_msg *msg,
			  uint64_t flags, enum fi_op_type op_type);
ssize_t sock_queue_cntr_op(struct fi_deferred_work *work, uint64_t flags);
void sock_cntr_check_trigger_list(struct sock_cntr *cntr);

static inline size_t sock_rx_avail_len(struct sock_rx_entry *rx_entry)
{
	return rx_entry->total_len - rx_entry->used;
}

int sock_ep_cm_start_thread(struct sock_ep_cm_head *cm_head);
void sock_ep_cm_signal(struct sock_ep_cm_head *cm_head);
void sock_ep_cm_stop_thread(struct sock_ep_cm_head *cm_head);
void sock_ep_cm_wait_handle_finalized(struct sock_ep_cm_head *cm_head,
                                      struct sock_conn_req_handle *handle);

#endif
