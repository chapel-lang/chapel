/*
 * Copyright (C) 2021-2023 by Cornelis Networks.
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

#include "rdma/fabric.h" // only for 'fi_addr_t' ... which is a typedef to uint64_t
#include "rdma/opx/fi_opx_hfi1.h"
#include "rdma/opx/fi_opx.h"
#include "rdma/opx/fi_opx_eq.h"
#include "rdma/opx/fi_opx_hfi1_sdma.h"
#include "ofi_mem.h"
#include "opa_user.h"
#include "fi_opx_hfi_select.h"

#define BYTE2DWORD_SHIFT	(2)

#define ESSP_SL_DEFAULT		(0)	/* PSMI_SL_DEFAULT */
#define ESSP_SC_DEFAULT		(0)	/* PSMI_SC_DEFAULT */
#define ESSP_VL_DEFAULT		(0)	/* PSMI_VL_DEFAULT */
#define ESSP_SC_ADMIN		(15)	/* PSMI_SC_ADMIN */
#define ESSP_VL_ADMIN		(15)	/* PSMI_VL_ADMIN */

struct fi_opx_hfi1_context_internal {
	struct fi_opx_hfi1_context	context;

	struct hfi1_user_info_dep	user_info;
	struct _hfi_ctrl *		ctrl;

};

/*
 * Return the NUMA node id where the process is currently running.
 */
static int opx_get_current_proc_location()
{
        int core_id, node_id;

    core_id = sched_getcpu();
    if (core_id < 0)
        return -EINVAL;

    node_id = numa_node_of_cpu(core_id);
    if (node_id < 0)
        return -EINVAL;

    return node_id;
}

static int opx_get_current_proc_core()
{
	int core_id;
	core_id = sched_getcpu();
	if (core_id < 0)
		return -EINVAL;
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
	return  (rcvhdrq_cnt - 1) * 32;
}

// Used by fi_opx_hfi1_context_open as a convenience.
static int opx_open_hfi_and_context(struct _hfi_ctrl **ctrl,
				    struct fi_opx_hfi1_context_internal *internal,
					uuid_t unique_job_key,
				    int hfi_unit_number)
{
	int fd;

	fd = opx_hfi_context_open(hfi_unit_number, 0, 0);
	if (fd < 0) {
		FI_WARN(&fi_opx_provider, FI_LOG_FABRIC, "Unable to open HFI unit %d.\n",
			hfi_unit_number);
		fd = -1;
	} else {
		memset(&internal->user_info, 0, sizeof(internal->user_info));
		internal->user_info.userversion =
			HFI1_USER_SWMINOR |
			(opx_hfi_get_user_major_version() << HFI1_SWMAJOR_SHIFT);

		/* do not share hfi contexts */
		internal->user_info.subctxt_id = 0;
		internal->user_info.subctxt_cnt = 0;

		memcpy(internal->user_info.uuid, unique_job_key,
			sizeof(internal->user_info.uuid));

		*ctrl = opx_hfi_userinit(fd, &internal->user_info);
		if (!*ctrl) {
			opx_hfi_context_close(fd);
			FI_WARN(&fi_opx_provider, FI_LOG_FABRIC,
				"Unable to open a context on HFI unit %d.\n",
				hfi_unit_number);
			fd = -1;
		}
	}
	return fd;
}

static int fi_opx_get_daos_hfi_rank_inst(const uint8_t hfi_unit_number, const uint32_t rank)
{
	struct fi_opx_daos_hfi_rank_key key;
	struct fi_opx_daos_hfi_rank *hfi_rank = NULL;

	memset(&key, 0, sizeof(key));
	key.hfi_unit_number = hfi_unit_number;
	key.rank = rank;

	HASH_FIND(hh, fi_opx_global.daos_hfi_rank_hashmap, &key,
		  sizeof(key), hfi_rank);

	if (hfi_rank) {
		hfi_rank->instance++;

		FI_INFO(fi_opx_global.prov, FI_LOG_EP_DATA,
			"HFI %d assigned rank %d again: %d.\n",
			key.hfi_unit_number, key.rank, hfi_rank->instance);
	} else {
		int rc __attribute__ ((unused));
		rc = posix_memalign((void **)&hfi_rank, 32, sizeof(*hfi_rank));
		assert(rc==0);

		hfi_rank->key = key;
		hfi_rank->instance = 0;
		HASH_ADD(hh, fi_opx_global.daos_hfi_rank_hashmap, key,
			 sizeof(hfi_rank->key), hfi_rank);

		FI_INFO(fi_opx_global.prov, FI_LOG_EP_DATA,
			"HFI %d assigned rank %d entry created.\n",
			key.hfi_unit_number, key.rank);
	}

	return hfi_rank->instance;
}

/*
 * Open a context on the first HFI that shares our process' NUMA node.
 * If no HFI shares our NUMA node, grab the first active HFI.
 */
struct fi_opx_hfi1_context *fi_opx_hfi1_context_open(struct fid_ep *ep, uuid_t unique_job_key)
{
	struct fi_opx_ep *opx_ep = (ep == NULL) ? NULL : container_of(ep, struct fi_opx_ep, ep_fid);
	int fd = -1;
	int hfi_unit_number = -1;
	int hfi_context_rank = -1;
	int hfi_context_rank_inst = -1;
	const int numa_node_id = opx_get_current_proc_location();
	const int core_id = opx_get_current_proc_core();
	const int hfi_count = opx_hfi_get_num_units();
	int hfi_candidates[FI_OPX_MAX_HFIS];
	int hfi_distances[FI_OPX_MAX_HFIS];
	int hfi_candidates_count = 0;
	int hfi_candidate_index = -1;
	struct _hfi_ctrl *ctrl = NULL;
	bool use_default_logic = true;

	struct fi_opx_hfi1_context_internal *internal =
		calloc(1, sizeof(struct fi_opx_hfi1_context_internal));

	struct fi_opx_hfi1_context *context = &internal->context;

	/*
	 * Force cpu affinity if desired. Normally you would let the
	 * job scheduler (such as mpirun) handle this.
	 */
	int force_cpuaffinity = 0;
	fi_param_get_bool(fi_opx_global.prov,"force_cpuaffinity",
		&force_cpuaffinity);
	if (force_cpuaffinity) {
		const int cpu_id = sched_getcpu();
		cpu_set_t cpuset;
		CPU_ZERO(&cpuset);
		CPU_SET(cpu_id, &cpuset);
		if (sched_setaffinity(0, sizeof(cpuset), &cpuset)) {
			FI_WARN(&fi_opx_provider, FI_LOG_FABRIC,
				"Unable to force cpu affinity. %s\n", strerror(errno));
		}
	}

	/*
	 * open the hfi1 context
	 */
	context->fd = -1;
	internal->ctrl = NULL;

	// If FI_OPX_HFI_SELECT is specified, skip all this and
	// use its value as the selected hfi unit.
	char *env = NULL;
	if (FI_SUCCESS == fi_param_get_str(&fi_opx_provider, "hfi_select", &env)) {

		struct hfi_selector selector = {0};
		use_default_logic = false;

		int selectors, matched;
		selectors = matched = 0;
		const char *s;
		for (s = env; *s != '\0'; ) {
			s = hfi_selector_next(s, &selector);
			if (!s) {
				FI_WARN(&fi_opx_provider, FI_LOG_FABRIC,
					"Error occurred parsing HFI selector string \"%s\"\n", env);
				return NULL;
			}

			if (selector.type == HFI_SELECTOR_DEFAULT) {
				use_default_logic = true;
				break;
			}

			if (selector.unit >= hfi_count) {
				FI_WARN(&fi_opx_provider, FI_LOG_FABRIC,
					"Error: selector unit %d >= number of HFIs %d\n",
					selector.unit, hfi_count);
				return NULL;
			} else if (!opx_hfi_get_unit_active(selector.unit)) {
				FI_WARN(&fi_opx_provider, FI_LOG_FABRIC,
					"Error: selected unit %d is not active\n", selector.unit);
				return NULL;
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
						return NULL;
					}

					if (selector.mapby.rangeE > max_numa){
						FI_WARN(&fi_opx_provider, FI_LOG_FABRIC,
							"mapby numa end of range %d > numa_max_node %d\n",
							selector.mapby.rangeE, max_numa);
						return NULL;
					}

					if (selector.mapby.rangeS <= numa_node_id && selector.mapby.rangeE >= numa_node_id){
						hfi_unit_number = selector.unit;
						matched++;
						break;
					}
				} else if (selector.mapby.type == HFI_SELECTOR_MAPBY_CORE) {
					int max_core = get_nprocs();
					if (selector.mapby.rangeS > max_core) {
						FI_WARN(&fi_opx_provider, FI_LOG_FABRIC,
							"Error: mapby core %d > nprocs %d\n",
							selector.mapby.rangeS, max_core);
						return NULL;
					}
					if (selector.mapby.rangeE > max_core) {
						FI_WARN(&fi_opx_provider, FI_LOG_FABRIC,
							"mapby core end of range %d > nprocs %d\n",
							selector.mapby.rangeE, max_core);
						return NULL;
					}
					if (selector.mapby.rangeS <= core_id && selector.mapby.rangeE >= core_id){
						hfi_unit_number = selector.unit;
						matched++;
						break;
					}
				} else {
					FI_WARN(&fi_opx_provider, FI_LOG_FABRIC,
						"Error: unsupported mapby type %d\n", selector.mapby.type);
					return NULL;
				}
			} else {
				FI_WARN(&fi_opx_provider, FI_LOG_FABRIC,
					"Error: unsupported selector type %d\n", selector.type);
				return NULL;
			}
			selectors++;
		}

		if (!use_default_logic) {
			if (!matched) {
				FI_WARN(&fi_opx_provider, FI_LOG_FABRIC, "No HFI selectors matched.\n");
				return NULL;
			}

			hfi_candidates[0] = hfi_unit_number;
			hfi_distances[0] = 0;
			hfi_candidates_count = 1;
			FI_INFO(&fi_opx_provider, FI_LOG_FABRIC,
				"User-specified HFI selection set to %d. Skipping HFI selection algorithm \n",
				hfi_unit_number);

			fd = opx_open_hfi_and_context(&ctrl, internal, unique_job_key,
				hfi_unit_number);
			if (fd < 0) {
				FI_WARN(&fi_opx_provider, FI_LOG_FABRIC,
					"Unable to open user-specified HFI.\n");
				return NULL;
			}
		}

	} else if (opx_ep && opx_ep->common_info->src_addr &&
		((union fi_opx_addr *)(opx_ep->common_info->src_addr))->hfi1_unit != opx_default_addr.hfi1_unit) {
		union fi_opx_addr addr;
		use_default_logic = false;
		/*
		 * DAOS Persistent Address Support:
		 * No Context Resource Management Framework is supported by OPX to enable
		 * acquiring a context with attributes that exactly match the specified
		 * source address.
		 *
		 * Therefore, treat the source address as an ‘opaque’ ID and extract the
		 * essential data required to create a context that at least maps to the
		 * same HFI and HFI port (Note, the assigned LID is unchanged unless modified
		 * by the OPA FM).
		 */
		memset(&addr, 0, sizeof(addr));
		memcpy(&addr.fi, opx_ep->common_info->src_addr, opx_ep->common_info->src_addrlen);

		if (addr.uid.fi != UINT32_MAX)
			hfi_context_rank = addr.uid.fi;
		hfi_unit_number = addr.hfi1_unit;
		hfi_candidates[0] = hfi_unit_number;
		hfi_distances[0] = 0;
		hfi_candidates_count = 1;

		if (hfi_context_rank != -1) {
			hfi_context_rank_inst =
				fi_opx_get_daos_hfi_rank_inst(hfi_unit_number, hfi_context_rank);
				
			FI_WARN(&fi_opx_provider, FI_LOG_FABRIC,
				"Application-specified HFI selection set to %d rank %d.%d. Skipping HFI selection algorithm\n",
				hfi_unit_number, hfi_context_rank, hfi_context_rank_inst);
		} else {
			FI_WARN(&fi_opx_provider, FI_LOG_FABRIC,
				"Application-specified HFI selection set to %d. Skipping HFI selection algorithm\n",
				hfi_unit_number);
		}

		fd = opx_open_hfi_and_context(&ctrl, internal, unique_job_key, hfi_unit_number);
		if (fd < 0) {
			FI_WARN(&fi_opx_provider, FI_LOG_FABRIC,
				"Unable to open application-specified HFI.\n");
			return NULL;
		}

	}
	if (use_default_logic){
		/* Select the best HFI to open a context on */
		FI_INFO(&fi_opx_provider, FI_LOG_FABRIC, "Found HFIs = %d\n", hfi_count);

		if (hfi_count == 0) {
			FI_WARN(&fi_opx_provider, FI_LOG_FABRIC,
				"FATAL: detected no HFIs, cannot continue\n");
			return NULL;
		}

		else if (hfi_count == 1) {
			if (opx_hfi_get_unit_active(0) > 0) {
				// Only 1 HFI, populate the candidate list and continue.
				FI_INFO(&fi_opx_provider, FI_LOG_FABRIC,
					"Detected one HFI and it has active ports, selected it\n");
				hfi_candidates[0] = 0;
				hfi_distances[0] = 0;
				hfi_candidates_count = 1;
			} else {
				// No active ports, we're done here.
				FI_WARN(&fi_opx_provider, FI_LOG_FABRIC,
					"FATAL: HFI has no active ports, cannot continue\n");
				return NULL;
			}

		} else {
			// The system has multiple HFIs. Sort them by distance from
			// this process.
			int hfi_n, hfi_d;
			for (int i = 0; i < hfi_count; i++) {
				if (opx_hfi_get_unit_active(i) > 0) {
					hfi_n = opx_hfi_sysfs_unit_read_node_s64(i);
					hfi_d = numa_distance(hfi_n, numa_node_id);
					FI_INFO(&fi_opx_provider, FI_LOG_FABRIC,
						"HFI unit %d in numa node %d has a distance of %d from this pid.\n",
						i, hfi_n, hfi_d);
					hfi_candidates[hfi_candidates_count] = i;
					hfi_distances[hfi_candidates_count] = hfi_d;
					int j = hfi_candidates_count;
					// Bubble the new HFI up till the list is sorted.
					// Yes, this is lame but the practical matter is that
					// there will never be so many HFIs on a single system
					// that a real insertion sort is justified. Also, doing it
					// this way results in a deterministic result - HFIs will
					// be implicitly sorted by their unit number as well as
					// by distance ensuring that all processes in a NUMA node
					// will see the HFIs in the same order.
					while (j > 0 && hfi_distances[j - 1] > hfi_distances[j]) {
						int t1 = hfi_distances[j - 1];
						int t2 = hfi_candidates[j - 1];
						hfi_distances[j - 1] = hfi_distances[j];
						hfi_candidates[j - 1] = hfi_candidates[j];
						hfi_distances[j] = t1;
						hfi_candidates[j] = t2;
						j--;
					}
					hfi_candidates_count++;
				}
			}
		}

		// At this point we have a list of HFIs, sorted by distance from this
		// pid (and by unit # as an implied key).  Pick from the closest HFIs
		// based on the modulo of the pid. If we fail to open that HFI, try
		// another one at the same distance. If that fails, we will try HFIs
		// that are further away.
		int lower = 0;
		int higher = 0;
		do {
			// Find the set of HFIs at this distance. Again, no attempt is
			// made to make this fast.
			higher = lower + 1;
			while (higher < hfi_candidates_count &&
			       hfi_distances[higher] == hfi_distances[lower]) {
				higher++;
			}

			// Use the modulo of the pid to select an HFI. The intent
			// is to use HFIs evenly rather than have many pids open
			// the 1st HFi then have many select the next HFI, etc...
			int range = higher - lower;
			hfi_candidate_index = getpid() % range + lower;
			hfi_unit_number = hfi_candidates[hfi_candidate_index];

			// Try to open the HFI. If we fail, try the other HFIs
			// at that distance until we run out of HFIs at that
			// distance.
			fd = opx_open_hfi_and_context(&ctrl, internal, unique_job_key,
				hfi_unit_number);
			int t = range;
			while (fd < 0 && t-- > 1) {
				hfi_candidate_index++;
				if (hfi_candidate_index >= higher)
					hfi_candidate_index = lower;
				hfi_unit_number = hfi_candidates[hfi_candidate_index];
				fd = opx_open_hfi_and_context(&ctrl, internal, unique_job_key,
					hfi_unit_number);
			}

			// If we still haven't successfully chosen an HFI,
			// try HFIs that are further away.
			lower = higher;
		} while (fd < 0 && lower < hfi_candidates_count);

		if (fd < 0) {
			FI_WARN(&fi_opx_provider, FI_LOG_FABRIC,
				"FATAL: Found %d active HFI device%s, unable to open %s.\n",
				hfi_candidates_count, (hfi_candidates_count > 1) ? "s" : "",
				(hfi_candidates_count > 1) ? "any of them" : "it");
			return NULL;
		}
	}

	FI_INFO(&fi_opx_provider, FI_LOG_FABRIC,
		"Selected HFI is %d; caller NUMA domain is %d; HFI NUMA domain is %"PRId64"\n",
		hfi_unit_number, numa_node_id, opx_hfi_sysfs_unit_read_node_s64(hfi_unit_number));

	// Alert user if the final choice wasn't optimal.
	if (opx_hfi_sysfs_unit_read_node_s64(hfi_unit_number) != numa_node_id) {
		FI_WARN(&fi_opx_provider, FI_LOG_FABRIC,
			"Selected HFI is %d. It does not appear to be local to this pid's numa domain which is %d\n",
			hfi_unit_number, numa_node_id);
	} else {
		FI_INFO(&fi_opx_provider, FI_LOG_FABRIC,
			"Selected HFI unit %d in the same numa node as this pid.\n",
			hfi_unit_number);
	}

	context->fd = fd;
	internal->ctrl = ctrl; /* memory was allocated during opx_open_hfi_and_context() -> opx_hfi_userinit() */
	context->ctrl  = ctrl; /* TODO? move required fields ctrl -> context? */

	int lid = 0;
	lid = opx_hfi_get_port_lid(ctrl->__hfi_unit, ctrl->__hfi_port);
	assert(lid > 0);

	uint64_t gid_hi, gid_lo;
	int rc __attribute__((unused)) = -1;
	rc = opx_hfi_get_port_gid(ctrl->__hfi_unit, ctrl->__hfi_port, &gid_hi, &gid_lo);
	assert(rc != -1);

	context->hfi_unit = ctrl->__hfi_unit;
	context->hfi_port = ctrl->__hfi_port;
	context->lid = (uint16_t)lid;
	context->gid_hi = gid_hi;
	context->gid_lo = gid_lo;
	context->daos_info.rank = hfi_context_rank;
	context->daos_info.rank_inst = hfi_context_rank_inst;

	context->sl = ESSP_SL_DEFAULT;

	rc = opx_hfi_get_port_sl2sc(ctrl->__hfi_unit, ctrl->__hfi_port, ESSP_SL_DEFAULT);
	if (rc < 0)
		context->sc = ESSP_SC_DEFAULT;
	else
		context->sc = rc;

	rc = opx_hfi_get_port_sc2vl(ctrl->__hfi_unit, ctrl->__hfi_port, context->sc);
	if (rc < 0)
		context->vl = ESSP_VL_DEFAULT;
	else
		context->vl = rc;

	assert(context->sc != ESSP_SC_ADMIN);
	assert(context->vl != ESSP_VL_ADMIN);
	assert((context->vl == 15) || (context->vl <= 7));

	context->mtu = opx_hfi_get_port_vl2mtu(ctrl->__hfi_unit, ctrl->__hfi_port, context->vl);
	assert(context->mtu >= 0);

	rc = opx_hfi_set_pkey(ctrl, HFI_DEFAULT_P_KEY);

	const struct hfi1_base_info *base_info = &ctrl->base_info;
	const struct hfi1_ctxt_info *ctxt_info = &ctrl->ctxt_info;

	/*
	 * initialize the hfi tx context
	 */

	context->bthqp = (uint8_t)base_info->bthqp;
	context->jkey = base_info->jkey;
	context->send_ctxt = ctxt_info->send_ctxt;

	context->info.pio.scb_sop_first =
		(volatile uint64_t *)(ptrdiff_t)base_info->pio_bufbase_sop; // tx->pio_bufbase_sop
	context->info.pio.scb_first =
		(volatile uint64_t *)(ptrdiff_t)base_info->pio_bufbase; // tx->pio_bufbase
	context->info.pio.credits_addr = (volatile uint64_t *)(ptrdiff_t)base_info->sc_credits_addr;

	const uint64_t credit_return = *(context->info.pio.credits_addr);
	context->state.pio.free_counter_shadow = (uint16_t)(credit_return & 0x00000000000007FFul);
	context->state.pio.fill_counter = 0;
	context->state.pio.scb_head_index = 0;
	context->state.pio.credits_total =
		ctxt_info->credits; /* yeah, yeah .. THIS field is static, but there was an unused halfword at this spot, so .... */

	/* move to domain ? */
	uint8_t i;
	for (i = 0; i < 32; ++i) {
		rc = opx_hfi_get_port_sl2sc(ctrl->__hfi_unit, ctrl->__hfi_port, i);

		if (rc < 0)
			context->sl2sc[i] = ESSP_SC_DEFAULT;
		else
			context->sl2sc[i] = rc;

		rc = opx_hfi_get_port_sc2vl(ctrl->__hfi_unit, ctrl->__hfi_port, i);
		if (rc < 0)
			context->sc2vl[i] = ESSP_VL_DEFAULT;
		context->sc2vl[i] = rc;
	}

	//TODO: There is a bug in the driver that does not properly handle all
	//      queue entries in use at once. As a temporary workaround, pretend
	//      there is one less entry than there actually is.
	context->info.sdma.queue_size = ctxt_info->sdma_ring_size - 1;
	context->info.sdma.available_counter = context->info.sdma.queue_size;
	context->info.sdma.fill_index = 0;
	context->info.sdma.done_index = 0;
	context->info.sdma.completion_queue = (struct hfi1_sdma_comp_entry *)base_info->sdma_comp_bufbase;
	assert(context->info.sdma.queue_size <= FI_OPX_HFI1_SDMA_MAX_COMP_INDEX);
	memset(context->info.sdma.queued_entries, 0, sizeof(context->info.sdma.queued_entries));

	/*
	 * initialize the hfi rx context
	 */

	context->info.rxe.id = ctrl->ctxt_info.ctxt;
	context->info.rxe.hdrq.rhf_off = (ctxt_info->rcvhdrq_entsize - 8) >> BYTE2DWORD_SHIFT;

	/* hardware registers */
	volatile uint64_t *uregbase = (volatile uint64_t *)(uintptr_t)base_info->user_regbase;
	context->info.rxe.hdrq.head_register = (volatile uint64_t *)&uregbase[ur_rcvhdrhead];
	context->info.rxe.hdrq.tail_register = (volatile uint64_t *)&uregbase[ur_rcvhdrtail];
	context->info.rxe.egrq.head_register = (volatile uint64_t *)&uregbase[ur_rcvegrindexhead];
	context->info.rxe.egrq.tail_register = (volatile uint64_t *)&uregbase[ur_rcvegrindextail];
	context->info.rxe.uregbase = uregbase;

	context->runtime_flags = ctxt_info->runtime_flags;

	if (context->runtime_flags & HFI1_CAP_DMA_RTAIL) {
		context->info.rxe.hdrq.rhf_notail = 0;
	} else {
		context->info.rxe.hdrq.rhf_notail = 1;
	}

	context->info.rxe.hdrq.elemsz = ctxt_info->rcvhdrq_entsize >> BYTE2DWORD_SHIFT;
	context->info.rxe.hdrq.elemcnt = ctxt_info->rcvhdrq_cnt;
	context->info.rxe.hdrq.elemlast =
		((context->info.rxe.hdrq.elemcnt - 1) * context->info.rxe.hdrq.elemsz);
	context->info.rxe.hdrq.rx_poll_mask =
		fi_opx_hfi1_header_count_to_poll_mask(ctxt_info->rcvhdrq_cnt);
	context->info.rxe.hdrq.base_addr = (uint32_t *)(uintptr_t)base_info->rcvhdr_bufbase;
	context->info.rxe.hdrq.rhf_base =
		context->info.rxe.hdrq.base_addr + context->info.rxe.hdrq.rhf_off;

	context->info.rxe.egrq.base_addr = (uint32_t *)(uintptr_t)base_info->rcvegr_bufbase;
	context->info.rxe.egrq.elemsz = ctxt_info->rcvegr_size;
	context->info.rxe.egrq.size = ctxt_info->rcvegr_size * ctxt_info->egrtids;

	FI_INFO(&fi_opx_provider, FI_LOG_FABRIC, "Context configured with HFI=%d PORT=%d LID=0x%x JKEY=%d\n", 
	context->hfi_unit, context->hfi_port, context->lid, context->jkey);

	return context;
}

int init_hfi1_rxe_state (struct fi_opx_hfi1_context * context,
		struct fi_opx_hfi1_rxe_state * rxe_state)
{
	rxe_state->hdrq.head = 0;

	if (context->runtime_flags & HFI1_CAP_DMA_RTAIL) {
		rxe_state->hdrq.rhf_seq = 0;		/* will be ignored */
	} else {
		rxe_state->hdrq.rhf_seq = 0x10000000u;
	}

	rxe_state->egrq.countdown = 8;

	return 0;
}



#include "rdma/opx/fi_opx_endpoint.h"
#include "rdma/opx/fi_opx_reliability.h"

ssize_t fi_opx_hfi1_tx_connect (struct fi_opx_ep *opx_ep, fi_addr_t peer)
{
	ssize_t rc = FI_SUCCESS;

	if ((opx_ep->tx->caps & FI_LOCAL_COMM) || ((opx_ep->tx->caps & (FI_LOCAL_COMM | FI_REMOTE_COMM)) == 0)) {

		const uint64_t lrh_dlid = FI_OPX_ADDR_TO_HFI1_LRH_DLID(peer);
		const uint16_t dlid_be16 = (uint16_t)(FI_OPX_HFI1_LRH_DLID_TO_LID(lrh_dlid));
		const uint16_t slid_be16 = htons(opx_ep->hfi->lid);

		if (slid_be16 == dlid_be16) {
			char buffer[128];
			union fi_opx_addr addr;
			addr.raw64b = (uint64_t)peer;

			uint32_t hfi_unit = addr.hfi1_unit;
			unsigned rx_index = addr.hfi1_rx;
			int inst = 0;

			/* HFI Rank Support:  Rank and PID included in the SHM file name */
			if (opx_ep->daos_info.hfi_rank_enabled) {
				rx_index = opx_shm_daos_rank_index(opx_ep->daos_info.rank,
					opx_ep->daos_info.rank_inst);
				inst = opx_ep->daos_info.rank_inst;
			}

			snprintf(buffer,sizeof(buffer),"%s-%02x.%d",
				opx_ep->domain->unique_job_key_str, hfi_unit, inst);

			rc = opx_shm_tx_connect(&opx_ep->tx->shm, (const char * const)buffer,
				rx_index, FI_OPX_SHM_FIFO_SIZE, FI_OPX_SHM_PACKET_SIZE);
		}
	}

	return rc;
}

int fi_opx_hfi1_do_rx_rzv_rts_intranode (union fi_opx_hfi1_deferred_work *work)
{
	struct fi_opx_hfi1_rx_rzv_rts_params *params = &work->rx_rzv_rts;

	struct fi_opx_ep * opx_ep = params->opx_ep;
	const uint64_t lrh_dlid = params->lrh_dlid;
	const uint64_t bth_rx = ((uint64_t)params->u8_rx) << 56;

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		"===================================== RECV, SHM -- RENDEZVOUS RTS (begin)\n");
	uint64_t pos;
	/* Possible SHM connections required for certain applications (i.e., DAOS)
	 * exceeds the max value of the legacy u8_rx field.  Use u32_extended field.
	 */
	ssize_t rc =
		fi_opx_shm_dynamic_tx_connect(params->is_intranode, opx_ep,
			params->u32_extended_rx, params->target_hfi_unit);

	if (OFI_UNLIKELY(rc)) {
		return -FI_EAGAIN;
	}

	union fi_opx_hfi1_packet_hdr * const tx_hdr =
		opx_shm_tx_next(&opx_ep->tx->shm, params->u8_rx, &pos,
			opx_ep->daos_info.hfi_rank_enabled, params->u32_extended_rx,
			opx_ep->daos_info.rank_inst, &rc);

	if(!tx_hdr) return rc;
	tx_hdr->qw[0] = opx_ep->rx->tx.cts.hdr.qw[0] | lrh_dlid;
	tx_hdr->qw[1] = opx_ep->rx->tx.cts.hdr.qw[1] | bth_rx;
	tx_hdr->qw[2] = opx_ep->rx->tx.cts.hdr.qw[2];
	tx_hdr->qw[3] = opx_ep->rx->tx.cts.hdr.qw[3];
	tx_hdr->qw[4] = opx_ep->rx->tx.cts.hdr.qw[4] | (params->niov << 48) | params->opcode;
	tx_hdr->qw[5] = params->origin_byte_counter_vaddr;
	tx_hdr->qw[6] = params->target_byte_counter_vaddr;

	union fi_opx_hfi1_packet_payload * const tx_payload =
		(union fi_opx_hfi1_packet_payload *)(tx_hdr+1);

	uintptr_t vaddr_with_offset = params->dst_vaddr;
	for(int i = 0; i < params->niov; i++) {
		tx_payload->cts.iov[i].rbuf = vaddr_with_offset;		/* receive buffer virtual address */
		tx_payload->cts.iov[i].sbuf = (uintptr_t)params->src_iov[i].iov_base;		/* send buffer virtual address */
		tx_payload->cts.iov[i].bytes = params->src_iov[i].iov_len;	/* number of bytes to transfer */
		vaddr_with_offset += params->src_iov[i].iov_len;
	}

	opx_shm_tx_advance(&opx_ep->tx->shm, (void*)tx_hdr, pos);

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		"===================================== RECV, SHM -- RENDEZVOUS RTS (end)\n");

	return FI_SUCCESS;
}

int fi_opx_hfi1_do_rx_rzv_rts_pio (union fi_opx_hfi1_deferred_work *work) {

	struct fi_opx_hfi1_rx_rzv_rts_params *params = &work->rx_rzv_rts;
	struct fi_opx_ep * opx_ep = params->opx_ep;
	const uint64_t lrh_dlid = params->lrh_dlid;
	const uint64_t bth_rx = ((uint64_t)params->u8_rx) << 56;

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		"===================================== RECV, HFI -- RENDEZVOUS EAGER/PIO RTS (begin)\n");
	const uint64_t payload_bytes = params->niov * sizeof(struct fi_opx_hfi1_dput_iov);
	const uint64_t pbc_dws =
		2 + /* pbc */
		2 + /* lrh */
		3 + /* bth */
		9 + /* kdeth; from "RcvHdrSize[i].HdrSize" CSR */
		(payload_bytes >> 2) +
		((payload_bytes & 0x3) ? 1 : 0); /* "struct fi_opx_hfi1_dput_iov" * niov */
	const uint16_t lrh_dws = htons(pbc_dws - 1);
	union fi_opx_hfi1_pio_state pio_state = *opx_ep->tx->pio_state;
	const uint16_t total_credits_needed = 1 +   /* packet header */
		((payload_bytes >> 6) + ((payload_bytes & 0x3f) ? 1 : 0)); /* payload blocks needed */
	uint64_t total_credits_available = FI_OPX_HFI1_AVAILABLE_CREDITS(pio_state, &opx_ep->tx->force_credit_return, total_credits_needed);

	if (OFI_UNLIKELY(total_credits_available < total_credits_needed)) {
		fi_opx_compiler_msync_writes();
		FI_OPX_HFI1_UPDATE_CREDITS(pio_state, opx_ep->tx->pio_credits_addr);
		total_credits_available = FI_OPX_HFI1_AVAILABLE_CREDITS(pio_state, &opx_ep->tx->force_credit_return, total_credits_needed);
		opx_ep->tx->pio_state->qw0 = pio_state.qw0;
		if (total_credits_available < total_credits_needed) {
			return -FI_EAGAIN;
		}
	}

	struct fi_opx_reliability_tx_replay *replay =
		fi_opx_reliability_client_replay_allocate(&opx_ep->reliability->state, false);
	if (replay == NULL) {
		return -FI_EAGAIN;
	}

	union fi_opx_reliability_tx_psn *psn_ptr = NULL;
	const int64_t psn = (params->reliability != OFI_RELIABILITY_KIND_NONE) ? /* compile-time constant expression */
			fi_opx_reliability_tx_next_psn(&opx_ep->ep_fid,
							&opx_ep->reliability->state,
							params->slid,
							params->u8_rx,
							params->origin_rs,
							&psn_ptr, 1) :
			0;
	if(OFI_UNLIKELY(psn == -1)) {
		fi_opx_reliability_client_replay_deallocate(&opx_ep->reliability->state, replay);
		return -FI_EAGAIN;
	}

	assert(payload_bytes <= FI_OPX_HFI1_PACKET_MTU);
	// The "memcopy first" code is here as an alternative to the more complicated
	// direct write to pio followed by memory copy of the reliability buffer
	replay->scb.qw0 = opx_ep->rx->tx.cts.qw0 | pbc_dws |
			  ((opx_ep->tx->force_credit_return & FI_OPX_HFI1_PBC_CR_MASK) << FI_OPX_HFI1_PBC_CR_SHIFT);
	replay->scb.hdr.qw[0] =
		opx_ep->rx->tx.cts.hdr.qw[0] | lrh_dlid | ((uint64_t)lrh_dws << 32);
	replay->scb.hdr.qw[1] = opx_ep->rx->tx.cts.hdr.qw[1] | bth_rx;
	replay->scb.hdr.qw[2] = opx_ep->rx->tx.cts.hdr.qw[2] | psn;
	replay->scb.hdr.qw[3] = opx_ep->rx->tx.cts.hdr.qw[3];
	replay->scb.hdr.qw[4] = opx_ep->rx->tx.cts.hdr.qw[4] | (params->niov << 48) | params->opcode;
	replay->scb.hdr.qw[5] = params->origin_byte_counter_vaddr;
	replay->scb.hdr.qw[6] = params->target_byte_counter_vaddr;

	union fi_opx_hfi1_packet_payload *const tx_payload =
		(union fi_opx_hfi1_packet_payload *) replay->payload;
	assert(((uint8_t *)tx_payload) == ((uint8_t *)&replay->data));

	uintptr_t vaddr_with_offset = params->dst_vaddr;
	for (int i = 0; i < params->niov; i++) {
		tx_payload->cts.iov[i].rbuf =
			vaddr_with_offset; /* receive buffer virtual address */
		tx_payload->cts.iov[i].sbuf =
			(uintptr_t)params->src_iov[i].iov_base; /* send buffer virtual address */
		tx_payload->cts.iov[i].bytes =
			params->src_iov[i].iov_len; /* number of bytes to transfer */
		vaddr_with_offset += params->src_iov[i].iov_len;
	}

	FI_OPX_HFI1_CLEAR_CREDIT_RETURN(opx_ep);

	fi_opx_reliability_service_do_replay(&opx_ep->reliability->service, replay);
	fi_opx_reliability_client_replay_register_no_update(&opx_ep->reliability->state,
							    params->slid,
							    params->origin_rs,
							    params->origin_rx,
							    psn_ptr, replay, params->reliability);
	FI_DBG_TRACE(
		fi_opx_global.prov, FI_LOG_EP_DATA,
		"===================================== RECV, HFI -- RENDEZVOUS EAGER/PIO RTS (end)\n");

	return FI_SUCCESS;
}

int fi_opx_hfi1_do_rx_rzv_rts_tid (union fi_opx_hfi1_deferred_work *work)
{

	struct fi_opx_hfi1_rx_rzv_rts_params *params = &work->rx_rzv_rts;
	struct fi_opx_ep * opx_ep = params->opx_ep;
	const uint64_t lrh_dlid = params->lrh_dlid;
	const uint64_t bth_rx = ((uint64_t)params->u8_rx) << 56;
	struct fi_opx_tid_reuse_cache *const tid_reuse_cache = opx_ep->tid_reuse_cache;

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		"===================================== RECV, HFI -- RENDEZVOUS EXPECTED TID RTS (begin)\n");

	/* If tidpairs is set, this is FI_EAGAIN so skip TID processing as we're committed to TID (not eager) */
	if (!params->ntidpairs) {
		/*******************************************************************************************************************/
		struct _hfi_ctrl *ctx = opx_ep->hfi->ctrl;
		const uint32_t pg_sz = ctx->__hfi_pg_sz;
		/* Caller adjusted pointers and lengths past the immediate data.
		 * Now align the destination buffer to be page aligned for expected TID writes
		 * This should point/overlap into the immediate data area.
		 * Then realign source buffer and lengths appropriately.
		 */
		uint64_t alignment_mask = ~((uint64_t)pg_sz - 1UL);
		uint64_t vaddr = ((uint64_t)params->dst_vaddr) & alignment_mask;

		/* Caller adjusted pointers and lengths past the immediate data.
		 * Now align the destination buffer to be page aligned for expected TID writes
		 * This should point/overlap into the immediate data area.
		 * Realign source buffer and lengths appropriately.
		 */

		/* aligned target must be within immediate data*/
		assert((vaddr >= ((uint64_t)params->dst_vaddr  - params->immediate_data)) && (vaddr <= ((uint64_t)params->dst_vaddr)));
		/*assert there was immediate data or the buffer must already be aligned */

#ifndef NDEBUG
		uint64_t immediate_data = params->immediate_data;
		uint64_t immediate_end_block_count = params->immediate_end_block_count;
		assert(immediate_end_block_count);
		assert(immediate_data || (vaddr == ((uint64_t)params->dst_vaddr)));
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "vaddr %#lX, params->dst_vaddr %#lX (aligned %#lX), immediate_data %#lX, original (no immediate) dst_vaddr %#lX\n",
			     vaddr,(uint64_t)params->dst_vaddr,(uint64_t)params->dst_vaddr & alignment_mask,immediate_data, ((uint64_t)params->dst_vaddr - immediate_data));
#endif
		/* First adjust for the start page alignment, using immediate data that was sent.*/
		int64_t alignment_adjustment = (uint64_t)params->dst_vaddr - vaddr;
		assert(alignment_adjustment <= pg_sz);

		/* Adjust length for aligning the buffer */
		int64_t length = params->src_iov[0].iov_len + alignment_adjustment;
#ifndef NDEBUG
		/* Now adjust again for total length, aligning to SDMA AHG payload requirements. */
		int64_t packet_length_alignment_adjustment = length & -64;
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
			     "iov_len %#lX, length %#lX, immediate_end_block_count %#lX, "
			     "packet_length_alignment_adjustment %#lX, alignment_adjustment %#lX\n",
			     params->src_iov[0].iov_len, length, immediate_end_block_count,
			     packet_length_alignment_adjustment, alignment_adjustment);
		length = packet_length_alignment_adjustment;
#else
		/* Now adjust again for total length, aligning to SDMA AHG payload requirements. */
		length = length & -64;
#endif
		OPX_BUFFER_RANGE_DEBUG(immediate_data, immediate_end_block_count, vaddr, length, alignment_adjustment,
					alignment_mask, params->dst_vaddr, params->src_iov,
					params->origin_byte_counter_vaddr, params->target_byte_counter_vaddr);

		/* If we've done the math right, and made the right assumptions,
		 * the new adjusted end pointers should cover the original endpointers
		 * less the immediate trailing data -- ie. no missing data  */

		/* The original iov didn't begin on a page, the new truncated start should be within the immediate data */
		assert((char*)params->src_iov[0].iov_base >= ((char*)((((uint64_t)params->src_iov[0].iov_base) - alignment_adjustment))));
		assert((char*)params->src_iov[0].iov_base <= (char*)((((uint64_t)params->src_iov[0].iov_base) - alignment_adjustment) + immediate_data));

		/* The original iov didn't begin on a page, but it should cover at least the new buffer */
		assert(((char*)params->src_iov[0].iov_base + params->src_iov[0].iov_len) >= ((char*)((((uint64_t)params->src_iov[0].iov_base) - alignment_adjustment) + length)));

		/* The original iov didn't end 64 aligned, the new truncated end should be < 64 (immediate ending bytes) off */
		assert(((char*)params->src_iov[0].iov_base + params->src_iov[0].iov_len - (immediate_end_block_count << 6)) <= ((char*)((((uint64_t)params->src_iov[0].iov_base) - alignment_adjustment) + length)));

		/* Make sure our new destination buffer is covered by the TID's we'll get */
		assert((uint64_t)params->dst_vaddr >= vaddr);
		assert(((uint64_t)params->dst_vaddr - immediate_data) <= vaddr);
		assert(((uint64_t)params->dst_vaddr + params->src_iov[0].iov_len) >= (vaddr + length));
		assert(((uint64_t)params->dst_vaddr + params->src_iov[0].iov_len - (immediate_end_block_count << 6)) <= (vaddr + length));

		/* New params were checked above but
		 * DO NOT CHANGE params->xxx or opx_ep->xxx until we know we will NOT fallback to eager rts */

		uint32_t ntidpairs = OPX_TID_NINFO(tid_reuse_cache);
#ifndef NDEBUG
		uint32_t tidcnt = (length / pg_sz) + (length % pg_sz ? 1 : 0);
		assert(vaddr + length <= vaddr + (tidcnt * pg_sz));
#endif

		/* Flush MMU notifications of page invalidation */
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_FABRIC,"opx_tid_cache_flush(opx_ep->tid_domain %p)\n",opx_ep->tid_domain);
		opx_tid_cache_flush(opx_ep->tid_domain, false);
		if(OPX_TID_IS_INVALID(tid_reuse_cache)) {
			assert(OPX_TID_REFCOUNT(tid_reuse_cache) == 0);
			OPX_TID_CACHE_RZV_RTS(tid_reuse_cache,"INVALID");
			FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "%lu FREE INVALID TIDs vaddr [%#lX - %#lX] length %lu, tid vaddr [%#lX - %#lX] , tid length %lu\n",OPX_TID_REFCOUNT(tid_reuse_cache),
				     vaddr, vaddr+length, length, OPX_TID_VADDR(tid_reuse_cache), OPX_TID_VADDR(tid_reuse_cache) + OPX_TID_LENGTH(tid_reuse_cache), OPX_TID_LENGTH(tid_reuse_cache));
			opx_free_tid(opx_ep);
			ntidpairs = OPX_TID_NINFO(tid_reuse_cache);
		}

		if (ntidpairs == 0) {
			assert(!OPX_TID_REFCOUNT(tid_reuse_cache));
			assert(!OPX_TID_VADDR(tid_reuse_cache));
			assert(!OPX_TID_LENGTH(tid_reuse_cache));

			FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "%lu NEW vaddr [%#lX - %#lX] length %lu, tid vaddr [%#lX - %#lX] , tid length %lu\n",OPX_TID_REFCOUNT(tid_reuse_cache),
				     vaddr, vaddr+length, length, OPX_TID_VADDR(tid_reuse_cache), OPX_TID_VADDR(tid_reuse_cache) + OPX_TID_LENGTH(tid_reuse_cache), OPX_TID_LENGTH(tid_reuse_cache));
			FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "opx_append_tid vaddr %#lX, length %#lX\n",vaddr, length);
			if(opx_append_tid(vaddr, length, opx_ep)) {
				FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA, "RENDEZVOUS EXPECTED TID CTS fallback to EAGER/PIO CTS\n");
#ifdef OPX_TID_FALLBACK_DEBUG
				fprintf(stderr, "RENDEZVOUS EXPECTED TID CTS fallback to EAGER/PIO CTS (3)\n");
#endif
				params->ntidpairs = 0;
				params->opcode = params->fallback_opcode; /* fallback */
				params->work_elem.work_fn = fi_opx_hfi1_do_rx_rzv_rts_pio;
				return params->work_elem.work_fn(work);
			}
		} else if ((OPX_TID_VADDR(tid_reuse_cache) == vaddr) && (OPX_TID_LENGTH(tid_reuse_cache) >= length)) {
			assert(!OPX_TID_IS_INVALID(tid_reuse_cache));
			++OPX_TID_REFCOUNT(tid_reuse_cache);
			OPX_TID_CACHE_RZV_RTS(tid_reuse_cache,"REUSE");
			FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "%lu REUSE TIDs vaddr [%#lX - %#lX] length %lu, tid vaddr [%#lX - %#lX] , tid length %lu\n",OPX_TID_REFCOUNT(tid_reuse_cache),
				     vaddr, vaddr+length, length, OPX_TID_VADDR(tid_reuse_cache), OPX_TID_VADDR(tid_reuse_cache) + OPX_TID_LENGTH(tid_reuse_cache), OPX_TID_LENGTH(tid_reuse_cache));
			FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "ntidpairs %u, reuse %u\n", OPX_TID_NINFO(tid_reuse_cache), opx_ep->reuse_tidpairs);
		} else if ((vaddr == OPX_TID_VADDR(tid_reuse_cache)) && (length > OPX_TID_LENGTH(tid_reuse_cache))) {
			/* Until we enhance this to "walk" the TID pairs to find the start, an append
			   MUST start on the same vadd and extend past the old TID update */
			assert(!OPX_TID_IS_INVALID(tid_reuse_cache));
			FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "%lu APPEND vaddr [%#lX - %#lX] length %lu, tid vaddr [%#lX - %#lX] , tid length %lu\n",OPX_TID_REFCOUNT(tid_reuse_cache),
				     vaddr, vaddr+length, length, OPX_TID_VADDR(tid_reuse_cache), OPX_TID_VADDR(tid_reuse_cache) + OPX_TID_LENGTH(tid_reuse_cache), OPX_TID_LENGTH(tid_reuse_cache));
			FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "opx_append_tid vaddr %#lX, length %#lX\n",vaddr, length);
			if(opx_append_tid(OPX_TID_VADDR(tid_reuse_cache)+OPX_TID_LENGTH(tid_reuse_cache), (length - OPX_TID_LENGTH(tid_reuse_cache)), opx_ep)) {
				FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA, "RENDEZVOUS EXPECTED TID CTS fallback to EAGER/PIO CTS\n");
#ifdef OPX_TID_FALLBACK_DEBUG
				fprintf(stderr, "RENDEZVOUS EXPECTED TID CTS fallback to EAGER/PIO CTS (4)\n");
#endif
				params->ntidpairs = 0;
				params->opcode = params->fallback_opcode; /* fallback */
				params->work_elem.work_fn = fi_opx_hfi1_do_rx_rzv_rts_pio;
				return params->work_elem.work_fn(work);
			}
		} else {
#ifdef OPX_TID_CACHE_DEBUG  /* NOT dependent on NDEBUG */
			/* Debug for explaining performance.  fprint things that disable TID and fallback or re-update the TID*/
			if ((vaddr < OPX_TID_VADDR(tid_reuse_cache)) && ((vaddr + length) > (OPX_TID_VADDR(tid_reuse_cache)  + OPX_TID_LENGTH(tid_reuse_cache)))) {
				OPX_TID_CACHE_RZV_RTS(tid_reuse_cache,"SUPERSET");
			} else if ((vaddr < OPX_TID_VADDR(tid_reuse_cache)) && ((vaddr + length) >= OPX_TID_VADDR(tid_reuse_cache)) && ((vaddr + length) <= (OPX_TID_VADDR(tid_reuse_cache)  + OPX_TID_LENGTH(tid_reuse_cache)))) {
				OPX_TID_CACHE_RZV_RTS(tid_reuse_cache,"PREPEND");
			} else if ((vaddr >= OPX_TID_VADDR(tid_reuse_cache)) && (vaddr <= (OPX_TID_VADDR(tid_reuse_cache) + OPX_TID_LENGTH(tid_reuse_cache))) && ((vaddr + length) > (OPX_TID_VADDR(tid_reuse_cache) + OPX_TID_LENGTH(tid_reuse_cache)))) {
				OPX_TID_CACHE_RZV_RTS(tid_reuse_cache,"CONCAT"); /* distinguish from APPEND, start address is different */
			} else if ((OPX_TID_VADDR(tid_reuse_cache) <= vaddr) && ((OPX_TID_VADDR(tid_reuse_cache) + OPX_TID_LENGTH(tid_reuse_cache)) >= (vaddr + length))) {
				OPX_TID_CACHE_RZV_RTS(tid_reuse_cache,"SUBSET");
			} else {
				OPX_TID_CACHE_RZV_RTS(tid_reuse_cache,"DISJOINT");
			}
#endif
			if(OPX_TID_REFCOUNT(tid_reuse_cache) == 0) {
				OPX_TID_CACHE_RZV_RTS(tid_reuse_cache,"FREE");
				FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "%lu FREE TIDs vaddr [%#lX - %#lX] length %lu, tid vaddr [%#lX - %#lX] , tid length %lu\n",OPX_TID_REFCOUNT(tid_reuse_cache),
					     vaddr, vaddr+length, length, OPX_TID_VADDR(tid_reuse_cache), OPX_TID_VADDR(tid_reuse_cache) + OPX_TID_LENGTH(tid_reuse_cache), OPX_TID_LENGTH(tid_reuse_cache));
				opx_free_tid(opx_ep);

				FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "%lu FREED/NEW vaddr [%#lX - %#lX] length %lu, tid vaddr [%#lX - %#lX] , tid length %lu\n",OPX_TID_REFCOUNT(tid_reuse_cache),
					     vaddr, vaddr+length, length, OPX_TID_VADDR(tid_reuse_cache), OPX_TID_VADDR(tid_reuse_cache) + OPX_TID_LENGTH(tid_reuse_cache), OPX_TID_LENGTH(tid_reuse_cache));
				FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "opx_append_tid vaddr %#lX, length %#lX\n",vaddr, length);
				if(opx_append_tid(vaddr, length, opx_ep)) {
					FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA, "RENDEZVOUS EXPECTED TID CTS fallback to EAGER/PIO CTS\n");
#ifdef OPX_TID_FALLBACK_DEBUG
					fprintf(stderr, "RENDEZVOUS EXPECTED TID CTS fallback to EAGER/PIO CTS (5)\n");
#endif
					params->ntidpairs = 0;
					params->opcode = params->fallback_opcode; /* fallback */
					params->work_elem.work_fn = fi_opx_hfi1_do_rx_rzv_rts_pio;
					return params->work_elem.work_fn(work);
				}
			} else {
				OPX_TID_CACHE_RZV_RTS(tid_reuse_cache,"IN USE/UNAVAILABLE");
				FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "%lu TID IN USE vaddr [%#lX - %#lX] length %lu, tid vaddr [%#lX - %#lX] , tid length %lu\n",OPX_TID_REFCOUNT(tid_reuse_cache),
					     vaddr, vaddr+length, length, OPX_TID_VADDR(tid_reuse_cache), OPX_TID_VADDR(tid_reuse_cache) + OPX_TID_LENGTH(tid_reuse_cache), OPX_TID_LENGTH(tid_reuse_cache));
				FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA, "RENDEZVOUS EXPECTED TID CTS fallback to EAGER/PIO CTS\n");
#ifdef OPX_TID_FALLBACK_DEBUG
				fprintf(stderr, "RENDEZVOUS EXPECTED TID CTS fallback to EAGER/PIO CTS (6)\n");
#endif
				params->opcode = params->fallback_opcode; /* fallback */
				params->work_elem.work_fn = fi_opx_hfi1_do_rx_rzv_rts_pio;
				return params->work_elem.work_fn(work);
			}
		}
		/* Copy from ep list to params list for further modifications */
		params->ntidpairs = OPX_TID_NPAIRS(tid_reuse_cache);
		assert(params->ntidpairs != 0);
		memcpy(params->tidpairs, &OPX_TID_PAIR(tid_reuse_cache,0),  (OPX_TID_NPAIRS(tid_reuse_cache) * sizeof(OPX_TID_PAIR(tid_reuse_cache,0))));
		OPX_DEBUG_TIDS("RTS tidpairs", params->ntidpairs, params->tidpairs);

		/* Alignment math was done, now (no fallback to eager) we can change params */
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, " ==== iov[%u].base %p len %zu/%#lX \n", 0, params->src_iov[0].iov_base,params->src_iov[0].iov_len,params->src_iov[0].iov_len);
		params->src_iov[0].iov_base = (void*)(((uint64_t)params->src_iov[0].iov_base) - alignment_adjustment);
		params->src_iov[0].iov_len = length;
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, " ==== iov[%u].base %p len %zu/%#lX \n", 0, params->src_iov[0].iov_base,params->src_iov[0].iov_len,params->src_iov[0].iov_len);
		/* Adjust the counter with the new length ... */
#ifndef NDEBUG
		/* In debug, do it the hard way and assert the math is consistent */
		*(uint64_t*)params->target_byte_counter_vaddr += alignment_adjustment  & -64;
		assert(*(uint64_t*)params->target_byte_counter_vaddr == length);
#else
		*(uint64_t*)params->target_byte_counter_vaddr = length;
#endif
	} else {
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "RETRY FI_EAGAIN\n");
		OPX_DEBUG_TIDS("RTS retry tidpairs", params->ntidpairs, params->tidpairs);
	}

	/*******************************************************************************************************************/
	/* Committed to expected receive (TID) but might FI_EAGAIN out and retry                                           */
	/*******************************************************************************************************************/
	/* open the region.  completion of the rzv will close it */
	uint64_t key = opx_ep->tid_reuse_cache->vaddr;
	size_t len = opx_ep->tid_reuse_cache->length;
	struct fi_opx_tid_mr **opx_tid_mr = &opx_ep->tid_mr;
#ifndef NDEBUG
	{
		/* exploring and validating the data structures are sane at this point */
		const struct iovec	mr_iov = { .iov_base = (void*) key,
			                           .iov_len = len};
		const struct fi_mr_attr attr = { .mr_iov = &mr_iov,
			                         .iov_count = 1,
		                                  .requested_key = key};
		struct ofi_mr_entry *mr = ofi_mr_cache_find(opx_ep->tid_domain->tid_cache, &attr);
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "ENTRY mr %p, opx_tid_mr %p, entry %p\n", mr, *opx_tid_mr, (*opx_tid_mr)->entry);
		assert(mr == (*opx_tid_mr)->entry);
		opx_tid_cache_close_region(*opx_tid_mr);
	}
#endif
	opx_tid_cache_open_region(opx_ep->tid_domain,  (void *)key, len, key, opx_ep, opx_tid_mr);

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "ntidpairs %u\n",params->ntidpairs);
	const uint64_t payload_bytes = params->niov * sizeof(struct fi_opx_hfi1_dput_iov) +
		sizeof(uint32_t) /* ntidpairs */ +
		params->ntidpairs * sizeof(uint32_t) /* tidpairs[]*/;

	const uint64_t pbc_dws =
		2 + /* pbc */
		2 + /* lrh */
		3 + /* bth */
		9 + /* kdeth; from "RcvHdrSize[i].HdrSize" CSR */
		(payload_bytes >> 2) +
		((payload_bytes & 0x3) ? 1 : 0); /* "struct fi_opx_hfi1_dput_iov" * niov */
	const uint16_t lrh_dws = htons(pbc_dws - 1);
	union fi_opx_hfi1_pio_state pio_state = *opx_ep->tx->pio_state;
	const uint16_t total_credits_needed = 1 +   /* packet header */
		((payload_bytes >> 6) + ((payload_bytes & 0x3f) ? 1 : 0)); /* payload blocks needed */
	uint64_t total_credits_available = FI_OPX_HFI1_AVAILABLE_CREDITS(pio_state, &opx_ep->tx->force_credit_return, total_credits_needed);

	if (OFI_UNLIKELY(total_credits_available < total_credits_needed)) {
		fi_opx_compiler_msync_writes();
		FI_OPX_HFI1_UPDATE_CREDITS(pio_state, opx_ep->tx->pio_credits_addr);
		total_credits_available = FI_OPX_HFI1_AVAILABLE_CREDITS(pio_state, &opx_ep->tx->force_credit_return, total_credits_needed);
		opx_ep->tx->pio_state->qw0 = pio_state.qw0;
		if (total_credits_available < total_credits_needed) {
			opx_tid_cache_close_region(*opx_tid_mr);
			FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "FI_EAGAIN\n");
			return -FI_EAGAIN;
		}
	}

	struct fi_opx_reliability_tx_replay *replay =
		fi_opx_reliability_client_replay_allocate(&opx_ep->reliability->state, false);
	if (replay == NULL) {
		opx_tid_cache_close_region(*opx_tid_mr);
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "FI_EAGAIN\n");
		return -FI_EAGAIN;
	}

	union fi_opx_reliability_tx_psn *psn_ptr = NULL;
	const int64_t psn = (params->reliability != OFI_RELIABILITY_KIND_NONE) ? /* compile-time constant expression */
			fi_opx_reliability_tx_next_psn(&opx_ep->ep_fid,
							&opx_ep->reliability->state,
							params->slid,
							params->u8_rx,
							params->origin_rs,
							&psn_ptr, 1) :
			0;
	if(OFI_UNLIKELY(psn == -1)) {
		opx_tid_cache_close_region(*opx_tid_mr);
		fi_opx_reliability_client_replay_deallocate(&opx_ep->reliability->state, replay);
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "FI_EAGAIN\n");
		return -FI_EAGAIN;
	}

	assert(payload_bytes <= FI_OPX_HFI1_PACKET_MTU);
	// The "memcopy first" code is here as an alternative to the more complicated
	// direct write to pio followed by memory copy of the reliability buffer
	replay->scb.qw0 = opx_ep->rx->tx.cts.qw0 | pbc_dws |
			  ((opx_ep->tx->force_credit_return & FI_OPX_HFI1_PBC_CR_MASK) << FI_OPX_HFI1_PBC_CR_SHIFT);

	replay->scb.hdr.qw[0] =
		opx_ep->rx->tx.cts.hdr.qw[0] | lrh_dlid | ((uint64_t)lrh_dws << 32);
	replay->scb.hdr.qw[1] = opx_ep->rx->tx.cts.hdr.qw[1] | bth_rx;
	replay->scb.hdr.qw[2] = opx_ep->rx->tx.cts.hdr.qw[2] | psn;
	replay->scb.hdr.qw[3] = opx_ep->rx->tx.cts.hdr.qw[3];
	replay->scb.hdr.qw[4] = opx_ep->rx->tx.cts.hdr.qw[4] | (uint64_t)params->ntidpairs << 32 | (params->niov << 48)  | params->opcode;
	replay->scb.hdr.qw[5] = params->origin_byte_counter_vaddr;
	replay->scb.hdr.qw[6] = params->target_byte_counter_vaddr;

	union fi_opx_hfi1_packet_payload *const tx_payload =
		(union fi_opx_hfi1_packet_payload *) replay->payload;
	assert(((uint8_t *)tx_payload) == ((uint8_t *)&replay->data));

	uintptr_t vaddr_with_offset = OPX_TID_VADDR(tid_reuse_cache);

	assert(params->niov == 1);

	tx_payload->tid_cts.iov[0].rbuf = vaddr_with_offset; /* receive buffer virtual address */
	tx_payload->tid_cts.iov[0].sbuf =
		(uintptr_t)params->src_iov[0].iov_base; /* send buffer virtual address */
	tx_payload->tid_cts.iov[0].bytes =
		params->src_iov[0].iov_len; /* number of bytes to transfer */

	/* copy tidpairs to packet */
	tx_payload->tid_cts.ntidpairs = params->ntidpairs;
	assert(params->tidpairs[0] != 0);
	memcpy(&tx_payload->tid_cts.tidpairs, params->tidpairs, params->ntidpairs * sizeof(uint32_t));

	FI_OPX_HFI1_CLEAR_CREDIT_RETURN(opx_ep);

	fi_opx_reliability_service_do_replay(&opx_ep->reliability->service, replay);
	fi_opx_reliability_client_replay_register_no_update(&opx_ep->reliability->state,
							    params->slid,
							    params->origin_rs,
							    params->origin_rx,
							    psn_ptr, replay, params->reliability);
	FI_DBG_TRACE(
		fi_opx_global.prov, FI_LOG_EP_DATA,
		"===================================== RECV, HFI -- RENDEZVOUS EXPECTED TID RTS (end)\n");

	return FI_SUCCESS;
}

void fi_opx_hfi1_rx_rzv_rts (struct fi_opx_ep *opx_ep,
			     const void * const hdr, const void * const payload,
			     const uint8_t u8_rx, const uint64_t niov,
			     uintptr_t origin_byte_counter_vaddr,
			     uintptr_t target_byte_counter_vaddr,
			     const uintptr_t dst_vaddr,
			     const uint64_t immediate_data,
			     const uint64_t immediate_end_block_count,
			     const struct iovec * src_iov,
			     uint8_t opcode,
			     const unsigned is_intranode,
			     const enum ofi_reliability_kind reliability,
				 const uint32_t u32_extended_rx)
{

	const union fi_opx_hfi1_packet_hdr * const hfi1_hdr =
		(const union fi_opx_hfi1_packet_hdr * const) hdr;

	union fi_opx_hfi1_deferred_work *work = ofi_buf_alloc(opx_ep->tx->work_pending_pool);
	assert(work != NULL);
	struct fi_opx_hfi1_rx_rzv_rts_params *params = &work->rx_rzv_rts;
	params->opx_ep = opx_ep;
	params->work_elem.slist_entry.next = NULL;

	params->opcode = opcode;
	params->fallback_opcode = opcode;
	if (is_intranode) {
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "is_intranode %u\n",is_intranode );
		params->work_elem.work_fn = fi_opx_hfi1_do_rx_rzv_rts_intranode;
	} else if (opx_ep->use_expected_tid_rzv) {
		/* further checks on whether TID rts is supported */
		if(niov != 1) {
			/* TID rts only supports 1 iov, use pio eager rts */
			FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "niov %lu\n",niov);
			params->work_elem.work_fn = fi_opx_hfi1_do_rx_rzv_rts_pio;
		} else if (!fi_opx_hfi1_sdma_use_sdma(opx_ep, (uint64_t)src_iov[0].iov_len, opcode, is_intranode)) {
			/* TID rts requires SDMA, use pio eager */
			FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "src_iov[0].iov_len %zu, opcode %u, is_intranode %u\n",src_iov[0].iov_len, opcode, is_intranode);
			params->work_elem.work_fn = fi_opx_hfi1_do_rx_rzv_rts_pio;
		} else {
			params->opcode = FI_OPX_HFI_DPUT_OPCODE_RZV_TID;
			FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "opx_ep->use_expected_tid_rzv %u, opcode %u, fallback opcode %u\n", opx_ep->use_expected_tid_rzv, params->opcode, params->fallback_opcode);
			params->work_elem.work_fn = fi_opx_hfi1_do_rx_rzv_rts_tid;
		}
	} else {
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "opx_ep->use_expected_tid_rzv %u, opcode %u\n", opx_ep->use_expected_tid_rzv, params->opcode);
		params->work_elem.work_fn = fi_opx_hfi1_do_rx_rzv_rts_pio;
	}
	params->work_elem.completion_action = NULL;
	params->work_elem.payload_copy = NULL;
	params->work_elem.complete = false;
	params->work_elem.low_priority = false;
	params->lrh_dlid = (hfi1_hdr->stl.lrh.qw[0] & 0xFFFF000000000000ul) >> 32;
	params->slid = hfi1_hdr->stl.lrh.slid;

	params->origin_rx = hfi1_hdr->rendezvous.origin_rx;
	params->origin_rs = hfi1_hdr->rendezvous.origin_rs;
	params->u8_rx = u8_rx;
	params->u32_extended_rx = u32_extended_rx;
	params->niov = niov;
	params->origin_byte_counter_vaddr = origin_byte_counter_vaddr;
	params->target_byte_counter_vaddr = target_byte_counter_vaddr;
	params->dst_vaddr = dst_vaddr;
	params->immediate_data = immediate_data;
	params->immediate_end_block_count = immediate_end_block_count,
	params->is_intranode = is_intranode;
	params->reliability = reliability;
	params->target_hfi_unit = opx_ep->hfi->hfi_unit;
	params->ntidpairs = 0;


	assert(niov <= FI_OPX_MAX_DPUT_IOV);
	for(int idx=0; idx < niov; idx++) {
		params->src_iov[idx] = src_iov[idx];
	}

	int rc = params->work_elem.work_fn(work);
	if(rc == FI_SUCCESS) {
		OPX_BUF_FREE(work);
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "FI_SUCCESS\n");
		return;
	}
	assert(rc == -FI_EAGAIN);
	/* Try again later*/
	assert(work->work_elem.slist_entry.next == NULL);
	slist_insert_tail(&work->work_elem.slist_entry, &opx_ep->tx->work_pending);
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "FI_EAGAIN\n");
}

int opx_hfi1_do_dput_fence(union fi_opx_hfi1_deferred_work *work)
{
	const uint64_t pbc_dws = 2 + /* pbc */
				2 + /* lrh */
				3 + /* bth */
				9;  /* kdeth; from "RcvHdrSize[i].HdrSize" CSR */
	const uint16_t lrh_dws = htons(pbc_dws - 1);

	struct fi_opx_hfi1_rx_dput_fence_params *params = &work->fence;
	struct fi_opx_ep * opx_ep = params->opx_ep;

	uint64_t pos;
	/* Possible SHM connections required for certain applications (i.e., DAOS)
	 * exceeds the max value of the legacy u8_rx field.  Use u32_extended field.
	 */
	ssize_t rc =
		fi_opx_shm_dynamic_tx_connect(1, opx_ep, params->u32_extended_rx,
			params->target_hfi_unit);
	if (OFI_UNLIKELY(rc)) {
		return -FI_EAGAIN;
	}

	union fi_opx_hfi1_packet_hdr *const tx_hdr =
			opx_shm_tx_next(&opx_ep->tx->shm, params->u8_rx, &pos,
				opx_ep->daos_info.hfi_rank_enabled, params->u32_extended_rx,
				opx_ep->daos_info.rank_inst, &rc);
	if (tx_hdr == NULL) {
		return rc;
	}

	tx_hdr->qw[0] = opx_ep->rx->tx.dput.hdr.qw[0] | params->lrh_dlid | ((uint64_t)lrh_dws << 32);
	tx_hdr->qw[1] = opx_ep->rx->tx.dput.hdr.qw[1] | params->bth_rx;
	tx_hdr->qw[2] = opx_ep->rx->tx.dput.hdr.qw[2];
	tx_hdr->qw[3] = opx_ep->rx->tx.dput.hdr.qw[3];
	tx_hdr->qw[4] = opx_ep->rx->tx.dput.hdr.qw[4] | FI_OPX_HFI_DPUT_OPCODE_FENCE;
	tx_hdr->qw[5] = (uint64_t)params->cc;
	tx_hdr->qw[6] = params->bytes_to_fence;

	opx_shm_tx_advance(&opx_ep->tx->shm, (void *)tx_hdr, pos);

	return FI_SUCCESS;
}

void opx_hfi1_dput_fence(struct fi_opx_ep *opx_ep,
			const union fi_opx_hfi1_packet_hdr *const hdr,
			const uint8_t u8_rx,
			const uint32_t u32_extended_rx)
{
	union fi_opx_hfi1_deferred_work *work = ofi_buf_alloc(opx_ep->tx->work_pending_pool);
	assert(work != NULL);
	struct fi_opx_hfi1_rx_dput_fence_params *params = &work->fence;
	params->opx_ep = opx_ep;
	params->work_elem.slist_entry.next = NULL;
	params->work_elem.work_fn = opx_hfi1_do_dput_fence;
	params->work_elem.completion_action = NULL;
	params->work_elem.payload_copy = NULL;
	params->work_elem.complete = false;
	params->work_elem.low_priority = false;

	params->lrh_dlid = (hdr->stl.lrh.qw[0] & 0xFFFF000000000000ul) >> 32;
	params->bth_rx = (uint64_t)u8_rx << 56;
	params->u8_rx = u8_rx;
	params->u32_extended_rx = u32_extended_rx;
	params->bytes_to_fence = hdr->dput.target.fence.bytes_to_fence;
	params->cc = (struct fi_opx_completion_counter *) hdr->dput.target.fence.completion_counter;
	params->target_hfi_unit = opx_ep->hfi->hfi_unit;

	int rc = opx_hfi1_do_dput_fence(work);

	if (rc == FI_SUCCESS) {
		OPX_BUF_FREE(work);
		return;
	}
	assert(rc == -FI_EAGAIN);
	/* Try again later*/
	assert(work->work_elem.slist_entry.next == NULL);
	slist_insert_tail(&work->work_elem.slist_entry, &opx_ep->tx->work_pending);
}

__OPX_FORCE_INLINE__
size_t opx_hfi1_dput_write_header_and_payload_put(
				struct fi_opx_ep *opx_ep,
				union fi_opx_hfi1_packet_hdr *tx_hdr,
				union fi_opx_hfi1_packet_payload *tx_payload,
				struct iovec *iov,
				const uint64_t op64,
				const uint64_t dt64,
				const size_t payload_bytes,
				const uint64_t key,
				uint8_t **sbuf,
				uintptr_t *rbuf)
{
	tx_hdr->qw[4] = opx_ep->rx->tx.dput.hdr.qw[4] | FI_OPX_HFI_DPUT_OPCODE_PUT |
			(dt64 << 16) | (op64 << 24) | (payload_bytes << 48);
	tx_hdr->qw[5] = key;
	tx_hdr->qw[6] = fi_opx_dput_rbuf_out(*rbuf);

	if (tx_payload) {
		assert(!iov);
		memcpy((void *)tx_payload, (const void *)*sbuf, payload_bytes);
	} else {
		assert(iov);
		iov->iov_base = (void *) *sbuf;
		iov->iov_len = payload_bytes;
	}

	(*sbuf) += payload_bytes;
	(*rbuf) += payload_bytes;

	return payload_bytes;
}

__OPX_FORCE_INLINE__
void opx_hfi1_dput_write_payload_atomic_fetch(
				union fi_opx_hfi1_packet_payload *tx_payload,
				const uint64_t dput_bytes,
				const uint64_t fetch_vaddr,
				const uintptr_t target_byte_counter_vaddr,
				uint64_t bytes_sent,
				uint8_t *sbuf)
{
	struct fi_opx_hfi1_dput_fetch *dput_fetch = (struct fi_opx_hfi1_dput_fetch *) tx_payload->byte;
	dput_fetch->fetch_rbuf = fetch_vaddr + bytes_sent;
	dput_fetch->fetch_counter_vaddr = target_byte_counter_vaddr;

	memcpy((void *)&tx_payload->byte[sizeof(*dput_fetch)], (const void *)sbuf, dput_bytes);
}

__OPX_FORCE_INLINE__
size_t opx_hfi1_dput_write_header_and_payload_atomic_fetch(
				struct fi_opx_ep *opx_ep,
				union fi_opx_hfi1_packet_hdr *tx_hdr,
				union fi_opx_hfi1_packet_payload *tx_payload,
				struct iovec *iov,
				const uint64_t op64,
				const uint64_t dt64,
				const size_t payload_bytes,
				const uint64_t key,
				const uint64_t fetch_vaddr,
				const uintptr_t target_byte_counter_vaddr,
				uint64_t bytes_sent,
				uint8_t **sbuf,
				uintptr_t *rbuf)
{
	tx_hdr->qw[4] = opx_ep->rx->tx.dput.hdr.qw[4] | FI_OPX_HFI_DPUT_OPCODE_ATOMIC_FETCH |
			(dt64 << 16) | (op64 << 24) | (payload_bytes << 48);
	tx_hdr->qw[5] = key;
	tx_hdr->qw[6] = fi_opx_dput_rbuf_out(*rbuf);

	size_t dput_bytes = payload_bytes - sizeof(struct fi_opx_hfi1_dput_fetch);

	if (tx_payload) {
		assert(!iov);
		opx_hfi1_dput_write_payload_atomic_fetch(tx_payload, dput_bytes,
					fetch_vaddr, target_byte_counter_vaddr,
					bytes_sent, *sbuf);

		/* Here the source buffer is the data with no fi_opx_hfi1_dput_fetch
		   contained in it, so we only want to advance the pointer by
		   the number of actual data bytes, not including the dput fetch
		   header info that we injected at the beginning in the target buffer. */
		(*sbuf) += dput_bytes;
	} else {
		assert(iov);
		iov->iov_base = (void *) *sbuf;
		iov->iov_len = payload_bytes;

		/* Here we're simply setting the IOV pointer, so the source buffer
		   must already be a fully built packet payload, so advance the
		   pointer by the full payload amount */
		(*sbuf) += payload_bytes;
	}

	(*rbuf) += dput_bytes;

	return dput_bytes;
}

__OPX_FORCE_INLINE__
void opx_hfi1_dput_write_payload_atomic_compare_fetch(
				union fi_opx_hfi1_packet_payload *tx_payload,
				const size_t dput_bytes_half,
				const uint64_t fetch_vaddr,
				const uintptr_t target_byte_counter_vaddr,
				uint64_t bytes_sent,
				uint8_t *sbuf,
				uint8_t *cbuf)
{
	struct fi_opx_hfi1_dput_fetch *dput_fetch = (struct fi_opx_hfi1_dput_fetch *) tx_payload->byte;
	dput_fetch->fetch_rbuf = fetch_vaddr + bytes_sent;
	dput_fetch->fetch_counter_vaddr = target_byte_counter_vaddr;

	/* The first 1/2 of the actual payload bytes contains the data for the elements
	   we want to write to memory at the destination. The second 1/2 contains the
	   data for the elements to use as compare values against the elements currently
	   in the destination's memory, to see if a swap should take place. */
	memcpy((void *)&tx_payload->byte[sizeof(*dput_fetch)],
		(const void *)sbuf, dput_bytes_half);
	memcpy((void *)&tx_payload->byte[sizeof(*dput_fetch) + dput_bytes_half],
		(const void *)cbuf, dput_bytes_half);
}

__OPX_FORCE_INLINE__
size_t opx_hfi1_dput_write_header_and_payload_atomic_compare_fetch(
				struct fi_opx_ep *opx_ep,
				union fi_opx_hfi1_packet_hdr *tx_hdr,
				union fi_opx_hfi1_packet_payload *tx_payload,
				struct iovec *iov,
				const uint64_t op64,
				const uint64_t dt64,
				const size_t payload_bytes,
				const uint64_t key,
				const uint64_t fetch_vaddr,
				const uintptr_t target_byte_counter_vaddr,
				uint64_t bytes_sent,
				uint8_t **sbuf,
				uint8_t **cbuf,
				uintptr_t *rbuf)
{
	tx_hdr->qw[4] = opx_ep->rx->tx.dput.hdr.qw[4] | FI_OPX_HFI_DPUT_OPCODE_ATOMIC_COMPARE_FETCH |
			(dt64 << 16) | (op64 << 24) | (payload_bytes << 48);
	tx_hdr->qw[5] = key;
	tx_hdr->qw[6] = fi_opx_dput_rbuf_out(*rbuf);

	size_t dput_bytes = payload_bytes - sizeof(struct fi_opx_hfi1_dput_fetch);
	size_t dput_bytes_half = dput_bytes >> 1;

	if (tx_payload) {
		assert(!iov);
		opx_hfi1_dput_write_payload_atomic_compare_fetch(tx_payload, dput_bytes_half,
					fetch_vaddr, target_byte_counter_vaddr,
					bytes_sent, *sbuf, *cbuf);

		/* Here the source buffer is the data with no fi_opx_hfi1_dput_fetch
		   contained in it, so we only want to advance the pointer by
		   the number of actual data bytes, not including the dput fetch
		   header info that we injected at the beginning in the target buffer. */
		(*sbuf) += dput_bytes_half;
	} else {
		assert(iov);
		iov->iov_base = (void *) *sbuf;
		iov->iov_len = payload_bytes;

		/* Here we're simply setting the IOV pointer, so the source buffer
		   must already be a fully built packet payload, so advance the
		   pointer by the full payload amount */
		(*sbuf) += payload_bytes;
	}

	(*rbuf) += dput_bytes_half;
	(*cbuf) += dput_bytes_half;

	return dput_bytes;
}

__OPX_FORCE_INLINE__
size_t opx_hfi1_dput_write_header_and_payload_get(
				struct fi_opx_ep *opx_ep,
				union fi_opx_hfi1_packet_hdr *tx_hdr,
				union fi_opx_hfi1_packet_payload *tx_payload,
				struct iovec *iov,
				const uint64_t dt64,
				const size_t payload_bytes,
				const uintptr_t target_byte_counter_vaddr,
				uint8_t **sbuf,
				uintptr_t *rbuf)
{
	tx_hdr->qw[4] = opx_ep->rx->tx.dput.hdr.qw[4] | FI_OPX_HFI_DPUT_OPCODE_GET |
			(dt64 << 16) | (payload_bytes << 48);
	tx_hdr->qw[5] = target_byte_counter_vaddr;
	tx_hdr->qw[6] = fi_opx_dput_rbuf_out(*rbuf);

	if (tx_payload) {
		assert(!iov);
		if (dt64 == (FI_VOID - 1)) {
			memcpy((void *)tx_payload, (const void *)*sbuf, payload_bytes);
		} else {
			fi_opx_rx_atomic_dispatch((void *)*sbuf,
						  (void *)tx_payload,
						  payload_bytes,
						  dt64,
						  FI_ATOMIC_WRITE);
		}
	} else {
		assert(iov);
		iov->iov_base = (void *) *sbuf;
		iov->iov_len = payload_bytes;
	}

	(*sbuf) += payload_bytes;
	(*rbuf) += payload_bytes;

	return payload_bytes;
}

__OPX_FORCE_INLINE__
size_t opx_hfi1_dput_write_header_and_payload_rzv(
				struct fi_opx_ep *opx_ep,
				union fi_opx_hfi1_packet_hdr *tx_hdr,
				union fi_opx_hfi1_packet_payload *tx_payload,
				struct iovec *iov,
				const uint64_t op64,
				const uint64_t dt64,
				const size_t payload_bytes,
				const uint32_t opcode,
				const uintptr_t target_byte_counter_vaddr,
				uint8_t **sbuf,
				uintptr_t *rbuf)
{
	tx_hdr->qw[4] = opx_ep->rx->tx.dput.hdr.qw[4] | (opcode) | (payload_bytes << 48);
	tx_hdr->qw[5] = target_byte_counter_vaddr;
	tx_hdr->qw[6] = fi_opx_dput_rbuf_out(*rbuf);

	if (tx_payload) {
		assert(!iov);
		memcpy((void *)tx_payload, (const void *)*sbuf, payload_bytes);
	} else {
		assert(iov);
		iov->iov_base = (void *) *sbuf;
		iov->iov_len = payload_bytes;
	}
	(*sbuf) += payload_bytes;
	(*rbuf) += payload_bytes;

	return payload_bytes;
}

__OPX_FORCE_INLINE__
size_t opx_hfi1_dput_write_header_and_payload(
				struct fi_opx_ep *opx_ep,
				union fi_opx_hfi1_packet_hdr *tx_hdr,
				union fi_opx_hfi1_packet_payload *tx_payload,
				struct iovec *iov,
				const uint32_t opcode,
				const int64_t psn_orig,
				const uint16_t lrh_dws,
				const uint64_t op64,
				const uint64_t dt64,
				const uint64_t lrh_dlid,
				const uint64_t bth_rx,
				const size_t payload_bytes,
				const uint64_t key,
				const uint64_t fetch_vaddr,
				const uintptr_t target_byte_counter_vaddr,
				uint64_t bytes_sent,
				uint8_t **sbuf,
				uint8_t **cbuf,
				uintptr_t *rbuf)
{
	uint64_t psn = (uint64_t) htonl((uint32_t)psn_orig);

	tx_hdr->qw[0] = opx_ep->rx->tx.dput.hdr.qw[0] | lrh_dlid | ((uint64_t)lrh_dws << 32);
	tx_hdr->qw[1] = opx_ep->rx->tx.dput.hdr.qw[1] | bth_rx;
	tx_hdr->qw[2] = opx_ep->rx->tx.dput.hdr.qw[2] | psn;
	tx_hdr->qw[3] = opx_ep->rx->tx.dput.hdr.qw[3];

	switch(opcode) {
	case FI_OPX_HFI_DPUT_OPCODE_RZV:
	case FI_OPX_HFI_DPUT_OPCODE_RZV_TID:
	case FI_OPX_HFI_DPUT_OPCODE_RZV_NONCONTIG:
		return opx_hfi1_dput_write_header_and_payload_rzv(
				opx_ep, tx_hdr, tx_payload, iov,
				op64, dt64, payload_bytes, opcode,
				target_byte_counter_vaddr, sbuf, rbuf);
		break;
	case FI_OPX_HFI_DPUT_OPCODE_GET:
		return opx_hfi1_dput_write_header_and_payload_get(
				opx_ep, tx_hdr, tx_payload, iov,
				dt64, payload_bytes,
				target_byte_counter_vaddr, sbuf, rbuf);
		break;
	case FI_OPX_HFI_DPUT_OPCODE_PUT:
		return opx_hfi1_dput_write_header_and_payload_put(
				opx_ep, tx_hdr, tx_payload,
				iov, op64, dt64, payload_bytes,
				key, sbuf, rbuf);
		break;
	case FI_OPX_HFI_DPUT_OPCODE_ATOMIC_FETCH:
		return opx_hfi1_dput_write_header_and_payload_atomic_fetch(
				opx_ep, tx_hdr, tx_payload, iov, op64, dt64,
				payload_bytes, key, fetch_vaddr,
				target_byte_counter_vaddr,
				bytes_sent, sbuf, rbuf);
		break;
	case FI_OPX_HFI_DPUT_OPCODE_ATOMIC_COMPARE_FETCH:
		return opx_hfi1_dput_write_header_and_payload_atomic_compare_fetch(
				opx_ep, tx_hdr, tx_payload, iov, op64, dt64,
				payload_bytes, key, fetch_vaddr,
				target_byte_counter_vaddr,
				bytes_sent, sbuf, cbuf, rbuf);
		break;
	default:
		FI_WARN(fi_opx_global.prov, FI_LOG_EP_DATA,
			"Invalid opcode %0X; abort\n", opcode);
		abort();
	}
}

int fi_opx_hfi1_do_dput (union fi_opx_hfi1_deferred_work * work)
{
	struct fi_opx_hfi1_dput_params *params = &work->dput;
	struct fi_opx_ep * opx_ep = params->opx_ep;
	struct fi_opx_mr * opx_mr = params->opx_mr;
	const uint8_t u8_rx = params->u8_rx;
	const uint32_t niov = params->niov;
	const struct fi_opx_hfi1_dput_iov * const dput_iov = params->dput_iov;
	const uintptr_t target_byte_counter_vaddr = params->target_byte_counter_vaddr;
	uint64_t * origin_byte_counter = params->origin_byte_counter;
	uint64_t key = params->key;
	struct fi_opx_completion_counter *cc = params->cc;
	uint64_t op64 = params->op;
	uint64_t dt64 = params->dt;
	uint32_t opcode = params->opcode;
	const unsigned is_intranode = params->is_intranode;
	const enum ofi_reliability_kind reliability = params->reliability;
	/* use the slid from the lrh header of the incoming packet
	 * as the dlid for the lrh header of the outgoing packet */
	const uint64_t lrh_dlid = params->lrh_dlid;
	const uint64_t bth_rx = ((uint64_t)u8_rx) << 56;
	assert ((opx_ep->tx->pio_max_eager_tx_bytes & 0x3fu) == 0);
	unsigned i;
	const void* sbuf_start = (opx_mr == NULL) ? 0 : opx_mr->buf;

	/* Note that lrh_dlid is just the version of params->slid shifted so
	   that it can be OR'd into the correct position in the packet header */
	assert(params->slid == (lrh_dlid >> 16));

	uint64_t max_bytes_per_packet;

	if (is_intranode) {
		max_bytes_per_packet = FI_OPX_HFI1_PACKET_MTU;
	} else {
		max_bytes_per_packet = opx_ep->tx->pio_flow_eager_tx_bytes;
	}

	assert(((opcode == FI_OPX_HFI_DPUT_OPCODE_ATOMIC_FETCH ||
			opcode == FI_OPX_HFI_DPUT_OPCODE_ATOMIC_COMPARE_FETCH) &&
			params->payload_bytes_for_iovec == sizeof(struct fi_opx_hfi1_dput_fetch))
		||
		(opcode != FI_OPX_HFI_DPUT_OPCODE_ATOMIC_FETCH &&
			opcode != FI_OPX_HFI_DPUT_OPCODE_ATOMIC_COMPARE_FETCH &&
			params->payload_bytes_for_iovec == 0));

	/* Possible SHM connections required for certain applications (i.e., DAOS)
	 * exceeds the max value of the legacy u8_rx field.  Use u32_extended field.
	 */
	ssize_t rc = fi_opx_shm_dynamic_tx_connect(params->is_intranode, opx_ep,
		params->u32_extended_rx, params->target_hfi_unit);

	if (OFI_UNLIKELY(rc)) {
		return -FI_EAGAIN;
	}

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		"===================================== SEND DPUT, %s opcode %d -- (begin)\n", is_intranode ? "SHM" : "HFI", opcode);

	for (i=params->cur_iov; i<niov; ++i) {
		uint8_t * sbuf = (uint8_t*)((uintptr_t)sbuf_start + (uintptr_t)dput_iov[i].sbuf + params->bytes_sent);
		uintptr_t rbuf = dput_iov[i].rbuf + params->bytes_sent;

		uint64_t bytes_to_send = dput_iov[i].bytes - params->bytes_sent;
		while (bytes_to_send > 0) {
			uint64_t bytes_to_send_this_packet = MIN(bytes_to_send + params->payload_bytes_for_iovec,
								max_bytes_per_packet);
			uint64_t tail_bytes = bytes_to_send_this_packet & 0x3Ful;
			uint64_t blocks_to_send_in_this_packet = (bytes_to_send_this_packet >> 6) + (tail_bytes ? 1 : 0);

			const uint64_t pbc_dws = 2 + /* pbc */
						 2 + /* lrh */
						 3 + /* bth */
						 9 + /* kdeth; from "RcvHdrSize[i].HdrSize" CSR */
						 (blocks_to_send_in_this_packet << 4);

			const uint16_t lrh_dws = htons(pbc_dws - 1);

			uint64_t bytes_sent;
			if (is_intranode) {
				uint64_t pos;
				union fi_opx_hfi1_packet_hdr * tx_hdr =
					opx_shm_tx_next(&opx_ep->tx->shm, u8_rx, &pos,
						opx_ep->daos_info.hfi_rank_enabled, params->u32_extended_rx,
						opx_ep->daos_info.rank_inst, &rc);

				if(!tx_hdr) return rc;

				union fi_opx_hfi1_packet_payload * const tx_payload =
					(union fi_opx_hfi1_packet_payload *)(tx_hdr+1);

				bytes_sent = opx_hfi1_dput_write_header_and_payload(
						opx_ep, tx_hdr, tx_payload, NULL,
						opcode, 0, lrh_dws, op64,
						dt64, lrh_dlid, bth_rx,
						bytes_to_send_this_packet, key,
						(const uint64_t)params->fetch_vaddr,
						target_byte_counter_vaddr,
						params->bytes_sent,
						&sbuf, (uint8_t **) &params->compare_vaddr,
						&rbuf);

				opx_shm_tx_advance(&opx_ep->tx->shm, (void*)tx_hdr, pos);
			} else {

				union fi_opx_hfi1_pio_state pio_state = *opx_ep->tx->pio_state;
				uint32_t total_credits_available =
					FI_OPX_HFI1_AVAILABLE_CREDITS(pio_state,
								      &opx_ep->tx->force_credit_return,
								      blocks_to_send_in_this_packet);

				if (total_credits_available <  blocks_to_send_in_this_packet) {
					fi_opx_compiler_msync_writes();
					FI_OPX_HFI1_UPDATE_CREDITS(pio_state, opx_ep->tx->pio_credits_addr);
					total_credits_available =
						FI_OPX_HFI1_AVAILABLE_CREDITS(pio_state,
									      &opx_ep->tx->force_credit_return,
									      blocks_to_send_in_this_packet);
					if (total_credits_available <  blocks_to_send_in_this_packet) {
						opx_ep->tx->pio_state->qw0 = pio_state.qw0;
						return -FI_EAGAIN;
					}
				}

				struct fi_opx_reliability_tx_replay *replay;
				union fi_opx_reliability_tx_psn *psn_ptr;
				int64_t psn;
				if (reliability != OFI_RELIABILITY_KIND_NONE) {
					replay = fi_opx_reliability_client_replay_allocate(&opx_ep->reliability->state, false);
					if(OFI_UNLIKELY(replay == NULL)) {
						return -FI_EAGAIN;
					}

					psn = fi_opx_reliability_tx_next_psn(&opx_ep->ep_fid,
									&opx_ep->reliability->state,
									params->slid,
									u8_rx,
									params->origin_rs,
									&psn_ptr, 1);

					if(OFI_UNLIKELY(psn == -1)) {
						fi_opx_reliability_client_replay_deallocate(&opx_ep->reliability->state, replay);
						return -FI_EAGAIN;
					}
				} else {
					replay = NULL;
					psn_ptr = NULL;
					psn = 0;
				}

				union fi_opx_hfi1_packet_payload *replay_payload =
					(union fi_opx_hfi1_packet_payload *) replay->payload;
				assert(!replay->use_iov);
				assert(((uint8_t *)replay_payload) == ((uint8_t *)&replay->data));
				replay->scb.qw0 = opx_ep->rx->tx.dput.qw0 | pbc_dws |
						((opx_ep->tx->force_credit_return & FI_OPX_HFI1_PBC_CR_MASK)
							<< FI_OPX_HFI1_PBC_CR_SHIFT);

				bytes_sent = opx_hfi1_dput_write_header_and_payload(
						opx_ep, &replay->scb.hdr, replay_payload,
						NULL, opcode, psn, lrh_dws, op64,
						dt64, lrh_dlid, bth_rx,
						bytes_to_send_this_packet, key,
						(const uint64_t) params->fetch_vaddr,
						target_byte_counter_vaddr,
						params->bytes_sent,
						&sbuf, (uint8_t **) &params->compare_vaddr,
						&rbuf);

				FI_OPX_HFI1_CLEAR_CREDIT_RETURN(opx_ep);

				if (opcode == FI_OPX_HFI_DPUT_OPCODE_PUT) {
					fi_opx_reliability_client_replay_register_with_update(
						&opx_ep->reliability->state, params->slid,
						params->origin_rs, u8_rx, psn_ptr, replay, cc,
						bytes_sent, reliability);

					fi_opx_reliability_service_do_replay(&opx_ep->reliability->service, replay);
				} else {
					fi_opx_reliability_service_do_replay(&opx_ep->reliability->service, replay);
					fi_opx_compiler_msync_writes();

					fi_opx_reliability_client_replay_register_no_update(
						&opx_ep->reliability->state, params->slid,
						params->origin_rs, u8_rx, psn_ptr, replay, reliability);
				}
			}

			bytes_to_send -= bytes_sent;
			params->bytes_sent += bytes_sent;

			if(origin_byte_counter) {
				*origin_byte_counter -= bytes_sent;
				assert(((int64_t)*origin_byte_counter) >= 0);
			}
		} /* while bytes_to_send */

		if (opcode == FI_OPX_HFI_DPUT_OPCODE_PUT && is_intranode) {  // RMA-type put, so send a ping/fence to better latency
			fi_opx_shm_write_fence(opx_ep, u8_rx, lrh_dlid, cc, params->bytes_sent, params->u32_extended_rx);
		}

		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
			"===================================== SEND DPUT, %s finished IOV=%d bytes_sent=%ld -- (end)\n",
			is_intranode ? "SHM" : "HFI", params->cur_iov, params->bytes_sent);

		params->bytes_sent = 0;
		params->cur_iov++;
	} /* for niov */

	params->work_elem.complete = true;
	return FI_SUCCESS;
}

int fi_opx_hfi1_dput_sdma_pending_completion(union fi_opx_hfi1_deferred_work *work)
{
	struct fi_opx_hfi1_dput_params *params = &work->dput;
	struct fi_opx_ep * opx_ep = params->opx_ep;

	assert(params->work_elem.low_priority);

	fi_opx_hfi1_sdma_poll_completion(opx_ep);

	struct fi_opx_hfi1_sdma_work_entry *we = (struct fi_opx_hfi1_sdma_work_entry *) params->sdma_reqs.head;
	while (we) {
		// If we're using the SDMA WE bounce buffer, we need to wait for
		// the hit_zero to mark the work element as complete. The replay
		// iovecs are pointing to the SDMA WE bounce buffers, so we can't
		// free the SDMA WEs until the replays are cleared.
		if (!params->work_elem.complete && we->use_bounce_buf) {
			FI_OPX_DEBUG_COUNTERS_INC(work->dput.opx_ep->debug_counters.sdma.eagain_pending_dc);
			return -FI_EAGAIN;
		}
		enum hfi1_sdma_comp_state we_status = fi_opx_hfi1_sdma_get_status(opx_ep, we);
		if (we_status == QUEUED) {
			FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.sdma.eagain_pending_writev);
			FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "FI_EAGAIN\n");
			return -FI_EAGAIN;
		}
		assert(we_status == COMPLETE);

		slist_remove_head(&params->sdma_reqs);
		we->next = NULL;
		fi_opx_hfi1_sdma_return_we(params->opx_ep, we);
		we = (struct fi_opx_hfi1_sdma_work_entry *) params->sdma_reqs.head;
	}

	assert(slist_empty(&params->sdma_reqs));

	if (!params->work_elem.complete) {
		assert(params->delivery_completion);
		FI_OPX_DEBUG_COUNTERS_INC(work->dput.opx_ep->debug_counters.sdma.eagain_pending_dc);
		return -FI_EAGAIN;
	}

	if (params->origin_byte_counter) {
		// If we're not doing delivery_competion, then origin_byte_counter
		// should have already been zero'd and NULL'd at the end of do_dput_sdma(...)
		assert(params->delivery_completion);
		*params->origin_byte_counter = 0;
		params->origin_byte_counter = NULL;
	}

	if (params->user_cc) {
		assert(params->user_cc->byte_counter >= params->cc->initial_byte_count);
		params->user_cc->byte_counter -= params->cc->initial_byte_count;
		if (params->user_cc->byte_counter == 0) {
			params->user_cc->hit_zero(params->user_cc);
		}
	}
	OPX_BUF_FREE(params->cc);

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		"===================================== PENDING DPUT %u COMPLETE\n", work->work_elem.complete);
	return FI_SUCCESS;
}

__OPX_FORCE_INLINE__
void fi_opx_hfi1_dput_copy_to_bounce_buf(uint32_t opcode,
					uint8_t *target_buf,
					uint8_t *source_buf,
					uint8_t *compare_buf,
					void *fetch_vaddr,
					uintptr_t target_byte_counter_vaddr,
					uint64_t buf_packet_bytes,
					uint64_t total_bytes,
					uint64_t bytes_sent)
{
	if (opcode == FI_OPX_HFI_DPUT_OPCODE_ATOMIC_FETCH) {
		while (total_bytes) {
			size_t dput_bytes = MIN(buf_packet_bytes, total_bytes);

			opx_hfi1_dput_write_payload_atomic_fetch(
				(union fi_opx_hfi1_packet_payload *)target_buf,
				dput_bytes,
				(const uint64_t) fetch_vaddr,
				target_byte_counter_vaddr,
				bytes_sent,
				source_buf);

			target_buf += dput_bytes + sizeof(struct fi_opx_hfi1_dput_fetch);
			source_buf += dput_bytes;
			bytes_sent += dput_bytes;

			total_bytes -= dput_bytes;
		}
	} else if (opcode == FI_OPX_HFI_DPUT_OPCODE_ATOMIC_COMPARE_FETCH) {
		buf_packet_bytes >>= 1;
		while (total_bytes) {
			size_t dput_bytes = MIN(buf_packet_bytes, total_bytes);
			size_t dput_bytes_half = dput_bytes >> 1;

			opx_hfi1_dput_write_payload_atomic_compare_fetch(
				(union fi_opx_hfi1_packet_payload *)target_buf,
				dput_bytes_half,
				(const uint64_t) fetch_vaddr,
				target_byte_counter_vaddr,
				bytes_sent,
				source_buf,
				compare_buf);

			target_buf += dput_bytes + sizeof(struct fi_opx_hfi1_dput_fetch);
			source_buf += dput_bytes_half;
			compare_buf += dput_bytes_half;
			bytes_sent += dput_bytes;

			total_bytes -= dput_bytes;
		}
	} else {
		assert(total_bytes <= FI_OPX_HFI1_SDMA_WE_BUF_LEN);
		memcpy(target_buf, source_buf, total_bytes);
	}

}

int fi_opx_hfi1_do_dput_sdma (union fi_opx_hfi1_deferred_work * work)
{
	struct fi_opx_hfi1_dput_params *params = &work->dput;
	struct fi_opx_ep * opx_ep = params->opx_ep;
	struct fi_opx_mr * opx_mr = params->opx_mr;
	const uint8_t u8_rx = params->u8_rx;
	const uint32_t niov = params->niov;
	const struct fi_opx_hfi1_dput_iov * const dput_iov = params->dput_iov;
	const uintptr_t target_byte_counter_vaddr = params->target_byte_counter_vaddr;
	uint64_t key = params->key;
	uint64_t op64 = params->op;
	uint64_t dt64 = params->dt;
	uint32_t opcode = params->opcode;
	const enum ofi_reliability_kind reliability = params->reliability;
	/* use the slid from the lrh header of the incoming packet
	 * as the dlid for the lrh header of the outgoing packet */
	const uint64_t lrh_dlid = params->lrh_dlid;
	const uint64_t bth_rx = ((uint64_t)u8_rx) << 56;
	assert ((opx_ep->tx->pio_max_eager_tx_bytes & 0x3fu) == 0);
	unsigned i;
	const void* sbuf_start = (opx_mr == NULL) ? 0 : opx_mr->buf;
	const bool delivery_completion = params->delivery_completion;
	const bool use_tid = params->use_tid;

	/* Note that lrh_dlid is just the version of params->slid shifted so
	   that it can be OR'd into the correct position in the packet header */
	assert(params->slid == (lrh_dlid >> 16));

	// We should never be in this function for intranode ops
	assert(!params->is_intranode);
	assert(opx_ep->rx->tx.dput.hdr.stl.lrh.slid != params->slid);

	assert(((opcode == FI_OPX_HFI_DPUT_OPCODE_ATOMIC_FETCH ||
			opcode == FI_OPX_HFI_DPUT_OPCODE_ATOMIC_COMPARE_FETCH) &&
			params->payload_bytes_for_iovec == sizeof(struct fi_opx_hfi1_dput_fetch))
		||
		(opcode != FI_OPX_HFI_DPUT_OPCODE_ATOMIC_FETCH &&
			opcode != FI_OPX_HFI_DPUT_OPCODE_ATOMIC_COMPARE_FETCH &&
			params->payload_bytes_for_iovec == 0));

	assert((opcode == FI_OPX_HFI_DPUT_OPCODE_PUT && params->delivery_completion) ||
		(opcode == FI_OPX_HFI_DPUT_OPCODE_GET && params->delivery_completion) ||
		(opcode != FI_OPX_HFI_DPUT_OPCODE_PUT && opcode != FI_OPX_HFI_DPUT_OPCODE_GET));

	// Even though we're using SDMA, replays would previously default to PIO,
	// which would limit the SDMA payload size on credit-constrained systems.
	// Instead, we'll use SDMA replays and use larger payloads - avoiding
	// PIO replay restrictions.
	bool replay_use_sdma = false; /* Use PIO replays by default */
	uint64_t max_eager_bytes = opx_ep->tx->pio_max_eager_tx_bytes;
	if (use_tid && max_eager_bytes < FI_OPX_HFI1_PACKET_MTU) {
		replay_use_sdma = true;
		max_eager_bytes = FI_OPX_HFI1_PACKET_MTU;
	}
	uint64_t max_dput_bytes = max_eager_bytes - params->payload_bytes_for_iovec;

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		"===================================== SEND DPUT SDMA, opcode %X -- (begin)\n", opcode);

	for (i=params->cur_iov; i<niov; ++i) {
		uint32_t *tidpairs= NULL;
		uint32_t tididx = params->tididx;
		uint32_t tidlen_consumed =  params->tidlen_consumed;
		uint32_t tidlen_remaining = params->tidlen_remaining;
		uint32_t prev_tididx = 0;
		uint32_t prev_tidlen_consumed = 0;
		uint32_t prev_tidlen_remaining = 0;
		uint32_t tidoffset = 0;
		uint32_t tidOMshift = 0;
		if (use_tid) {
			if (tididx == -1U) { /* first time */
				tididx = 0;
				tidpairs = (uint32_t *)params->tid_iov.iov_base;
				tidlen_remaining = FI_OPX_EXP_TID_GET(tidpairs[0],LEN);
				tidlen_consumed =  0;
			} else { /* eagain retry, restore previous TID state */
				tidpairs = (uint32_t *)params->tid_iov.iov_base;
			}
		}
		uint32_t starting_tid_idx = tididx;

		assert((i == 0) || (use_tid == 0));
		uint8_t * sbuf = (uint8_t*)((uintptr_t)sbuf_start + (uintptr_t)dput_iov[i].sbuf + params->bytes_sent);
		uintptr_t rbuf = dput_iov[i].rbuf + params->bytes_sent;

		uint64_t bytes_to_send = dput_iov[i].bytes - params->bytes_sent;

		while (bytes_to_send > 0) {
			fi_opx_hfi1_sdma_poll_completion(opx_ep);
			if (!params->sdma_we) {
				/* Get an SDMA work entry since we don't already have one */
				params->sdma_we = opx_sdma_get_new_work_entry(opx_ep,
							&params->sdma_reqs_used,
							&params->sdma_reqs,
							params->sdma_we);
				if (!params->sdma_we) {
					FI_OPX_DEBUG_COUNTERS_INC_COND((params->sdma_reqs_used < FI_OPX_HFI1_SDMA_MAX_WE_PER_REQ),
									opx_ep->debug_counters.sdma.eagain_sdma_we_none_free);
					FI_OPX_DEBUG_COUNTERS_INC_COND((params->sdma_reqs_used == FI_OPX_HFI1_SDMA_MAX_WE_PER_REQ),
									opx_ep->debug_counters.sdma.eagain_sdma_we_max_used);
					FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
						"===================================== SEND DPUT SDMA, !WE FI_EAGAIN\n");
					return -FI_EAGAIN;
				}
				assert(params->sdma_we->total_payload == 0);
				fi_opx_hfi1_sdma_init_we(params->sdma_we,
							params->cc,
							params->slid,
							params->origin_rs,
							params->u8_rx);
			}
			assert(!fi_opx_hfi1_sdma_has_unsent_packets(params->sdma_we));

			if (opx_ep->hfi->info.sdma.available_counter < 1) {
				FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.sdma.eagain_fill_index);
				FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
					"===================================== SEND DPUT SDMA, !CNTR FI_EAGAIN\n");
				return -FI_EAGAIN;
			}

			/* The driver treats the offset as a 4-byte value, so we
			 * need to avoid sending a payload size that would wrap
			 * that in a single SDMA send */
			uintptr_t rbuf_wrap = (rbuf + 0x100000000ul) & 0xFFFFFFFF00000000ul;
			uint64_t sdma_we_bytes = MIN(bytes_to_send, rbuf_wrap - rbuf);
			uint64_t packet_count = (sdma_we_bytes / max_dput_bytes) +
						((sdma_we_bytes % max_dput_bytes) ? 1 : 0);

			assert(packet_count > 0);
			packet_count = MIN(packet_count, FI_OPX_HFI1_SDMA_MAX_PACKETS);

			int32_t psns_avail = fi_opx_reliability_tx_available_psns(&opx_ep->ep_fid,
										  &opx_ep->reliability->state,
										  params->slid,
										  params->u8_rx,
										  params->origin_rs,
										  &params->sdma_we->psn_ptr,
										  packet_count,
										  max_eager_bytes);

			if (psns_avail < (int64_t) packet_count) {
				FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.sdma.eagain_psn);
				FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
					     "===================================== SEND DPUT SDMA, !PSN FI_EAGAIN\n");
				return -FI_EAGAIN;
			}
#ifndef OPX_TID_SEQ_WRAP /* defining this will force replay of some packets over SDMA */
			if (use_tid) {
				const int psn = params->sdma_we->psn_ptr->psn.psn;
				packet_count = MIN(packet_count, 0x800 - (psn & 0x7FF));
			}
#else
			replay_use_sdma = true; /* Use SDMA replays for debug */
			FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,"OPX_TID_SEQ_WRAP replay SDMA %u\n",replay_use_sdma);
#endif
			/* In the unlikely event that we'll be sending a single
			 * packet who's payload size is not a multiple of 4,
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
			bool need_padding = (packet_count == 1 && (sdma_we_bytes & 0x3ul));
			params->sdma_we->use_bounce_buf = (!delivery_completion ||
				opcode == FI_OPX_HFI_DPUT_OPCODE_ATOMIC_FETCH ||
				opcode == FI_OPX_HFI_DPUT_OPCODE_ATOMIC_COMPARE_FETCH ||
				need_padding);

			uint8_t *sbuf_tmp;
			if (params->sdma_we->use_bounce_buf) {
				fi_opx_hfi1_dput_copy_to_bounce_buf(opcode,
							params->sdma_we->bounce_buf.buf,
							sbuf,
							(uint8_t *) params->compare_vaddr,
							params->fetch_vaddr,
							params->target_byte_counter_vaddr,
							max_dput_bytes,
							MIN((packet_count * max_dput_bytes), sdma_we_bytes),
							params->bytes_sent);
				sbuf_tmp = params->sdma_we->bounce_buf.buf;
			} else {
				sbuf_tmp = sbuf;
			}
			// At this point, we have enough SDMA queue entries and PSNs
			// to send packet_count packets. The only limit now is how
			// many replays can we get.
			for (int p = 0; p < packet_count; ++p) {
				assert(sdma_we_bytes); // If this fails, we did math wrong
				uint64_t packet_bytes = MIN(sdma_we_bytes, max_dput_bytes) + params->payload_bytes_for_iovec;
				assert(packet_bytes <= FI_OPX_HFI1_PACKET_MTU);
				if (use_tid) {
					if (p == 0) { /* First packet header is user's responsibility even with SDMA/AHG*/
						/* set fields for first header */
						starting_tid_idx = tididx; /* first tid this write() */
						if ((FI_OPX_EXP_TID_GET(tidpairs[tididx],LEN)) >= KDETH_OM_MAX_SIZE/FI_OPX_HFI1_TID_SIZE) {
							tidOMshift = (1 << HFI_KHDR_OM_SHIFT);
							tidoffset = (tidlen_consumed * FI_OPX_HFI1_TID_SIZE) >> KDETH_OM_LARGE_SHIFT;
						} else {
							tidOMshift = 0;
							tidoffset = (tidlen_consumed * FI_OPX_HFI1_TID_SIZE) >> KDETH_OM_SMALL_SHIFT;
						}
					}
					/* Save current values in case we can't process this packet (!REPLAY)
					   and need to restore state */
					prev_tididx = tididx;
					prev_tidlen_consumed = tidlen_consumed;
					prev_tidlen_remaining = tidlen_remaining;
					/* Check tid for each packet and determine if AHG will use 4k or 8k packet */
					/* Assume any CTRL 3 tidpair optimizations were already done, or are not wanted,
					   so only a single tidpair per packet is possible. */
					if (packet_bytes > FI_OPX_HFI1_TID_SIZE && tidlen_remaining >= 2) {
						/* at least 2 pages, 8k mapped by this tidpair,
						   calculated packet_bytes is ok. */
						assert(max_eager_bytes==FI_OPX_HFI1_PACKET_MTU);
						tidlen_remaining -= 2;
						tidlen_consumed  += 2;
					} else {
						/* only 1 page left or only 4k packet possible */
						packet_bytes = MIN(packet_bytes, FI_OPX_HFI1_TID_SIZE);
						tidlen_remaining -= 1;
						tidlen_consumed  += 1;
					}
					if (tidlen_remaining == 0) {
						tididx++;
						tidlen_remaining = FI_OPX_EXP_TID_GET(tidpairs[tididx],LEN);
						tidlen_consumed =  0;
					}
				}

				struct fi_opx_reliability_tx_replay *replay;
				if (reliability != OFI_RELIABILITY_KIND_NONE) {
					replay = fi_opx_reliability_client_replay_allocate(
						&opx_ep->reliability->state, true);
					if(OFI_UNLIKELY(replay == NULL)) {
						/* Restore previous values in case since we can't process this
						 * packet. We may or may not -FI_EAGAIN later (!REPLAY).*/
						tididx = prev_tididx;
						tidlen_consumed = prev_tidlen_consumed;
						tidlen_remaining = prev_tidlen_remaining;
						FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
							     "!REPLAY on packet %u out of %lu, params->sdma_we->num_packets %u\n",
							     p, packet_count, params->sdma_we->num_packets);
						break;
					}
					replay->use_sdma = replay_use_sdma;
				} else {
					replay = NULL;
				}

				// Round packet_bytes up to the next multiple of 4,
				// then divide by 4 to get the correct number of dws.
				uint64_t payload_dws = ((packet_bytes + 3) & -4) >> 2;
				const uint64_t pbc_dws = 2 + /* pbc */
							2 + /* lrh */
							3 + /* bth */
							9 + /* kdeth; from "RcvHdrSize[i].HdrSize" CSR */
							payload_dws;

				const uint16_t lrh_dws = htons(pbc_dws - 1);

				replay->scb.qw0 = opx_ep->rx->tx.dput.qw0 | pbc_dws;

				// Passing in PSN of 0 for this because we'll set it later
				uint64_t bytes_sent =
					opx_hfi1_dput_write_header_and_payload(
						opx_ep, &replay->scb.hdr, NULL,	
						replay->iov, opcode, 0,
						lrh_dws, op64, dt64, lrh_dlid,
						bth_rx, packet_bytes, key,
						(const uint64_t) params->fetch_vaddr,
						target_byte_counter_vaddr,
						params->bytes_sent, &sbuf_tmp,
						(uint8_t **) &params->compare_vaddr,
						&rbuf);

				params->cc->byte_counter += params->payload_bytes_for_iovec;
				fi_opx_hfi1_sdma_add_packet(params->sdma_we, replay, packet_bytes);

				bytes_to_send -= bytes_sent;
				sdma_we_bytes -= bytes_sent;
				params->bytes_sent += bytes_sent;
				sbuf += bytes_sent;
			}

			// Must be we had trouble getting a replay buffer
			if (OFI_UNLIKELY(params->sdma_we->num_packets == 0)) {
				FI_OPX_DEBUG_COUNTERS_INC(opx_ep->debug_counters.sdma.eagain_replay);
				FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
					"===================================== SEND DPUT SDMA, !REPLAY FI_EAGAIN\n");
				return -FI_EAGAIN;
			}

			fi_opx_hfi1_sdma_flush(opx_ep,
					       params->sdma_we,
					       &params->sdma_reqs,
					       use_tid,
					       &params->tid_iov,
					       starting_tid_idx,
					       tidOMshift,
					       tidoffset,
					       reliability);
			params->sdma_we = NULL;
			/* save our 'done' tid state incase we return EAGAIN next loop */
			params->tididx = tididx;
			params->tidlen_consumed = tidlen_consumed;
			params->tidlen_remaining = tidlen_remaining;

		} /* while bytes_to_send */

		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
			"===================================== SEND DPUT SDMA, finished IOV=%d(%d) bytes_sent=%ld\n",
			params->cur_iov, niov, params->bytes_sent);

		params->bytes_sent = 0;
		params->cur_iov++;
	} /* for niov */
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		"===================================== SEND DPUT SDMA, exit (end)\n");

	// At this point, all SDMA WE should have succeeded sending, and only reside on the reqs list
	assert(params->sdma_we == NULL);
	assert(!slist_empty(&params->sdma_reqs));

	// If we're not doing delivery completion, the user's payload would have
	// been copied to bounce buffer(s), so at this point, it should be safe
	// for the user to alter the send buffer even though the send may still
	// be in progress.
	if (!params->delivery_completion) {
		assert(params->origin_byte_counter);
		*params->origin_byte_counter = 0;
		params->origin_byte_counter = NULL;
	}
	params->work_elem.low_priority = true;
	params->work_elem.work_fn = fi_opx_hfi1_dput_sdma_pending_completion;

	return fi_opx_hfi1_dput_sdma_pending_completion(work);
}

union fi_opx_hfi1_deferred_work* fi_opx_hfi1_rx_rzv_cts (struct fi_opx_ep * opx_ep,
							 struct fi_opx_mr * opx_mr,
							 const void * const hdr,
							 const void * const payload,
							 size_t payload_bytes_to_copy,
							 const uint8_t u8_rx,
							 const uint8_t origin_rs,
							 const uint32_t niov,
							 const struct fi_opx_hfi1_dput_iov * const dput_iov,
							 const uint8_t op,
							 const uint8_t dt,
							 const uintptr_t target_byte_counter_vaddr,
							 uint64_t * origin_byte_counter,
							 uint32_t opcode,
							 void (*completion_action)(union fi_opx_hfi1_deferred_work * work_state),
							 const unsigned is_intranode,
							 const enum ofi_reliability_kind reliability,
							 const uint32_t u32_extended_rx) {
	const union fi_opx_hfi1_packet_hdr * const hfi1_hdr =
		(const union fi_opx_hfi1_packet_hdr * const) hdr;

	union fi_opx_hfi1_deferred_work *work = ofi_buf_alloc(opx_ep->tx->work_pending_pool);
	struct fi_opx_hfi1_dput_params *params = &work->dput;

	params->work_elem.slist_entry.next = NULL;
	params->work_elem.work_fn = fi_opx_hfi1_do_dput;
	params->work_elem.completion_action = completion_action;
	params->work_elem.payload_copy = NULL;
	params->work_elem.complete = false;
	params->work_elem.low_priority = false;
	params->opx_ep = opx_ep;
	params->opx_mr = opx_mr;
	params->lrh_dlid = (hfi1_hdr->stl.lrh.qw[0] & 0xFFFF000000000000ul) >> 32;
	params->slid = hfi1_hdr->stl.lrh.slid;
	params->origin_rs = origin_rs;
	params->u8_rx = u8_rx;
	params->u32_extended_rx = u32_extended_rx;
	params->niov = niov;
	params->dput_iov = &params->iov[0];
	params->cur_iov = 0;
	params->bytes_sent = 0;
	params->cc = NULL;
	params->user_cc = NULL;
	params->payload_bytes_for_iovec = 0;
	params->delivery_completion = false;

	params->target_byte_counter_vaddr = target_byte_counter_vaddr;
	params->origin_byte_counter = origin_byte_counter;
	params->opcode = opcode;
	params->op = op;
	params->dt = dt;
	params->is_intranode = is_intranode;
	params->reliability = reliability;
	params->target_hfi_unit = opx_ep->hfi->hfi_unit;

	uint64_t iov_total_bytes = 0;
	for(int idx=0; idx < niov; idx++) {
		params->iov[idx] = dput_iov[idx];
		iov_total_bytes += dput_iov[idx].bytes;
	}
	const uint32_t ntidpairs = ((hfi1_hdr->cts.target.vaddr.opcode == FI_OPX_HFI_DPUT_OPCODE_RZV) ||
				    (hfi1_hdr->cts.target.vaddr.opcode == FI_OPX_HFI_DPUT_OPCODE_RZV_TID) ||
				    (hfi1_hdr->cts.target.vaddr.opcode == FI_OPX_HFI_DPUT_OPCODE_RZV_NONCONTIG)) ?
		hfi1_hdr->cts.target.vaddr.ntidpairs: 0;
	const uint32_t  *const tidpairs = ntidpairs ? ((union fi_opx_hfi1_packet_payload *)payload)->tid_cts.tidpairs : NULL;
	assert((ntidpairs == 0) || (niov == 1));
	if (ntidpairs > 0) {
		/* Receiver may have adjusted the length for expected TID alignment.*/
		assert(origin_byte_counter == NULL || (iov_total_bytes >= *origin_byte_counter));
		if(origin_byte_counter) *origin_byte_counter = iov_total_bytes;
	}
	assert(origin_byte_counter == NULL || iov_total_bytes == *origin_byte_counter);
	fi_opx_hfi1_dput_sdma_init(opx_ep, params, iov_total_bytes, ntidpairs, tidpairs);

	// We can't/shouldn't start this work until any pending work is finished.
	if (slist_empty(&opx_ep->tx->work_pending)) {
		int rc = params->work_elem.work_fn(work);
		if(rc == FI_SUCCESS) {
			FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
				"===================================== CTS done %u\n", params->work_elem.complete);
			assert(params->work_elem.complete);
			OPX_BUF_FREE(work);
			return NULL;
		}
		assert(rc == -FI_EAGAIN);
		if (params->work_elem.low_priority) {
			FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
				"===================================== CTS FI_EAGAIN queued low priority %u\n", params->work_elem.complete);
			slist_insert_tail(&work->work_elem.slist_entry, &opx_ep->tx->work_pending_completion);
			return NULL;
		}
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
			"===================================== CTS FI_EAGAIN queued %u, payload_bytes_to_copy %zu\n", params->work_elem.complete,payload_bytes_to_copy);
	} else {
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
			"===================================== CTS queued with work pending %u, payload_bytes_to_copy %zu\n", params->work_elem.complete,payload_bytes_to_copy);
	}

	/* Try again later*/
	if(payload_bytes_to_copy) {
		params->work_elem.payload_copy = ofi_buf_alloc(opx_ep->tx->rma_payload_pool);
		memcpy(params->work_elem.payload_copy, payload, payload_bytes_to_copy);
	}
	assert(work->work_elem.slist_entry.next == NULL);
	slist_insert_tail(&work->work_elem.slist_entry, &opx_ep->tx->work_pending);
	return work;
}



static inline size_t fi_opx_iov_to_payload_blocks(size_t niov) {
	size_t sz_bytes = niov * sizeof(struct iovec);
	sz_bytes += (sizeof(uintptr_t) + // origin_byte_counter_vaddr
				 sizeof(size_t));    // unused field
	/* (bytes * 64) + ((bytes % 64) ? 1 : 0) */
	return (sz_bytes >> 6) + ((sz_bytes & 0x3f) ? 1 : 0);
}

uint64_t num_sends;
uint64_t total_sendv_bytes;
ssize_t fi_opx_hfi1_tx_sendv_rzv(struct fid_ep *ep, const struct iovec *iov, size_t niov,
				 size_t total_len, void *desc, fi_addr_t dest_addr, uint64_t tag,
				 void *context, const uint32_t data, int lock_required,
				 const unsigned override_flags, uint64_t tx_op_flags,
				 const uint64_t dest_rx, const uintptr_t origin_byte_counter_vaddr,
				 uint64_t *origin_byte_counter_value, const uint64_t caps,
				 const enum ofi_reliability_kind reliability)
{
	// We should already have grabbed the lock prior to calling this function
	assert(!lock_required);

	struct fi_opx_ep *opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);
	const union fi_opx_addr addr = { .fi = dest_addr };
	const uint64_t payload_blocks_total = fi_opx_iov_to_payload_blocks(niov);
	const uint64_t bth_rx = ((uint64_t)dest_rx) << 56;
	const uint64_t lrh_dlid = FI_OPX_ADDR_TO_HFI1_LRH_DLID(addr.fi);
	assert(niov <= FI_OPX_MAX_DPUT_IOV);
	*origin_byte_counter_value = total_len;

	/* This is a hack to trick an MPICH test to make some progress    */
	/* As it erroneously overflows the send buffers by never checking */
	/* for multi-receive overflows properly in some onesided tests    */
	/* There are almost certainly better ways to do this */
	if((tx_op_flags & FI_MSG) && (total_sendv_bytes+=total_len > opx_ep->rx->min_multi_recv)) {
		total_sendv_bytes = 0;
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "FI_EAGAIN\n");
		return -FI_EAGAIN;
	}

	const uint64_t pbc_dws = 2 + /* pbc */
				 2 + /* lhr */
				 3 + /* bth */
				 9 + /* kdeth; from "RcvHdrSize[i].HdrSize" CSR */
				 (payload_blocks_total << 4);

	const uint16_t lrh_dws = htons(pbc_dws - 1);

	if (((caps & (FI_LOCAL_COMM | FI_REMOTE_COMM)) == FI_LOCAL_COMM) || /* compile-time constant expression */
	    (((caps & (FI_LOCAL_COMM | FI_REMOTE_COMM)) == (FI_LOCAL_COMM | FI_REMOTE_COMM)) &&
	     (opx_ep->tx->send.hdr.stl.lrh.slid == addr.uid.lid))) {
		FI_DBG_TRACE(
			fi_opx_global.prov, FI_LOG_EP_DATA,
			"===================================== SENDV, SHM -- RENDEZVOUS RTS Noncontig (begin) context %p\n",context);

		uint64_t pos;
		ssize_t rc;
		union fi_opx_hfi1_packet_hdr *const hdr = opx_shm_tx_next(
			&opx_ep->tx->shm, dest_rx, &pos, opx_ep->daos_info.hfi_rank_enabled,
			opx_ep->daos_info.rank, opx_ep->daos_info.rank_inst, &rc);

		if (!hdr) return rc;

		hdr->qw[0] = opx_ep->tx->rzv.hdr.qw[0] | lrh_dlid | ((uint64_t)lrh_dws << 32);
		hdr->qw[1] = opx_ep->tx->rzv.hdr.qw[1] | bth_rx |
			     ((caps & FI_MSG) ? (uint64_t)FI_OPX_HFI_BTH_OPCODE_MSG_RZV_RTS :
						(uint64_t)FI_OPX_HFI_BTH_OPCODE_TAG_RZV_RTS);

		hdr->qw[2] = opx_ep->tx->rzv.hdr.qw[2];
		hdr->qw[3] = opx_ep->tx->rzv.hdr.qw[3] | (((uint64_t)data) << 32);
		hdr->qw[4] = opx_ep->tx->rzv.hdr.qw[4] | (niov << 48);
		hdr->qw[5] = total_len;
		hdr->qw[6] = tag;

		union fi_opx_hfi1_packet_payload *const payload =
			(union fi_opx_hfi1_packet_payload *)(hdr + 1);

		payload->rendezvous.noncontiguous.origin_byte_counter_vaddr = origin_byte_counter_vaddr;
		payload->rendezvous.noncontiguous.unused = 0;
		ssize_t idx;
		for(idx = 0; idx < niov; idx++) {
			payload->rendezvous.noncontiguous.iov[idx] = iov[idx];
		}

		opx_shm_tx_advance(&opx_ep->tx->shm, (void *)hdr, pos);

		FI_DBG_TRACE(
			fi_opx_global.prov, FI_LOG_EP_DATA,
			"===================================== SENDV, SHM -- RENDEZVOUS RTS (end) context %p\n",context);
		fi_opx_shm_poll_many(&opx_ep->ep_fid, 0);
		return FI_SUCCESS;
	}
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		     "===================================== SENDV, HFI -- RENDEZVOUS RTS (begin) context %p\n",context);

	union fi_opx_hfi1_pio_state pio_state = *opx_ep->tx->pio_state;
	const uint16_t total_credits_needed = 1 +   /* packet header */
					      payload_blocks_total; /* packet payload */

	uint64_t total_credits_available = FI_OPX_HFI1_AVAILABLE_CREDITS(pio_state, &opx_ep->tx->force_credit_return, total_credits_needed);
	unsigned loop = 0;
	while (OFI_UNLIKELY(total_credits_available < total_credits_needed)) {
		if (loop++ > FI_OPX_HFI1_TX_SEND_RZV_CREDIT_MAX_WAIT) {
			opx_ep->tx->pio_state->qw0 = pio_state.qw0;
			FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "FI_EAGAIN\n");
			return -FI_EAGAIN;
		}
		fi_opx_compiler_msync_writes();
		FI_OPX_HFI1_UPDATE_CREDITS(pio_state, opx_ep->tx->pio_credits_addr);
		total_credits_available = FI_OPX_HFI1_AVAILABLE_CREDITS(pio_state, &opx_ep->tx->force_credit_return, total_credits_needed);
	}
	if (OFI_UNLIKELY(loop)) {
		opx_ep->tx->pio_state->qw0 = pio_state.qw0;
	}

	struct fi_opx_reliability_tx_replay *replay = (reliability != OFI_RELIABILITY_KIND_NONE) ?
		fi_opx_reliability_client_replay_allocate(&opx_ep->reliability->state, false) : NULL;
	if (replay == NULL) {
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "FI_EAGAIN\n");
		return -FI_EAGAIN;
	}


	union fi_opx_reliability_tx_psn *psn_ptr = NULL;
	const int64_t psn =
		(reliability != OFI_RELIABILITY_KIND_NONE) ? /* compile-time constant expression */
			fi_opx_reliability_tx_next_psn(&opx_ep->ep_fid,
							&opx_ep->reliability->state,
							addr.uid.lid,
							dest_rx,
							addr.reliability_rx,
							&psn_ptr, 1) :
			0;
	if(OFI_UNLIKELY(psn == -1)) {
		fi_opx_reliability_client_replay_deallocate(&opx_ep->reliability->state, replay);
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA, "FI_EAGAIN\n");
		return -FI_EAGAIN;
	}

	volatile uint64_t * const scb =
		FI_OPX_HFI1_PIO_SCB_HEAD(opx_ep->tx->pio_scb_sop_first, pio_state);
	uint64_t tmp[8];
	assert(opx_ep->tx->rzv.qw0 == 0);
	tmp[0] = opx_ep->tx->rzv.qw0 | pbc_dws | ((opx_ep->tx->force_credit_return & FI_OPX_HFI1_PBC_CR_MASK) << FI_OPX_HFI1_PBC_CR_SHIFT);
	tmp[1] = opx_ep->tx->rzv.hdr.qw[0] | lrh_dlid | ((uint64_t)lrh_dws << 32);
	tmp[2] = opx_ep->tx->rzv.hdr.qw[1] | bth_rx |
		((caps & FI_MSG) ? (uint64_t)FI_OPX_HFI_BTH_OPCODE_MSG_RZV_RTS :
		 (uint64_t)FI_OPX_HFI_BTH_OPCODE_TAG_RZV_RTS);
	tmp[3] = opx_ep->tx->rzv.hdr.qw[2] | psn;
	tmp[4] = opx_ep->tx->rzv.hdr.qw[3] | (((uint64_t)data) << 32);
	tmp[5] = opx_ep->tx->rzv.hdr.qw[4] | (niov << 48);
	tmp[6] = total_len;
	tmp[7] = tag;

	scb[0] = tmp[0];
	scb[1] = tmp[1];
	scb[2] = tmp[2];
	scb[3] = tmp[3];
	scb[4] = tmp[4];
	scb[5] = tmp[5];
	scb[6] = tmp[6];
	scb[7] = tmp[7];

	/* consume one credit for the packet header */
	--total_credits_available;
	FI_OPX_HFI1_CONSUME_SINGLE_CREDIT(pio_state);
#ifndef NDEBUG
	unsigned credits_consumed = 1;
#endif

	FI_OPX_HFI1_CLEAR_CREDIT_RETURN(opx_ep);

	/* write the payload */
	const ssize_t total_payload_bytes = niov*sizeof(*iov) /* iovec array */
		             + 16; /* byte counter and unused fields */
	const size_t payload_qws_total = total_payload_bytes >> 3;
	const size_t payload_qws_tail = payload_qws_total & 0x07ul;
	ssize_t iov_idx = 0, iov_base_offset = 0;
	uint64_t tmp_value = 0;
	struct iovec src_iov[3] = {{ (void*)&origin_byte_counter_vaddr, 8 },
							   { &tmp_value, 8 },
							   { (void*)&iov[0], niov*sizeof(*iov)}
				};
	const uint16_t contiguous_credits_until_wrap =
		(uint16_t)(pio_state.credits_total - pio_state.scb_head_index);

	const uint16_t contiguous_credits_available =
		MIN(total_credits_available, contiguous_credits_until_wrap);

	uint16_t full_block_credits_needed = (uint16_t)(payload_qws_total >> 3);
	if(full_block_credits_needed > 0) {
		volatile uint64_t * scb_payload = (uint64_t *)FI_OPX_HFI1_PIO_SCB_HEAD(opx_ep->tx->pio_scb_first, pio_state);
		const uint16_t contiguous_full_blocks_to_write =
			MIN(full_block_credits_needed, contiguous_credits_available);
		int64_t remain = MIN(total_payload_bytes, contiguous_full_blocks_to_write << 6);
		while (false ==
		       fi_opx_hfi1_fill_from_iov8(
			       src_iov,             /* In:  iovec array                      */
			       3,                   /* In:  total iovecs                     */
			       scb_payload,         /* In:  target buffer to fill            */
			       &remain,             /* In/Out:  buffer length to fill        */
			       &iov_idx,            /* In/Out:  start index, returns end     */
			       &iov_base_offset)) { /* In/Out:  start offset, returns offset */
			// copy until done;
		}
		assert(remain == 0);
		full_block_credits_needed -= contiguous_full_blocks_to_write;
		FI_OPX_HFI1_CONSUME_CREDITS(pio_state, contiguous_full_blocks_to_write);
#ifndef NDEBUG
		credits_consumed += contiguous_full_blocks_to_write;
#endif
	}
	if (OFI_UNLIKELY(full_block_credits_needed > 0)) {
		/*
		 * handle wrap condition
		 */
		volatile uint64_t *scb_payload =
			FI_OPX_HFI1_PIO_SCB_HEAD(opx_ep->tx->pio_scb_first, pio_state);

		int64_t remain = (full_block_credits_needed << 6);
		while (false ==
		       fi_opx_hfi1_fill_from_iov8(
			       src_iov,     /* In:  iovec array */
			       3,                   /* In:  total iovecs */
			       scb_payload,         /* In:  target buffer to fill */
			       &remain,             /* In/Out:  buffer length to fill */
			       &iov_idx,            /* In/Out:  start index, returns end */
			       &iov_base_offset)) { /* In/Out:  start offset, returns offset */
			// copy until done;
		}
		assert(remain == 0);
		FI_OPX_HFI1_CONSUME_CREDITS(pio_state, full_block_credits_needed);
#ifndef NDEBUG
		credits_consumed += full_block_credits_needed;
#endif
	}

	if (payload_qws_tail > 0) {
		volatile uint64_t *scb_payload =
			FI_OPX_HFI1_PIO_SCB_HEAD(opx_ep->tx->pio_scb_first, pio_state);
		assert(payload_qws_tail < 8);
		int64_t remain = (payload_qws_tail << 3);
		assert(remain < 64);
		while (false ==
		       fi_opx_hfi1_fill_from_iov8(
			       src_iov, /* In:  iovec array */
			       3, /* In:  total iovecs */
			       scb_payload, /* In:  target buffer to fill */
			       &remain, /* In/Out:  buffer length to fill */
			       &iov_idx, /* In/Out:  start index, returns end */
			       &iov_base_offset)) { /* In/Out:  start offset, returns offset */
			// copy until done;
		}
		for (int i = payload_qws_tail; i < 8; ++i) {
			scb_payload[i] = 0;
		}
		FI_OPX_HFI1_CONSUME_SINGLE_CREDIT(pio_state);
#ifndef NDEBUG
		++credits_consumed;
#endif
	}

	FI_OPX_HFI1_CHECK_CREDITS_FOR_ERROR(opx_ep->tx->pio_credits_addr);
#ifndef NDEBUG
	assert(credits_consumed == total_credits_needed);
#endif

	if (reliability != OFI_RELIABILITY_KIND_NONE) { /* compile-time constant expression */
		replay->scb.qw0 = tmp[0];
		replay->scb.hdr.qw[0] = tmp[1];
		replay->scb.hdr.qw[1] = tmp[2];
		replay->scb.hdr.qw[2] = tmp[3];
		replay->scb.hdr.qw[3] = tmp[4];
		replay->scb.hdr.qw[4] = tmp[5];
		replay->scb.hdr.qw[5] = tmp[6];
		replay->scb.hdr.qw[6] = tmp[7];
		iov_idx = 0;
		iov_base_offset = 0;
		uint64_t *payload = replay->payload;
		assert(!replay->use_iov);
		assert(((uint8_t *)payload) == ((uint8_t *) &replay->data));
		int64_t remain = total_payload_bytes;
		while (false ==
		       fi_opx_hfi1_fill_from_iov8(
			       src_iov, /* In:  iovec array */
			       3, /* In:  total iovecs */
			       payload, /* In:  target buffer to fill */
			       &remain, /* In/Out:  buffer length to fill */
			       &iov_idx, /* In/Out:  start index, returns end */
			       &iov_base_offset)) { /* In/Out:  start offset, returns offset */
			// copy until done;
		}
		assert(remain == 0);
		fi_opx_reliability_client_replay_register_no_update(&opx_ep->reliability->state,
								    addr.uid.lid,
								    addr.reliability_rx, dest_rx,
								    psn_ptr, replay, reliability);
	}


	/* update the hfi txe state */
	opx_ep->tx->pio_state->qw0 = pio_state.qw0;

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		     "===================================== SENDV, HFI -- RENDEZVOUS RTS (end) context %p\n",context);

	return FI_SUCCESS;
}

ssize_t fi_opx_hfi1_tx_send_rzv (struct fid_ep *ep,
		const void *buf, size_t len, void *desc,
		fi_addr_t dest_addr, uint64_t tag, void* context,
		const uint32_t data, int lock_required,
		const unsigned override_flags, uint64_t tx_op_flags,
		const uint64_t dest_rx,
		const uintptr_t origin_byte_counter_vaddr,
		uint64_t *origin_byte_counter_value,
		const uint64_t caps,
		const enum ofi_reliability_kind reliability)
{
	// We should already have grabbed the lock prior to calling this function
	assert(!lock_required);

	//Need at least one full block of payload
	assert(len >= FI_OPX_HFI1_TX_MIN_RZV_PAYLOAD_BYTES);

	struct fi_opx_ep * opx_ep = container_of(ep, struct fi_opx_ep, ep_fid);
	const union fi_opx_addr addr = { .fi = dest_addr };

#ifndef NDEBUG
	const uint64_t max_immediate_block_count = (FI_OPX_HFI1_PACKET_MTU >> 6)-2 ;
#endif
	/* Expected tid needs to send a page (64 block) immediate for alignment.
           Limit this to SDMA (8K+) for now  */
	const bool use_immediate_blocks = len > FI_OPX_SDMA_MIN_LENGTH ? (opx_ep->use_expected_tid_rzv ?  1 : 0) : 0;
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		     "use_immediate_blocks %u *origin_byte_counter_value %#lX, origin_byte_counter_vaddr %p, "
		     "*origin_byte_counter_vaddr %lu/%#lX, len %lu/%#lX\n",
		     use_immediate_blocks, *origin_byte_counter_value, (uint64_t*)origin_byte_counter_vaddr,
		     origin_byte_counter_vaddr ? *(uint64_t*)origin_byte_counter_vaddr : -1UL,
		     origin_byte_counter_vaddr ? *(uint64_t*)origin_byte_counter_vaddr : -1UL, len, len );

	const uint64_t immediate_block_count  = use_immediate_blocks ? 64 : 0;

	/* Expected tid needs to send a trailing data block for alignment.
	   Limit this to SDMA (8K+) for now  */
	const uint64_t immediate_end_block_count = use_immediate_blocks ? 1 : 0;

	assert((immediate_block_count + immediate_end_block_count) <= max_immediate_block_count);

	const uint64_t payload_blocks_total =
		1 +				/* rzv metadata */
		1 +				/* immediate data tail */
		immediate_block_count +
		immediate_end_block_count;


	const uint64_t bth_rx = ((uint64_t)dest_rx) << 56;
	const uint64_t lrh_dlid = FI_OPX_ADDR_TO_HFI1_LRH_DLID(dest_addr);

	const uint64_t immediate_byte_count = len & 0x0007ul;
	const uint64_t immediate_qw_count = (len >> 3) & 0x0007ul;
	/* Immediate total does not include trailing block */
	const uint64_t immediate_total = immediate_byte_count +
		immediate_qw_count * sizeof(uint64_t) +
		immediate_block_count * sizeof(union cacheline);

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		     "max_immediate_block_count %#lX, len %#lX >> 6 %#lX, immediate_total %#lX, "
		     "immediate_byte_count %#lX, immediate_qw_count %#lX, immediate_block_count %#lX, "
		     "origin_byte_counter %lu/%#lX, adjusted origin_byte_counter %lu/%#lX\n",
		     max_immediate_block_count, len, (len >> 6), immediate_total, immediate_byte_count,
		     immediate_qw_count, immediate_block_count, *origin_byte_counter_value,
		     *origin_byte_counter_value, len - immediate_total, len - immediate_total);

	assert(((len - immediate_total) & 0x003Fu) == 0);

	*origin_byte_counter_value = len - immediate_total;
	const uint64_t pbc_dws =
		2 +			/* pbc */
		2 +			/* lhr */
		3 +			/* bth */
		9 +			/* kdeth; from "RcvHdrSize[i].HdrSize" CSR */
		(payload_blocks_total << 4);

	const uint16_t lrh_dws = htons(pbc_dws-1);

	if (((caps & (FI_LOCAL_COMM | FI_REMOTE_COMM)) == FI_LOCAL_COMM) ||	/* compile-time constant expression */
		(((caps & (FI_LOCAL_COMM | FI_REMOTE_COMM)) == (FI_LOCAL_COMM | FI_REMOTE_COMM)) &&
			(opx_ep->tx->send.hdr.stl.lrh.slid == addr.uid.lid))) {

		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
			"===================================== SEND, SHM -- RENDEZVOUS RTS (begin) context %p\n",context);
		uint64_t pos;
		ssize_t rc;
		union fi_opx_hfi1_packet_hdr * const hdr =
			opx_shm_tx_next(&opx_ep->tx->shm, dest_rx, &pos,
				opx_ep->daos_info.hfi_rank_enabled, opx_ep->daos_info.rank,
				opx_ep->daos_info.rank_inst, &rc);

		if (!hdr) {
			FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,"return %zd\n",rc);
			return rc;
		}

		hdr->qw[0] = opx_ep->tx->rzv.hdr.qw[0] | lrh_dlid | ((uint64_t)lrh_dws << 32);

		hdr->qw[1] = opx_ep->tx->rzv.hdr.qw[1] | bth_rx |
			((caps & FI_MSG) ?
				(uint64_t)FI_OPX_HFI_BTH_OPCODE_MSG_RZV_RTS :
				(uint64_t)FI_OPX_HFI_BTH_OPCODE_TAG_RZV_RTS);

		hdr->qw[2] = opx_ep->tx->rzv.hdr.qw[2];
		hdr->qw[3] = opx_ep->tx->rzv.hdr.qw[3] | (((uint64_t)data) << 32);
		hdr->qw[4] = opx_ep->tx->rzv.hdr.qw[4] | (1ull << 48); /* effectively 1 iov */
		hdr->qw[5] = len;
		hdr->qw[6] = tag;

		union fi_opx_hfi1_packet_payload * const payload =
			(union fi_opx_hfi1_packet_payload *)(hdr+1);
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,"hdr %p, payuload %p, sbuf %p, sbuf+immediate_total %p, immediate_total %#lX, adj len %#lX\n",
			     hdr, payload,
			     buf, ((char*)buf + immediate_total),immediate_total, (len - immediate_total));

		payload->rendezvous.contiguous.src_vaddr = (uintptr_t)buf + immediate_total;
		payload->rendezvous.contiguous.src_blocks = (len - immediate_total) >> 6;
		payload->rendezvous.contiguous.immediate_byte_count = immediate_byte_count;
		payload->rendezvous.contiguous.immediate_qw_count = immediate_qw_count;
		payload->rendezvous.contiguous.immediate_block_count = immediate_block_count;
		payload->rendezvous.contiguous.origin_byte_counter_vaddr = origin_byte_counter_vaddr;
		payload->rendezvous.contiguous.immediate_end_block_count =  immediate_end_block_count;
		payload->rendezvous.contiguous.unused[0] = 0;


		uint8_t *sbuf = (uint8_t *)buf;

		if (immediate_byte_count > 0) {
			memcpy((void*)&payload->rendezvous.contiguous.immediate_byte, (const void*)sbuf, immediate_byte_count);
			sbuf += immediate_byte_count;
		}

		uint64_t * sbuf_qw = (uint64_t *)sbuf;
		unsigned i=0;
		for (i=0; i<immediate_qw_count; ++i) {
			payload->rendezvous.contiguous.immediate_qw[i] = sbuf_qw[i];
		}

		sbuf_qw += immediate_qw_count;

		memcpy((void*)payload->rendezvous.contiguous.immediate_block,
			(const void *)sbuf_qw, immediate_block_count * 64); /* immediate_end_block_count */

		opx_shm_tx_advance(&opx_ep->tx->shm, (void*)hdr, pos);

		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
			"===================================== SEND, SHM -- RENDEZVOUS RTS (end) context %p\n",context);

		return FI_SUCCESS;
	}
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		"===================================== SEND, HFI -- RENDEZVOUS RTS (begin) context %p\n",context);

	/*
	 * While the bulk of the payload data will be sent via SDMA once we
	 * get the CTS from the receiver, the initial RTS packet is sent via PIO.
	 */

	union fi_opx_hfi1_pio_state pio_state = *opx_ep->tx->pio_state;

	const uint16_t total_credits_needed =
		1 +				/* packet header */
		payload_blocks_total;		/* packet payload */

	uint64_t total_credits_available = FI_OPX_HFI1_AVAILABLE_CREDITS(pio_state, &opx_ep->tx->force_credit_return, total_credits_needed);
	unsigned loop = 0;
	while (OFI_UNLIKELY(total_credits_available < total_credits_needed)) {
		/*
		 * TODO: Implement PAUSE time-out functionality using time-out configuration
		 * parameter(s).
		 */
		if (loop++ > FI_OPX_HFI1_TX_SEND_RZV_CREDIT_MAX_WAIT) {
			opx_ep->tx->pio_state->qw0 = pio_state.qw0;
			return -FI_EAGAIN;
		}
		fi_opx_compiler_msync_writes();

		FI_OPX_HFI1_UPDATE_CREDITS(pio_state, opx_ep->tx->pio_credits_addr);
		total_credits_available = FI_OPX_HFI1_AVAILABLE_CREDITS(pio_state, &opx_ep->tx->force_credit_return, total_credits_needed);
	}
	if (OFI_UNLIKELY(loop)) {
		opx_ep->tx->pio_state->qw0 = pio_state.qw0;
	}

	struct fi_opx_reliability_tx_replay * replay = NULL;
	if (reliability != OFI_RELIABILITY_KIND_NONE) {	/* compile-time constant expression */
		replay = fi_opx_reliability_client_replay_allocate(&opx_ep->reliability->state, false);
		if(replay == NULL) {
			return -FI_EAGAIN;
		}
	}

	union fi_opx_reliability_tx_psn *psn_ptr = NULL;
	const int64_t psn = (reliability != OFI_RELIABILITY_KIND_NONE) ?	/* compile-time constant expression */
		fi_opx_reliability_tx_next_psn(&opx_ep->ep_fid, &opx_ep->reliability->state,
						addr.uid.lid, dest_rx, addr.reliability_rx, &psn_ptr, 1) :
		0;
	if(OFI_UNLIKELY(psn == -1)) {
		fi_opx_reliability_client_replay_deallocate(&opx_ep->reliability->state, replay);
		return -FI_EAGAIN;
	}

	/*
	 * Write the 'start of packet' (hw+sw header) 'send control block'
	 * which will consume a single pio credit.
	 */

	volatile uint64_t * const scb =
		FI_OPX_HFI1_PIO_SCB_HEAD(opx_ep->tx->pio_scb_sop_first, pio_state);

	uint64_t tmp[8];

	fi_opx_set_scb(scb, tmp,
				   opx_ep->tx->rzv.qw0 | pbc_dws | ((opx_ep->tx->force_credit_return & FI_OPX_HFI1_PBC_CR_MASK) << FI_OPX_HFI1_PBC_CR_SHIFT),
				   opx_ep->tx->rzv.hdr.qw[0] | lrh_dlid | ((uint64_t)lrh_dws << 32),

				   opx_ep->tx->rzv.hdr.qw[1] | bth_rx |
				   ((caps & FI_MSG) ?
					(uint64_t)FI_OPX_HFI_BTH_OPCODE_MSG_RZV_RTS :
					(uint64_t)FI_OPX_HFI_BTH_OPCODE_TAG_RZV_RTS),

				   opx_ep->tx->rzv.hdr.qw[2] | psn,
				   opx_ep->tx->rzv.hdr.qw[3] | (((uint64_t)data) << 32),
				   opx_ep->tx->rzv.hdr.qw[4] | (1ull << 48),
				   len, tag);

	/* consume one credit for the packet header */
	FI_OPX_HFI1_CONSUME_SINGLE_CREDIT(pio_state);
#ifndef NDEBUG
	unsigned credits_consumed = 1;
#endif

	FI_OPX_HFI1_CLEAR_CREDIT_RETURN(opx_ep);

	if (reliability != OFI_RELIABILITY_KIND_NONE) {	/* compile-time constant expression */
		replay->scb.qw0 = tmp[0];
		replay->scb.hdr.qw[0] = tmp[1];
		replay->scb.hdr.qw[1] = tmp[2];
		replay->scb.hdr.qw[2] = tmp[3];
		replay->scb.hdr.qw[3] = tmp[4];
		replay->scb.hdr.qw[4] = tmp[5];
		replay->scb.hdr.qw[5] = tmp[6];
		replay->scb.hdr.qw[6] = tmp[7];
	}

	/*
	 * write the rendezvous payload "send control blocks"
	 */

	volatile uint64_t * scb_payload = (uint64_t *)FI_OPX_HFI1_PIO_SCB_HEAD(opx_ep->tx->pio_scb_first, pio_state);

	fi_opx_set_scb(scb_payload, tmp,
		       (uintptr_t)buf + immediate_total,	/* src_vaddr */
		       (len - immediate_total) >> 6,		/* src_blocks */
		       immediate_byte_count,
		       immediate_qw_count,
		       immediate_block_count,
		       origin_byte_counter_vaddr,
		       immediate_end_block_count,
		       0 /* unused */);

	/* consume one credit for the rendezvous payload metadata */
	FI_OPX_HFI1_CONSUME_SINGLE_CREDIT(pio_state);
#ifndef NDEBUG
	++credits_consumed;
#endif

	uint64_t * replay_payload =
		(reliability != OFI_RELIABILITY_KIND_NONE) ?	/* compile-time constant expression */
		replay->payload : NULL;

	if (reliability != OFI_RELIABILITY_KIND_NONE) {	/* compile-time constant expression */
		assert(!replay->use_iov);
		assert(((uint8_t *)replay_payload) == ((uint8_t *)&replay->data));
		fi_opx_copy_scb(replay_payload, tmp);
		replay_payload += 8;
	}

	scb_payload = (uint64_t *)FI_OPX_HFI1_PIO_SCB_HEAD(opx_ep->tx->pio_scb_first, pio_state);

	/* immediate_byte and immediate_qw are "packed" in the current implementation             */
	/* meaning the immediate bytes are filled, then followed by the rest of the data directly */
	/* adjacent to the packed bytes.  It's probably more efficient to leave a pad and not go  */
	/* through the confusion of finding these boundaries on both sides of the rendezvous      */
	/* That is, just pack the immediate bytes, then pack the "rest" in the immediate qws      */
	/* This would lead to more efficient packing on both sides at the expense of              */
	/* wasting space of a common 0 byte immediate                                             */
	/* tmp_payload_t represents the second cache line of the rts packet                       */
	/* fi_opx_hfi1_packet_payload -> rendezvous -> contiguous                               */
	struct tmp_payload_t {
		uint8_t		immediate_byte[8];
		uint64_t	immediate_qw[7];
	} __attribute__((packed));

	struct tmp_payload_t *tmp_payload = (void*)tmp;
	uint8_t *sbuf = (uint8_t *)buf;
	if (immediate_byte_count > 0) {
		memcpy((void*)tmp_payload->immediate_byte, (const void*)sbuf, immediate_byte_count);
		sbuf += immediate_byte_count;
	}

	uint64_t * sbuf_qw = (uint64_t *)sbuf;
	int i=0;
	for (i=0; i<immediate_qw_count; ++i) {
		tmp_payload->immediate_qw[i] = sbuf_qw[i];
	}
	fi_opx_copy_scb(scb_payload, tmp);
	sbuf_qw += immediate_qw_count;

	if (reliability != OFI_RELIABILITY_KIND_NONE) {	/* compile-time constant expression */
		fi_opx_copy_scb(replay_payload, tmp);
		replay_payload += 8;
	}

	/* consume one credit for the rendezvous payload immediate data */
	FI_OPX_HFI1_CONSUME_SINGLE_CREDIT(pio_state);
#ifndef NDEBUG
	++credits_consumed;
#endif

	if(immediate_block_count) {
#ifndef NDEBUG
		/* assert immediate_block_count can be used for both
		 * full_block_credits_needed and total_credits_available parameters
		 * on the call
		 */
		assert((credits_consumed + immediate_block_count) <= total_credits_needed);
		ssize_t credits =
#endif
			fi_opx_hfi1_tx_egr_write_full_payload_blocks(opx_ep,
								     &pio_state,
								     sbuf_qw,
								     immediate_block_count,
								     immediate_block_count);
		if (reliability != OFI_RELIABILITY_KIND_NONE) {
			assert(immediate_block_count == 64); /* always 4k or 0 */
			memcpy(replay_payload, sbuf_qw, 4096);
			/* replay_payload is pointer to uint64_t */
			replay_payload += 512; /* 4096/sizeof(uint64_t) */
		}


#ifndef NDEBUG
		assert(credits == immediate_block_count);
		credits_consumed+= (unsigned) credits;
#endif

	}
	if(immediate_end_block_count) {
		char* sbuf_end = (char *)buf + len - (immediate_end_block_count << 6);
		FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,"buf %p, buf end %p, sbuf end block %p\n",(char *)buf, (char *)buf+len, sbuf_end);
		union {
			uint8_t		immediate_byte[64];
			uint64_t	immediate_qw[8];
		} align_tmp;
		assert(immediate_end_block_count == 1);
		memcpy(align_tmp.immediate_byte, sbuf_end, (immediate_end_block_count << 6));
		scb_payload = (uint64_t *)FI_OPX_HFI1_PIO_SCB_HEAD(opx_ep->tx->pio_scb_first, pio_state);
		fi_opx_copy_scb(scb_payload, align_tmp.immediate_qw);

		if (reliability != OFI_RELIABILITY_KIND_NONE) { /* compile-time constant expression */
			fi_opx_copy_scb(replay_payload, align_tmp.immediate_qw);
			replay_payload += 8;
		}

		FI_OPX_HFI1_CONSUME_SINGLE_CREDIT(pio_state);
#ifndef NDEBUG
		++credits_consumed;
#endif
	}

	if (reliability != OFI_RELIABILITY_KIND_NONE) {	/* compile-time constant expression */
		fi_opx_reliability_client_replay_register_no_update(&opx_ep->reliability->state,
								    addr.uid.lid, addr.reliability_rx,
								    dest_rx, psn_ptr, replay, reliability);
	}

	FI_OPX_HFI1_CHECK_CREDITS_FOR_ERROR(opx_ep->tx->pio_credits_addr);
#ifndef NDEBUG
	assert(credits_consumed == total_credits_needed);
#endif

	/* update the hfi txe state */
	opx_ep->tx->pio_state->qw0 = pio_state.qw0;

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_EP_DATA,
		"===================================== SEND, HFI -- RENDEZVOUS RTS (end) context %p\n",context);

	return FI_SUCCESS;
}


unsigned fi_opx_hfi1_handle_poll_error(struct fi_opx_ep * opx_ep,
					volatile uint32_t * rhf_ptr,
					const uint32_t rhf_msb,
					const uint32_t rhf_lsb,
					const uint32_t rhf_seq,
					const uint64_t hdrq_offset,
					const uint32_t hdrq_offset_notifyhw)
{
#define HFI1_RHF_ICRCERR (0x80000000u)
#define HFI1_RHF_ECCERR (0x20000000u)
#define HFI1_RHF_TIDERR (0x08000000u)
#define HFI1_RHF_DCERR (0x00800000u)
#define HFI1_RHF_DCUNCERR (0x00400000u)
	(void)rhf_ptr;  /* unused unless debug is turned on */
	if ((rhf_msb & (HFI1_RHF_ICRCERR | HFI1_RHF_ECCERR | HFI1_RHF_TIDERR | HFI1_RHF_DCERR |
			HFI1_RHF_DCUNCERR)) != 0) {
		/* drop this packet and allow reliability protocol to retry */
		if (rhf_seq == (rhf_lsb & 0xF0000000u)) {
#ifdef OPX_RELIABILITY_DEBUG
			const uint64_t hdrq_offset_dws = (rhf_msb >> 12) & 0x01FFu;

			uint32_t *pkt = (uint32_t *)rhf_ptr -
					32 + /* header queue entry size in dw */
					2 + /* rhf field size in dw */
					hdrq_offset_dws;

			const union fi_opx_hfi1_packet_hdr *const hdr =
				(union fi_opx_hfi1_packet_hdr *)pkt;

			fprintf(stderr,
				"%s:%s():%d drop this packet and allow reliability protocol to retry, psn = %u\n",
				__FILE__, __func__, __LINE__, FI_OPX_HFI1_PACKET_PSN(hdr));
#endif
			if ((rhf_lsb & 0x00008000u) == 0x00008000u) {
				/* "consume" this egrq element */
				const uint32_t egrbfr_index =
					(rhf_lsb >> FI_OPX_HFI1_RHF_EGRBFR_INDEX_SHIFT) &
					FI_OPX_HFI1_RHF_EGRBFR_INDEX_MASK;
				const uint32_t last_egrbfr_index =
					opx_ep->rx->egrq.last_egrbfr_index;
				if (OFI_UNLIKELY(last_egrbfr_index != egrbfr_index)) {
					*opx_ep->rx->egrq.head_register = last_egrbfr_index;
					opx_ep->rx->egrq.last_egrbfr_index = egrbfr_index;
				}
			}

			/* "consume" this hdrq element */
			opx_ep->rx->state.hdrq.rhf_seq = (rhf_seq < 0xD0000000u) * rhf_seq + 0x10000000u;
			opx_ep->rx->state.hdrq.head = hdrq_offset +	32;

			fi_opx_hfi1_update_hdrq_head_register(opx_ep, hdrq_offset, hdrq_offset_notifyhw);

		}
		/*
		 * The "else" case, where rhf_seq != (rhf_lsb & 0xF0000000u) indicates
		 * the WFR is dropping headers. We just ignore this and let
		 * reliability re-send the packet.
		 *
		 * TODO: Can we send a NACK in this case?
		 */
		return 1;
	}

	FI_WARN(&fi_opx_provider, FI_LOG_FABRIC, "RECEIVE ERROR: rhf_msb = 0x%08x, rhf_lsb = 0x%08x, rhf_seq = 0x%1x\n", rhf_msb, rhf_lsb, rhf_seq);

	abort();
}
