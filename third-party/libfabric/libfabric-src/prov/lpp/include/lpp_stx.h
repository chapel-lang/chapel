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

#ifndef _LPP_STX_H_
#define _LPP_STX_H_

#include "lpp.h"

#define	LPP_TX_ENTRY_SRQ	(0x01UL)
#define LPP_TX_ENTRY_ONLIST	(0x02UL)
#define LPP_TX_ENTRY_CQ_RSRV	(0x04UL)
#define LPP_TX_ENTRY_RDZV_PEND	(0x08UL)
#define LPP_TX_ENTRY_GPU	(0x10UL)
#define LPP_TX_ENTRY_MR		(0x20UL)

struct lpp_tx_entry {
	struct dlist_entry		dle;
	void				*context;
	uint64_t			flags;
	uint64_t			lpp_flags;
	void*				gpu_stage_buf;
	uint32_t			generation;
	union lpp_fi_addr_storage	dst_addr;
	struct iovec			msg_iov[KLPP_MAX_SEND_IOVS];
	struct lpp_mr			*mrp[KLPP_MAX_SEND_IOVS];
	size_t				iov_count;
	struct lpp_atomic_hdr		atomic_hdr;
	struct iovec			atomic_result;
	// TODO: make a buf_pool to save space?
	uint8_t				inject_data[LPP_MAX_INJECT];
	enum fi_hmem_iface		hmem_iface;
	struct klpp_msg_hdr		msg_hdr;
};

struct lpp_stx {
	struct fid_stx		stx_fid;
	struct fi_tx_attr	attr;

	struct lpp_ep		*ep;
	struct lpp_cq		*cq;
	struct lpp_cntr		*cntr;
	struct lpp_domain	*domain;

	ofi_atomic32_t		ref_count;

	struct lpp_ctx		ctx;

	klpp_id_t		id;
	int			is_enabled;

	struct lpp_tx_entry	*tx_entries;
	struct dlist_entry	tx_entry_freelist;
	struct dlist_entry	tx_entry_unsent;
	struct dlist_entry	tx_entry_atomic_unsent;
	struct dlist_entry	tx_entry_dqp;
	struct dlist_entry	tx_entry_srq;
};

void lpp_stx_bind_ep(struct lpp_stx *lpp_stxp, struct lpp_ep *lpp_epp);
void lpp_stx_unbind_ep(struct lpp_stx *lpp_stxp);

struct fi_tx_attr lpp_tx_attrs(const struct klppioc_lf *klpp_devinfo);
int lpp_verify_tx_attrs(const struct klppioc_lf *klpp_devinfo, struct fi_tx_attr *tx_attrs);

int lpp_fi_stx_context(struct fid_domain *domain, struct fi_tx_attr *attr, struct fid_stx **stx, void *context);
int lpp_stx_close(struct lpp_stx *lpp_stxp);
int lpp_fi_stx_close(struct fid *fid);

struct lpp_stx *lpp_stx_alloc(struct lpp_domain *domain, struct fi_tx_attr *attr, void *context);

int lpp_fi_stx_context(struct fid_domain *domain, struct fi_tx_attr *attr, struct fid_stx **stx, void *context);
int lpp_fi_stx_close(struct fid *stx);
struct lpp_tx_entry *lpp_tx_entry_get(struct lpp_stx *lpp_stxp);
uint64_t lpp_tx_entry_to_token(struct lpp_stx *lpp_stxp,
			       struct lpp_tx_entry *tx_entry);
ssize_t lpp_send_common(struct lpp_ep *lpp_epp, struct lpp_tx_entry *tx_entry);
ssize_t lpp_tx_verify_flags(struct lpp_stx *lpp_stxp, uint64_t flags);
ssize_t lpp_tx_verify_iov_count(struct lpp_stx *lpp_stxp, size_t iov_count);
ssize_t lpp_tx_verify_rma_iov_count(struct lpp_stx *lpp_stxp, size_t iov_count);
void lpp_tx_process_deliv_comp(struct lpp_ep *lpp_epp, struct klpp_msg_hdr *hdr);
void lpp_stx_rdzv_done(struct lpp_ep *lpp_epp, struct klpp_umc_k2u *k2u);
void lpp_tx_process_discard(struct lpp_ep *lpp_epp, struct klpp_msg_hdr *hdr);
void lpp_stx_path_down(struct lpp_ep *lpp_epp, int remote_node_id, int remote_umc_id);
void lpp_tx_process_invalid_dst(struct lpp_ep *lpp_epp, struct klpp_msg_hdr *hdr);
void lpp_stx_update_rx_time(struct lpp_ep *lpp_epp, struct lpp_fi_addr remote_addr);
int lpp_stx_flush_unsent_all(struct lpp_ep *lpp_epp);
ssize_t lpp_atomic_common(struct lpp_ep *lpp_epp, struct lpp_tx_entry *tx_entry);
void lpp_tx_process_atomic_deliv_comp(struct lpp_ep *lpp_epp, struct klpp_msg_hdr *hdr);

static inline uint64_t lpp_stx_msg_hdr_flags(uint64_t user_op_flags,
					     uint64_t required_hdr_flags,
					     struct lpp_stx *lpp_stxp)
{
	/* Let's honor the user_op_flag instead of always defaulting
	 * to completion, there was a comment here mentioning the per-operation
	 * use of TRANSMIT_COMPLETE, which resulted in issues with running the
	 * "concurrent_spawns" test. But it's unclear if that's an issue with MPI.
	 */
	if (user_op_flags &
	     (FI_DELIVERY_COMPLETE | FI_TRANSMIT_COMPLETE))
		return required_hdr_flags | KLPP_MSG_TOKEN;
	else
		return required_hdr_flags;
}

#endif //  _LPP_STX_H_
