/*
 * Copyright (C) 2022-2024 Cornelis Networks.
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
#ifndef _FI_PROV_OPX_TID_DOMAIN_H_
#define _FI_PROV_OPX_TID_DOMAIN_H_

#include "rdma/fi_domain.h"

#include "ofi_util.h"
#include "ofi_mr.h"

#define OPX_TID_NO_LOCK_ON_CLEANUP (0)

#ifdef __cplusplus
extern "C" {
#endif

struct opx_tid_fabric {
	struct util_fabric util_fabric;
};

struct opx_tid_domain {
	struct util_domain     util_domain;
	struct opx_tid_fabric *fabric;
	struct ofi_mr_cache   *tid_cache;
	uint64_t	       key;
	struct dlist_entry     list_entry; /* linked to tid_domain_list */
};

int opx_close_tid_domain(struct opx_tid_domain *tid_domain, int locked);
int opx_open_tid_domain(struct opx_tid_fabric *tid_fabric, struct fi_info *info,
			struct opx_tid_domain **opx_tid_domain);

int opx_close_tid_fabric(struct opx_tid_fabric *opx_tid_fabric);
int opx_open_tid_fabric(struct opx_tid_fabric **opx_tid_fabric);

#ifdef __cplusplus
}
#endif

#endif /* _FI_PROV_OPX_TID_DOMAIN_H_ */
