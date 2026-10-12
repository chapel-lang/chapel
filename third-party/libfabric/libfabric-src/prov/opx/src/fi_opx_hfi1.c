/*
 * Copyright (C) 2021-2025 by Cornelis Networks.
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
#include <assert.h>
#include <stdlib.h>
#include <numa.h>
#include <inttypes.h>
#include <sys/sysinfo.h>
#include <sys/file.h>
#include <fcntl.h>
#include <unistd.h>

#include "rdma/fabric.h" // only for 'fi_addr_t' ... which is a typedef to uint64_t
#include "rdma/opx/fi_opx_hfi1.h"
#include "rdma/opx/fi_opx_hfi1_inlines.h"
#include "rdma/opx/fi_opx.h"
#include "rdma/opx/fi_opx_eq.h"
#include "rdma/opx/fi_opx_hfi1_sdma.h"
#include "ofi_mem.h"

#include "fi_opx_hfi_select.h"
#include "rdma/opx/opx_hfi1_cn5000.h"

#include "rdma/opx/opx_tracer.h"

#define OPX_SHM_ENABLE_ON      1
#define OPX_SHM_ENABLE_OFF     0
#define OPX_SHM_ENABLE_DEFAULT OPX_SHM_ENABLE_ON

#define BYTE2DWORD_SHIFT (2)

/* RZV messages under FI_OPX_TID_MSG_MISALIGNED_THRESHOLD
 * will fallback to Eager Ring (not TID) RZV if the
 * buffer is misaligned more than FI_OPX_TID_MISALIGNED_THRESHOLD
 */

/* Number of bytes allowed to be misaligned on small TID RZV
 * FI_OPX_TID_MISALIGNED_THRESHOLD is arbitrary, based on testing.
 *  - 64 bytes
 */
#ifndef FI_OPX_TID_MISALIGNED_THRESHOLD
#define FI_OPX_TID_MISALIGNED_THRESHOLD 64
#endif

/* Maximum message size that falls back on misaligned buffers
 * FI_OPX_TID_MSG_MISALIGNED_THRESHOLD is arbitrary, based on testing.
 *  - 15 pages (64K)
 */
#ifndef FI_OPX_TID_MSG_MISALIGNED_THRESHOLD
#define FI_OPX_TID_MSG_MISALIGNED_THRESHOLD (15 * OPX_HFI1_TID_PAGESIZE)
#endif

#define OPX_GPU_IPC_MIN_THRESHOLD 2048
/*
 * Return the NUMA node id where the process is currently running.
 */
static int opx_get_current_proc_location()
{
	int core_id, node_id;

	core_id = sched_getcpu();
	if (core_id < 0) {
		return -EINVAL;
	}

	node_id = numa_node_of_cpu(core_id);
	if (node_id < 0) {
		return -EINVAL;
	}

	return node_id;
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

static inline uint64_t fi_opx_hfi1_header_count_to_poll_mask(uint64_t rcvhdrq_cnt)
{
	/* For optimization, the fi_opx_hfi1_poll_once() function uses a mask to wrap around the end of the
	** ring buffer.  To compute the mask, multiply the number of entries in the ring buffer by the sizeof
	** one entry.  Since the count is 0-based, subtract 1 from the value of
	** /sys/module/hfi1/parameters/rcvhdrcnt, which is set in the hfi1 module parms and
	** will not change at runtime
	*/
	return (rcvhdrq_cnt - 1) * 32;
}

static void opx_hfi_setup_ctx_shring_grps(int hfi_unit_number, int *ctx_groups, int *ep_per_hfi_context,
					  int *group_offset)
{
	*ctx_groups	    = 1;
	*ep_per_hfi_context = 0;
	*group_offset	    = 0;

	int num_ctxs = opx_hfi_get_num_contexts(hfi_unit_number);
	if (num_ctxs == 0) {
		FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA,
			"Unable to determine the number of contexts available for use on HFI unit %d. Context sharing will be disabled.\n",
			hfi_unit_number);
		return;
	}

	*ctx_groups = num_ctxs;

	/* Use an offset based on hfi_unit number when setting subctxt_id. On multirail systems
	the subctxt_id must be unique on a given node even if there are multiple HFIs.*/
	*group_offset = hfi_unit_number * num_ctxs;

	if (fi_param_get_int(fi_opx_global.prov, "endpoints_per_hfi_context", ep_per_hfi_context) == FI_SUCCESS) {
		if ((*ep_per_hfi_context < 2) || (*ep_per_hfi_context > HFI1_MAX_SHARED_CTXTS)) {
			FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA,
				"Invalid value for FI_OPX_ENDPOINTS_PER_HFI_CONTEXT. Valid values are 2 through 8. Context sharing will be disabled.\n");
			*ctx_groups = 1;
		} else {
			FI_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
				 "FI_OPX_ENDPOINTS_PER_HFI_CONTEXT set to %d. Mapping %d endpoints per HFI context.\n",
				 *ep_per_hfi_context, *ep_per_hfi_context);
		}
	} else {
		long nproc = sysconf(_SC_NPROCESSORS_ONLN);
		if (nproc == -1) {
			// TODO: Should context sharing be disabled in this case instead of
			// defaulting to 2 endpoints per HFI context?
			FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA,
				"Unable to determine the number of processors online. Defaulting to 2 endpoints per HFI context. Please use FI_OPX_ENDPOINTS_PER_HFI_CONTEXT for finer grain control.\n");
			*ep_per_hfi_context = 2;
		} else {
			*ep_per_hfi_context = MIN(HFI1_MAX_SHARED_CTXTS, nproc / num_ctxs);
			FI_TRACE(
				fi_opx_global.prov, FI_LOG_EP_DATA,
				"FI_OPX_ENDPOINTS_PER_HFI_CONTEXT not specified. Mapping %d endpoints per HFI context as a default value based on (%ld processors online) / (%d available contexts on HFI unit %d).\n",
				*ep_per_hfi_context, nproc, num_ctxs, hfi_unit_number);
		}
	}
}

// Used by fi_opx_hfi1_context_open as a convenience.
static int opx_open_hfi_and_context(struct _hfi_ctrl **ctrl, struct fi_opx_hfi1_context_internal *internal,
				    uuid_t unique_job_key, int hfi_unit_number, int *fd_cdev, int *fd_verbs)
{
	int	     port = opx_select_port_index(hfi_unit_number) + 1;
	unsigned int user_version;
	void	    *ibv_context;
	if (opx_hfi1_wrapper_context_open(hfi_unit_number, port, 0, OPX_SW_HFI1_TYPE, &ibv_context, &user_version,
					  fd_cdev, fd_verbs) != 0) {
		FI_WARN(&fi_opx_provider, FI_LOG_FABRIC, "Unable to open HFI unit %d.\n", hfi_unit_number);
		return -1;
	}
	FI_DBG_TRACE(&fi_opx_provider, FI_LOG_FABRIC, "opx_hfi_context_open fd_cdev %d fd_verbs %d.\n", *fd_cdev,
		     *fd_verbs);
	internal->context.ibv_context = ibv_context;

	// Check whether user wants to enable context sharing or not.
	int context_sharing_enabled = 0;
	if (fi_param_get_bool(fi_opx_global.prov, "context_sharing", &context_sharing_enabled) == FI_SUCCESS) {
		if (context_sharing_enabled) {
			FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA,
				"FI_OPX_CONTEXT_SHARING specified as TRUE, enabling context sharing.\n");
		} else {
			FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA,
				"FI_OPX_CONTEXT_SHARING specified as FALSE (default option), disabling context sharing.\n");
		}
	}

	/* When context sharing is enabled each endpoint will map N available contexts on a given node to 0, 1,
	... N - 1 context sharing groups. Each of these groups will have up to X number of endpoints based on
	either a user configurable value read from FI_OPX_ENDPOINTS_PER_HFI_CONTEXT or a default derived value
	based on system core count and the number of available contexts. When context sharing is disabled there
	will always be 1 ctx group with only 1 endpoint in it (no sharing).*/
	int ctx_groups	       = 1;
	int ep_per_hfi_context = 0;
	int group_offset       = 0;
	if (context_sharing_enabled) {
		opx_hfi_setup_ctx_shring_grps(hfi_unit_number, &ctx_groups, &ep_per_hfi_context, &group_offset);
	}

	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA, "ctx_groups %d, ep_per_hfi_context %d, group_offset %d\n",
	       ctx_groups, ep_per_hfi_context, group_offset);

	memset(&internal->user_info, 0, sizeof(internal->user_info));
	memcpy(internal->user_info.uuid, unique_job_key, sizeof(internal->user_info.uuid));

	/* When no context sharing is enabled in OPX this while loop acts as a single call to
	opx_hfi1_wrapper_userinit. When context sharing is enabled this loop will iterate through all available
	ctx groups attempting to find a free slot in one. A context group will be filled up to the maximum of
	ep_per_hfi_context before endpoints in a job and on the same node move on to the next group to attempt
	opening a shared context.*/
	int i = 0;
	while (i < ctx_groups) {
		/* Both subctxt_id and subctxt_cnt should be set to zero if context sharing is disabled. */
		internal->user_info.userversion = user_version;
		internal->user_info.subctxt_id	= i + group_offset;
		internal->user_info.subctxt_cnt = ep_per_hfi_context;
		*ctrl				= opx_hfi1_wrapper_userinit(*fd_cdev, internal, hfi_unit_number, port);

		if (*ctrl) {
			assert((*ctrl)->__hfi_pg_sz == OPX_HFI1_TID_PAGESIZE);
			break;
		}

		i = i + 1;
	}

	if (!*ctrl) {
		opx_hfi_context_close(*fd_cdev, *fd_verbs);
		FI_WARN(&fi_opx_provider, FI_LOG_FABRIC, "Unable to open a context on HFI unit %d.\n", hfi_unit_number);
		(*fd_cdev)  = -1;
		(*fd_verbs) = -1;
		return -1;
	}

	return 0;
}

void opx_reset_context(struct fi_opx_ep *opx_ep, uint64_t events, const enum opx_hfi1_type hfi1_type,
		       const bool ctx_sharing)
{
	fi_opx_compiler_msync_writes();

	/* When there is a HFI1_EVENT_FROZEN event, both Tx and Rx contexts are halted by the driver.
	   However, for a HFI1_EVENT_LINK_DOWN, only the Tx is halted, Rx continues without changes.
	   Hence there is no need to reset the Rx related variables */
	if (events & HFI1_EVENT_FROZEN) {
		if (ctx_sharing) {
			opx_ep->rx->shd_ctx.hwcontext_ctrl->rx_hdrq_rhf_seq   = OPX_RHF_SEQ_INIT_VAL(OPX_SW_HFI1_TYPE);
			opx_ep->rx->shd_ctx.hwcontext_ctrl->hdrq_head	      = 0;
			opx_ep->rx->shd_ctx.hwcontext_ctrl->last_egrbfr_index = 0;
		} else {
			opx_ep->rx->state.hdrq.rhf_seq	   = OPX_RHF_SEQ_INIT_VAL(OPX_SW_HFI1_TYPE);
			opx_ep->rx->state.hdrq.head	   = 0;
			opx_ep->rx->egrq.last_egrbfr_index = 0;
		}
	}

	if (opx_hfi1_wrapper_reset_context(opx_ep->hfi)) {
		FI_WARN(&fi_opx_provider, FI_LOG_FABRIC, "Send context reset failed: %d.\n", errno);
		abort();
	}
	FI_WARN(&fi_opx_provider, FI_LOG_FABRIC, "Send context reset successfully.\n");

	opx_link_up_update_pio_credit_addr(opx_ep->hfi, opx_ep, ctx_sharing);
	opx_ep->tx->pio_state->fill_counter   = 0;
	opx_ep->tx->pio_state->scb_head_index = 0;
	union fi_opx_hfi1_pio_state pio_state = *opx_ep->tx->pio_state;
	FI_OPX_HFI1_UPDATE_CREDITS(pio_state, opx_ep->tx->pio_credits_addr);
	opx_ep->tx->pio_state->qw0 = pio_state.qw0;

	fi_opx_hfi1_poll_sdma_completion(opx_ep);
	opx_hfi1_sdma_process_pending(opx_ep);
}

/* When the link is down, prevent reading the mmaped free_counters because the value is not reliable.
   Hence point the addr to a dummy location which always creates a credit full condition */
void opx_link_down_update_pio_credit_addr(struct fi_opx_hfi1_context *context, struct fi_opx_ep *opx_ep,
					  const bool ctx_sharing)
{
	if (ctx_sharing) {
		OPX_SHD_CTX_PIO_LOCK(ctx_sharing, opx_ep->tx);
		/* pio_credit_addr is unique for every process in the context sharing group. Hence use a local copy */
		context->credits_addr_copy     = context->info.pio.credits_addr;
		context->dummy_free_credits    = context->spio_ctrl->pio.free_counter_shadow;
		context->info.pio.credits_addr = &(context->dummy_free_credits);
		opx_ep->tx->pio_credits_addr   = context->info.pio.credits_addr;

		/* Since pio_state is shared, point to a dummy field until the pio_credits_addr is restored */
		context->dummy_pio_state.qw0 = context->spio_ctrl->pio.qw0;
		opx_ep->tx->pio_state	     = &(context->dummy_pio_state);

		FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA, "Pointing credit addr to %p from %p\n",
		       opx_ep->tx->pio_credits_addr, context->credits_addr_copy);
		FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA, "Pointing pio state to %p from %p\n", opx_ep->tx->pio_state,
		       &(context->spio_ctrl->pio));

		OPX_SHD_CTX_PIO_UNLOCK(ctx_sharing, opx_ep->tx);
	} else {
		context->credits_addr_copy = context->info.pio.credits_addr;

		context->dummy_free_credits    = context->state.pio.free_counter_shadow;
		context->info.pio.credits_addr = &(context->dummy_free_credits);
		opx_ep->tx->pio_credits_addr   = context->info.pio.credits_addr;

		FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA,
		       "Pointing credit addr to dummy field dummy_free_counter = %ld\n", context->dummy_free_credits);
	}
}

/* When the link comes back up, reset the credits pointer to the valid mmaped location
	In case of context sharing, the lock is already held by the caller */
void opx_link_up_update_pio_credit_addr(struct fi_opx_hfi1_context *context, struct fi_opx_ep *opx_ep,
					const bool ctx_sharing)
{
	if (ctx_sharing) {
		context->info.pio.credits_addr = context->credits_addr_copy;
		opx_ep->tx->pio_credits_addr   = context->info.pio.credits_addr;
		FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA,
		       "Pointing credit addr back to actual addr %p send_ctxt %d subctxt %d\n",
		       opx_ep->tx->pio_credits_addr, context->send_ctxt, context->subctxt);
		opx_ep->tx->pio_state = &(context->spio_ctrl->pio);
		FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA, "Pointing pio state to %p from %p\n", opx_ep->tx->pio_state,
		       &(context->spio_ctrl->pio));

	} else {
		FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA, "Pointing credit addr back to actual addr %p\n",
		       context->credits_addr_copy);
		context->info.pio.credits_addr = context->credits_addr_copy;
		opx_ep->tx->pio_credits_addr   = context->info.pio.credits_addr;
	}
}

static int fi_opx_get_daos_hfi_rank_inst(const uint8_t hfi_unit_number, const uint32_t rank)
{
	struct fi_opx_daos_hfi_rank_key key;
	struct fi_opx_daos_hfi_rank    *hfi_rank = NULL;

	memset(&key, 0, sizeof(key));
	key.hfi_unit_number = hfi_unit_number;
	key.rank	    = rank;

	HASH_FIND(hh, fi_opx_global.daos_hfi_rank_hashmap, &key, sizeof(key), hfi_rank);

	if (hfi_rank) {
		hfi_rank->instance++;

		FI_INFO(fi_opx_global.prov, FI_LOG_EP_DATA, "HFI %d assigned rank %d again: %d.\n", key.hfi_unit_number,
			key.rank, hfi_rank->instance);
	} else {
		int rc __attribute__((unused));
		rc = posix_memalign((void **) &hfi_rank, 32, sizeof(*hfi_rank));
		assert(rc == 0);

		hfi_rank->key	   = key;
		hfi_rank->instance = 0;
		HASH_ADD(hh, fi_opx_global.daos_hfi_rank_hashmap, key, sizeof(hfi_rank->key), hfi_rank);

		FI_INFO(fi_opx_global.prov, FI_LOG_EP_DATA, "HFI %d assigned rank %d entry created.\n",
			key.hfi_unit_number, key.rank);
	}

	return hfi_rank->instance;
}

void process_hfi_lookup(const int hfi_unit, const opx_lid_t lid)
{
	int lid_index = opx_local_lid_index(lid);

	if (lid_index != -1) {
		fi_opx_global.hfi_local_info.local_lid_entries[lid_index].instance++;

		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "HFI %d LID 0x%x again: %d.\n",
			     fi_opx_global.hfi_local_info.local_lid_entries[lid_index].hfi_unit, lid,
			     fi_opx_global.hfi_local_info.local_lid_entries[lid_index].instance);
	} else {
		assert(fi_opx_global.hfi_local_info.local_lids_size < OPX_MAX_HFIS);
		int new_index = fi_opx_global.hfi_local_info.local_lids_size++;
		fi_opx_global.hfi_local_info.local_lid_entries[new_index].hfi_unit = hfi_unit;
		fi_opx_global.hfi_local_info.local_lid_entries[new_index].instance = 0;
		fi_opx_global.hfi_local_info.local_lid_ids[new_index]		   = lid;

		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "HFI %u LID 0x%x entry created.\n", hfi_unit, lid);
	}
}

/* Remove "self" lid from shm and therefore use loopback */
void opx_remove_self_lid(const int hfi_unit, const opx_lid_t lid)
{
	assert(fi_opx_global.hfi_local_info.sriov);
	assert(fi_opx_global.hfi_local_info.local_lids_size <= 1);
	int lid_index = opx_local_lid_index(lid);
	/* Only used in sr-iov which only had one (self/my) lid */
	FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA, "HFI %u LID[%d] 0x%x entry removed.\n", hfi_unit, lid_index, lid);
	assert(lid_index <= 0);
	if (lid_index == -1) {
		/* Duplicate calls, already removed */
		return;
	}
	assert(fi_opx_global.hfi_local_info.local_lid_entries[lid_index].hfi_unit == hfi_unit);
	fi_opx_global.hfi_local_info.local_lids_size--;
	fi_opx_global.hfi_local_info.local_lid_entries[lid_index].hfi_unit = 0;
	fi_opx_global.hfi_local_info.local_lid_entries[lid_index].instance = 0;
	fi_opx_global.hfi_local_info.local_lid_ids[lid_index]		   = 0;
}

void fi_opx_init_hfi_lookup(bool sriov)
{
	int hfi_unit  = 0;
	int hfi_units = MIN(opx_hfi_get_num_units(), OPX_MAX_HFIS);

	if (hfi_units == 0) {
		FI_WARN(&fi_opx_provider, FI_LOG_EP_DATA, "No HFI units found.\n");
		return;
	}

	int shm_enable_env;
	if (fi_param_get_bool(fi_opx_global.prov, "shm_enable", &shm_enable_env) != FI_SUCCESS) {
		FI_INFO(fi_opx_global.prov, FI_LOG_EP_DATA, "shm_enable param not specified\n");
		shm_enable_env = OPX_SHM_ENABLE_DEFAULT;
	}
	if (sriov) {
		/* sr-iov will initially enable shm for "my" lid only, as-if the env was set, so fake it here.
		 * later checks may completely disable shm and thus enable self loopback later */
		FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA, "SR-IOV overrides FI_OPX_SHM_ENABLE always disables SHM\n");
		shm_enable_env = OPX_SHM_ENABLE_OFF;
	}

	/* Always insert an entry for self */
	process_hfi_lookup(fi_opx_global.hfi_local_info.hfi_unit, fi_opx_global.hfi_local_info.lid);

	if (shm_enable_env != OPX_SHM_ENABLE_ON) {
		return;
	}

	for (hfi_unit = 0; hfi_unit < hfi_units; hfi_unit++) {
		int num_ports = opx_hfi_get_num_ports(hfi_unit);
		for (int port = OPX_MIN_PORT; port <= num_ports; port++) {
			opx_lid_t lid = opx_hfi_get_port_lid(hfi_unit, port);
			if (lid > 0) {
				process_hfi_lookup(hfi_unit, lid);
			} else {
				FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA,
					"No LID found for HFI unit %d of %d units and port %d of %d ports: ret = %d, %s.\n",
					hfi_unit, hfi_units, port, num_ports, lid, strerror(errno));
			}
		}
	}
}

/* This function will define structures in shared memory setup by the driver when context sharing is in use. */
static void opx_define_ctx_sharing_shared_memory(struct fi_opx_hfi1_context *context)
{
	// Don't bother with setting up context sharing shared memory if it is not being used
	if (!OPX_IS_CTX_SHARING_ENABLED) {
		return;
	}

	uintptr_t    all_subcontext_uregbase = (uintptr_t) context->ctrl->base_info.subctxt_uregbase;
	unsigned int subctxt_cnt	     = context->subctxt_cnt;
	assert(all_subcontext_uregbase);

	/* Save a pointer to the base of each opx_subcontext_ureg structure. There can be up to
		HFI1_MAX_SHARED_CTXTS defined in subctxt_uregbase. */
	for (unsigned int i = 0; i < HFI1_MAX_SHARED_CTXTS; i++) {
		struct opx_subcontext_ureg *subcontext_ureg = (struct opx_subcontext_ureg *) all_subcontext_uregbase;

		/* Set unused subcontext_ureg entries to null if they aren't being used. */
		context->subcontext_ureg[i] = (i < subctxt_cnt) ? subcontext_ureg : NULL;

		// Move to next opx_subcontext_ureg.
		all_subcontext_uregbase += sizeof(struct opx_subcontext_ureg);

		FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA, "subcontext_ureg[%d] = %p\n", i,
		       context->subcontext_ureg[i]);
	}

	/* One hwcontext_ctrl defined for keeping track of the HFI RHQ state */
	context->hwcontext_ctrl = (struct opx_hwcontext_ctrl *) all_subcontext_uregbase;
	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA, "hwcontext_ctrl = %p\n", context->hwcontext_ctrl);

	/* Move to the PIO shared state structure*/
	all_subcontext_uregbase += sizeof(struct opx_hwcontext_ctrl);

	context->spio_ctrl = (struct opx_spio_ctrl *) all_subcontext_uregbase;
	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA, "spio_ctrl = %p\n", context->spio_ctrl);

	return;
}

/*
 * Open a context on the first HFI that shares our process' NUMA node.
 * If no HFI shares our NUMA node, grab the first active HFI.
 */
struct fi_opx_hfi1_context *fi_opx_hfi1_context_open(struct fid_ep *ep, uuid_t unique_job_key)
{
	struct fi_opx_ep *opx_ep		= (ep == NULL) ? NULL : container_of(ep, struct fi_opx_ep, ep_fid);
	int		  fd_cdev		= -1;
	int		  fd_verbs		= -1;
	int		  hfi_unit_number	= -6;
	int		  hfi_context_rank	= -1;
	int		  hfi_context_rank_inst = -1;
	const int	  numa_node_id		= opx_get_current_proc_location();
	const int	  core_id		= opx_get_current_proc_core();
	const int	  hfi_count		= opx_hfi_get_num_units();
	int		  hfi_candidates[OPX_MAX_HFIS];
	int		  hfi_distances[OPX_MAX_HFIS];
	int		  hfi_freectxs[OPX_MAX_HFIS];
	int		  hfi_candidates_count = 0;
	int		  hfi_candidate_index  = -1;
	struct _hfi_ctrl *ctrl		       = NULL;
	bool		  use_default_logic    = true;
	int		  dirfd		       = -1;

	memset(hfi_candidates, 0, sizeof(*hfi_candidates) * OPX_MAX_HFIS);
	memset(hfi_distances, 0, sizeof(*hfi_distances) * OPX_MAX_HFIS);
	memset(hfi_freectxs, 0, sizeof(*hfi_freectxs) * OPX_MAX_HFIS);

	struct fi_opx_hfi1_context_internal *internal = calloc(1, sizeof(struct fi_opx_hfi1_context_internal));
	if (!internal) {
		FI_WARN(&fi_opx_provider, FI_LOG_FABRIC,
			"Error: Memory allocation failure for fi_opx_hfi_context_internal.\n");
		return NULL;
	}

	struct fi_opx_hfi1_context *context = &internal->context;

	/*
	 * Force cpu affinity if desired. Normally you would let the
	 * job scheduler (such as mpirun) handle this.
	 */
	int force_cpuaffinity = 0;
	fi_param_get_bool(fi_opx_global.prov, "force_cpuaffinity", &force_cpuaffinity);
	if (force_cpuaffinity) {
		const int cpu_id = sched_getcpu();
		cpu_set_t cpuset;
		CPU_ZERO(&cpuset);
		CPU_SET(cpu_id, &cpuset);
		if (sched_setaffinity(0, sizeof(cpuset), &cpuset)) {
			FI_WARN(&fi_opx_provider, FI_LOG_FABRIC, "Unable to force cpu affinity. %s\n", strerror(errno));
		}
	}

	/*
	 * open the hfi1 context
	 */
	context->fd_cdev  = -1;
	context->fd_verbs = -1;
	internal->ctrl	  = NULL;

	// If FI_OPX_HFI_SELECT is specified, skip all this and
	// use its value as the selected hfi unit.
	char *env = NULL;
	if (FI_SUCCESS == fi_param_get_str(&fi_opx_provider, "hfi_select", &env)) {
		struct hfi_selector selector = {0};
		use_default_logic	     = false;

		int selectors, matched;
		selectors = matched = 0;
		const char *s;
		for (s = env; *s != '\0';) {
			s = hfi_selector_next(s, &selector);
			if (!s) {
				FI_WARN(&fi_opx_provider, FI_LOG_FABRIC,
					"Error occurred parsing HFI selector string \"%s\"\n", env);
				goto ctxt_open_err;
			}

			if (selector.type == HFI_SELECTOR_DEFAULT) {
				use_default_logic = true;
				break;
			}

			if (!opx_hfi_get_unit_active(selector.unit)) {
				FI_WARN(&fi_opx_provider, FI_LOG_FABRIC, "Error: selected unit %d is not active\n",
					selector.unit);
				goto ctxt_open_err;
			}

			if (selector.type == HFI_SELECTOR_FIXED) {
				hfi_unit_number = selector.unit;
				matched++;
				break;
			} else if (selector.type == HFI_SELECTOR_MAPBY) {
				if (selector.mapby.type == HFI_SELECTOR_MAPBY_NUMA) {
					int max_numa = numa_max_node();
					if (selector.mapby.rangeS > max_numa) {
						FI_WARN(&fi_opx_provider, FI_LOG_FABRIC,
							"Error: mapby numa %d > numa_max_node %d\n",
							selector.mapby.rangeS, max_numa);
						goto ctxt_open_err;
					}

					if (selector.mapby.rangeE > max_numa) {
						FI_WARN(&fi_opx_provider, FI_LOG_FABRIC,
							"mapby numa end of range %d > numa_max_node %d\n",
							selector.mapby.rangeE, max_numa);
						goto ctxt_open_err;
					}

					if (selector.mapby.rangeS <= numa_node_id &&
					    selector.mapby.rangeE >= numa_node_id) {
						hfi_unit_number = selector.unit;
						matched++;
						break;
					}
				} else if (selector.mapby.type == HFI_SELECTOR_MAPBY_CORE) {
					int max_core = get_nprocs();
					if (selector.mapby.rangeS > max_core) {
						FI_WARN(&fi_opx_provider, FI_LOG_FABRIC,
							"Error: mapby core %d > nprocs %d\n", selector.mapby.rangeS,
							max_core);
						goto ctxt_open_err;
					}
					if (selector.mapby.rangeE > max_core) {
						FI_WARN(&fi_opx_provider, FI_LOG_FABRIC,
							"mapby core end of range %d > nprocs %d\n",
							selector.mapby.rangeE, max_core);
						goto ctxt_open_err;
					}
					if (selector.mapby.rangeS <= core_id && selector.mapby.rangeE >= core_id) {
						hfi_unit_number = selector.unit;
						matched++;
						break;
					}
				} else {
					FI_WARN(&fi_opx_provider, FI_LOG_FABRIC, "Error: unsupported mapby type %d\n",
						selector.mapby.type);
					goto ctxt_open_err;
				}
			} else {
				FI_WARN(&fi_opx_provider, FI_LOG_FABRIC, "Error: unsupported selector type %d\n",
					selector.type);
				goto ctxt_open_err;
			}
			selectors++;
		}

		(void) selectors;

		if (!use_default_logic) {
			if (!matched) {
				FI_WARN(&fi_opx_provider, FI_LOG_FABRIC, "No HFI selectors matched.\n");
				goto ctxt_open_err;
			}

			hfi_candidates[0]    = hfi_unit_number;
			hfi_distances[0]     = 0;
			hfi_candidates_count = 1;
			FI_INFO(&fi_opx_provider, FI_LOG_FABRIC,
				"User-specified HFI selection set to %d. Skipping HFI selection algorithm \n",
				hfi_unit_number);

			if (opx_open_hfi_and_context(&ctrl, internal, unique_job_key, hfi_unit_number, &fd_cdev,
						     &fd_verbs) != 0) {
				FI_WARN(&fi_opx_provider, FI_LOG_FABRIC, "Unable to open user-specified HFI.\n");
				goto ctxt_open_err;
			}
			FI_INFO(&fi_opx_provider, FI_LOG_FABRIC, "Opened fd_cdev %d fd_verbs %d\n", fd_cdev, fd_verbs);
		}

	} else if (opx_ep && opx_ep->common_info->src_addr &&
		   ((union fi_opx_addr *) (opx_ep->common_info->src_addr))->hfi1_unit != opx_default_addr.hfi1_unit) {
		union fi_opx_addr addr;
		use_default_logic = false;
		/*
		 * DAOS Persistent Address Support:
		 * No Context Resource Management Framework is supported by OPX to enable
		 * acquiring a context with attributes that exactly match the specified
		 * source address.
		 *
		 * Therefore, treat the source address as an opaque ID and extract the
		 * essential data required to create a context that at least maps to the
		 * same HFI and HFI port (Note, the assigned LID is unchanged unless modified
		 * by the OPA FM).
		 */
		memset(&addr, 0, sizeof(addr));
		memcpy(&addr.fi, opx_ep->common_info->src_addr, opx_ep->common_info->src_addrlen);

		uint32_t uid = addr.lid << 8 | addr.hfi1_subctxt_rx;

		if (addr.lid != UINT32_MAX) {
			hfi_context_rank = uid;
		}
		hfi_unit_number	     = addr.hfi1_unit;
		hfi_candidates[0]    = hfi_unit_number;
		hfi_distances[0]     = 0;
		hfi_candidates_count = 1;

		if (hfi_context_rank != -1) {
			hfi_context_rank_inst = fi_opx_get_daos_hfi_rank_inst(hfi_unit_number, hfi_context_rank);

			FI_DBG_TRACE(
				&fi_opx_provider, FI_LOG_FABRIC,
				"Application-specified HFI selection set to %d rank %d.%d. Skipping HFI selection algorithm\n",
				hfi_unit_number, hfi_context_rank, hfi_context_rank_inst);
		} else {
			FI_DBG_TRACE(
				&fi_opx_provider, FI_LOG_FABRIC,
				"Application-specified HFI selection set to %d. Skipping HFI selection algorithm\n",
				hfi_unit_number);
		}

		if (opx_open_hfi_and_context(&ctrl, internal, unique_job_key, hfi_unit_number, &fd_cdev, &fd_verbs) !=
		    0) {
			FI_WARN(&fi_opx_provider, FI_LOG_FABRIC, "Unable to open application-specified HFI.\n");
			goto ctxt_open_err;
		}
		FI_INFO(&fi_opx_provider, FI_LOG_FABRIC, "Opened fd_cdev %d fd_verbs %d\n", fd_cdev, fd_verbs);
	}
	if (use_default_logic) {
		/* Select the best HFI to open a context on */
		FI_INFO(&fi_opx_provider, FI_LOG_FABRIC, "Found HFIs = %d\n", hfi_count);

		if (hfi_count == 0) {
			FI_WARN(&fi_opx_provider, FI_LOG_FABRIC, "FATAL: detected no HFIs, cannot continue\n");
			goto ctxt_open_err;
		} else if (hfi_count == 1) {
			if (opx_hfi_get_unit_active(0) > 0) {
				// Only 1 HFI, populate the candidate list and continue.
				FI_INFO(&fi_opx_provider, FI_LOG_FABRIC,
					"Detected one HFI and it has active ports, selected it\n");
				hfi_candidates[0]    = 0;
				hfi_distances[0]     = 0;
				hfi_candidates_count = 1;
			} else {
				// No active ports, we're done here.
				FI_WARN(&fi_opx_provider, FI_LOG_FABRIC,
					"FATAL: HFI has no active ports, cannot continue\n");
				goto ctxt_open_err;
			}

		} else {
			// Lock on the opx class directory path so that HFI selection based on distance and
			// number of free credits available is atomic. This is to avoid the situation where several
			// processes go to read the number of free contexts available in each HFI at the same time
			// and choose the same HFi with the smallest load as well as closest to the corresponding
			// process. If the processes of selection and then context openning is atomic here, this
			// situation is avoided and hfi selection should be evenly balanced.
			if ((dirfd = open(OPX_CLASS_DIR_PATH, O_RDONLY)) == -1) {
				FI_WARN(&fi_opx_provider, FI_LOG_FABRIC, "Failed to open %s: %s for flock use.\n",
					OPX_CLASS_DIR_PATH, strerror(errno));
				goto ctxt_open_err;
			}

			if (flock(dirfd, LOCK_EX) == -1) {
				FI_WARN(&fi_opx_provider, FI_LOG_FABRIC, "Flock exclusive lock failure: %s\n",
					strerror(errno));
				close(dirfd);
				goto ctxt_open_err;
			}

			// The system has multiple HFIs. Sort them by distance from
			// this process. HFIs with same distance are sorted by number of
			// free contexts available.
			int hfi_n, hfi_d, hfi_f;
			for (int i = 0; i < hfi_count; i++) {
				if (opx_hfi_get_unit_active(i) > 0) {
					hfi_n = opx_hfi_sysfs_unit_read_node_s64(i);
					hfi_d = numa_distance(hfi_n, numa_node_id);
					hfi_f = opx_hfi_get_num_free_contexts(i);
					FI_INFO(&fi_opx_provider, FI_LOG_FABRIC,
						"HFI unit %d in numa node %d has a distance of %d from this pid with"
						" %d free contexts available.\n",
						i, hfi_n, hfi_d, hfi_f);
					hfi_candidates[hfi_candidates_count] = i;
					hfi_distances[hfi_candidates_count]  = hfi_d;
					hfi_freectxs[hfi_candidates_count]   = hfi_f;
					int j				     = hfi_candidates_count;
					// Bubble the new HFI up till the list is sorted by distance
					// and then by number of free contexts. Yes, this is lame but
					// the practical matter is that there will never be so many HFIs
					// on a single system that a real insertion sort is justified.
					while (j > 0 && ((hfi_distances[j - 1] > hfi_distances[j]) ||
							 ((hfi_distances[j - 1] == hfi_distances[j]) &&
							  (hfi_freectxs[j - 1] < hfi_freectxs[j])))) {
						int t1		      = hfi_distances[j - 1];
						int t2		      = hfi_candidates[j - 1];
						int t3		      = hfi_freectxs[j - 1];
						hfi_distances[j - 1]  = hfi_distances[j];
						hfi_candidates[j - 1] = hfi_candidates[j];
						hfi_freectxs[j - 1]   = hfi_freectxs[j];
						hfi_distances[j]      = t1;
						hfi_candidates[j]     = t2;
						hfi_freectxs[j]	      = t3;
						j--;
					}
					hfi_candidates_count++;
				}
			}
		}

		// At this point we have a list of HFIs, sorted by distance from this pid (and by unit # as an implied
		// key). HFIs that have the same distance are sorted by number of free contexts available. Pick the
		// closest HFI that has the smallest load (largest number of free contexts). If we fail to open that
		// HFI, try another one at the same distance but potentially under a heavier load. If that fails, we
		// will try HFIs that are further away.
		int lower  = 0;
		int higher = 0;
		int context_open_failed;
		do {
			// Find the set of HFIs at this distance. Again, no attempt is
			// made to make this fast.
			higher = lower + 1;
			while (higher < hfi_candidates_count && hfi_distances[higher] == hfi_distances[lower]) {
				higher++;
			}

			// Select the hfi that is under the smallest load. All
			// hfis from [lower, higher) are sorted by number of free contexts
			// available with lower having the most contexts free.
			int range	    = higher - lower;
			hfi_candidate_index = lower;
			hfi_unit_number	    = hfi_candidates[hfi_candidate_index];

			context_open_failed = opx_open_hfi_and_context(&ctrl, internal, unique_job_key, hfi_unit_number,
								       &fd_cdev, &fd_verbs);
			int t		    = range;
			while (context_open_failed && t-- > 1) {
				hfi_candidate_index++;
				if (hfi_candidate_index >= higher) {
					hfi_candidate_index = lower;
				}
				hfi_unit_number	    = hfi_candidates[hfi_candidate_index];
				context_open_failed = opx_open_hfi_and_context(&ctrl, internal, unique_job_key,
									       hfi_unit_number, &fd_cdev, &fd_verbs);
			}
			FI_INFO(&fi_opx_provider, FI_LOG_FABRIC, "Opened fd_cdev %d fd_verbs %d\n", fd_cdev, fd_verbs);

			// If we still haven't successfully chosen an HFI,
			// try HFIs that are further away.
			lower = higher;
		} while (context_open_failed && lower < hfi_candidates_count);

		if (dirfd != -1) {
			if (flock(dirfd, LOCK_UN) == -1) {
				FI_WARN(&fi_opx_provider, FI_LOG_FABRIC, "Flock unlock failure: %s\n", strerror(errno));
				close(dirfd);

				if (!context_open_failed) {
					opx_hfi_context_close(fd_cdev, fd_verbs);
				}
				goto ctxt_open_err;
			}
			close(dirfd);
		}

		if (context_open_failed) {
			FI_WARN(&fi_opx_provider, FI_LOG_FABRIC,
				"FATAL: Found %d active HFI device%s, unable to open %s.\n", hfi_candidates_count,
				(hfi_candidates_count > 1) ? "s" : "",
				(hfi_candidates_count > 1) ? "any of them" : "it");
			goto ctxt_open_err;
		}
	}

	FI_INFO(&fi_opx_provider, FI_LOG_FABRIC,
		"Selected HFI is %d; caller NUMA domain is %d; HFI NUMA domain is %" PRId64 "\n", hfi_unit_number,
		numa_node_id, opx_hfi_sysfs_unit_read_node_s64(hfi_unit_number));

	// Alert user if the final choice wasn't optimal.
	if (opx_hfi_sysfs_unit_read_node_s64(hfi_unit_number) != numa_node_id) {
		if (hfi_count == 1) {
			/* No choice, not worth a warning */
			FI_INFO(&fi_opx_provider, FI_LOG_FABRIC,
				"Selected HFI is %d. It does not appear to be local to this pid's numa domain which is %d\n",
				hfi_unit_number, numa_node_id);
		} else {
			FI_WARN(&fi_opx_provider, FI_LOG_FABRIC,
				"Selected HFI is %d. It does not appear to be local to this pid's numa domain which is %d\n",
				hfi_unit_number, numa_node_id);
		}
	} else {
		FI_INFO(&fi_opx_provider, FI_LOG_FABRIC, "Selected HFI unit %d in the same numa node as this pid.\n",
			hfi_unit_number);
	}

	context->fd_cdev  = fd_cdev;
	context->fd_verbs = fd_verbs;
	internal->ctrl	  = ctrl; /* memory was allocated during opx_open_hfi_and_context() -> opx_hfi_userinit() */
	context->ctrl	  = ctrl; /* TODO? move required fields ctrl -> context? */

	opx_lid_t lid = 0;
	lid	      = opx_hfi_get_port_lid(ctrl->__hfi_unit, ctrl->__hfi_port);
	FI_DBG_TRACE(&fi_opx_provider, FI_LOG_FABRIC, "lid = %d ctrl->__hfi_unit %u, ctrl->__hfi_port %u\n", lid,
		     ctrl->__hfi_unit, ctrl->__hfi_port);

	assert(lid > 0);

	uint64_t gid_hi, gid_lo;
	int	 rc __attribute__((unused)) = -1;
	rc = opx_hfi_get_port_gid(ctrl->__hfi_unit, ctrl->__hfi_port, &gid_hi, &gid_lo);
	assert(rc != -1);

	context->hfi_unit	     = ctrl->__hfi_unit;
	context->hfi_port	     = ctrl->__hfi_port;
	context->lid		     = lid;
	context->gid_hi		     = gid_hi;
	context->gid_lo		     = gid_lo;
	context->daos_info.rank	     = hfi_context_rank;
	context->daos_info.rank_inst = hfi_context_rank_inst;

	// If a user wants an HPC job ran on a non-default Service Level,
	// they set FI_OPX_SL to the deseried SL with will then determine the SC and VL
	int user_sl = -1;
	if (fi_param_get_int(fi_opx_global.prov, "sl", &user_sl) == FI_SUCCESS) {
		if ((user_sl >= 0) && (user_sl <= 31)) {
			context->sl = user_sl;
			FI_INFO(&fi_opx_provider, FI_LOG_FABRIC,
				"Detected user specfied ENV FI_OPX_SL, so set the service level to %d\n", user_sl);
		} else {
			FI_WARN(&fi_opx_provider, FI_LOG_FABRIC,
				"Error: User specfied an env FI_OPX_SL.  Valid data is an positive integer 0 - 31 (Default is 0).  User specified %d.  Using default value of %d instead\n",
				user_sl, FI_OPX_HFI1_SL_DEFAULT);
			context->sl = FI_OPX_HFI1_SL_DEFAULT;
		}
	} else {
		context->sl = FI_OPX_HFI1_SL_DEFAULT;
	}

	rc = opx_hfi_get_port_sl2sc(ctrl->__hfi_unit, ctrl->__hfi_port, context->sl);
	if (rc < 0) {
		context->sc = FI_OPX_HFI1_SC_DEFAULT;
	} else {
		context->sc = rc;
	}

	rc = opx_hfi_get_port_sc2vl(ctrl->__hfi_unit, ctrl->__hfi_port, context->sc);
	if (rc < 0) {
		context->vl = FI_OPX_HFI1_VL_DEFAULT;
	} else {
		context->vl = rc;
	}

	if (context->sc == FI_OPX_HFI1_SC_ADMIN || context->vl == FI_OPX_HFI1_VL_ADMIN) {
		FI_WARN(&fi_opx_provider, FI_LOG_FABRIC,
			"Detected user set ENV FI_OPX_SL of %ld, which has translated to admin-level Service class (SC=%ld) and/or admin-level Virtual Lane(VL=%ld), which is invalid for user traffic.  Using default values instead\n",
			context->sl, context->sc, context->vl);
		context->sl = FI_OPX_HFI1_SL_DEFAULT;
		context->sc = FI_OPX_HFI1_SC_DEFAULT;
		context->vl = FI_OPX_HFI1_VL_DEFAULT;
	}

	if (context->vl > 7) {
		FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA,
			"VL is > 7, this may not be supported.  SL=%ld SC=%ld VL=%ld\n", context->sl, context->sc,
			context->vl);
	}

	context->mtu = opx_hfi_get_port_vl2mtu(ctrl->__hfi_unit, ctrl->__hfi_port, context->vl);
	assert(context->mtu >= 0);

	// If a user wants an HPC job ran on a non-default Partition key,
	// they set FI_OPX_PKEY env to specify it (Same behavior as PSM2_PKEY)
	int user_pkey = -1;
	if (fi_param_get_int(fi_opx_global.prov, "pkey", &user_pkey) == FI_SUCCESS) {
		if (user_pkey < 0) {
			FI_WARN(&fi_opx_provider, FI_LOG_FABRIC,
				"Detected user specified FI_OPX_PKEY of %d (0x%x), which is an invalid value.\n",
				user_pkey, user_pkey);
			if (fd_cdev >= 0) {
				opx_hfi_context_close(fd_cdev, fd_verbs);
			}
			goto ctxt_open_err;
		}
		rc = opx_hfi1_wrapper_set_pkey(internal, user_pkey);
		if (rc) {
			fprintf(stderr,
				"Detected user specified FI_OPX_PKEY of 0x%x, but got internal driver error on set. This pkey is likely not registered/valid.\n",
				user_pkey);
			if (fd_cdev >= 0) {
				opx_hfi_context_close(fd_cdev, fd_verbs);
			}
			goto ctxt_open_err;
		} else {
			context->pkey = user_pkey;
			FI_INFO(&fi_opx_provider, FI_LOG_FABRIC,
				"Detected user specfied ENV FI_OPX_PKEY, so set partition key to 0x%x\n", user_pkey);
		}
	} else {
		int default_pkey = opx_hfi_get_port_index2pkey(context->hfi_unit, context->hfi_port, 0);
		if (default_pkey < 0) {
			fprintf(stderr,
				"Unable to get default Pkey. Please specify a different Pkey using FI_OPX_PKEY\n");
			if (fd_cdev >= 0) {
				opx_hfi_context_close(fd_cdev, fd_verbs);
			}
			goto ctxt_open_err;
		}
		rc = opx_hfi1_wrapper_set_pkey(internal, default_pkey);
		if (rc) {
			fprintf(stderr,
				"Error in setting default Pkey %#x. Please specify a different Pkey using FI_OPX_PKEY\n",
				default_pkey);
			if (fd_cdev >= 0) {
				opx_hfi_context_close(fd_cdev, fd_verbs);
			}
			goto ctxt_open_err;
		} else {
			context->pkey = default_pkey;
		}
	}

	FI_INFO(&fi_opx_provider, FI_LOG_FABRIC, "Service Level: SL=%ld SC=%ld VL=%ld PKEY=0x%lx MTU=%d\n", context->sl,
		context->sc, context->vl, context->pkey, context->mtu);

	/*
	 * Initialize the hfi tx context
	 */

	const struct hfi1_base_info *base_info = &ctrl->base_info;
	const struct hfi1_ctxt_info *ctxt_info = &ctrl->ctxt_info;

	context->bthqp	   = (uint8_t) base_info->bthqp;
	context->jkey	   = base_info->jkey;
	context->send_ctxt = ctxt_info->send_ctxt;

	/*
	 * Initialize context sharing values
	 */
	context->subctxt_cnt		  = internal->user_info.subctxt_cnt;
	context->subctxt		  = ctxt_info->subctxt;
	fi_opx_global.ctx_sharing_enabled = context->subctxt_cnt > 0 ? OPX_CTX_SHARING_ON : OPX_CTX_SHARING_OFF;
	context->hfi1_frozen_count	  = 0;

	FI_DBG(&fi_opx_provider, FI_LOG_FABRIC, "subctxt %u, subctxt_cnt %u, subctxt_id %u\n", context->subctxt,
	       context->subctxt_cnt, internal->user_info.subctxt_id);

	// Define the layout of context sharing shared memory regions
	opx_define_ctx_sharing_shared_memory(context);

	OPX_OPEN_BAR(context->hfi_unit, ctrl->ctxt_info.ctxt, ctxt_info->send_ctxt);
	context->info.pio.scb_sop_first =
		OPX_HFI1_INIT_PIO_SOP(context->send_ctxt, (volatile uint64_t *) (ptrdiff_t) base_info->pio_bufbase_sop);
	context->info.pio.scb_first =
		OPX_HFI1_INIT_PIO(context->send_ctxt, (volatile uint64_t *) (ptrdiff_t) base_info->pio_bufbase);
	context->info.pio.credits_addr = (volatile uint64_t *) (ptrdiff_t) base_info->sc_credits_addr;
	context->credits_addr_copy     = context->info.pio.credits_addr;

	const uint64_t credit_return	       = *(context->info.pio.credits_addr);
	context->state.pio.free_counter_shadow = (uint16_t) (credit_return & 0x00000000000007FFul);
	context->state.pio.fill_counter	       = 0;
	context->state.pio.scb_head_index      = 0;
	context->state.pio.credits_total = ctxt_info->credits; /* yeah, yeah .. THIS field is static, but there was an
								  unused halfword at this spot, so .... */

	/* move to domain ? */
	uint8_t i;
	for (i = 0; i < 32; ++i) {
		rc = opx_hfi_get_port_sl2sc(ctrl->__hfi_unit, ctrl->__hfi_port, i);

		if (rc < 0) {
			context->sl2sc[i] = FI_OPX_HFI1_SC_DEFAULT;
		} else {
			context->sl2sc[i] = rc;
		}

		rc = opx_hfi_get_port_sc2vl(ctrl->__hfi_unit, ctrl->__hfi_port, i);
		if (rc < 0) {
			context->sc2vl[i] = FI_OPX_HFI1_VL_DEFAULT;
		}
		context->sc2vl[i] = rc;
	}

	// TODO: There is a bug in the driver that does not properly handle all
	//       queue entries in use at once. As a temporary workaround, pretend
	//       there is one less entry than there actually is.
	context->info.sdma.queue_size	     = ctxt_info->sdma_ring_size - 1;
	context->info.sdma.available_counter = context->info.sdma.queue_size;
	context->info.sdma.fill_index	     = 0;
	context->info.sdma.done_index	     = 0;
	context->info.sdma.completion_queue  = (struct hfi1_sdma_comp_entry *) base_info->sdma_comp_bufbase;
	assert(context->info.sdma.queue_size <= FI_OPX_HFI1_SDMA_MAX_COMP_INDEX);
	memset(context->info.sdma.queued_entries, 0, sizeof(context->info.sdma.queued_entries));

	/*
	 * initialize the hfi rx context
	 */

	context->info.rxe.id	       = ctrl->ctxt_info.ctxt;
	context->info.rxe.hdrq.rhf_off = (ctxt_info->rcvhdrq_entsize - 8) >> BYTE2DWORD_SHIFT;

	/* hardware registers */
	volatile uint64_t *uregbase =
		OPX_HFI1_INIT_UREGS(ctrl->ctxt_info.ctxt, (volatile uint64_t *) (uintptr_t) base_info->user_regbase);
	context->info.rxe.hdrq.head_register = (volatile uint64_t *) &uregbase[ur_rcvhdrhead];
	context->info.rxe.egrq.head_register = (volatile uint64_t *) &uregbase[ur_rcvegrindexhead];
	volatile uint64_t *tidflowtable	     = (volatile uint64_t *) &uregbase[ur_rcvtidflowtable];

#ifndef NDEBUG
	uint64_t debug_value = OPX_HFI1_BAR_UREG_LOAD(&uregbase[ur_rcvhdrtail]);
	FI_DBG(fi_opx_global.prov, FI_LOG_CORE, "&uregbase[ur_rcvhdrtail]       %p = %#16.16lX \n",
	       &uregbase[ur_rcvhdrtail], debug_value);
	debug_value = OPX_HFI1_BAR_UREG_LOAD(&uregbase[ur_rcvhdrhead]);
	FI_DBG(fi_opx_global.prov, FI_LOG_CORE, "&uregbase[ur_rcvhdrhead]       %p = %#16.16lX \n",
	       &uregbase[ur_rcvhdrhead], debug_value);
	debug_value = OPX_HFI1_BAR_UREG_LOAD(&uregbase[ur_rcvegrindextail]);
	FI_DBG(fi_opx_global.prov, FI_LOG_CORE, "&uregbase[ur_rcvegrindextail]  %p = %#16.16lX \n",
	       &uregbase[ur_rcvegrindextail], debug_value);
	debug_value = OPX_HFI1_BAR_UREG_LOAD(&uregbase[ur_rcvegrindexhead]);
	FI_DBG(fi_opx_global.prov, FI_LOG_CORE, "&uregbase[ur_rcvegrindexhead]  %p = %#16.16lX \n",
	       &uregbase[ur_rcvegrindexhead], debug_value);
	debug_value = OPX_HFI1_BAR_UREG_LOAD(&uregbase[ur_rcvegroffsettail]);
	FI_DBG(fi_opx_global.prov, FI_LOG_CORE, "&uregbase[ur_rcvegroffsettail] %p = %#16.16lX \n",
	       &uregbase[ur_rcvegroffsettail], debug_value);
	for (int i = 0; i < 32; ++i) {
		debug_value = OPX_HFI1_BAR_UREG_LOAD(&tidflowtable[i]);
		FI_DBG(fi_opx_global.prov, FI_LOG_CORE, "uregbase[ur_rcvtidflowtable][%u] = %#16.16lX \n", i,
		       debug_value);
	}
#endif
	/* TID flows aren't cleared between jobs, do it now. */
	for (int i = 0; i < 32; ++i) {
		OPX_HFI1_BAR_UREG_STORE(&tidflowtable[i], 0UL);
	}
	assert(ctrl->__hfi_tidexpcnt <= OPX_MAX_TID_COUNT);
	context->runtime_flags = ctxt_info->runtime_flags;

	/* OPX relies on RHF.SeqNum, not the RcvHdrTail */
	assert(!(context->runtime_flags & HFI1_CAP_DMA_RTAIL));

	context->info.rxe.hdrq.elemsz = ctxt_info->rcvhdrq_entsize >> BYTE2DWORD_SHIFT;
	if (context->info.rxe.hdrq.elemsz < FI_OPX_HFI1_HDRQ_ENTRY_SIZE_DWS_MIN) {
		FI_WARN(fi_opx_global.prov, FI_LOG_CORE, "Invalid hdrq_entsize %u (%lu is min)\n",
			context->info.rxe.hdrq.elemsz, FI_OPX_HFI1_HDRQ_ENTRY_SIZE_DWS_MIN);
		abort();
	}
	fi_opx_global.rcvhdrq_entry_dws = context->info.rxe.hdrq.elemsz;

	context->info.rxe.hdrq.elemcnt	    = ctxt_info->rcvhdrq_cnt;
	context->info.rxe.hdrq.elemlast	    = ((context->info.rxe.hdrq.elemcnt - 1) * context->info.rxe.hdrq.elemsz);
	context->info.rxe.hdrq.rx_poll_mask = fi_opx_hfi1_header_count_to_poll_mask(ctxt_info->rcvhdrq_cnt);
	context->info.rxe.hdrq.base_addr    = (uint32_t *) (uintptr_t) base_info->rcvhdr_bufbase;
	context->info.rxe.hdrq.rhf_base	    = context->info.rxe.hdrq.base_addr + context->info.rxe.hdrq.rhf_off;

	context->info.rxe.egrq.base_addr = (uint32_t *) (uintptr_t) base_info->rcvegr_bufbase;
	context->info.rxe.egrq.elemsz	 = ctxt_info->rcvegr_size;
	context->info.rxe.egrq.size	 = ctxt_info->rcvegr_size * ctxt_info->egrtids;

	fi_opx_ref_init(&context->ref_cnt, "HFI context");
	FI_INFO(&fi_opx_provider, FI_LOG_FABRIC, "Context configured with HFI=%d PORT=%d LID=0x%x JKEY=%d\n",
		context->hfi_unit, context->hfi_port, context->lid, context->jkey);

	context->status_lasterr		     = 0;
	context->status_check_next_usec	     = 0;
	context->link_down_wait_time_max_sec = OPX_LINK_DOWN_WAIT_TIME_MAX_SEC_DEFAULT;

	int link_down_wait_time_max_sec;
	if (fi_param_get_int(fi_opx_global.prov, "link_down_wait_time_max_sec", &link_down_wait_time_max_sec) ==
	    FI_SUCCESS) {
		if (link_down_wait_time_max_sec > 0) {
			context->link_down_wait_time_max_sec = link_down_wait_time_max_sec;
		} else {
			FI_WARN(&fi_opx_provider, FI_LOG_FABRIC,
				"Error: Invalid link_down_wait_time_max_sec value %d.  Using default value of %f instead\n",
				link_down_wait_time_max_sec, OPX_LINK_DOWN_WAIT_TIME_MAX_SEC_DEFAULT);
		}
	}

	opx_print_context(context);

	return context;

ctxt_open_err:
	free(internal);
	return NULL;
}

int init_hfi1_rxe_state(struct fi_opx_hfi1_context *context, struct fi_opx_hfi1_rxe_state *rxe_state)
{
	rxe_state->hdrq.head = 0;

	assert(!(context->runtime_flags & HFI1_CAP_DMA_RTAIL));
	rxe_state->hdrq.rhf_seq = OPX_RHF_SEQ_INIT_VAL(OPX_SW_HFI1_TYPE);
	/*  OPX relies on RHF.SeqNum, not the RcvHdrTail
		if (context->runtime_flags & HFI1_CAP_DMA_RTAIL) {
			rxe_state->hdrq.rhf_seq = 0;
		} else {
			rxe_state->hdrq.rhf_seq = OPX_WFR_RHF_SEQ_INIT_VAL;
		}
	*/
	return 0;
}

#include "rdma/opx/fi_opx_endpoint.h"
#include "rdma/opx/fi_opx_reliability.h"

ssize_t fi_opx_check_tx_rctxt(struct fi_opx_ep *opx_ep, fi_addr_t peer)
{
	ssize_t rc = FI_SUCCESS;

	assert(fi_opx_global.hfi_local_info.sriov);

	if ((opx_ep->tx->caps & FI_LOCAL_COMM) || ((opx_ep->tx->caps & (FI_LOCAL_COMM | FI_REMOTE_COMM)) == 0)) {
		const union fi_opx_addr addr = {.fi = peer};

		if ((addr.lid != fi_opx_global.hfi_local_info.lid)) {
			FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA, "multi-lid %u != %u, rx %u:%u, %u-%u\n",
			       fi_opx_global.hfi_local_info.lid, addr.lid,
			       OPX_HFI1_SUBCTXT(addr.hfi1_subctxt_rx, OPX_SW_HFI1_TYPE),
			       OPX_HFI1_RX(addr.hfi1_subctxt_rx, OPX_SW_HFI1_TYPE),
			       fi_opx_global.hfi_local_info.min_rctxt, fi_opx_global.hfi_local_info.max_rctxt);
			fi_opx_global.hfi_local_info.multi_lid = true;
		} else {
			if ((OPX_HFI1_RX(addr.hfi1_subctxt_rx, OPX_SW_HFI1_TYPE) >
			     fi_opx_global.hfi_local_info.max_rctxt) ||
			    (OPX_HFI1_RX(addr.hfi1_subctxt_rx, OPX_SW_HFI1_TYPE) <
			     fi_opx_global.hfi_local_info.min_rctxt)) {
				FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA, "multi-vm lid %u == %u, rx %u:%u, %u-%u\n",
				       fi_opx_global.hfi_local_info.lid, addr.lid,
				       OPX_HFI1_SUBCTXT(addr.hfi1_subctxt_rx, OPX_SW_HFI1_TYPE),
				       OPX_HFI1_RX(addr.hfi1_subctxt_rx, OPX_SW_HFI1_TYPE),
				       fi_opx_global.hfi_local_info.min_rctxt, fi_opx_global.hfi_local_info.max_rctxt);
				fi_opx_global.hfi_local_info.multi_vm = true;
				assert(fi_opx_global.hfi_local_info.sriov);
			}
		}
		/* JKR/sr-iov(alpha) limitation: Fail if self (rctxt check) is multi-VM and multi-lid */
		if ((OPX_HW_HFI1_TYPE & OPX_HFI1_JKR) && fi_opx_global.hfi_local_info.multi_vm &&
		    fi_opx_global.hfi_local_info.multi_lid) {
			FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA,
				"Shared lids across VMs with other lids are not supported on CN5000. \n");
			rc = -FI_EINVAL;
		}
	} else {
		__attribute__((__unused__)) const union fi_opx_addr addr = {.fi = peer};
		FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA,
		       "NO CONNECT: FI_LOCAL_COMM %llu, FI_REMOTE_COMM %llu, lid %u\n",
		       (opx_ep->tx->caps & FI_LOCAL_COMM), (opx_ep->tx->caps & FI_REMOTE_COMM), addr.lid);
	}

	return rc;
}

ssize_t fi_opx_hfi1_tx_connect(struct fi_opx_ep *opx_ep, fi_addr_t peer)
{
	ssize_t rc = FI_SUCCESS;

	if ((opx_ep->tx->caps & FI_LOCAL_COMM) || ((opx_ep->tx->caps & (FI_LOCAL_COMM | FI_REMOTE_COMM)) == 0)) {
		const union fi_opx_addr taddr = {.fi = peer};

		if (opx_lid_is_shm(taddr.lid)) {
			char		  buffer[128];
			union fi_opx_addr addr;
			addr.raw64b		 = (uint64_t) peer;
			uint8_t	 hfi_unit	 = addr.hfi1_unit;
			uint16_t hfi1_subctxt_rx = addr.hfi1_subctxt_rx;
			int	 inst		 = 0;
			uint32_t segment_index	 = OPX_SHM_SEGMENT_INDEX(hfi_unit, hfi1_subctxt_rx);

#ifdef OPX_DAOS
			/* HFI Rank Support:  Rank and PID included in the SHM file name */
			if (opx_ep->daos_info.hfi_rank_enabled) {
				rx_index = opx_shm_daos_rank_index(opx_ep->daos_info.rank, opx_ep->daos_info.rank_inst);
				inst	 = opx_ep->daos_info.rank_inst;
				segment_index = rx_index;
			}
#endif

			snprintf(buffer, sizeof(buffer), OPX_SHM_FILE_NAME_PREFIX_FORMAT,
				 opx_ep->domain->unique_job_key_str, hfi_unit, inst);

			rc = opx_shm_tx_connect(&opx_ep->tx->shm, (const char *const) buffer, segment_index,
						hfi1_subctxt_rx, FI_OPX_SHM_FIFO_SIZE, FI_OPX_SHM_PACKET_SIZE);
		}
	}

	return rc;
}

int opx_hfi1_rx_rzv_rts_send_cts_shm(union fi_opx_hfi1_deferred_work *work)
{
	struct fi_opx_hfi1_rx_rzv_rts_params *params	  = &work->rx_rzv_rts;
	struct fi_opx_ep		     *opx_ep	  = params->opx_ep;
	const uint64_t			      lrh_dlid_9B = params->lrh_dlid;
	const uint64_t bth_subctxt_rx			  = ((uint64_t) params->origin_rx) << OPX_BTH_SUBCTXT_RX_SHIFT;

	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA,
	       "===================================== RECV, SHM -- RENDEZVOUS RTS (begin)\n");
	OPX_TRACER_TRACE(OPX_TRACER_BEGIN, "RECV-RZV-RTS-SHM");
	uint64_t pos;
	/* Possible SHM connections required for certain applications (i.e., DAOS)
	 * exceeds the max value of the legacy origin_subctxt_rx field.  Use u32_extended field.
	 */
	ssize_t rc = fi_opx_shm_dynamic_tx_connect(OPX_SHM_TRUE, opx_ep, params->origin_rx, params->target_hfi_unit);

	if (OFI_UNLIKELY(rc)) {
		return -FI_EAGAIN;
	}

	union opx_hfi1_packet_hdr *const hdr = opx_shm_tx_next(
		&opx_ep->tx->shm, params->target_hfi_unit, params->origin_rx, &pos, opx_ep->daos_info.hfi_rank_enabled,
		params->u32_extended_rx, opx_ep->daos_info.rank_inst, &rc);

	if (!hdr) {
		return rc;
	}

	/* Note that we do not set stl.hdr.lrh.pktlen here (usually lrh_dws << 32),
	   because this is shm and since it's a CTS packet, lrh.pktlen
	   isn't used/needed */
	hdr->qw_9B[0] = opx_ep->rx->tx.cts_9B.hdr.qw_9B[0] | lrh_dlid_9B;
	hdr->qw_9B[1] = opx_ep->rx->tx.cts_9B.hdr.qw_9B[1] | bth_subctxt_rx;
	hdr->qw_9B[2] = opx_ep->rx->tx.cts_9B.hdr.qw_9B[2];

	hdr->qw_9B[3] = opx_ep->rx->tx.cts_9B.hdr.qw_9B[3];
	hdr->qw_9B[4] = opx_ep->rx->tx.cts_9B.hdr.qw_9B[4] | (params->niov << 48) | (params->opcode);
	hdr->qw_9B[5] = params->origin_byte_counter_vaddr;
	hdr->qw_9B[6] = (uint64_t) params->rzv_comp;

	union fi_opx_hfi1_packet_payload *const tx_payload = (union fi_opx_hfi1_packet_payload *) (hdr + 1);

	for (int i = 0; i < params->niov; i++) {
		tx_payload->cts.iov[i] = params->dput_iov[i];
	}

	opx_shm_tx_advance(&opx_ep->tx->shm, (void *) hdr, pos);

	OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "RECV-RZV-RTS-SHM");
	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA,
	       "===================================== RECV, SHM -- RENDEZVOUS RTS (end)\n");

	return FI_SUCCESS;
}

int opx_hfi1_rx_rzv_rts_send_cts_shm_16B(union fi_opx_hfi1_deferred_work *work)
{
	struct fi_opx_hfi1_rx_rzv_rts_params *params	   = &work->rx_rzv_rts;
	struct fi_opx_ep		     *opx_ep	   = params->opx_ep;
	const uint64_t			      lrh_dlid_16B = params->lrh_dlid;
	const uint64_t bth_subctxt_rx			   = ((uint64_t) params->origin_rx) << OPX_BTH_SUBCTXT_RX_SHIFT;

	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA,
	       "===================================== RECV 16B, SHM -- RENDEZVOUS RTS (begin)\n");
	OPX_TRACER_TRACE(OPX_TRACER_BEGIN, "RECV-RZV-RTS-SHM");
	uint64_t pos;
	/* Possible SHM connections required for certain applications (i.e., DAOS)
	 * exceeds the max value of the legacy origin_subctxt_rx field.  Use u32_extended field.
	 */
	ssize_t rc = fi_opx_shm_dynamic_tx_connect(OPX_SHM_TRUE, opx_ep, params->origin_rx, params->target_hfi_unit);

	if (OFI_UNLIKELY(rc)) {
		return -FI_EAGAIN;
	}

	union opx_hfi1_packet_hdr *const hdr = opx_shm_tx_next(
		&opx_ep->tx->shm, params->target_hfi_unit, params->origin_rx, &pos, opx_ep->daos_info.hfi_rank_enabled,
		params->u32_extended_rx, opx_ep->daos_info.rank_inst, &rc);

	if (!hdr) {
		return rc;
	}

	/* Note that we do not set stl.hdr.lrh.pktlen here (usually lrh_dws << 32),
	   because this is shm and since it's a CTS packet, lrh.pktlen
	   isn't used/needed */
	hdr->qw_16B[0] =
		opx_ep->rx->tx.cts_16B.hdr.qw_16B[0] |
		((uint64_t) ((lrh_dlid_16B & OPX_LRH_JKR_16B_DLID_MASK_16B) << OPX_LRH_JKR_16B_DLID_SHIFT_16B));
	hdr->qw_16B[1] =
		opx_ep->rx->tx.cts_16B.hdr.qw_16B[1] |
		((uint64_t) ((lrh_dlid_16B & OPX_LRH_JKR_16B_DLID20_MASK_16B) >> OPX_LRH_JKR_16B_DLID20_SHIFT_16B)) |
		(uint64_t) (bth_subctxt_rx >> OPX_LRH_JKR_BTH_RX_ENTROPY_SHIFT_16B);
	hdr->qw_16B[2] = opx_ep->rx->tx.cts_16B.hdr.qw_16B[2] | bth_subctxt_rx;
	hdr->qw_16B[3] = opx_ep->rx->tx.cts_16B.hdr.qw_16B[3];
	hdr->qw_16B[4] = opx_ep->rx->tx.cts_16B.hdr.qw_16B[4];
	hdr->qw_16B[5] = opx_ep->rx->tx.cts_16B.hdr.qw_16B[5] | (params->niov << 48) | (params->opcode);
	hdr->qw_16B[6] = params->origin_byte_counter_vaddr;
	hdr->qw_16B[7] = (uint64_t) params->rzv_comp;

	union fi_opx_hfi1_packet_payload *const tx_payload = (union fi_opx_hfi1_packet_payload *) (hdr + 1);

	for (int i = 0; i < params->niov; i++) {
		tx_payload->cts.iov[i] = params->dput_iov[i];
	}

	opx_shm_tx_advance(&opx_ep->tx->shm, (void *) hdr, pos);

	OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "RECV-RZV-RTS-SHM");
	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA,
	       "===================================== RECV 16B, SHM -- RENDEZVOUS RTS (end)\n");

	return FI_SUCCESS;
}

int opx_hfi1_rx_rzv_rts_send_cts(union fi_opx_hfi1_deferred_work *work)
{
	struct fi_opx_hfi1_rx_rzv_rts_params *params	  = &work->rx_rzv_rts;
	struct fi_opx_ep		     *opx_ep	  = params->opx_ep;
	const uint64_t			      lrh_dlid_9B = params->lrh_dlid;
	const uint64_t			      bth_rx	  = ((uint64_t) params->origin_rx) << OPX_BTH_SUBCTXT_RX_SHIFT;

	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA,
	       "===================================== RECV, HFI -- RENDEZVOUS %s RTS (begin) (params=%p rzv_comp=%p context=%p)\n",
	       params->tid_info.npairs ? "EXPECTED TID" : "EAGER", params, params->rzv_comp, params->rzv_comp->context);
	assert(params->rzv_comp->context->byte_counter >= params->dput_iov[0].bytes);
	OPX_TRACER_TRACE(OPX_TRACER_BEGIN, "SEND-RZV-CTS-HFI:%p", params->rzv_comp);
	const uint64_t tid_payload =
		params->tid_info.npairs ? ((params->tid_info.npairs + 4) * sizeof(params->tidpairs[0])) : 0;
	const uint64_t payload_bytes = (params->niov * sizeof(union opx_hfi1_dput_iov)) + tid_payload;
	const uint64_t pbc_dws	     = 2 + /* pbc */
				 2 +	   /* lrh */
				 3 +	   /* bth */
				 9 +	   /* kdeth; from "RcvHdrSize[i].HdrSize" CSR */
				 ((payload_bytes + 3) >> 2);
	const uint16_t lrh_dws = htons(
		pbc_dws - 2 + 1); /* (BE: LRH DW) does not include pbc (8 bytes), but does include icrc (4 bytes) */

	OPX_SHD_CTX_PIO_LOCK(OPX_IS_CTX_SHARING_ENABLED, opx_ep->tx);

	union fi_opx_hfi1_pio_state pio_state		 = *opx_ep->tx->pio_state;
	const uint16_t		    total_credits_needed = 1 +		   /* packet header */
					      ((payload_bytes + 63) >> 6); /* payload blocks needed */
	uint64_t total_credits_available =
		FI_OPX_HFI1_AVAILABLE_CREDITS(pio_state, &opx_ep->tx->force_credit_return, total_credits_needed);

	if (OFI_UNLIKELY(total_credits_available < total_credits_needed)) {
		fi_opx_compiler_msync_writes();
		FI_OPX_HFI1_UPDATE_CREDITS(pio_state, opx_ep->tx->pio_credits_addr);
		total_credits_available	   = FI_OPX_HFI1_AVAILABLE_CREDITS(pio_state, &opx_ep->tx->force_credit_return,
									   total_credits_needed);
		opx_ep->tx->pio_state->qw0 = pio_state.qw0;

		if (total_credits_available < total_credits_needed) {
			FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA,
			       "===================================== RECV, HFI -- RENDEZVOUS %s RTS (EAGAIN credits) (params=%p rzv_comp=%p context=%p)\n",
			       params->tid_info.npairs ? "EXPECTED TID" : "EAGER", params, params->rzv_comp,
			       params->rzv_comp->context);
			OPX_SHD_CTX_PIO_UNLOCK(OPX_IS_CTX_SHARING_ENABLED, opx_ep->tx);
			return -FI_EAGAIN;
		}
	}

	struct fi_opx_reliability_tx_replay *replay;
	union fi_opx_reliability_tx_psn	    *psn_ptr;
	int64_t				     psn;

	psn = fi_opx_reliability_get_replay(&opx_ep->ep_fid, opx_ep->reli_service, params->slid, params->origin_rx,
					    &psn_ptr, &replay, params->reliability, OPX_SW_HFI1_TYPE);
	if (OFI_UNLIKELY(psn == -1)) {
		FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA,
		       "===================================== RECV, HFI -- RENDEZVOUS %s RTS (EAGAIN psn/replay) (params=%p rzv_comp=%p context=%p)\n",
		       params->tid_info.npairs ? "EXPECTED TID" : "EAGER", params, params->rzv_comp,
		       params->rzv_comp->context);
		OPX_SHD_CTX_PIO_UNLOCK(OPX_IS_CTX_SHARING_ENABLED, opx_ep->tx);
		return -FI_EAGAIN;
	}

	assert(payload_bytes <= OPX_HFI1_PKT_SIZE);

	// The "memcopy first" code is here as an alternative to the more complicated
	// direct write to pio followed by memory copy of the reliability buffer
	replay->scb.scb_9B.qw0 = opx_ep->rx->tx.cts_9B.qw0 | OPX_PBC_LEN(pbc_dws, hfi1_type) | params->pbc_dlid |
				 OPX_PBC_LOOPBACK(params->pbc_dlid, OPX_SW_HFI1_TYPE);
	replay->scb.scb_9B.hdr.qw_9B[0] = opx_ep->rx->tx.cts_9B.hdr.qw_9B[0] | lrh_dlid_9B | ((uint64_t) lrh_dws << 32);
	replay->scb.scb_9B.hdr.qw_9B[1] = opx_ep->rx->tx.cts_9B.hdr.qw_9B[1] | bth_rx;
	replay->scb.scb_9B.hdr.qw_9B[2] = opx_ep->rx->tx.cts_9B.hdr.qw_9B[2] | psn;
	replay->scb.scb_9B.hdr.qw_9B[3] = opx_ep->rx->tx.cts_9B.hdr.qw_9B[3];
	replay->scb.scb_9B.hdr.qw_9B[4] = opx_ep->rx->tx.cts_9B.hdr.qw_9B[4] |
					  ((uint64_t) params->tid_info.npairs << 32) | (params->niov << 48) |
					  (params->opcode);
	replay->scb.scb_9B.hdr.qw_9B[5] = params->origin_byte_counter_vaddr;
	replay->scb.scb_9B.hdr.qw_9B[6] = (uint64_t) params->rzv_comp;
	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA,
	       "replay->scb_9B.qw0 = %#lx pbc_dws = %ld lrh_dws = %d replay->scb_9B.hdr.lrh_9B.pktlen = %d\n",
	       replay->scb.scb_9B.qw0, pbc_dws, lrh_dws, replay->scb.scb_9B.hdr.lrh_9B.pktlen);

	union fi_opx_hfi1_packet_payload *const tx_payload = (union fi_opx_hfi1_packet_payload *) replay->payload;
	assert(((uint8_t *) tx_payload) == ((uint8_t *) &replay->data));

	for (int i = 0; i < params->niov; i++) {
		tx_payload->cts.iov[i] = params->dput_iov[i];
	}

	/* copy tidpairs to packet */
	if (params->tid_info.npairs) {
		assert(params->tid_info.npairs < FI_OPX_MAX_DPUT_TIDPAIRS);
		assert(params->tidpairs[0] != 0);
		assert(params->niov == 1);
		assert(params->rzv_comp->context->byte_counter >= params->dput_iov[0].bytes);

		/* coverity[missing_lock] */
		tx_payload->tid_cts.tid_offset		       = params->tid_info.offset;
		tx_payload->tid_cts.ntidpairs		       = params->tid_info.npairs;
		tx_payload->tid_cts.origin_byte_counter_adjust = params->tid_info.origin_byte_counter_adj;
		for (int i = 0; i < params->tid_info.npairs; ++i) {
			tx_payload->tid_cts.tidpairs[i] = params->tidpairs[i];
		}
	}

	fi_opx_reliability_service_do_replay(opx_ep, opx_ep->reli_service, replay);

	OPX_SHD_CTX_PIO_UNLOCK(OPX_IS_CTX_SHARING_ENABLED, opx_ep->tx);

	fi_opx_reliability_service_replay_register_no_update(opx_ep->reli_service, psn_ptr, replay, params->reliability,
							     OPX_SW_HFI1_TYPE);
	OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "SEND-RZV-CTS-HFI:%p", params->rzv_comp);
	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA,
	       "===================================== RECV, HFI -- RENDEZVOUS %s RTS (end) (params=%p rzv_comp=%p context=%p)\n",
	       params->tid_info.npairs ? "EXPECTED TID" : "EAGER", params, params->rzv_comp, params->rzv_comp->context);
	return FI_SUCCESS;
}

int opx_hfi1_rx_rzv_rts_send_cts_16B(union fi_opx_hfi1_deferred_work *work)
{
	struct fi_opx_hfi1_rx_rzv_rts_params *params	   = &work->rx_rzv_rts;
	struct fi_opx_ep		     *opx_ep	   = params->opx_ep;
	const uint64_t			      lrh_dlid_16B = params->lrh_dlid;
	const uint64_t			      bth_rx	   = ((uint64_t) params->origin_rx) << OPX_BTH_SUBCTXT_RX_SHIFT;

	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA,
	       "===================================== RECV 16B, HFI -- RENDEZVOUS %s RTS (begin) (params=%p rzv_comp=%p context=%p)\n",
	       params->tid_info.npairs ? "EXPECTED TID" : "EAGER", params, params->rzv_comp, params->rzv_comp->context);
	assert(params->rzv_comp->context->byte_counter >= params->dput_iov[0].bytes);
	OPX_TRACER_TRACE(OPX_TRACER_BEGIN, "SEND-RZV-CTS-HFI:%p", params->rzv_comp);
	const uint64_t tid_payload =
		params->tid_info.npairs ? ((params->tid_info.npairs + 4) * sizeof(params->tidpairs[0])) : 0;
	const uint64_t payload_bytes = (params->niov * sizeof(union opx_hfi1_dput_iov)) + tid_payload;
	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA, "payload_bytes = %ld\n", payload_bytes);
	const uint64_t pbc_dws = 2 +				     /* pbc */
				 4 +				     /* lrh uncompressed */
				 3 +				     /* bth */
				 9 +				     /* kdeth; from "RcvHdrSize[i].HdrSize" CSR */
				 (((payload_bytes + 7) & -8) >> 2) + /* 16B is QW length/padded */
				 2;				     /* ICRC/tail */
	const uint16_t lrh_qws = (pbc_dws - 2) >> 1;		     /* (LRH QW) does not include pbc (8 bytes) */

	OPX_SHD_CTX_PIO_LOCK(OPX_IS_CTX_SHARING_ENABLED, opx_ep->tx);
	union fi_opx_hfi1_pio_state pio_state		 = *opx_ep->tx->pio_state;
	const uint16_t		    total_credits_needed = 2 +		   /* packet header */
					      ((payload_bytes + 63) >> 6); /* payload blocks needed */
	uint64_t total_credits_available =
		FI_OPX_HFI1_AVAILABLE_CREDITS(pio_state, &opx_ep->tx->force_credit_return, total_credits_needed);

	if (OFI_UNLIKELY(total_credits_available < total_credits_needed)) {
		fi_opx_compiler_msync_writes();
		FI_OPX_HFI1_UPDATE_CREDITS(pio_state, opx_ep->tx->pio_credits_addr);
		total_credits_available	   = FI_OPX_HFI1_AVAILABLE_CREDITS(pio_state, &opx_ep->tx->force_credit_return,
									   total_credits_needed);
		opx_ep->tx->pio_state->qw0 = pio_state.qw0;

		if (total_credits_available < total_credits_needed) {
			FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA,
			       "===================================== RECV, HFI -- RENDEZVOUS %s RTS (EAGAIN credits) (params=%p rzv_comp=%p context=%p)\n",
			       params->tid_info.npairs ? "EXPECTED TID" : "EAGER", params, params->rzv_comp,
			       params->rzv_comp->context);
			OPX_SHD_CTX_PIO_UNLOCK(OPX_IS_CTX_SHARING_ENABLED, opx_ep->tx);
			return -FI_EAGAIN;
		}
	}

	struct fi_opx_reliability_tx_replay *replay;
	union fi_opx_reliability_tx_psn	    *psn_ptr;
	int64_t				     psn;

	psn = fi_opx_reliability_get_replay(&opx_ep->ep_fid, opx_ep->reli_service, params->slid, params->origin_rx,
					    &psn_ptr, &replay, params->reliability, OPX_SW_HFI1_TYPE);
	if (OFI_UNLIKELY(psn == -1)) {
		FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA,
		       "===================================== RECV, HFI -- RENDEZVOUS %s RTS (EAGAIN psn/replay) (params=%p rzv_comp=%p context=%p)\n",
		       params->tid_info.npairs ? "EXPECTED TID" : "EAGER", params, params->rzv_comp,
		       params->rzv_comp->context);
		OPX_SHD_CTX_PIO_UNLOCK(OPX_IS_CTX_SHARING_ENABLED, opx_ep->tx);
		return -FI_EAGAIN;
	}

	assert(payload_bytes <= OPX_HFI1_PKT_SIZE);

	// The "memcopy first" code is here as an alternative to the more complicated
	// direct write to pio followed by memory copy of the reliability buffer
	assert(~(OPX_SW_HFI1_TYPE & OPX_HFI1_WFR));
	replay->scb.scb_16B.qw0 = opx_ep->rx->tx.cts_16B.qw0 | OPX_PBC_LEN(pbc_dws, OPX_HFI1_JKR) | params->pbc_dlid |
				  OPX_PBC_LOOPBACK(params->pbc_dlid, OPX_HFI1_JKR);
	replay->scb.scb_16B.hdr.qw_16B[0] =
		opx_ep->rx->tx.cts_16B.hdr.qw_16B[0] |
		((uint64_t) (lrh_dlid_16B & OPX_LRH_JKR_16B_DLID_MASK_16B) << OPX_LRH_JKR_16B_DLID_SHIFT_16B) |
		((uint64_t) lrh_qws << 20);
	replay->scb.scb_16B.hdr.qw_16B[1] =
		opx_ep->rx->tx.cts_16B.hdr.qw_16B[1] |
		((uint64_t) ((lrh_dlid_16B & OPX_LRH_JKR_16B_DLID20_MASK_16B) >> OPX_LRH_JKR_16B_DLID20_SHIFT_16B)) |
		(uint64_t) (bth_rx >> OPX_LRH_JKR_BTH_RX_ENTROPY_SHIFT_16B);
	replay->scb.scb_16B.hdr.qw_16B[2] = opx_ep->rx->tx.cts_16B.hdr.qw_16B[2] | bth_rx;
	replay->scb.scb_16B.hdr.qw_16B[3] = opx_ep->rx->tx.cts_16B.hdr.qw_16B[3] | psn;
	replay->scb.scb_16B.hdr.qw_16B[4] = opx_ep->rx->tx.cts_16B.hdr.qw_16B[4];
	replay->scb.scb_16B.hdr.qw_16B[5] = opx_ep->rx->tx.cts_16B.hdr.qw_16B[5] |
					    ((uint64_t) params->tid_info.npairs << 32) | (params->niov << 48) |
					    params->opcode;
	replay->scb.scb_16B.hdr.qw_16B[6] = params->origin_byte_counter_vaddr;

	replay->scb.scb_16B.hdr.qw_16B[7] = (uint64_t) params->rzv_comp;
	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA,
	       "replay->scb_16B.qw0 = %#lx pbc_dws = %ld lrh_qws = %d replay->scb_16B.hdr.lrh_16B.pktlen = %d\n",
	       replay->scb.scb_16B.qw0, pbc_dws, lrh_qws, replay->scb.scb_16B.hdr.lrh_16B.pktlen);

	union fi_opx_hfi1_packet_payload *const tx_payload = (union fi_opx_hfi1_packet_payload *) (replay->payload);

	assert(((uint8_t *) tx_payload) == ((uint8_t *) &(replay->data)));

	for (int i = 0; i < params->niov; i++) {
		tx_payload->cts.iov[i] = params->dput_iov[i];
	}

	/* copy tidpairs to packet */
	if (params->tid_info.npairs) {
		assert(params->tid_info.npairs < FI_OPX_MAX_DPUT_TIDPAIRS);
		assert(params->tidpairs[0] != 0);
		assert(params->niov == 1);
		assert(params->rzv_comp->context->byte_counter >= params->dput_iov[0].bytes);

		/* coverity[missing_lock] */
		tx_payload->tid_cts.tid_offset		       = params->tid_info.offset;
		tx_payload->tid_cts.ntidpairs		       = params->tid_info.npairs;
		tx_payload->tid_cts.origin_byte_counter_adjust = params->tid_info.origin_byte_counter_adj;
		for (int i = 0; i < params->tid_info.npairs; ++i) {
			tx_payload->tid_cts.tidpairs[i] = params->tidpairs[i];
		}
	}
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		     "fi_opx_reliability_service_do_replay opx_ep->reli_service %p, replay %p\n", opx_ep->reli_service,
		     replay);
	fi_opx_reliability_service_do_replay(opx_ep, opx_ep->reli_service, replay);

	OPX_SHD_CTX_PIO_UNLOCK(OPX_IS_CTX_SHARING_ENABLED, opx_ep->tx);

	fi_opx_reliability_service_replay_register_no_update(opx_ep->reli_service, psn_ptr, replay, params->reliability,
							     OPX_HFI1_JKR);
	OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "SEND-RZV-CTS-HFI:%p", params->rzv_comp);
	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA,
	       "===================================== RECV, HFI -- RENDEZVOUS %s RTS (end) (params=%p rzv_comp=%p context=%p)\n",
	       params->tid_info.npairs ? "EXPECTED TID" : "EAGER", params, params->rzv_comp, params->rzv_comp->context);
	return FI_SUCCESS;
}

__OPX_FORCE_INLINE__
int opx_hfi1_rx_rzv_rts_tid_eligible(struct fi_opx_ep *opx_ep, struct fi_opx_hfi1_rx_rzv_rts_params *params,
				     const uint64_t niov, const uint64_t immediate_data, const uint64_t immediate_tail,
				     const uint64_t is_hmem, const uint64_t is_hmem_unified, const uint64_t is_shm,
				     const enum fi_hmem_iface iface, uint8_t opcode)
{
	if (is_shm || !opx_ep->use_expected_tid_rzv || (niov != 1) ||
	    (params->dput_iov[0].bytes < opx_ep->tx->tid_min_payload_bytes) ||
	    (opcode != FI_OPX_HFI_DPUT_OPCODE_RZV && opcode != FI_OPX_HFI_DPUT_OPCODE_RZV_NONCONTIG) ||
	    is_hmem_unified ||
	    !fi_opx_hfi1_sdma_use_sdma(opx_ep, params->dput_iov[0].bytes, opcode, is_hmem, OPX_SHM_FALSE)) {
		FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.expected_receive.rts_tid_ineligible);
		return 0;
	}

#ifndef NDEBUG
	const uintptr_t rbuf_end = params->dst_vaddr + params->dput_iov[0].bytes;
#endif

	/* Caller adjusted pointers and lengths past the immediate data.
	 * Now align the destination buffer to be page aligned for expected TID writes
	 * This should point/overlap into the immediate data area.
	 * Then realign source buffer and lengths appropriately.
	 */
	/* TID writes must start on 64 byte boundaries */
	uintptr_t vaddr_aligned64 = params->dst_vaddr & -64;

	int64_t byte_counter_adjust;

	if (vaddr_aligned64 >= (params->dst_vaddr - immediate_data)) {
		size_t alignment_adjustment = params->dst_vaddr - vaddr_aligned64;
		params->dst_vaddr -= alignment_adjustment;
		params->dput_iov[0].rbuf -= alignment_adjustment;
		params->dput_iov[0].sbuf -= alignment_adjustment;
		params->dput_iov[0].bytes += alignment_adjustment;

		byte_counter_adjust = alignment_adjustment;

		params->elided_head.bytes = 0;
	} else {
		// Round up to next 64-byte boundary.
		vaddr_aligned64 = (params->dst_vaddr + 63) & -64;

		// If params->dst_vaddr is already on a 64-byte boundary, then
		// adding 63 to it and ANDing that with -64 would result in the
		// same address. *But* in that situation, we should not have
		// taken this else branch, so rounding up to the next boundary
		// should definitely result in vaddr being > params->dst_vaddr
		assert(vaddr_aligned64 > params->dst_vaddr);

		// Get the portion of bytes at the start of the buffer that
		// we'll need to send a separate CTS for, and then adjust the
		// original buffers
		params->elided_head.bytes	= vaddr_aligned64 - params->dst_vaddr;
		params->elided_head.rbuf	= params->dst_vaddr;
		params->elided_head.rbuf_iface	= params->dput_iov[0].rbuf_iface;
		params->elided_head.rbuf_device = params->dput_iov[0].rbuf_device;
		params->elided_head.sbuf	= params->dput_iov[0].sbuf;
		params->elided_head.sbuf_iface	= params->dput_iov[0].sbuf_iface;
		params->elided_head.sbuf_device = params->dput_iov[0].sbuf_device;
		params->elided_head.sbuf_handle = params->dput_iov[0].sbuf_handle;

		params->dst_vaddr	 = vaddr_aligned64;
		params->dput_iov[0].rbuf = vaddr_aligned64;
		params->dput_iov[0].sbuf += params->elided_head.bytes;
		params->dput_iov[0].bytes -= params->elided_head.bytes;

		// No byte counter adjustment necessary because we didn't
		// overlap with immediate data so we aren't requesting bytes
		// to be sent that were already sent.
		byte_counter_adjust = 0;
	}

	// Make sure that our buffer still ends in the same place, even after
	// adjusting the start to be cacheline-aligned
	assert((params->dst_vaddr + params->dput_iov[0].bytes) == rbuf_end);

	/* We need to ensure the length is a qw multiple. If a shorter length
	   is needed, and no immediate tail data was sent, we'll need to get
	   the elided tail data via separate CTS packet */
	const size_t elided_tail_bytes = params->dput_iov[0].bytes & 7;
	const size_t qw_floor_length   = params->dput_iov[0].bytes & -8;
	if (elided_tail_bytes && !immediate_tail) {
		params->elided_tail.bytes	= elided_tail_bytes;
		params->elided_tail.rbuf	= params->dput_iov[0].rbuf + qw_floor_length;
		params->elided_tail.sbuf	= params->dput_iov[0].sbuf + qw_floor_length;
		params->elided_tail.rbuf_iface	= params->dput_iov[0].rbuf_iface;
		params->elided_tail.rbuf_device = params->dput_iov[0].rbuf_device;
		params->elided_tail.sbuf_iface	= params->dput_iov[0].sbuf_iface;
		params->elided_tail.sbuf_device = params->dput_iov[0].sbuf_device;
		params->elided_tail.sbuf_handle = params->dput_iov[0].sbuf_handle;
	} else {
		// If elided_tail_bytes was non-zero, then it must be the case
		// that we had immediate_tail data and don't need to request those
		// bytes to be sent via separate CTS packet. But we still do need
		// to subtract them from the byte counter.
		byte_counter_adjust -= elided_tail_bytes;
		params->elided_tail.bytes = 0;
	}

	params->dput_iov[0].bytes = qw_floor_length;
	params->rzv_comp->context->byte_counter += byte_counter_adjust;
	params->tid_info.origin_byte_counter_adj = (int32_t) byte_counter_adjust;

	FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.expected_receive.rts_tid_eligible);

	return 1;
}

__OPX_FORCE_INLINE__
union fi_opx_hfi1_deferred_work *opx_hfi1_rx_rzv_rts_tid_prep_cts(union fi_opx_hfi1_deferred_work      *work,
								  struct fi_opx_hfi1_rx_rzv_rts_params *params,
								  const struct opx_tid_addr_block      *tid_addr_block,
								  const size_t cur_addr_range_tid_len,
								  const bool   last_cts)
{
	union fi_opx_hfi1_deferred_work	     *cts_work;
	struct fi_opx_hfi1_rx_rzv_rts_params *cts_params;

	// If this will not be the last CTS we send, allocate a new deferred
	// work item and rzv completion to use for the CTS, and copy the first
	// portion of the current work item into it. If this will be the last
	// CTS, we'll just use the existing deferred work item and rzv completion
	if (!last_cts) {
		cts_work = ofi_buf_alloc(params->opx_ep->tx->work_pending_pool);
		if (OFI_UNLIKELY(cts_work == NULL)) {
			FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA, "Failed to allocate deferred work item!\n");
			return NULL;
		}
		struct fi_opx_rzv_completion *rzv_comp = ofi_buf_alloc(params->opx_ep->rzv_completion_pool);
		if (OFI_UNLIKELY(rzv_comp == NULL)) {
			FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA, "Failed to allocate rendezvous completion item!\n");
			OPX_BUF_FREE(cts_work);
			return NULL;
		}

		// Add 1 to the offset so we end up with a cacheline multiple length
		const size_t copy_length = offsetof(struct fi_opx_hfi1_rx_rzv_rts_params, multi_cts_copy_boundary) + 1;
		assert(copy_length < sizeof(*work));
		memcpy(cts_work, work, copy_length);

		cts_work->work_elem.slist_entry.next = NULL;
		cts_params			     = &cts_work->rx_rzv_rts;
		cts_params->rzv_comp		     = rzv_comp;
		cts_params->rzv_comp->context	     = params->rzv_comp->context;
	} else {
		cts_work   = work;
		cts_params = params;
	}

	// Calculate the offset of the target buffer relative to the
	// original target buffer address, and then use that to set
	// the address for the source buffer
	size_t	  target_offset	      = params->tid_info.cur_addr_range.buf - params->dput_iov[params->cur_iov].rbuf;
	uintptr_t adjusted_source_buf = params->dput_iov[params->cur_iov].sbuf + target_offset;

	cts_params->niov		    = 1;
	cts_params->dput_iov[0].rbuf_iface  = params->dput_iov[params->cur_iov].rbuf_iface;
	cts_params->dput_iov[0].rbuf_device = params->dput_iov[params->cur_iov].rbuf_device;
	cts_params->dput_iov[0].sbuf_iface  = params->dput_iov[params->cur_iov].sbuf_iface;
	cts_params->dput_iov[0].sbuf_device = params->dput_iov[params->cur_iov].sbuf_device;
	cts_params->dput_iov[0].sbuf_handle = params->dput_iov[params->cur_iov].sbuf_handle;
	cts_params->dput_iov[0].rbuf	    = params->tid_info.cur_addr_range.buf;
	cts_params->dput_iov[0].sbuf	    = adjusted_source_buf;
	cts_params->dput_iov[0].bytes	    = cur_addr_range_tid_len;
	cts_params->dst_vaddr		    = params->tid_info.cur_addr_range.buf;

	cts_params->rzv_comp->tid_vaddr		= params->tid_info.cur_addr_range.buf;
	cts_params->rzv_comp->tid_length	= cur_addr_range_tid_len;
	cts_params->rzv_comp->byte_counter	= cur_addr_range_tid_len;
	cts_params->rzv_comp->bytes_accumulated = 0;

	cts_params->tid_info.npairs		     = tid_addr_block->npairs;
	cts_params->tid_info.offset		     = tid_addr_block->offset;
	cts_params->tid_info.origin_byte_counter_adj = params->tid_info.origin_byte_counter_adj;

	assert(cur_addr_range_tid_len <= cts_params->rzv_comp->context->byte_counter);
	assert(tid_addr_block->npairs < FI_OPX_MAX_DPUT_TIDPAIRS);
	for (int i = 0; i < tid_addr_block->npairs; i++) {
		cts_params->tidpairs[i] = tid_addr_block->pairs[i];
	}

	assert(cur_addr_range_tid_len <= cts_params->rzv_comp->context->byte_counter);

	if (OPX_SW_HFI1_TYPE & (OPX_HFI1_WFR | OPX_HFI1_MIXED_9B)) {
		cts_params->work_elem.work_fn = opx_hfi1_rx_rzv_rts_send_cts;
	} else {
		cts_params->work_elem.work_fn = opx_hfi1_rx_rzv_rts_send_cts_16B;
	}
	cts_params->work_elem.work_type = OPX_WORK_TYPE_PIO;

	return cts_work;
}

__OPX_FORCE_INLINE__
int opx_hfi1_rx_rzv_rts_tid_fallback(union fi_opx_hfi1_deferred_work	  *work,
				     struct fi_opx_hfi1_rx_rzv_rts_params *params)
{
	/* Since we may have already sent one or more CTS packets covering
	   some portion of the receive range using TID, we now need to
	   adjust the buf pointers and length in the dput_iov we were
	   working on to reflect only the unsent portion */
	assert(params->tid_info.cur_addr_range.buf >= ((uintptr_t) params->dput_iov[params->cur_iov].rbuf));
	size_t bytes_already_sent =
		params->tid_info.cur_addr_range.buf - ((uintptr_t) params->dput_iov[params->cur_iov].rbuf);
	assert(bytes_already_sent < params->dput_iov[params->cur_iov].bytes);

	params->dput_iov[params->cur_iov].rbuf = params->tid_info.cur_addr_range.buf;
	params->dput_iov[params->cur_iov].sbuf += bytes_already_sent;
	params->dput_iov[params->cur_iov].bytes -= bytes_already_sent;
	params->dst_vaddr = params->dput_iov[params->cur_iov].rbuf;

	params->tid_info.npairs = 0;

	if (OPX_SW_HFI1_TYPE & (OPX_HFI1_WFR | OPX_HFI1_MIXED_9B)) {
		params->work_elem.work_fn = opx_hfi1_rx_rzv_rts_send_cts;
	} else {
		params->work_elem.work_fn = opx_hfi1_rx_rzv_rts_send_cts_16B;
	}
	params->work_elem.work_type = OPX_WORK_TYPE_PIO;
	params->opcode		    = FI_OPX_HFI_DPUT_OPCODE_RZV;

	FI_OPX_DEBUG_COUNTERS_INC(params->opx_ep->debug_counters.expected_receive.rts_fallback_eager_reg_rzv);
	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA,
	       "===================================== RECV, HFI -- RENDEZVOUS RTS TID SETUP (end) EPERM, switching to non-TID send CTS (params=%p rzv_comp=%p context=%p)\n",
	       params, params->rzv_comp, params->rzv_comp->context);

	return params->work_elem.work_fn(work);
}

int opx_hfi1_rx_rzv_rts_tid_setup(union fi_opx_hfi1_deferred_work *work)
{
	struct fi_opx_hfi1_rx_rzv_rts_params *params = &work->rx_rzv_rts;

	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA,
	       "===================================== RECV, HFI -- RENDEZVOUS RTS TID SETUP (begin) (params=%p rzv_comp=%p context=%p)\n",
	       params, params->rzv_comp, params->rzv_comp->context);

	struct opx_tid_addr_block tid_addr_block = {};

	int register_rc = opx_register_for_rzv(params->opx_ep, &params->tid_info.cur_addr_range, &tid_addr_block);

	/* TID has been disabled for this endpoint, fall back to rendezvous */
	if (OFI_UNLIKELY(register_rc == -FI_EPERM)) {
		return opx_hfi1_rx_rzv_rts_tid_fallback(work, params);
	} else if (register_rc != FI_SUCCESS) {
		assert(register_rc == -FI_EAGAIN);
		FI_OPX_DEBUG_COUNTERS_INC(params->opx_ep->debug_counters.expected_receive.rts_tid_setup_retries);
		FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA,
		       "===================================== RECV, HFI -- RENDEZVOUS RTS TID SETUP (end) EAGAIN (No Progress) (params=%p rzv_comp=%p context=%p)\n",
		       params, params->rzv_comp, params->rzv_comp->context);
		return -FI_EAGAIN;
	}

	void *cur_addr_range_end = (void *) (params->tid_info.cur_addr_range.buf + params->tid_info.cur_addr_range.len);
	void *tid_addr_block_end =
		(void *) ((uintptr_t) tid_addr_block.target_iov.iov_base + tid_addr_block.target_iov.iov_len);

	// The start of the Current Address Range should always fall within the
	// resulting tid_addr_block IOV
	assert(tid_addr_block.target_iov.iov_base <= (void *) params->tid_info.cur_addr_range.buf);
	assert(tid_addr_block_end > (void *) params->tid_info.cur_addr_range.buf);

	// Calculate the portion of cur_addr_range that we were able to get TIDs for
	size_t cur_addr_range_tid_len =
		((uintptr_t) MIN(tid_addr_block_end, cur_addr_range_end)) - params->tid_info.cur_addr_range.buf;
	assert(cur_addr_range_tid_len <= params->rzv_comp->context->byte_counter);

	// If this is the last IOV and the tid range covers the end of the current
	// range, then this will be the last CTS we need to send.
	const bool last_cts = (params->cur_iov == (params->niov - 1)) && (tid_addr_block_end >= cur_addr_range_end);

	union fi_opx_hfi1_deferred_work *cts_work =
		opx_hfi1_rx_rzv_rts_tid_prep_cts(work, params, &tid_addr_block, cur_addr_range_tid_len, last_cts);

	if (last_cts) {
		assert(cts_work == work);

		if (OPX_SW_HFI1_TYPE & (OPX_HFI1_WFR | OPX_HFI1_MIXED_9B)) {
			assert(work->work_elem.work_fn == opx_hfi1_rx_rzv_rts_send_cts);
		} else {
			assert(work->work_elem.work_fn == opx_hfi1_rx_rzv_rts_send_cts_16B);
		}
		FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA,
		       "===================================== RECV, HFI -- RENDEZVOUS RTS TID SETUP (end) SUCCESS (params=%p rzv_comp=%p context=%p)\n",
		       params, params->rzv_comp, params->rzv_comp->context);

		FI_OPX_DEBUG_COUNTERS_INC(params->opx_ep->debug_counters.expected_receive.rts_tid_setup_success);

		// This is the "FI_SUCCESS" exit point for this function
		return cts_work->work_elem.work_fn(cts_work);
	}

	assert(cts_work != work);

	int rc = cts_work->work_elem.work_fn(cts_work);
	if (rc == FI_SUCCESS) {
		OPX_BUF_FREE(cts_work);
	} else {
		assert(rc == -FI_EAGAIN);
		slist_insert_tail(&cts_work->work_elem.slist_entry,
				  &params->opx_ep->tx->work_pending[cts_work->work_elem.work_type]);
	}

	// We shouldn't need to adjust the origin byte counter after sending the
	// first CTS packet.
	params->tid_info.origin_byte_counter_adj = 0;

	/* Adjust Current Address Range for next iteration */
	if (tid_addr_block_end >= cur_addr_range_end) {
		// We finished processing the current IOV, so move on to the next one
		++params->cur_iov;
		assert(params->cur_iov < params->niov);
		params->tid_info.cur_addr_range.buf    = params->dput_iov[params->cur_iov].rbuf;
		params->tid_info.cur_addr_range.len    = params->dput_iov[params->cur_iov].bytes;
		params->tid_info.cur_addr_range.iface  = params->dput_iov[params->cur_iov].rbuf_iface;
		params->tid_info.cur_addr_range.device = params->dput_iov[params->cur_iov].rbuf_device;
	} else {
		params->tid_info.cur_addr_range.buf += cur_addr_range_tid_len;
		params->tid_info.cur_addr_range.len -= cur_addr_range_tid_len;
	}

	// Wait until the next poll cycle before trying to register more TIDs.
	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA,
	       "===================================== RECV, HFI -- RENDEZVOUS RTS TID SETUP (end) EAGAIN (Progress) (params=%p rzv_comp=%p context=%p)\n",
	       params, params->rzv_comp, params->rzv_comp->context);

	return -FI_EAGAIN;
}

int opx_hfi1_rx_rzv_rts_elided(struct fi_opx_ep *opx_ep, union fi_opx_hfi1_deferred_work *work,
			       struct fi_opx_hfi1_rx_rzv_rts_params *params)
{
	assert(params->elided_head.bytes || params->elided_tail.bytes);
	assert(!params->is_shm); // We should never be doing this function for shm

	union fi_opx_hfi1_deferred_work	     *cts_work;
	struct fi_opx_hfi1_rx_rzv_rts_params *cts_params;

	cts_work = ofi_buf_alloc(params->opx_ep->tx->work_pending_pool);
	if (OFI_UNLIKELY(cts_work == NULL)) {
		FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA, "Failed to allocate deferred work item!\n");
		return -FI_ENOMEM;
	}
	struct fi_opx_rzv_completion *rzv_comp = ofi_buf_alloc(params->opx_ep->rzv_completion_pool);
	if (OFI_UNLIKELY(rzv_comp == NULL)) {
		FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA, "Failed to allocate rendezvous completion item!\n");
		OPX_BUF_FREE(cts_work);
		return -FI_ENOMEM;
	}

	// Add 1 to the offset so we end up with a cacheline multiple length
	const size_t copy_length = offsetof(struct fi_opx_hfi1_rx_rzv_rts_params, multi_cts_copy_boundary) + 1;
	assert(copy_length < sizeof(*cts_work));
	memcpy(cts_work, work, copy_length);

	cts_work->work_elem.slist_entry.next = NULL;
	cts_params			     = &cts_work->rx_rzv_rts;
	cts_params->rzv_comp		     = rzv_comp;
	cts_params->rzv_comp->context	     = params->rzv_comp->context;

	int niov = 0;

	if (params->elided_head.bytes) {
		cts_params->dput_iov[niov++] = params->elided_head;
	}

	if (params->elided_tail.bytes) {
		cts_params->dput_iov[niov++] = params->elided_tail;
	}

	cts_params->dst_vaddr	    = cts_params->dput_iov[0].rbuf;
	cts_params->cur_iov	    = 0;
	cts_params->niov	    = niov;
	cts_params->tid_info.npairs = 0;

	rzv_comp->byte_counter	    = params->elided_head.bytes + params->elided_tail.bytes;
	rzv_comp->bytes_accumulated = 0;

	if (OPX_SW_HFI1_TYPE & (OPX_HFI1_WFR | OPX_HFI1_MIXED_9B)) {
		cts_params->work_elem.work_fn = opx_hfi1_rx_rzv_rts_send_cts;
	} else {
		cts_params->work_elem.work_fn = opx_hfi1_rx_rzv_rts_send_cts_16B;
	}
	cts_params->work_elem.work_type = OPX_WORK_TYPE_PIO;

	int rc = cts_params->work_elem.work_fn(cts_work);
	if (rc == FI_SUCCESS) {
		OPX_BUF_FREE(cts_work);
	} else {
		assert(rc == -FI_EAGAIN);
		/* Try again later*/
		assert(cts_work->work_elem.slist_entry.next == NULL);
		slist_insert_tail(&cts_work->work_elem.slist_entry, &opx_ep->tx->work_pending[OPX_WORK_TYPE_PIO]);
	}
	return FI_SUCCESS;
}

int opx_hfi1_rx_rzv_rts_send_etrunc_shm(union fi_opx_hfi1_deferred_work *work)
{
	struct fi_opx_hfi1_rx_rzv_rts_params *params = &work->rx_rzv_rts;

	struct fi_opx_ep *opx_ep      = params->opx_ep;
	const uint64_t	  lrh_dlid_9B = params->lrh_dlid;
	const uint64_t	  bth_rx      = ((uint64_t) params->origin_rx) << OPX_BTH_SUBCTXT_RX_SHIFT;

	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA,
	       "===================================== RECV, SHM -- RENDEZVOUS RTS ETRUNC (begin)\n");
	uint64_t pos;
	/* Possible SHM connections required for certain applications (i.e., DAOS)
	 * exceeds the max value of the legacy origin_subctxt_rx field.  Use u32_extended field.
	 */
	ssize_t rc =
		fi_opx_shm_dynamic_tx_connect(OPX_SHM_TRUE, opx_ep, params->u32_extended_rx, params->target_hfi_unit);

	if (OFI_UNLIKELY(rc)) {
		return -FI_EAGAIN;
	}

	union opx_hfi1_packet_hdr *const tx_hdr = opx_shm_tx_next(
		&opx_ep->tx->shm, params->target_hfi_unit, params->origin_rx, &pos, opx_ep->daos_info.hfi_rank_enabled,
		params->u32_extended_rx, opx_ep->daos_info.rank_inst, &rc);

	if (!tx_hdr) {
		return rc;
	}

	/* Note that we do not set stl.hdr.lrh.pktlen here (usually lrh_dws << 32),
	   because this is shm and since it's a CTS packet, lrh.pktlen
	   isn't used/needed */
	tx_hdr->qw_9B[0] = opx_ep->rx->tx.cts_9B.hdr.qw_9B[0] | lrh_dlid_9B;
	tx_hdr->qw_9B[1] = opx_ep->rx->tx.cts_9B.hdr.qw_9B[1] | bth_rx;
	tx_hdr->qw_9B[2] = opx_ep->rx->tx.cts_9B.hdr.qw_9B[2];
	tx_hdr->qw_9B[3] = opx_ep->rx->tx.cts_9B.hdr.qw_9B[3];
	tx_hdr->qw_9B[4] = opx_ep->rx->tx.cts_9B.hdr.qw_9B[4] | (params->opcode);
	tx_hdr->qw_9B[5] = params->origin_byte_counter_vaddr;

	opx_shm_tx_advance(&opx_ep->tx->shm, (void *) tx_hdr, pos);

	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA,
	       "===================================== RECV, SHM -- RENDEZVOUS RTS ETRUNC (end)\n");

	return FI_SUCCESS;
}

int opx_hfi1_rx_rzv_rts_send_etrunc_shm_16B(union fi_opx_hfi1_deferred_work *work)
{
	struct fi_opx_hfi1_rx_rzv_rts_params *params	   = &work->rx_rzv_rts;
	struct fi_opx_ep		     *opx_ep	   = params->opx_ep;
	const uint64_t			      lrh_dlid_16B = params->lrh_dlid;
	const uint64_t			      bth_rx	   = ((uint64_t) params->origin_rx) << OPX_BTH_SUBCTXT_RX_SHIFT;

	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA,
	       "===================================== RECV 16B, SHM -- RENDEZVOUS RTS ETRUNC (begin)\n");
	uint64_t pos;
	/* Possible SHM connections required for certain applications (i.e., DAOS)
	 * exceeds the max value of the legacy origin_subctxt_rx field.  Use u32_extended field.
	 */
	ssize_t rc =
		fi_opx_shm_dynamic_tx_connect(OPX_SHM_TRUE, opx_ep, params->u32_extended_rx, params->target_hfi_unit);

	if (OFI_UNLIKELY(rc)) {
		return -FI_EAGAIN;
	}

	union opx_hfi1_packet_hdr *const tx_hdr = opx_shm_tx_next(
		&opx_ep->tx->shm, params->target_hfi_unit, params->origin_rx, &pos, opx_ep->daos_info.hfi_rank_enabled,
		params->u32_extended_rx, opx_ep->daos_info.rank_inst, &rc);

	if (!tx_hdr) {
		return rc;
	}

	/* Note that we do not set stl.hdr.lrh.pktlen here (usually lrh_dws << 32),
	   because this is shm and since it's a CTS packet, lrh.pktlen
	   isn't used/needed */
	tx_hdr->qw_16B[0] =
		opx_ep->rx->tx.cts_16B.hdr.qw_16B[0] |
		((uint64_t) ((lrh_dlid_16B & OPX_LRH_JKR_16B_DLID_MASK_16B) << OPX_LRH_JKR_16B_DLID_SHIFT_16B));
	tx_hdr->qw_16B[1] =
		opx_ep->rx->tx.cts_16B.hdr.qw_16B[1] |
		((uint64_t) ((lrh_dlid_16B & OPX_LRH_JKR_16B_DLID20_MASK_16B) >> OPX_LRH_JKR_16B_DLID20_SHIFT_16B)) |
		(uint64_t) (bth_rx >> OPX_LRH_JKR_BTH_RX_ENTROPY_SHIFT_16B);
	tx_hdr->qw_16B[2] = opx_ep->rx->tx.cts_16B.hdr.qw_16B[2] | bth_rx;
	tx_hdr->qw_16B[3] = opx_ep->rx->tx.cts_16B.hdr.qw_16B[3];
	tx_hdr->qw_16B[4] = opx_ep->rx->tx.cts_16B.hdr.qw_16B[4];
	tx_hdr->qw_16B[5] = opx_ep->rx->tx.cts_16B.hdr.qw_16B[5] | params->opcode;
	tx_hdr->qw_16B[6] = params->origin_byte_counter_vaddr;

	opx_shm_tx_advance(&opx_ep->tx->shm, (void *) tx_hdr, pos);

	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA,
	       "===================================== RECV, SHM -- RENDEZVOUS RTS ETRUNC (end)\n");

	return FI_SUCCESS;
}

int opx_hfi1_rx_rzv_rts_send_etrunc(union fi_opx_hfi1_deferred_work *work)
{
	struct fi_opx_hfi1_rx_rzv_rts_params *params	  = &work->rx_rzv_rts;
	struct fi_opx_ep		     *opx_ep	  = params->opx_ep;
	const uint64_t			      lrh_dlid_9B = params->lrh_dlid;
	const uint64_t			      bth_rx	  = ((uint64_t) params->origin_rx) << OPX_BTH_SUBCTXT_RX_SHIFT;

	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA,
	       "===================================== RECV, HFI -- RENDEZVOUS EAGER RTS ETRUNC (begin)\n");

	const uint64_t pbc_dws = 2 + /* pbc */
				 2 + /* lrh */
				 3 + /* bth */
				 9;  /* kdeth; from "RcvHdrSize[i].HdrSize" CSR */
	const uint16_t lrh_dws = htons(
		pbc_dws - 2 + 1); /* (BE: LRH DW) does not include pbc (8 bytes), but does include icrc (4 bytes) */

	OPX_SHD_CTX_PIO_LOCK(OPX_IS_CTX_SHARING_ENABLED, opx_ep->tx);
	union fi_opx_hfi1_pio_state pio_state = *opx_ep->tx->pio_state;

	if (OFI_UNLIKELY(FI_OPX_HFI1_AVAILABLE_CREDITS(pio_state, &opx_ep->tx->force_credit_return, 1) < 1)) {
		FI_OPX_HFI1_UPDATE_CREDITS(pio_state, opx_ep->tx->pio_credits_addr);
		opx_ep->tx->pio_state->qw0 = pio_state.qw0;
		if (FI_OPX_HFI1_AVAILABLE_CREDITS(pio_state, &opx_ep->tx->force_credit_return, 1) < 1) {
			FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA,
			       "===================================== RECV, HFI -- RENDEZVOUS EAGER RTS ETRUNC (EAGAIN credits)\n");
			OPX_SHD_CTX_PIO_UNLOCK(OPX_IS_CTX_SHARING_ENABLED, opx_ep->tx);
			return -FI_EAGAIN;
		}
	}

	struct fi_opx_reliability_tx_replay *replay;
	union fi_opx_reliability_tx_psn	    *psn_ptr;
	int64_t				     psn;

	psn = fi_opx_reliability_get_replay(&opx_ep->ep_fid, opx_ep->reli_service, params->slid, params->origin_rx,
					    &psn_ptr, &replay, params->reliability, OPX_SW_HFI1_TYPE);
	if (OFI_UNLIKELY(psn == -1)) {
		FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA,
		       "===================================== RECV, HFI -- RENDEZVOUS EAGER RTS ETRUNC (EAGAIN psn/replay)\n");
		OPX_SHD_CTX_PIO_UNLOCK(OPX_IS_CTX_SHARING_ENABLED, opx_ep->tx);
		return -FI_EAGAIN;
	}

	volatile uint64_t *const scb = FI_OPX_HFI1_PIO_SCB_HEAD(opx_ep->tx->pio_scb_sop_first, pio_state);

	opx_cacheline_copy_qw_vol(scb, replay->scb.qws,
				  opx_ep->rx->tx.cts_9B.qw0 | OPX_PBC_LEN(pbc_dws, OPX_SW_HFI1_TYPE) |
					  params->pbc_dlid | OPX_PBC_LOOPBACK(params->pbc_dlid, OPX_SW_HFI1_TYPE),
				  opx_ep->rx->tx.cts_9B.hdr.qw_9B[0] | lrh_dlid_9B | ((uint64_t) lrh_dws << 32),
				  opx_ep->rx->tx.cts_9B.hdr.qw_9B[1] | bth_rx, opx_ep->rx->tx.cts_9B.hdr.qw_9B[2] | psn,
				  opx_ep->rx->tx.cts_9B.hdr.qw_9B[3],
				  opx_ep->rx->tx.cts_9B.hdr.qw_9B[4] | params->opcode,
				  params->origin_byte_counter_vaddr, 0);

	/* consume one credit */
	FI_OPX_HFI1_CONSUME_SINGLE_CREDIT(pio_state);
	FI_OPX_HFI1_CHECK_CREDITS_FOR_ERROR(opx_ep->tx->pio_credits_addr);
	FI_OPX_HFI1_CLEAR_CREDIT_RETURN(opx_ep);

	/* save the updated txe state */
	opx_ep->tx->pio_state->qw0 = pio_state.qw0;

	OPX_SHD_CTX_PIO_UNLOCK(OPX_IS_CTX_SHARING_ENABLED, opx_ep->tx);

	fi_opx_reliability_service_replay_register_no_update(opx_ep->reli_service, psn_ptr, replay, params->reliability,
							     OPX_SW_HFI1_TYPE);

	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA,
	       "===================================== RECV, HFI -- RENDEZVOUS EAGER RTS ETRUNC (end)");

	return FI_SUCCESS;
}

int opx_hfi1_rx_rzv_rts_send_etrunc_16B(union fi_opx_hfi1_deferred_work *work)
{
	struct fi_opx_hfi1_rx_rzv_rts_params *params	   = &work->rx_rzv_rts;
	struct fi_opx_ep		     *opx_ep	   = params->opx_ep;
	const uint64_t			      lrh_dlid_16B = params->lrh_dlid;
	const uint64_t			      bth_rx	   = ((uint64_t) params->origin_rx) << OPX_BTH_SUBCTXT_RX_SHIFT;

	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA,
	       "===================================== RECV, HFI -- RENDEZVOUS EAGER RTS ETRUNC (begin)\n");

	const uint64_t pbc_dws = 2 +		     /* pbc */
				 4 +		     /* lrh uncompressed */
				 3 +		     /* bth */
				 9 +		     /* kdeth; from "RcvHdrSize[i].HdrSize" CSR */
				 2;		     /* ICRC/tail */
	const uint16_t lrh_qws = (pbc_dws - 2) >> 1; /* (LRH QW) does not include pbc (8 bytes) */

	OPX_SHD_CTX_PIO_LOCK(OPX_IS_CTX_SHARING_ENABLED, opx_ep->tx);
	union fi_opx_hfi1_pio_state pio_state = *opx_ep->tx->pio_state;

	// Note: Only need 1 credit here for the message truncation error case. Just
	// the opcode and origin_byte_counter_vaddr is needed for replaying back to the
	// sender.
	if (OFI_UNLIKELY(FI_OPX_HFI1_AVAILABLE_CREDITS(pio_state, &opx_ep->tx->force_credit_return, 2) < 2)) {
		FI_OPX_HFI1_UPDATE_CREDITS(pio_state, opx_ep->tx->pio_credits_addr);
		opx_ep->tx->pio_state->qw0 = pio_state.qw0;
		if (FI_OPX_HFI1_AVAILABLE_CREDITS(pio_state, &opx_ep->tx->force_credit_return, 2) < 2) {
			FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA,
			       "===================================== RECV, HFI -- RENDEZVOUS EAGER RTS ETRUNC (EAGAIN credits)\n");
			OPX_SHD_CTX_PIO_UNLOCK(OPX_IS_CTX_SHARING_ENABLED, opx_ep->tx);
			return -FI_EAGAIN;
		}
	}

	struct fi_opx_reliability_tx_replay *replay;
	union fi_opx_reliability_tx_psn	    *psn_ptr;
	int64_t				     psn;

	psn = fi_opx_reliability_get_replay(&opx_ep->ep_fid, opx_ep->reli_service, params->slid, params->origin_rx,
					    &psn_ptr, &replay, params->reliability, OPX_SW_HFI1_TYPE);
	if (OFI_UNLIKELY(psn == -1)) {
		FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA,
		       "===================================== RECV, HFI -- RENDEZVOUS EAGER RTS ETRUNC (EAGAIN psn/replay)\n");
		OPX_SHD_CTX_PIO_UNLOCK(OPX_IS_CTX_SHARING_ENABLED, opx_ep->tx);
		return -FI_EAGAIN;
	}

	volatile uint64_t *const scb = FI_OPX_HFI1_PIO_SCB_HEAD(opx_ep->tx->pio_scb_sop_first, pio_state);

	opx_cacheline_copy_qw_vol(
		scb, replay->scb.qws,
		opx_ep->rx->tx.cts_16B.qw0 | OPX_PBC_LEN(pbc_dws, OPX_SW_HFI1_TYPE) | params->pbc_dlid |
			OPX_PBC_LOOPBACK(params->pbc_dlid, OPX_SW_HFI1_TYPE),
		opx_ep->rx->tx.cts_16B.hdr.qw_16B[0] |
			((uint64_t) (lrh_dlid_16B & OPX_LRH_JKR_16B_DLID_MASK_16B) << OPX_LRH_JKR_16B_DLID_SHIFT_16B) |
			((uint64_t) lrh_qws << 20),
		opx_ep->rx->tx.cts_16B.hdr.qw_16B[1] |
			((uint64_t) ((lrh_dlid_16B & OPX_LRH_JKR_16B_DLID20_MASK_16B) >>
				     OPX_LRH_JKR_16B_DLID20_SHIFT_16B)) |
			(uint64_t) (bth_rx >> OPX_LRH_JKR_BTH_RX_ENTROPY_SHIFT_16B),
		opx_ep->rx->tx.cts_16B.hdr.qw_16B[2] | bth_rx, opx_ep->rx->tx.cts_16B.hdr.qw_16B[3] | psn,
		opx_ep->rx->tx.cts_16B.hdr.qw_16B[4], opx_ep->rx->tx.cts_16B.hdr.qw_16B[5] | params->opcode,
		params->origin_byte_counter_vaddr);

	FI_OPX_HFI1_CONSUME_SINGLE_CREDIT(pio_state);

	// 2nd cacheline
	volatile uint64_t *const scb2 = FI_OPX_HFI1_PIO_SCB_HEAD(opx_ep->tx->pio_scb_first, pio_state);

	opx_cacheline_store_qw_vol(scb2, 0, 0, 0, 0, 0, 0, 0, 0);

	FI_OPX_HFI1_CONSUME_SINGLE_CREDIT(pio_state);
	FI_OPX_HFI1_CHECK_CREDITS_FOR_ERROR(opx_ep->tx->pio_credits_addr);
	FI_OPX_HFI1_CLEAR_CREDIT_RETURN(opx_ep);

	/* save the updated txe state */
	opx_ep->tx->pio_state->qw0 = pio_state.qw0;

	OPX_SHD_CTX_PIO_UNLOCK(OPX_IS_CTX_SHARING_ENABLED, opx_ep->tx);
	fi_opx_reliability_service_replay_register_no_update(opx_ep->reli_service, psn_ptr, replay, params->reliability,
							     OPX_SW_HFI1_TYPE);

	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA,
	       "===================================== RECV, HFI -- RENDEZVOUS EAGER RTS ETRUNC (end)");

	return FI_SUCCESS;
}

void fi_opx_hfi1_rx_rzv_rts_etrunc(struct fi_opx_ep *opx_ep, const union opx_hfi1_packet_hdr *const hdr,
				   const uint16_t origin_rx, uintptr_t origin_byte_counter_vaddr, const unsigned is_shm,
				   const enum ofi_reliability_kind reliability, const uint32_t u32_extended_rx,
				   const enum opx_hfi1_type hfi1_type)
{
	union fi_opx_hfi1_deferred_work *work = ofi_buf_alloc(opx_ep->tx->work_pending_pool);
	assert(work != NULL);
	struct fi_opx_hfi1_rx_rzv_rts_params *params = &work->rx_rzv_rts;
	params->opx_ep				     = opx_ep;
	params->work_elem.slist_entry.next	     = NULL;

	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA, "is_shm %u, opcode=%u\n", is_shm, FI_OPX_HFI_DPUT_OPCODE_RZV_ETRUNC);

	opx_lid_t lid;
	if (hfi1_type & OPX_HFI1_WFR) {
		lid		 = (opx_lid_t) __be16_to_cpu24((__be16) hdr->lrh_9B.slid);
		params->slid	 = lid;
		params->lrh_dlid = (hdr->lrh_9B.qw[0] & 0xFFFF000000000000ul) >> 32;
		params->pbc_dlid = OPX_PBC_DLID(lid, OPX_HFI1_WFR);

	} else if (hfi1_type & OPX_HFI1_MIXED_9B) {
		lid		 = (opx_lid_t) __be16_to_cpu24((__be16) hdr->lrh_9B.slid);
		params->slid	 = lid;
		params->lrh_dlid = (hdr->lrh_9B.qw[0] & 0xFFFF000000000000ul) >> 32;
		params->pbc_dlid = OPX_PBC_DLID(lid, OPX_HFI1_MIXED_9B);
	} else if (hfi1_type & OPX_HFI1_JKR) {
		lid		 = (opx_lid_t) __le24_to_cpu(hdr->lrh_16B.slid20 << 20 | hdr->lrh_16B.slid);
		params->slid	 = lid;
		params->lrh_dlid = lid; // Send CTS to the SLID that sent RTS
		params->pbc_dlid = OPX_PBC_DLID(lid, OPX_HFI1_JKR);
	} else {
		lid		 = (opx_lid_t) __le24_to_cpu(hdr->lrh_16B.slid20 << 20 | hdr->lrh_16B.slid);
		params->slid	 = lid;
		params->lrh_dlid = lid; // Send CTS to the SLID that sent RTS
		params->pbc_dlid = OPX_PBC_DLID(lid, OPX_HFI1_CYR);
	}

	if (is_shm) {
		if (hfi1_type & (OPX_HFI1_WFR | OPX_HFI1_MIXED_9B)) {
			params->work_elem.work_fn = opx_hfi1_rx_rzv_rts_send_etrunc_shm;
		} else {
			params->work_elem.work_fn = opx_hfi1_rx_rzv_rts_send_etrunc_shm_16B;
		}
		params->work_elem.work_type = OPX_WORK_TYPE_SHM;
		params->target_hfi_unit	    = fi_opx_hfi1_get_lid_local_unit(lid);
	} else {
		if (hfi1_type & (OPX_HFI1_WFR | OPX_HFI1_MIXED_9B)) {
			params->work_elem.work_fn = opx_hfi1_rx_rzv_rts_send_etrunc;
		} else {
			params->work_elem.work_fn = opx_hfi1_rx_rzv_rts_send_etrunc_16B;
		}
		params->work_elem.work_type = OPX_WORK_TYPE_PIO;
		params->target_hfi_unit	    = 0xFF;
	}

	params->origin_rx		  = origin_rx;
	params->u32_extended_rx		  = u32_extended_rx;
	params->origin_byte_counter_vaddr = origin_byte_counter_vaddr;
	params->is_shm			  = is_shm;
	params->reliability		  = reliability;
	params->opcode			  = FI_OPX_HFI_DPUT_OPCODE_RZV_ETRUNC;

	int rc = params->work_elem.work_fn(work);
	if (rc == FI_SUCCESS) {
		OPX_BUF_FREE(work);
		FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA, "FI_SUCCESS\n");
		return;
	}
	assert(rc == -FI_EAGAIN);
	/* Try again later*/
	assert(work->work_elem.slist_entry.next == NULL);
	slist_insert_tail(&work->work_elem.slist_entry, &opx_ep->tx->work_pending[params->work_elem.work_type]);
	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA, "FI_EAGAIN\n");
}

void fi_opx_hfi1_rx_rzv_rts(struct fi_opx_ep *opx_ep, const union opx_hfi1_packet_hdr *const hdr,
			    const void *const payload, const uint16_t origin_rx, const uint64_t niov,
			    uintptr_t origin_byte_counter_vaddr, struct opx_context *const target_context,
			    const uintptr_t dst_vaddr, const enum fi_hmem_iface dst_iface, const uint64_t dst_device,
			    const uint64_t immediate_data, const uint64_t immediate_end_bytes,
			    const struct fi_opx_hmem_iov *src_iovs, uint8_t opcode, const unsigned is_shm,
			    const enum ofi_reliability_kind reliability, const uint32_t u32_extended_rx,
			    const enum opx_hfi1_type hfi1_type)
{
	OPX_TRACER_TRACE(OPX_TRACER_BEGIN, "RECV-RZV-RTS-HFI:%ld", hdr->qw_9B[6]);
	union fi_opx_hfi1_deferred_work *work = ofi_buf_alloc(opx_ep->tx->work_pending_pool);
	assert(work != NULL);
	struct fi_opx_hfi1_rx_rzv_rts_params *params = &work->rx_rzv_rts;
	params->opx_ep				     = opx_ep;
	params->work_elem.slist_entry.next	     = NULL;

	assert(niov <= MIN(FI_OPX_MAX_HMEM_IOV, FI_OPX_MAX_DPUT_IOV));

	const struct fi_opx_hmem_iov *src_iov	  = src_iovs;
	uint64_t		      is_hmem	  = dst_iface;
	uint64_t		      rbuf_offset = 0;
	for (int i = 0; i < niov; i++) {
#ifdef OPX_HMEM
		is_hmem |= src_iov->iface;
#endif
		params->dput_iov[i].sbuf	= src_iov->buf;
		params->dput_iov[i].sbuf_iface	= src_iov->iface;
		params->dput_iov[i].sbuf_device = src_iov->device;
		params->dput_iov[i].rbuf	= dst_vaddr + rbuf_offset;
		params->dput_iov[i].rbuf_iface	= dst_iface;
		params->dput_iov[i].rbuf_device = dst_device;
		params->dput_iov[i].bytes	= src_iov->len;
		params->dput_iov[i].sbuf_handle = 0; // Set this properly after implementing gdrcopy intranode fallback
		rbuf_offset += src_iov->len;
		++src_iov;
	}

	opx_lid_t lid;
	if (hfi1_type & OPX_HFI1_WFR) {
		lid		 = (opx_lid_t) __be16_to_cpu24((__be16) hdr->lrh_9B.slid);
		params->slid	 = lid;
		params->lrh_dlid = (hdr->lrh_9B.qw[0] & 0xFFFF000000000000ul) >> 32;
		params->pbc_dlid = OPX_PBC_DLID(lid, OPX_HFI1_WFR);
	} else if (hfi1_type & OPX_HFI1_MIXED_9B) {
		lid		 = (opx_lid_t) __be16_to_cpu24((__be16) hdr->lrh_9B.slid);
		params->slid	 = lid;
		params->lrh_dlid = (hdr->lrh_9B.qw[0] & 0xFFFF000000000000ul) >> 32;
		params->pbc_dlid = OPX_PBC_DLID(lid, OPX_HFI1_MIXED_9B);
	} else if (hfi1_type & OPX_HFI1_JKR) {
		lid		 = (opx_lid_t) __le24_to_cpu(hdr->lrh_16B.slid20 << 20 | hdr->lrh_16B.slid);
		params->slid	 = lid;
		params->lrh_dlid = lid; // Send CTS to the SLID that sent RTS
		params->pbc_dlid = OPX_PBC_DLID(lid, OPX_HFI1_JKR);
	} else {
		lid		 = (opx_lid_t) __le24_to_cpu(hdr->lrh_16B.slid20 << 20 | hdr->lrh_16B.slid);
		params->slid	 = lid;
		params->lrh_dlid = lid; // Send CTS to the SLID that sent RTS
		params->pbc_dlid = OPX_PBC_DLID(lid, OPX_HFI1_CYR);
	}

	if (is_shm) {
		FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA, "is_shm %u\n", is_shm);
		if (hfi1_type & (OPX_HFI1_WFR | OPX_HFI1_MIXED_9B)) {
			params->work_elem.work_fn = opx_hfi1_rx_rzv_rts_send_cts_shm;
		} else {
			params->work_elem.work_fn = opx_hfi1_rx_rzv_rts_send_cts_shm_16B;
		}
		params->work_elem.work_type = OPX_WORK_TYPE_SHM;
		params->target_hfi_unit	    = fi_opx_hfi1_get_lid_local_unit(lid);
	} else {
		FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA, "opx_ep->use_expected_tid_rzv=%u niov=%lu opcode=%X\n",
		       opx_ep->use_expected_tid_rzv, niov, opcode);

		if (hfi1_type & (OPX_HFI1_WFR | OPX_HFI1_MIXED_9B)) {
			params->work_elem.work_fn = opx_hfi1_rx_rzv_rts_send_cts;
		} else {
			params->work_elem.work_fn = opx_hfi1_rx_rzv_rts_send_cts_16B;
		}
		params->work_elem.work_type = OPX_WORK_TYPE_PIO;
		params->target_hfi_unit	    = 0xFF;
	}
	params->work_elem.completion_action = NULL;
	params->work_elem.payload_copy	    = NULL;
	params->work_elem.complete	    = false;

	params->origin_rx			 = origin_rx;
	params->u32_extended_rx			 = u32_extended_rx;
	params->niov				 = niov;
	params->cur_iov				 = 0;
	params->origin_byte_counter_vaddr	 = origin_byte_counter_vaddr;
	params->rzv_comp			 = ofi_buf_alloc(opx_ep->rzv_completion_pool);
	params->rzv_comp->tid_vaddr		 = 0UL;
	params->rzv_comp->tid_length		 = 0UL;
	params->rzv_comp->byte_counter		 = 0UL;
	params->rzv_comp->bytes_accumulated	 = 0UL;
	params->rzv_comp->context		 = target_context;
	params->dst_vaddr			 = dst_vaddr;
	params->is_shm				 = is_shm;
	params->reliability			 = reliability;
	params->tid_info.npairs			 = 0;
	params->tid_info.offset			 = 0;
	params->tid_info.origin_byte_counter_adj = 0;
	params->opcode				 = opcode;
	params->elided_head.bytes		 = 0;
	params->elided_tail.bytes		 = 0;

	if (opx_hfi1_rx_rzv_rts_tid_eligible(opx_ep, params, niov, immediate_data, immediate_end_bytes, is_hmem,
					     ((struct fi_opx_hmem_info *) target_context->hmem_info_qws)->is_unified,
					     is_shm, dst_iface, opcode)) {
		if (params->elided_head.bytes || params->elided_tail.bytes) {
			opx_hfi1_rx_rzv_rts_elided(opx_ep, work, params);
		}
		params->tid_info.cur_addr_range.buf    = params->dput_iov[0].rbuf;
		params->tid_info.cur_addr_range.len    = params->dput_iov[0].bytes;
		params->tid_info.cur_addr_range.iface  = params->dput_iov[0].rbuf_iface;
		params->tid_info.cur_addr_range.device = params->dput_iov[0].rbuf_device;

		params->work_elem.work_fn   = opx_hfi1_rx_rzv_rts_tid_setup;
		params->work_elem.work_type = OPX_WORK_TYPE_TID_SETUP;
		params->opcode		    = FI_OPX_HFI_DPUT_OPCODE_RZV_TID;
	}

	params->rzv_comp->byte_counter = target_context->byte_counter;

	int rc = params->work_elem.work_fn(work);
	if (rc == FI_SUCCESS) {
		OPX_BUF_FREE(work);
		OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "RECV-RZV-RTS-HFI:%ld", hdr->qw_9B[6]);
		FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA, "FI_SUCCESS\n");
		return;
	}
	assert(rc == -FI_EAGAIN);
	/* Try again later*/
	assert(work->work_elem.slist_entry.next == NULL);
	slist_insert_tail(&work->work_elem.slist_entry, &opx_ep->tx->work_pending[params->work_elem.work_type]);
	OPX_TRACER_TRACE(OPX_TRACER_END_EAGAIN, "RECV-RZV-RTS-HFI:%ld", hdr->qw_9B[6]);
	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA, "FI_EAGAIN\n");
}

#ifdef OPX_HMEM
void opx_hfi1_rx_ipc_rts(struct fi_opx_ep *opx_ep, const union opx_hfi1_packet_hdr *const hdr,
			 const union fi_opx_hfi1_packet_payload *const payload, const uint16_t origin_rx,
			 const uint64_t niov, const uint64_t is_hmem, struct opx_context *const context,
			 const uint64_t xfer_len, const uint32_t u32_extended_rx, const enum opx_hfi1_type hfi1_type)
{
	OPX_TRACER_TRACE(OPX_TRACER_BEGIN, "RECV-IPC-RTS-HFI");

	assert(payload->rendezvous.ipc.ipc_info.iface != FI_HMEM_SYSTEM);

	void		    *device_ptr;
	struct ofi_mr_entry *entry;
	struct ipc_info	    *ipc_info = (struct ipc_info *) &payload->rendezvous.ipc.ipc_info;

	OPX_TRACER_TRACE(OPX_TRACER_BEGIN, "IPC-RECV-OPEN-HANDLE");
	int ret = ofi_ipc_cache_search(opx_ep->domain->hmem_domain->ipc_cache, origin_rx, ipc_info, &entry);
	if (ret) {
		FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA,
			"FATAL ERROR, Failed to open IPC handle for Device. Error code: %d, abort\n", ret);
		abort();
	}
	device_ptr = (char *) (uintptr_t) entry->info.mapped_addr + (uintptr_t) ipc_info->offset;
	OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "IPC-RECV-OPEN-HANDLE");

	/* Most modern GPUs have support for Unified Virtual Addressing (UVA).
	   This allows us to use generic cudaMemcpy for DtoH and DtoD */
	OPX_TRACER_TRACE(OPX_TRACER_BEGIN, "IPC-P2P-DIRECT-COPY");
	union opx_hmem_event *event = NULL;
	if (!is_hmem) {
		ret = ofi_copy_from_hmem(ipc_info->iface, ipc_info->device, context->buf, device_ptr, xfer_len);
		if (ret) {
			FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA,
				"FATAL ERROR, cudaMemcpy with IPC handle failed. Abort\n");
			abort();
		}
	} else {
		if (ipc_info->iface == FI_HMEM_CUDA) {
			opx_hmem_memcpy_async(FI_HMEM_CUDA, ipc_info->device, context->buf, device_ptr, xfer_len,
					      opx_ep->domain->hmem_domain, &event, OPX_HMEM_MEMCPY_ASYNC_DTOD);
		} else if (ipc_info->iface == FI_HMEM_ROCR) {
			opx_hmem_memcpy_async(FI_HMEM_ROCR, ipc_info->device, context->buf, device_ptr, xfer_len,
					      opx_ep->domain->hmem_domain, &event, OPX_HMEM_MEMCPY_ASYNC_DTOD);
		} else {
			FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA,
				"FATAL ERROR, memcpy with IPC handle unexpected iface=%lu. Abort\n", ipc_info->iface);
			abort();
		}
	}
	OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "IPC-P2P-DIRECT-COPY");
	if (event == NULL) {
		OPX_TRACER_TRACE(OPX_TRACER_BEGIN, "IPC-DESTROY-HANDLE");
		ofi_mr_cache_delete(opx_ep->domain->hmem_domain->ipc_cache, entry);
		OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "IPC-DESTROY-HANDLE");
		context->byte_counter = 0;
		context->flags &= ~FI_OPX_CQ_CONTEXT_HMEM;
		slist_insert_tail((struct slist_entry *) context, opx_ep->rx->cq_completed_ptr);
	}

	union fi_opx_hfi1_deferred_work *work = ofi_buf_alloc(opx_ep->tx->work_pending_pool);
	assert(work != NULL);
	struct opx_hfi1_rx_ipc_rts_params *params = &work->rx_ipc_rts;

	params->work_elem.work_type	    = OPX_WORK_TYPE_SHM;
	params->work_elem.completion_action = NULL;
	params->work_elem.payload_copy	    = NULL;
	params->work_elem.complete	    = false;
	params->work_elem.slist_entry.next  = NULL;
	params->opx_ep			    = opx_ep;
	params->origin_byte_counter_vaddr   = payload->rendezvous.ipc.origin_byte_counter_vaddr;
	params->niov			    = niov;
	params->context			    = context;
	params->cache_entry		    = entry;
	params->hmem_event		    = event;

	opx_lid_t lid;
	if (hfi1_type & (OPX_HFI1_WFR | OPX_HFI1_MIXED_9B)) {
		lid			  = (opx_lid_t) __be16_to_cpu24((__be16) hdr->lrh_9B.slid);
		params->lrh_dlid	  = (hdr->lrh_9B.qw[0] & 0xFFFF000000000000ul) >> 32;
		params->work_elem.work_fn = opx_ipc_send_cts_9B;
	} else {
		lid			  = (opx_lid_t) __le24_to_cpu(hdr->lrh_16B.slid20 << 20 | hdr->lrh_16B.slid);
		params->lrh_dlid	  = lid; // Send CTS to the SLID that sent RTS
		params->work_elem.work_fn = opx_ipc_send_cts_16B;
	}

	params->u32_extended_rx = u32_extended_rx;
	params->origin_rx	= origin_rx;
	// Assumes shm
	assert(opx_lid_is_shm(lid));
	params->target_hfi_unit = fi_opx_hfi1_get_lid_local_unit(lid);

	int rc = params->work_elem.work_fn(work);
	if (rc == FI_SUCCESS) {
		OPX_BUF_FREE(work);
		OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "RECV-IPC-RTS-HFI");
		FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA, "FI_SUCCESS\n");
		return;
	}
	assert(rc == -FI_EAGAIN);
	/* Try again later*/
	assert(work->work_elem.slist_entry.next == NULL);
	slist_insert_tail(&work->work_elem.slist_entry, &opx_ep->tx->work_pending[params->work_elem.work_type]);
	OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "RECV-IPC-RTS-HFI");
	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA, "FI_EAGAIN\n");
}
#endif

int opx_hfi1_do_dput_fence(union fi_opx_hfi1_deferred_work *work)
{
	struct fi_opx_hfi1_rx_dput_fence_params *params = &work->fence;
	struct fi_opx_ep			*opx_ep = params->opx_ep;

	uint64_t pos;
	/* Possible SHM connections required for certain applications (i.e., DAOS)
	 * exceeds the max value of the legacy origin_subctxt_rx field.  Use u32_extended field.
	 */
	ssize_t rc = fi_opx_shm_dynamic_tx_connect(OPX_SHM_TRUE, opx_ep, params->origin_rx, params->target_hfi_unit);
	if (OFI_UNLIKELY(rc)) {
		return -FI_EAGAIN;
	}

	union opx_hfi1_packet_hdr *const hdr = opx_shm_tx_next(
		&opx_ep->tx->shm, params->target_hfi_unit, params->origin_rx, &pos, opx_ep->daos_info.hfi_rank_enabled,
		params->u32_extended_rx, opx_ep->daos_info.rank_inst, &rc);
	if (hdr == NULL) {
		return rc;
	}

	if (OPX_SW_HFI1_TYPE & (OPX_HFI1_WFR | OPX_HFI1_MIXED_9B)) {
		const uint64_t pbc_dws = 2 + /* pbc */
					 2 + /* lrh */
					 3 + /* bth */
					 9;  /* kdeth; from "RcvHdrSize[i].HdrSize" CSR */
		const uint16_t lrh_dws =
			htons(pbc_dws - 2 +
			      1); /* (BE: LRH DW) does not include pbc (8 bytes), but does include icrc (4 bytes) */

		hdr->qw_9B[0] = opx_ep->rx->tx.dput_9B.hdr.qw_9B[0] | params->lrh_dlid | ((uint64_t) lrh_dws << 32);
		hdr->qw_9B[1] = opx_ep->rx->tx.dput_9B.hdr.qw_9B[1] | params->bth_subctxt_rx;
		hdr->qw_9B[2] = opx_ep->rx->tx.dput_9B.hdr.qw_9B[2];
		hdr->qw_9B[3] = opx_ep->rx->tx.dput_9B.hdr.qw_9B[3];
		hdr->qw_9B[4] = opx_ep->rx->tx.dput_9B.hdr.qw_9B[4] | (FI_OPX_HFI_DPUT_OPCODE_FENCE << 4);
		hdr->qw_9B[5] = (uint64_t) params->cc;
		hdr->qw_9B[6] = params->bytes_to_fence;
	} else {
		const uint64_t bth_rx  = params->bth_subctxt_rx;
		const uint64_t pbc_dws = 2 +		     /* pbc */
					 4 +		     /* lrh uncompressed */
					 3 +		     /* bth */
					 9 +		     /* kdeth; from "RcvHdrSize[i].HdrSize" CSR */
					 2;		     /* ICRC/tail */
		const uint16_t lrh_dws = (pbc_dws - 2) >> 1; /* (LRH QW) does not include pbc (8 bytes) */
		hdr->qw_16B[0]	       = opx_ep->rx->tx.dput_16B.hdr.qw_16B[0] |
				 ((uint64_t) (params->lrh_dlid & OPX_LRH_JKR_16B_DLID_MASK_16B)
				  << OPX_LRH_JKR_16B_DLID_SHIFT_16B) |
				 ((uint64_t) lrh_dws << 20);
		hdr->qw_16B[1] = opx_ep->rx->tx.dput_16B.hdr.qw_16B[1] |
				 ((uint64_t) ((params->lrh_dlid & OPX_LRH_JKR_16B_DLID20_MASK_16B) >>
					      OPX_LRH_JKR_16B_DLID20_SHIFT_16B)) |
				 (uint64_t) (bth_rx >> OPX_LRH_JKR_BTH_RX_ENTROPY_SHIFT_16B);
		hdr->qw_16B[2] = opx_ep->rx->tx.dput_16B.hdr.qw_16B[2] | bth_rx;
		hdr->qw_16B[3] = opx_ep->rx->tx.dput_16B.hdr.qw_16B[3];
		hdr->qw_16B[4] = opx_ep->rx->tx.dput_16B.hdr.qw_16B[4];
		hdr->qw_16B[5] =
			opx_ep->rx->tx.dput_16B.hdr.qw_16B[5] | (FI_OPX_HFI_DPUT_OPCODE_FENCE << 4) | (0ULL << 32);
		hdr->qw_16B[6] = (uintptr_t) params->cc;
		hdr->qw_16B[7] = params->bytes_to_fence;
	}

	opx_shm_tx_advance(&opx_ep->tx->shm, (void *) hdr, pos);

	return FI_SUCCESS;
}

void opx_hfi1_dput_fence(struct fi_opx_ep *opx_ep, const union opx_hfi1_packet_hdr *const hdr, const uint16_t origin_rx,
			 const uint32_t u32_extended_rx, const enum opx_hfi1_type hfi1_type)
{
	union fi_opx_hfi1_deferred_work *work = ofi_buf_alloc(opx_ep->tx->work_pending_pool);
	assert(work != NULL);
	struct fi_opx_hfi1_rx_dput_fence_params *params = &work->fence;
	params->opx_ep					= opx_ep;
	params->work_elem.slist_entry.next		= NULL;
	params->work_elem.work_fn			= opx_hfi1_do_dput_fence;
	params->work_elem.completion_action		= NULL;
	params->work_elem.payload_copy			= NULL;
	params->work_elem.complete			= false;
	params->work_elem.work_type			= OPX_WORK_TYPE_SHM;

	opx_lid_t slid;
	if (hfi1_type & (OPX_HFI1_WFR | OPX_HFI1_MIXED_9B)) {
		slid		 = (opx_lid_t) __be16_to_cpu24((__be16) hdr->lrh_9B.slid);
		params->lrh_dlid = (hdr->lrh_9B.qw[0] & 0xFFFF000000000000ul) >> 32;
	} else {
		slid		 = (opx_lid_t) __le24_to_cpu(hdr->lrh_16B.slid20 << 20 | hdr->lrh_16B.slid);
		params->lrh_dlid = hdr->lrh_16B.slid20 << 20 | hdr->lrh_16B.slid;
	}

	params->bth_subctxt_rx	= (uint64_t) origin_rx << OPX_BTH_SUBCTXT_RX_SHIFT;
	params->origin_rx	= origin_rx;
	params->u32_extended_rx = u32_extended_rx;
	params->bytes_to_fence	= hdr->dput.target.fence.bytes_to_fence;
	params->cc		= (struct fi_opx_completion_counter *) hdr->dput.target.fence.completion_counter;
	// Assumes shm but will need loopback/shm for sriov.  Both slid/dlid == my lid?
	assert(opx_lid_is_shm(slid));
	params->target_hfi_unit = fi_opx_hfi1_get_lid_local_unit(slid);

	int rc = opx_hfi1_do_dput_fence(work);

	if (rc == FI_SUCCESS) {
		OPX_BUF_FREE(work);
		return;
	}
	assert(rc == -FI_EAGAIN);
	/* Try again later*/
	assert(work->work_elem.slist_entry.next == NULL);
	slist_insert_tail(&work->work_elem.slist_entry, &opx_ep->tx->work_pending[OPX_WORK_TYPE_SHM]);
}

int opx_hfi1_rx_rma_rts_send_cts_shm(union fi_opx_hfi1_deferred_work *work)
{
	struct opx_hfi1_rma_rts_params *params	  = &work->rma_rts;
	struct fi_opx_ep	       *opx_ep	  = params->opx_ep;
	const uint64_t			lrh_dlid  = params->lrh_dlid;
	const uint64_t			bth_rx	  = ((uint64_t) params->origin_rx) << OPX_BTH_SUBCTXT_RX_SHIFT;
	const enum opx_hfi1_type	hfi1_type = OPX_SW_HFI1_TYPE;

	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA,
	       "===================================== RECV, SHM -- RMA RTS (begin)\n");
	OPX_TRACER_TRACE(OPX_TRACER_BEGIN, "RECV-RMA-RTS-SHM");
	uint64_t pos;
	/* Possible SHM connections required for certain applications (i.e., DAOS)
	 * exceeds the max value of the legacy origin_subctxt_rx field.  Use u32_extended field.
	 */
	ssize_t rc =
		fi_opx_shm_dynamic_tx_connect(OPX_SHM_TRUE, opx_ep, params->u32_extended_rx, params->target_hfi_unit);

	if (OFI_UNLIKELY(rc)) {
		return -FI_EAGAIN;
	}

	// TODO pass in subctxt
	union opx_hfi1_packet_hdr *const hdr = opx_shm_tx_next(
		&opx_ep->tx->shm, params->target_hfi_unit, params->origin_rx, &pos, opx_ep->daos_info.hfi_rank_enabled,
		params->u32_extended_rx, opx_ep->daos_info.rank_inst, &rc);

	if (!hdr) {
		return rc;
	}

	/* Note that we do not set stl.hdr.lrh.pktlen here (usually lrh_dws << 32),
	   because this is shm and since it's a CTS packet, lrh.pktlen
	   isn't used/needed */
	uint64_t niov = params->niov << 48;
	uint64_t op64 = ((uint64_t) params->op) << 40;
	uint64_t dt64 = ((uint64_t) params->dt) << 32;
	if (hfi1_type & (OPX_HFI1_WFR | OPX_HFI1_MIXED_9B)) {
		hdr->qw_9B[0] = opx_ep->rx->tx.cts_9B.hdr.qw_9B[0] | lrh_dlid;
		hdr->qw_9B[1] = opx_ep->rx->tx.cts_9B.hdr.qw_9B[1] | bth_rx;
		hdr->qw_9B[2] = opx_ep->rx->tx.cts_9B.hdr.qw_9B[2];
		hdr->qw_9B[3] = opx_ep->rx->tx.cts_9B.hdr.qw_9B[3];
		hdr->qw_9B[4] = opx_ep->rx->tx.cts_9B.hdr.qw_9B[4] | niov | op64 | dt64 | (params->opcode);
		hdr->qw_9B[5] = (uint64_t) params->origin_rma_req;
		hdr->qw_9B[6] = (uint64_t) params->rma_req;
	} else {
		hdr->qw_16B[0] =
			opx_ep->rx->tx.cts_16B.hdr.qw_16B[0] |
			((uint64_t) (lrh_dlid & OPX_LRH_JKR_16B_DLID_MASK_16B) << OPX_LRH_JKR_16B_DLID_SHIFT_16B);
		hdr->qw_16B[1] = opx_ep->rx->tx.cts_16B.hdr.qw_16B[1] |
				 ((uint64_t) ((lrh_dlid & OPX_LRH_JKR_16B_DLID20_MASK_16B) >>
					      OPX_LRH_JKR_16B_DLID20_SHIFT_16B)) |
				 (uint64_t) (bth_rx >> OPX_LRH_JKR_BTH_RX_ENTROPY_SHIFT_16B);
		hdr->qw_16B[2] = opx_ep->rx->tx.cts_16B.hdr.qw_16B[2] | bth_rx;
		hdr->qw_16B[3] = opx_ep->rx->tx.cts_16B.hdr.qw_16B[3];
		hdr->qw_16B[4] = opx_ep->rx->tx.cts_16B.hdr.qw_16B[4];
		hdr->qw_16B[5] = opx_ep->rx->tx.cts_16B.hdr.qw_16B[5] | niov | op64 | dt64 | params->opcode;
		hdr->qw_16B[6] = (uint64_t) params->origin_rma_req;
		hdr->qw_16B[7] = (uint64_t) params->rma_req;
	}

	union fi_opx_hfi1_packet_payload *const tx_payload = (union fi_opx_hfi1_packet_payload *) (hdr + 1);

	for (int i = 0; i < params->niov; i++) {
		tx_payload->cts.iov[i] = params->dput_iov[i];
	}

	opx_shm_tx_advance(&opx_ep->tx->shm, (void *) hdr, pos);

	OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "RECV-RMA-RTS-SHM");
	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA,
	       "===================================== RECV, SHM -- RMA RTS (end)\n");

	return FI_SUCCESS;
}

int opx_hfi1_rx_rma_rts_send_cts(union fi_opx_hfi1_deferred_work *work)
{
	struct opx_hfi1_rma_rts_params *params	 = &work->rma_rts;
	struct fi_opx_ep	       *opx_ep	 = params->opx_ep;
	const uint64_t			lrh_dlid = params->lrh_dlid;
	const uint64_t			bth_rx	 = ((uint64_t) params->origin_rx) << OPX_BTH_SUBCTXT_RX_SHIFT;

	const enum opx_hfi1_type hfi1_type = OPX_SW_HFI1_TYPE;

	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA,
	       "===================================== RECV, HFI -- RMA RTS (begin) (params=%p rma_req=%p context=%p)\n",
	       params, params->rma_req, params->rma_req->context);
	assert(params->rma_req->context->byte_counter >= params->dput_iov[0].bytes);
	OPX_TRACER_TRACE(OPX_TRACER_BEGIN, "SEND-RMA-CTS-HFI:%p", params->rma_req);
	const uint64_t payload_bytes = (params->niov * sizeof(union opx_hfi1_dput_iov));

	OPX_SHD_CTX_PIO_LOCK(OPX_IS_CTX_SHARING_ENABLED, opx_ep->tx);

	union fi_opx_hfi1_pio_state pio_state		 = *opx_ep->tx->pio_state;
	const uint16_t		    total_credits_needed = ((hfi1_type & OPX_HFI1_CNX000) ? 2 : 1) + /* packet header */
					      ((payload_bytes + 63) >> 6); /* payload blocks needed */
	uint64_t total_credits_available =
		FI_OPX_HFI1_AVAILABLE_CREDITS(pio_state, &opx_ep->tx->force_credit_return, total_credits_needed);

	if (OFI_UNLIKELY(total_credits_available < total_credits_needed)) {
		fi_opx_compiler_msync_writes();
		FI_OPX_HFI1_UPDATE_CREDITS(pio_state, opx_ep->tx->pio_credits_addr);
		total_credits_available	   = FI_OPX_HFI1_AVAILABLE_CREDITS(pio_state, &opx_ep->tx->force_credit_return,
									   total_credits_needed);
		opx_ep->tx->pio_state->qw0 = pio_state.qw0;

		if (total_credits_available < total_credits_needed) {
			OPX_TRACER_TRACE(OPX_TRACER_END_EAGAIN, "SEND-RMA-CTS-HFI:%p", params->rma_req);
			FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA,
			       "===================================== RECV, HFI -- RMA RTS (EAGAIN credits) (params=%p rzv_comp=%p context=%p)\n",
			       params, params->rma_req, params->rma_req->context);
			OPX_SHD_CTX_PIO_UNLOCK(OPX_IS_CTX_SHARING_ENABLED, opx_ep->tx);
			return -FI_EAGAIN;
		}
	}

	struct fi_opx_reliability_tx_replay *replay;
	union fi_opx_reliability_tx_psn	    *psn_ptr;
	int64_t				     psn;

	psn = fi_opx_reliability_get_replay(&opx_ep->ep_fid, opx_ep->reli_service, params->slid, params->origin_rx,
					    &psn_ptr, &replay, params->reliability, hfi1_type);
	if (OFI_UNLIKELY(psn == -1)) {
		OPX_TRACER_TRACE(OPX_TRACER_END_EAGAIN, "SEND-RMA-CTS-HFI:%p", params->rma_req);
		FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA,
		       "===================================== RECV, HFI -- RMA RTS (EAGAIN psn/replay) (params=%p rzv_comp=%p context=%p)\n",
		       params, params->rma_req, params->rma_req->context);
		OPX_SHD_CTX_PIO_UNLOCK(OPX_IS_CTX_SHARING_ENABLED, opx_ep->tx);
		return -FI_EAGAIN;
	}

	assert(payload_bytes <= OPX_HFI1_PKT_SIZE);

	// The "memcopy first" code is here as an alternative to the more complicated
	// direct write to pio followed by memory copy of the reliability buffer
	uint64_t niov = params->niov << 48;
	uint64_t op64 = ((uint64_t) params->op) << 40;
	uint64_t dt64 = ((uint64_t) params->dt) << 32;
	if (hfi1_type & (OPX_HFI1_WFR | OPX_HFI1_MIXED_9B)) {
		const uint64_t pbc_dws = 2 + /* pbc */
					 2 + /* lrh */
					 3 + /* bth */
					 9 + /* kdeth; from "RcvHdrSize[i].HdrSize" CSR */
					 ((payload_bytes + 3) >> 2);
		const uint16_t lrh_dws =
			htons(pbc_dws - 2 +
			      1); /* (BE: LRH DW) does not include pbc (8 bytes), but does include icrc (4 bytes) */
		replay->scb.scb_9B.qw0 = opx_ep->rx->tx.cts_9B.qw0 | OPX_PBC_LEN(pbc_dws, hfi1_type) |
					 params->pbc_dlid | OPX_PBC_LOOPBACK(params->pbc_dlid, hfi1_type);
		replay->scb.scb_9B.hdr.qw_9B[0] =
			opx_ep->rx->tx.cts_9B.hdr.qw_9B[0] | lrh_dlid | ((uint64_t) lrh_dws << 32);
		replay->scb.scb_9B.hdr.qw_9B[1] = opx_ep->rx->tx.cts_9B.hdr.qw_9B[1] | bth_rx;
		replay->scb.scb_9B.hdr.qw_9B[2] = opx_ep->rx->tx.cts_9B.hdr.qw_9B[2] | psn;
		replay->scb.scb_9B.hdr.qw_9B[3] = opx_ep->rx->tx.cts_9B.hdr.qw_9B[3];
		replay->scb.scb_9B.hdr.qw_9B[4] =
			opx_ep->rx->tx.cts_9B.hdr.qw_9B[4] | niov | op64 | dt64 | (params->opcode);
		replay->scb.scb_9B.hdr.qw_9B[5] = (uint64_t) params->origin_rma_req;
		replay->scb.scb_9B.hdr.qw_9B[6] = (uint64_t) params->rma_req;
		FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA,
		       "replay->scb_9B.qw0 = %#lx pbc_dws = %ld lrh_dws = %d replay->scb_9B.hdr.lrh_9B.pktlen = %d\n",
		       replay->scb.scb_9B.qw0, pbc_dws, lrh_dws, replay->scb.scb_9B.hdr.lrh_9B.pktlen);

	} else {
		const uint64_t pbc_dws = 2 + /* pbc */
					 4 + /* lrh uncompressed */
					 3 + /* bth */
					 9 + /* kdeth; from "RcvHdrSize[i].HdrSize" CSR */
					 (((payload_bytes + 7) & -8) >> 2) + /* 16B is QW length/padded */
					 2;				     /* ICRC/tail */
		const uint16_t lrh_qws = (pbc_dws - 2) >> 1; /* (LRH QW) does not include pbc (8 bytes) */
		__attribute__((__unused__)) const uint64_t pbc_dlid = OPX_PBC_DLID(lrh_dlid, OPX_HFI1_CYR);
		assert(params->pbc_dlid == pbc_dlid);
		replay->scb.scb_16B.qw0 = opx_ep->rx->tx.cts_16B.qw0 | OPX_PBC_LEN(pbc_dws, OPX_HFI1_CYR) |
					  params->pbc_dlid | OPX_PBC_LOOPBACK(params->pbc_dlid, OPX_HFI1_CYR);
		replay->scb.scb_16B.hdr.qw_16B[0] =
			opx_ep->rx->tx.cts_16B.hdr.qw_16B[0] |
			((uint64_t) (lrh_dlid & OPX_LRH_JKR_16B_DLID_MASK_16B) << OPX_LRH_JKR_16B_DLID_SHIFT_16B) |
			((uint64_t) lrh_qws << 20);
		replay->scb.scb_16B.hdr.qw_16B[1] = opx_ep->rx->tx.cts_16B.hdr.qw_16B[1] |
						    ((uint64_t) ((lrh_dlid & OPX_LRH_JKR_16B_DLID20_MASK_16B) >>
								 OPX_LRH_JKR_16B_DLID20_SHIFT_16B)) |
						    (uint64_t) (bth_rx >> OPX_LRH_JKR_BTH_RX_ENTROPY_SHIFT_16B);

		replay->scb.scb_16B.hdr.qw_16B[2] = opx_ep->rx->tx.cts_16B.hdr.qw_16B[2] | bth_rx;
		replay->scb.scb_16B.hdr.qw_16B[3] = opx_ep->rx->tx.cts_16B.hdr.qw_16B[3] | psn;
		replay->scb.scb_16B.hdr.qw_16B[4] = opx_ep->rx->tx.cts_16B.hdr.qw_16B[4];
		replay->scb.scb_16B.hdr.qw_16B[5] =
			opx_ep->rx->tx.cts_16B.hdr.qw_16B[5] | niov | op64 | dt64 | params->opcode;
		replay->scb.scb_16B.hdr.qw_16B[6] = (uint64_t) params->origin_rma_req;
		replay->scb.scb_16B.hdr.qw_16B[7] = (uint64_t) params->rma_req;
		FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA,
		       "replay->scb_16B.qw0 = %#lx pbc_dws = %ld lrh_qws = %d replay->scb_16B.hdr.lrh_16B.pktlen = %d\n",
		       replay->scb.scb_16B.qw0, pbc_dws, lrh_qws, replay->scb.scb_16B.hdr.lrh_16B.pktlen);
	}

	union fi_opx_hfi1_packet_payload *const tx_payload = (union fi_opx_hfi1_packet_payload *) replay->payload;
	assert(((uint8_t *) tx_payload) == ((uint8_t *) &replay->data));

	for (int i = 0; i < params->niov; i++) {
		tx_payload->cts.iov[i] = params->dput_iov[i];
	}

	fi_opx_reliability_service_do_replay(opx_ep, opx_ep->reli_service, replay);
	OPX_SHD_CTX_PIO_UNLOCK(OPX_IS_CTX_SHARING_ENABLED, opx_ep->tx);
	fi_opx_reliability_service_replay_register_no_update(opx_ep->reli_service, psn_ptr, replay, params->reliability,
							     hfi1_type);
	OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "SEND-RMA-CTS-HFI:%p", params->rma_req);
	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA,
	       "===================================== RECV, HFI -- RMA RTS (end) (params=%p rma_req=%p context=%p)\n",
	       params, params->rma_req, params->rma_req->context);
	return FI_SUCCESS;
}

void opx_hfi1_rx_rma_rts(struct fi_opx_ep *opx_ep, const union opx_hfi1_packet_hdr *const hdr,
			 const void *const payload, const uint64_t niov, uintptr_t origin_rma_req,
			 struct opx_context *const target_context, const uintptr_t dst_vaddr,
			 const enum fi_hmem_iface dst_iface, const uint64_t dst_device, const uint64_t dst_handle,
			 const union opx_hfi1_dput_iov *src_iovs, const unsigned is_shm,
			 const enum ofi_reliability_kind reliability, const enum opx_hfi1_type hfi1_type)
{
	OPX_TRACER_TRACE(OPX_TRACER_BEGIN, "RECV-RMA-RTS-HFI:%ld", hdr->qw_9B[6]);
	union fi_opx_hfi1_deferred_work *work = ofi_buf_alloc(opx_ep->tx->work_pending_pool);
	assert(work != NULL);
	struct opx_hfi1_rma_rts_params *params = &work->rma_rts;
	params->work_elem.slist_entry.next     = NULL;
	params->opx_ep			       = opx_ep;

	opx_lid_t lid;
	if (hfi1_type & OPX_HFI1_WFR) {
		lid		 = (opx_lid_t) __be16_to_cpu24((__be16) hdr->lrh_9B.slid);
		params->lrh_dlid = (hdr->lrh_9B.qw[0] & 0xFFFF000000000000ul) >> 32;
		params->pbc_dlid = OPX_PBC_DLID(lid, OPX_HFI1_WFR);
	} else if (hfi1_type & OPX_HFI1_MIXED_9B) {
		lid		 = (opx_lid_t) __be16_to_cpu24((__be16) hdr->lrh_9B.slid);
		params->lrh_dlid = (hdr->lrh_9B.qw[0] & 0xFFFF000000000000ul) >> 32;
		params->pbc_dlid = OPX_PBC_DLID(lid, OPX_HFI1_MIXED_9B);
	} else if (hfi1_type & OPX_HFI1_JKR) {
		lid		 = (opx_lid_t) __le24_to_cpu(hdr->lrh_16B.slid20 << 20 | hdr->lrh_16B.slid);
		params->lrh_dlid = lid; // Send CTS to the SLID that sent RTS
		params->pbc_dlid = OPX_PBC_DLID(lid, OPX_HFI1_JKR);
	} else {
		lid		 = (opx_lid_t) __le24_to_cpu(hdr->lrh_16B.slid20 << 20 | hdr->lrh_16B.slid);
		params->lrh_dlid = lid; // Send CTS to the SLID that sent RTS
		params->pbc_dlid = OPX_PBC_DLID(lid, OPX_HFI1_CYR);
	}
	params->slid = lid;

	assert(niov <= MIN(FI_OPX_MAX_HMEM_IOV, FI_OPX_MAX_DPUT_IOV));
	params->niov				   = niov;
	const union opx_hfi1_dput_iov *src_iov	   = src_iovs;
	uint64_t		       rbuf_offset = 0;
	for (int i = 0; i < niov; i++) {
		params->dput_iov[i].sbuf	= src_iov->sbuf;
		params->dput_iov[i].sbuf_iface	= src_iov->sbuf_iface;
		params->dput_iov[i].sbuf_device = src_iov->sbuf_device;
		params->dput_iov[i].sbuf_handle = src_iov->sbuf_handle;
		params->dput_iov[i].rbuf	= dst_vaddr + rbuf_offset;
		params->dput_iov[i].rbuf_iface	= dst_iface;
		params->dput_iov[i].rbuf_device = dst_device;
		params->dput_iov[i].bytes	= src_iov->bytes;
		rbuf_offset += src_iov->bytes;
		++src_iov;
	}
	target_context->len = target_context->byte_counter = rbuf_offset;

	if (is_shm) {
		params->work_elem.work_fn   = opx_hfi1_rx_rma_rts_send_cts_shm;
		params->work_elem.work_type = OPX_WORK_TYPE_SHM;
		params->target_hfi_unit	    = fi_opx_hfi1_get_lid_local_unit(params->slid);
	} else {
		params->work_elem.work_fn   = opx_hfi1_rx_rma_rts_send_cts;
		params->work_elem.work_type = OPX_WORK_TYPE_PIO;
		params->target_hfi_unit	    = 0xFF;
	}
	params->work_elem.completion_action = NULL;
	params->work_elem.payload_copy	    = NULL;
	params->work_elem.complete	    = false;

	params->origin_rx	= FI_OPX_HFI1_PACKET_ORIGIN_RX(hdr);
	params->u32_extended_rx = fi_opx_ep_get_u32_extended_rx(opx_ep, is_shm, params->origin_rx);
	params->reliability	= reliability;
	params->is_shm		= is_shm;
	params->opcode		= FI_OPX_HFI_DPUT_OPCODE_PUT_CQ;
	params->dt		= hdr->rma_rts.dt;
	params->op		= hdr->rma_rts.op;

	params->origin_rma_req	     = (struct fi_opx_rma_request *) origin_rma_req;
	params->rma_req		     = ofi_buf_alloc(opx_ep->tx->rma_request_pool);
	params->rma_req->context     = target_context;
	params->rma_req->hmem_device = dst_device;
	params->rma_req->hmem_iface  = dst_iface;
	params->rma_req->hmem_handle = dst_handle;

	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA, "is_shm=%u niov=%lu opcode=%u\n", is_shm, niov, params->opcode);

	int rc = params->work_elem.work_fn(work);
	if (rc == FI_SUCCESS) {
		OPX_BUF_FREE(work);
		OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "RECV-RMA-RTS-HFI:%ld", hdr->qw_9B[6]);
		FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA, "FI_SUCCESS\n");
		return;
	}
	assert(rc == -FI_EAGAIN);
	/* Try again later*/
	assert(work->work_elem.slist_entry.next == NULL);
	slist_insert_tail(&work->work_elem.slist_entry, &opx_ep->tx->work_pending[params->work_elem.work_type]);
	OPX_TRACER_TRACE(OPX_TRACER_END_EAGAIN, "RECV-RMA-RTS-HFI:%ld", hdr->qw_9B[6]);
	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA, "FI_EAGAIN\n");
}

int opx_hfi1_tx_rma_rts(union fi_opx_hfi1_deferred_work *work)
{
	struct opx_hfi1_rma_rts_params *params	 = &work->rma_rts;
	struct fi_opx_ep	       *opx_ep	 = params->opx_ep;
	const uint64_t			lrh_dlid = params->lrh_dlid;
	const uint64_t			bth_rx	 = ((uint64_t) params->dest_rx) << OPX_BTH_SUBCTXT_RX_SHIFT;

	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA,
	       "===================================== SEND, HFI -- RMA RTS (begin) (params=%p origin_rma_req=%p cc=%p)\n",
	       params, params->origin_rma_req, params->origin_rma_req->cc);
	assert(params->origin_rma_req->cc->byte_counter >= params->dput_iov[0].bytes);
	OPX_TRACER_TRACE(OPX_TRACER_BEGIN, "SEND-RMA-RTS-HFI:%p", params->origin_rma_req);

	const uint64_t payload_bytes = (params->niov * sizeof(union opx_hfi1_dput_iov));
	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA, "payload_bytes = %ld\n", payload_bytes);

	OPX_SHD_CTX_PIO_LOCK(OPX_IS_CTX_SHARING_ENABLED, opx_ep->tx);

	union fi_opx_hfi1_pio_state pio_state		 = *opx_ep->tx->pio_state;
	const uint16_t		    total_credits_needed = 1 +		   /* packet header */
					      ((payload_bytes + 63) >> 6); /* payload blocks needed */
	uint64_t total_credits_available =
		FI_OPX_HFI1_AVAILABLE_CREDITS(pio_state, &opx_ep->tx->force_credit_return, total_credits_needed);

	if (OFI_UNLIKELY(total_credits_available < total_credits_needed)) {
		fi_opx_compiler_msync_writes();
		FI_OPX_HFI1_UPDATE_CREDITS(pio_state, opx_ep->tx->pio_credits_addr);
		total_credits_available	   = FI_OPX_HFI1_AVAILABLE_CREDITS(pio_state, &opx_ep->tx->force_credit_return,
									   total_credits_needed);
		opx_ep->tx->pio_state->qw0 = pio_state.qw0;
		if (total_credits_available < total_credits_needed) {
			FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA,
			       "===================================== SEND, HFI -- RMA RTS (EAGAIN credits) (params=%p origin_rma_req=%p cc=%p)\n",
			       params, params->origin_rma_req, params->origin_rma_req->cc);
			OPX_TRACER_TRACE(OPX_TRACER_END_EAGAIN, "SEND-RMA-RTS-HFI:%p", params->origin_rma_req);
			OPX_SHD_CTX_PIO_UNLOCK(OPX_IS_CTX_SHARING_ENABLED, opx_ep->tx);
			return -FI_EAGAIN;
		}
	}

	struct fi_opx_reliability_tx_replay *replay;
	union fi_opx_reliability_tx_psn	    *psn_ptr;
	int64_t				     psn;

	psn = fi_opx_reliability_get_replay(&opx_ep->ep_fid, opx_ep->reli_service, params->slid, params->dest_rx,
					    &psn_ptr, &replay, params->reliability, OPX_SW_HFI1_TYPE);
	if (OFI_UNLIKELY(psn == -1)) {
		FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA,
		       "===================================== SEND, HFI -- RMA RTS (EAGAIN psn/replay) (params=%p origin_rma_req=%p cc=%p) opcode=%d\n",
		       params, params->origin_rma_req, params->origin_rma_req->cc, params->opcode);
		OPX_TRACER_TRACE(OPX_TRACER_END_EAGAIN, "SEND-RMA-RTS-HFI:%p", params->origin_rma_req);
		OPX_SHD_CTX_PIO_UNLOCK(OPX_IS_CTX_SHARING_ENABLED, opx_ep->tx);
		return -FI_EAGAIN;
	}

	assert(payload_bytes <= OPX_HFI1_PKT_SIZE);

	const enum opx_hfi1_type hfi1_type = OPX_SW_HFI1_TYPE;

	// The "memcopy first" code is here as an alternative to the more complicated
	// direct write to pio followed by memory copy of the reliability buffer

	uint64_t cq_data = ((uint64_t) params->data) << 32;
	uint64_t niov	 = params->niov << 48;
	uint64_t op64	 = ((uint64_t) params->op) << 40;
	uint64_t dt64	 = ((uint64_t) params->dt) << 32;
	assert(params->dt == (FI_VOID - 1) || params->dt < FI_DATATYPE_LAST);
	if (hfi1_type & (OPX_HFI1_WFR | OPX_HFI1_MIXED_9B)) {
		const uint64_t pbc_dws = 2 +			     /* pbc */
					 2 +			     /* lhr */
					 3 +			     /* bth */
					 9 +			     /* kdeth; from "RcvHdrSize[i].HdrSize" CSR */
					 ((payload_bytes + 3) >> 2); /* payload dws for rma data */

		const uint16_t lrh_dws =
			htons(pbc_dws - 2 +
			      1); /* (BE: LRH DW) does not include pbc (8 bytes), but does include icrc (4 bytes) */

		replay->scb.scb_9B.qw0 = opx_ep->rx->tx.rma_rts_9B.qw0 | OPX_PBC_LEN(pbc_dws, hfi1_type) |
					 params->pbc_dlid | OPX_PBC_LOOPBACK(params->pbc_dlid, hfi1_type);
		replay->scb.scb_9B.hdr.qw_9B[0] =
			opx_ep->rx->tx.rma_rts_9B.hdr.qw_9B[0] | lrh_dlid | ((uint64_t) lrh_dws << 32);
		replay->scb.scb_9B.hdr.qw_9B[1] = opx_ep->rx->tx.rma_rts_9B.hdr.qw_9B[1] | bth_rx;
		replay->scb.scb_9B.hdr.qw_9B[2] = opx_ep->rx->tx.rma_rts_9B.hdr.qw_9B[2] | psn;
		replay->scb.scb_9B.hdr.qw_9B[3] = opx_ep->rx->tx.rma_rts_9B.hdr.qw_9B[3] | cq_data;
		replay->scb.scb_9B.hdr.qw_9B[4] =
			opx_ep->rx->tx.rma_rts_9B.hdr.qw_9B[4] | niov | op64 | dt64 | params->opcode;
		replay->scb.scb_9B.hdr.qw_9B[5] = params->key;
		replay->scb.scb_9B.hdr.qw_9B[6] = (uint64_t) params->origin_rma_req;
		FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA,
		       "replay->scb_9B.qw0 = %#lx pbc_dws = %ld lrh_dws = %d replay->scb_9B.hdr.lrh_9B.pktlen = %d \n",
		       replay->scb.scb_9B.qw0, pbc_dws, lrh_dws, replay->scb.scb_9B.hdr.lrh_9B.pktlen);
	} else {
		const uint64_t pbc_dws = 2 + /* pbc */
					 4 + /* lrh uncompressed */
					 3 + /* bth */
					 9 + /* kdeth; from "RcvHdrSize[i].HdrSize" CSR */
					 (((payload_bytes + 7) & -8) >> 2) + /* 16B is QW length/padded */
					 2;				     /* ICRC/tail */
		const uint16_t lrh_qws	= (pbc_dws - 2) >> 1; /* (LRH QW) does not include pbc (8 bytes) */
		replay->scb.scb_16B.qw0 = opx_ep->rx->tx.rma_rts_16B.qw0 | OPX_PBC_LEN(pbc_dws, OPX_HFI1_CYR) |
					  params->pbc_dlid | OPX_PBC_LOOPBACK(params->pbc_dlid, OPX_HFI1_CYR);
		replay->scb.scb_16B.hdr.qw_16B[0] = opx_ep->rx->tx.rma_rts_16B.hdr.qw_16B[0] |
						    ((uint64_t) (params->lrh_dlid & OPX_LRH_JKR_16B_DLID_MASK_16B)
						     << OPX_LRH_JKR_16B_DLID_SHIFT_16B) |
						    ((uint64_t) lrh_qws << 20);
		replay->scb.scb_16B.hdr.qw_16B[1] = opx_ep->rx->tx.rma_rts_16B.hdr.qw_16B[1] |
						    ((uint64_t) ((params->lrh_dlid & OPX_LRH_JKR_16B_DLID20_MASK_16B) >>
								 OPX_LRH_JKR_16B_DLID20_SHIFT_16B)) |
						    (uint64_t) (bth_rx >> OPX_LRH_JKR_BTH_RX_ENTROPY_SHIFT_16B);

		replay->scb.scb_16B.hdr.qw_16B[2] = opx_ep->rx->tx.rma_rts_16B.hdr.qw_16B[2] | bth_rx;
		replay->scb.scb_16B.hdr.qw_16B[3] = opx_ep->rx->tx.rma_rts_16B.hdr.qw_16B[3] | psn;
		replay->scb.scb_16B.hdr.qw_16B[4] = opx_ep->rx->tx.rma_rts_16B.hdr.qw_16B[4] | cq_data;
		replay->scb.scb_16B.hdr.qw_16B[5] =
			opx_ep->rx->tx.rma_rts_16B.hdr.qw_16B[5] | niov | op64 | dt64 | params->opcode;
		replay->scb.scb_16B.hdr.qw_16B[6] = params->key;
		replay->scb.scb_16B.hdr.qw_16B[7] = (uint64_t) params->origin_rma_req;
		FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA,
		       "replay->scb_16B.qw0 = %#lx pbc_dws = %ld lrh_qws = %d replay->scb_16B.hdr.lrh_16B.pktlen = %d\n",
		       replay->scb.scb_16B.qw0, pbc_dws, lrh_qws, replay->scb.scb_16B.hdr.lrh_16B.pktlen);
	}

	union fi_opx_hfi1_packet_payload *const tx_payload = (union fi_opx_hfi1_packet_payload *) replay->payload;
	assert(((uint8_t *) tx_payload) == ((uint8_t *) &replay->data));

	for (int i = 0; i < params->niov; i++) {
		tx_payload->rma_rts.iov[i] = params->dput_iov[i];
	}

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		     "fi_opx_reliability_service_do_replay opx_ep->reli_service %p, replay %p\n", opx_ep->reli_service,
		     replay);
	fi_opx_reliability_service_do_replay(opx_ep, opx_ep->reli_service, replay);
	OPX_SHD_CTX_PIO_UNLOCK(OPX_IS_CTX_SHARING_ENABLED, opx_ep->tx);
	if (hfi1_type & (OPX_HFI1_WFR | OPX_HFI1_MIXED_9B)) {
		fi_opx_reliability_service_replay_register_no_update(opx_ep->reli_service, psn_ptr, replay,
								     params->reliability, OPX_HFI1_WFR);
	} else if (hfi1_type & OPX_HFI1_JKR) {
		fi_opx_reliability_service_replay_register_no_update(opx_ep->reli_service, psn_ptr, replay,
								     params->reliability, OPX_HFI1_JKR);
	} else {
		fi_opx_reliability_service_replay_register_no_update(opx_ep->reli_service, psn_ptr, replay,
								     params->reliability, OPX_HFI1_CYR);
	}

	OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "SEND-RMA-RTS-HFI:%p", params->origin_rma_req);
	FI_DBG_TRACE(
		fi_opx_global.prov, FI_LOG_EP_DATA,
		"===================================== SEND, HFI -- RMA RTS (end) (params=%p origin_rma_req=%p cc=%p)\n",
		params, params->origin_rma_req, params->origin_rma_req->cc);

	params->work_elem.complete = true;
	return FI_SUCCESS;
}

int opx_hfi1_tx_rma_rts_shm(union fi_opx_hfi1_deferred_work *work)
{
	struct opx_hfi1_rma_rts_params *params	 = &work->rma_rts;
	struct fi_opx_ep	       *opx_ep	 = params->opx_ep;
	const uint64_t			lrh_dlid = params->lrh_dlid;
	const uint64_t			bth_rx	 = ((uint64_t) params->dest_rx) << OPX_BTH_SUBCTXT_RX_SHIFT;
	uint64_t			pos;

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		     "===================================== SEND, SHM -- RENDEZVOUS RMA (begin) context %p\n", NULL);
	OPX_TRACER_TRACE(OPX_TRACER_BEGIN, "SEND-RZV-RMA-SHM");

	/* Possible SHM connections required for certain applications (i.e., DAOS)
	 * exceeds the max value of the legacy origin_subctxt_rx field.  Use u32_extended field.
	 */
	// TODO: Pass in subctxt
	ssize_t rc =
		fi_opx_shm_dynamic_tx_connect(OPX_SHM_TRUE, opx_ep, params->u32_extended_rx, params->target_hfi_unit);

	if (OFI_UNLIKELY(rc)) {
		return -FI_EAGAIN;
	}

	// TODO pass in subctxt
	union opx_hfi1_packet_hdr *const hdr = opx_shm_tx_next(
		&opx_ep->tx->shm, params->target_hfi_unit, params->dest_rx, &pos, opx_ep->daos_info.hfi_rank_enabled,
		params->u32_extended_rx, opx_ep->daos_info.rank_inst, &rc);

	if (!hdr) {
		return rc;
	}

	/* Note that we do not set stl.hdr.lrh.pktlen here (usually lrh_dws << 32),
	   because this is shm and since it's a RTS packet, lrh.pktlen
	   isn't used/needed */

	uint64_t cq_data = ((uint64_t) params->data) << 32;
	uint64_t niov	 = params->niov << 48;
	uint64_t op64	 = ((uint64_t) params->op) << 40;
	uint64_t dt64	 = ((uint64_t) params->dt) << 32;
	assert(params->dt == (FI_VOID - 1) || params->dt < FI_DATATYPE_LAST);

	if (OPX_SW_HFI1_TYPE & (OPX_HFI1_WFR | OPX_HFI1_MIXED_9B)) {
		hdr->qw_9B[0] = opx_ep->rx->tx.rma_rts_9B.hdr.qw_9B[0] | lrh_dlid;
		hdr->qw_9B[1] = opx_ep->rx->tx.rma_rts_9B.hdr.qw_9B[1] | bth_rx;
		hdr->qw_9B[2] = opx_ep->rx->tx.rma_rts_9B.hdr.qw_9B[2];
		hdr->qw_9B[3] = opx_ep->rx->tx.rma_rts_9B.hdr.qw_9B[3] | cq_data;
		hdr->qw_9B[4] = opx_ep->rx->tx.rma_rts_9B.hdr.qw_9B[4] | niov | op64 | dt64 | params->opcode;
		hdr->qw_9B[5] = params->key;
		hdr->qw_9B[6] = (uint64_t) params->origin_rma_req;
	} else {
		hdr->qw_16B[0] =
			opx_ep->rx->tx.rma_rts_16B.hdr.qw_16B[0] |
			((uint64_t) ((lrh_dlid & OPX_LRH_JKR_16B_DLID_MASK_16B) << OPX_LRH_JKR_16B_DLID_SHIFT_16B));
		hdr->qw_16B[1] = opx_ep->rx->tx.rma_rts_16B.hdr.qw_16B[1] |
				 ((uint64_t) ((lrh_dlid & OPX_LRH_JKR_16B_DLID20_MASK_16B) >>
					      OPX_LRH_JKR_16B_DLID20_SHIFT_16B)) |
				 (uint64_t) (bth_rx >> OPX_LRH_JKR_BTH_RX_ENTROPY_SHIFT_16B);
		hdr->qw_16B[2] = opx_ep->rx->tx.rma_rts_16B.hdr.qw_16B[2] | bth_rx;
		hdr->qw_16B[3] = opx_ep->rx->tx.rma_rts_16B.hdr.qw_16B[3];
		hdr->qw_16B[4] = opx_ep->rx->tx.rma_rts_16B.hdr.qw_16B[4] | cq_data;
		hdr->qw_16B[5] = opx_ep->rx->tx.rma_rts_16B.hdr.qw_16B[5] | niov | op64 | dt64 | params->opcode;
		hdr->qw_16B[6] = params->key;
		hdr->qw_16B[7] = (uint64_t) params->origin_rma_req;
	}
	union fi_opx_hfi1_packet_payload *const tx_payload = (union fi_opx_hfi1_packet_payload *) (hdr + 1);

	uintptr_t vaddr_with_offset = 0; /* receive buffer virtual address */
	for (int i = 0; i < params->niov; i++) {
		tx_payload->cts.iov[i] = params->dput_iov[i];
		tx_payload->cts.iov[i].rbuf += vaddr_with_offset;
		vaddr_with_offset += params->dput_iov[i].bytes;
	}

	opx_shm_tx_advance(&opx_ep->tx->shm, (void *) hdr, pos);

	OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "SEND-RZV-RTS-SHM");
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		     "===================================== SEND, SHM -- RENDEZVOUS RTS (end) context %p\n", NULL);

	params->work_elem.complete = true;
	return FI_SUCCESS;
}

__OPX_FORCE_INLINE__
int fi_opx_hfi1_do_dput(union fi_opx_hfi1_deferred_work *work, const enum opx_hfi1_type hfi1_type)
{
	struct fi_opx_hfi1_dput_params	    *params		       = &work->dput;
	struct fi_opx_ep		    *opx_ep		       = params->opx_ep;
	const uint16_t			     subctxt_rx		       = params->origin_rx;
	const uint32_t			     niov		       = params->niov;
	const union opx_hfi1_dput_iov *const dput_iov		       = params->dput_iov;
	const uintptr_t			     target_byte_counter_vaddr = params->target_byte_counter_vaddr;
	uint64_t			    *origin_byte_counter       = params->origin_byte_counter;
	uint64_t			     key		       = params->key;
	struct fi_opx_completion_counter    *cc			       = params->cc;
	uint64_t			     op64		       = params->op;
	uint64_t			     dt64		       = params->dt;
	uint32_t			     opcode		       = params->opcode;
	const unsigned			     is_shm		       = params->is_shm;
	const enum ofi_reliability_kind	     reliability	       = params->reliability;
	/* use the slid from the lrh header of the incoming packet
	 * as the dlid for the lrh header of the outgoing packet */
	const uint64_t lrh_dlid	      = params->lrh_dlid;
	const uint64_t bth_subctxt_rx = ((uint64_t) subctxt_rx) << OPX_BTH_SUBCTXT_RX_SHIFT;

	enum fi_hmem_iface cbuf_iface  = params->compare_iov.iface;
	uint64_t	   cbuf_device = params->compare_iov.device;

	assert((opx_ep->tx->pio_max_eager_tx_bytes & 0x3fu) == 0);
	unsigned    i;
	const void *sbuf_start = params->src_base_addr;

	/* Note that lrh_dlid is just the version of params->slid shifted so
	   that it can be OR'd into the correct position in the packet header */
	if (hfi1_type & (OPX_HFI1_WFR | OPX_HFI1_MIXED_9B)) {
		assert(__cpu24_to_be16(params->slid) == (lrh_dlid >> 16));
	} else {
		assert(params->slid == lrh_dlid);
	}

	uint64_t max_bytes_per_packet;

	ssize_t rc;
	if (is_shm) {
		/* Possible SHM connections required for certain applications (i.e., DAOS)
		 * exceeds the max value of the legacy u8_origin_rx field.  Use u32_extended field.
		 */
		rc = fi_opx_shm_dynamic_tx_connect(params->is_shm, opx_ep, params->origin_rx, params->target_hfi_unit);

		if (OFI_UNLIKELY(rc)) {
			return -FI_EAGAIN;
		}

		max_bytes_per_packet = OPX_HFI1_PKT_SIZE;
	} else {
		max_bytes_per_packet = opx_ep->tx->pio_flow_eager_tx_bytes;
	}

	assert(((opcode == FI_OPX_HFI_DPUT_OPCODE_ATOMIC_FETCH ||
		 opcode == FI_OPX_HFI_DPUT_OPCODE_ATOMIC_COMPARE_FETCH) &&
		params->payload_bytes_for_iovec == sizeof(struct fi_opx_hfi1_dput_fetch)) ||
	       (opcode != FI_OPX_HFI_DPUT_OPCODE_ATOMIC_FETCH &&
		opcode != FI_OPX_HFI_DPUT_OPCODE_ATOMIC_COMPARE_FETCH && params->payload_bytes_for_iovec == 0));

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		     "===================================== SEND DPUT, %s opcode %d -- (begin)\n",
		     is_shm ? "SHM" : "HFI", opcode);
	OPX_TRACER_TRACE(OPX_TRACER_BEGIN, "SEND-DPUT-%s", is_shm ? "SHM" : "HFI");

	for (i = params->cur_iov; i < niov; ++i) {
		uint8_t *sbuf =
			(uint8_t *) ((uintptr_t) sbuf_start + (uintptr_t) dput_iov[i].sbuf + params->bytes_sent);
		uintptr_t rbuf = dput_iov[i].rbuf + params->bytes_sent;

		enum fi_hmem_iface sbuf_iface  = dput_iov[i].sbuf_iface;
		uint64_t	   sbuf_device = dput_iov[i].sbuf_device;
		uint64_t	   sbuf_handle = dput_iov[i].sbuf_handle;

		uint64_t bytes_to_send = dput_iov[i].bytes - params->bytes_sent;
		while (bytes_to_send > 0) {
			uint64_t bytes_to_send_this_packet;
			uint64_t blocks_to_send_in_this_packet;
			uint64_t pbc_dws;
			uint16_t lrh_dws;
			if (hfi1_type & (OPX_HFI1_WFR | OPX_HFI1_MIXED_9B)) {
				bytes_to_send_this_packet =
					MIN(bytes_to_send + params->payload_bytes_for_iovec, max_bytes_per_packet);
				uint64_t tail_bytes	      = bytes_to_send_this_packet & 0x3Ful;
				blocks_to_send_in_this_packet = (bytes_to_send_this_packet >> 6) + (tail_bytes ? 1 : 0);
				pbc_dws			      = 2 + /* pbc */
					  2 +			    /* lrh */
					  3 +			    /* bth */
					  9 +			    /* kdeth; from "RcvHdrSize[i].HdrSize" CSR */
					  (blocks_to_send_in_this_packet << 4);
				lrh_dws = htons(pbc_dws - 2 + 1); /* (BE: LRH DW) does not include pbc (8 bytes), but
								     does include icrc (4 bytes) */
			} else {
				/* 1 QW for hdr that spills to 2nd cacheline + 1 QW for ICRC/tail */
				const uint64_t additional_hdr_tail_byte = 2 * 8;
				uint64_t       payload_n_additional_hdr_tail_bytes =
					(MIN(bytes_to_send + params->payload_bytes_for_iovec + additional_hdr_tail_byte,
					     max_bytes_per_packet));
				uint64_t tail_bytes = payload_n_additional_hdr_tail_bytes & 0x3Ful;
				blocks_to_send_in_this_packet =
					(payload_n_additional_hdr_tail_bytes >> 6) + (tail_bytes ? 1 : 0);
				bytes_to_send_this_packet =
					payload_n_additional_hdr_tail_bytes - additional_hdr_tail_byte;
				pbc_dws = 2 + /* pbc */
					  4 + /* lrh uncompressed */
					  3 + /* bth */
					  7 + /* kdeth */
					  (blocks_to_send_in_this_packet
					   << 4); // ICRC and the kdeth in the second cacheline are accounted for here
				lrh_dws = (pbc_dws - 2) >> 1; /* (LRH QW) does not include pbc (8 bytes) */
			}

			uint64_t bytes_sent;
			if (is_shm) {
				uint64_t		   pos;
				union opx_hfi1_packet_hdr *hdr =
					opx_shm_tx_next(&opx_ep->tx->shm, params->target_hfi_unit, subctxt_rx, &pos,
							opx_ep->daos_info.hfi_rank_enabled, params->u32_extended_rx,
							opx_ep->daos_info.rank_inst, &rc);

				if (!hdr) {
					return rc;
				}

				union fi_opx_hfi1_packet_payload *const tx_payload =
					(union fi_opx_hfi1_packet_payload *) (hdr + 1);

				if (hfi1_type & OPX_HFI1_JKR) {
					bytes_sent = opx_hfi1_dput_write_header_and_payload(
						opx_ep, hdr, tx_payload, opcode, 0, lrh_dws, op64, dt64, lrh_dlid,
						bth_subctxt_rx, bytes_to_send_this_packet, key,
						(const uint64_t) params->fetch_vaddr, target_byte_counter_vaddr,
						params->rma_request_vaddr, params->bytes_sent, &sbuf, sbuf_iface,
						sbuf_device, sbuf_handle, (uint8_t **) &params->compare_vaddr,
						cbuf_iface, cbuf_device, &rbuf, OPX_HFI1_JKR);
				} else if (hfi1_type & OPX_HFI1_CYR) {
					bytes_sent = opx_hfi1_dput_write_header_and_payload(
						opx_ep, hdr, tx_payload, opcode, 0, lrh_dws, op64, dt64, lrh_dlid,
						bth_subctxt_rx, bytes_to_send_this_packet, key,
						(const uint64_t) params->fetch_vaddr, target_byte_counter_vaddr,
						params->rma_request_vaddr, params->bytes_sent, &sbuf, sbuf_iface,
						sbuf_device, sbuf_handle, (uint8_t **) &params->compare_vaddr,
						cbuf_iface, cbuf_device, &rbuf, OPX_HFI1_CYR);
				} else {
					bytes_sent = opx_hfi1_dput_write_header_and_payload(
						opx_ep, hdr, tx_payload, opcode, 0, lrh_dws, op64, dt64, lrh_dlid,
						bth_subctxt_rx, bytes_to_send_this_packet, key,
						(const uint64_t) params->fetch_vaddr, target_byte_counter_vaddr,
						params->rma_request_vaddr, params->bytes_sent, &sbuf, sbuf_iface,
						sbuf_device, sbuf_handle, (uint8_t **) &params->compare_vaddr,
						cbuf_iface, cbuf_device, &rbuf, OPX_HFI1_WFR);
				}

				opx_shm_tx_advance(&opx_ep->tx->shm, (void *) hdr, pos);
			} else {
				OPX_SHD_CTX_PIO_LOCK(OPX_IS_CTX_SHARING_ENABLED, opx_ep->tx);

				union fi_opx_hfi1_pio_state pio_state = *opx_ep->tx->pio_state;

				const uint16_t credits_needed	       = blocks_to_send_in_this_packet + 1 /* header */;
				uint32_t       total_credits_available = FI_OPX_HFI1_AVAILABLE_CREDITS(
					      pio_state, &opx_ep->tx->force_credit_return, credits_needed);

				if (total_credits_available < (uint32_t) credits_needed) {
					fi_opx_compiler_msync_writes();
					FI_OPX_HFI1_UPDATE_CREDITS(pio_state, opx_ep->tx->pio_credits_addr);
					total_credits_available = FI_OPX_HFI1_AVAILABLE_CREDITS(
						pio_state, &opx_ep->tx->force_credit_return, credits_needed);
					if (total_credits_available < (uint32_t) credits_needed) {
						opx_ep->tx->pio_state->qw0 = pio_state.qw0;
						OPX_SHD_CTX_PIO_UNLOCK(OPX_IS_CTX_SHARING_ENABLED, opx_ep->tx);
						return -FI_EAGAIN;
					}
				}

				struct fi_opx_reliability_tx_replay *replay;
				union fi_opx_reliability_tx_psn	    *psn_ptr;
				int64_t				     psn;

				psn = fi_opx_reliability_get_replay(&opx_ep->ep_fid, opx_ep->reli_service, params->slid,
								    subctxt_rx, &psn_ptr, &replay, reliability,
								    hfi1_type);
				if (OFI_UNLIKELY(psn == -1)) {
					OPX_SHD_CTX_PIO_UNLOCK(OPX_IS_CTX_SHARING_ENABLED, opx_ep->tx);
					return -FI_EAGAIN;
				}

				assert(replay != NULL);
				union fi_opx_hfi1_packet_payload *replay_payload =
					(union fi_opx_hfi1_packet_payload *) replay->payload;
				assert(!replay->use_iov);
				assert(((uint8_t *) replay_payload) == ((uint8_t *) &replay->data));

				if (hfi1_type & OPX_HFI1_JKR) {
					replay->scb.scb_16B.qw0 =
						opx_ep->rx->tx.dput_16B.qw0 | OPX_PBC_LEN(pbc_dws, OPX_HFI1_JKR) |
						OPX_PBC_CR(opx_ep->tx->force_credit_return, OPX_HFI1_JKR) |
						params->pbc_dlid | OPX_PBC_LOOPBACK(params->pbc_dlid, OPX_HFI1_JKR);
					bytes_sent = opx_hfi1_dput_write_header_and_payload(
						opx_ep, &replay->scb.scb_16B.hdr, replay_payload, opcode, psn, lrh_dws,
						op64, dt64, lrh_dlid, bth_subctxt_rx, bytes_to_send_this_packet, key,
						(const uint64_t) params->fetch_vaddr, target_byte_counter_vaddr,
						params->rma_request_vaddr, params->bytes_sent, &sbuf, sbuf_iface,
						sbuf_device, sbuf_handle, (uint8_t **) &params->compare_vaddr,
						cbuf_iface, cbuf_device, &rbuf, OPX_HFI1_JKR);
				} else if (hfi1_type & OPX_HFI1_CYR) {
					replay->scb.scb_16B.qw0 =
						opx_ep->rx->tx.dput_16B.qw0 | OPX_PBC_LEN(pbc_dws, OPX_HFI1_CYR) |
						OPX_PBC_CR(opx_ep->tx->force_credit_return, OPX_HFI1_CYR) |
						params->pbc_dlid | OPX_PBC_LOOPBACK(params->pbc_dlid, OPX_HFI1_CYR);
					bytes_sent = opx_hfi1_dput_write_header_and_payload(
						opx_ep, &replay->scb.scb_16B.hdr, replay_payload, opcode, psn, lrh_dws,
						op64, dt64, lrh_dlid, bth_subctxt_rx, bytes_to_send_this_packet, key,
						(const uint64_t) params->fetch_vaddr, target_byte_counter_vaddr,
						params->rma_request_vaddr, params->bytes_sent, &sbuf, sbuf_iface,
						sbuf_device, sbuf_handle, (uint8_t **) &params->compare_vaddr,
						cbuf_iface, cbuf_device, &rbuf, OPX_HFI1_CYR);
				} else {
					replay->scb.scb_9B.qw0 =
						opx_ep->rx->tx.dput_9B.qw0 | OPX_PBC_LEN(pbc_dws, hfi1_type) |
						OPX_PBC_CR(opx_ep->tx->force_credit_return, hfi1_type) |
						params->pbc_dlid | OPX_PBC_LOOPBACK(params->pbc_dlid, hfi1_type);
					bytes_sent = opx_hfi1_dput_write_header_and_payload(
						opx_ep, &replay->scb.scb_9B.hdr, replay_payload, opcode, psn, lrh_dws,
						op64, dt64, lrh_dlid, bth_subctxt_rx, bytes_to_send_this_packet, key,
						(const uint64_t) params->fetch_vaddr, target_byte_counter_vaddr,
						params->rma_request_vaddr, params->bytes_sent, &sbuf, sbuf_iface,
						sbuf_device, sbuf_handle, (uint8_t **) &params->compare_vaddr,
						cbuf_iface, cbuf_device, &rbuf, OPX_HFI1_WFR);
				}

				FI_OPX_HFI1_CLEAR_CREDIT_RETURN(opx_ep);

				if (opcode == FI_OPX_HFI_DPUT_OPCODE_PUT || opcode == FI_OPX_HFI_DPUT_OPCODE_PUT_CQ) {
					if (hfi1_type & (OPX_HFI1_WFR | OPX_HFI1_MIXED_9B)) {
						if (bytes_to_send == bytes_sent) {
							/* This is the last packet to send for this PUT.
							Turn on the immediate ACK request bit so the
							user gets control of their buffer back ASAP */
							replay->scb.scb_9B.hdr.qw_9B[2] |= (uint64_t) htonl(0x80000000);
							replay->scb.scb_9B.hdr.dput.target.last_bytes =
								replay->scb.scb_9B.hdr.dput.target.bytes;
						}
						fi_opx_reliability_service_replay_register_with_update(
							opx_ep->reli_service, psn_ptr, replay, cc, bytes_sent,
							reliability, OPX_HFI1_WFR);
					} else if (hfi1_type & OPX_HFI1_JKR) {
						if (bytes_to_send == bytes_sent) {
							replay->scb.scb_16B.hdr.qw_16B[3] |=
								(uint64_t) htonl(0x80000000);
							replay->scb.scb_16B.hdr.dput.target.last_bytes =
								replay->scb.scb_16B.hdr.dput.target.bytes;
						}
						fi_opx_reliability_service_replay_register_with_update(
							opx_ep->reli_service, psn_ptr, replay, cc, bytes_sent,
							reliability, OPX_HFI1_JKR);
					} else {
						if (bytes_to_send == bytes_sent) {
							replay->scb.scb_16B.hdr.qw_16B[3] |=
								(uint64_t) htonl(0x80000000);
							replay->scb.scb_16B.hdr.dput.target.last_bytes =
								replay->scb.scb_16B.hdr.dput.target.bytes;
						}
						fi_opx_reliability_service_replay_register_with_update(
							opx_ep->reli_service, psn_ptr, replay, cc, bytes_sent,
							reliability, OPX_HFI1_CYR);
					}

					fi_opx_reliability_service_do_replay(opx_ep, opx_ep->reli_service, replay);
				} else {
					fi_opx_reliability_service_do_replay(opx_ep, opx_ep->reli_service, replay);
					fi_opx_compiler_msync_writes();

					if (hfi1_type & (OPX_HFI1_WFR | OPX_HFI1_MIXED_9B)) {
						fi_opx_reliability_service_replay_register_no_update(
							opx_ep->reli_service, psn_ptr, replay, reliability,
							OPX_HFI1_WFR);
					} else if (hfi1_type & OPX_HFI1_JKR) {
						fi_opx_reliability_service_replay_register_no_update(
							opx_ep->reli_service, psn_ptr, replay, reliability,
							OPX_HFI1_JKR);
					} else {
						fi_opx_reliability_service_replay_register_no_update(
							opx_ep->reli_service, psn_ptr, replay, reliability,
							OPX_HFI1_CYR);
					}
				}
				OPX_SHD_CTX_PIO_UNLOCK(OPX_IS_CTX_SHARING_ENABLED, opx_ep->tx);
			}

			bytes_to_send -= bytes_sent;
			params->bytes_sent += bytes_sent;

			if (origin_byte_counter) {
				*origin_byte_counter -= bytes_sent;
				assert(((int64_t) *origin_byte_counter) >= 0);
			}
		} /* while bytes_to_send */

		if ((opcode == FI_OPX_HFI_DPUT_OPCODE_PUT || opcode == FI_OPX_HFI_DPUT_OPCODE_PUT_CQ) &&
		    is_shm) { // RMA-type put, so send a ping/fence to better latency
			fi_opx_shm_write_fence(opx_ep, params->target_hfi_unit, subctxt_rx, lrh_dlid, cc,
					       params->bytes_sent, params->u32_extended_rx, hfi1_type);
		}

		OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "SEND-DPUT-%s", is_shm ? "SHM" : "HFI");
		FI_DBG_TRACE(
			fi_opx_global.prov, FI_LOG_EP_DATA,
			"===================================== SEND DPUT, %s finished IOV=%d bytes_sent=%ld -- (end)\n",
			is_shm ? "SHM" : "HFI", params->cur_iov, params->bytes_sent);

		params->bytes_sent = 0;
		params->cur_iov++;
	} /* for niov */

	params->work_elem.complete = true;
	return FI_SUCCESS;
}

int fi_opx_hfi1_do_dput_wfr(union fi_opx_hfi1_deferred_work *work)
{
	return fi_opx_hfi1_do_dput(work, OPX_HFI1_WFR);
}

int fi_opx_hfi1_do_dput_jkr(union fi_opx_hfi1_deferred_work *work)
{
	return fi_opx_hfi1_do_dput(work, OPX_HFI1_JKR);
}

int fi_opx_hfi1_do_dput_cyr(union fi_opx_hfi1_deferred_work *work)
{
	return fi_opx_hfi1_do_dput(work, OPX_HFI1_CYR);
}

int fi_opx_hfi1_do_dput_jkr_9B(union fi_opx_hfi1_deferred_work *work)
{
	return fi_opx_hfi1_do_dput(work, OPX_HFI1_MIXED_9B);
}
__OPX_FORCE_INLINE__
void fi_opx_hfi1_dput_copy_to_bounce_buf(uint32_t opcode, uint8_t *target_buf, uint8_t *source_buf,
					 uint8_t *compare_buf, void *fetch_vaddr, uintptr_t target_byte_counter_vaddr,
					 uint64_t buf_packet_bytes, uint64_t total_bytes, uint64_t bytes_sent,
					 enum fi_hmem_iface sbuf_iface, uint64_t sbuf_device,
					 enum fi_hmem_iface cbuf_iface, uint64_t cbuf_device)
{
	if (opcode == FI_OPX_HFI_DPUT_OPCODE_ATOMIC_FETCH) {
		while (total_bytes) {
			size_t dput_bytes = MIN(buf_packet_bytes, total_bytes);

			opx_hfi1_dput_write_payload_atomic_fetch((union fi_opx_hfi1_packet_payload *) target_buf,
								 dput_bytes, (const uint64_t) fetch_vaddr,
								 target_byte_counter_vaddr, bytes_sent, source_buf,
								 sbuf_iface, sbuf_device);

			target_buf += dput_bytes + sizeof(struct fi_opx_hfi1_dput_fetch);
			source_buf += dput_bytes;
			bytes_sent += dput_bytes;

			total_bytes -= dput_bytes;
		}
	} else if (opcode == FI_OPX_HFI_DPUT_OPCODE_ATOMIC_COMPARE_FETCH) {
		buf_packet_bytes >>= 1;
		while (total_bytes) {
			size_t dput_bytes      = MIN(buf_packet_bytes, total_bytes);
			size_t dput_bytes_half = dput_bytes >> 1;

			opx_hfi1_dput_write_payload_atomic_compare_fetch(
				(union fi_opx_hfi1_packet_payload *) target_buf, dput_bytes_half,
				(const uint64_t) fetch_vaddr, target_byte_counter_vaddr, bytes_sent, source_buf,
				sbuf_iface, sbuf_device, compare_buf, cbuf_iface, cbuf_device);

			target_buf += dput_bytes + sizeof(struct fi_opx_hfi1_dput_fetch);
			source_buf += dput_bytes_half;
			compare_buf += dput_bytes_half;
			bytes_sent += dput_bytes;

			total_bytes -= dput_bytes;
		}
	} else {
		assert(total_bytes <= FI_OPX_HFI1_SDMA_WE_BUF_LEN);
		OPX_HMEM_COPY_FROM(target_buf, source_buf, total_bytes, OPX_HMEM_NO_HANDLE,
				   OPX_HMEM_DEV_REG_THRESHOLD_NOT_SET, sbuf_iface, sbuf_device);
	}
}

__OPX_FORCE_INLINE__
int fi_opx_hfi1_do_dput_sdma(union fi_opx_hfi1_deferred_work *work, const enum opx_hfi1_type hfi1_type)
{
	struct fi_opx_hfi1_dput_params	    *params		       = &work->dput;
	struct fi_opx_ep		    *opx_ep		       = params->opx_ep;
	const uint16_t			     subctxt_rx		       = params->origin_rx;
	const uint32_t			     niov		       = params->niov;
	const union opx_hfi1_dput_iov *const dput_iov		       = params->dput_iov;
	const uintptr_t			     target_byte_counter_vaddr = params->target_byte_counter_vaddr;
	uint64_t			     key		       = params->key;
	uint64_t			     op64		       = params->op;
	uint64_t			     dt64		       = params->dt;
	uint32_t			     opcode		       = params->opcode;
	const enum ofi_reliability_kind	     reliability	       = params->reliability;
	/* use the slid from the lrh header of the incoming packet
	 * as the dlid for the lrh header of the outgoing packet */

	const uint64_t lrh_dlid	      = params->lrh_dlid;
	const uint64_t bth_subctxt_rx = ((uint64_t) subctxt_rx) << OPX_BTH_SUBCTXT_RX_SHIFT;
	assert((opx_ep->tx->pio_max_eager_tx_bytes & 0x3fu) == 0);
	unsigned    i;
	const void *sbuf_start	       = params->src_base_addr;
	const bool  sdma_no_bounce_buf = params->sdma_no_bounce_buf;

	/* Note that lrh_dlid is just the version of params->slid shifted so
	   that it can be OR'd into the correct position in the packet header */
	if (hfi1_type & (OPX_HFI1_WFR | OPX_HFI1_MIXED_9B)) {
		assert(__cpu24_to_be16(params->slid) == (lrh_dlid >> 16));
		assert(opx_ep->rx->tx.rzv_dput_9B.hdr.lrh_9B.slid != params->slid);
	} else {
		assert(params->slid == lrh_dlid);
	}

	// We should never be in this function for shm ops
	assert(!params->is_shm);

	assert(((opcode == FI_OPX_HFI_DPUT_OPCODE_ATOMIC_FETCH ||
		 opcode == FI_OPX_HFI_DPUT_OPCODE_ATOMIC_COMPARE_FETCH) &&
		params->payload_bytes_for_iovec == sizeof(struct fi_opx_hfi1_dput_fetch)) ||
	       (opcode != FI_OPX_HFI_DPUT_OPCODE_ATOMIC_FETCH &&
		opcode != FI_OPX_HFI_DPUT_OPCODE_ATOMIC_COMPARE_FETCH && params->payload_bytes_for_iovec == 0));

	assert((opcode == FI_OPX_HFI_DPUT_OPCODE_PUT && params->sdma_no_bounce_buf) ||
	       (opcode == FI_OPX_HFI_DPUT_OPCODE_PUT_CQ && params->sdma_no_bounce_buf) ||
	       (opcode == FI_OPX_HFI_DPUT_OPCODE_GET && params->sdma_no_bounce_buf) ||
	       (opcode != FI_OPX_HFI_DPUT_OPCODE_PUT && opcode != FI_OPX_HFI_DPUT_OPCODE_PUT_CQ &&
		opcode != FI_OPX_HFI_DPUT_OPCODE_GET));

	uint64_t max_eager_bytes = opx_ep->tx->pio_max_eager_tx_bytes;
	uint64_t max_dput_bytes	 = max_eager_bytes - params->payload_bytes_for_iovec;

	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA,
	       "%p:===================================== SEND DPUT SDMA, opcode %X -- (begin)\n", params, opcode);
	OPX_TRACER_TRACE(OPX_TRACER_BEGIN, "SEND-DPUT-SDMA:%p:%ld", (void *) target_byte_counter_vaddr,
			 dput_iov[params->cur_iov].bytes);

	for (i = params->cur_iov; i < niov; ++i) {
		uint8_t *sbuf =
			(uint8_t *) ((uintptr_t) sbuf_start + (uintptr_t) dput_iov[i].sbuf + params->bytes_sent);
		uintptr_t rbuf = dput_iov[i].rbuf + params->bytes_sent;

		uint64_t bytes_to_send = dput_iov[i].bytes - params->bytes_sent;
		while (bytes_to_send > 0) {
			if (!fi_opx_hfi1_sdma_queue_has_room(opx_ep, OPX_SDMA_NONTID_IOV_COUNT)) {
				FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA,
				       "%p:===================================== SEND DPUT SDMA QUEUE FULL FI_EAGAIN\n",
				       params);
				OPX_TRACER_TRACE(OPX_TRACER_END_EAGAIN, "SEND-DPUT-SDMA:%p",
						 (void *) target_byte_counter_vaddr);
				return -FI_EAGAIN;
			}
			if (!params->sdma_we) {
				/* Get an SDMA work entry since we don't already have one */
				params->sdma_we = opx_sdma_get_new_work_entry(opx_ep, &params->sdma_reqs_used,
									      &params->sdma_reqs, params->sdma_we);
				if (!params->sdma_we) {
					FI_OPX_DEBUG_COUNTERS_INC_COND(
						(params->sdma_reqs_used < FI_OPX_HFI1_SDMA_MAX_WE_PER_REQ),
						opx_ep->debug_counters.sdma.eagain_sdma_we_none_free);
					FI_OPX_DEBUG_COUNTERS_INC_COND(
						(params->sdma_reqs_used == FI_OPX_HFI1_SDMA_MAX_WE_PER_REQ),
						opx_ep->debug_counters.sdma.eagain_sdma_we_max_used);
					FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA,
					       "%p:===================================== SEND DPUT SDMA, !WE FI_EAGAIN\n",
					       params);
					OPX_TRACER_TRACE(OPX_TRACER_END_EAGAIN, "SEND-DPUT-SDMA:%p",
							 (void *) target_byte_counter_vaddr);
					return -FI_EAGAIN;
				}
				assert(params->sdma_we->total_payload == 0);
				fi_opx_hfi1_sdma_init_we(params->sdma_we, params->cc, params->slid, subctxt_rx,
							 dput_iov[i].sbuf_iface, (int) dput_iov[i].sbuf_device);
			}
			assert(!fi_opx_hfi1_sdma_has_unsent_packets(params->sdma_we));

			/* The driver treats the offset as a 4-byte value, so we
			 * need to avoid sending a payload size that would wrap
			 * that in a single SDMA send */
			uintptr_t rbuf_wrap	= (rbuf + 0x100000000ul) & 0xFFFFFFFF00000000ul;
			uint64_t  sdma_we_bytes = MIN(bytes_to_send, (rbuf_wrap - rbuf));
			uint64_t  packet_count =
				(sdma_we_bytes / max_dput_bytes) + ((sdma_we_bytes % max_dput_bytes) ? 1 : 0);

			assert(packet_count > 0);

			/* In the unlikely event that we'll be sending any
			 * packet who's payload size is not a multiple of 8,
			 * we'll need to add padding, in which case we'll need
			 * to use a bounce buffer, regardless if we're
			 * doing delivery completion. This is because the
			 * SDMA engine requires the LRH DWs add up to exactly
			 * the number of bytes used to fill the packet. To do
			 * the padding, we'll copy the payload to the
			 * bounce buffer, and then add the necessary padding
			 * to the iovec length we pass to the SDMA engine.
			 * The extra pad bytes will be ignored by the receiver,
			 * since it uses the byte count in the DPUT header
			 * which will still be set correctly.
			 */
			bool need_padding = (sdma_we_bytes & 0x7ul) != 0;
			params->sdma_we->use_bounce_buf =
				(!sdma_no_bounce_buf || opcode == FI_OPX_HFI_DPUT_OPCODE_ATOMIC_FETCH ||
				 opcode == FI_OPX_HFI_DPUT_OPCODE_ATOMIC_COMPARE_FETCH || need_padding);

			const uint64_t max_pkts_per_req = params->sdma_we->use_bounce_buf ?
								  OPX_SDMA_MAX_PKTS_BOUNCE_BUF :
								  opx_ep->tx->sdma_max_pkts;
			packet_count			= MIN(packet_count, max_pkts_per_req);

			int32_t psns_avail = fi_opx_reliability_tx_available_psns(
				&opx_ep->ep_fid, opx_ep->reli_service, params->slid, params->origin_rx,
				&params->sdma_we->psn_ptr, packet_count, max_eager_bytes);

			if (psns_avail < (int64_t) packet_count) {
				FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.sdma.eagain_psn);
				FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA,
				       "%p:===================================== SEND DPUT SDMA, !PSN FI_EAGAIN\n",
				       params);
				OPX_TRACER_TRACE(OPX_TRACER_END_EAGAIN, "SEND-DPUT-SDMA:%p",
						 (void *) target_byte_counter_vaddr);
				return -FI_EAGAIN;
			}

			uint8_t *sbuf_tmp;
			bool	 replay_use_sdma;
			if (params->sdma_we->use_bounce_buf) {
				fi_opx_hfi1_dput_copy_to_bounce_buf(
					opcode, params->sdma_we->bounce_buf.buf, sbuf,
					(uint8_t *) params->compare_iov.buf, params->fetch_vaddr,
					params->target_byte_counter_vaddr, max_dput_bytes,
					MIN((packet_count * max_dput_bytes), sdma_we_bytes), params->bytes_sent,
					dput_iov[i].sbuf_iface, dput_iov[i].sbuf_device, params->compare_iov.iface,
					params->compare_iov.device);
				sbuf_tmp	= params->sdma_we->bounce_buf.buf;
				replay_use_sdma = false;
			} else {
				sbuf_tmp	= sbuf;
				replay_use_sdma = (dput_iov[i].sbuf_iface != FI_HMEM_SYSTEM);
			}
			// At this point, we have enough SDMA queue entries and PSNs
			// to send packet_count packets. The only limit now is how
			// many replays can we get.
			for (int p = 0; (p < packet_count) && sdma_we_bytes; ++p) {
				uint64_t packet_bytes =
					MIN(sdma_we_bytes, max_dput_bytes) + params->payload_bytes_for_iovec;
				assert(packet_bytes <= OPX_HFI1_PKT_SIZE);

				struct fi_opx_reliability_tx_replay *replay;
				replay = fi_opx_reliability_service_replay_allocate(opx_ep->reli_service, true);
				if (OFI_UNLIKELY(replay == NULL)) {
					FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA,
					       "%p:!REPLAY on packet %u out of %lu, params->sdma_we->num_packets %u\n",
					       params, p, packet_count, params->sdma_we->num_packets);
					break;
				}
				replay->use_sdma = replay_use_sdma;

				// Round packet_bytes up to the next multiple of 8,
				// then divide by 4 to get the correct number of dws.
				uint64_t payload_dws = ((packet_bytes + 7) & -8) >> 2;
				assert((payload_dws << 2) == packet_bytes || need_padding);
				uint64_t pbc_dws;
				uint16_t lrh_dws;
				uint64_t bytes_sent;
				if (hfi1_type & (OPX_HFI1_WFR | OPX_HFI1_MIXED_9B)) {
					pbc_dws = 2 + /* pbc */
						  2 + /* lrh */
						  3 + /* bth */
						  9 + /* kdeth; from "RcvHdrSize[i].HdrSize" CSR */
						  payload_dws;
					lrh_dws = htons(pbc_dws - 2 + 1); /* (BE: LRH DW) does not include pbc (8
									     bytes), but does include icrc (4 bytes) */
					replay->scb.scb_9B.qw0 = opx_ep->rx->tx.rzv_dput_9B.qw0 |
								 OPX_PBC_LEN(pbc_dws, hfi1_type) | params->pbc_dlid |
								 OPX_PBC_LOOPBACK(params->pbc_dlid, hfi1_type);
					bytes_sent = opx_hfi1_dput_write_header_and_iov(
						opx_ep, &replay->scb.scb_9B.hdr, replay->iov, opcode, lrh_dws, op64,
						dt64, lrh_dlid, bth_subctxt_rx, packet_bytes, key,
						(const uint64_t) params->fetch_vaddr, target_byte_counter_vaddr,
						params->rma_request_vaddr, params->bytes_sent, &sbuf_tmp,
						(uint8_t **) &params->compare_vaddr, &rbuf, OPX_HFI1_WFR);
				} else if (hfi1_type & OPX_HFI1_JKR) {
					pbc_dws = 2 + /* pbc */
						  4 + /* lrh uncompressed */
						  3 + /* bth */
						  9 + /* kdeth; from "RcvHdrSize[i].HdrSize" CSR */
						  2 + /* ICRC/tail */
						  payload_dws;
					lrh_dws = (pbc_dws - 2) >> 1; /* (LRH QW) does not include pbc (8 bytes) */
					replay->scb.scb_16B.qw0 =
						opx_ep->rx->tx.rzv_dput_16B.qw0 | OPX_PBC_LEN(pbc_dws, OPX_HFI1_JKR) |
						params->pbc_dlid | OPX_PBC_LOOPBACK(params->pbc_dlid, OPX_HFI1_JKR);
					bytes_sent = opx_hfi1_dput_write_header_and_iov(
						opx_ep, &replay->scb.scb_16B.hdr, replay->iov, opcode, lrh_dws, op64,
						dt64, lrh_dlid, bth_subctxt_rx, packet_bytes, key,
						(const uint64_t) params->fetch_vaddr, target_byte_counter_vaddr,
						params->rma_request_vaddr, params->bytes_sent, &sbuf_tmp,
						(uint8_t **) &params->compare_vaddr, &rbuf, OPX_HFI1_JKR);
				} else {
					pbc_dws = 2 + /* pbc */
						  4 + /* lrh uncompressed */
						  3 + /* bth */
						  9 + /* kdeth; from "RcvHdrSize[i].HdrSize" CSR */
						  2 + /* ICRC/tail */
						  payload_dws;
					lrh_dws = (pbc_dws - 2) >> 1; /* (LRH QW) does not include pbc (8 bytes) */
					replay->scb.scb_16B.qw0 =
						opx_ep->rx->tx.rzv_dput_16B.qw0 | OPX_PBC_LEN(pbc_dws, OPX_HFI1_CYR) |
						params->pbc_dlid | OPX_PBC_LOOPBACK(params->pbc_dlid, OPX_HFI1_CYR);
					bytes_sent = opx_hfi1_dput_write_header_and_iov(
						opx_ep, &replay->scb.scb_16B.hdr, replay->iov, opcode, lrh_dws, op64,
						dt64, lrh_dlid, bth_subctxt_rx, packet_bytes, key,
						(const uint64_t) params->fetch_vaddr, target_byte_counter_vaddr,
						params->rma_request_vaddr, params->bytes_sent, &sbuf_tmp,
						(uint8_t **) &params->compare_vaddr, &rbuf, OPX_HFI1_CYR);
				}

				params->cc->byte_counter += params->payload_bytes_for_iovec;
				fi_opx_hfi1_sdma_add_packet(params->sdma_we, replay, packet_bytes);

				bytes_to_send -= bytes_sent;
				sdma_we_bytes -= bytes_sent;
				params->bytes_sent += bytes_sent;
				params->origin_bytes_sent += bytes_sent;
				sbuf += bytes_sent;
			}

			// Must be we had trouble getting a replay buffer
			if (OFI_UNLIKELY(params->sdma_we->num_packets == 0)) {
				FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.sdma.eagain_replay);
				FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA,
				       "%p:===================================== SEND DPUT SDMA, !REPLAY FI_EAGAIN\n",
				       params);
				OPX_TRACER_TRACE(OPX_TRACER_END_EAGAIN, "SEND-DPUT-SDMA:%p",
						 (void *) target_byte_counter_vaddr);
				return -FI_EAGAIN;
			}

			if (hfi1_type & (OPX_HFI1_WFR | OPX_HFI1_MIXED_9B)) {
				opx_hfi1_sdma_flush(opx_ep, params->sdma_we, &params->sdma_reqs, 0, /* do not use tid */
						    NULL, 0, 0, 0, 0, reliability, OPX_HFI1_WFR);
			} else {
				opx_hfi1_sdma_flush(opx_ep, params->sdma_we, &params->sdma_reqs, 0, /* do not use tid */
						    NULL, 0, 0, 0, 0, reliability, OPX_HFI1_CYR);
			}
			params->sdma_we = NULL;

		} /* while bytes_to_send */

		FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA,
		       "%p:===================================== SEND DPUT SDMA, finished IOV=%d(%d) bytes_sent=%ld\n",
		       params, params->cur_iov, niov, params->bytes_sent);

		params->bytes_sent = 0;
		params->cur_iov++;
	} /* for niov */
	OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "SEND-DPUT-SDMA:%p", (void *) target_byte_counter_vaddr);
	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA,
	       "%p:===================================== SEND DPUT SDMA, exit (end)\n", params);

	// At this point, all SDMA WE should have succeeded sending, and only reside on the reqs list
	assert(params->sdma_we == NULL);
	assert(!slist_empty(&params->sdma_reqs));

	// If we're not doing delivery completion, the user's payload would have
	// been copied to bounce buffer(s), so at this point, it should be safe
	// for the user to alter the send buffer even though the send may still
	// be in progress.
	if (!params->sdma_no_bounce_buf) {
		assert(params->origin_byte_counter);
		assert((*params->origin_byte_counter) >= params->origin_bytes_sent);
		*params->origin_byte_counter -= params->origin_bytes_sent;
		params->origin_byte_counter = NULL;
	}
	params->work_elem.work_type = OPX_WORK_TYPE_LAST;
	params->work_elem.work_fn   = fi_opx_hfi1_dput_sdma_pending_completion;

	// The SDMA request has been queued for sending, but not actually sent
	// yet, so there's no point in checking for completion right away. Wait
	// until the next poll cycle.
	return -FI_EAGAIN;
}

int fi_opx_hfi1_do_dput_sdma_wfr(union fi_opx_hfi1_deferred_work *work)
{
	return fi_opx_hfi1_do_dput_sdma(work, OPX_HFI1_WFR);
}

int fi_opx_hfi1_do_dput_sdma_jkr(union fi_opx_hfi1_deferred_work *work)
{
	return fi_opx_hfi1_do_dput_sdma(work, OPX_HFI1_JKR);
}

int fi_opx_hfi1_do_dput_sdma_cyr(union fi_opx_hfi1_deferred_work *work)
{
	return fi_opx_hfi1_do_dput_sdma(work, OPX_HFI1_CYR);
}

int fi_opx_hfi1_do_dput_sdma_jkr_9B(union fi_opx_hfi1_deferred_work *work)
{
	return fi_opx_hfi1_do_dput_sdma(work, OPX_HFI1_MIXED_9B);
}

__OPX_FORCE_INLINE__
int fi_opx_hfi1_do_dput_sdma_tid(union fi_opx_hfi1_deferred_work *work, const enum opx_hfi1_type hfi1_type)
{
	struct fi_opx_hfi1_dput_params	    *params		       = &work->dput;
	struct fi_opx_ep		    *opx_ep		       = params->opx_ep;
	const uint16_t			     subctxt_rx		       = params->origin_rx;
	const uint32_t			     niov		       = params->niov;
	const union opx_hfi1_dput_iov *const dput_iov		       = params->dput_iov;
	const uintptr_t			     target_byte_counter_vaddr = params->target_byte_counter_vaddr;
	uint64_t			     key		       = params->key;
	uint64_t			     op64		       = params->op;
	uint64_t			     dt64		       = params->dt;
	uint32_t			     opcode		       = params->opcode;
	const enum ofi_reliability_kind	     reliability	       = params->reliability;
	/* use the slid from the lrh header of the incoming packet
	 * as the dlid for the lrh header of the outgoing packet */
	const uint64_t lrh_dlid	      = params->lrh_dlid;
	const uint64_t bth_subctxt_rx = ((uint64_t) subctxt_rx) << OPX_BTH_SUBCTXT_RX_SHIFT;
	unsigned       i;
	const void    *sbuf_start	  = params->src_base_addr;
	const bool     sdma_no_bounce_buf = params->sdma_no_bounce_buf;
	const uint64_t max_pkts_per_req =
		sdma_no_bounce_buf ? opx_ep->tx->sdma_max_pkts_tid : OPX_SDMA_MAX_PKTS_BOUNCE_BUF;

	assert(params->ntidpairs != 0);
	assert(niov == 1);

	/* Note that lrh_dlid is just the version of params->slid shifted so
	   that it can be OR'd into the correct position in the packet header */
	if (hfi1_type & (OPX_HFI1_WFR | OPX_HFI1_MIXED_9B)) {
		assert(__cpu24_to_be16(params->slid) == (lrh_dlid >> 16));
		assert(opx_ep->rx->tx.rzv_dput_9B.hdr.lrh_9B.slid != params->slid);
	} else {
		assert(params->slid == lrh_dlid);
	}

	// We should never be in this function for shm ops
	assert(!params->is_shm);

	assert((opcode == FI_OPX_HFI_DPUT_OPCODE_RZV_TID) && (params->payload_bytes_for_iovec == 0));

	// With SDMA replay we can support MTU packet sizes even
	// on credit-constrained systems with smaller PIO packet
	// sizes. Ignore pio_max_eager_tx_bytes
	const uint64_t max_dput_bytes = OPX_HFI1_PKT_SIZE;

	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA,
	       "%p:===================================== SEND DPUT SDMA TID, opcode %X -- (begin)\n", params, opcode);
	OPX_TRACER_TRACE(OPX_TRACER_BEGIN, "SEND-DPUT-SDMA-TID");

	for (i = params->cur_iov; i < niov; ++i) {
		uint32_t *tidpairs = (uint32_t *) params->tid_iov.iov_base;
		uint32_t  tididx   = params->tididx;
		uint32_t  tidbytes_consumed;
		uint32_t  tidbytes_remaining;
		uint32_t  prev_tididx		  = 0;
		uint32_t  prev_tidbytes_consumed  = 0;
		uint32_t  prev_tidbytes_remaining = 0;
		uint32_t  tidoffset		  = 0;
		uint32_t  tidOMshift		  = 0;
		if (tididx == -1U) { /* first time */
			FI_OPX_DEBUG_COUNTERS_INC_COND_N(
				(opx_ep->debug_counters.expected_receive.first_tidpair_minoffset == 0),
				params->tidoffset, opx_ep->debug_counters.expected_receive.first_tidpair_minoffset);
			FI_OPX_DEBUG_COUNTERS_MIN_OF(opx_ep->debug_counters.expected_receive.first_tidpair_minoffset,
						     params->tidoffset);
			FI_OPX_DEBUG_COUNTERS_MAX_OF(opx_ep->debug_counters.expected_receive.first_tidpair_maxoffset,
						     params->tidoffset);

			tididx		  = 0;
			tidbytes_consumed = params->tidoffset;
			tidbytes_remaining =
				(FI_OPX_EXP_TID_GET(tidpairs[0], LEN) * OPX_HFI1_TID_PAGESIZE) - tidbytes_consumed;
			if (tidbytes_consumed) {
				FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA,
				       "params->tidoffset %u, tidlen_consumed %u, tidlen_remaining %u, length  %llu\n",
				       params->tidoffset, tidbytes_consumed, tidbytes_remaining,
				       FI_OPX_EXP_TID_GET(tidpairs[0], LEN));
			}
		} else { /* eagain retry, restore previous TID state */
			tidbytes_consumed  = params->tidbytes_consumed;
			tidbytes_remaining = params->tidbytes_remaining;
		}

		uint32_t starting_tid_idx = tididx;

		uint8_t *sbuf =
			(uint8_t *) ((uintptr_t) sbuf_start + (uintptr_t) dput_iov[i].sbuf + params->bytes_sent);
		uintptr_t rbuf = dput_iov[i].rbuf + params->bytes_sent;

		uint64_t bytes_to_send = dput_iov[i].bytes - params->bytes_sent;
		FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA,
		       " sbuf %p, sbuf_start %p, dput_iov[%u].sbuf %p, dput_iov[i].bytes %lu/%#lX, bytes sent %lu/%#lX, bytes_to_send %lu/%#lX, origin_byte_counter %ld\n",
		       sbuf, sbuf_start, i, (void *) dput_iov[i].sbuf, dput_iov[i].bytes, dput_iov[i].bytes,
		       params->bytes_sent, params->bytes_sent, bytes_to_send, bytes_to_send,
		       params->origin_byte_counter ? *(params->origin_byte_counter) : -1UL);
		FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA,
		       " rbuf %p, dput_iov[%u].rbuf %p, dput_iov[i].bytes %lu/%#lX, bytes sent %lu/%#lX, bytes_to_send %lu/%#lX\n",
		       (void *) rbuf, i, (void *) dput_iov[i].rbuf, dput_iov[i].bytes, dput_iov[i].bytes,
		       params->bytes_sent, params->bytes_sent, bytes_to_send, bytes_to_send);
		while (bytes_to_send > 0) {
			if (!fi_opx_hfi1_sdma_queue_has_room(opx_ep, OPX_SDMA_TID_IOV_COUNT)) {
				FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA,
				       "%p:===================================== SEND DPUT SDMA QUEUE FULL FI_EAGAIN\n",
				       params);
				OPX_TRACER_TRACE(OPX_TRACER_END_EAGAIN_SDMA_QUEUE_FULL, "SEND-DPUT-SDMA-TID");
				return -FI_EAGAIN;
			}
			if (!params->sdma_we) {
				/* Get an SDMA work entry since we don't already have one */
				params->sdma_we = opx_sdma_get_new_work_entry(opx_ep, &params->sdma_reqs_used,
									      &params->sdma_reqs, params->sdma_we);
				if (!params->sdma_we) {
					FI_OPX_DEBUG_COUNTERS_INC_COND(
						(params->sdma_reqs_used < FI_OPX_HFI1_SDMA_MAX_WE_PER_REQ),
						opx_ep->debug_counters.sdma.eagain_sdma_we_none_free);
					FI_OPX_DEBUG_COUNTERS_INC_COND(
						(params->sdma_reqs_used == FI_OPX_HFI1_SDMA_MAX_WE_PER_REQ),
						opx_ep->debug_counters.sdma.eagain_sdma_we_max_used);
					FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA,
					       "%p:===================================== SEND DPUT SDMA TID, !WE FI_EAGAIN\n",
					       params);
					OPX_TRACER_TRACE(OPX_TRACER_END_EAGAIN_SDMA_NO_WE, "SEND-DPUT-SDMA-TID");
					return -FI_EAGAIN;
				}
				assert(params->sdma_we->total_payload == 0);
				fi_opx_hfi1_sdma_init_we(params->sdma_we, params->cc, params->slid, params->origin_rx,
							 dput_iov[i].sbuf_iface, (int) dput_iov[i].sbuf_device);
			}
			assert(!fi_opx_hfi1_sdma_has_unsent_packets(params->sdma_we));

			uint64_t packet_count =
				(bytes_to_send / max_dput_bytes) + ((bytes_to_send % max_dput_bytes) ? 1 : 0);

			assert(packet_count > 0);
			packet_count = MIN(packet_count, max_pkts_per_req);

			if (packet_count < max_pkts_per_req) {
				/* Calc worst case based on single page length TIDs (4k packets) */
				packet_count = (bytes_to_send + (OPX_HFI1_TID_PAGESIZE - 1)) / OPX_HFI1_TID_PAGESIZE;
				packet_count = MIN(packet_count, max_pkts_per_req);
			}
			int32_t psns_avail = fi_opx_reliability_tx_available_psns(
				&opx_ep->ep_fid, opx_ep->reli_service, params->slid, params->origin_rx,
				&params->sdma_we->psn_ptr, packet_count, max_dput_bytes);

			if (psns_avail < (int64_t) packet_count) {
				FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.sdma.eagain_psn);
				FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA,
				       "%p:===================================== SEND DPUT SDMA TID, !PSN FI_EAGAIN\n",
				       params);
				OPX_TRACER_TRACE(OPX_TRACER_END_EAGAIN_SDMA_PSNS, "SEND-DPUT-SDMA-TID:%d:%ld",
						 psns_avail, packet_count);
				return -FI_EAGAIN;
			}
#ifndef OPX_RELIABILITY_TEST /* defining this will force reliability replay of some packets */
			{
				const int psn = params->sdma_we->psn_ptr->psn.psn;
				/* SDMA header auto-generation splits psn into
				 * generation and sequence numbers.
				 * In a writev, the generation is not incremented,
				 * instead the sequence wraps resulting in a psn
				 * that is dropped by the remote, forcing reliability
				 * replay.  We must break the writev at the wrap point
				 * and start the next writev with the next generation
				 * incremented.
				 *
				 * Since this is useful debug, it's #ifndef'd
				 * instead of just being implemented (correctly) */
				uint64_t const prev_packet_count = packet_count;
				packet_count			 = MIN(packet_count, 0x800 - (psn & 0x7FF));
				if (packet_count < prev_packet_count) {
					FI_OPX_DEBUG_COUNTERS_INC(
						opx_ep->debug_counters.expected_receive.generation_wrap);
				}
			}
#endif
			/* TID cannot add padding and has aligned buffers
			 * appropriately.  Assert that. Bounce buffers
			 * are used when not DC or fetch, not for "padding".
			 */
			assert(!(packet_count == 1 && (bytes_to_send & 0x3ul)));
			params->sdma_we->use_bounce_buf = !sdma_no_bounce_buf;

			uint8_t *sbuf_tmp;
			if (params->sdma_we->use_bounce_buf) {
				OPX_HMEM_COPY_FROM(params->sdma_we->bounce_buf.buf, sbuf,
						   MIN((packet_count * max_dput_bytes), bytes_to_send),
						   dput_iov[i].sbuf_handle, OPX_HMEM_DEV_REG_SEND_THRESHOLD,
						   dput_iov[i].sbuf_iface, dput_iov[i].sbuf_device);
				sbuf_tmp = params->sdma_we->bounce_buf.buf;
			} else {
				sbuf_tmp = sbuf;
			}
			// At this point, we have enough SDMA queue entries and PSNs
			// to send packet_count packets. The only limit now is how
			// many replays can we get.
			for (int p = 0; (p < packet_count) && bytes_to_send; ++p) {
				assert(tididx < params->ntidpairs);

				uint64_t packet_bytes = MIN(bytes_to_send, max_dput_bytes);
				if (p == 0) { /* First packet header is user's responsibility even with SDMA header
						 auto-generation*/
					/* set fields for first header */
					unsigned offset_shift;
					starting_tid_idx = tididx; /* first tid this write() */
					if ((FI_OPX_EXP_TID_GET(tidpairs[tididx], LEN)) >=
					    (KDETH_OM_MAX_SIZE / OPX_HFI1_TID_PAGESIZE)) {
						tidOMshift   = (1 << HFI_KHDR_OM_SHIFT);
						offset_shift = KDETH_OM_LARGE_SHIFT;
					} else {
						tidOMshift   = 0;
						offset_shift = KDETH_OM_SMALL_SHIFT;
					}
					tidoffset = tidbytes_consumed >> offset_shift;
					FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA, "%p:tidoffset %#X/%#X\n", params,
					       tidoffset, tidoffset << offset_shift);
				}

				/* Save current values in case we can't process this packet (!REPLAY)
					   and need to restore state */
				prev_tididx		= tididx;
				prev_tidbytes_consumed	= tidbytes_consumed;
				prev_tidbytes_remaining = tidbytes_remaining;
				/* If we offset into this TID, SDMA header auto-generation will have sent
				 * 4k/8k packets but now we have to adjust our length on the last packet
				 * to not exceed the pinned pages (subtract the offset from the last
				 * packet) like SDMA header auto-generation will do.
				 */
				packet_bytes = MIN(packet_bytes, tidbytes_remaining);
				FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA,
				       "%p:tid[%u], tidlen_remaining %u, packet_bytes %#lX, packet_count %lu\n", params,
				       tididx, tidbytes_remaining, packet_bytes, packet_count);

				tidbytes_remaining -= packet_bytes;
				tidbytes_consumed += packet_bytes;
				if (tidbytes_remaining == 0 && tididx < (params->ntidpairs - 1)) {
					tididx++;
					tidbytes_remaining =
						FI_OPX_EXP_TID_GET(tidpairs[tididx], LEN) * OPX_HFI1_TID_PAGESIZE;
					tidbytes_consumed = 0;
				}
				FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA,
				       "%p:tid[%u/%u], tidlen_remaining %u, packet_bytes %#lX, packet_count %lu\n",
				       params, tididx, params->ntidpairs, tidbytes_remaining, packet_bytes,
				       packet_count);

				struct fi_opx_reliability_tx_replay *replay;
				replay = fi_opx_reliability_service_replay_allocate(opx_ep->reli_service, true);
				if (OFI_UNLIKELY(replay == NULL)) {
					/* Restore previous values in case since we can't process this
					 * packet. We may or may not -FI_EAGAIN later (!REPLAY).*/
					tididx		   = prev_tididx;
					tidbytes_consumed  = prev_tidbytes_consumed;
					tidbytes_remaining = prev_tidbytes_remaining;
					FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA,
					       "%p:!REPLAY on packet %u out of %lu, params->sdma_we->num_packets %u\n",
					       params, p, packet_count, params->sdma_we->num_packets);
					break;
				}
				replay->use_sdma = true; /* Always replay TID packets with SDMA */

				// Round packet_bytes up to the next multiple of 4,
				// then divide by 4 to get the correct number of dws.
				uint64_t pbc_dws;
				uint16_t lrh_dws;
				uint64_t bytes_sent;
				if (hfi1_type & (OPX_HFI1_WFR | OPX_HFI1_MIXED_9B)) {
					uint64_t payload_dws = (packet_bytes + 3) >> 2;
					pbc_dws		     = 2 + /* pbc */
						  2 +		   /* lrh */
						  3 +		   /* bth */
						  9 +		   /* kdeth; from "RcvHdrSize[i].HdrSize" CSR */
						  payload_dws;
					lrh_dws = htons(pbc_dws - 2 + 1); /* (BE: LRH DW) does not include pbc (8
									     bytes), but does include icrc (4 bytes) */
					replay->scb.scb_9B.qw0 = opx_ep->rx->tx.rzv_dput_9B.qw0 |
								 OPX_PBC_LEN(pbc_dws, hfi1_type) | params->pbc_dlid |
								 OPX_PBC_LOOPBACK(params->pbc_dlid, hfi1_type);
					/* The fetch_vaddr and cbuf arguments are only used
					for atomic fetch operations, which by their one-
					sided nature will never use TID, so they are
					hard-coded to 0/NULL respectively */
					bytes_sent = opx_hfi1_dput_write_header_and_iov(
						opx_ep, &replay->scb.scb_9B.hdr, replay->iov, opcode, lrh_dws, op64,
						dt64, lrh_dlid, bth_subctxt_rx, packet_bytes, key, 0ul,
						target_byte_counter_vaddr, params->rma_request_vaddr,
						params->bytes_sent, &sbuf_tmp, NULL, &rbuf, OPX_HFI1_WFR);
				} else {
					uint64_t payload_dws =
						((packet_bytes + 7) & -8) >> 2; /* 16B is QW length/padded */
					pbc_dws = 2 +				/* pbc */
						  4 +				/* lrh uncompressed */
						  3 +				/* bth */
						  9 + /* kdeth; from "RcvHdrSize[i].HdrSize" CSR */
						  2 + /* ICRC/tail */
						  payload_dws;
					lrh_dws = (pbc_dws - 2) >> 1; /* (LRH QW) does not include pbc (8 bytes) */
					replay->scb.scb_16B.qw0 =
						opx_ep->rx->tx.rzv_dput_16B.qw0 | OPX_PBC_LEN(pbc_dws, OPX_HFI1_CYR) |
						params->pbc_dlid | OPX_PBC_LOOPBACK(params->pbc_dlid, OPX_HFI1_CYR);
					/* The fetch_vaddr and cbuf arguments are only used
					for atomic fetch operations, which by their one-
					sided nature will never use TID, so they are
					hard-coded to 0/NULL respectively */
					bytes_sent = opx_hfi1_dput_write_header_and_iov(
						opx_ep, &replay->scb.scb_16B.hdr, replay->iov, opcode, lrh_dws, op64,
						dt64, lrh_dlid, bth_subctxt_rx, packet_bytes, key, 0ul,
						target_byte_counter_vaddr, params->rma_request_vaddr,
						params->bytes_sent, &sbuf_tmp, NULL, &rbuf, OPX_HFI1_CYR);
				}

				fi_opx_hfi1_sdma_add_packet(params->sdma_we, replay, packet_bytes);

				bytes_to_send -= bytes_sent;
				params->bytes_sent += bytes_sent;
				params->origin_bytes_sent += bytes_sent;
				sbuf += bytes_sent;
			}

			// Must be we had trouble getting a replay buffer
			if (OFI_UNLIKELY(params->sdma_we->num_packets == 0)) {
				FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.sdma.eagain_replay);
				FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA,
				       "%p:===================================== SEND DPUT SDMA TID, !REPLAY FI_EAGAIN\n",
				       params);
				OPX_TRACER_TRACE(OPX_TRACER_END_EAGAIN_SDMA_REPLAY_BUFFER, "SEND-DPUT-SDMA-TID");
				return -FI_EAGAIN;
			}

			if (hfi1_type & (OPX_HFI1_WFR | OPX_HFI1_MIXED_9B)) {
				opx_hfi1_sdma_flush(opx_ep, params->sdma_we, &params->sdma_reqs, 1, /* use tid */
						    &params->tid_iov, starting_tid_idx, tididx, tidOMshift, tidoffset,
						    reliability, OPX_HFI1_WFR);
			} else {
				opx_hfi1_sdma_flush(opx_ep, params->sdma_we, &params->sdma_reqs, 1, /* use tid */
						    &params->tid_iov, starting_tid_idx, tididx, tidOMshift, tidoffset,
						    reliability, OPX_HFI1_CYR);
			}
			params->sdma_we = NULL;
			/* save our 'done' tid state incase we return EAGAIN next loop */
			params->tididx		   = tididx;
			params->tidbytes_consumed  = tidbytes_consumed;
			params->tidbytes_remaining = tidbytes_remaining;

		} /* while bytes_to_send */

		FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA,
		       "%p:===================================== SEND DPUT SDMA TID, finished IOV=%d(%d) bytes_sent=%ld\n",
		       params, params->cur_iov, niov, params->bytes_sent);

		params->bytes_sent = 0;
		params->cur_iov++;
	} /* for niov */
	OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "SEND-DPUT-SDMA-TID");
	FI_DBG(fi_opx_global.prov, FI_LOG_EP_DATA,
	       "%p:===================================== SEND DPUT SDMA TID, exit (end)\n", params);

	// At this point, all SDMA WE should have succeeded sending, and only reside on the reqs list
	assert(params->sdma_we == NULL);
	assert(!slist_empty(&params->sdma_reqs));

	// If we're not doing delivery completion, the user's payload would have
	// been copied to bounce buffer(s), so at this point, it should be safe
	// for the user to alter the send buffer even though the send may still
	// be in progress.
	if (!params->sdma_no_bounce_buf) {
		assert(params->origin_byte_counter);
		assert((*params->origin_byte_counter) >= params->origin_bytes_sent);
		*params->origin_byte_counter -= params->origin_bytes_sent;
		params->origin_byte_counter = NULL;
	}
	params->work_elem.work_type = OPX_WORK_TYPE_LAST;
	params->work_elem.work_fn   = fi_opx_hfi1_dput_sdma_pending_completion;

	// The SDMA request has been queued for sending, but not actually sent
	// yet, so there's no point in checking for completion right away. Wait
	// until the next poll cycle.
	return -FI_EAGAIN;
}

int fi_opx_hfi1_do_dput_sdma_tid_wfr(union fi_opx_hfi1_deferred_work *work)
{
	return fi_opx_hfi1_do_dput_sdma_tid(work, OPX_HFI1_WFR);
}

int fi_opx_hfi1_do_dput_sdma_tid_jkr(union fi_opx_hfi1_deferred_work *work)
{
	return fi_opx_hfi1_do_dput_sdma_tid(work, OPX_HFI1_JKR);
}

int fi_opx_hfi1_do_dput_sdma_tid_cyr(union fi_opx_hfi1_deferred_work *work)
{
	return fi_opx_hfi1_do_dput_sdma_tid(work, OPX_HFI1_CYR);
}

int fi_opx_hfi1_do_dput_sdma_tid_jkr_9B(union fi_opx_hfi1_deferred_work *work)
{
	return fi_opx_hfi1_do_dput_sdma_tid(work, OPX_HFI1_MIXED_9B);
}
union fi_opx_hfi1_deferred_work *
fi_opx_hfi1_rx_rzv_cts(struct fi_opx_ep *opx_ep, const union opx_hfi1_packet_hdr *const hdr, const void *const payload,
		       size_t payload_bytes_to_copy, const uint16_t origin_rx, const uint32_t niov,
		       const union opx_hfi1_dput_iov *const dput_iov, uint8_t *src_base_addr, const uint8_t op,
		       const uint8_t dt, const uintptr_t rma_request_vaddr, const uintptr_t target_byte_counter_vaddr,
		       uint64_t *origin_byte_counter, uint32_t dput_opcode,
		       void (*completion_action)(union fi_opx_hfi1_deferred_work *work_state), const unsigned is_shm,
		       const enum ofi_reliability_kind reliability, const uint32_t u32_extended_rx,
		       const enum opx_hfi1_type hfi1_type)
{
	if (dput_opcode == FI_OPX_HFI_DPUT_OPCODE_PUT_CQ) {
		struct fi_opx_completion_counter *cc = ((struct fi_opx_rma_request *) rma_request_vaddr)->cc;
		if (cc->byte_counter == 0) {
			OPX_BUF_FREE((struct fi_opx_rma_request *) rma_request_vaddr);
			cc->hit_zero(cc);
			return NULL;
		}
	}

	union fi_opx_hfi1_deferred_work *work	= ofi_buf_alloc(opx_ep->tx->work_pending_pool);
	struct fi_opx_hfi1_dput_params	*params = &work->dput;

	params->work_elem.slist_entry.next  = NULL;
	params->work_elem.completion_action = completion_action;
	params->work_elem.payload_copy	    = NULL;
	params->work_elem.complete	    = false;
	params->opx_ep			    = opx_ep;
	params->src_base_addr		    = src_base_addr;
	if (hfi1_type & OPX_HFI1_WFR) {
		params->slid	 = (opx_lid_t) __be16_to_cpu24((__be16) hdr->lrh_9B.slid);
		params->lrh_dlid = (hdr->lrh_9B.qw[0] & 0xFFFF000000000000ul) >> 32;
		params->pbc_dlid = OPX_PBC_DLID(params->slid, OPX_HFI1_WFR);
	} else if (hfi1_type & OPX_HFI1_MIXED_9B) {
		params->slid	 = (opx_lid_t) __be16_to_cpu24((__be16) hdr->lrh_9B.slid);
		params->lrh_dlid = (hdr->lrh_9B.qw[0] & 0xFFFF000000000000ul) >> 32;
		params->pbc_dlid = OPX_PBC_DLID(params->slid, OPX_HFI1_MIXED_9B);
	} else {
		params->slid	 = (opx_lid_t) __le24_to_cpu(hdr->lrh_16B.slid20 << 20 | hdr->lrh_16B.slid);
		params->lrh_dlid = params->slid; // Send dput to the SLID that sent CTS
		params->pbc_dlid = OPX_PBC_DLID(params->slid, OPX_HFI1_CYR);
	}
	params->origin_rx	  = origin_rx;
	params->u32_extended_rx	  = u32_extended_rx;
	params->niov		  = niov;
	params->dput_iov	  = &params->iov[0];
	params->cur_iov		  = 0;
	params->bytes_sent	  = 0;
	params->origin_bytes_sent = 0;

	if (dput_opcode == FI_OPX_HFI_DPUT_OPCODE_PUT_CQ) {
		params->cc = ((struct fi_opx_rma_request *) rma_request_vaddr)->cc;
		OPX_BUF_FREE((struct fi_opx_rma_request *) rma_request_vaddr);
		params->rma_request_vaddr = (uintptr_t) NULL;
	} else {
		params->cc		  = NULL;
		params->rma_request_vaddr = rma_request_vaddr;
	}
	params->user_cc			= NULL;
	params->payload_bytes_for_iovec = 0;
	params->sdma_no_bounce_buf	= false;

	params->target_byte_counter_vaddr = target_byte_counter_vaddr;
	params->origin_byte_counter	  = origin_byte_counter;
	params->opcode			  = dput_opcode;
	params->op			  = op;
	params->dt			  = dt;
	params->is_shm			  = is_shm;
	params->reliability		  = reliability;
	params->target_hfi_unit		  = is_shm ? fi_opx_hfi1_get_lid_local_unit(params->slid) : 0xFF;

	uint64_t is_hmem	 = 0;
	uint64_t iov_total_bytes = 0;
	for (int idx = 0; idx < niov; idx++) {
#ifdef OPX_HMEM
		/* If either the send or receive buffer's iface is non-zero, i.e. not system memory, set hmem on */
		is_hmem |= (dput_iov[idx].rbuf_iface | dput_iov[idx].sbuf_iface);
#endif
		params->iov[idx] = dput_iov[idx];
		iov_total_bytes += dput_iov[idx].bytes;
	}
	/* Only RZV TID sets ntidpairs */
	uint32_t  ntidpairs = 0;
	uint32_t  tidoffset = 0;
	uint32_t *tidpairs  = NULL;

	if (dput_opcode == FI_OPX_HFI_DPUT_OPCODE_RZV_TID) {
		ntidpairs = hdr->cts.target.vaddr.ntidpairs;
		if (ntidpairs) {
			union fi_opx_hfi1_packet_payload *tid_payload = (union fi_opx_hfi1_packet_payload *) payload;
			tidpairs				      = tid_payload->tid_cts.tidpairs;
			tidoffset				      = tid_payload->tid_cts.tid_offset;
			/* Receiver may have adjusted the length for expected TID alignment.*/
			if (origin_byte_counter) {
				(*origin_byte_counter) += tid_payload->tid_cts.origin_byte_counter_adjust;
			}
		}
	}

	assert((ntidpairs == 0) || (niov == 1));
	assert(origin_byte_counter == NULL || iov_total_bytes <= *origin_byte_counter);
	fi_opx_hfi1_dput_sdma_init(opx_ep, params, iov_total_bytes, tidoffset, ntidpairs, tidpairs, is_hmem, hfi1_type);

	FI_OPX_DEBUG_COUNTERS_INC_COND(is_hmem && is_shm, opx_ep->debug_counters.hmem.dput_rzv_intranode);
	FI_OPX_DEBUG_COUNTERS_INC_COND(is_hmem && !is_shm &&
					       (params->work_elem.work_fn == fi_opx_hfi1_do_dput_wfr ||
						params->work_elem.work_fn == fi_opx_hfi1_do_dput_jkr ||
						params->work_elem.work_fn == fi_opx_hfi1_do_dput_cyr ||
						params->work_elem.work_fn == fi_opx_hfi1_do_dput_jkr_9B),
				       opx_ep->debug_counters.hmem.dput_rzv_pio);
	FI_OPX_DEBUG_COUNTERS_INC_COND(is_hmem && (params->work_elem.work_fn == fi_opx_hfi1_do_dput_sdma_wfr ||
						   params->work_elem.work_fn == fi_opx_hfi1_do_dput_sdma_jkr ||
						   params->work_elem.work_fn == fi_opx_hfi1_do_dput_sdma_cyr ||
						   params->work_elem.work_fn == fi_opx_hfi1_do_dput_sdma_jkr_9B),
				       opx_ep->debug_counters.hmem.dput_rzv_sdma);
	FI_OPX_DEBUG_COUNTERS_INC_COND(is_hmem && (params->work_elem.work_fn == fi_opx_hfi1_do_dput_sdma_tid_wfr ||
						   params->work_elem.work_fn == fi_opx_hfi1_do_dput_sdma_tid_jkr ||
						   params->work_elem.work_fn == fi_opx_hfi1_do_dput_sdma_tid_cyr ||
						   params->work_elem.work_fn == fi_opx_hfi1_do_dput_sdma_tid_jkr_9B),
				       opx_ep->debug_counters.hmem.dput_rzv_tid);

	// We can't/shouldn't start this work until any pending work is finished.
	if (params->work_elem.work_type != OPX_WORK_TYPE_SDMA &&
	    slist_empty(&opx_ep->tx->work_pending[params->work_elem.work_type])) {
		int rc = params->work_elem.work_fn(work);
		if (rc == FI_SUCCESS) {
			FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
				     "===================================== CTS done %u\n", params->work_elem.complete);
			assert(params->work_elem.complete);
			OPX_BUF_FREE(work);
			return NULL;
		}
		assert(rc == -FI_EAGAIN);
		if (params->work_elem.work_type == OPX_WORK_TYPE_LAST) {
			FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
				     "===================================== CTS FI_EAGAIN queued low priority %u\n",
				     params->work_elem.complete);
			slist_insert_tail(&work->work_elem.slist_entry, &opx_ep->tx->work_pending_completion);
			return NULL;
		}
		FI_DBG_TRACE(
			fi_opx_global.prov, FI_LOG_EP_DATA,
			"===================================== CTS FI_EAGAIN queued %u, payload_bytes_to_copy %zu\n",
			params->work_elem.complete, payload_bytes_to_copy);
	} else {
		FI_DBG_TRACE(
			fi_opx_global.prov, FI_LOG_EP_DATA,
			"===================================== CTS queued with work pending %u, payload_bytes_to_copy %zu\n",
			params->work_elem.complete, payload_bytes_to_copy);
	}

	/* Try again later*/
	if (payload_bytes_to_copy) {
		params->work_elem.payload_copy = ofi_buf_alloc(opx_ep->tx->rma_payload_pool);
		memcpy(params->work_elem.payload_copy, payload, payload_bytes_to_copy);
	}
	assert(work->work_elem.slist_entry.next == NULL);
	slist_insert_tail(&work->work_elem.slist_entry, &opx_ep->tx->work_pending[params->work_elem.work_type]);
	return work;
}

uint64_t num_sends;
uint64_t total_sendv_bytes;
ssize_t	 opx_hfi1_tx_sendv_rzv(struct fid_ep *ep, const struct iovec *iov, size_t niov, size_t total_len,
			       fi_addr_t dest_addr, uint64_t tag, void *user_context, const uint32_t data,
			       int lock_required, const unsigned override_flags, const uint64_t tx_op_flags,
			       const uint64_t dest_rx, const uint64_t caps, const enum ofi_reliability_kind reliability,
			       const uint64_t do_cq_completion, const enum fi_hmem_iface hmem_iface,
			       const uint64_t hmem_device, const uint64_t hmem_handle,
			       const enum opx_hfi1_type hfi1_type, const bool ctx_sharing)
{
	// We should already have grabbed the lock prior to calling this function
	assert(!lock_required);

	struct fi_opx_ep       *opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);
	const union fi_opx_addr addr   = {.fi = dest_addr};
	const uint64_t		bth_rx = ((uint64_t) dest_rx) << OPX_BTH_SUBCTXT_RX_SHIFT;
	assert(niov <= MIN(FI_OPX_MAX_DPUT_IOV, FI_OPX_MAX_HMEM_IOV));

	FI_OPX_DEBUG_COUNTERS_DECLARE_TMP(hmem_non_system);

	/* This is a hack to trick an MPICH test to make some progress    */
	/* As it erroneously overflows the send buffers by never checking */
	/* for multi-receive overflows properly in some onesided tests    */
	/* There are almost certainly better ways to do this */
	if ((tx_op_flags & FI_MSG) && (total_sendv_bytes += total_len > opx_ep->rx->min_multi_recv)) {
		total_sendv_bytes = 0;
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "FI_EAGAIN\n");
		return -FI_EAGAIN;
	}

	// Calculate space for each IOV, then add in the origin_byte_counter_vaddr,
	// and round to the next 64-byte block.
	const uint64_t icrc_and_tail_block = ((hfi1_type & OPX_HFI1_CNX000) ? 1 : 0);
	const uint64_t payload_blocks_total =
		((niov * sizeof(struct fi_opx_hmem_iov)) + sizeof(uintptr_t) + icrc_and_tail_block + 63) >> 6;
	assert(payload_blocks_total > 0 && payload_blocks_total < (OPX_HFI1_PKT_SIZE >> 6));

	uint64_t pbc_dws;
	uint16_t lrh_dws;

	if (hfi1_type & (OPX_HFI1_WFR | OPX_HFI1_MIXED_9B)) {
		pbc_dws = 2 + /* pbc */
			  2 + /* lrh */
			  3 + /* bth */
			  9 + /* kdeth; from "RcvHdrSize[i].HdrSize" CSR */
			  (payload_blocks_total << 4);

		lrh_dws = htons(pbc_dws - 2 +
				1); /* (BE: LRH DW) does not include pbc (8 bytes), but does include icrc (4 bytes) */
	} else {
		pbc_dws = 2 +			       /* pbc */
			  4 +			       /* lrh uncompressed */
			  3 +			       /* bth */
			  9 +			       /* kdeth; from "RcvHdrSize[i].HdrSize" CSR */
			  (payload_blocks_total << 4); /* ICRC/tail is accounted for here */
		lrh_dws = (pbc_dws - 2) >> 1;	       /* (LRH QW) does not include pbc (8 bytes) */
	}

	if (fi_opx_hfi1_tx_is_shm(opx_ep, addr, caps)) {
		FI_DBG_TRACE(
			fi_opx_global.prov, FI_LOG_EP_DATA,
			"===================================== SENDV, SHM -- RENDEZVOUS RTS Noncontig (begin) context %p\n",
			user_context);

		OPX_TRACER_TRACE(OPX_TRACER_BEGIN, "SENDV-RZV-RTS-NONCONTIG-SHM");
		uint64_t			 pos;
		ssize_t				 rc;
		union opx_hfi1_packet_hdr *const hdr = opx_shm_tx_next(
			&opx_ep->tx->shm, addr.hfi1_unit, dest_rx, &pos, opx_ep->daos_info.hfi_rank_enabled,
			opx_ep->daos_info.rank, opx_ep->daos_info.rank_inst, &rc);

		if (!hdr) {
			return rc;
		}

		struct opx_context *context;
		uintptr_t	    origin_byte_counter_vaddr;
		if (OFI_LIKELY(do_cq_completion)) {
			context = (struct opx_context *) ofi_buf_alloc(opx_ep->rx->ctx_pool);
			if (OFI_UNLIKELY(context == NULL)) {
				FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA, "Out of memory.\n");
				return -FI_ENOMEM;
			}
			context->err_entry.err	      = 0;
			context->err_entry.op_context = user_context;
			context->next		      = NULL;
			context->byte_counter	      = total_len;
			origin_byte_counter_vaddr     = (uintptr_t) &context->byte_counter;
		} else {
			context			  = NULL;
			origin_byte_counter_vaddr = (uintptr_t) NULL;
		}

		if (hfi1_type & (OPX_HFI1_WFR | OPX_HFI1_MIXED_9B)) {
			const uint64_t lrh_dlid_9B = FI_OPX_ADDR_TO_HFI1_LRH_DLID_9B(addr.lid);
			hdr->qw_9B[0] = opx_ep->tx->rzv_9B.hdr.qw_9B[0] | lrh_dlid_9B | ((uint64_t) lrh_dws << 32);
			hdr->qw_9B[1] = opx_ep->tx->rzv_9B.hdr.qw_9B[1] | bth_rx |
					((caps & FI_MSG) ? ((tx_op_flags & FI_REMOTE_CQ_DATA) ?
								    (uint64_t) FI_OPX_HFI_BTH_OPCODE_MSG_RZV_RTS_CQ :
								    FI_OPX_HFI_BTH_OPCODE_MSG_RZV_RTS) :
							   ((tx_op_flags & FI_REMOTE_CQ_DATA) ?
								    (uint64_t) FI_OPX_HFI_BTH_OPCODE_TAG_RZV_RTS_CQ :
								    FI_OPX_HFI_BTH_OPCODE_TAG_RZV_RTS));
			hdr->qw_9B[2] = opx_ep->tx->rzv_9B.hdr.qw_9B[2];
			hdr->qw_9B[3] = opx_ep->tx->rzv_9B.hdr.qw_9B[3] | (((uint64_t) data) << 32);
			hdr->qw_9B[4] =
				opx_ep->tx->rzv_9B.hdr.qw_9B[4] | (niov << 48) | FI_OPX_PKT_RZV_FLAGS_NONCONTIG_MASK;
			hdr->qw_9B[5] = total_len;
			hdr->qw_9B[6] = tag;
		} else {
			const uint64_t lrh_dlid_16B = addr.lid;
			hdr->qw_16B[0]		    = opx_ep->tx->rzv_16B.hdr.qw_16B[0] |
					 ((uint64_t) (lrh_dlid_16B & OPX_LRH_JKR_16B_DLID_MASK_16B)
					  << OPX_LRH_JKR_16B_DLID_SHIFT_16B) |
					 ((uint64_t) lrh_dws << 20);
			hdr->qw_16B[1] = opx_ep->tx->rzv_16B.hdr.qw_16B[1] |
					 ((uint64_t) ((lrh_dlid_16B & OPX_LRH_JKR_16B_DLID20_MASK_16B) >>
						      OPX_LRH_JKR_16B_DLID20_SHIFT_16B)) |
					 (uint64_t) (bth_rx >> OPX_LRH_JKR_BTH_RX_ENTROPY_SHIFT_16B);
			hdr->qw_16B[2] = opx_ep->tx->rzv_16B.hdr.qw_16B[2] | bth_rx |
					 ((caps & FI_MSG) ? ((tx_op_flags & FI_REMOTE_CQ_DATA) ?
								     (uint64_t) FI_OPX_HFI_BTH_OPCODE_MSG_RZV_RTS_CQ :
								     FI_OPX_HFI_BTH_OPCODE_MSG_RZV_RTS) :
							    ((tx_op_flags & FI_REMOTE_CQ_DATA) ?
								     (uint64_t) FI_OPX_HFI_BTH_OPCODE_TAG_RZV_RTS_CQ :
								     FI_OPX_HFI_BTH_OPCODE_TAG_RZV_RTS));
			hdr->qw_16B[3] = opx_ep->tx->rzv_16B.hdr.qw_16B[3];
			hdr->qw_16B[4] = opx_ep->tx->rzv_16B.hdr.qw_16B[4] | (((uint64_t) data) << 32);
			hdr->qw_16B[5] =
				opx_ep->tx->rzv_16B.hdr.qw_16B[5] | (niov << 48) | FI_OPX_PKT_RZV_FLAGS_NONCONTIG_MASK;
			hdr->qw_16B[6] = total_len;
			hdr->qw_16B[7] = tag;
		}

		union fi_opx_hfi1_packet_payload *const payload = (union fi_opx_hfi1_packet_payload *) (hdr + 1);

		payload->rendezvous.noncontiguous.origin_byte_counter_vaddr = origin_byte_counter_vaddr;
		struct fi_opx_hmem_iov *payload_iov			    = &payload->rendezvous.noncontiguous.iov[0];
		struct iovec	       *input_iov			    = (struct iovec *) iov;

		for (int i = 0; i < niov; i++) {
#ifdef OPX_HMEM
			// TODO: desc is plumbed into this function as a single pointer
			//       only representing the first IOV. It should be changed
			//       to void ** to get an array of desc, one for each IOV.
			//       For now, just use the first iov's desc, assuming all
			//       the IOVs will reside in the same HMEM space.
			FI_OPX_DEBUG_COUNTERS_INC_COND(hmem_iface != FI_HMEM_SYSTEM, hmem_non_system);
#endif
			payload_iov->buf    = (uintptr_t) input_iov->iov_base;
			payload_iov->len    = input_iov->iov_len;
			payload_iov->device = hmem_device;
			payload_iov->iface  = hmem_iface;
			payload_iov++;
			input_iov++;
		}

		FI_OPX_DEBUG_COUNTERS_INC_COND(
			hmem_non_system,
			opx_ep->debug_counters.hmem.intranode.kind[(caps & FI_MSG) ? FI_OPX_KIND_MSG : FI_OPX_KIND_TAG]
				.send.rzv_noncontig);
		opx_shm_tx_advance(&opx_ep->tx->shm, (void *) hdr, pos);

		if (OFI_LIKELY(do_cq_completion)) {
			fi_opx_ep_tx_cq_completion_rzv(ep, context, total_len, lock_required, tag, caps);
		}

		OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "SENDV-RZV-RTS-NONCONTIG-SHM");
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
			     "===================================== SENDV, SHM -- RENDEZVOUS RTS (end) context %p\n",
			     user_context);
		fi_opx_shm_poll_many(&opx_ep->ep_fid, 0, hfi1_type);
		return FI_SUCCESS;
	}
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		     "===================================== SENDV, HFI -- RENDEZVOUS RTS (begin) context %p\n",
		     user_context);
	OPX_TRACER_TRACE(OPX_TRACER_BEGIN, "SENDV-RZV-RTS-HFI");

	OPX_SHD_CTX_PIO_LOCK(OPX_IS_CTX_SHARING_ENABLED, opx_ep->tx);
	union fi_opx_hfi1_pio_state pio_state = *opx_ep->tx->pio_state;

	const uint16_t total_credits_needed = 1 +		    /* packet header */
					      payload_blocks_total; /* packet payload */

	uint64_t total_credits_available =
		FI_OPX_HFI1_AVAILABLE_CREDITS(pio_state, &opx_ep->tx->force_credit_return, total_credits_needed);
	if (OFI_UNLIKELY(total_credits_available < total_credits_needed)) {
		FI_OPX_HFI1_UPDATE_CREDITS(pio_state, opx_ep->tx->pio_credits_addr);
		total_credits_available = FI_OPX_HFI1_AVAILABLE_CREDITS(pio_state, &opx_ep->tx->force_credit_return,
									total_credits_needed);
		if (total_credits_available < total_credits_needed) {
			opx_ep->tx->pio_state->qw0 = pio_state.qw0;
			OPX_SHD_CTX_PIO_UNLOCK(OPX_IS_CTX_SHARING_ENABLED, opx_ep->tx);
			return -FI_EAGAIN;
		}
	}

	struct opx_context *context;
	uintptr_t	    origin_byte_counter_vaddr;
	if (OFI_LIKELY(do_cq_completion)) {
		context = (struct opx_context *) ofi_buf_alloc(opx_ep->rx->ctx_pool);
		if (OFI_UNLIKELY(context == NULL)) {
			FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA, "Out of memory.\n");
			opx_ep->tx->pio_state->qw0 = pio_state.qw0;
			OPX_SHD_CTX_PIO_UNLOCK(OPX_IS_CTX_SHARING_ENABLED, opx_ep->tx);
			return -FI_ENOMEM;
		}
		context->err_entry.err	      = 0;
		context->err_entry.op_context = user_context;
		context->next		      = NULL;
		context->byte_counter	      = total_len;
		origin_byte_counter_vaddr     = (uintptr_t) &context->byte_counter;
	} else {
		context			  = NULL;
		origin_byte_counter_vaddr = (uintptr_t) NULL;
	}

	struct fi_opx_reliability_tx_replay *replay;
	union fi_opx_reliability_tx_psn	    *psn_ptr;
	int64_t				     psn;

	psn = fi_opx_reliability_get_replay(&opx_ep->ep_fid, opx_ep->reli_service, addr.lid, addr.hfi1_subctxt_rx,
					    &psn_ptr, &replay, reliability, hfi1_type);
	if (OFI_UNLIKELY(psn == -1)) {
		if (OFI_LIKELY(do_cq_completion)) {
			OPX_BUF_FREE(context);
		}
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "FI_EAGAIN\n");
		opx_ep->tx->pio_state->qw0 = pio_state.qw0;
		OPX_SHD_CTX_PIO_UNLOCK(OPX_IS_CTX_SHARING_ENABLED, opx_ep->tx);
		return -FI_EAGAIN;
	}

	struct fi_opx_hmem_iov hmem_iov[FI_OPX_MAX_HMEM_IOV];
	unsigned	       hmem_niov = MIN(niov, FI_OPX_MAX_HMEM_IOV);
	for (int i = 0; i < hmem_niov; ++i) {
		hmem_iov[i].buf = (uintptr_t) iov[i].iov_base;
		hmem_iov[i].len = iov[i].iov_len;
#ifdef OPX_HMEM
		hmem_iov[i].iface  = hmem_iface;
		hmem_iov[i].device = hmem_device;
		FI_OPX_DEBUG_COUNTERS_INC_COND(hmem_iov[i].iface != FI_HMEM_SYSTEM, hmem_non_system);
#else
		hmem_iov[i].iface  = FI_HMEM_SYSTEM;
		hmem_iov[i].device = 0;
#endif
	}
	FI_OPX_DEBUG_COUNTERS_INC_COND(
		hmem_non_system,
		opx_ep->debug_counters.hmem.hfi.kind[(caps & FI_MSG) ? FI_OPX_KIND_MSG : FI_OPX_KIND_TAG]
			.send.rzv_noncontig);

	assert(opx_ep->tx->rzv_9B.qw0 == 0);
	const uint64_t force_credit_return = OPX_PBC_CR(opx_ep->tx->force_credit_return, hfi1_type);

	volatile uint64_t *const scb = FI_OPX_HFI1_PIO_SCB_HEAD(opx_ep->tx->pio_scb_sop_first, pio_state);

	if (hfi1_type & (OPX_HFI1_WFR | OPX_HFI1_MIXED_9B)) {
		const uint64_t lrh_dlid_9B = FI_OPX_ADDR_TO_HFI1_LRH_DLID_9B(addr.lid);
		const uint64_t pbc_dlid	   = OPX_PBC_DLID(addr.lid, hfi1_type);
		opx_cacheline_copy_qw_vol(
			scb, replay->scb.qws,
			opx_ep->tx->rzv_9B.qw0 | OPX_PBC_LEN(pbc_dws, hfi1_type) | force_credit_return | pbc_dlid |
				OPX_PBC_LOOPBACK(pbc_dlid, hfi1_type),
			opx_ep->tx->rzv_9B.hdr.qw_9B[0] | lrh_dlid_9B | ((uint64_t) lrh_dws << 32),
			opx_ep->tx->rzv_9B.hdr.qw_9B[1] | bth_rx |
				((caps & FI_MSG) ? ((tx_op_flags & FI_REMOTE_CQ_DATA) ?
							    (uint64_t) FI_OPX_HFI_BTH_OPCODE_MSG_RZV_RTS_CQ :
							    FI_OPX_HFI_BTH_OPCODE_MSG_RZV_RTS) :
						   ((tx_op_flags & FI_REMOTE_CQ_DATA) ?
							    (uint64_t) FI_OPX_HFI_BTH_OPCODE_TAG_RZV_RTS_CQ :
							    FI_OPX_HFI_BTH_OPCODE_TAG_RZV_RTS)),
			opx_ep->tx->rzv_9B.hdr.qw_9B[2] | psn,
			opx_ep->tx->rzv_9B.hdr.qw_9B[3] | (((uint64_t) data) << 32),
			opx_ep->tx->rzv_9B.hdr.qw_9B[4] | (niov << 48) | FI_OPX_PKT_RZV_FLAGS_NONCONTIG_MASK, total_len,
			tag);
	} else {
		const uint64_t lrh_dlid_16B = addr.lid;
		const uint64_t pbc_dlid	    = OPX_PBC_DLID(addr.lid, hfi1_type);
		opx_cacheline_copy_qw_vol(
			scb, replay->scb.qws,
			opx_ep->tx->rzv_16B.qw0 | OPX_PBC_LEN(pbc_dws, hfi1_type) | force_credit_return | pbc_dlid |
				OPX_PBC_LOOPBACK(pbc_dlid, hfi1_type),
			opx_ep->tx->rzv_16B.hdr.qw_16B[0] |
				((uint64_t) (lrh_dlid_16B & OPX_LRH_JKR_16B_DLID_MASK_16B)
				 << OPX_LRH_JKR_16B_DLID_SHIFT_16B) |
				((uint64_t) lrh_dws << 20),
			opx_ep->tx->rzv_16B.hdr.qw_16B[1] |
				((uint64_t) ((lrh_dlid_16B & OPX_LRH_JKR_16B_DLID20_MASK_16B) >>
					     OPX_LRH_JKR_16B_DLID20_SHIFT_16B)) |
				(uint64_t) (bth_rx >> OPX_LRH_JKR_BTH_RX_ENTROPY_SHIFT_16B),
			opx_ep->tx->rzv_16B.hdr.qw_16B[2] | bth_rx |
				((caps & FI_MSG) ? ((tx_op_flags & FI_REMOTE_CQ_DATA) ?
							    (uint64_t) FI_OPX_HFI_BTH_OPCODE_MSG_RZV_RTS_CQ :
							    FI_OPX_HFI_BTH_OPCODE_MSG_RZV_RTS) :
						   ((tx_op_flags & FI_REMOTE_CQ_DATA) ?
							    (uint64_t) FI_OPX_HFI_BTH_OPCODE_TAG_RZV_RTS_CQ :
							    FI_OPX_HFI_BTH_OPCODE_TAG_RZV_RTS)),
			opx_ep->tx->rzv_16B.hdr.qw_16B[3] | psn,
			opx_ep->tx->rzv_16B.hdr.qw_16B[4] | (((uint64_t) data) << 32),
			opx_ep->tx->rzv_16B.hdr.qw_16B[5] | (niov << 48) | FI_OPX_PKT_RZV_FLAGS_NONCONTIG_MASK,
			total_len);
	}

	FI_OPX_HFI1_CLEAR_CREDIT_RETURN(opx_ep);

	/* consume one credit for the packet header */
	--total_credits_available;
	FI_OPX_HFI1_CONSUME_SINGLE_CREDIT(pio_state);
#ifndef NDEBUG
	unsigned credits_consumed = 1;
#endif

	/* write the payload */
	uint64_t *replay_payload = replay->payload;
	assert(!replay->use_iov);
	assert(((uint8_t *) replay_payload) == ((uint8_t *) &replay->data));
	uint64_t	   rem_payload_size;
	uint64_t	  *iov_qws     = (uint64_t *) &hmem_iov[0];
	volatile uint64_t *scb_payload = FI_OPX_HFI1_PIO_SCB_HEAD(opx_ep->tx->pio_scb_first, pio_state);

	if (hfi1_type & (OPX_HFI1_WFR | OPX_HFI1_MIXED_9B)) {
		opx_cacheline_copy_qw_vol(scb_payload, replay_payload, origin_byte_counter_vaddr, iov_qws[0],
					  iov_qws[1], iov_qws[2], iov_qws[3], iov_qws[4], iov_qws[5], iov_qws[6]);
		iov_qws += 7;
		replay_payload += FI_OPX_CACHE_LINE_QWS;
		rem_payload_size = sizeof(struct fi_opx_hmem_iov) * (niov - 2);
	} else {
		opx_cacheline_copy_qw_vol(scb_payload, &replay->scb.qws[8], tag, origin_byte_counter_vaddr, iov_qws[0],
					  iov_qws[1], iov_qws[2], iov_qws[3], iov_qws[4], iov_qws[5]);
		iov_qws += 6;
		rem_payload_size =
			(sizeof(struct fi_opx_hmem_iov) * (niov - 2)) + 8; // overflow 8 bytes from 2nd cacheline
	}

	/* consume one credit for the rendezvous payload metadata */
	--total_credits_available;
	FI_OPX_HFI1_CONSUME_SINGLE_CREDIT(pio_state);
#ifndef NDEBUG
	++credits_consumed;
#endif

	if (payload_blocks_total > 1) {
		assert(niov > 2);

#ifndef NDEBUG
		credits_consumed +=
#endif
			fi_opx_hfi1_tx_egr_store_full_payload_blocks(opx_ep, &pio_state, iov_qws,
								     payload_blocks_total - 1, total_credits_available);

		memcpy(replay_payload, iov_qws, rem_payload_size);
	}

#ifndef NDEBUG
	assert(credits_consumed == total_credits_needed);
#endif

	FI_OPX_HFI1_CHECK_CREDITS_FOR_ERROR(opx_ep->tx->pio_credits_addr);

	/* update the hfi txe state */
	opx_ep->tx->pio_state->qw0 = pio_state.qw0;
	OPX_SHD_CTX_PIO_UNLOCK(OPX_IS_CTX_SHARING_ENABLED, opx_ep->tx);

	fi_opx_reliability_service_replay_register_no_update(opx_ep->reli_service, psn_ptr, replay, reliability,
							     hfi1_type);

	if (OFI_LIKELY(do_cq_completion)) {
		fi_opx_ep_tx_cq_completion_rzv(ep, context, total_len, lock_required, tag, caps);
	}
	OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "SENDV-RZV-RTS-HFI");
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		     "===================================== SENDV, HFI -- RENDEZVOUS RTS (end) context %p\n", context);

	return FI_SUCCESS;
}

ssize_t opx_hfi1_tx_send_rzv(struct fid_ep *ep, const void *buf, size_t len, fi_addr_t dest_addr, uint64_t tag,
			     void *user_context, const uint32_t data, int lock_required, const unsigned override_flags,
			     const uint64_t tx_op_flags, const uint64_t dest_rx, const uint64_t caps,
			     const enum ofi_reliability_kind reliability, const uint64_t do_cq_completion,
			     const enum fi_hmem_iface src_iface, const uint64_t src_device_id,
			     const uint64_t src_handle, const enum opx_hfi1_type hfi1_type, const bool ctx_sharing)
{
	// We should already have grabbed the lock prior to calling this function
	assert(!lock_required);

	// Need at least one full block of payload
	assert(len >= FI_OPX_HFI1_TX_MIN_RZV_PAYLOAD_BYTES);

	struct fi_opx_ep       *opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);
	const union fi_opx_addr addr   = {.fi = dest_addr};

	const uint64_t is_shm = fi_opx_hfi1_tx_is_shm(opx_ep, addr, caps);

	const uint64_t bth_rx	   = ((uint64_t) dest_rx) << OPX_BTH_SUBCTXT_RX_SHIFT;
	const uint64_t lrh_dlid_9B = FI_OPX_ADDR_TO_HFI1_LRH_DLID_9B(addr.lid);

	/* TODO - IPC does not need any of the immediate block calculations.  Determine if moving helps performance or
	 * if the compiler already optimizes this. */
	const bool send_immed_data = (src_iface == FI_HMEM_SYSTEM);

#ifndef NDEBUG
	const uint64_t max_immediate_block_count = (OPX_HFI1_PKT_SIZE >> 6) - 2;
#endif
	/* Expected tid needs to send a leading data block and trailing data
	 * for alignment. TID writes must start on a 64-byte boundary, so we
	 * need to send 64 bytes of leading immediate data that allow us
	 * to shift the receive buffer starting offset to a TID-friendly value.
	 * TID writes must also be a length that is a multiple of a DW (WFR & JKR 9B)
	 * or a QW (JKR), so send the last 7 bytes of the source data immediately
	 * so we can adjust the length after proper alignment has been achieved. */
	const uint8_t immediate_block =
		(send_immed_data && !is_shm && opx_ep->use_expected_tid_rzv &&
		 len >= opx_ep->tx->sdma_min_payload_bytes && len >= opx_ep->tx->tid_min_payload_bytes) ?
			1 :
			0;
	const uint8_t immediate_tail = immediate_block;

	assert(immediate_block <= 1);
	assert(immediate_tail <= 1);
	assert(immediate_block <= max_immediate_block_count);

	const uint8_t immediate_byte_count = send_immed_data ? (uint8_t) (len & 0x0007ul) : 0;
	const uint8_t immediate_qw_count   = send_immed_data ? (uint8_t) ((len >> 3) & 0x0007ul) : 0;
	const uint8_t immediate_fragment   = send_immed_data ? (uint8_t) (((len & 0x003Ful) + 63) >> 6) : 0;
	assert(immediate_fragment == 1 || immediate_fragment == 0);

	/* Immediate total does not include trailing block */
	const uint64_t immediate_total = immediate_byte_count + immediate_qw_count * sizeof(uint64_t) +
					 immediate_block * sizeof(union cacheline);

	union fi_opx_hfi1_rzv_rts_immediate_info immediate_info = {
		.count = (immediate_byte_count << OPX_IMMEDIATE_BYTE_COUNT_SHIFT) |
			 (immediate_qw_count << OPX_IMMEDIATE_QW_COUNT_SHIFT) |
			 (immediate_block << OPX_IMMEDIATE_BLOCK_SHIFT) | (immediate_tail << OPX_IMMEDIATE_TAIL_SHIFT),
		.tail_bytes = {}};

	assert(!send_immed_data || ((len - immediate_total) & 0x003Fu) == 0);

	const uint64_t payload_blocks_total = 1 + /* rzv metadata */
					      immediate_fragment + immediate_block;

	const uint64_t pbc_dws = 2 + /* pbc */
				 2 + /* lhr */
				 3 + /* bth */
				 9 + /* kdeth; from "RcvHdrSize[i].HdrSize" CSR */
				 (payload_blocks_total << 4);

	if (is_shm) {
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
			     "===================================== SEND, SHM -- RENDEZVOUS RTS (begin) context %p\n",
			     user_context);
		OPX_TRACER_TRACE(OPX_TRACER_BEGIN, "SEND-RZV-RTS-SHM");
		uint64_t			 pos;
		ssize_t				 rc;
		union opx_hfi1_packet_hdr *const hdr = opx_shm_tx_next(
			&opx_ep->tx->shm, addr.hfi1_unit, dest_rx, &pos, opx_ep->daos_info.hfi_rank_enabled,
			opx_ep->daos_info.rank, opx_ep->daos_info.rank_inst, &rc);

		if (!hdr) {
			FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "return %zd\n", rc);
			OPX_TRACER_TRACE(OPX_TRACER_END_EAGAIN, "SEND-RZV-RTS-SHM");
			return rc;
		}

		struct opx_context *context;
		uintptr_t	    origin_byte_counter_vaddr;
		if (OFI_LIKELY(do_cq_completion)) {
			context = (struct opx_context *) ofi_buf_alloc(opx_ep->rx->ctx_pool);
			if (OFI_UNLIKELY(context == NULL)) {
				FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA, "Out of memory.\n");
				OPX_TRACER_TRACE(OPX_TRACER_END_EAGAIN, "SEND-RZV-RTS-SHM");
				return -FI_ENOMEM;
			}
			context->err_entry.err	      = 0;
			context->err_entry.op_context = user_context;
			context->next		      = NULL;
			context->byte_counter	      = len - immediate_total;
			origin_byte_counter_vaddr     = (uintptr_t) &context->byte_counter;
		} else {
			context			  = NULL;
			origin_byte_counter_vaddr = (uintptr_t) NULL;
		}

		FI_OPX_DEBUG_COUNTERS_INC_COND(
			src_iface != FI_HMEM_SYSTEM,
			opx_ep->debug_counters.hmem.intranode.kind[(caps & FI_MSG) ? FI_OPX_KIND_MSG : FI_OPX_KIND_TAG]
				.send.rzv);

		const uint64_t opcode =
			(uint64_t) ((caps & FI_MSG) ?
					    ((tx_op_flags & FI_REMOTE_CQ_DATA) ? FI_OPX_HFI_BTH_OPCODE_MSG_RZV_RTS_CQ :
										 FI_OPX_HFI_BTH_OPCODE_MSG_RZV_RTS) :
					    ((tx_op_flags & FI_REMOTE_CQ_DATA) ? FI_OPX_HFI_BTH_OPCODE_TAG_RZV_RTS_CQ :
										 FI_OPX_HFI_BTH_OPCODE_TAG_RZV_RTS));

		union fi_opx_hfi1_packet_payload *const payload = (union fi_opx_hfi1_packet_payload *) (hdr + 1);
		FI_DBG_TRACE(
			fi_opx_global.prov, FI_LOG_EP_DATA,
			"hdr %p, payload %p, sbuf %p, sbuf+immediate_total %p, immediate_total %#lX, adj len %#lX\n",
			hdr, payload, buf, ((char *) buf + immediate_total), immediate_total, (len - immediate_total));

#ifdef OPX_HMEM
		if (opx_ep->use_gpu_ipc == src_iface && src_iface != FI_HMEM_SYSTEM &&
		    len >= OPX_GPU_IPC_MIN_THRESHOLD) {
			int ret = ofi_hmem_get_base_addr(src_iface, buf, len,
							 (void **) &payload->rendezvous.ipc.ipc_info.base_addr,
							 &payload->rendezvous.ipc.ipc_info.base_length);
			if (ret) {
				FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA,
					"Failed to get base addr for IPC handle. Falling back to non-IPC case."
					"Error code: %d\n",
					ret);
				goto non_ipc;
			}

			payload->rendezvous.ipc.origin_byte_counter_vaddr = origin_byte_counter_vaddr;
			payload->rendezvous.ipc.ipc_info.iface		  = src_iface;
			payload->rendezvous.ipc.ipc_info.device		  = src_device_id;
			payload->rendezvous.ipc.ipc_info.offset =
				(uint64_t) buf - (uint64_t) payload->rendezvous.ipc.ipc_info.base_addr;

			OPX_TRACER_TRACE(OPX_TRACER_BEGIN, "IPC-SENDER-CREATE-HANDLE");
			ret = ofi_hmem_get_handle(src_iface, (void *) payload->rendezvous.ipc.ipc_info.base_addr, len,
						  (void **) &payload->rendezvous.ipc.ipc_info.ipc_handle);
			OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "IPC-SENDER-CREATE-HANDLE");

			if (ret) {
				FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA,
					"Failed to create IPC handle for Device. Falling back to non-IPC case."
					"Error code: %d\n",
					ret);
				goto non_ipc;
			}

			const uint16_t lrh_dws =
				__cpu_to_be16(2 + /* lhr */
					      3 + /* bth */
					      9 + /* kdeth; from "RcvHdrSize[i].HdrSize" CSR */
					      (sizeof(payload->rendezvous.ipc) / 4) + 1 /* icrc */); /* (BE: LRH DW) */

			hdr->qw_9B[0] = opx_ep->tx->rzv_9B.hdr.qw_9B[0] | lrh_dlid_9B | ((uint64_t) lrh_dws << 32);
			hdr->qw_9B[1] = opx_ep->tx->rzv_9B.hdr.qw_9B[1] | bth_rx | opcode;
			hdr->qw_9B[2] = opx_ep->tx->rzv_9B.hdr.qw_9B[2];
			hdr->qw_9B[3] = opx_ep->tx->rzv_9B.hdr.qw_9B[3] | (((uint64_t) data) << 32);
			hdr->qw_9B[4] = opx_ep->tx->rzv_9B.hdr.qw_9B[4] | (1ull << 48) /* effectively 1 iov */
					| (FI_OPX_PKT_RZV_FLAGS_IPC_MASK);
			hdr->qw_9B[5] = len;
			hdr->qw_9B[6] = tag;

			opx_shm_tx_advance(&opx_ep->tx->shm, (void *) hdr, pos);

			if (OFI_LIKELY(do_cq_completion)) {
				fi_opx_ep_tx_cq_completion_rzv(ep, context, len, lock_required, tag, caps);
			}

			OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "SEND-RZV-RTS-SHM");
			FI_DBG_TRACE(
				fi_opx_global.prov, FI_LOG_EP_DATA,
				"===================================== SEND, SHM -- RENDEZVOUS RTS (end) context %p\n",
				user_context);
			return FI_SUCCESS;
		}
	non_ipc:;
#endif
		const uint16_t lrh_dws = __cpu_to_be16(
			pbc_dws - 2 +
			1); /* (BE: LRH DW) does not include pbc (8 bytes), but does include icrc (4 bytes) */
		hdr->qw_9B[0] = opx_ep->tx->rzv_9B.hdr.qw_9B[0] | lrh_dlid_9B | ((uint64_t) lrh_dws << 32);
		hdr->qw_9B[1] = opx_ep->tx->rzv_9B.hdr.qw_9B[1] | bth_rx | opcode;
		hdr->qw_9B[2] = opx_ep->tx->rzv_9B.hdr.qw_9B[2];
		hdr->qw_9B[3] = opx_ep->tx->rzv_9B.hdr.qw_9B[3] | (((uint64_t) data) << 32);
		hdr->qw_9B[4] = opx_ep->tx->rzv_9B.hdr.qw_9B[4] | (1ull << 48); /* effectively 1 iov */
		hdr->qw_9B[5] = len;
		hdr->qw_9B[6] = tag;

		struct opx_payload_rzv_contig *contiguous = &payload->rendezvous.contiguous;
		payload->rendezvous.contig_9B_padding	  = 0;
		contiguous->src_vaddr			  = (uintptr_t) buf + immediate_total;
		contiguous->src_len			  = len - immediate_total;
		contiguous->src_device_id		  = src_device_id;
		contiguous->src_iface			  = (uint64_t) src_iface;
		contiguous->immediate_info		  = immediate_info.qw0;
		contiguous->origin_byte_counter_vaddr	  = origin_byte_counter_vaddr;
		contiguous->unused			  = 0;

		if (immediate_total) {
			uint8_t *sbuf;
			if (src_iface != FI_HMEM_SYSTEM) {
				opx_copy_from_hmem(src_iface, src_device_id, src_handle, opx_ep->hmem_copy_buf, buf,
						   immediate_total, OPX_HMEM_DEV_REG_SEND_THRESHOLD);
				sbuf = opx_ep->hmem_copy_buf;
			} else {
				sbuf = (uint8_t *) buf;
			}

			for (int i = 0; i < immediate_byte_count; ++i) {
				contiguous->immediate_byte[i] = sbuf[i];
			}
			sbuf += immediate_byte_count;

			uint64_t *sbuf_qw = (uint64_t *) sbuf;
			for (int i = 0; i < immediate_qw_count; ++i) {
				contiguous->immediate_qw[i] = sbuf_qw[i];
			}

			if (immediate_block) {
				sbuf_qw += immediate_qw_count;
				uint64_t *payload_cacheline =
					(uint64_t *) (&contiguous->cache_line_1 + immediate_fragment);
				opx_cacheline_store_block(payload_cacheline, sbuf_qw);
			}
		}

		opx_shm_tx_advance(&opx_ep->tx->shm, (void *) hdr, pos);

		if (OFI_LIKELY(do_cq_completion)) {
			fi_opx_ep_tx_cq_completion_rzv(ep, context, len, lock_required, tag, caps);
		}

		OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "SEND-RZV-RTS-SHM");
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
			     "===================================== SEND, SHM -- RENDEZVOUS RTS (end) context %p\n",
			     user_context);

		return FI_SUCCESS;
	}
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		     "===================================== SEND, HFI -- RENDEZVOUS RTS (begin) context %p\n",
		     user_context);
	OPX_TRACER_TRACE(OPX_TRACER_BEGIN, "SEND-RZV-RTS-HFI:%ld", tag);

	/*
	 * While the bulk of the payload data will be sent via SDMA once we
	 * get the CTS from the receiver, the initial RTS packet is sent via PIO.
	 */

	OPX_SHD_CTX_PIO_LOCK(OPX_IS_CTX_SHARING_ENABLED, opx_ep->tx);

	union fi_opx_hfi1_pio_state pio_state = *opx_ep->tx->pio_state;

	const uint16_t total_credits_needed = 1 +		    /* packet header */
					      payload_blocks_total; /* packet payload */

	uint64_t total_credits_available =
		FI_OPX_HFI1_AVAILABLE_CREDITS(pio_state, &opx_ep->tx->force_credit_return, total_credits_needed);
	if (OFI_UNLIKELY(total_credits_available < total_credits_needed)) {
		FI_OPX_HFI1_UPDATE_CREDITS(pio_state, opx_ep->tx->pio_credits_addr);
		total_credits_available = FI_OPX_HFI1_AVAILABLE_CREDITS(pio_state, &opx_ep->tx->force_credit_return,
									total_credits_needed);
		if (total_credits_available < total_credits_needed) {
			opx_ep->tx->pio_state->qw0 = pio_state.qw0;
			OPX_TRACER_TRACE(OPX_TRACER_END_EAGAIN, "SEND-RZV-RTS-HFI:%ld", tag);
			OPX_SHD_CTX_PIO_UNLOCK(OPX_IS_CTX_SHARING_ENABLED, opx_ep->tx);
			return -FI_EAGAIN;
		}
	}

	struct opx_context *context;
	uintptr_t	    origin_byte_counter_vaddr;
	if (OFI_LIKELY(do_cq_completion)) {
		context = (struct opx_context *) ofi_buf_alloc(opx_ep->rx->ctx_pool);
		if (OFI_UNLIKELY(context == NULL)) {
			FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA, "Out of memory.\n");
			opx_ep->tx->pio_state->qw0 = pio_state.qw0;
			OPX_TRACER_TRACE(OPX_TRACER_END_EAGAIN, "SEND-RZV-RTS-HFI:%ld", tag);
			OPX_SHD_CTX_PIO_UNLOCK(OPX_IS_CTX_SHARING_ENABLED, opx_ep->tx);
			return -FI_ENOMEM;
		}
		context->err_entry.err	      = 0;
		context->err_entry.op_context = user_context;
		context->next		      = NULL;
		context->byte_counter	      = len - immediate_total;
		origin_byte_counter_vaddr     = (uintptr_t) &context->byte_counter;
	} else {
		context			  = NULL;
		origin_byte_counter_vaddr = (uintptr_t) NULL;
	}

	struct fi_opx_reliability_tx_replay *replay;
	union fi_opx_reliability_tx_psn	    *psn_ptr;
	int64_t				     psn;

	psn = fi_opx_reliability_get_replay(&opx_ep->ep_fid, opx_ep->reli_service, addr.lid, addr.hfi1_subctxt_rx,
					    &psn_ptr, &replay, reliability, hfi1_type);
	if (OFI_UNLIKELY(psn == -1)) {
		if (OFI_LIKELY(do_cq_completion)) {
			OPX_BUF_FREE(context);
		}
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "FI_EAGAIN\n");
		opx_ep->tx->pio_state->qw0 = pio_state.qw0;
		OPX_TRACER_TRACE(OPX_TRACER_END_EAGAIN, "SEND-RZV-RTS-HFI:%ld", tag);
		OPX_SHD_CTX_PIO_UNLOCK(OPX_IS_CTX_SHARING_ENABLED, opx_ep->tx);
		return -FI_EAGAIN;
	}

	FI_OPX_DEBUG_COUNTERS_INC_COND(
		src_iface != FI_HMEM_SYSTEM,
		opx_ep->debug_counters.hmem.hfi.kind[(caps & FI_MSG) ? FI_OPX_KIND_MSG : FI_OPX_KIND_TAG].send.rzv);

	if (immediate_tail) {
		uint8_t *buf_tail_bytes = ((uint8_t *) buf + len) - OPX_IMMEDIATE_TAIL_BYTE_COUNT;
		if (src_iface != FI_HMEM_SYSTEM) {
			opx_copy_from_hmem(src_iface, src_device_id, src_handle, opx_ep->hmem_copy_buf, buf_tail_bytes,
					   OPX_IMMEDIATE_TAIL_BYTE_COUNT, OPX_HMEM_DEV_REG_SEND_THRESHOLD);
			buf_tail_bytes = opx_ep->hmem_copy_buf;
		}

		for (int i = 0; i < OPX_IMMEDIATE_TAIL_BYTE_COUNT; ++i) {
			immediate_info.tail_bytes[i] = buf_tail_bytes[i];
		}
	}

	/*
	 * Write the 'start of packet' (hw+sw header) 'send control block'
	 * which will consume a single pio credit.
	 */

	uint64_t       force_credit_return = OPX_PBC_CR(opx_ep->tx->force_credit_return, hfi1_type);
	const uint64_t opcode =
		(uint64_t) ((caps & FI_MSG) ?
				    ((tx_op_flags & FI_REMOTE_CQ_DATA) ? FI_OPX_HFI_BTH_OPCODE_MSG_RZV_RTS_CQ :
									 FI_OPX_HFI_BTH_OPCODE_MSG_RZV_RTS) :
				    ((tx_op_flags & FI_REMOTE_CQ_DATA) ? FI_OPX_HFI_BTH_OPCODE_TAG_RZV_RTS_CQ :
									 FI_OPX_HFI_BTH_OPCODE_TAG_RZV_RTS));

	volatile uint64_t *const scb = FI_OPX_HFI1_PIO_SCB_HEAD(opx_ep->tx->pio_scb_sop_first, pio_state);

	const uint16_t lrh_dws = __cpu_to_be16(
		pbc_dws - 2 + 1); /* (BE: LRH DW) does not include pbc (8 bytes), but does include icrc (4 bytes) */
	const uint64_t pbc_dlid = OPX_PBC_DLID(addr.lid, hfi1_type);
	opx_cacheline_copy_qw_vol(scb, replay->scb.qws,
				  opx_ep->tx->rzv_9B.qw0 | OPX_PBC_LEN(pbc_dws, hfi1_type) | force_credit_return |
					  pbc_dlid | OPX_PBC_LOOPBACK(pbc_dlid, hfi1_type),
				  opx_ep->tx->rzv_9B.hdr.qw_9B[0] | lrh_dlid_9B | ((uint64_t) lrh_dws << 32),
				  opx_ep->tx->rzv_9B.hdr.qw_9B[1] | bth_rx | opcode,
				  opx_ep->tx->rzv_9B.hdr.qw_9B[2] | psn,
				  opx_ep->tx->rzv_9B.hdr.qw_9B[3] | (((uint64_t) data) << 32),
				  opx_ep->tx->rzv_9B.hdr.qw_9B[4] | (1ull << 48), len, tag);

	/* consume one credit for the packet header */
	FI_OPX_HFI1_CONSUME_SINGLE_CREDIT(pio_state);
#ifndef NDEBUG
	unsigned credits_consumed = 1;
#endif

	FI_OPX_HFI1_CLEAR_CREDIT_RETURN(opx_ep);

	uint64_t *replay_payload = replay->payload;

	assert(!replay->use_iov);
	assert(((uint8_t *) replay_payload) == ((uint8_t *) &replay->data));

	/*
	 * write the rendezvous payload "send control blocks"
	 */

	volatile uint64_t *scb_payload = FI_OPX_HFI1_PIO_SCB_HEAD(opx_ep->tx->pio_scb_first, pio_state);
	opx_cacheline_copy_qw_vol(scb_payload, replay->payload, 0,   /* contig_9B_padding */
				  (uintptr_t) buf + immediate_total, /* src_vaddr */
				  (len - immediate_total),	     /* src_len */
				  src_device_id, (uint64_t) src_iface, immediate_info.qw0, origin_byte_counter_vaddr,
				  0 /* unused */);

	/* consume one credit for the rendezvous payload metadata */
	FI_OPX_HFI1_CONSUME_SINGLE_CREDIT(pio_state);
#ifndef NDEBUG
	++credits_consumed;
#endif

	replay_payload += FI_OPX_CACHE_LINE_QWS;

	uint8_t *sbuf;
	if (src_iface != FI_HMEM_SYSTEM && immediate_total) {
		opx_copy_from_hmem(src_iface, src_device_id, src_handle, opx_ep->hmem_copy_buf, buf, immediate_total,
				   OPX_HMEM_DEV_REG_SEND_THRESHOLD);
		sbuf = opx_ep->hmem_copy_buf;
	} else {
		sbuf = (uint8_t *) buf;
	}

	scb_payload = FI_OPX_HFI1_PIO_SCB_HEAD(opx_ep->tx->pio_scb_first, pio_state);

	/* immediate_byte and immediate_qw are "packed" in the current implementation             */
	/* meaning the immediate bytes are filled, then followed by the rest of the data directly */
	/* adjacent to the packed bytes.  It's probably more efficient to leave a pad and not go  */
	/* through the confusion of finding these boundaries on both sides of the rendezvous      */
	/* That is, just pack the immediate bytes, then pack the "rest" in the immediate qws      */
	/* This would lead to more efficient packing on both sides at the expense of              */
	/* wasting space of a common 0 byte immediate                                             */
	/* tmp_payload_t represents the second cache line of the rts packet                       */
	/* fi_opx_hfi1_packet_payload -> rendezvous -> contiguous                                 */
	struct tmp_payload_t {
		uint8_t	 immediate_byte[8];
		uint64_t immediate_qw[7];
	} __attribute__((packed));

	uint64_t *sbuf_qw = (uint64_t *) (sbuf + immediate_byte_count);
	if (immediate_fragment) {
		struct tmp_payload_t *tmp_payload = (struct tmp_payload_t *) replay_payload;

		for (int i = 0; i < immediate_byte_count; ++i) {
			tmp_payload->immediate_byte[i] = sbuf[i];
		}

		for (int i = 0; i < immediate_qw_count; ++i) {
			tmp_payload->immediate_qw[i] = sbuf_qw[i];
		}
		opx_cacheline_store_block_vol(scb_payload, replay_payload);
		sbuf_qw += immediate_qw_count;

		replay_payload += FI_OPX_CACHE_LINE_QWS;

		/* consume one credit for the rendezvous payload immediate data */
		FI_OPX_HFI1_CONSUME_SINGLE_CREDIT(pio_state);
#ifndef NDEBUG
		++credits_consumed;
#endif
	}

	if (immediate_block) {
		scb_payload = FI_OPX_HFI1_PIO_SCB_HEAD(opx_ep->tx->pio_scb_first, pio_state);
		opx_cacheline_copy_block_vol(scb_payload, replay_payload, sbuf_qw);

		FI_OPX_HFI1_CONSUME_SINGLE_CREDIT(pio_state);
#ifndef NDEBUG
		++credits_consumed;
#endif
	}

#ifndef NDEBUG
	assert(credits_consumed == total_credits_needed);
#endif
	FI_OPX_HFI1_CHECK_CREDITS_FOR_ERROR(opx_ep->tx->pio_credits_addr);

	/* update the hfi txe state */
	opx_ep->tx->pio_state->qw0 = pio_state.qw0;
	OPX_SHD_CTX_PIO_UNLOCK(OPX_IS_CTX_SHARING_ENABLED, opx_ep->tx);

	fi_opx_reliability_service_replay_register_no_update(opx_ep->reli_service, psn_ptr, replay, reliability,
							     hfi1_type);

	if (OFI_LIKELY(do_cq_completion)) {
		fi_opx_ep_tx_cq_completion_rzv(ep, context, len, lock_required, tag, caps);
	}

	OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "SEND-RZV-RTS-HFI:%ld", tag);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		     "===================================== SEND, HFI -- RENDEZVOUS RTS (end) context %p\n",
		     user_context);

	return FI_SUCCESS;
}

ssize_t opx_hfi1_tx_rzv_rts_hfisvc(struct fi_opx_ep *opx_ep, const void *buf, const size_t xfer_len, const uint64_t tag,
				   const uint32_t data, uint64_t bth_rx, uint64_t lrh_dlid_16B,
				   const uint64_t do_cq_completion, void *user_context, union fi_opx_addr addr,
				   const uint64_t caps, const enum fi_hmem_iface src_iface,
				   const uint64_t src_device_id, uint64_t tx_op_flags, const bool ctx_sharing)
{
#if HAVE_HFISVC
	FI_DBG_TRACE(
		fi_opx_global.prov, FI_LOG_EP_DATA,
		"===================================== SEND 16B, HFI -- RENDEZVOUS RTS HFISVC (begin) context %p\n",
		user_context);
	OPX_TRACER_TRACE(OPX_TRACER_BEGIN, "SEND-RZV-RTS-HFI:%ld", tag);
	FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.hfisvc.rzv_send_rts.attempt);
	struct fi_opx_ep_tx *tx = opx_ep->tx;

	uint32_t			     access_key = (uint32_t) -1;
	struct fi_opx_rzv_completion	    *rzv_comp	= NULL;
	struct opx_context		    *context	= NULL;
	struct fi_opx_reliability_tx_replay *replay	= NULL;
	union fi_opx_reliability_tx_psn	    *psn_ptr	= NULL;
	ssize_t				     rc		= FI_SUCCESS;

	OPX_SHD_CTX_PIO_LOCK(ctx_sharing, opx_ep->tx);
	union fi_opx_hfi1_pio_state pio_state = *tx->pio_state;

	if (opx_hfisvc_keyset_alloc_key(opx_ep->domain->hfisvc.access_key_set, &access_key,
					FI_OPX_DEBUG_COUNTERS_GET_PTR(opx_ep))) {
		OPX_HFISVC_DEBUG_LOG("EAGAIN (No free keys)\n");
		FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.hfisvc.rzv_send_rts.eagain_access_key);
		rc = -FI_EAGAIN;
		goto err;
	}

	rzv_comp = (struct fi_opx_rzv_completion *) ofi_buf_alloc(opx_ep->rzv_completion_pool);
	if (OFI_UNLIKELY(rzv_comp == NULL)) {
		OPX_HFISVC_DEBUG_LOG("ENOMEM (rzv_comp)\n");
		FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.hfisvc.rzv_send_rts.enomem_completion);
		rc = -FI_ENOMEM;
		goto err;
	}

	const uint16_t credits_needed = 2;

	uint16_t credits_available =
		FI_OPX_HFI1_AVAILABLE_CREDITS(pio_state, &opx_ep->tx->force_credit_return, credits_needed);
	if (OFI_UNLIKELY(credits_available < credits_needed)) {
		FI_OPX_HFI1_UPDATE_CREDITS(pio_state, opx_ep->tx->pio_credits_addr);

		credits_available =
			FI_OPX_HFI1_AVAILABLE_CREDITS(pio_state, &opx_ep->tx->force_credit_return, credits_needed);
		if (credits_available < credits_needed) {
			OPX_HFISVC_DEBUG_LOG("EAGAIN (credits)\n");
			FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.hfisvc.rzv_send_rts.eagain_credits);
			rc = -FI_EAGAIN;
			goto err;
		}
	}

	if (OFI_LIKELY(do_cq_completion)) {
		context = (struct opx_context *) ofi_buf_alloc(
			opx_ep->rx->ctx_pool); // What the heck!? why do we allocate contexts from the rx side for tx
					       // side completions?
					       // Because that's just where the context buf pool lives. The buf pool
					       // could be moved to the top level endpoint struct instead...
		if (OFI_UNLIKELY(context == NULL)) {
			OPX_HFISVC_DEBUG_LOG("ENOMEM (context)\n");
			FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA, "Out of memory.\n");
			FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.hfisvc.rzv_send_rts.enomem_context);
			rc = -FI_ENOMEM;
			goto err;
		}
		context->err_entry.err	      = 0;
		context->err_entry.op_context = user_context;
		context->next		      = NULL;
		context->byte_counter	      = xfer_len;
	} else {
		context = NULL;
	}

	rzv_comp->context = context;

	int64_t psn =
		fi_opx_reliability_get_replay(&opx_ep->ep_fid, opx_ep->reli_service, addr.lid, addr.hfi1_subctxt_rx,
					      &psn_ptr, &replay, OFI_RELIABILITY_KIND_ONLOAD, OPX_HFI1_JKR);
	if (OFI_UNLIKELY(psn == -1)) {
		OPX_HFISVC_DEBUG_LOG("EAGAIN (PSN)\n");
		FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.hfisvc.rzv_send_rts.eagain_psn);
		rc = -FI_EAGAIN;
		goto err;
	}

	rzv_comp->access_key = access_key;

	struct hfisvc_client_completion completion = {
		.flags		= HFISVC_CLIENT_COMPLETION_FLAG_CQ,
		.cq.handle	= opx_ep->hfisvc.internal_completion_queue,
		.cq.app_context = (uint64_t) rzv_comp,
	};
	rc = (*opx_ep->domain->hfisvc.cmd_dma_access_once_va)(opx_ep->hfisvc.command_queue, completion, 0UL /* flags */,
							      access_key, xfer_len, (void *) buf);

	if (OFI_UNLIKELY(rc != 0)) {
		OPX_HFISVC_DEBUG_LOG("EAGAIN (hfisvc_client queue returned %ld)\n", rc);
		FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.hfisvc.rzv_send_rts.eagain_hfisvc);
		rc = -FI_EAGAIN;
		goto err;
	}

	OPX_HFISVC_DEBUG_LOG(
		"HFISVC RZV Send RTS: Successfully registered DMA buf=%p len=%lu access_key=%u rzv_comp=%p context=%p\n",
		buf, xfer_len, access_key, rzv_comp, context);
	FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.hfisvc.rzv_send_rts.reg_dma_buf);

	rc = (*opx_ep->domain->hfisvc.doorbell)(opx_ep->domain->hfisvc.ctx);

	assert(rc == 0);

	// Put the context on the pending queue before we send out the RTS packet to ensure it's
	// queued before the remote rdma_read completes.
	if (OFI_LIKELY(do_cq_completion)) {
		fi_opx_ep_tx_cq_completion_rzv(&opx_ep->ep_fid, context, xfer_len, FI_OPX_LOCK_NOT_REQUIRED, tag, caps);
	}

	union opx_hfisvc_iov hfisvc_iov = {
		.access_key  = access_key,
		.len	     = xfer_len,
		.hmem_iface  = src_iface,
		.hmem_device = src_device_id,
		.offset	     = 0UL,
	};

	const uint32_t niov    = 1;
	const uint64_t pbc_dws = 2 /* pbc */ + 30;   /* rzv_rts */
	const uint16_t lrh_qws = (pbc_dws - 2) >> 1; /* (LRH QW) does not include pbc (8 bytes) */
	const uint64_t opcode =
		(uint64_t) ((caps & FI_MSG) ?
				    ((tx_op_flags & FI_REMOTE_CQ_DATA) ? FI_OPX_HFI_BTH_OPCODE_MSG_RZV_RTS_HFISVC_CQ :
									 FI_OPX_HFI_BTH_OPCODE_MSG_RZV_RTS_HFISVC) :
				    ((tx_op_flags & FI_REMOTE_CQ_DATA) ? FI_OPX_HFI_BTH_OPCODE_TAG_RZV_RTS_HFISVC_CQ :
									 FI_OPX_HFI_BTH_OPCODE_TAG_RZV_RTS_HFISVC));
	const uint64_t niov_client_key = ((uint64_t) niov << 32) | (uint64_t) opx_ep->domain->hfisvc.key;

	const uint64_t		 pbc_dlid = OPX_PBC_DLID(addr.lid, OPX_HFI1_JKR);
	volatile uint64_t *const scb	  = FI_OPX_HFI1_PIO_SCB_HEAD(tx->pio_scb_sop_first, pio_state);

	opx_cacheline_copy_qw_vol(
		scb, replay->scb.qws,
		tx->rzv_16B.qw0 | OPX_PBC_LEN(pbc_dws, OPX_HFI1_JKR) |
			OPX_PBC_CR(opx_ep->tx->force_credit_return, hfi1_type) | pbc_dlid |
			OPX_PBC_LOOPBACK(pbc_dlid, OPX_HFI1_JKR),
		tx->rzv_16B.hdr.qw_16B[0] |
			((uint64_t) (lrh_dlid_16B & OPX_LRH_JKR_16B_DLID_MASK_16B) << OPX_LRH_JKR_16B_DLID_SHIFT_16B) |
			((uint64_t) lrh_qws << 20),
		tx->rzv_16B.hdr.qw_16B[1] |
			((uint64_t) ((lrh_dlid_16B & OPX_LRH_JKR_16B_DLID20_MASK_16B) >>
				     OPX_LRH_JKR_16B_DLID20_SHIFT_16B)) |
			(uint64_t) (bth_rx >> OPX_LRH_JKR_BTH_RX_ENTROPY_SHIFT_16B),
		tx->rzv_16B.hdr.qw_16B[2] | bth_rx | opcode, tx->rzv_16B.hdr.qw_16B[3] | psn,
		tx->rzv_16B.hdr.qw_16B[4] | (((uint64_t) data) << 32), niov_client_key, xfer_len);

	FI_OPX_HFI1_CONSUME_SINGLE_CREDIT(pio_state);

	volatile uint64_t *scb_payload = FI_OPX_HFI1_PIO_SCB_HEAD(tx->pio_scb_first, pio_state);

	opx_cacheline_copy_qw_vol(scb_payload, &replay->scb.qws[8], tag, hfisvc_iov.qws[0], hfisvc_iov.qws[1],
				  hfisvc_iov.qws[2], hfisvc_iov.qws[3], OPX_JKR_16B_PAD_QWORD, OPX_JKR_16B_PAD_QWORD,
				  OPX_JKR_16B_PAD_QWORD);

	FI_OPX_HFI1_CONSUME_SINGLE_CREDIT(pio_state);

	FI_OPX_HFI1_CLEAR_CREDIT_RETURN(opx_ep);
	uint64_t *payload = replay->payload;
	for (int i = 0; i < (sizeof(union opx_hfisvc_iov) >> 3); i++) {
		payload[i] = hfisvc_iov.qws[i];
	}

	fi_opx_reliability_service_replay_register_no_update(opx_ep->reli_service, psn_ptr, replay,
							     OFI_RELIABILITY_KIND_ONLOAD, OPX_HFI1_JKR);

	tx->pio_state->qw0 = pio_state.qw0;
	FI_OPX_HFI1_CHECK_CREDITS_FOR_ERROR(tx->pio_credits_addr);
	OPX_SHD_CTX_PIO_UNLOCK(ctx_sharing, opx_ep->tx);
	OPX_HFISVC_DEBUG_LOG("Send RZV RTS SUCCESS\n");
	FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.hfisvc.rzv_send_rts.success);
	OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "SEND-RZV-RTS-HFI:%ld", tag);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		     "===================================== SEND 16B, HFI -- RENDEZVOUS RTS HFISVC (end) context %p\n",
		     user_context);

	return FI_SUCCESS;

err:
	tx->pio_state->qw0 = pio_state.qw0;
	OPX_SHD_CTX_PIO_UNLOCK(ctx_sharing, opx_ep->tx);

	if (replay) {
		// If we successfully allocated a replay, we should also have a PSN
		assert(psn_ptr != NULL);
		assert(psn >= 0 && psn <= MAX_PSN);
		fi_opx_reliability_tx_return_psn(psn_ptr, (uint32_t) psn);
		fi_opx_reliability_service_replay_deallocate(opx_ep->reli_service, replay);
	}
	if (context) {
		OPX_BUF_FREE(context);
	}
	if (rzv_comp) {
		OPX_BUF_FREE(rzv_comp);
	}
	if ((int32_t) access_key >= 0) {
		opx_hfisvc_keyset_free_key(opx_ep->domain->hfisvc.access_key_set, access_key,
					   FI_OPX_DEBUG_COUNTERS_GET_PTR(opx_ep));
	}
	OPX_TRACER_TRACE(OPX_TRACER_END_EAGAIN, "SEND-RZV-RTS-HFI:%ld", tag);
	return rc;
#else
	return FI_SUCCESS;
#endif
}

ssize_t opx_hfi1_tx_send_rzv_16B(struct fid_ep *ep, const void *buf, size_t len, fi_addr_t dest_addr, uint64_t tag,
				 void *user_context, const uint32_t data, int lock_required,
				 const unsigned override_flags, const uint64_t tx_op_flags, const uint64_t dest_rx,
				 const uint64_t caps, const enum ofi_reliability_kind reliability,
				 const uint64_t do_cq_completion, const enum fi_hmem_iface src_iface,
				 const uint64_t src_device_id, const uint64_t src_handle,
				 const enum opx_hfi1_type hfi1_type, const bool ctx_sharing)
{
	// We should already have grabbed the lock prior to calling this function
	assert(!lock_required);

	// Need at least one full block of payload
	assert(len >= FI_OPX_HFI1_TX_MIN_RZV_PAYLOAD_BYTES);

	struct fi_opx_ep       *opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);
	const union fi_opx_addr addr   = {.fi = dest_addr};

	const uint64_t is_shm = fi_opx_hfi1_tx_is_shm(opx_ep, addr, caps);

	const uint64_t bth_rx	    = ((uint64_t) dest_rx) << OPX_BTH_SUBCTXT_RX_SHIFT;
	const uint64_t lrh_dlid_16B = addr.lid;

#if HAVE_HFISVC
	if (opx_ep->use_hfisvc && !is_shm && (src_iface == FI_HMEM_SYSTEM) && (hfi1_type & OPX_HFI1_JKR) &&
	    !(caps & FI_MSG)) {
		OPX_HFISVC_DEBUG_LOG("Sending RZV RTS HFI SVC\n");
		return opx_hfi1_tx_rzv_rts_hfisvc(opx_ep, buf, len, tag, data, bth_rx, lrh_dlid_16B, do_cq_completion,
						  user_context, addr, caps, src_iface, src_device_id, tx_op_flags,
						  ctx_sharing);
	}
#endif
	OPX_HFISVC_DEBUG_LOG("Sending RZV RTS NORMAL\n");

	const bool send_immed_data = (src_iface == FI_HMEM_SYSTEM);

#ifndef NDEBUG
	const uint64_t max_immediate_block_count = (OPX_HFI1_PKT_SIZE >> 6) - 2;
#endif
	/* Expected tid needs to send a leading data block and trailing data
	 * for alignment. TID writes must start on a 64-byte boundary, so we
	 * need to send 64 bytes of leading immediate data that allow us
	 * to shift the receive buffer starting offset to a TID-friendly value.
	 * TID writes must also be a length that is a multiple of a DW (WFR & JKR 9B)
	 * or a QW (JKR), so send the last 7 bytes of the source data immediately
	 * so we can adjust the length after proper alignment has been achieved. */
	const uint8_t immediate_block =
		(send_immed_data && !is_shm && opx_ep->use_expected_tid_rzv &&
		 len >= opx_ep->tx->sdma_min_payload_bytes && len >= opx_ep->tx->tid_min_payload_bytes) ?
			1 :
			0;
	const uint8_t immediate_tail = immediate_block;

	assert(immediate_block <= 1);
	assert(immediate_tail <= 1);
	assert(immediate_block <= max_immediate_block_count);

	const uint8_t immediate_byte_count = send_immed_data ? (uint8_t) (len & 0x0007ul) : 0;
	const uint8_t immediate_qw_count   = send_immed_data ? (uint8_t) ((len >> 3) & 0x0007ul) : 0;
	const uint8_t immediate_fragment   = send_immed_data ? (uint8_t) (((len & 0x003Ful) + 63) >> 6) : 0;
	assert(immediate_fragment == 1 || immediate_fragment == 0);

	/* Need a full block for ICRC after the end block... */
	const uint64_t icrc_end_block = immediate_block;

	/* ... otherwise need a qw (or block) in the immediate fragment */
	const uint64_t icrc_fragment = icrc_end_block ? 0 : immediate_fragment;

	/* if there are already 7 qw's need a full block */
	const uint64_t icrc_fragment_block = icrc_fragment && (immediate_qw_count == 7) ? 1 : 0;

	/* Summary: we can add the tail qw in...
	 * - rzv metadata if there is no other immediate data
	 * - an empty fragment qw if there are no other blocks (icrc_fragment & !icrc_fragment_block)
	 * - a full (additional) fragment block if there are no other blocks (icrc_fragment & icrc_fragment_block)
	 * - a full (additional) trailing block after the end (icrc_end_block)
	 */

	/* Immediate total does not include trailing block */
	const uint64_t immediate_total = immediate_byte_count + immediate_qw_count * sizeof(uint64_t) +
					 immediate_block * sizeof(union cacheline);

	union fi_opx_hfi1_rzv_rts_immediate_info immediate_info = {
		.count = (immediate_byte_count << OPX_IMMEDIATE_BYTE_COUNT_SHIFT) |
			 (immediate_qw_count << OPX_IMMEDIATE_QW_COUNT_SHIFT) |
			 (immediate_block << OPX_IMMEDIATE_BLOCK_SHIFT) | (immediate_tail << OPX_IMMEDIATE_TAIL_SHIFT),
		.tail_bytes = {}};

	assert(icrc_end_block + icrc_fragment_block < 2); /* not both */
	assert(((len - immediate_total) & 0x003Fu) == 0);

	/* full blocks only. icrc_end_block/icrc_fragment_block count 1 qw only */
	const uint64_t payload_blocks_total = 1 + /* last kdeth + rzv metadata */
					      immediate_fragment + immediate_block;

	const uint64_t pbc_dws = 2 + /* pbc */
				 4 + /* lhr */
				 3 + /* bth */
				 /* 9 +  kdeth; from "RcvHdrSize[i].HdrSize" CSR */
				 7 +			       /* kdeth */
				 (payload_blocks_total << 4) + /* includes last kdeth + metadata + immediate data */
				 ((icrc_end_block | icrc_fragment_block) << 1); /* 1 QW of any added tail block */

	if (is_shm) {
		FI_DBG_TRACE(
			fi_opx_global.prov, FI_LOG_EP_DATA,
			"===================================== SEND 16B, SHM -- RENDEZVOUS RTS (begin) context %p\n",
			user_context);
		OPX_TRACER_TRACE(OPX_TRACER_BEGIN, "SEND-RZV-RTS-SHM");
		uint64_t			 pos;
		ssize_t				 rc;
		union opx_hfi1_packet_hdr *const hdr = opx_shm_tx_next(
			&opx_ep->tx->shm, addr.hfi1_unit, dest_rx, &pos, opx_ep->daos_info.hfi_rank_enabled,
			opx_ep->daos_info.rank, opx_ep->daos_info.rank_inst, &rc);

		if (!hdr) {
			FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "return %zd\n", rc);
			OPX_TRACER_TRACE(OPX_TRACER_END_EAGAIN, "SEND-RZV-RTS-SHM");
			return rc;
		}

		struct opx_context *context;
		uintptr_t	    origin_byte_counter_vaddr;
		if (OFI_LIKELY(do_cq_completion)) {
			context = (struct opx_context *) ofi_buf_alloc(opx_ep->rx->ctx_pool);
			if (OFI_UNLIKELY(context == NULL)) {
				FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA, "Out of memory.\n");
				OPX_TRACER_TRACE(OPX_TRACER_END_EAGAIN, "SEND-RZV-RTS-SHM");
				return -FI_ENOMEM;
			}
			context->err_entry.err	      = 0;
			context->err_entry.op_context = user_context;
			context->next		      = NULL;
			context->byte_counter	      = len - immediate_total;
			origin_byte_counter_vaddr     = (uintptr_t) &context->byte_counter;
		} else {
			context			  = NULL;
			origin_byte_counter_vaddr = (uintptr_t) NULL;
		}

		FI_OPX_DEBUG_COUNTERS_INC_COND(
			src_iface != FI_HMEM_SYSTEM,
			opx_ep->debug_counters.hmem.intranode.kind[(caps & FI_MSG) ? FI_OPX_KIND_MSG : FI_OPX_KIND_TAG]
				.send.rzv);

		const uint64_t opcode =
			(uint64_t) ((caps & FI_MSG) ?
					    ((tx_op_flags & FI_REMOTE_CQ_DATA) ? FI_OPX_HFI_BTH_OPCODE_MSG_RZV_RTS_CQ :
										 FI_OPX_HFI_BTH_OPCODE_MSG_RZV_RTS) :
					    ((tx_op_flags & FI_REMOTE_CQ_DATA) ? FI_OPX_HFI_BTH_OPCODE_TAG_RZV_RTS_CQ :
										 FI_OPX_HFI_BTH_OPCODE_TAG_RZV_RTS));

		union fi_opx_hfi1_packet_payload *const payload = (union fi_opx_hfi1_packet_payload *) (hdr + 1);

#ifdef OPX_HMEM
		if (opx_ep->use_gpu_ipc == src_iface && src_iface != FI_HMEM_SYSTEM &&
		    len >= OPX_GPU_IPC_MIN_THRESHOLD) {
			int ret = ofi_hmem_get_base_addr(src_iface, buf, len,
							 (void **) &payload->rendezvous.ipc.ipc_info.base_addr,
							 &payload->rendezvous.ipc.ipc_info.base_length);
			if (ret) {
				FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA,
					"Failed to get base addr for IPC handle. Falling back to non-IPC case."
					"Error code: %d\n",
					ret);
				goto non_ipc;
			}

			payload->rendezvous.ipc.origin_byte_counter_vaddr = origin_byte_counter_vaddr;
			payload->rendezvous.ipc.ipc_info.iface		  = src_iface;
			payload->rendezvous.ipc.ipc_info.device		  = src_device_id;
			payload->rendezvous.ipc.ipc_info.offset =
				(uint64_t) buf - (uint64_t) payload->rendezvous.ipc.ipc_info.base_addr;

			OPX_TRACER_TRACE(OPX_TRACER_BEGIN, "IPC-SENDER-CREATE-HANDLE");
			ret = ofi_hmem_get_handle(src_iface, (void *) payload->rendezvous.ipc.ipc_info.base_addr, len,
						  (void **) &payload->rendezvous.ipc.ipc_info.ipc_handle);
			OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "IPC-SENDER-CREATE-HANDLE");

			if (ret) {
				FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA,
					"Failed to create IPC handle for Device. Falling back to non-IPC case."
					"Error code: %d\n",
					ret);
				goto non_ipc;
			}

			const uint16_t lrh_qws =
				8 + /* 2 lhr + 1.5 bth + 4.5 kdeth from "RcvHdrSize[i].HdrSize" CSR*/
				(sizeof(payload->rendezvous.ipc) / 8); /* (LRH QW) does not include pbc (8 bytes) */
			hdr->qw_16B[0] = opx_ep->tx->rzv_16B.hdr.qw_16B[0] |
					 ((uint64_t) (lrh_dlid_16B & OPX_LRH_JKR_16B_DLID_MASK_16B)
					  << OPX_LRH_JKR_16B_DLID_SHIFT_16B) |
					 ((uint64_t) lrh_qws << 20);

			hdr->qw_16B[1] = opx_ep->tx->rzv_16B.hdr.qw_16B[1] |
					 ((uint64_t) ((lrh_dlid_16B & OPX_LRH_JKR_16B_DLID20_MASK_16B) >>
						      OPX_LRH_JKR_16B_DLID20_SHIFT_16B)) |
					 (uint64_t) (bth_rx >> OPX_LRH_JKR_BTH_RX_ENTROPY_SHIFT_16B);

			hdr->qw_16B[2] = opx_ep->tx->rzv_16B.hdr.qw_16B[2] | bth_rx | opcode;
			hdr->qw_16B[3] = opx_ep->tx->rzv_16B.hdr.qw_16B[3];
			hdr->qw_16B[4] = opx_ep->tx->rzv_16B.hdr.qw_16B[4] | (((uint64_t) data) << 32);
			hdr->qw_16B[5] = opx_ep->tx->rzv_16B.hdr.qw_16B[5] | (1ull << 48) /* effectively 1 iov */
					 | (FI_OPX_PKT_RZV_FLAGS_IPC_MASK);
			hdr->qw_16B[6] = len;
			hdr->qw_16B[7] = tag;

			opx_shm_tx_advance(&opx_ep->tx->shm, (void *) hdr, pos);

			if (OFI_LIKELY(do_cq_completion)) {
				fi_opx_ep_tx_cq_completion_rzv(ep, context, len, lock_required, tag, caps);
			}

			OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "SEND-RZV-RTS-SHM");
			FI_DBG_TRACE(
				fi_opx_global.prov, FI_LOG_EP_DATA,
				"===================================== SEND, SHM -- RENDEZVOUS RTS (end) context %p\n",
				user_context);
			return FI_SUCCESS;
		}

	non_ipc:;
#endif

		const uint16_t lrh_qws = (pbc_dws - 2) >> 1; /* (LRH QW) does not include pbc (8 bytes) */
		hdr->qw_16B[0] =
			opx_ep->tx->rzv_16B.hdr.qw_16B[0] |
			((uint64_t) (lrh_dlid_16B & OPX_LRH_JKR_16B_DLID_MASK_16B) << OPX_LRH_JKR_16B_DLID_SHIFT_16B) |
			((uint64_t) lrh_qws << 20);

		hdr->qw_16B[1] = opx_ep->tx->rzv_16B.hdr.qw_16B[1] |
				 ((uint64_t) ((lrh_dlid_16B & OPX_LRH_JKR_16B_DLID20_MASK_16B) >>
					      OPX_LRH_JKR_16B_DLID20_SHIFT_16B)) |
				 (uint64_t) (bth_rx >> OPX_LRH_JKR_BTH_RX_ENTROPY_SHIFT_16B);

		hdr->qw_16B[2] = opx_ep->tx->rzv_16B.hdr.qw_16B[2] | bth_rx | opcode;
		hdr->qw_16B[3] = opx_ep->tx->rzv_16B.hdr.qw_16B[3];
		hdr->qw_16B[4] = opx_ep->tx->rzv_16B.hdr.qw_16B[4] | (((uint64_t) data) << 32);
		hdr->qw_16B[5] = opx_ep->tx->rzv_16B.hdr.qw_16B[5] | (1ull << 48); /* effectively 1 iov */
		hdr->qw_16B[6] = len;
		hdr->qw_16B[7] = tag;

		struct opx_payload_rzv_contig *contiguous = &payload->rendezvous.contiguous_16B;
		contiguous->src_vaddr			  = (uintptr_t) buf + immediate_total;
		contiguous->src_len			  = len - immediate_total;
		contiguous->src_device_id		  = src_device_id;
		contiguous->src_iface			  = (uint64_t) src_iface;
		contiguous->immediate_info		  = immediate_info.qw0;
		contiguous->origin_byte_counter_vaddr	  = origin_byte_counter_vaddr;
		contiguous->unused			  = 0;

		if (immediate_total) {
			uint8_t *sbuf;
			if (src_iface != FI_HMEM_SYSTEM) {
				opx_copy_from_hmem(src_iface, src_device_id, src_handle, opx_ep->hmem_copy_buf, buf,
						   immediate_total, OPX_HMEM_DEV_REG_SEND_THRESHOLD);
				sbuf = opx_ep->hmem_copy_buf;
			} else {
				sbuf = (uint8_t *) buf;
			}

			for (int i = 0; i < immediate_byte_count; ++i) {
				contiguous->immediate_byte[i] = sbuf[i];
			}
			sbuf += immediate_byte_count;

			uint64_t *sbuf_qw = (uint64_t *) sbuf;
			for (int i = 0; i < immediate_qw_count; ++i) {
				contiguous->immediate_qw[i] = sbuf_qw[i];
			}

			if (immediate_block) {
				sbuf_qw += immediate_qw_count;
				uint64_t *payload_cacheline =
					(uint64_t *) (&contiguous->cache_line_1 + immediate_fragment);
				opx_cacheline_store_block(payload_cacheline, sbuf_qw);
			}
		}

		opx_shm_tx_advance(&opx_ep->tx->shm, (void *) hdr, pos);

		OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "SEND-RZV-RTS-SHM");
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
			     "===================================== SEND 16B, SHM -- RENDEZVOUS RTS (end) context %p\n",
			     user_context);

		if (OFI_LIKELY(do_cq_completion)) {
			fi_opx_ep_tx_cq_completion_rzv(ep, context, len, lock_required, tag, caps);
		}
		return FI_SUCCESS;
	}
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		     "===================================== SEND 16B, HFI -- RENDEZVOUS RTS (begin) context %p\n",
		     user_context);
	OPX_TRACER_TRACE(OPX_TRACER_BEGIN, "SEND-RZV-RTS-HFI:%ld", tag);

	/*
	 * While the bulk of the payload data will be sent via SDMA once we
	 * get the CTS from the receiver, the initial RTS packet is sent via PIO.
	 */
	OPX_SHD_CTX_PIO_LOCK(OPX_IS_CTX_SHARING_ENABLED, opx_ep->tx);
	union fi_opx_hfi1_pio_state pio_state = *opx_ep->tx->pio_state;

	const uint16_t lrh_qws		    = (pbc_dws - 2) >> 1; /* (LRH QW) does not include pbc (8 bytes) */
	const uint16_t total_credits_needed = (lrh_qws + 1 /* pbc */ + 7) >> 3;

	uint64_t total_credits_available =
		FI_OPX_HFI1_AVAILABLE_CREDITS(pio_state, &opx_ep->tx->force_credit_return, total_credits_needed);
	if (OFI_UNLIKELY(total_credits_available < total_credits_needed)) {
		FI_OPX_HFI1_UPDATE_CREDITS(pio_state, opx_ep->tx->pio_credits_addr);
		total_credits_available = FI_OPX_HFI1_AVAILABLE_CREDITS(pio_state, &opx_ep->tx->force_credit_return,
									total_credits_needed);
		if (total_credits_available < total_credits_needed) {
			opx_ep->tx->pio_state->qw0 = pio_state.qw0;
			OPX_TRACER_TRACE(OPX_TRACER_END_EAGAIN, "SEND-RZV-RTS-HFI:%ld", tag);
			OPX_SHD_CTX_PIO_UNLOCK(OPX_IS_CTX_SHARING_ENABLED, opx_ep->tx);
			return -FI_EAGAIN;
		}
	}

	struct opx_context *context;
	uintptr_t	    origin_byte_counter_vaddr;
	if (OFI_LIKELY(do_cq_completion)) {
		context = (struct opx_context *) ofi_buf_alloc(opx_ep->rx->ctx_pool);
		if (OFI_UNLIKELY(context == NULL)) {
			opx_ep->tx->pio_state->qw0 = pio_state.qw0;
			OPX_TRACER_TRACE(OPX_TRACER_END_EAGAIN, "SEND-RZV-RTS-HFI:%ld", tag);
			OPX_SHD_CTX_PIO_UNLOCK(OPX_IS_CTX_SHARING_ENABLED, opx_ep->tx);
			FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA, "Out of memory.\n");
			return -FI_ENOMEM;
		}
		context->err_entry.err	      = 0;
		context->err_entry.op_context = user_context;
		context->next		      = NULL;
		context->byte_counter	      = len - immediate_total;
		origin_byte_counter_vaddr     = (uintptr_t) &context->byte_counter;
	} else {
		context			  = NULL;
		origin_byte_counter_vaddr = (uintptr_t) NULL;
	}

	struct fi_opx_reliability_tx_replay *replay;
	union fi_opx_reliability_tx_psn	    *psn_ptr;
	int64_t				     psn;

	psn = fi_opx_reliability_get_replay(&opx_ep->ep_fid, opx_ep->reli_service, addr.lid, addr.hfi1_subctxt_rx,
					    &psn_ptr, &replay, reliability, hfi1_type);
	if (OFI_UNLIKELY(psn == -1)) {
		if (OFI_LIKELY(do_cq_completion)) {
			OPX_BUF_FREE(context);
		}
		opx_ep->tx->pio_state->qw0 = pio_state.qw0;
		OPX_TRACER_TRACE(OPX_TRACER_END_EAGAIN, "SEND-RZV-RTS-HFI:%ld", tag);
		OPX_SHD_CTX_PIO_UNLOCK(OPX_IS_CTX_SHARING_ENABLED, opx_ep->tx);
		return -FI_EAGAIN;
	}

	FI_OPX_DEBUG_COUNTERS_INC_COND(
		src_iface != FI_HMEM_SYSTEM,
		opx_ep->debug_counters.hmem.hfi.kind[(caps & FI_MSG) ? FI_OPX_KIND_MSG : FI_OPX_KIND_TAG].send.rzv);

	if (immediate_tail) {
		uint8_t *buf_tail_bytes = ((uint8_t *) buf + len) - OPX_IMMEDIATE_TAIL_BYTE_COUNT;
		if (src_iface != FI_HMEM_SYSTEM) {
			opx_copy_from_hmem(src_iface, src_device_id, src_handle, opx_ep->hmem_copy_buf, buf_tail_bytes,
					   OPX_IMMEDIATE_TAIL_BYTE_COUNT, OPX_HMEM_DEV_REG_SEND_THRESHOLD);
			buf_tail_bytes = opx_ep->hmem_copy_buf;
		}

		for (int i = 0; i < OPX_IMMEDIATE_TAIL_BYTE_COUNT; ++i) {
			immediate_info.tail_bytes[i] = buf_tail_bytes[i];
		}
	}

	/*
	 * Write the 'start of packet' (hw+sw header) 'send control block'
	 * which will consume a single pio credit.
	 */

	uint64_t       force_credit_return = OPX_PBC_CR(opx_ep->tx->force_credit_return, hfi1_type);
	const uint64_t opcode =
		(uint64_t) ((caps & FI_MSG) ?
				    ((tx_op_flags & FI_REMOTE_CQ_DATA) ? FI_OPX_HFI_BTH_OPCODE_MSG_RZV_RTS_CQ :
									 FI_OPX_HFI_BTH_OPCODE_MSG_RZV_RTS) :
				    ((tx_op_flags & FI_REMOTE_CQ_DATA) ? FI_OPX_HFI_BTH_OPCODE_TAG_RZV_RTS_CQ :
									 FI_OPX_HFI_BTH_OPCODE_TAG_RZV_RTS));

	volatile uint64_t *const scb	  = FI_OPX_HFI1_PIO_SCB_HEAD(opx_ep->tx->pio_scb_sop_first, pio_state);
	const uint64_t		 pbc_dlid = OPX_PBC_DLID(addr.lid, hfi1_type);
	opx_cacheline_copy_qw_vol(
		scb, replay->scb.qws,
		opx_ep->tx->rzv_16B.qw0 | OPX_PBC_LEN(pbc_dws, hfi1_type) | force_credit_return | pbc_dlid |
			OPX_PBC_LOOPBACK(pbc_dlid, hfi1_type),
		opx_ep->tx->rzv_16B.hdr.qw_16B[0] |
			((uint64_t) (lrh_dlid_16B & OPX_LRH_JKR_16B_DLID_MASK_16B) << OPX_LRH_JKR_16B_DLID_SHIFT_16B) |
			((uint64_t) lrh_qws << 20),
		opx_ep->tx->rzv_16B.hdr.qw_16B[1] |
			((uint64_t) ((lrh_dlid_16B & OPX_LRH_JKR_16B_DLID20_MASK_16B) >>
				     OPX_LRH_JKR_16B_DLID20_SHIFT_16B)) |
			(uint64_t) (bth_rx >> OPX_LRH_JKR_BTH_RX_ENTROPY_SHIFT_16B),
		opx_ep->tx->rzv_16B.hdr.qw_16B[2] | bth_rx | opcode, opx_ep->tx->rzv_16B.hdr.qw_16B[3] | psn,
		opx_ep->tx->rzv_16B.hdr.qw_16B[4] | (((uint64_t) data) << 32),
		opx_ep->tx->rzv_16B.hdr.qw_16B[5] | (1ull << 48), len);

	/* consume one credit for the packet header */
	FI_OPX_HFI1_CONSUME_SINGLE_CREDIT(pio_state);
#ifndef NDEBUG
	unsigned credits_consumed = 1;
#endif
	FI_OPX_HFI1_CLEAR_CREDIT_RETURN(opx_ep);

	/*
	 * write the rendezvous payload "send control blocks"
	 */

	volatile uint64_t *scb_payload = FI_OPX_HFI1_PIO_SCB_HEAD(opx_ep->tx->pio_scb_first, pio_state);

	opx_cacheline_copy_qw_vol(scb_payload, &replay->scb.qws[8], tag, /* end of header */
				  /* start of receiver payload/cacheline                                             */
				  (uintptr_t) buf + immediate_total, /* rzv.contiguous.src_vaddr                 */
				  (len - immediate_total),	     /* rzv.contiguous.src_len                   */
				  src_device_id,		     /* rzv.contiguous.src_device_id             */
				  (uint64_t) src_iface,		     /* rzv.contiguous.src_iface                 */
				  immediate_info.qw0,		     /* rzv.contiguous.immediate_info            */
				  origin_byte_counter_vaddr,	     /* rzv.contiguous.origin_byte_counter_vaddr */
				  -1UL /* unused */);		     /* rzv.contiguous.unused[0]                 */

	/* consume one credit for the rendezvous payload metadata */
	FI_OPX_HFI1_CONSUME_SINGLE_CREDIT(pio_state);
#ifndef NDEBUG
	++credits_consumed;
#endif

	uint64_t *replay_payload = replay->payload;

	assert(!replay->use_iov);
	assert(((uint8_t *) replay_payload) == ((uint8_t *) &replay->data));

	/* temp is hdr (1 QW) + payload (7 QW) */
	replay_payload[0] = replay->scb.qws[9];
	replay_payload[1] = replay->scb.qws[10];
	replay_payload[2] = replay->scb.qws[11];
	replay_payload[3] = replay->scb.qws[12];
	replay_payload[4] = replay->scb.qws[13];
	replay_payload[5] = replay->scb.qws[14];
	replay_payload[6] = replay->scb.qws[15];

	replay_payload += OPX_JKR_16B_PAYLOAD_AFTER_HDR_QWS;

	uint8_t *sbuf;
	if (src_iface != FI_HMEM_SYSTEM && immediate_total) {
		opx_copy_from_hmem(src_iface, src_device_id, src_handle, opx_ep->hmem_copy_buf, buf, immediate_total,
				   OPX_HMEM_DEV_REG_SEND_THRESHOLD);
		sbuf = opx_ep->hmem_copy_buf;
	} else {
		sbuf = (uint8_t *) buf;
	}

	/* immediate_byte and immediate_qw are "packed" in the current implementation             */
	/* meaning the immediate bytes are filled, then followed by the rest of the data directly */
	/* adjacent to the packed bytes.  It's probably more efficient to leave a pad and not go  */
	/* through the confusion of finding these boundaries on both sides of the rendezvous      */
	/* That is, just pack the immediate bytes, then pack the "rest" in the immediate qws      */
	/* This would lead to more efficient packing on both sides at the expense of              */
	/* wasting space of a common 0 byte immediate                                             */
	/* tmp_payload_t represents the second cache line of the rts packet                       */
	/* fi_opx_hfi1_packet_payload -> rendezvous -> contiguous                                 */
	struct tmp_payload_t {
		uint8_t	 immediate_byte[8]; /* rendezvous.contiguous.immediate_byte */
		uint64_t immediate_qw[7];   /* rendezvous.contiguous.immediate_qw */
	} __attribute__((packed));

	uint64_t *sbuf_qw = (uint64_t *) (sbuf + immediate_byte_count);
	if (immediate_fragment) {
		struct tmp_payload_t *tmp_payload = (struct tmp_payload_t *) replay_payload;

		for (int i = 0; i < immediate_byte_count; ++i) {
			tmp_payload->immediate_byte[i] = sbuf[i];
		}

		for (int i = 0; i < immediate_qw_count; ++i) {
			tmp_payload->immediate_qw[i] = sbuf_qw[i];
		}
		scb_payload = FI_OPX_HFI1_PIO_SCB_HEAD(opx_ep->tx->pio_scb_first, pio_state);
		opx_cacheline_store_block_vol(scb_payload, replay_payload);
		sbuf_qw += immediate_qw_count;

		replay_payload += FI_OPX_CACHE_LINE_QWS;

		/* consume one credit for the rendezvous payload immediate data */
		FI_OPX_HFI1_CONSUME_SINGLE_CREDIT(pio_state);
#ifndef NDEBUG
		++credits_consumed;
#endif
		/* Need a full tail block */
		if (icrc_fragment_block) {
			/* No other tail or immediate block after this */
			assert(!icrc_end_block && !immediate_block);

			/* Write another block to accomodate the ICRC and tail */
			uint64_t temp_0[8] = {-2UL};
			scb_payload	   = FI_OPX_HFI1_PIO_SCB_HEAD(opx_ep->tx->pio_scb_first, pio_state);

			opx_cacheline_copy_block_vol(scb_payload, replay_payload, temp_0);
			replay_payload += FI_OPX_CACHE_LINE_QWS;

			FI_OPX_HFI1_CONSUME_SINGLE_CREDIT(pio_state);
#ifndef NDEBUG
			++credits_consumed;
#endif
		}
#ifndef NDEBUG
		else if (icrc_fragment) { /* used an immediate qw for tail */
			/* No other tail or immediate block after this */
			assert(!icrc_end_block && !immediate_block);
		} else {
			/* Must be tail and immediate blocks after this */
			assert(icrc_end_block && immediate_block);
		}
#endif
	}

	if (immediate_block) {
		/* Tail will be it's own block */
		assert(icrc_end_block && !icrc_fragment_block && !icrc_fragment);
		scb_payload = FI_OPX_HFI1_PIO_SCB_HEAD(opx_ep->tx->pio_scb_first, pio_state);
		opx_cacheline_copy_block_vol(scb_payload, replay_payload, sbuf_qw);
		replay_payload += FI_OPX_CACHE_LINE_QWS;

		FI_OPX_HFI1_CONSUME_SINGLE_CREDIT(pio_state);
#ifndef NDEBUG
		++credits_consumed;
#endif
		/* Write another block to accomodate the ICRC and tail */
		uint64_t temp_0[8] = {-3UL};
		scb_payload	   = FI_OPX_HFI1_PIO_SCB_HEAD(opx_ep->tx->pio_scb_first, pio_state);
		opx_cacheline_copy_block_vol(scb_payload, replay_payload, temp_0);

		FI_OPX_HFI1_CONSUME_SINGLE_CREDIT(pio_state);
#ifndef NDEBUG
		++credits_consumed;
#endif
	}

#ifndef NDEBUG
	assert(credits_consumed == total_credits_needed);
#endif

	FI_OPX_HFI1_CHECK_CREDITS_FOR_ERROR(opx_ep->tx->pio_credits_addr);

	/* update the hfi txe state */
	opx_ep->tx->pio_state->qw0 = pio_state.qw0;
	OPX_SHD_CTX_PIO_UNLOCK(OPX_IS_CTX_SHARING_ENABLED, opx_ep->tx);

	fi_opx_reliability_service_replay_register_no_update(opx_ep->reli_service, psn_ptr, replay, reliability,
							     hfi1_type);

	if (OFI_LIKELY(do_cq_completion)) {
		fi_opx_ep_tx_cq_completion_rzv(ep, context, len, lock_required, tag, caps);
	}

	OPX_TRACER_TRACE(OPX_TRACER_END_SUCCESS, "SEND-RZV-RTS-HFI:%ld", tag);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		     "===================================== SEND 16B, HFI -- RENDEZVOUS RTS (end) context %p\n",
		     user_context);

	return FI_SUCCESS;
}

unsigned fi_opx_hfi1_handle_poll_error(struct fi_opx_ep *opx_ep, volatile uint64_t *rhe_ptr, volatile uint32_t *rhf_ptr,
				       uint64_t *p_rhf_seq, uint64_t *p_hdrq_head, uint32_t *p_last_egrbfr_index,
				       volatile uint64_t *hdrq_head_reg, volatile uint64_t *egrq_head_reg,
				       const uint32_t rhf_msb, const uint32_t rhf_lsb, const uint64_t rhf_seq,
				       const uint64_t hdrq_offset, const uint64_t rhf_rcvd,
				       const union opx_hfi1_packet_hdr *const hdr, const enum opx_hfi1_type hfi1_type)
{
	/* We are assuming that we can process any error and consume this header,
	   let reliability detect and replay it as needed. */

	(void) rhf_ptr; /* unused unless debug is turned on */

	/* drop this packet and allow reliability protocol to retry */
#ifdef OPX_RELIABILITY_DEBUG
	fprintf(stderr,
		"%s:%s():%d drop this packet and allow reliability protocol to retry, psn = %u, RHF %#16.16lX, OPX_RHF_IS_USE_EGR_BUF %u, hdrq_offset %lu\n",
		__FILE__, __func__, __LINE__, FI_OPX_HFI1_PACKET_PSN(hdr), rhf_rcvd,
		OPX_RHF_IS_USE_EGR_BUF(rhf_rcvd, hfi1_type), hdrq_offset);

#endif

	OPX_RHE_DEBUG(opx_ep, rhe_ptr, rhf_ptr, rhf_msb, rhf_lsb, rhf_seq, hdrq_offset, rhf_rcvd, hdr, hfi1_type,
		      *p_last_egrbfr_index);

	if (OPX_RHF_IS_USE_EGR_BUF(rhf_rcvd, hfi1_type)) {
		/* "consume" this egrq element */
		const uint32_t egrbfr_index	 = OPX_RHF_EGR_INDEX(rhf_rcvd, hfi1_type);
		uint32_t       last_egrbfr_index = *p_last_egrbfr_index;
		if (OFI_UNLIKELY(last_egrbfr_index != egrbfr_index)) {
			OPX_HFI1_BAR_UREG_STORE(egrq_head_reg, ((const uint64_t) last_egrbfr_index));
			*p_last_egrbfr_index = egrbfr_index;
		}
	}

	/* "consume" this hdrq element */
	*p_rhf_seq   = OPX_RHF_SEQ_INCREMENT(rhf_seq, hfi1_type);
	*p_hdrq_head = hdrq_offset + FI_OPX_HFI1_HDRQ_ENTRY_SIZE_DWS;

	fi_opx_hfi1_update_hdrq_head_register(opx_ep, hdrq_offset, hdrq_head_reg);

	return 1;
}
