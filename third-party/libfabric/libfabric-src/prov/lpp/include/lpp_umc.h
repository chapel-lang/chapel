/*
 * Copyright (c) 2018-2024 GigaIO, Inc. All Rights Reserved.
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

#ifndef _LPP_UMC_H_
#define _LPP_UMC_H_

#include "lpp.h"

#define LPP_DQP_UNSET		(-1)

struct lpp_umc_srq {
	struct klpp_mq		*mq;
	/*
	 * remote_mq is only used to update fields
	 *	- remote_cons_idx
	 */
	struct klpp_mq		*remote_mq;
};

#define LPP_DQP_ENABLED		(0x1)
#define LPP_DQP_NEW		(0x2)

struct lpp_umc_dqp {
	struct klpp_mq		*tx;
	uint64_t		tx_prod_idx;

	struct klpp_mq		*rx;
	uint64_t		rx_cons_idx;
	//uint64_t		rx_last_synced_cons_idx;

	struct lpp_fi_addr	remote_src_ids[KLPP_UMC_MAX_EP];

	uint64_t		flags;
	uint64_t		persistent_tag;
};

#define LPP_UMCR_PERST_TAG	(0x1)

struct lpp_umc_remote {
	struct dlist_entry	dle;
	uint32_t		node_id;
	uint32_t		umc_id;
	int16_t			dqp_idx;
	uint8_t			keep_alive_cnt;
	uint64_t		last_ka_time;
	uint8_t			umc_gen;
	uint32_t		srq_refcount;
	uint64_t		last_rx_time;
	uint64_t		persistent_tag;
	uint64_t		flags;
};

#define LPP_UMC_KSERVICE	(0x1)

typedef void (*lpp_umc_progfn)(struct lpp_ep *);
typedef void (*lpp_spool_cb)(struct lpp_ep *lpp_epp, void *data, int status);

struct lpp_spooled_msg_dqp {
	struct dlist_entry	dle;
	lpp_spool_cb		callback;
	struct iovec		iov;
	size_t			iov_count;
	struct lpp_fi_addr	addr;
	struct klpp_msg_hdr	msg_hdr;
};

struct lpp_spooled_msg_srq {
	lpp_spool_cb			callback;
	struct iovec			iov;
	struct klpp_ioc_kmc_send	ioc;
};

struct lpp_cmpl_spool_srq {
	struct klpp_ioc_kmc_send	*head;
	struct klpp_ioc_kmc_send	*tail;
	size_t				size;
};

struct lpp_umc {
	klpp_id_t		id;
	uint64_t		generation;

	struct index_map	rumc_idm;

	void			*rx_base;
	void			*tx_base;
	void			*k2u_base;
	void			*u2k_base;

	struct lpp_umc_srq	*srqs;
	struct lpp_umc_dqp	*dqps;
	struct klpp_ringbuf_k2u *k2u;
	struct klpp_ringbuf_u2k *u2k;

	uint64_t		flags;
	uint64_t		prog_time;
	struct dlist_entry	keep_alive_rumcs;
	lpp_umc_progfn		progfn;

	struct dlist_entry	cmpl_spool_dqp;
	struct lpp_cmpl_spool_srq cmpl_spool_srq;

	ofi_mutex_t		lock;
	int			xxx_poll_cnt;
};

int lpp_umc_acquire(struct lpp_ep *lpp_epp);
void lpp_umc_release(struct lpp_ep *lpp_epp, struct lpp_umc *umc);
int lpp_umc_tx_simple(struct lpp_ep *lpp_epp, struct lpp_fi_addr dst_addr,
		      struct klpp_msg_hdr *hdr, const struct iovec *iov,
		      size_t iov_count);
void lpp_umc_u2k_enqueue(struct lpp_ep *lpp_epp, struct klpp_umc_u2k *u2k);
int lpp_umc_dqp_tx(struct lpp_ep *lpp_epp, struct lpp_umc_remote *rumc,
		   struct klpp_msg_hdr *hdr, const struct iovec *iov,
		   size_t iov_count);
struct lpp_umc_remote *lpp_rumc_get(struct lpp_ep *lpp_epp, int node_id, int umc_id);
struct lpp_umc_remote *lpp_rumc_get_insert(struct lpp_ep *lpp_epp, int node_id,
					   int umc_id);
int lpp_umc_tx_cmpl(struct lpp_ep *lpp_epp, struct lpp_fi_addr addr,
		    struct klpp_msg_hdr *hdr, void *data, size_t data_len,
		    lpp_spool_cb callback, size_t cb_data_len, void **cb_data);

#endif /* _LPP_UMC_H_ */
