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

#ifndef _LPP_MR_H_
#define _LPP_MR_H_

struct lpp_mr {
	struct fid_mr		mr_fid;

	struct lpp_domain	*domain;
	void			*start;
	void			*end;
	struct fi_mr_attr	attr;

	klpp_id_t		id;
	ofi_atomic32_t		ref_count;
};

/* User visible functions. */
int lpp_mr_reg_internal(struct lpp_domain *lpp_domainp, const void *buf, size_t len,
			uint64_t access, uint64_t requested_key,
			enum fi_hmem_iface iface, void *context, struct lpp_mr **mr_ret);
int lpp_mr_close_internal(struct lpp_mr *lpp_mrp);
int lpp_mr_map_init(void **rbtree);
void lpp_mr_map_destroy(void *rbtree);
int lpp_mr_cache_entry_reg(struct ofi_mr_cache *cache, struct ofi_mr_entry *entry);
void lpp_mr_cache_entry_dereg(struct ofi_mr_cache *cache, struct ofi_mr_entry *entry);
struct lpp_mr *lpp_mr_cache_find(struct lpp_domain *lpp_domainp, void *addr,
			     uint64_t len, uint64_t key);
void lpp_mr_cache_drop(struct lpp_domain *lpp_domainp, struct lpp_mr *mrp);

#endif // _LPP_MR_H_
