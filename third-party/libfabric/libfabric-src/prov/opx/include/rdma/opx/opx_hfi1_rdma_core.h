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
int opx_hfi1_wrapper_context_open(const int unit, const int port, const uint64_t open_timeout,
				  const enum opx_hfi1_type hfi1_type, void **ibv_context, unsigned int *user_version,
				  int *fd_cdev, int *fd_verbs);

/* Initialize the context with hfi1 direct or cdev */
struct _hfi_ctrl *opx_hfi1_wrapper_userinit(int fd, struct fi_opx_hfi1_context_internal *internal, int unit, int port);

/* Set PKEY */
int opx_hfi1_wrapper_set_pkey(struct fi_opx_hfi1_context_internal *internal, uint16_t pkey);

/* Ack events (link bounce support) */
int opx_hfi1_wrapper_ack_events(struct fi_opx_hfi1_context *context, uint64_t ackbits);

/* Reset context (link bounce support) */
int opx_hfi1_wrapper_reset_context(struct fi_opx_hfi1_context *context);

/* Close the rdma-core context */
void opx_hfi1_rdma_context_close(void *ibv_context);

/* Close the rdma-core lib */
void opx_hfi1_rdma_lib_close();

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

#if HAVE_HFI1_DIRECT_VERBS
#include <infiniband/hfi1dv.h>
#include <infiniband/verbs.h>

struct opx_rdma_ops_struct {
	/* static flags */

	bool hfi1_direct_verbs_enabled; /* run-time check based on dlopen() */
	bool one_time_setup;		/* one time setup is done */

	/* dlopen libraries */

	void *libhfi1verbs;
	void *libibverbs;

	/* verbs.h */

	struct ibv_device **(*fn_ibv_get_device_list)(int *num_devices);
	const char *(*fn_ibv_get_device_name)(struct ibv_device *device);
	struct ibv_context *(*fn_ibv_open_device)(struct ibv_device *device);
	void (*fn_ibv_free_device_list)(struct ibv_device **list);
	int (*fn_ibv_close_device)(struct ibv_context *);

	/* hfi1dv.h */

	int (*fn_hfi1_get_vers)(struct ibv_context *, struct hfi1_get_vers_rsp *);
	int (*fn_hfi1_assign_ctxt)(struct ibv_context *, struct hfi1_assign_ctxt_cmd *);
	int (*fn_hfi1_ctxt_info)(struct ibv_context *, struct hfi1_ctxt_info_rsp *);
	int (*fn_hfi1_user_info)(struct ibv_context *, struct hfi1_user_info_rsp *);
	int (*fn_hfi1_set_pkey)(struct ibv_context *, struct hfi1_set_pkey_cmd *);
	int (*fn_hfi1_tid_update)(struct ibv_context *context, struct hfi1_tid_update_cmd *cmd,
				  struct hfi1_tid_update_rsp *rsp);
	int (*fn_hfi1_tid_free)(struct ibv_context *context, struct hfi1_tid_free_cmd *cmd,
				struct hfi1_tid_free_rsp *rsp);
	int (*fn_hfi1_ack_event)(struct ibv_context *context, struct hfi1_ack_event_cmd *cmd);
	int (*fn_hfi1_ctxt_reset)(struct ibv_context *context);
};
#endif

#endif /* _FI_PROV_OPX_HFI1_RDMA_CORE_H_ */
