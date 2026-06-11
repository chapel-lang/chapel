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

#ifndef _LPP_DOMAIN_H_
#define _LPP_DOMAIN_H_

#include "lpp.h"

struct lpp_domain {
	struct util_domain	util_domain;
	struct fi_info		info;
	struct lpp_fabric	*fabric;

	int			fd;
	int			index;
	struct klppioc_lf	devinfo;

	ofi_mutex_t		ep_lock;

	struct fi_domain_attr	attr;

	ofi_mutex_t		cntr_lock;
	struct lpp_cntr		*cntr_array;
	void			*cntr_block;
	int			cntr_used;
	int			cntr_total;

	struct klpp_rmr_meta	*rmr_meta_arr;
	void			*rmr_rw;
	void			*rmr_ro;
	void			*rmr_srq;

	ofi_mutex_lock_t	lock_acquire;
	ofi_mutex_unlock_t	lock_release;

	enum fi_hmem_iface	enabled_iface;

	struct ofi_mr_cache	*mr_cache;
};

/* Prototypes */
int lpp_fi_domain(struct fid_fabric *fabric, struct fi_info *info,
		  struct fid_domain **domain, void *context);
int lpp_fi_domain_close(struct fid *domain);
int lpp_domain_verify_attrs(const struct klppioc_lf *klpp_devinfo,
			    struct fi_domain_attr *attr, uint32_t version);
void lpp_domain_setup_attrs(const struct klppioc_lf *klpp_devinfo,
			    const struct fi_info *hints_domain,
			    struct fi_domain_attr *tmp_domain);
#endif // _LPP_DOMAIN_H_
