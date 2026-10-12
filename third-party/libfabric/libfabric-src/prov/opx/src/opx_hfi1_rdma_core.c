/*
 * Copyright (C) 2024-2025 by Cornelis Networks.
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

#include "rdma/opx/opx_hfi1_rdma_core.h"

/* kernel headers (new and old interface structures) */
#include <rdma/hfi/hfi1_ioctl.h>
#include <rdma/hfi/hfi1_user.h>

#define OPX_HFI1_VERBS_CONTEXTS_ONLY (false)

/* Check for hfi1 verbs direct (rdma-core) interface support before including
 * new headers.
 * Configure assumed that verbs.h comes with hfi1dv.h
 */
#if HAVE_HFI1_DIRECT_VERBS == 1 /* config.h */

/* Validate the new header versioning from hfi1_user.h.
 * This should be in sync with the new hfidv.h but check it anyway,
 */
#if defined(HFI1_RDMA_USER_SWMAJOR) && defined(HFI1_RDMA_USER_SWMINOR)

#else
#warning Old hfi1_user header found along with hfi1dv header
#warning Continuing ... if it fails configure with OPX_RDMA_CORE=0
#endif

/* OPX does not require/link rdma-core or hfi1 direct verbs, dlopen only */
#include <dlfcn.h>

/* Build RDMA CORE and HFI1 Direct Verbs support */
#include <infiniband/hfi1dv.h>
#include <infiniband/verbs.h>

#endif /* HAVE_HFI1_DIRECT_VERBS */

#include <stdio.h>

/* Legacy HFI/CDEV headers */
#include "opa_service.h"
#include "opa_user_gen1.h"

#include "rdma/opx/fi_opx_hfi1.h"
#include "rdma/opx/opx_hfi1_cn5000.h"

/* TID function pointers */
int32_t (*opx_fn_hfi1_free_tid)(struct fi_opx_hfi1_context *context, uint64_t tidlist, uint32_t tidcnt);
int32_t (*opx_fn_hfi1_update_tid)(struct fi_opx_hfi1_context *context, uint64_t vaddr, uint32_t *length,
				  uint64_t tidlist, uint32_t *tidcnt, uint16_t flags);

/* TID function prototypes */
int32_t opx_hfi1_rdma_free_tid(struct fi_opx_hfi1_context *context, uint64_t tidlist, uint32_t tidcnt);
int32_t opx_hfi_free_tid(struct fi_opx_hfi1_context *context, uint64_t tidlist, uint32_t tidcnt);
int32_t opx_hfi1_rdma_update_tid(struct fi_opx_hfi1_context *context, uint64_t vaddr, uint32_t *length,
				 uint64_t tidlist, uint32_t *tidcnt, uint16_t flags);
int32_t opx_hfi_update_tid(struct fi_opx_hfi1_context *context, uint64_t vaddr, uint32_t *length, uint64_t tidlist,
			   uint32_t *tidcnt, uint16_t flags);

#if HAVE_HFI1_DIRECT_VERBS == 1 /* Enabled in the build */
/* Define the new interfaces and structures */

struct opx_rdma_ops_struct opx_rdma_ops = {
	/* static initial values */
	.hfi1_direct_verbs_enabled = true,
	.one_time_setup		   = false,
	.libhfi1verbs		   = NULL,
	.libibverbs		   = NULL};

#define OPX_HFI1_RDMA_FN(fn) (*(opx_rdma_ops.fn_##fn))

#define OPX_HFI1_RDMA_OP_DLSYM(handle, fn)                                                           \
	opx_rdma_ops.fn_##fn = dlsym(handle, #fn);                                                   \
	if (!opx_rdma_ops.fn_##fn) {                                                                 \
		FI_WARN(fi_opx_global.prov, FI_LOG_DOMAIN, "[HFI1-DIRECT] dlsym(" #fn ") failed\n"); \
		opx_rdma_ops.hfi1_direct_verbs_enabled = false;                                      \
	} else {                                                                                     \
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_DOMAIN, "[HFI1-DIRECT] dlsym(" #fn ") \n");  \
	}

__OPX_FORCE_INLINE__
bool opx_hfi1_direct_verbs_enabled(void)
{
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "[HFI1-DIRECT] HAVE_HFI1_DIRECT_VERBS %u \n",
		     opx_rdma_ops.hfi1_direct_verbs_enabled);
	return opx_rdma_ops.hfi1_direct_verbs_enabled;
}

/* Couldn't initialize RDMA core or some unknown error, fallback to cdev APIs */
__OPX_FORCE_INLINE__
int opx_open_hfi1_rdma_fallback(void)
{
	FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA, "[HFI1-DIRECT] HAVE_HFI1_DIRECT_VERBS %u \n",
		opx_rdma_ops.hfi1_direct_verbs_enabled);

	/* reset tid funtions */
	opx_fn_hfi1_free_tid   = opx_hfi_free_tid;
	opx_fn_hfi1_update_tid = opx_hfi_update_tid;

	opx_rdma_ops.hfi1_direct_verbs_enabled = false;
	return -1;
}

__OPX_FORCE_INLINE__
bool opx_hfi1_rdma_op_initialize(const bool use_new_tid_ops)
{
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "[HFI1-DIRECT] HAVE_HFI1_DIRECT_VERBS %u \n",
		     opx_rdma_ops.hfi1_direct_verbs_enabled);
	if (opx_rdma_ops.one_time_setup) {
		return opx_rdma_ops.hfi1_direct_verbs_enabled;
	}
	opx_rdma_ops.one_time_setup = true;

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "[HFI1-DIRECT] one time initialization\n");

	/* Set old tid functions until/unless we pass and enable the new support */
	opx_fn_hfi1_free_tid   = opx_hfi_free_tid;
	opx_fn_hfi1_update_tid = opx_hfi_update_tid;

	opx_rdma_ops.libhfi1verbs = dlopen("libhfi1verbs.so", RTLD_LAZY);

	if (!opx_rdma_ops.libhfi1verbs) {
		FI_WARN(fi_opx_global.prov, FI_LOG_DOMAIN, "[HFI1-DIRECT] Could not dlopen libhfi1verbs.\n");
		opx_rdma_ops.hfi1_direct_verbs_enabled = false;
		return opx_rdma_ops.hfi1_direct_verbs_enabled;
	}

	opx_rdma_ops.libibverbs = dlopen("libibverbs.so", RTLD_LAZY);
	if (!opx_rdma_ops.libibverbs) {
		FI_WARN(fi_opx_global.prov, FI_LOG_DOMAIN, "[HFI1-DIRECT] Could not dlopen libibverbs.\n");
		opx_rdma_ops.hfi1_direct_verbs_enabled = false;
		dlclose(opx_rdma_ops.libhfi1verbs);
		opx_rdma_ops.libhfi1verbs = NULL;
		return opx_rdma_ops.hfi1_direct_verbs_enabled;
	}
	FI_WARN(fi_opx_global.prov, FI_LOG_DOMAIN, "[HFI-DIRECT] libibverbs found\n");

	OPX_HFI1_RDMA_OP_DLSYM(opx_rdma_ops.libibverbs, ibv_get_device_list);
	OPX_HFI1_RDMA_OP_DLSYM(opx_rdma_ops.libibverbs, ibv_get_device_name);
	OPX_HFI1_RDMA_OP_DLSYM(opx_rdma_ops.libibverbs, ibv_open_device);
	OPX_HFI1_RDMA_OP_DLSYM(opx_rdma_ops.libibverbs, ibv_free_device_list);
	OPX_HFI1_RDMA_OP_DLSYM(opx_rdma_ops.libibverbs, ibv_close_device);

	OPX_HFI1_RDMA_OP_DLSYM(opx_rdma_ops.libhfi1verbs, hfi1_get_vers);
	OPX_HFI1_RDMA_OP_DLSYM(opx_rdma_ops.libhfi1verbs, hfi1_assign_ctxt);
	OPX_HFI1_RDMA_OP_DLSYM(opx_rdma_ops.libhfi1verbs, hfi1_ctxt_info);
	OPX_HFI1_RDMA_OP_DLSYM(opx_rdma_ops.libhfi1verbs, hfi1_user_info);
	OPX_HFI1_RDMA_OP_DLSYM(opx_rdma_ops.libhfi1verbs, hfi1_set_pkey);
	OPX_HFI1_RDMA_OP_DLSYM(opx_rdma_ops.libhfi1verbs, hfi1_tid_update);
	OPX_HFI1_RDMA_OP_DLSYM(opx_rdma_ops.libhfi1verbs, hfi1_tid_free);
	OPX_HFI1_RDMA_OP_DLSYM(opx_rdma_ops.libhfi1verbs, hfi1_ack_event);
	OPX_HFI1_RDMA_OP_DLSYM(opx_rdma_ops.libhfi1verbs, hfi1_ctxt_reset);

	if (!opx_rdma_ops.hfi1_direct_verbs_enabled) {
		FI_WARN(fi_opx_global.prov, FI_LOG_DOMAIN, "[HFI1-DIRECT] Could not setup HFI1 Direct Verbs.\n");
		dlclose(opx_rdma_ops.libhfi1verbs);
		opx_rdma_ops.libhfi1verbs = NULL;
		dlclose(opx_rdma_ops.libibverbs);
		opx_rdma_ops.libibverbs = NULL;
	}

	/* Set new tid functions */
	if (use_new_tid_ops) {
		opx_fn_hfi1_update_tid = opx_hfi1_rdma_update_tid;
		opx_fn_hfi1_free_tid   = opx_hfi1_rdma_free_tid;
	}

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "[HFI1-DIRECT] HAVE_HFI1_DIRECT_VERBS %u \n",
		     opx_rdma_ops.hfi1_direct_verbs_enabled);

	return opx_rdma_ops.hfi1_direct_verbs_enabled;
}

static int opx_hfi1_rdma_context_open(int unit, int port, uint64_t open_timeout, const bool set_user_version,
				      unsigned int *user_version, void **p_ibv_context)
{
	struct ibv_device **dev_list	= NULL;
	struct ibv_context *ibv_context = NULL;
	const char	   *dev_name	= NULL;
	struct ibv_device  *ibv_device	= NULL;
	int		    num_devices;
	int		    ret;

	assert(opx_rdma_ops.hfi1_direct_verbs_enabled);

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "[HFI1-DIRECT] HAVE_HFI1_DIRECT_VERBS %u \n",
		     opx_rdma_ops.hfi1_direct_verbs_enabled);

	(void) open_timeout; /* unused */

	dev_list = OPX_HFI1_RDMA_FN(ibv_get_device_list)(&num_devices);
	if (!dev_list || !*dev_list) {
		FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA,
			"[HFI1-DIRECT] No device found. dev_list %p/%p, err %d %s\n", dev_list,
			dev_list ? *dev_list : NULL, errno, strerror(errno));
		return opx_open_hfi1_rdma_fallback();
	}
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "[HFI1-DIRECT] %d devices found\n", num_devices);

	if (num_devices == 0) {
		FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA, "[HFI1-DIRECT] %d devices found\n", num_devices);
		return opx_open_hfi1_rdma_fallback();
	}

	if (unit >= OPX_MAX_HFIS) {
		FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA, "[HFI1-DIRECT] Invalid HFI_UNIT=%d\n", unit);
		return opx_open_hfi1_rdma_fallback();
	}

	char requested_device[IBV_SYSFS_NAME_MAX];
	sprintf(requested_device, "hfi1_%d", unit);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "[HFI1-DIRECT] Requested device %s\n", requested_device);

	// look for a match
	for (int i = 0; i < num_devices; i++) {
		struct ibv_device *temp_dev  = dev_list[i];
		const char	  *temp_name = OPX_HFI1_RDMA_FN(ibv_get_device_name)(temp_dev);
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "[HFI1-DIRECT] device[%d] %s\n", i, temp_name);
		if (temp_name && dev_name == NULL &&
		    strncmp(temp_name, requested_device, strlen(requested_device)) == 0) {
			dev_name   = temp_name;
			ibv_device = temp_dev;
		}
	}
	if (!dev_name) {
		FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA, "[HFI1-DIRECT] No matching device\n");
		return opx_open_hfi1_rdma_fallback();
	}
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "[HFI1-DIRECT] Using device %s\n", dev_name);

	ibv_context = OPX_HFI1_RDMA_FN(ibv_open_device)(ibv_device);
	if (!ibv_context) {
		FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA,
			"[HFI1-DIRECT] ** failed to open device context for unit %d\n", unit);
		return opx_open_hfi1_rdma_fallback();
	}
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_DOMAIN, "[HFI1-DIRECT] Open context %p\n", ibv_context);

	// done with device list
	OPX_HFI1_RDMA_FN(ibv_free_device_list)(dev_list);

	struct hfi1_get_vers_rsp get_vers_rsp = {};

	// call the test ioctl
	ret = OPX_HFI1_RDMA_FN(hfi1_get_vers)(ibv_context, &get_vers_rsp);
	if (ret) {
		FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA,
			"[HFI1-DIRECT] hfi_unit %d hfi1_get_vers fail, ret %d, err %d %s\n", unit, ret, errno,
			strerror(errno));
		return opx_open_hfi1_rdma_fallback();
	}
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		     "[HFI1-DIRECT] hfi_unit %d hfi1_get_vers success, version 0x%x\n", unit, get_vers_rsp.version);

	if (set_user_version) {
		*user_version = get_vers_rsp.version;
	}

	*p_ibv_context = (void *) ibv_context;

	return ibv_context->cmd_fd;
}

static struct _hfi_ctrl *opx_hfi1_rdma_userinit(int fd, struct fi_opx_hfi1_context_internal *internal, int unit,
						int port)
{
	assert(opx_rdma_ops.hfi1_direct_verbs_enabled);

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "[HFI1-DIRECT] fd %d\n", fd);
	int ret;
	/* We still use cdev structs internally */
	struct hfi1_ctxt_info	  *cinfo;			 /* (rdma-core) struct hfi1_ctxt_info_rsp */
	struct hfi1_base_info	  *binfo;			 /* (rdma-core) struct hfi1_user_info_rsp */
	struct hfi1_user_info_dep *uinfo = &internal->user_info; /* (rdma-core) struct hfi1_assign_ctxt_cmd */

	if (!(internal->ctrl = calloc(1, sizeof(struct _hfi_ctrl)))) {
		FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA,
			"[HFI1-DIRECT] Warning: can't allocate memory for hfi_ctrl: %s\n", strerror(errno));
		goto err_calloc_hfi_ctrl;
	}

	internal->ctrl->__hfi_unit = -2U;
	struct _hfi_ctrl *pctrl	   = internal->ctrl; /* local ptr */
	cinfo			   = &pctrl->ctxt_info;
	binfo			   = &pctrl->base_info;

	struct fi_opx_hfi1_context *context	= &internal->context;
	struct ibv_context	   *ibv_context = (struct ibv_context *) context->ibv_context;

	assert(fd == ibv_context->cmd_fd);

	/* First get the page size */
	int __hfi_pg_sz = sysconf(_SC_PAGESIZE);
	FI_DBG_TRACE(
		fi_opx_global.prov, FI_LOG_EP_DATA,
		"[HFI1-DIRECT] CONTEXT INIT __hfi_pg_sz %d uinfo: ver %#x, pad %d, alg %d, subc_cnt %d, subc_id %d\n",
		__hfi_pg_sz, uinfo->userversion, uinfo->pad, uinfo->hfi1_alg, uinfo->subctxt_cnt, uinfo->subctxt_id);

	struct hfi1_assign_ctxt_cmd assign_ctxt_cmd = {}; /* (cdev) struct hfi1_user_info_dep */

	assign_ctxt_cmd.userversion = uinfo->userversion;
	assign_ctxt_cmd.port	    = port;

	/* do not share contexts */
	assign_ctxt_cmd.subctxt_id  = uinfo->subctxt_id;
	assign_ctxt_cmd.subctxt_cnt = uinfo->subctxt_cnt;

	memcpy(assign_ctxt_cmd.uuid, uinfo->uuid, sizeof(assign_ctxt_cmd.uuid));
	FI_DBG_TRACE(
		fi_opx_global.prov, FI_LOG_EP_DATA,
		"[HFI1-DIRECT] CONTEXT INIT assign_cmd_cmd: userversion %#X, port %u, kdeth_rcvhdrsz %u, reserved1 %#X, subctxt_cnt %u, subctxt_id %u, uuid[16] %#lX %#lX reserved2 %#X\n",
		assign_ctxt_cmd.userversion, assign_ctxt_cmd.port, assign_ctxt_cmd.kdeth_rcvhdrsz,
		assign_ctxt_cmd.reserved1, assign_ctxt_cmd.subctxt_cnt, assign_ctxt_cmd.subctxt_id,
		*(uint64_t *) &assign_ctxt_cmd.uuid[0], *(uint64_t *) &assign_ctxt_cmd.uuid[4],
		assign_ctxt_cmd.reserved2);

	ret = OPX_HFI1_RDMA_FN(hfi1_assign_ctxt)(ibv_context, &assign_ctxt_cmd);
	if (ret) {
		FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA,
			"[HFI1-DIRECT] hfi_unit %d hfi1_assign_ctxt fail, ret %d, err %d %s\n", context->hfi_unit, ret,
			errno, strerror(errno));
		goto err_hfi_cmd_assign_ctxt;
	}

	struct hfi1_ctxt_info_rsp ctxt_info_rsp = {}; /* (cdev) struct hfi1_ctxt_info */

	ret = OPX_HFI1_RDMA_FN(hfi1_ctxt_info)(ibv_context, &ctxt_info_rsp);
	if (ret) {
		FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA,
			"[HFI1-DIRECT] hfi_unit %d hfi1_ctxt_info fail, ret %d, err %d %s\n", context->hfi_unit, ret,
			errno, strerror(errno));
		goto err_hfi_cmd_ctxt_info;
	}

	struct hfi1_user_info_rsp user_info_rsp = {}; /* (cdev) struct hfi1_base_info */

	ret = OPX_HFI1_RDMA_FN(hfi1_user_info)(ibv_context, &user_info_rsp);
	if (ret) {
		FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA,
			"[HFI1-DIRECT] hfi_unit %d hfi1_user_info fail, ret %d, err %d %s\n", context->hfi_unit, ret,
			errno, strerror(errno));
		goto err_hfi_cmd_ctxt_info;
	}

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "[HFI1-DIRECT] hfi_unit %d hw version  0x%x sw version 0x%x\n",
		     context->hfi_unit, user_info_rsp.hw_version, user_info_rsp.sw_version);

	context->hfi1_type = opx_hfi1_check_hwversion(user_info_rsp.hw_version);
	assert(context->hfi1_type &
	       (OPX_HFI1_CYR | OPX_HFI1_JKR | OPX_HFI1_WFR)); /* OPX_HFI1_MIXED_9B is determined later */

	/* Need the global set early, may be changed later on mixed networks */
	if (OPX_SW_HFI1_TYPE == OPX_HFI1_UNDEF) {
		OPX_SW_HFI1_TYPE = context->hfi1_type;
		OPX_HW_HFI1_TYPE = context->hfi1_type;
	}

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		     "[HFI1-DIRECT] global type %d, opx_hfi1_check_hwversion base_info->hw_version %#X, %s\n",
		     OPX_SW_HFI1_TYPE, user_info_rsp.hw_version, OPX_HFI1_TYPE_STRING(context->hfi1_type));

	/* Copy the the new 'hfi1_user_info_rsp' to the  old 'hfi1_base_info'
	 * struct */
	binfo->hw_version	 = user_info_rsp.hw_version;
	binfo->sw_version	 = user_info_rsp.sw_version;
	binfo->jkey		 = user_info_rsp.jkey;
	binfo->bthqp		 = user_info_rsp.bthqp;
	binfo->sc_credits_addr	 = user_info_rsp.sc_credits_addr;
	binfo->pio_bufbase_sop	 = user_info_rsp.pio_bufbase_sop;
	binfo->pio_bufbase	 = user_info_rsp.pio_bufbase;
	binfo->rcvhdr_bufbase	 = user_info_rsp.rcvhdr_bufbase;
	binfo->rcvegr_bufbase	 = user_info_rsp.rcvegr_bufbase;
	binfo->sdma_comp_bufbase = user_info_rsp.sdma_comp_bufbase;
	binfo->user_regbase	 = user_info_rsp.user_regbase;
	binfo->events_bufbase	 = user_info_rsp.events_bufbase;
	binfo->status_bufbase	 = user_info_rsp.status_bufbase;
	binfo->rcvhdrtail_base	 = user_info_rsp.rcvhdrtail_base;
	binfo->subctxt_uregbase	 = user_info_rsp.subctxt_uregbase;
	binfo->subctxt_rcvegrbuf = user_info_rsp.subctxt_rcvegrbuf;
	binfo->subctxt_rcvhdrbuf = user_info_rsp.subctxt_rcvhdrbuf;

	context->info.rxe.hdrq.rhe_base = (uint64_t *) user_info_rsp.rheq_bufbase; /* New field */

	/* Copy the new 'hfi1_ctxt_info_rsp' struct to the old 'hfi1_ctxt_info'
	 * struct */
	cinfo->runtime_flags   = ctxt_info_rsp.runtime_flags;
	cinfo->rcvegr_size     = ctxt_info_rsp.rcvegr_size;
	cinfo->num_active      = ctxt_info_rsp.num_active;
	cinfo->unit	       = ctxt_info_rsp.unit;
	cinfo->ctxt	       = ctxt_info_rsp.ctxt;
	cinfo->subctxt	       = ctxt_info_rsp.subctxt;
	cinfo->rcvtids	       = ctxt_info_rsp.rcvtids;
	cinfo->credits	       = ctxt_info_rsp.credits;
	cinfo->numa_node       = ctxt_info_rsp.numa_node;
	cinfo->rec_cpu	       = ctxt_info_rsp.rec_cpu;
	cinfo->send_ctxt       = ctxt_info_rsp.send_ctxt;
	cinfo->egrtids	       = ctxt_info_rsp.egrtids;
	cinfo->rcvhdrq_cnt     = ctxt_info_rsp.rcvhdrq_cnt;
	cinfo->rcvhdrq_entsize = ctxt_info_rsp.rcvhdrq_entsize;
	cinfo->sdma_ring_size  = ctxt_info_rsp.sdma_ring_size;

	FI_DBG_TRACE(
		fi_opx_global.prov, FI_LOG_EP_DATA,
		"[HFI1-DIRECT] CONTEXT INIT base_info: hw_version %#X, sw_version %#X, jkey %#X, bthqp %u, sc_credits_addr %#llX, pio_bufbase_sop %#llX, pio_bufbase %#llX, rcvhdr_bufbase %#llX, rcvegr_bufbase %#llX, sdma_comp_bufbase %#llX, user_regbase %#llX, events_bufbase %#llX, status_bufbase %#llX, rcvhdrtail_base %#llX, subctxt_uregbase%#llX, subctxt_rcvegrbuf %#llX, subctxt_rcvhdrbuf %#llX, rheq_bufbase %#llX\n",
		binfo->hw_version, binfo->sw_version, binfo->jkey, binfo->bthqp, binfo->sc_credits_addr,
		binfo->pio_bufbase_sop, binfo->pio_bufbase, binfo->rcvhdr_bufbase, binfo->rcvegr_bufbase,
		binfo->sdma_comp_bufbase, binfo->user_regbase, binfo->events_bufbase, binfo->status_bufbase,
		binfo->rcvhdrtail_base, binfo->subctxt_uregbase, binfo->subctxt_rcvegrbuf, binfo->subctxt_rcvhdrbuf,
		(__u64) context->info.rxe.hdrq.rhe_base);

	FI_DBG_TRACE(
		fi_opx_global.prov, FI_LOG_EP_DATA,
		"[HFI1-DIRECT] CONTEXT INIT ctxt_info: runtime_flags %#llX, rcvegr_size%d, num_active %d, unit %d, ctxt %d, subctxt %d, rcvtids %d, credits %d, numa_node %d, rec_cpu %d, send_ctxt %d, egrtids %d, rcvhdrq_cnt %d, rcvhdrq_entsize %d, sdma_ring_size %d\n",
		cinfo->runtime_flags, cinfo->rcvegr_size, cinfo->num_active, cinfo->unit, cinfo->ctxt, cinfo->subctxt,
		cinfo->rcvtids, cinfo->credits, cinfo->numa_node, cinfo->rec_cpu, cinfo->send_ctxt, cinfo->egrtids,
		cinfo->rcvhdrq_cnt, cinfo->rcvhdrq_entsize, cinfo->sdma_ring_size);

	/* New addition to JKR */
	__aligned_u64 rheq_bufbase = (__u64) context->info.rxe.hdrq.rhe_base;

	ret = opx_map_hfi_mem(fd, pctrl, 0 /* uinfo->subctxt_cnt */, &rheq_bufbase, context->hfi1_type);
	if (ret == -1) {
		FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA, "[HFI1-DIRECT] Failed to map HFI memory. errno %s\n",
			strerror(errno));
		goto err_map_hfi_mem;
	}
	/* Save the new mmap (from the token) in the context */
	context->info.rxe.hdrq.rhe_base = (uint64_t *) rheq_bufbase;

	FI_DBG_TRACE(
		fi_opx_global.prov, FI_LOG_EP_DATA,
		"[HFI1-DIRECT] CONTEXT INIT base_info: hw_version %#X, sw_version %#X, jkey %#X, bthqp %u, sc_credits_addr %#llX, pio_bufbase_sop %#llX, pio_bufbase %#llX, rcvhdr_bufbase %#llX, rcvegr_bufbase %#llX, sdma_comp_bufbase %#llX, user_regbase %#llX, events_bufbase %#llX, status_bufbase %#llX, rcvhdrtail_base %#llX, subctxt_uregbase%#llX, subctxt_rcvegrbuf %#llX, subctxt_rcvhdrbuf %#llX, rheq_bufbase %#llX\n",
		binfo->hw_version, binfo->sw_version, binfo->jkey, binfo->bthqp, binfo->sc_credits_addr,
		binfo->pio_bufbase_sop, binfo->pio_bufbase, binfo->rcvhdr_bufbase, binfo->rcvegr_bufbase,
		binfo->sdma_comp_bufbase, binfo->user_regbase, binfo->events_bufbase, binfo->status_bufbase,
		binfo->rcvhdrtail_base, binfo->subctxt_uregbase, binfo->subctxt_rcvegrbuf, binfo->subctxt_rcvhdrbuf,
		(__u64) context->info.rxe.hdrq.rhe_base);

	FI_DBG_TRACE(
		fi_opx_global.prov, FI_LOG_EP_DATA,
		"[HFI1-DIRECT] CONTEXT INIT ctxt_info: runtime_flags %#llX, rcvegr_size%d, num_active %d, unit %d, ctxt %d, subctxt %d, rcvtids %d, credits %d, numa_node %d, rec_cpu %d, send_ctxt %d, egrtids %d, rcvhdrq_cnt %d, rcvhdrq_entsize %d, sdma_ring_size %d\n",
		cinfo->runtime_flags, cinfo->rcvegr_size, cinfo->num_active, cinfo->unit, cinfo->ctxt, cinfo->subctxt,
		cinfo->rcvtids, cinfo->credits, cinfo->numa_node, cinfo->rec_cpu, cinfo->send_ctxt, cinfo->egrtids,
		cinfo->rcvhdrq_cnt, cinfo->rcvhdrq_entsize, cinfo->sdma_ring_size);

	/* sanity checking... */
	if (cinfo->rcvtids % 8) {
		FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA, "[HFI1-DIRECT] rcvtids not 8 multiple: %d\n",
			cinfo->rcvtids);
		goto err_sanity_check;
	}
	if (cinfo->egrtids % 8) {
		FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA, "[HFI1-DIRECT] egrtids not 8 multiple: %d\n",
			cinfo->egrtids);
		goto err_sanity_check;
	}
	if (cinfo->rcvtids < cinfo->egrtids) {
		FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA, "[HFI1-DIRECT] rcvtids(%d) < egrtids(%d)\n", cinfo->rcvtids,
			cinfo->egrtids);
		goto err_sanity_check;
	}
	if (cinfo->rcvhdrq_cnt % 32) {
		FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA, "[HFI1-DIRECT] rcvhdrq_cnt not 32 multiple: %d\n",
			cinfo->rcvhdrq_cnt);
		goto err_sanity_check;
	}
	if (cinfo->rcvhdrq_entsize % 64) {
		FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA, "[HFI1-DIRECT] rcvhdrq_entsize not 64 multiple: %d\n",
			cinfo->rcvhdrq_entsize);
		goto err_sanity_check;
	}
	if (cinfo->rcvegr_size % __hfi_pg_sz) {
		FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA, "[HFI1-DIRECT] rcvegr_size not page multiple: %d\n",
			cinfo->rcvegr_size);
		goto err_sanity_check;
	}

	/*
	 * More version checks...
	 */
	if ((binfo->sw_version >> HFI1_SWMAJOR_SHIFT) != opx_hfi_get_user_major_version()) {
		FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA,
			"[HFI1-DIRECT] User major version 0x%x not same as driver major 0x%x\n",
			opx_hfi_get_user_major_version(), binfo->sw_version >> HFI1_SWMAJOR_SHIFT);
		if ((binfo->sw_version >> HFI1_SWMAJOR_SHIFT) < opx_hfi_get_user_major_version()) {
			goto err_version_mismatch; /* else assume driver knows
						      how to be compatible */
		}
	} else if ((binfo->sw_version & 0xffff) != HFI1_USER_SWMINOR) {
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
			     "[HFI1-DIRECT] User minor version 0x%x not same as driver minor 0x%x, opx minor %#x\n",
			     HFI1_USER_SWMINOR, binfo->sw_version & 0xffff, opx_hfi_get_user_minor_version());
	}

	/* Save some info. */
	pctrl->fd	   = fd; /* Sure save the fd all over these structs? */
	pctrl->__hfi_pg_sz = __hfi_pg_sz;
	pctrl->__hfi_unit  = cinfo->unit;

	pctrl->__hfi_port      = port;
	pctrl->__hfi_tidegrcnt = cinfo->egrtids;
	pctrl->__hfi_tidexpcnt = cinfo->rcvtids - cinfo->egrtids;

	FI_DBG_TRACE(
		fi_opx_global.prov, FI_LOG_EP_DATA,
		"[HFI1-DIRECT] CONTEXT INIT spctrl __hfi_unit %u, __hfi_port %u, __hfi_tidegrcnt %u, __hfi_tidexpcnt %u\n",
		pctrl->__hfi_unit, pctrl->__hfi_port, pctrl->__hfi_tidegrcnt, pctrl->__hfi_tidexpcnt);

	return pctrl;

err_map_hfi_mem:
err_version_mismatch:
err_sanity_check:
err_hfi_cmd_ctxt_info:
	/* de-assign context here? */
	// without de-assigning the context, all subsequent
	// opx_hfi_userinit_internal() calls are going to fail
	//
	// No known api to de-assign and since this is fatal, doesn't matter.
	FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA,
		"[HFI1-DIRECT] An unrecoverable error occurred while communicating with the driver\n");
	abort();
	// no recovery here

	/* if we failed to allocate memory or to assign the context, we might
	 * still recover from this. Returning NULL will cause the function to be
	 * reinvoked n times. Do we really want this behavior?
	 */
err_hfi_cmd_assign_ctxt:
	free(pctrl);
	pctrl = NULL;

err_calloc_hfi_ctrl:
	FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA, "[HFI1-DIRECT] Error initializing the context\n");
	return NULL;
}

static int opx_hfi1_rdma_set_pkey(struct fi_opx_hfi1_context_internal *internal, uint16_t pkey)
{
	struct hfi1_set_pkey_cmd set_pkey_cmd = {.pkey = pkey};
	struct ibv_context	*ibv_context  = (struct ibv_context *) internal->context.ibv_context;

	assert(opx_rdma_ops.hfi1_direct_verbs_enabled);

	int ret = OPX_HFI1_RDMA_FN(hfi1_set_pkey)(ibv_context, &set_pkey_cmd);
	if (ret) {
		FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA,
			"[HFI1-DIRECT] hfi_unit %d hfi1_set_pkey(%#X) fail, ret %d, err %d %s\n",
			internal->context.hfi_unit, pkey, ret, errno, strerror(errno));
		return -1;
	} else {
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
			     "[HFI1-DIRECT] hfi_unit %d hfi1_set_pkey(%#X) success\n", internal->context.hfi_unit,
			     pkey);
	}

	int selinux = 0;
	fi_param_get_bool(fi_opx_global.prov, "selinux", &selinux);
	if (selinux) {
		/*
		 * If SELinux is in use the kernel may have changed our JKey
		 * based on what we supply for the PKey so go ahead and
		 * interrogate the user info again and update our saved copy. In
		 * the future there may be a new IOCTL to get the JKey only. For
		 * now, this temporary workaround works.
		 */
		struct hfi1_user_info_rsp user_info_rsp = {}; /* (cdev) struct hfi1_base_info */

		ret = OPX_HFI1_RDMA_FN(hfi1_user_info)(ibv_context, &user_info_rsp);
		if (ret) {
			FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA,
				"[HFI1-DIRECT] hfi_unit %d hfi1_user_info fail, ret %d, err %d %s\n",
				internal->context.hfi_unit, ret, errno, strerror(errno));
			return -1;
		}
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "FI_OPX_SELINUX is set, updating jkey to 0x%04x\n",
			     user_info_rsp.jkey);
		internal->ctrl->base_info.jkey = user_info_rsp.jkey;
	}

	return 0;
}

static int opx_hfi1_rdma_ack_events(struct fi_opx_hfi1_context *context, uint64_t ackbits)
{
	struct hfi1_ack_event_cmd ack_event_cmd = {.event = ackbits};
	struct ibv_context	 *ibv_context	= (struct ibv_context *) context->ibv_context;

	assert(opx_rdma_ops.hfi1_direct_verbs_enabled);

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "[HFI1-DIRECT] hfi1_ack_event(ackbits %#lX)\n", ackbits);
	int ret = OPX_HFI1_RDMA_FN(hfi1_ack_event)(ibv_context, &ack_event_cmd);
	if (ret) {
		FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA, "[HFI1-DIRECT] hfi_unit %d fail, ret %d, err %d %s\n",
			context->hfi_unit, ret, errno, strerror(errno));
		return -1;
	} else {
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "[HFI1-DIRECT] hfi_unit %d success\n",
			     context->hfi_unit);
	}

	return 0;
}

static int opx_hfi1_rdma_reset_context(struct fi_opx_hfi1_context *context)
{
	assert(opx_rdma_ops.hfi1_direct_verbs_enabled);
	struct ibv_context *ibv_context = (struct ibv_context *) context->ibv_context;

	int ret = OPX_HFI1_RDMA_FN(hfi1_ctxt_reset)(ibv_context);
	if (ret) {
		FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA, "[HFI1-DIRECT] hfi_unit %d fail, ret %d, err %d %s\n",
			context->hfi_unit, ret, errno, strerror(errno));
		return -1;
	} else {
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "[HFI1-DIRECT] hfi_unit %d success\n",
			     context->hfi_unit);
	}

	return 0;
}

int32_t opx_hfi1_rdma_update_tid(struct fi_opx_hfi1_context *context, uint64_t vaddr, uint32_t *length,
				 uint64_t tidlist, uint32_t *tidcnt, uint16_t flags)
{
	assert(opx_rdma_ops.hfi1_direct_verbs_enabled);
	struct ibv_context *ibv_context = (struct ibv_context *) context->ibv_context;

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		     "[HFI1-DIRECT] ibv_context %p, vaddr %#lX, length %u, tidlist %#lX, tidcnt %u, flags %#hX\n",
		     ibv_context, vaddr, *length, tidlist, *tidcnt, flags);

	struct hfi1_tid_update_cmd cmd = {};
	struct hfi1_tid_update_rsp rsp = {};

	cmd.context = 0ULL;
	cmd.flags   = flags;
	cmd.length  = *length;
	cmd.tidcnt  = *tidcnt;
	cmd.tidlist = tidlist;
	cmd.vaddr   = vaddr;

	int ret = OPX_HFI1_RDMA_FN(hfi1_tid_update)(ibv_context, &cmd, &rsp);
	if (ret) {
		FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA, "[HFI1-DIRECT] hfi_unit %d ret %d, err %d %s\n",
			context->hfi_unit, ret, errno, strerror(errno));
	} else {
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
			     "[HFI1-DIRECT] hfi_unit %d success, length %d, tidcnt %d\n", context->hfi_unit, rsp.length,
			     rsp.tidcnt);
	}

	*length = rsp.length;
	*tidcnt = rsp.tidcnt;

	return ret;
}

int32_t opx_hfi1_rdma_free_tid(struct fi_opx_hfi1_context *context, uint64_t tidlist, uint32_t tidcnt)
{
	assert(opx_rdma_ops.hfi1_direct_verbs_enabled);
	struct ibv_context *ibv_context = (struct ibv_context *) context->ibv_context;

	struct hfi1_tid_free_cmd cmd = {};
	struct hfi1_tid_free_rsp rsp = {};

	cmd.tidcnt  = tidcnt;
	cmd.tidlist = tidlist;

	int ret = OPX_HFI1_RDMA_FN(hfi1_tid_free)(ibv_context, &cmd, &rsp);
	if (ret) {
		FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA, "[HFI1-DIRECT] hfi_unit %d fail, ret %d, err %d %s\n",
			context->hfi_unit, ret, errno, strerror(errno));
	} else {
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "[HFI1-DIRECT] hfi_unit %d success, rsp.tidcnt %d\n",
			     context->hfi_unit, rsp.tidcnt);
	}

	return ret;
}

void opx_hfi1_rdma_context_close(void *ibv_context)
{
	/* ignore errors */

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_DOMAIN, "[HFI1-DIRECT] Close context %p and libraries\n", ibv_context);
	if (opx_rdma_ops.libibverbs && ibv_context) {
		OPX_HFI1_RDMA_FN(ibv_close_device)((struct ibv_context *) ibv_context);
	}
}

void opx_hfi1_rdma_lib_close()
{
	if (opx_rdma_ops.libhfi1verbs) {
		dlclose(opx_rdma_ops.libhfi1verbs);
	}
	opx_rdma_ops.libhfi1verbs = NULL;

	if (opx_rdma_ops.libibverbs) {
		dlclose(opx_rdma_ops.libibverbs);
	}
	opx_rdma_ops.libibverbs = NULL;

	opx_rdma_ops.one_time_setup = false;
}

#else /* HAVE_HFI1_DIRECT_VERBS */

/* Define dummy functions */

__OPX_FORCE_INLINE__
bool opx_hfi1_direct_verbs_enabled(void)
{
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "[HFI1-DIRECT] !HAVE_HFI1_DIRECT_VERBS\n");
	return false;
}

__OPX_FORCE_INLINE__
int opx_hfi1_rdma_context_open(int unit, int port, uint64_t open_timeout, const bool set_user_version,
			       unsigned int *user_version, void **ibv_context)
{
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "[HFI1-DIRECT] !HAVE_HFI1_DIRECT_VERBS\n");
	return -1;
}

__OPX_FORCE_INLINE__
struct _hfi_ctrl *opx_hfi1_rdma_userinit(int fd, struct fi_opx_hfi1_context_internal *internal, int unit, int port)
{
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "[HFI1-DIRECT] !HAVE_HFI1_DIRECT_VERBS\n");
	return (struct _hfi_ctrl *) NULL;
}

__OPX_FORCE_INLINE__
int opx_hfi1_rdma_set_pkey(struct fi_opx_hfi1_context_internal *internal, uint16_t pkey)
{
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "[HFI1-DIRECT] !HAVE_HFI1_DIRECT_VERBS\n");
	return -1;
}

__OPX_FORCE_INLINE__
int opx_hfi1_rdma_ack_events(struct fi_opx_hfi1_context *context, uint64_t ackbits)
{
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "[HFI1-DIRECT] !HAVE_HFI1_DIRECT_VERBS\n");
	return -1;
}

__OPX_FORCE_INLINE__
int opx_hfi1_rdma_reset_context(struct fi_opx_hfi1_context *context)
{
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "[HFI1-DIRECT] !HAVE_HFI1_DIRECT_VERBS\n");
	return -1;
}

__OPX_FORCE_INLINE__
bool opx_hfi1_rdma_op_initialize(const bool use_new_tid_ops)
{
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "[HFI1-DIRECT] !HAVE_HFI1_DIRECT_VERBS\n");

	/* Set old tid functions */
	opx_fn_hfi1_free_tid   = opx_hfi_free_tid;
	opx_fn_hfi1_update_tid = opx_hfi_update_tid;

	return false;
}

int32_t opx_hfi1_rdma_update_tid(struct fi_opx_hfi1_context *context, uint64_t vaddr, uint32_t *length,
				 uint64_t tidlist, uint32_t *tidcnt, uint16_t flags)
{
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "[HFI1-DIRECT] !HAVE_HFI1_DIRECT_VERBS\n");
	return -1U;
}

int32_t opx_hfi1_rdma_free_tid(struct fi_opx_hfi1_context *context, uint64_t tidlist, uint32_t tidcnt)
{
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "[HFI1-DIRECT] !HAVE_HFI1_DIRECT_VERBS\n");
	return -1U;
}

void opx_hfi1_rdma_context_close(void *ibv_context)
{
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "[HFI1-DIRECT] !HAVE_HFI1_DIRECT_VERBS\n");
}

void opx_hfi1_rdma_lib_close()
{
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "[HFI1-DIRECT] !HAVE_HFI1_DIRECT_VERBS\n");
}

#endif /* HAVE_HFI1_DIRECT_VERBS */

/* clang-format off

   Old structurs             New structures
   ====================      ====================================
   hfi1_user_info_dep        hfi1_assign_ctxt_cmd
   hfi1_ctxt_info            hfi1_ctxt_info_rsp
   hfi1_base_info            hfi1_user_info_rsp
   struct hfi1_tid_info      hfi1_tid_update_cmd/hfi1_tid_update_rsp/hfi1_tid_free_cmd/hfi1_tid_free_rsp

   CDEV/RDMA CORE Wrapper function table

   WRAPPER                              CDEV                                 HFI1-DIRECT/HFI1 DIRECT VERBS
   ==================================   =========================            ===============================
   opx_hfi1_wrapper_context_open        opx_hfi_context_open                 opx_hfi1_rdma_context_open
   opx_hfi1_wrapper_userinit            opx_hfi_userinit                     opx_hfi1_rdma_userinit
   opx_hfi1_wrapper_set_pkey            opx_hfi_set_pkey                     opx_hfi1_rdma_set_pkey
   opx_hfi1_wrapper_update_tid          opx_hfi_update_tid                   opx_hfi1_rdma_update_tid
   opx_hfi1_wrapper_free_tid            opx_hfi_free_tid                     opx_hfi1_rdma_free_tid
   opx_hfi1_wrapper_ack_events          opx_hfi_ack_events                   opx_hfi1_rdma_ack_events
   opx_hfi1_wrapper_reset_context       opx_hfi_reset_context                opx_hfi1_rdma_reset_context

   clang-format on
 */

/* Context open function */
int opx_hfi1_wrapper_context_open(const int unit, const int port, const uint64_t open_timeout,
				  const enum opx_hfi1_type hfi1_type, void **ibv_context, unsigned int *user_version,
				  int *fd_cdev, int *fd_verbs)
{
	if (!OPX_HFI1_VERBS_CONTEXTS_ONLY) {
		int fd = opx_hfi_context_open(unit, port, open_timeout, user_version);
		if (fd < 0) {
			FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA,
				"Failed to open cdev context for HFI unit %d port %d type %u\n", unit, port, hfi1_type);
			return -1;
		}

		(*fd_cdev) = fd;
	}

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "[HFI1-DIRECT] Attempt HAVE_HFI1_DIRECT_VERBS\n");

	/* TODO: verbs contexts: Until we switch over to using verbs contexts exclusively, we need to avoid using the
	 * new TID functions and setting the user_version. */
	void *tmp_ibv_context = NULL;
	int   verbs_fd	      = -1;
	if (opx_hfi1_rdma_op_initialize(OPX_HFI1_VERBS_CONTEXTS_ONLY)) {
		verbs_fd = opx_hfi1_rdma_context_open(unit, port, open_timeout, OPX_HFI1_VERBS_CONTEXTS_ONLY,
						      user_version, &tmp_ibv_context);
	}

	(*fd_verbs)    = verbs_fd;
	(*ibv_context) = (verbs_fd != -1) ? tmp_ibv_context : NULL;
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "[HFI1-DIRECT] hfi1_type %u %s\n", hfi1_type,
		     (verbs_fd != -1) ? "ENABLED" : "DISABLED");

	/* If we failed to open a verbs context, and we didn't already open a cdev context, fall back to cdev */
	if (verbs_fd == -1 && OPX_HFI1_VERBS_CONTEXTS_ONLY) {
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "[HFI1-DIRECT] !HAVE_HFI1_DIRECT_VERBS fallback\n");
		(*fd_cdev) = opx_hfi_context_open(unit, port, open_timeout, user_version);
	}
	return ((*fd_cdev) < 0);
}

static int opx_get_current_proc_core()
{
	int core_id;
	core_id = sched_getcpu();
	if (core_id < 0) {
		return -EINVAL;
	}
	return core_id;
}
void opx_verbose_selection(struct fi_opx_hfi1_context_internal *internal, struct _hfi_ctrl *ctrl)
{
	const int core_id   = opx_get_current_proc_core();
	const int hfi_count = opx_hfi_get_num_units();

	struct hfi1_ctxt_info *cinfo = &ctrl->ctxt_info;

	const short int ctxt	  = cinfo->ctxt;      /* ctxt on unit assigned to caller */
	const short int subctxt	  = cinfo->subctxt;   /* subctxt on unit assigned to caller */
	const short int numa_node = cinfo->numa_node; /* NUMA node of the assigned device */
	const short int rec_cpu	  = cinfo->rec_cpu;   /* cpu # for affinity (0xffff if none) */
	const short int send_ctxt = cinfo->send_ctxt; /* send context in use by this user context */

	struct hfi1_base_info *binfo = &ctrl->base_info;

	const int hw_version = binfo->hw_version; /* version of hardware, for feature checking. */
	const int sw_version = binfo->sw_version; /* version of software, for feature checking. */

	const int	unit	  = ctrl->__hfi_unit;
	const int	port	  = ctrl->__hfi_port;
	const int	num_ports = opx_hfi_get_num_ports(unit);
	const opx_lid_t lid	  = opx_hfi_get_port_lid(unit, port);

	// too early for env to have been checked
	int mixed_network = OPX_SW_HFI1_TYPE;
	if (!(OPX_SW_HFI1_TYPE & (OPX_HFI1_WFR | OPX_HFI1_MIXED_9B))) {
		if (fi_param_get_bool(fi_opx_global.prov, "mixed_network", &mixed_network) == FI_SUCCESS) {
			if (mixed_network) {
				mixed_network = OPX_HFI1_MIXED_9B;
			}
		} else { // default is mixed
			mixed_network = OPX_HFI1_MIXED_9B;
		}
	}

	FI_TRACE(&fi_opx_provider, FI_LOG_FABRIC, "SW/HW version %#X/%#X. API version %#X. Core %d(%d). \n", sw_version,
		 hw_version, internal->user_info.userversion, rec_cpu, core_id);
	FI_TRACE(&fi_opx_provider, FI_LOG_FABRIC, "Selected %s unit %d (%d units) and port %d (%d ports); \n",
		 OPX_HFI1_TYPE_STRING(mixed_network), unit, hfi_count, port, num_ports);
	FI_TRACE(&fi_opx_provider, FI_LOG_FABRIC, "Core %d(%d). NUMA domain is %d; HFI NUMA domain is %ld. \n", rec_cpu,
		 core_id, numa_node, opx_hfi_sysfs_unit_read_node_s64(unit));
	FI_TRACE(&fi_opx_provider, FI_LOG_FABRIC, "LID %d, Receive context %d, sub-context %d, Send context %d\n", lid,
		 ctxt, subctxt, send_ctxt);
	FI_TRACE(&fi_opx_provider, FI_LOG_FABRIC,
		 "credits %u, rcvegr_size %u, rcvtids %u, egrtids %u, rcvhdrq_cnt %u, rcvhdrq_entsize  %u\n",
		 cinfo->credits, cinfo->rcvegr_size, cinfo->rcvtids, cinfo->egrtids, cinfo->rcvhdrq_cnt,
		 cinfo->rcvhdrq_entsize);

	FI_TRACE(&fi_opx_provider, FI_LOG_FABRIC, "fi_opx_global.hfi_local_info.local_lids_size         = %d\n",
		 fi_opx_global.hfi_local_info.local_lids_size);
	FI_TRACE(&fi_opx_provider, FI_LOG_FABRIC, "fi_opx_global.hfi_local_info.sw_type                 = %d\n",
		 fi_opx_global.hfi_local_info.sw_type);
	FI_TRACE(&fi_opx_provider, FI_LOG_FABRIC, "fi_opx_global.hfi_local_info.hw_type                 = %d\n",
		 fi_opx_global.hfi_local_info.hw_type);
	FI_TRACE(&fi_opx_provider, FI_LOG_FABRIC, "fi_opx_global.hfi_local_info.sim_rctxt_fd            = %d\n",
		 fi_opx_global.hfi_local_info.sim_rctxt_fd);
	FI_TRACE(&fi_opx_provider, FI_LOG_FABRIC, "fi_opx_global.hfi_local_info.sim_sctxt_fd            = %d\n",
		 fi_opx_global.hfi_local_info.sim_sctxt_fd);
	FI_TRACE(&fi_opx_provider, FI_LOG_FABRIC, "fi_opx_global.hfi_local_info.lid                     = %d\n",
		 fi_opx_global.hfi_local_info.lid);
	FI_TRACE(&fi_opx_provider, FI_LOG_FABRIC, "fi_opx_global.hfi_local_info.hfi_unit                = %d\n",
		 fi_opx_global.hfi_local_info.hfi_unit);
	FI_TRACE(&fi_opx_provider, FI_LOG_FABRIC, "fi_opx_global.hfi_local_info.sriov                   = %d\n",
		 fi_opx_global.hfi_local_info.sriov);
	FI_TRACE(&fi_opx_provider, FI_LOG_FABRIC, "fi_opx_global.hfi_local_info.multi_vm                = %d\n",
		 fi_opx_global.hfi_local_info.multi_vm);
	FI_TRACE(&fi_opx_provider, FI_LOG_FABRIC, "fi_opx_global.hfi_local_info.multi_lid               = %d\n",
		 fi_opx_global.hfi_local_info.multi_lid);
	FI_TRACE(&fi_opx_provider, FI_LOG_FABRIC, "fi_opx_global.hfi_local_info.min_rctxt               = %d\n",
		 fi_opx_global.hfi_local_info.min_rctxt);
	FI_TRACE(&fi_opx_provider, FI_LOG_FABRIC, "fi_opx_global.hfi_local_info.max_rctxt               = %d\n",
		 fi_opx_global.hfi_local_info.max_rctxt);

	FI_TRACE(&fi_opx_provider, FI_LOG_FABRIC, "fi_opx_global.progress                               = %s\n",
		 fi_tostr(&fi_opx_global.progress, FI_TYPE_PROGRESS));
	FI_TRACE(&fi_opx_provider, FI_LOG_FABRIC, "fi_opx_global.rcvhdrq_entry_dws                      = %d\n",
		 fi_opx_global.rcvhdrq_entry_dws);
	FI_TRACE(&fi_opx_provider, FI_LOG_FABRIC, "fi_opx_global.pkt_size                               = %d\n",
		 fi_opx_global.pkt_size);

	FI_TRACE(&fi_opx_provider, FI_LOG_FABRIC, "fi_opx_global.opx_hfi1_type_strings[OPX_HFI1_UNDEF]  = %s\n",
		 fi_opx_global.opx_hfi1_type_strings[OPX_HFI1_UNDEF]);
	FI_TRACE(&fi_opx_provider, FI_LOG_FABRIC, "fi_opx_global.opx_hfi1_type_strings[OPX_HFI1_WFR]    = %s\n",
		 fi_opx_global.opx_hfi1_type_strings[OPX_HFI1_WFR]);
	FI_TRACE(&fi_opx_provider, FI_LOG_FABRIC, "fi_opx_global.opx_hfi1_type_strings[OPX_HFI1_JKR]    = %s\n",
		 fi_opx_global.opx_hfi1_type_strings[OPX_HFI1_JKR]);
	FI_TRACE(&fi_opx_provider, FI_LOG_FABRIC, "fi_opx_global.opx_hfi1_type_strings[OPX_HFI1_CYR]    = %s\n",
		 fi_opx_global.opx_hfi1_type_strings[OPX_HFI1_CYR]);
	FI_TRACE(&fi_opx_provider, FI_LOG_FABRIC, "fi_opx_global.opx_hfi1_type_strings[OPX_HFI1_CNX000] = %s\n",
		 fi_opx_global.opx_hfi1_type_strings[OPX_HFI1_CNX000]);
}
/* Environment variable is not published */
#define OPX_VERBOSE_SELECTION(_internal, _ctrl)          \
	if (getenv("FI_OPX_VERBOSE_SELECTION")) {        \
		opx_verbose_selection(_internal, _ctrl); \
	}

struct _hfi_ctrl *opx_hfi1_wrapper_userinit(int fd, struct fi_opx_hfi1_context_internal *internal, int unit, int port)
{
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "[HFI1-DIRECT] fd %d\n", fd);
	if (OPX_HFI1_VERBS_CONTEXTS_ONLY && opx_hfi1_direct_verbs_enabled()) {
		struct _hfi_ctrl *ctrl = opx_hfi1_rdma_userinit(fd, internal, unit, port);
		OPX_VERBOSE_SELECTION(internal, ctrl);
		return ctrl;
	}

	struct _hfi_ctrl *ctrl = opx_hfi_userinit(fd, internal, unit, port);
	OPX_VERBOSE_SELECTION(internal, ctrl);
	return ctrl;
}

int opx_hfi1_wrapper_set_pkey(struct fi_opx_hfi1_context_internal *internal, uint16_t pkey)
{
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "[HFI1-DIRECT] ibv_context %p, pkkey %#X\n",
		     internal->context.ibv_context, pkey);
	if (!OPX_HFI1_VERBS_CONTEXTS_ONLY || !opx_hfi1_direct_verbs_enabled()) {
		struct _hfi_ctrl *ctrl = internal->ctrl;
		return opx_hfi_set_pkey(ctrl, pkey);
	}
	return opx_hfi1_rdma_set_pkey(internal, pkey);
}

int opx_hfi1_wrapper_ack_events(struct fi_opx_hfi1_context *context, uint64_t ackbits)
{
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "[HFI1-DIRECT] ibv_context %p, ackbits %#lX\n",
		     context->ibv_context, ackbits);
	if (!OPX_HFI1_VERBS_CONTEXTS_ONLY || !opx_hfi1_direct_verbs_enabled()) {
		return opx_hfi_ack_events(context->fd_cdev, ackbits);
	}
	return opx_hfi1_rdma_ack_events(context, ackbits);
}

int opx_hfi1_wrapper_reset_context(struct fi_opx_hfi1_context *context)
{
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "[HFI1-DIRECT] ibv_context %pX\n", context->ibv_context);
	if (!OPX_HFI1_VERBS_CONTEXTS_ONLY || !opx_hfi1_direct_verbs_enabled()) {
		return opx_hfi_reset_context(context->fd_cdev);
	}
	return opx_hfi1_rdma_reset_context(context);
}

/* TID Functions */

/* update length and tidcnt expected TID entries from the array pointed to by tidinfo. */
/* Returns 0 on success, else -1 with errno set.
   See full description at declaration */
int32_t opx_hfi_update_tid(struct fi_opx_hfi1_context *context, uint64_t vaddr, uint32_t *length, uint64_t tidlist,
			   uint32_t *tidcnt, uint16_t flags)
{
	struct hfi1_cmd	  cmd;
	struct _hfi_ctrl *ctrl = context->ctrl;

#ifdef OPX_HMEM
	struct hfi1_tid_info_v3 tidinfo;
#else
	struct hfi1_tid_info tidinfo;
#endif

	int err;

	tidinfo.vaddr  = vaddr;	  /* base address for this send to map */
	tidinfo.length = *length; /* length of vaddr */

	tidinfo.tidlist = tidlist; /* driver copies tids back directly */
	tidinfo.tidcnt	= 0;	   /* clear to zero */

#ifdef OPX_HMEM
	cmd.type	= OPX_HFI_CMD_TID_UPDATE_V3;
	tidinfo.flags	= flags;
	tidinfo.context = 0ull;
#else
	cmd.type = OPX_HFI_CMD_TID_UPDATE; /* HFI1_IOCTL_TID_UPDATE */
#endif
	FI_DBG(&fi_opx_provider, FI_LOG_MR, "OPX_DEBUG_ENTRY update [%p - %p], length %u (pages %lu)\n", (void *) vaddr,
	       (void *) (vaddr + *length), *length, (*length) / PAGE_SIZE);

	cmd.len	 = sizeof(tidinfo);
	cmd.addr = (__u64) &tidinfo;

	errno = 0;
	err   = opx_hfi_cmd_write(ctrl->fd, &cmd, sizeof(cmd));

	if (err != -1) {
		assert(err == 0);
		struct hfi1_tid_info *rettidinfo = (struct hfi1_tid_info *) cmd.addr;
		assert(rettidinfo->length);
		assert(rettidinfo->tidcnt);

		if (rettidinfo->length != *length) {
			FI_DBG(&fi_opx_provider, FI_LOG_MR,
			       "PARTIAL UPDATE errno %d  \"%s\" INPUTS vaddr [%p - %p] length %u (pages %lu), OUTPUTS vaddr [%p - %p] length %u (pages %lu), tidcnt %u\n",
			       errno, strerror(errno), (void *) vaddr, (void *) (vaddr + *length), *length,
			       (*length) / PAGE_SIZE, (void *) rettidinfo->vaddr,
			       (void *) (rettidinfo->vaddr + rettidinfo->length), rettidinfo->length,
			       rettidinfo->length / PAGE_SIZE, rettidinfo->tidcnt);
		}
		/* Always update outputs, even on soft errors */
		*length = rettidinfo->length;
		*tidcnt = rettidinfo->tidcnt;

		FI_DBG(&fi_opx_provider, FI_LOG_MR,
		       "TID UPDATE IOCTL returned %d errno %d  \"%s\" vaddr [%p - %p] length %u (pages %lu), tidcnt %u\n",
		       err, errno, strerror(errno), (void *) vaddr, (void *) (vaddr + *length), *length,
		       (*length) / PAGE_SIZE, *tidcnt);

		return 0;
	}

	if (errno == ENOSPC) {
		FI_DBG(&fi_opx_provider, FI_LOG_MR,
		       "IOCTL FAILED : No TIDs available, requested range=%p-%p (%u bytes, %lu pages)\n",
		       (void *) vaddr, (void *) (vaddr + *length), *length, (*length) / PAGE_SIZE);
		err = -FI_ENOSPC;
	} else {
		FI_WARN(&fi_opx_provider, FI_LOG_MR,
			"IOCTL FAILED ERR %d errno %d \"%s\" requested range=%p-%p (%u bytes, %lu pages)\n", err, errno,
			strerror(errno), (void *) vaddr, (void *) (vaddr + *length), *length, (*length) / PAGE_SIZE);
	}

	/* Hard error, we can't trust these */
	*length = 0;
	*tidcnt = 0;

	return err;
}

int32_t opx_hfi_free_tid(struct fi_opx_hfi1_context *context, uint64_t tidlist, uint32_t tidcnt)
{
	struct hfi1_cmd	     cmd;
	struct hfi1_tid_info tidinfo;
	int		     err;
	struct _hfi_ctrl    *ctrl = context->ctrl;

	tidinfo.tidlist = tidlist; /* input to driver */
	tidinfo.tidcnt	= tidcnt;

	cmd.type = OPX_HFI_CMD_TID_FREE; /* HFI1_IOCTL_TID_FREE */
	cmd.len	 = sizeof(tidinfo);
	cmd.addr = (__u64) &tidinfo;
	FI_DBG(&fi_opx_provider, FI_LOG_MR, "OPX_DEBUG_ENTRY tidcnt %u\n", tidcnt);
	errno					    = 0;
	err					    = opx_hfi_cmd_write(ctrl->fd, &cmd, sizeof(cmd));
	__attribute__((__unused__)) int saved_errno = errno;

	if (err == -1) {
		FI_WARN(&fi_opx_provider, FI_LOG_MR, "FAILED ERR %d  errno %d \"%s\" tidcnt %u\n", err, saved_errno,
			strerror(saved_errno), tidcnt);
	}
	FI_DBG(&fi_opx_provider, FI_LOG_MR, "ERR %d  errno %d  \"%s\"\n", err, saved_errno, strerror(saved_errno));

	return err;
}
