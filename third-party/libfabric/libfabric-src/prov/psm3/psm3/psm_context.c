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

static int psmi_parse_nic_selection_algorithm(void);
static psm2_error_t
psm3_ep_verify_pkey(psm2_ep_t ep, uint16_t pkey, uint16_t *opkey, uint16_t* oindex);

psm2_error_t psm3_context_interrupt_set(psm2_ep_t ep, int enable)
{
	int poll_type;
	int ret;

	if (!enable == !psmi_hal_has_sw_status(PSM_HAL_PSMI_RUNTIME_INTR_ENABLED))
		return PSM2_OK;

	if (enable)
		poll_type = PSMI_HAL_POLL_TYPE_URGENT;
	else
		poll_type = 0;

	ret = psmi_hal_poll_type(poll_type, ep);

	if (ret != 0) return PSM2_EP_NO_RESOURCES;

	if (enable)
		psmi_hal_add_sw_status(PSM_HAL_PSMI_RUNTIME_INTR_ENABLED);
	else
		psmi_hal_sub_sw_status(PSM_HAL_PSMI_RUNTIME_INTR_ENABLED);
	return PSM2_OK;
}

int psm3_context_interrupt_isenabled(psm2_ep_t ep)
{
	return psmi_hal_has_sw_status(PSM_HAL_PSMI_RUNTIME_INTR_ENABLED);
}


/* returns the 8-bit hash value of an uuid. */
static inline
uint8_t
psm3_get_uuid_hash(psm2_uuid_t const uuid)
{
	int i;
	uint8_t hashed_uuid = 0;

	for (i=0; i < sizeof(psm2_uuid_t); ++i)
		hashed_uuid ^= *((uint8_t const *)uuid + i);

	return hashed_uuid;
}

int psm3_get_current_proc_location()
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

/* search the list of all units for those which are active
 * and optionally match the given NUMA node_id (when node_id >= 0)
 * returns the number of active units found.
 * Note get_unit_active tests for active ports, valid addresses and
 * performs filtering as done in get_port_subnets
 */
static int
hfi_find_active_hfis(int nunits, int node_id, int *saved_hfis)
{
	int found = 0, unit_id;

	for (unit_id = 0; unit_id < nunits; unit_id++) {
		int node_id_i;

		if (psmi_hal_get_unit_active(unit_id) <= 0)
			continue;

		if (node_id < 0) {
			saved_hfis[found++] = unit_id;
			_HFI_DBG("RoundRobinAll Found NIC unit= %d, local rank=%d.\n",
				unit_id, psm3_get_mylocalrank());
		} else if (!psmi_hal_get_node_id(unit_id, &node_id_i)
				&& node_id_i == node_id) {
			saved_hfis[found++] = unit_id;
			_HFI_DBG("RoundRobin Found NIC unit= %d, node = %d, local rank=%d.\n",
				unit_id, node_id, psm3_get_mylocalrank());
		}
	}
	return found;
}

static void
psmi_spread_nic_selection(psm2_uuid_t const job_key, long *unit_start,
			     long *unit_end, int nunits)
{
	{
		int found, saved_hfis[nunits];

		/* else, we are going to look at:
		   (a hash of the job key plus the local rank id) mod nunits. */
		found = hfi_find_active_hfis(nunits, -1, saved_hfis);
		if (found)
			*unit_start = saved_hfis[((psm3_get_mylocalrank()+1) +
				psm3_get_uuid_hash(job_key)) % found];
		else
			*unit_start = 0; // caller will fail
		/* just in case, caller will check all other units, with wrap */
		if (*unit_start > 0)
			*unit_end = *unit_start - 1;
		else
			*unit_end = nunits-1;
	}
	_HFI_DBG("RoundRobinAll Will select 1st viable NIC unit= %ld to %ld.\n",
		*unit_start, *unit_end);
}

static int
psm3_create_and_open_affinity_shm(psm2_uuid_t const job_key)
{
	int shm_fd, ret;
	int first_to_create = 0;
	size_t shm_name_len = 256;

	psmi_assert_always(psm3_affinity_semaphore_open);
	if (psm3_affinity_shared_file_opened) {
		/* opened and have our reference counted in shm */
		psmi_assert_always(psm3_affinity_shm_name != NULL);
		psmi_assert_always(psm3_shared_affinity_ptr != NULL);
		return 0;
	}

	psm3_shared_affinity_ptr = NULL;
	psm3_affinity_shm_name = (char *) psmi_malloc(PSMI_EP_NONE, UNDEFINED, shm_name_len);

	psmi_assert_always(psm3_affinity_shm_name != NULL);
	snprintf(psm3_affinity_shm_name, shm_name_len,
		 AFFINITY_SHM_BASENAME".%d",
		 psm3_get_uuid_hash(job_key));
	shm_fd = shm_open(psm3_affinity_shm_name, O_RDWR | O_CREAT | O_EXCL,
			  S_IRUSR | S_IWUSR);
	if ((shm_fd < 0) && (errno == EEXIST)) {
		shm_fd = shm_open(psm3_affinity_shm_name, O_RDWR, S_IRUSR | S_IWUSR);
		if (shm_fd < 0) {
			_HFI_VDBG("Cannot open affinity shared mem fd:%s, errno=%d\n",
				  psm3_affinity_shm_name, errno);
			goto free_name;
		}
	} else if (shm_fd >= 0) {
		first_to_create = 1;
	} else {
		_HFI_VDBG("Cannot create affinity shared mem fd:%s, errno=%d\n",
			  psm3_affinity_shm_name, errno);
		goto free_name;
	}

	ret = ftruncate(shm_fd, PSMI_PAGESIZE);
	if ( ret < 0 ) {
		_HFI_VDBG("Cannot truncate affinity shared mem fd:%s, errno=%d\n",
			psm3_affinity_shm_name, errno);
		goto close_shm;
	}

	psm3_shared_affinity_ptr = (uint64_t *) mmap(NULL, PSMI_PAGESIZE, PROT_READ | PROT_WRITE,
					MAP_SHARED, shm_fd, 0);
	if (psm3_shared_affinity_ptr == MAP_FAILED) {
		_HFI_VDBG("Cannot mmap affinity shared memory: %s, errno=%d\n",
			  psm3_affinity_shm_name, errno);
		goto close_shm;
	}
	close(shm_fd);
	shm_fd = -1;

	if (first_to_create) {
		_HFI_VDBG("Initializing shm to store NIC affinity per socket: %s\n", psm3_affinity_shm_name);

		memset(psm3_shared_affinity_ptr, 0, PSMI_PAGESIZE);

		/*
		 * Once shm object is initialized, unlock others to be able to
		 * use it.
		 */
		psmi_sem_post(psm3_sem_affinity_shm_rw, psm3_sem_affinity_shm_rw_name);
	} else {
		_HFI_VDBG("Opened shm object to read/write NIC affinity per socket: %s\n", psm3_affinity_shm_name);
	}

	/*
	 * Start critical section to increment reference count when creating
	 * or opening shm object. Decrement of ref count will be done before
	 * closing the shm.
	 */
	if (psmi_sem_timedwait(psm3_sem_affinity_shm_rw, psm3_sem_affinity_shm_rw_name)) {
		_HFI_VDBG("Could not enter critical section to update shm refcount\n");
		goto unmap_shm;
	}

	psm3_shared_affinity_ptr[AFFINITY_SHM_REF_COUNT_LOCATION] += 1;
	_HFI_VDBG("shm refcount = %"PRId64"\n",  psm3_shared_affinity_ptr[AFFINITY_SHM_REF_COUNT_LOCATION]);

	/* End critical section */
	psmi_sem_post(psm3_sem_affinity_shm_rw, psm3_sem_affinity_shm_rw_name);

	psm3_affinity_shared_file_opened = 1;

	return 0;

unmap_shm:
	munmap(psm3_shared_affinity_ptr, PSMI_PAGESIZE);
	psm3_shared_affinity_ptr = NULL;
close_shm:
	if (shm_fd >= 0) close(shm_fd);
free_name:
	psmi_free(psm3_affinity_shm_name);
	psm3_affinity_shm_name = NULL;
	return -1;
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
	if (!psm3_affinity_semaphore_open)
		goto spread_hfi_fallback;

	ret = psm3_create_and_open_affinity_shm(job_key);
	if (ret < 0)
		goto spread_hfi_fallback;

	shm_location = AFFINITY_SHM_HFI_INDEX_LOCATION + node_id;
	if (shm_location > PSMI_PAGESIZE)
		goto spread_hfi_fallback;

	/* Start critical section to read/write shm object */
	if (psmi_sem_timedwait(psm3_sem_affinity_shm_rw, psm3_sem_affinity_shm_rw_name)) {
		_HFI_VDBG("Could not enter critical section to update NIC index\n");
		goto spread_hfi_fallback;
	}

	*unit_start = *unit_end = saved_hfis[psm3_shared_affinity_ptr[shm_location]];
	psm3_shared_affinity_ptr[shm_location] =
		(psm3_shared_affinity_ptr[shm_location] + 1) % found;
	_HFI_DBG("RoundRobin Selected NIC unit= %ld, Next NIC=%ld, node = %d, local rank=%d, found=%d.\n",
		  *unit_start, psm3_shared_affinity_ptr[shm_location], node_id,
		  psm3_get_mylocalrank(), found);

	/* End Critical Section */
	psmi_sem_post(psm3_sem_affinity_shm_rw, psm3_sem_affinity_shm_rw_name);

	return;

spread_hfi_fallback:
	*unit_start = *unit_end = saved_hfis[0];
}

static void
psm3_create_affinity_semaphores(psm2_uuid_t const job_key)
{
	int ret;
	size_t sem_len = 256;

	/*
	 * If already opened, no need to do anything else.
	 * This could be true for Multi-EP cases where a different thread has
	 * already created the semaphores. We don't need separate locks here as
	 * we are protected by the overall "psm3_creation_lock" which each
	 * thread will take in psm3_ep_open()
	 */
	if (psm3_affinity_semaphore_open)
		return;

	psm3_sem_affinity_shm_rw_name = (char *) psmi_malloc(PSMI_EP_NONE, UNDEFINED, sem_len);
	psmi_assert_always(psm3_sem_affinity_shm_rw_name != NULL);
	snprintf(psm3_sem_affinity_shm_rw_name, sem_len,
		 SEM_AFFINITY_SHM_RW_BASENAME".%d",
		 psm3_get_uuid_hash(job_key));

	ret = psmi_init_semaphore(&psm3_sem_affinity_shm_rw, psm3_sem_affinity_shm_rw_name,
				  S_IRUSR | S_IWUSR, 0);
	if (ret) {
		_HFI_VDBG("Cannot initialize semaphore: %s for read-write access to shm object.\n",
			  psm3_sem_affinity_shm_rw_name);
		if (psm3_sem_affinity_shm_rw)
			sem_close(psm3_sem_affinity_shm_rw);
		psmi_free(psm3_sem_affinity_shm_rw_name);
		psm3_sem_affinity_shm_rw_name = NULL;
		return;
	}

	_HFI_VDBG("Semaphore: %s created for read-write access to shm object.\n",
		  psm3_sem_affinity_shm_rw_name);

	psm3_affinity_semaphore_open = 1;

	return;
}

// return set of units to consider and which to start at.
// caller will use 1st active unit which can be opened.
// caller will wrap around so it's valid for start > end
// Note: When using multiple rails per PSM process, higher level code will
// walk through desired units and unit_param will specify a specific unit
static
psm2_error_t
psmi_compute_start_and_end_unit(long unit_param, long addr_index,
				int nunitsactive,int nunits,
				psm2_uuid_t const job_key,
				long *unit_start,long *unit_end)
{
	unsigned short nic_sel_alg = PSMI_UNIT_SEL_ALG_ACROSS;
	int node_id, found = 0;
	int saved_hfis[nunits];

	/* if the user did not set PSM3_NIC then ... */
	if (unit_param == PSM3_NIC_ANY)
	{
		if (nunitsactive > 1) {
			// if NICs are on different planes (non-routed subnets)
			// we need to have all ranks default to the same plane
			// so force 1st active NIC in that case
			int have_subnet = 0, unit_id;
			psmi_subnet128_t got_subnet = { };
			for (unit_id = 0; unit_id < nunits; unit_id++) {
				psmi_subnet128_t subnet;
				if (psmi_hal_get_unit_active(unit_id) <= 0)
					continue;
				if (0 != psmi_hal_get_port_subnet(unit_id, 1 /* VERBS_PORT*/,
								addr_index>0?addr_index:0,
								&subnet, NULL, NULL, NULL))
					continue; // can't access NIC
				if (! have_subnet) {
					have_subnet = 1;
					got_subnet = subnet;
				} else if (! psm3_subnets_match(got_subnet,
								subnet)) {
					// active units have different tech
					// (IB/OPA vs Eth) or different subnets
					// caller will pick 1st active unit
					*unit_start = 0;
					*unit_end = nunits - 1;
					_HFI_DBG("Multi-Plane config: Will select 1st viable NIC unit= %ld to %ld.\n",
						*unit_start, *unit_end);
					return PSM2_OK;
				}
			}
		}

		/* Get the actual selection algorithm from the environment: */
		nic_sel_alg = psmi_parse_nic_selection_algorithm();
		/* If round-robin is selection algorithm and ... */
		if ((nic_sel_alg == PSMI_UNIT_SEL_ALG_ACROSS) &&
		    /* there are more than 1 active units then ... */
		    (nunitsactive > 1))
		{
			/*
			 * Pick first HFI we find on same root complex
			 * as current task. If none found, fall back to
			 * RoundRobinAll load-balancing algorithm.
			 */
			node_id = psm3_get_current_proc_location();
			if (node_id >= 0) {
				found = hfi_find_active_hfis(nunits, node_id,
								saved_hfis);
				if (found > 1) {
					psm3_create_affinity_semaphores(job_key);
					psmi_spread_hfi_within_socket(unit_start, unit_end,
								      node_id, saved_hfis,
								      found, job_key);
				} else if (found == 1) {
					*unit_start = *unit_end = saved_hfis[0];
					_HFI_DBG("RoundRobin Selected NIC unit= %ld, node = %d, local rank=%d, found=%d.\n",
						*unit_start, node_id,
						psm3_get_mylocalrank(), found);
				}
			}

			if (node_id < 0 || !found) {
				_HFI_DBG("RoundRobin No local NIC found, using RoundRobinAll, node = %d, local rank=%d, found=%d.\n",
						node_id,
						psm3_get_mylocalrank(), found);
				psmi_spread_nic_selection(job_key, unit_start,
							  unit_end, nunits);
			}
		} else if ((nic_sel_alg == PSMI_UNIT_SEL_ALG_ACROSS_ALL) &&
			 (nunitsactive > 1)) {
				psmi_spread_nic_selection(job_key, unit_start,
							  unit_end, nunits);
		}
		else { // PSMI_UNIT_SEL_ALG_WITHIN or only 1 active unit
			// caller will pick 1st active unit
			*unit_start = 0;
			*unit_end = nunits - 1;
			_HFI_DBG("%s: Will select 1st viable NIC unit= %ld to %ld.\n",
				(nic_sel_alg == PSMI_UNIT_SEL_ALG_WITHIN)
					?"Packed":"Only 1 viable NIC",
				*unit_start, *unit_end);
		}
	} else if (unit_param >= 0) {
		/* the user specified PSM3_NIC, we use it. */
		*unit_start = *unit_end = unit_param;
		_HFI_DBG("Caller selected NIC %ld.\n", *unit_start);
	} else {
		psm3_handle_error(NULL, PSM2_EP_DEVICE_FAILURE,
				 "PSM3 can't open unit: %ld for reading and writing",
				 unit_param);
		return PSM2_EP_DEVICE_FAILURE;
	}

	return PSM2_OK;
}

static int psmi_hash_addr_index(long unit, long port, long addr_index)
{
	/* if the user did not set addr_index, then use a hash */
	if (addr_index == PSM3_ADDR_INDEX_ANY) {
		addr_index = (psm3_get_mylocalrank() + psm3_opened_endpoint_count) % psm3_addr_per_nic;
		if (psmi_hal_get_port_lid(unit, port?port:1, addr_index) <= 0)
			return 0;
	}
	return addr_index;
}

psm2_error_t
psm3_context_open(const psm2_ep_t ep, long unit_param, long port, long addr_index,
		  psm2_uuid_t const job_key, uint16_t network_pkey,
		  int64_t timeout_ns)
{
	long open_timeout = 0, unit_start, unit_end, unit_id, unit_id_prev;
	psm2_error_t err = PSM2_OK;
	int nunits = psmi_hal_get_num_units(), nunitsactive=0;
	union psmi_envvar_val env_rcvthread;
	static int norcvthread;	/* only for first rail */

	/*
	 * If shared contexts are enabled, try our best to schedule processes
	 * across one or many devices
	 */

	/* if no units, then no joy. */
	if (nunits <= 0)
	{
		err = psm3_handle_error(NULL, PSM2_EP_DEVICE_FAILURE,
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
		err = psm3_handle_error(NULL, PSM2_EP_DEVICE_FAILURE,
					"PSM3 no nic units are active");
		goto ret;
	}
	if (timeout_ns > 0)
		open_timeout = (long)(timeout_ns / MSEC_ULL);


	unit_start = 0; unit_end = nunits - 1;
	err = psmi_compute_start_and_end_unit(unit_param, addr_index,
					      nunitsactive, nunits, job_key,
					      &unit_start, &unit_end);
	if (err != PSM2_OK)
		goto ret;

	/* this is the start of a loop that starts at unit_start and goes to unit_end.
	   but note that the way the loop computes the loop control variable is by
	   an expression involving the mod operator. */
	int success = 0;
	unit_id_prev = unit_id = unit_start;
	do
	{
		/* if the unit_id is not active, go to next one. */
		if (psmi_hal_get_unit_active(unit_id) <= 0) {
			unit_id_prev = unit_id;
			unit_id = (unit_id + 1) % nunits;
			continue;
		}

		/* open this unit. */
		if (psmi_hal_context_open(unit_id, port,
				psmi_hash_addr_index(unit_id, port, addr_index),
				open_timeout,
				ep, job_key, HAL_CONTEXT_OPEN_RETRY_MAX)) {
			/* go to next unit if failed to open. */
			unit_id_prev = unit_id;
			unit_id = (unit_id + 1) % nunits;
			continue;
		}
		// HAL context_open has initialized:
		// ep->unit_id, ep->portnum, ep->addr_index,
		// ep->dev_name, ep->subnet, ep->addr, ep->gid, ep->wiremode,
		// ep->epid and
		// HAL specific ep fields (context, verbs_ep or sockets_ep)
		psmi_assert_always(! psm3_epid_zero_internal(ep->epid));
		success = 1;
		break;

	} while (unit_id_prev != unit_end);

	if (!success)
	{
		err = psm3_handle_error(NULL, PSM2_EP_DEVICE_FAILURE,
					"PSM3 can't open nic unit: %ld",unit_param);
		goto bail;
	}

	_HFI_VDBG("hal_context_open() passed.\n");

	psmi_assert_always(PSMI_IPS_ADDR_FMT_IS_VALID(ep->addr.fmt));
	psmi_assert_always(ep->addr.fmt == ep->subnet.fmt);
	psmi_assert_always(ep->addr.prefix_len == ep->subnet.prefix_len);
	ep->addr_fmt = ep->addr.fmt;

	_HFI_DBG("[%d]use unit %d port %d addr %d\n", getpid(), ep->unit_id, 1, ep->addr_index);

	/* device is opened, make sure we can find a valid desirable pkey */
	if ((err =
	     psm3_ep_verify_pkey(ep, network_pkey,
			&ep->network_pkey, &ep->network_pkey_index)) != PSM2_OK)
		goto close;

	/* See if we want to activate support for receive thread */
	psm3_getenv("PSM3_RCVTHREAD",
		    "Enable Recv thread (0 disables thread)",
		    PSMI_ENVVAR_LEVEL_USER, PSMI_ENVVAR_TYPE_UINT_FLAGS,
			// default to 0 for all but 1st rail
		    (union psmi_envvar_val)(norcvthread++ ? 0 :
					    PSMI_RCVTHREAD_FLAGS),
		    &env_rcvthread);

	/* If enabled, use the polling capability to implement a receive
	 * interrupt thread that can handle urg packets */
	if (env_rcvthread.e_uint) {
		psmi_hal_add_sw_status(PSM_HAL_PSMI_RUNTIME_RTS_RX_THREAD);
#ifdef PSMI_PLOCK_IS_NOLOCK
		psm3_handle_error(PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,
				  "#define PSMI_PLOCK_IS_NOLOCK not functional yet "
				  "with RCVTHREAD on");
#endif
	}
	_HFI_PRDBG("Opened unit %ld port %ld: EPID=%s %s\n", unit_id, port,
		psm3_epid_fmt_internal(ep->epid, 0), psm3_epid_fmt_addr(ep->epid, 1));

	goto ret;

close:
	psmi_hal_close_context(ep);
bail:
	_HFI_PRDBG("open failed: unit_id: %ld, err: %d (%s)\n", unit_id, err, strerror(errno));
ret:

	_HFI_VDBG("psm3_context_open() return %d\n", err);
	return err;
}

psm2_error_t psm3_context_close(psm2_ep_t ep)
{
	psmi_hal_close_context(ep);

	return PSM2_OK;
}

static inline char * _dump_cpu_affinity(char *buf, size_t buf_size, cpu_set_t * cpuset) {
	int i;
	int isfirst = 1;
	char tmp[25]; //%d = 10 :: 10 + '-' + 10 + ',' + '\0' = 23
	int first = -1, last = -1;

	for (i = 0; i < CPU_SETSIZE; i++) {
		if (CPU_ISSET(i, cpuset)) {
			if (first == -1) {
				first = last = i;
			} else if ((last+1) == i) {
				last = i;
			}
		} else if (first != -1) {
			if (first == last) {
				snprintf(tmp, sizeof(tmp), "%d,", first);
			} else {
				snprintf(tmp, sizeof(tmp), "%d-%d,", first, last);
			}
			first = last = -1;

			if (isfirst) {
				strncpy(buf, tmp, buf_size-1);
				isfirst=0;
			} else {
				strncat(buf, tmp, buf_size-1);
			}
			buf[buf_size-1] = '\0';
		}
	}

	if (first != -1) {
		if (first == last) {
			snprintf(tmp, sizeof(tmp), "%d,", first);
		} else {
			snprintf(tmp, sizeof(tmp), "%d-%d,", first, last);
		}
		if (isfirst) {
			strncpy(buf, tmp, buf_size-1);
		} else {
			strncat(buf, tmp, buf_size-1);
		}
		buf[buf_size-1] = '\0';
	}
	char *comma = strrchr(buf, ',');
	if (comma) comma[0] = '\0';

	return buf;
}

// called by HAL context_open to set affinity consistent with
// NIC NUMA location when NIC NUMA location is a superset of thread CPU set
// TBD unclear when this provides value.
int
psm3_context_set_affinity(psm2_ep_t ep, cpu_set_t nic_cpuset)
{
	pthread_t mythread = pthread_self();
	cpu_set_t cpuset;
	CPU_ZERO(&cpuset);

	int s = pthread_getaffinity_np(mythread, sizeof(cpu_set_t), &cpuset);
	if (s != 0) {
		psm3_handle_error(NULL, PSM2_EP_DEVICE_FAILURE,
			"Can't get CPU affinity: %s\n", strerror(errno));
		goto bail;
	}

	if (_HFI_DBG_ON) {
		char cpu_buf[128] = {0};
		_HFI_DBG_ALWAYS( "CPU affinity Before set: %s\n", _dump_cpu_affinity(cpu_buf, 128, &cpuset));
	}

	/*
	 * PSM3_FORCE_CPUAFFINITY forces affinity setting
	 * Otherwise, skip affinity if:
	 * 1. User explicitly sets PSM3_NO_AFFINITY in environment.
	 * 2. User doesn't set affinity in environment and PSM is opened with
	 *    option affinity skip.
	 */
	if (getenv("PSM3_FORCE_CPUAFFINITY") ||
		!(getenv("PSM3_NO_CPUAFFINITY") || ep->skip_affinity))
	{
		cpu_set_t andcpuset;

		int cpu_count = CPU_COUNT(&cpuset);
		int nic_count = CPU_COUNT(&nic_cpuset);
		if (cpu_count > nic_count) {
			andcpuset = cpuset;
		} else {
			CPU_AND(&andcpuset, &cpuset, &nic_cpuset);
		}
		int cpu_and_count = CPU_COUNT(&andcpuset);

		if (cpu_and_count > 0 && pthread_setaffinity_np(mythread, sizeof(andcpuset), &andcpuset)) {
			// bug on OPA, dev_name, unit_id not yet initialized
			// ok on UD and UDP
			_HFI_ERROR( "Failed to set %s (unit %d) cpu set: %s\n", ep->dev_name,  ep->unit_id, strerror(errno));
			//err = -PSM_HAL_ERROR_GENERAL_ERROR;
			goto bail;
		} else if (cpu_and_count == 0 && _HFI_DBG_ON) {
			char buf1[128] = {0};
			char buf2[128] = {0};
			_HFI_DBG_ALWAYS( "CPU affinity not set, NIC selected is not on the same socket as thread (\"%s\" & \"%s\" == 0).\n",
				_dump_cpu_affinity(buf1, 128, &nic_cpuset), _dump_cpu_affinity(buf2, 128, &cpuset));
		}
	}
	if (_HFI_DBG_ON) {
		CPU_ZERO(&cpuset);
		int s = pthread_getaffinity_np(mythread, sizeof(cpu_set_t), &cpuset);
		if (s != 0) {
			psm3_handle_error(NULL, PSM2_EP_DEVICE_FAILURE,
				"Can't get CPU affinity: %s\n", strerror(errno));
			goto bail;
		}
		char cpu_buf[128] = {0};
		_HFI_DBG_ALWAYS( "CPU affinity After set: %s\n", _dump_cpu_affinity(cpu_buf, 128, &cpuset));
	}
	return 0;

bail:
	return -1;
}

static psm2_error_t
psm3_ep_verify_pkey(psm2_ep_t ep, uint16_t pkey, uint16_t *opkey, uint16_t* oindex)
{
	int i, ret;
	psm2_error_t err;

	_HFI_PRDBG("looking for pkey 0x%x\n", pkey);
	for (i = 0; i < 16; i++) {
		ret = psmi_hal_get_port_index2pkey(ep, i);
		if (ret < 0) {
			err = psm3_handle_error(NULL, PSM2_EP_DEVICE_FAILURE,
						"Can't get a valid pkey value from pkey table on %s port %u\n", ep->dev_name, ep->portnum);
			return err;
		}
		// pkey == 0 means just get slot 0
		if (! pkey && ! i)
			break;
		if ((pkey & 0x7fff) == (uint16_t)(ret & 0x7fff)) {
			break;
		}
	}

	/* if pkey does not match */
	if (i == 16) {
		err = psm3_handle_error(NULL, PSM2_EP_DEVICE_FAILURE,
					"Wrong pkey 0x%x on %s port %u, please use PSM3_PKEY to specify a valid pkey\n",
					pkey, ep->dev_name, ep->portnum);
		return err;
	}

	if (((uint16_t)ret & 0x8000) == 0) {
		err = psm3_handle_error(NULL, PSM2_EP_DEVICE_FAILURE,
					"Limited Member pkey 0x%x on %s port %u, please use PSM3_PKEY to specify a valid pkey\n",
					(uint16_t)ret, ep->dev_name, ep->portnum);
		return err;
	}

	/* return the final pkey */
	*opkey = (uint16_t)ret;
	*oindex = (uint16_t)i;

	return PSM2_OK;
}

static
int psmi_parse_nic_selection_algorithm(void)
{
	union psmi_envvar_val env_nic_alg;
	int nic_alg = PSMI_UNIT_SEL_ALG_ACROSS;

	/* If a specific unit is set in the environment, use that one. */
	psm3_getenv("PSM3_NIC_SELECTION_ALG",
		    "NIC Device Selection Algorithm to use. Round Robin[RoundRobin or rr] (Default) "
		    ", Packed[p] or Round Robin All[RoundRobinAll or rra].",
		    PSMI_ENVVAR_LEVEL_USER, PSMI_ENVVAR_TYPE_STR,
		    (union psmi_envvar_val)"rr", &env_nic_alg);

	if (!strcasecmp(env_nic_alg.e_str, "Round Robin")
		|| !strcasecmp(env_nic_alg.e_str, "RoundRobin")
		|| !strcasecmp(env_nic_alg.e_str, "rr"))
		nic_alg = PSMI_UNIT_SEL_ALG_ACROSS;
	else if (!strcasecmp(env_nic_alg.e_str, "Packed")
			 || !strcasecmp(env_nic_alg.e_str, "p"))
		nic_alg = PSMI_UNIT_SEL_ALG_WITHIN;
	else if (!strcasecmp(env_nic_alg.e_str, "Round Robin All")
			 || !strcasecmp(env_nic_alg.e_str, "RoundRobinAll")
			 || !strcasecmp(env_nic_alg.e_str, "rra"))
		nic_alg = PSMI_UNIT_SEL_ALG_ACROSS_ALL;
	else {
		_HFI_ERROR
		    ("Unknown NIC selection algorithm %s. Defaulting to Round Robin "
		     "allocation of NICs.\n", env_nic_alg.e_str);
		nic_alg = PSMI_UNIT_SEL_ALG_ACROSS;
	}

	return nic_alg;
}
