/*
 * Copyright (C) 2024 Cornelis Networks.
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
#ifndef _FI_PROV_OPX_HFI1_RDMA_CORE_H_
#define _FI_PROV_OPX_HFI1_RDMA_CORE_H_

#include "rdma/opx/fi_opx.h"
#include "rdma/opx/fi_opx_compiler.h"

/* Wrapper functions to select between
 *
 * - old cdev ioctl() interface, or
 * - new hfi1 direct verbs (rdma-core) interface
 *
 * New implementation is completely in opx_hfi_rdma_core.c
 *
 */

/* Forward declare parameters for the wrappers */
struct fi_opx_hfi1_context_internal;
struct fi_opx_hfi1_context;
struct _hfi_ctrl;
struct hfi1_user_info_dep;

/* Open a context with hfi1 direct or cdev */
int opx_hfi1_wrapper_context_open(struct fi_opx_hfi1_context_internal *internal, int unit, int port,
				  uint64_t open_timeout, unsigned int *user_version);

/* Initialize the context with hfi1 direct or cdev */
struct _hfi_ctrl *opx_hfi1_wrapper_userinit(int fd, struct fi_opx_hfi1_context_internal *internal, int unit, int port);

/* Set PKEY */
int opx_hfi1_wrapper_set_pkey(struct fi_opx_hfi1_context_internal *internal, uint16_t pkey);

/* Ack events (link bounce support) */
int opx_hfi1_wrapper_ack_events(struct fi_opx_hfi1_context *context, uint64_t ackbits);

/* Reset context (link bounce support) */
int opx_hfi1_wrapper_reset_context(struct fi_opx_hfi1_context *context);

/* Close the rdma-core lib/context */
void opx_hfi1_rdma_context_close(void *ibv_context);

/* TID function pointers and wrapper */
struct fi_opx_hfi1_context;

extern int32_t (*opx_fn_hfi1_free_tid)(struct fi_opx_hfi1_context *context, uint64_t tidlist, uint32_t tidcnt);

extern int32_t (*opx_fn_hfi1_update_tid)(struct fi_opx_hfi1_context *context, uint64_t vaddr, uint32_t *length,
					 uint64_t tidlist, uint32_t *tidcnt, uint16_t flags);

/* De-register/Free tid */
__OPX_FORCE_INLINE__
int32_t opx_hfi1_wrapper_free_tid(struct fi_opx_hfi1_context *context, uint64_t tidlist, uint32_t tidcnt)
{
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "[HFI1-DIRECT] context %p, tidlist %#lX, tidcnt %u\n", context,
		     tidlist, tidcnt);
	return (*opx_fn_hfi1_free_tid)(context, tidlist, tidcnt);
}

/* Register/update tid */
__OPX_FORCE_INLINE__
int32_t opx_hfi1_wrapper_update_tid(struct fi_opx_hfi1_context *context, uint64_t vaddr, uint32_t *length,
				    uint64_t tidlist, uint32_t *tidcnt, uint16_t flags)
{
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		     "[HFI1-DIRECT] context %p, vaddr %#lX, length %u, tidlist %#lX, tidcnt %u, flags %#X\n", context,
		     vaddr, *length, tidlist, *tidcnt, flags);
	return (*opx_fn_hfi1_update_tid)(context, vaddr, length, tidlist, tidcnt, flags);
}
#endif /* _FI_PROV_OPX_HFI1_RDMA_CORE_H_ */
