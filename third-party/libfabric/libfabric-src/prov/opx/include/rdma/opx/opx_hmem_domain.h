/*
 * Copyright (C) 2024-2025 Cornelis Networks.
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
#ifdef OPX_HMEM
#ifndef _FI_PROV_OPX_HMEM_DOMAIN_H_
#define _FI_PROV_OPX_HMEM_DOMAIN_H_

#include "rdma/fi_domain.h"
#include "rdma/opx/fi_opx_hfi1_packet.h"

#include "ofi_util.h"
#include "ofi_mr.h"

#define OPX_HMEM_NO_LOCK_ON_CLEANUP (0)

#define OPX_HMEM_DEV_REG_SEND_THRESHOLD_DEFAULT (4096)
#define OPX_HMEM_DEV_REG_RECV_THRESHOLD_DEFAULT (OPX_HFI1_PKT_SIZE)
#define OPX_HMEM_DEV_REG_THRESHOLD_MAX		(OPX_HFI1_PKT_SIZE)
#define OPX_HMEM_DEV_REG_THRESHOLD_MIN		(0)

#ifdef __cplusplus
extern "C" {
#endif

struct opx_hmem_fabric {
	struct util_fabric util_fabric;
};

union opx_hmem_stream;

struct opx_hmem_domain {
	struct util_domain    util_domain;
	struct ofi_mr_cache  *hmem_cache;
	struct fi_opx_domain *opx_domain;
	struct dlist_entry    list_entry; /* linked to hmem_domain_list */
	struct ofi_mr_cache  *ipc_cache;
	struct {
		union opx_hmem_stream *stream;
		struct ofi_bufpool    *event_pool;
		enum fi_hmem_iface     type;
	} hmem_stream;
	uint32_t devreg_copy_from_threshold;
	uint32_t devreg_copy_to_threshold;
};

int opx_hmem_close_domain(struct opx_hmem_domain *hmem_domain, int locked);
int opx_hmem_open_domain(struct opx_hmem_fabric *hmem_fabric, struct fi_info *info,
			 struct opx_hmem_domain **opx_hmem_domain);

int opx_hmem_close_fabric(struct opx_hmem_fabric *opx_hmem_fabric);
int opx_hmem_open_fabric(struct opx_hmem_fabric **opx_hmem_fabric);

#ifdef __cplusplus
}
#endif

#endif /* _FI_PROV_OPX_HMEM_DOMAIN_H_ */
#endif /* OPX_HMEM */
