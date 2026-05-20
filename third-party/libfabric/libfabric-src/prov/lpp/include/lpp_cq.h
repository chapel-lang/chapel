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

#ifndef _LPP_CQ_H_
#define _LPP_CQ_H_

#include "lpp.h"

struct lpp_cq {
	struct fid_cq		cq_fid;
	struct lpp_domain	*domain;
	struct fi_cq_attr	attr;
	klpp_id_t		id;
	int			num_entries;
	size_t			cq_size;
	void			*kernel_shared_base;
	struct klpp_cq_header	*header;
	struct klpp_ringbuf_cq	*cq;
	struct klpp_ringbuf_cqerr *cqerr;
	ofi_atomic32_t		ref_count;
	struct fid_wait		*waitset;
	int			signal;
	ofi_atomic32_t		signaled;
	struct dlist_entry	ep_list;
	unsigned int		empty_cnt;
	size_t			entry_size;
	ofi_mutex_t		lock;
};

/* User visible functions. */
int lpp_fi_cq_open(struct fid_domain *domain, struct fi_cq_attr *attr, struct fid_cq **cq, void *context);
int lpp_fi_cq_close(struct fid *cq);
ssize_t lpp_fi_cq_read(struct fid_cq *cq, void *buf, size_t count);
ssize_t lpp_fi_cq_readerr(struct fid_cq *cq, struct fi_cq_err_entry *buf, uint64_t flags);
ssize_t lpp_fi_cq_readfrom(struct fid_cq *cq, void *buf, size_t count,
			   fi_addr_t *src_addr);
ssize_t lpp_fi_cq_sread(struct fid_cq *cq, void *buf, size_t count,
			const void *cond, int timeout);
ssize_t lpp_fi_cq_sreadfrom(struct fid_cq *cq, void *buf, size_t count,
			    fi_addr_t *src_addr, const void *cond, int timeout);

/* Internal functions. */
void lpp_cq_enqueue_entry(struct lpp_cq *, struct klpp_cq_tagged_entry *);
void lpp_cq_enqueue_err_entry(struct lpp_cq *, struct klpp_cq_err_entry *);
int lpp_cq_reserve_slot(struct lpp_cq *, uint64_t);
void lpp_cq_unreserve_slot(struct lpp_cq *);

#endif // _LPP_CQ_H_
