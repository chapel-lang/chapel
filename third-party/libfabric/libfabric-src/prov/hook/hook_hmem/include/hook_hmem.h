/*
 * Copyright (c) 2021 Intel Corporation. All rights reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL); Version 2, available from the file
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

#ifndef _HOOK_HMEM_H_
#define _HOOK_HMEM_H_

#include "ofi_hook.h"
#include "ofi.h"
#include "ofi_util.h"
#include "ofi_mr.h"

//TODO hook into getinfo call and pass in FI_HMEM, FI_MR_HMEM,
//and check iov_limit/adjust the hook iov limit
#define HOOK_HMEM_IOV_LIMIT	4

struct hook_hmem_domain {
	struct hook_domain	hook_domain;
	ofi_mutex_t		lock;
	int			mr_mode;
	struct ofi_bufpool	*mr_pool;
	struct ofi_bufpool	*ctx_pool;
	struct ofi_rbmap	rbmap;
	struct dlist_entry	mr_list;
};

struct hook_hmem_ep {
	struct hook_ep	hook_ep;
	uint64_t	tx_op_flags;
	uint64_t	rx_op_flags;
	uint64_t	tx_msg_flags;
	uint64_t	rx_msg_flags;
};

struct hook_hmem_desc {
	struct fid_mr		*mr_fid;
	void			*desc;
	struct iovec		iov;
	struct dlist_entry	entry;
	uint64_t		device;
	uint64_t		flags;
	uint64_t		count;
};

struct hook_hmem_ctx {
	struct fi_context2	hmem_ctx;
	void			*app_ctx;
	uint64_t		flags;
	struct hook_hmem_domain	*domain;
	uint64_t		comp_flags;
	uint8_t			desc_count;
	uint8_t			res_count;
	uint8_t			comp_count;
	struct hook_hmem_desc	*hmem_desc[HOOK_HMEM_IOV_LIMIT];
	//optimizing for typical use case (not ATOMIC)
	struct hook_hmem_desc	**res_desc;
	struct hook_hmem_desc	**comp_desc;
};

#endif /* _HOOK_HMEM_H_ */
