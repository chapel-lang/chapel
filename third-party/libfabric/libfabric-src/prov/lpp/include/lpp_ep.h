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

#ifndef _LPP_EP_H_
#define _LPP_EP_H_

#include "lpp.h"

struct lpp_ep {
	struct fid_ep		ep_fid;
	struct fi_ep_attr	attr;
	struct fi_tx_attr	tx_attr;
	struct fi_rx_attr	rx_attr;

	struct lpp_stx		*stx;
	struct lpp_rx		*rx;
	struct lpp_av		*av;
	struct lpp_mr		*mr;
	struct lpp_cq		*cq_transmit;
	struct lpp_cq		*cq_recv;
	struct lpp_cntr		*cntr_send;
	struct lpp_cntr		*cntr_recv;
	struct lpp_cntr		*cntr_read;
	struct lpp_cntr		*cntr_write;
	struct lpp_cntr		*cntr_remote_read;
	struct lpp_cntr		*cntr_remote_write;
	struct lpp_domain	*domain;

	klpp_id_t		id;
	int			is_enabled;
	int			stx_internal;
	uint64_t		transmit_bind_flags;
	uint64_t		recv_bind_flags;
	ofi_atomic32_t		ref_count;

	struct klpp_ep_io_stat	*io_stat;
	struct lpp_umc		*umc;
	uint8_t			umc_src_id;
	uint16_t		port;
	union lpp_fi_addr_storage addr;
};

struct fi_ep_attr lpp_ep_attrs(const struct klppioc_lf *klpp_devinfo);

static inline void lpp_ep_op_lock(struct lpp_ep *lpp_epp)
{
	lpp_lock(lpp_epp->domain, &lpp_epp->umc->lock);
}

static inline void lpp_ep_op_unlock(struct lpp_ep *lpp_epp)
{
	lpp_unlock(lpp_epp->domain, &lpp_epp->umc->lock);
}

int lpp_fi_endpoint(struct fid_domain *domain, struct fi_info *info,
		    struct fid_ep **ep, void *context);
int lpp_fi_ep_close(struct fid *ep);
int lpp_fi_ep_bind(struct fid *fid, struct fid *bfid, uint64_t flags);
int lpp_fi_ep_control(struct fid *ep, int command, void *arg);
ssize_t lpp_fi_cancel(fid_t ep, void *context);
int lpp_fi_setopt(struct fid *ep, int level, int optname, const void *optval,
		  size_t optlen);
void lpp_ep_progress(struct dlist_entry *ep_list);
void lpp_ep_setup_attrs(const struct klppioc_lf *klpp_devinfo,
			const struct fi_info *hints,
			struct fi_ep_attr *tmp_fabric);
int lpp_verify_ep_attrs(const struct klppioc_lf *klpp_devinfo,
			struct fi_ep_attr *ep_attrs);
#endif //  _LPP_EP_H_
