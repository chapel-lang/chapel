/*
 * Copyright (C) 2024-2024 Cornelis Networks.
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
#ifndef _FI_PROV_OPX_HMEM_CACHE_H_
#define _FI_PROV_OPX_HMEM_CACHE_H_

#include "config.h"
#include <ofi_util.h>
#include "rdma/opx/opx_hmem_domain.h"

#ifdef __cplusplus
extern "C" {
#endif

/* @brief Setup the MR cache.
 *
 * This function enables the MR cache using the util MR cache code.
 *
 * @param cache		The ofi_mr_cache that is to be set up.
 * @param domain	The OPX domain where cache will be used.
 * @return 0 on success, fi_errno on failure.
 */
int opx_hmem_cache_setup(struct ofi_mr_cache **cache, struct opx_hmem_domain *domain);

int  opx_hmem_cache_add_region(struct ofi_mr_cache *cache, struct ofi_mr_entry *entry);
void opx_hmem_cache_delete_region(struct ofi_mr_cache *cache, struct ofi_mr_entry *entry);

#ifdef __cplusplus
}
#endif

#endif /* _FI_PROV_OPX_HMEM_CACHE_H_ */
#endif /* OPX_HMEM */
