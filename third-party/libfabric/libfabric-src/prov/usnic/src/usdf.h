/*
 * Copyright (c) 2014-2019, Cisco Systems, Inc. All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef _USDF_H_
#define _USDF_H_

#include <config.h>

#include <sys/queue.h>
#include <pthread.h>

#include <rdma/providers/fi_log.h>
#include <ofi_epoll.h>

#include "usdf_progress.h"
#include "usd.h"


#define USDF_PROV_NAME "usnic"
#define USDF_MAJOR_VERS 1
#define USDF_MINOR_VERS 0
#define USDF_PROV_VERSION FI_VERSION(USDF_MAJOR_VERS, USDF_MINOR_VERS)

extern struct fi_provider usdf_ops;

#define USDF_WARN_SYS(subsys, ...) \
	FI_WARN(&usdf_ops, FI_LOG_ ## subsys, __VA_ARGS__)
#define USDF_TRACE_SYS(subsys, ...) \
	FI_TRACE(&usdf_ops, FI_LOG_ ## subsys, __VA_ARGS__)
#define USDF_INFO_SYS(subsys, ...) \
	FI_INFO(&usdf_ops, FI_LOG_ ## subsys, __VA_ARGS__)
#define USDF_DBG_SYS(subsys, ...) \
	FI_DBG(&usdf_ops, FI_LOG_ ## subsys, __VA_ARGS__)

/* default to "FI_LOG_FABRIC" */
#define USDF_WARN(...) USDF_WARN_SYS(FABRIC, __VA_ARGS__)
#define USDF_TRACE(...) USDF_TRACE_SYS(FABRIC, __VA_ARGS__)
#define USDF_INFO(...) USDF_INFO_SYS(FABRIC, __VA_ARGS__)
#define USDF_DBG(...)  USDF_DBG_SYS(FABRIC, __VA_ARGS__)

#define USDF_HDR_BUF_ENTRY 64
#define USDF_EP_CAP_PIO (1ULL << 63)

#define USDF_MAX_PEERS (16 * 1024)

/* usdf event flags */
#define USDF_EVENT_FLAG_ERROR (1ULL << 62)
#define USDF_EVENT_FLAG_FREE_BUF (1ULL << 63)

/* usdf domain capability: no loopback */
#define USDF_DOM_CAPS (FI_REMOTE_COMM)

#define USDF_MR_IOV_LIMIT 1
#define USDF_MR_CNT (65535)
#define USDF_ADDR_STR_LEN (INET6_ADDRSTRLEN+8)

/*
 *  TAILQ stuff that should exist
 */
#define TAILQ_REMOVE_MARK(head, elm, link)	\
	do {					\
		TAILQ_REMOVE(head, elm, link);	\
		(elm)->link.tqe_prev = NULL;    \
	} while (0)

#define TAILQ_ON_LIST(elm, link) ((elm)->link.tqe_prev != NULL)

struct usdf_domain;

struct usdf_dev_entry {
	struct usd_device *ue_dev;
	struct usd_device_attrs ue_dattr;
	int ue_dev_ok;
};
struct usdf_usnic_info {
	int uu_num_devs;
	struct usd_device_entry uu_devs[USD_MAX_DEVICES];
	struct usdf_dev_entry uu_info[USD_MAX_DEVICES];
};
extern struct usdf_usnic_info *__usdf_devinfo;

struct usdf_fabric {
	struct fid_fabric   fab_fid;
	struct fi_fabric_attr fab_attr;
	struct usd_device_attrs *fab_dev_attrs;
	int fab_arp_sockfd;
	ofi_atomic32_t fab_refcnt;
	ofi_atomic32_t num_blocked_waiting;
	LIST_HEAD(,usdf_domain) fab_domain_list;

	/* progression */
	pthread_t fab_thread;
	int fab_exit;
	ofi_epoll_t fab_epollfd;
	int fab_eventfd;
	struct usdf_poll_item fab_poll_item;

	/* timer vars */
	uint32_t fab_active_timer_count;
	LIST_HEAD(usdf_timer_bucket, usdf_timer_entry) *fab_timer_buckets;
	uint64_t fab_cur_bucket_ms;
	uint32_t fab_cur_bucket;
	pthread_spinlock_t fab_timer_lock;
};
#define fab_ftou(FAB) container_of(FAB, struct usdf_fabric, fab_fid)
#define fab_utof(FP) (&(FP)->fab_fid)
#define fab_fidtou(FID) container_of(FID, struct usdf_fabric, fab_fid.fid)

struct usdf_domain {
	struct fid_domain   dom_fid;
	struct usdf_fabric *dom_fabric;
	struct fi_info *dom_info;
	ofi_atomic32_t dom_refcnt;
	struct usdf_eq *dom_eq;
	struct usd_device   *dom_dev;

	pthread_spinlock_t dom_progress_lock;
	TAILQ_HEAD(,usdf_tx) dom_tx_ready;
	TAILQ_HEAD(,usdf_cq_hard) dom_hcq_list;

	/* used only by connected endpoints */
	struct usdf_ep **dom_peer_tab;
	uint32_t dom_next_peer;

	LIST_ENTRY(usdf_domain) dom_link;
};
#define dom_ftou(FDOM) container_of(FDOM, struct usdf_domain, dom_fid)
#define dom_utof(DOM) (&(DOM)->dom_fid)
#define dom_fidtou(FID) container_of(FID, struct usdf_domain, dom_fid.fid)

enum usdf_pep_state {
	USDF_PEP_UNBOUND,
	USDF_PEP_BOUND,
	USDF_PEP_LISTENING,

	/* A "ROBBED" PEP has had its socket stolen.  The only valid operation
	 * to call on a ROBBED PEP is fi_close(). */
	USDF_PEP_ROBBED
};

struct usdf_pep {
	struct fid_pep pep_fid;
	ofi_atomic32_t pep_refcnt;
	struct usdf_fabric *pep_fabric;
	struct usdf_eq *pep_eq;
	int pep_sock;
	union {
		struct sockaddr_in sin;
		char addr_str[USDF_ADDR_STR_LEN];
	} pep_src_addr;
	enum usdf_pep_state pep_state;
	struct usdf_poll_item pep_pollitem;
	struct fi_info *pep_info;

	pthread_spinlock_t pep_cr_lock;
	size_t pep_cr_max_data;
	uint32_t pep_backlog;
	uint32_t pep_cr_alloced;
	TAILQ_HEAD(,usdf_connreq) pep_cr_free;
	TAILQ_HEAD(,usdf_connreq) pep_cr_pending;
};
#define pep_ftou(FPEP) container_of(FPEP, struct usdf_pep, pep_fid)
#define pep_fidtou(FID) container_of(FID, struct usdf_pep, pep_fid.fid)
#define pep_utof(PEP) (&(PEP)->pep_fid)
#define pep_utofid(PEP) (&(PEP)->pep_fid.fid)

struct usdf_tx {
	struct fid_stx tx_fid;
	ofi_atomic32_t tx_refcnt;
	struct usdf_domain *tx_domain;
	TAILQ_ENTRY(usdf_tx) tx_link;

	struct fi_tx_attr tx_attr;
	struct usd_qp *tx_qp;
	void (*tx_progress)(struct usdf_tx *tx);

	union {
		struct {
			struct usdf_cq_hard *tx_hcq;

			uint8_t *tx_inject_bufs;
			struct usdf_msg_qe *tx_wqe_buf;
			TAILQ_HEAD(,usdf_msg_qe) tx_free_wqe;
			TAILQ_HEAD(,usdf_ep) tx_ep_ready;
			TAILQ_HEAD(,usdf_ep) tx_ep_have_acks;
			size_t tx_num_free_wqe;
		} msg;
		struct {
			struct usdf_cq_hard *tx_hcq;

			ofi_atomic32_t tx_next_msg_id;
			struct usdf_rdm_qe *tx_wqe_buf;
			uint8_t *tx_inject_bufs;
			TAILQ_HEAD(,usdf_rdm_qe) tx_free_wqe;
			TAILQ_HEAD(,usdf_rdm_connection) tx_rdc_ready;
			TAILQ_HEAD(,usdf_rdm_connection) tx_rdc_have_acks;
			size_t tx_num_free_wqe;
		} rdm;
	} t;
};
#define tx_ftou(FEP) container_of(FEP, struct usdf_tx, tx_fid)
#define tx_fidtou(FID) container_of(FID, struct usdf_tx, tx_fid)
#define tx_utof(RX) (&(RX)->tx_fid)
#define tx_utofid(RX) (&(RX)->tx_fid.fid)

struct usdf_rx {
	struct fid_ep rx_fid;
	ofi_atomic32_t rx_refcnt;
	struct usdf_domain *rx_domain;

	struct fi_rx_attr rx_attr;
	struct usd_qp *rx_qp;

	union {
		struct {
			struct usdf_cq_hard *rx_hcq;

			uint8_t *rx_bufs;
			struct usdf_msg_qe *rx_rqe_buf;
			TAILQ_HEAD(,usdf_msg_qe) rx_free_rqe;
			TAILQ_HEAD(,usdf_msg_qe) rx_posted_rqe;
			size_t rx_num_free_rqe;
		} msg;
		struct {
			int rx_sock;
			struct usdf_cq_hard *rx_hcq;
			struct usdf_tx *rx_tx;

			uint8_t *rx_bufs;
			struct usdf_rdm_qe *rx_rqe_buf;
			TAILQ_HEAD(,usdf_rdm_qe) rx_free_rqe;
			TAILQ_HEAD(,usdf_rdm_qe) rx_posted_rqe;
			size_t rx_num_free_rqe;
		} rdm;
	} r;
};
#define rx_ftou(FEP) container_of(FEP, struct usdf_rx, rx_fid)
#define rx_fidtou(FID) container_of(FID, struct usdf_rx, rx_fid)
#define rx_utof(RX) (&(RX)->rx_fid)
#define rx_utofid(RX) (&(RX)->rx_fid.fid)

enum {
	USDF_EP_ENABLED = (1 << 0)
};

struct usdf_ep {
	struct fid_ep ep_fid;
	struct usdf_domain *ep_domain;
	ofi_atomic32_t ep_refcnt;
	uint64_t ep_caps;
	uint64_t ep_mode;

	uint8_t ep_tx_dflt_signal_comp;
	uint8_t ep_rx_dflt_signal_comp;

	uint8_t ep_tx_completion;
	uint8_t ep_rx_completion;

	uint32_t flags;

	uint32_t ep_wqe;	/* requested queue sizes */
	uint32_t ep_rqe;

	struct usd_qp_attrs ep_qp_attrs;

	struct usdf_eq *ep_eq;

	struct usdf_tx *ep_tx;
	struct usdf_rx *ep_rx;

	size_t max_msg_size;

	union {
		struct {
			struct usd_qp *ep_qp;
			struct usdf_cq *ep_wcq;
			struct usdf_cq *ep_rcq;

			int ep_sock;
			struct usdf_av *ep_av;

			/* TODO: Remove in favor of accessing op flags through
			 * ep_tx and ep_rx. Update once tx/rx context support
			 * is added to dgram */
			uint64_t tx_op_flags;
			uint64_t rx_op_flags;

			size_t tx_iov_limit;
			size_t rx_iov_limit;

			void *ep_hdr_buf;
			struct usd_udp_hdr **ep_hdr_ptr;
		} dg;
		struct {
			struct usdf_connreq *ep_connreq;
			int ep_cm_sock;
			struct sockaddr_in ep_lcl_addr;
			struct usd_dest *ep_dest;
			uint32_t ep_rem_peer_id;
			uint32_t ep_lcl_peer_id;

			TAILQ_HEAD(,usdf_msg_qe) ep_posted_wqe;
			TAILQ_HEAD(usdf_msg_qe_head ,usdf_msg_qe) ep_sent_wqe;
			uint32_t ep_fairness_credits;
			uint32_t ep_seq_credits;
			uint16_t ep_next_tx_seq;
			uint16_t ep_last_rx_ack;
			int ep_send_nak;

			struct usdf_msg_qe *ep_cur_recv;
			uint16_t ep_next_rx_seq;
			TAILQ_ENTRY(usdf_ep) ep_ack_link;

			struct usdf_timer_entry *ep_ack_timer;

			TAILQ_ENTRY(usdf_ep) ep_link;
		} msg;
		struct {
			int ep_sock;
			struct usdf_av *ep_av;

		} rdm;
	 } e;
};
#define ep_ftou(FEP) container_of(FEP, struct usdf_ep, ep_fid)
#define ep_fidtou(FID) container_of(FID, struct usdf_ep, ep_fid.fid)
#define ep_utof(EP) (&(EP)->ep_fid)
#define ep_utofid(EP) (&(EP)->ep_fid.fid)

struct usdf_mr {
	struct fid_mr mr_fid;
	struct usd_mr *mr_mr;
};

struct usdf_cq_hard {
	struct usdf_cq *cqh_cq;
	struct usd_cq *cqh_ucq;
	ofi_atomic32_t cqh_refcnt;
	void (*cqh_progress)(struct usdf_cq_hard *hcq);
	void (*cqh_post)(struct usdf_cq_hard *hcq, void *context, size_t len,
			int prov_errno, uint64_t flags);
	TAILQ_ENTRY(usdf_cq_hard) cqh_link;
	TAILQ_ENTRY(usdf_cq_hard) cqh_dom_link;
};

struct usdf_cq_soft_entry {
	void		*cse_context;
	uint64_t	cse_flags;
	size_t		cse_len;
	void		*cse_buf;
	uint64_t	cse_data;
	int		cse_prov_errno;
};

struct usdf_cq {
	struct fid_cq cq_fid;
	ofi_atomic32_t cq_refcnt;
	struct usdf_domain *cq_domain;
	struct fi_cq_attr cq_attr;
	uint8_t cq_is_soft;
	uint8_t cq_waiting;

	union {
		int fd;
		struct fi_mutex_cond mutex_cond;
	} object;

	union {
		struct {
			struct usd_cq *cq_cq;
		} hard;
		struct {
			struct usdf_cq_soft_entry *cq_comps;
			struct usdf_cq_soft_entry *cq_end;
			struct usdf_cq_soft_entry *cq_head;
			struct usdf_cq_soft_entry *cq_tail;
			/* Last operation used to distinguish full vs empty. */
			uint8_t cq_last_op;
			TAILQ_HEAD(,usdf_cq_hard) cq_list;
		} soft;
	} c;
	struct usd_completion cq_comp;
	struct fi_ops_cq cq_ops;
};

enum {
	USDF_SOFT_CQ_READ,
	USDF_SOFT_CQ_WRITE
};

#define cq_ftou(FCQ) container_of(FCQ, struct usdf_cq, cq_fid)
#define cq_fidtou(FID) container_of(FID, struct usdf_cq, cq_fid.fid)
#define cq_utof(CQ) (&(CQ)->cq_fid)

struct usdf_err_data_entry {
	struct slist_entry entry;
	uint8_t seen;
	uint8_t err_data[];
};

struct usdf_event {
	uint32_t ue_event;
	void *ue_buf;
	size_t ue_len;
	uint64_t ue_flags;
};

struct usdf_eq {
	struct fid_eq eq_fid;
	struct usdf_fabric *eq_fabric;
	ofi_atomic32_t eq_refcnt;

	pthread_spinlock_t eq_lock;

	struct fi_eq_err_entry *eq_ev_buf;
	struct usdf_event *eq_ev_ring;
	struct usdf_event *eq_ev_head;
	struct usdf_event *eq_ev_tail;
	struct usdf_event *eq_ev_end;
	int eq_ev_ring_size;
	ofi_atomic32_t eq_num_events;

	/* various ways to wait */
	struct fi_eq_attr eq_attr;
	union {
		int eq_fd;
	};

	struct slist eq_err_data;
	struct fi_ops_eq eq_ops_data;
};
#define eq_ftou(FEQ) container_of(FEQ, struct usdf_eq, eq_fid)
#define eq_fidtou(FID) container_of(FID, struct usdf_eq, eq_fid.fid)
#define eq_utof(EQ) (&(EQ)->eq_fid)

/*
 * Prototypes
 */

ssize_t usdf_eq_write_internal(struct usdf_eq *eq, uint32_t event,
		const void *buf, size_t len, uint64_t flags);

/* fi_ops_fabric */
int usdf_domain_open(struct fid_fabric *fabric, struct fi_info *info,
	struct fid_domain **domain, void *context);
int usdf_eq_open(struct fid_fabric *fabric, struct fi_eq_attr *attr,
	struct fid_eq **eq, void *context);
int usdf_pep_open(struct fid_fabric *fabric, struct fi_info *info,
		struct fid_pep **pep_p, void *context);

/* fi_ops_domain */
int usdf_cq_open(struct fid_domain *domain, struct fi_cq_attr *attr,
		 struct fid_cq **cq_o, void *context);
int usdf_endpoint_open(struct fid_domain *domain, struct fi_info *info,
		struct fid_ep **ep, void *context);
int usdf_av_open(struct fid_domain *domain, struct fi_av_attr *attr,
		 struct fid_av **av_o, void *context);
int usdf_query_atomic(struct fid_domain *domain, enum fi_datatype datatype,
		enum fi_op op, struct fi_atomic_attr *attr, uint64_t flags);

/* Domain name functionality */
int usdf_domain_getname(uint32_t version, struct usd_device_attrs *dap,
			char **name);
bool usdf_domain_checkname(uint32_t version, struct usd_device_attrs *dap,
			const char *hint);

/* fi_ops_mr */
int usdf_reg_mr(struct fid *fid, const void *buf, size_t len,
		uint64_t access, uint64_t offset, uint64_t requested_key,
		uint64_t flags, struct fid_mr **mr_o, void *context);
int usdf_regv_mr(struct fid *fid, const struct iovec *iov,
		 size_t count, uint64_t access,
		 uint64_t offset, uint64_t requested_key,
		 uint64_t flags, struct fid_mr **mr, void *context);
int usdf_regattr(struct fid *fid, const struct fi_mr_attr *attr,
		 uint64_t flags, struct fid_mr **mr);

/* Fake IBV provider */
void usdf_setup_fake_ibv_provider(void);

/* passive endpoint functions */
int usdf_pep_steal_socket(struct usdf_pep *pep, int *is_bound, int *sock_o);

/* Utility functions */
int usdf_catch_dom_attr(uint32_t version, const struct fi_info *hints,
			struct fi_domain_attr *dom_attr);
int usdf_catch_tx_attr(uint32_t version, const struct fi_tx_attr *tx_attr);
int usdf_catch_rx_attr(uint32_t version, const struct fi_rx_attr *rx_attr);
struct sockaddr_in *usdf_format_to_sin(const struct fi_info *info, const void *addr);
void *usdf_sin_to_format(const struct fi_info *info, void *addr, size_t *len);
void usdf_free_sin_if_needed(const struct fi_info *info, struct sockaddr_in *sin);

#endif /* _USDF_H_ */
