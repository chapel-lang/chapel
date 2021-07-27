/*

  This file is provided under a dual BSD/GPLv2 license.  When using or
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2015 Intel Corporation.

  This program is free software; you can redistribute it and/or modify
  it under the terms of version 2 of the GNU General Public License as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  Contact Information:
  Intel Corporation, www.intel.com

  BSD LICENSE

  Copyright(c) 2015 Intel Corporation.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in
      the documentation and/or other materials provided with the
      distribution.
    * Neither the name of Intel Corporation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

/* Copyright (c) 2003-2015 Intel Corporation. All rights reserved. */

#include <sys/types.h>
#include <sys/stat.h>
#include "psm_user.h"
#include "psm2_hal.h"

static int psmi_get_hfi_selection_algorithm(void);

psm2_error_t psmi_context_interrupt_set(psmi_context_t *context, int enable)
{
	int poll_type;
	int ret;

	if (!enable == !psmi_hal_has_sw_status(PSM_HAL_PSMI_RUNTIME_INTR_ENABLED))
		return PSM2_OK;

	if (enable)
		poll_type = PSMI_HAL_POLL_TYPE_URGENT;
	else
		poll_type = 0;

	// we need the ep->verbs_ep and no way to get from psm_hw_ctxt to
	// the ep.  So we need a new function instead of just changing a HAL func
	// if verbs_ep was the psm_hw_ctxt for UD HAL, this would not be necessary
	ret = __psm2_ep_poll_type(poll_type, context->ep);

	if (ret != 0)
		return PSM2_EP_NO_RESOURCES;
	else {
		if (enable)
			psmi_hal_add_sw_status(PSM_HAL_PSMI_RUNTIME_INTR_ENABLED);
		else
			psmi_hal_sub_sw_status(PSM_HAL_PSMI_RUNTIME_INTR_ENABLED);
		return PSM2_OK;
	}
}

int psmi_context_interrupt_isenabled(psmi_context_t *context)
{
	return psmi_hal_has_sw_status(PSM_HAL_PSMI_RUNTIME_INTR_ENABLED);
}


/* returns the 8-bit hash value of an uuid. */
static inline
uint8_t
psmi_get_uuid_hash(psm2_uuid_t const uuid)
{
	int i;
	uint8_t hashed_uuid = 0;

	for (i=0; i < sizeof(psm2_uuid_t); ++i)
		hashed_uuid ^= *((uint8_t const *)uuid + i);

	return hashed_uuid;
}

int psmi_get_current_proc_location()
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

static void
psmi_spread_hfi_selection(psm2_uuid_t const job_key, long *unit_start,
			     long *unit_end, int nunits)
{
	{
		/* else, we are going to look at:
		   (a hash of the job key plus the local rank id) mod nunits. */

		*unit_start = ((hfi_get_mylocalrank()+1) +
			psmi_get_uuid_hash(job_key)) % nunits;
		if (*unit_start > 0)
			*unit_end = *unit_start - 1;
		else
			*unit_end = nunits-1;
	}
}

static int
psmi_create_and_open_affinity_shm(psm2_uuid_t const job_key)
{
	int shm_fd, ret;
	int first_to_create = 0;
	size_t shm_name_len = 256;
	shared_affinity_ptr = NULL;
	affinity_shm_name = NULL;
	affinity_shm_name = (char *) psmi_malloc(PSMI_EP_NONE, UNDEFINED, shm_name_len);

	psmi_assert_always(affinity_shm_name != NULL);
	snprintf(affinity_shm_name, shm_name_len,
		 AFFINITY_SHM_BASENAME".%d",
		 psmi_get_uuid_hash(job_key));
	shm_fd = shm_open(affinity_shm_name, O_RDWR | O_CREAT | O_EXCL,
			  S_IRUSR | S_IWUSR);
	if ((shm_fd < 0) && (errno == EEXIST)) {
		shm_fd = shm_open(affinity_shm_name, O_RDWR, S_IRUSR | S_IWUSR);
		if (shm_fd < 0) {
			_HFI_VDBG("Cannot open affinity shared mem fd:%s, errno=%d\n",
				  affinity_shm_name, errno);
			return shm_fd;
		}
	} else if (shm_fd > 0) {
		first_to_create = 1;
	} else {
		_HFI_VDBG("Cannot create affinity shared mem fd:%s, errno=%d\n",
			  affinity_shm_name, errno);
	}

	ret = ftruncate(shm_fd, AFFINITY_SHMEMSIZE);
	if ( ret < 0 ) {
		_HFI_VDBG("Cannot truncate affinity shared mem fd:%s, errno=%d\n",
			affinity_shm_name, errno);
		if (shm_fd >= 0) close(shm_fd);
		return ret;
	}

	shared_affinity_ptr = (uint64_t *) mmap(NULL, AFFINITY_SHMEMSIZE, PROT_READ | PROT_WRITE,
					MAP_SHARED, shm_fd, 0);
	if (shared_affinity_ptr == MAP_FAILED) {
		_HFI_VDBG("Cannot mmap affinity shared memory. errno=%d\n",
			  errno);
		close(shm_fd);
		return -1;
	}
	close(shm_fd);

	psmi_affinity_shared_file_opened = 1;

	if (first_to_create) {
		_HFI_VDBG("Creating shm to store NIC affinity per socket\n");

		memset(shared_affinity_ptr, 0, AFFINITY_SHMEMSIZE);

		/*
		 * Once shm object is initialized, unlock others to be able to
		 * use it.
		 */
		psmi_sem_post(sem_affinity_shm_rw, sem_affinity_shm_rw_name);
	} else {
		_HFI_VDBG("Opening shm object to read/write NIC affinity per socket\n");
	}

	/*
	 * Start critical section to increment reference count when creating
	 * or opening shm object. Decrement of ref count will be done before
	 * closing the shm.
	 */
	if (psmi_sem_timedwait(sem_affinity_shm_rw, sem_affinity_shm_rw_name)) {
		_HFI_VDBG("Could not enter critical section to update shm refcount\n");
		return -1;
	}

	shared_affinity_ptr[AFFINITY_SHM_REF_COUNT_LOCATION] += 1;

	/* End critical section */
	psmi_sem_post(sem_affinity_shm_rw, sem_affinity_shm_rw_name);

	return 0;
}

/*
 * Spread HFI selection between units if we find more than one within a socket.
 */
static void
psmi_spread_hfi_within_socket(long *unit_start, long *unit_end, int node_id,
			      int *saved_hfis, int found, psm2_uuid_t const job_key)
{
	int ret, shm_location;

	/*
	 * Take affinity lock and open shared memory region to be able to
	 * accurately determine which HFI to pick for this process. If any
	 * issues, bail by picking first known HFI.
	 */
	if (!psmi_affinity_semaphore_open)
		goto spread_hfi_fallback;

	ret = psmi_create_and_open_affinity_shm(job_key);
	if (ret < 0)
		goto spread_hfi_fallback;

	shm_location = AFFINITY_SHM_HFI_INDEX_LOCATION + node_id;
	if (shm_location > AFFINITY_SHMEMSIZE)
		goto spread_hfi_fallback;

	/* Start critical section to read/write shm object */
	if (psmi_sem_timedwait(sem_affinity_shm_rw, sem_affinity_shm_rw_name)) {
		_HFI_VDBG("Could not enter critical section to update NIC index\n");
		goto spread_hfi_fallback;
	}

	*unit_start = *unit_end = saved_hfis[shared_affinity_ptr[shm_location]];
	shared_affinity_ptr[shm_location] =
		(shared_affinity_ptr[shm_location] + 1) % found;
	_HFI_VDBG("Selected NIC index= %ld, Next NIC=%ld, node = %d, local rank=%d, found=%d.\n",
		  *unit_start, shared_affinity_ptr[shm_location], node_id,
		  hfi_get_mylocalrank(), found);

	/* End Critical Section */
	psmi_sem_post(sem_affinity_shm_rw, sem_affinity_shm_rw_name);

	return;

spread_hfi_fallback:
	*unit_start = *unit_end = saved_hfis[0];
}

static void
psmi_create_affinity_semaphores(psm2_uuid_t const job_key)
{
	int ret;
	sem_affinity_shm_rw_name = NULL;
	size_t sem_len = 256;

	/*
	 * If already opened, no need to do anything else.
	 * This could be true for Multi-EP cases where a different thread has
	 * already created the semaphores. We don't need separate locks here as
	 * we are protected by the overall "psmi_creation_lock" which each
	 * thread will take in psm2_ep_open()
	 */
	if (psmi_affinity_semaphore_open)
		return;

	sem_affinity_shm_rw_name = (char *) psmi_malloc(PSMI_EP_NONE, UNDEFINED, sem_len);
	psmi_assert_always(sem_affinity_shm_rw_name != NULL);
	snprintf(sem_affinity_shm_rw_name, sem_len,
		 SEM_AFFINITY_SHM_RW_BASENAME".%d",
		 psmi_get_uuid_hash(job_key));

	ret = psmi_init_semaphore(&sem_affinity_shm_rw, sem_affinity_shm_rw_name,
				  S_IRUSR | S_IWUSR, 0);
	if (ret) {
		_HFI_VDBG("Cannot initialize semaphore: %s for read-write access to shm object.\n",
			  sem_affinity_shm_rw_name);
		sem_close(sem_affinity_shm_rw);
		psmi_free(sem_affinity_shm_rw_name);
		sem_affinity_shm_rw_name = NULL;
		return;
	}

	_HFI_VDBG("Semaphore: %s created for read-write access to shm object.\n",
		  sem_affinity_shm_rw_name);

	psmi_affinity_semaphore_open = 1;

	return;
}

// return set of units to consider and which to start at.
// caller will use 1st active unit which can be opened.
// caller will wrap around so it's valid for start > end
static
psm2_error_t
psmi_compute_start_and_end_unit(long unit_param,int nunitsactive,int nunits,
				psm2_uuid_t const job_key,
				long *unit_start,long *unit_end)
{
	unsigned short hfi_sel_alg = PSMI_UNIT_SEL_ALG_ACROSS;
	int node_id, unit_id, found = 0;
	int saved_hfis[nunits];

	/* if the user did not set PSM3_NIC then ... */
	if (unit_param == PSM3_NIC_ANY)
	{
		if (nunitsactive > 1) {
			// if NICs are on different subnets, and ! allow_routers
			// we need to have all ranks default to the same subnet
			// so force 1st active NIC in that case
			uint64_t subnet;
			int have_subnet = 0;
			int have_eth = 0;
			for (unit_id = 0; unit_id < nunits; unit_id++) {
				uint64_t gid_hi, hi;
				int is_eth = 0;
				if (psmi_hal_get_unit_active(unit_id) <= 0)
					continue;
				if (0 != psmi_hal_get_port_subnet(unit_id, 1 /* VERBS_PORT*/,
								&gid_hi, NULL, NULL, NULL, NULL, &hi, NULL))
					continue; // can't access NIC
				is_eth = (gid_hi != hi);
				if (! have_subnet) {
					subnet = gid_hi;
					have_subnet = 1;
					have_eth = is_eth;
				} else if (have_eth != is_eth
					   || (subnet != gid_hi
						 && (! is_eth || ! psmi_allow_routers))) {
					// active units have different subnets
					// caller will pick 1st active unit
					*unit_start = 0;
					*unit_end = nunits - 1;
					return PSM2_OK;
				}
			}
		}

		/* Get the actual selection algorithm from the environment: */
		hfi_sel_alg = psmi_get_hfi_selection_algorithm();
		/* If round-robin is selection algorithm and ... */
		if ((hfi_sel_alg == PSMI_UNIT_SEL_ALG_ACROSS) &&
		    /* there are more than 1 active units then ... */
		    (nunitsactive > 1))
		{
			/*
			 * Pick first HFI we find on same root complex
			 * as current task. If none found, fall back to
			 * load-balancing algorithm.
			 */
			node_id = psmi_get_current_proc_location();
			if (node_id >= 0) {
				for (unit_id = 0; unit_id < nunits; unit_id++) {
					if (psmi_hal_get_unit_active(unit_id) <= 0)
						continue;

					int node_id_i;

					if (!psmi_hal_get_node_id(unit_id, &node_id_i)) {
						if (node_id_i == node_id) {
							saved_hfis[found] = unit_id;
							found++;
						}
					}
				}

				if (found > 1) {
					psmi_create_affinity_semaphores(job_key);
					psmi_spread_hfi_within_socket(unit_start, unit_end,
								      node_id, saved_hfis,
								      found, job_key);
				} else if (found == 1) {
					*unit_start = *unit_end = saved_hfis[0];
				}
			}

			if (node_id < 0 || !found) {
				psmi_spread_hfi_selection(job_key, unit_start,
							  unit_end, nunits);
			}
		} else if ((hfi_sel_alg == PSMI_UNIT_SEL_ALG_ACROSS_ALL) &&
			 (nunitsactive > 1)) {
				psmi_spread_hfi_selection(job_key, unit_start,
							  unit_end, nunits);
		}
		else { // PSMI_UNIT_SEL_ALG_WITHIN or only 1 active unit
			// caller will pick 1st active unit
			*unit_start = 0;
			*unit_end = nunits - 1;
		}
	} else if (unit_param >= 0) {
		/* the user specified PSM3_NIC, we use it. */
		*unit_start = *unit_end = unit_param;
	} else {
		psmi_handle_error(NULL, PSM2_EP_DEVICE_FAILURE,
				 "PSM3 can't open unit: %ld for reading and writing",
				 unit_param);
		return PSM2_EP_DEVICE_FAILURE;
	}

	return PSM2_OK;
}

psm2_error_t
psmi_context_open(const psm2_ep_t ep, long unit_param, long port,
		  psm2_uuid_t const job_key, int64_t timeout_ns,
		  psmi_context_t *context)
{
	long open_timeout = 0, unit_start, unit_end, unit_id, unit_id_prev;
	psm2_error_t err = PSM2_OK;
	int nunits = psmi_hal_get_num_units(), nunitsactive=0;

	/*
	 * If shared contexts are enabled, try our best to schedule processes
	 * across one or many devices
	 */

	/* if no units, then no joy. */
	if (nunits <= 0)
	{
		err = psmi_handle_error(NULL, PSM2_EP_DEVICE_FAILURE,
					"PSM3 no nic units are available");
		goto ret;
	}

	/* Calculate the number of active units: */
	for (unit_id=0;unit_id < nunits;unit_id++)
	{
		if (psmi_hal_get_unit_active(unit_id) > 0)
			nunitsactive++;
	}
	/* if no active units, then no joy. */
	if (nunitsactive == 0)
	{
		err = psmi_handle_error(NULL, PSM2_EP_DEVICE_FAILURE,
					"PSM3 no nic units are active");
		goto ret;
	}
	if (timeout_ns > 0)
		open_timeout = (long)(timeout_ns / MSEC_ULL);


	unit_start = 0; unit_end = nunits - 1;
	err = psmi_compute_start_and_end_unit(unit_param, nunitsactive,
					      nunits, job_key,
					      &unit_start, &unit_end);
	if (err != PSM2_OK)
		return err;

	/* this is the start of a loop that starts at unit_start and goes to unit_end.
	   but note that the way the loop computes the loop control variable is by
	   an expression involving the mod operator. */
	int success = 0;
	unit_id_prev = unit_id = unit_start;
	do
	{
		/* close previous opened unit fd before attempting open of current unit. */
		if (context->psm_hw_ctxt) {
			psmi_hal_close_context(&context->psm_hw_ctxt);
			context->psm_hw_ctxt = 0;
		}

		/* if the unit_id is not active, go to next one. */
		if (psmi_hal_get_unit_active(unit_id) <= 0) {
			unit_id_prev = unit_id;
			unit_id = (unit_id + 1) % nunits;
			continue;
		}

		/* open this unit. */
		int rv = psmi_hal_context_open(unit_id, port, open_timeout,
					       ep, job_key, context,
					       psmi_hal_has_sw_status(PSM_HAL_PSMI_RUNTIME_RX_THREAD_STARTED),
					       HAL_CONTEXT_OPEN_RETRY_MAX);

		/* go to next unit if failed to open. */
		if (rv || context->psm_hw_ctxt == NULL) {
			unit_id_prev = unit_id;
			unit_id = (unit_id + 1) % nunits;
			continue;
		}

		success = 1;
		break;

	} while (unit_id_prev != unit_end);

	if (!success)
	{
		err = psmi_handle_error(NULL, PSM2_EP_DEVICE_FAILURE,
					"PSM3 can't open nic unit: %ld",unit_param);
		goto bail;
	}

	context->ep = (psm2_ep_t) ep;

	/* Check backward compatibility bits here and save the info */
	if (psmi_hal_has_cap(PSM_HAL_CAP_GPUDIRECT_OT))
	{
#ifdef PSM_CUDA
		is_driver_gpudirect_enabled = 1;
#else
		psmi_handle_error(PSMI_EP_NORETURN, PSM2_INTERNAL_ERR, "FATAL ERROR: "
				  "CUDA version of rendezvous driver is loaded with non-CUDA version of "
				  "psm3 provider.\n");
#endif
	}
#ifdef PSM_CUDA
	else
		fprintf(stderr,"WARNING: running CUDA version of psm3 provider with non CUDA version of rendezvous driver.\n");
#endif
	_HFI_VDBG("hal_context_open() passed.\n");

	/* Construct epid for this Endpoint */
	psmi_assert_always(PSMI_EPID_VERSION == PSMI_EPID_V3
						|| PSMI_EPID_VERSION == PSMI_EPID_V4);
	psmi_assert_always (ep->verbs_ep.context);
	// TBD - if we put the verbs_ep in hw_ctxt we could push this to HAL
	// verbs_ep_open has initialized: ep->unit_id, ep->portnum,
	//	ep->gid_hi, ep->gid_lo
	if (ep->verbs_ep.link_layer == IBV_LINK_LAYER_ETHERNET) {
		char buf[INET_ADDRSTRLEN];
		int netmask_bits = psmi_count_high_bits(ep->verbs_ep.ip_netmask);
		if (netmask_bits < 0) {
			err = psmi_handle_error(NULL, PSM2_EP_DEVICE_FAILURE,
					"PSM3 invalid netmask: %s",
					psmi_ipv4_ntop(ep->verbs_ep.ip_netmask, buf, sizeof(buf)));
			goto bail;
		}
		psmi_epid_ver = PSMI_EPID_V4;	// overide default based on device
		context->epid = PSMI_EPID_PACK_V4(ep->verbs_ep.ip_addr,
							ep->verbs_ep.qp->qp_num, netmask_bits);
		_HFI_VDBG("construct epid v4: 0x%"PRIx64" ip %s subnet_bits %u qp %d mtu %d\n",
						context->epid,
						psmi_ipv4_ntop(ep->verbs_ep.ip_addr, buf, sizeof(buf)),
						netmask_bits, ep->verbs_ep.qp->qp_num, ep->mtu);
	} else {
		unsigned subnet = ep->gid_hi & 0xffff;
		psmi_epid_ver = PSMI_EPID_V3;	// overide default based on device
		context->epid = PSMI_EPID_PACK_V3(ep->verbs_ep.port_attr.lid,
							ep->verbs_ep.qp->qp_num,
							subnet /*ep->gid_hi*/);
		_HFI_VDBG("construct epid v3: 0x%"PRIx64" lid %d qp %d subnet 0x%x mtu %d\n",
						context->epid, ep->verbs_ep.port_attr.lid,
						ep->verbs_ep.qp->qp_num, subnet, ep->mtu);
	}

	goto ret;

bail:
	_HFI_PRDBG("open failed: unit_id: %ld, err: %d (%s)\n", unit_id, err, strerror(errno));
	if (context->psm_hw_ctxt) {
		psmi_hal_close_context(&context->psm_hw_ctxt);
		context->psm_hw_ctxt = 0;
	}
ret:

	_HFI_VDBG("psmi_context_open() return %d\n", err);
	return err;
}

psm2_error_t psmi_context_close(psmi_context_t *context)
{
	if (context->psm_hw_ctxt) {
		psmi_hal_close_context(&context->psm_hw_ctxt);
		context->psm_hw_ctxt = 0;
	}

	return PSM2_OK;
}

/*
 * This function works whether a context is initialized or not in a psm2_ep.
 *
 * Returns one of
 *
 * PSM2_OK: Port status is ok (or context not initialized yet but still "ok")
 * PSM2_OK_NO_PROGRESS: Cable pulled
 * PSM2_EP_NO_NETWORK: No network, no lid, ...
 * PSM2_EP_DEVICE_FAILURE: Chip failures, rxe/txe parity, etc.
 * The message follows the per-port status
 * As of 7322-ready driver, need to check port-specific qword for IB
 * as well as older unit-only.  For now, we don't have the port interface
 * defined, so just check port 0 qword for spi_status
 */
psm2_error_t psmi_context_check_status(const psmi_context_t *contexti)
{
	psm2_error_t err = PSM2_OK;
	return err;
}

static
int psmi_get_hfi_selection_algorithm(void)
{
	union psmi_envvar_val env_hfi1_alg;
	int hfi1_alg = PSMI_UNIT_SEL_ALG_ACROSS;

	/* If a specific unit is set in the environment, use that one. */
	psmi_getenv("PSM3_NIC_SELECTION_ALG",
		    "NIC Device Selection Algorithm to use. Round Robin[RoundRobin or rr] (Default) "
		    ", Packed[p] or Round Robin All[RoundRobinAll or rra].",
		    PSMI_ENVVAR_LEVEL_USER, PSMI_ENVVAR_TYPE_STR,
		    (union psmi_envvar_val)"rr", &env_hfi1_alg);

	if (!strcasecmp(env_hfi1_alg.e_str, "Round Robin")
		|| !strcasecmp(env_hfi1_alg.e_str, "RoundRobin")
		|| !strcasecmp(env_hfi1_alg.e_str, "rr"))
		hfi1_alg = PSMI_UNIT_SEL_ALG_ACROSS;
	else if (!strcasecmp(env_hfi1_alg.e_str, "Packed")
			 || !strcasecmp(env_hfi1_alg.e_str, "p"))
		hfi1_alg = PSMI_UNIT_SEL_ALG_WITHIN;
	else if (!strcasecmp(env_hfi1_alg.e_str, "Round Robin All")
			 || !strcasecmp(env_hfi1_alg.e_str, "RoundRobinAll")
			 || !strcasecmp(env_hfi1_alg.e_str, "rra"))
		hfi1_alg = PSMI_UNIT_SEL_ALG_ACROSS_ALL;
	else {
		_HFI_ERROR
		    ("Unknown NIC selection algorithm %s. Defaulting to Round Robin "
		     "allocation of NICs.\n", env_hfi1_alg.e_str);
		hfi1_alg = PSMI_UNIT_SEL_ALG_ACROSS;
	}

	return hfi1_alg;
}
