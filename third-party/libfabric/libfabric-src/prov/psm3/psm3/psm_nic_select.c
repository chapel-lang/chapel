/*

  This file is provided under a dual BSD/GPLv2 license.  When using or
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2024 Intel Corporation.

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

  Copyright(c) 2024 Intel Corporation.

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

/* Copyright (c) 2003-2024 Intel Corporation. All rights reserved. */

#include <sys/types.h>
#include <sys/stat.h>
#include "psm_user.h"
#include "psm2_hal.h"
#ifdef PSM_USE_HWLOC
#include <hwloc.h>
#include <dlfcn.h>
#endif

#define MAX_MAP_LEN (PSMI_MAX_RAILS*128)

// sanity check, psm_user.h should ensure this, unless user tried to
// manually set PSM_HAVE_GPU_CENTRIC_AFFINITY
#ifdef PSM_HAVE_GPU_CENTRIC_AFFINITY
#ifndef PSM_USE_HWLOC
#error "PSM_HAVE_GPU_CENTRIC_AFFINITY set without PSM_USE_HWLOC"
#endif
#endif

// PSM3_NIC_SELECTION_ALG choices.
// ALG_NUMA is the default. This option spreads the NIC selection within the
// local CPU socket's NICs (NUMA).
// If it is preferred to spread job over over entire set of NICs within the
// system, use ALG_ANY.
// For systems with PCIe switches for GPU Direct, GPU_CENTRIC is typically best.
// For GPU systems w/o switches, CPU_CENTRIC may be best.
#define PSMI_NIC_SEL_ALG_NUMA        0 /* Round Robin within NUMA */
#define PSMI_NIC_SEL_ALG_FIRST       1 /* First Active NIC */
#define PSMI_NIC_SEL_ALG_ANY         2 /* Round Robin All */
#define PSMI_NIC_SEL_ALG_CPU_CENTRIC 3 /* Round Robin, prefer CPU distance */
#ifdef PSM_HAVE_GPU_CENTRIC_AFFINITY
#define PSMI_NIC_SEL_ALG_GPU_CENTRIC 4 /* Round Robin, prefer GPU distance */
#endif


// subnuma is risky right now, so disable and explore in future
//#ifdef PSM_USE_HWLOC
//#define PSM3_HAVE_CPU_SUBNUMA
//#endif
#undef PSM3_HAVE_CPU_SUBNUMA

#if defined(PSM_HAVE_GPU_CENTRIC_AFFINITY) || defined(PSM3_HAVE_CPU_SUBNUMA)
struct pci_addr {
	uint32_t domain;
	uint32_t bus;
	uint32_t dev;
	uint32_t func;
};
#endif

// table of refcount per unit_id counting references by endpoints within
// local process
// protected by psm3_creation_lock (held in psm_ep.c during EP open and close)
static uint64_t psm3_nic_refcount[PSMI_MAX_RAILS];

// psm3_shared_affinity_nic_refcount_ptr is the pointer to table of refcount
// per unit_id countting references by all processes within node.
// protected by psm3_sem_affinity_shm_rw semaphore

static int psmi_parse_nic_selection_algorithm(void);

#if defined(PSM_HAVE_GPU_CENTRIC_AFFINITY) || defined(PSM3_HAVE_CPU_SUBNUMA)
static hwloc_topology_t psm3_hwloc_topology;
static int psm3_hwloc_topology_initialized;
static int psm3_hwloc_topology_init_failed;
static void psm3_deferred_hwloc_topology_init();
#endif

#ifdef PSM_HAVE_GPU_CENTRIC_AFFINITY
static int psm3_get_distance_between_pcis(const struct pci_addr *pci_addr_1,
                                          const struct pci_addr *pci_addr_2);
#endif
#ifdef PSM3_HAVE_CPU_SUBNUMA
static hwloc_obj_t psm3_get_non_io_ancestor_obj(
											const struct pci_addr *pci_addr);
#endif

// As we consider and select NICs, we fill in additional information
// or set filtered to exclude the NIC from further consideration.
// The use of filtered avoids the cost of repeatedly compressing the list.
struct nic_info {
	uint8_t filtered;	// has NIC been filtered out from possible selection
	psmi_subnet128_t subnet;
	unsigned unit;
	unsigned port;
	unsigned addr_index;
	int numa_id;	// CPU NUMA location of NIC
#if defined(PSM_HAVE_GPU_CENTRIC_AFFINITY) || defined(PSM3_HAVE_CPU_SUBNUMA)
	struct pci_addr pci_addr;
#endif
#ifdef PSM3_HAVE_CPU_SUBNUMA
	int cpu_close;	// is CPU sub-numa close to NIC
#endif
#ifdef PSM_HAVE_GPU_CENTRIC_AFFINITY
	int gpu_distance;
#endif
};


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

// print a bitmask in condensed form at _HFI_VBG level
// condensed form consolidates sequential numbers such as: "0-43,88-131"
static void vdbg_print_bitmask(const char* prefix, struct bitmask *bmp)
{
	if (_HFI_VDBG_ON) {
		int i, len;
		char buf[1024];
		int last=-1;
		int first=-1;
		int max = numa_num_possible_nodes();

		snprintf(buf, sizeof(buf), "%s", prefix);
		len = strlen(buf);
		for (i=0; i<max; i++) {
			if (! numa_bitmask_isbitset(bmp, i))
				continue;
			if (last == -1) {
				// 1st found
				snprintf(&buf[len], sizeof(buf)-len, "%d", i);
				first = i;
				last = first;
			} else if ((i-last) > 1) {
				if (first == last) {
					// first in a possible sequence
					snprintf(&buf[len], sizeof(buf)-len, ",%d", i);
				} else {
					// complete prior sequence, first in a new sequence
					snprintf(&buf[len], sizeof(buf)-len, "-%d,%d", last, i);
				}
				first = i;
				last = first;
			} else {
				last = i;
			}
			len = strlen(buf);
		}
		// complete prior sequence as needed
		if (first>=0 && first != last)
			snprintf(&buf[len], sizeof(buf)-len, "-%d", last);
		_HFI_VDBG("%s\n", buf);
	}
}

// return the largest possible numa ID of a CPU in this system
int psm3_get_max_cpu_numa()
{
	static int max_cpu_numa = -1;
	struct bitmask *cpumask, *empty_cpumask;
	int i;

	if (max_cpu_numa >= 0)
		return max_cpu_numa;

	// we don't depend on numa_num_configured_nodes since in theory there
	// could be non-CPU memory NUMA nodes.  We only need to know the
	// largest possible value for a CPU numa node ID

	// numa_max_node - largest NUMA node which is not disabled
	// numa_node_to_cpus - given a NUMA node, create list of CPUs
	// numa_node_of_cpu - cpu ID to NUMA (or error if invalid CPU)
	// numa_node_to_cpus - cpumask of CPUs on given NUMA node

	max_cpu_numa = -1;
	empty_cpumask = numa_allocate_cpumask();
	numa_bitmask_clearall(empty_cpumask);
	//vdbg_print_bitmask("empty_cpumask: ", empty_cpumask);

	cpumask = numa_allocate_cpumask();
	_HFI_VDBG("numa_max_node=%d\n", numa_max_node());
	for (i=numa_max_node(); i >= 0; i--) {
		numa_bitmask_clearall(cpumask);
		int ret = numa_node_to_cpus(i, cpumask);
		_HFI_VDBG("i=%d node_to_cpus ret=%d\n", i, ret);
		vdbg_print_bitmask("cpumask: ", cpumask);
		if (ret >= 0 && ! numa_bitmask_equal(cpumask, empty_cpumask)) {
			max_cpu_numa = i;
			break;
		}
	}
	numa_free_cpumask(cpumask);
	numa_free_cpumask(empty_cpumask);
	psmi_assert_always(max_cpu_numa >= 0);
	return max_cpu_numa;
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

/*
 * Get all the ports and optionally addr_index'es with a valid lid and gid,
 * one port per unit but up to PSM3_ADDR_PER_NIC addresses.
 *
 * Returns count of entries put in nic_info
 *
 * There will be exactly per_addr_index entries per active unit all for the
 * same port within the unit
 */
unsigned nic_info_init(struct nic_info *nic_info, unsigned nunits, int per_addr_index)
{
	unsigned unit, port, addr_index;
	unsigned num_addr_index = per_addr_index?psm3_addr_per_nic:1;
	int ret;
	unsigned count = 0;

	for (unit = 0; unit < nunits; unit++) {
		// get_unit_active is redundant since it loops on all ports and
		// confirms at least 1 port has a valid lid.  We test that below.
		//if (psmi_hal_get_unit_active(unit) <= 0)
		//	continue;
		for (port = PSM3_NIC_MIN_PORT; port <= PSM3_NIC_MAX_PORT; port++) {
			int got_port = 0;
			for (addr_index = 0; addr_index < num_addr_index; addr_index++) {
				psmi_subnet128_t subnet;
				ret = psmi_hal_get_port_lid(unit, port, addr_index);
				if (ret <= 0)
					continue;
				ret = psmi_hal_get_port_subnet(unit, port, addr_index, &subnet, NULL, NULL, NULL);
				if (ret == -1)
					continue;

				nic_info[count].filtered = 0;
				nic_info[count].subnet = subnet;
				nic_info[count].unit = unit;
				nic_info[count].port = port;
				nic_info[count].addr_index = addr_index;
#if defined(PSM_HAVE_GPU_CENTRIC_AFFINITY) || defined(PSM3_HAVE_CPU_SUBNUMA)
				nic_info[count].pci_addr.domain = UINT32_MAX;
#endif
				got_port = 1;
				count++;
			}
			if (got_port)	// one port per unit
				break;
		}
	}
	return count;
}

/* If at least 1 NIC matches the current CPUs NUMA id,
 * filter out all NICs which do not match.
 * If none match, noop.
 * Also initializes nic_info.numa_id
 */
void nic_info_filter_numa(struct nic_info *nic_info, unsigned ninfo)
{
	unsigned i;
	int found = 0;

	int cpu_numa_id = psm3_get_current_proc_location();
	if (cpu_numa_id < 0) {
		_HFI_DBG("Unable to determine CPU NUMA location, skipping filter of NIC CPU NUMA location\n");
		return;
	}

	for (i=0; i < ninfo; i++)
	{
		if (nic_info[i].filtered)
			continue;

		if (psmi_hal_get_node_id(nic_info[i].unit, &nic_info[i].numa_id) != 0) {
			// assume match (don't filter this NIC)
			_HFI_DBG("Unable to determine NIC NUMA location for unit %d (%s), assuming local to CPU NUMA (%d)\n",
				nic_info[i].unit, psm3_sysfs_unit_dev_name(nic_info[i].unit),
				cpu_numa_id);
			nic_info[i].numa_id = cpu_numa_id;
		} else {
			_HFI_DBG("NIC NUMA location for unit %d (%s) is %d\n",
				nic_info[i].unit, psm3_sysfs_unit_dev_name(nic_info[i].unit),
				nic_info[i].numa_id);
		}
		found += (nic_info[i].numa_id == cpu_numa_id);
	}
	if (found) {
		_HFI_DBG("Found %d unfiltered NUMA local NICs for CPU NUMA id = %d\n",
					found, cpu_numa_id);
		// filter out NICs not in cpu_numa_id
		for (i=0; i < ninfo; i++)
		{
			if (nic_info[i].filtered)
				continue;
			nic_info[i].filtered = (nic_info[i].numa_id != cpu_numa_id);
		}
	} else {
		_HFI_DBG("No NUMA local NIC found, CPU NUMA id = %d\n", cpu_numa_id);
	}
}

/* If at least 1 NIC matches the current CPUs sub-NUMA group,
 * filter out all NICs which do not match.
 * If none match, noop.
 * Also initializes nic_info.pci_addr and nic_info.cpu_close
 */
void nic_info_filter_sub_numa(struct nic_info *nic_info, unsigned ninfo)
{
#ifdef PSM3_HAVE_CPU_SUBNUMA
	unsigned i;
	int found = 0;
	hwloc_cpuset_t cpu_bind_set;

	psm3_deferred_hwloc_topology_init();
	if (psm3_hwloc_topology_init_failed)
		return;	// hwloc incorrect version
	psmi_assert(psm3_hwloc_topology_initialized);

	// here we use entire CPU bind set, (should match pthread_getaffinity_np)
	// as opposed to just the current process location.
	cpu_bind_set = hwloc_bitmap_alloc();
	if (! cpu_bind_set) {
		_HFI_DBG("Unable to allocate CPU set, skipping filter of CPU sub-NUMA location\n");
		return;
	}
#if 0
	// use current process affinity
	if (hwloc_get_cpubind(psm3_hwloc_topology, cpu_bind_set,
							HWLOC_CPUBIND_PROCESS)) {
		_HFI_DBG("Unable to determine process CPU binding, skipping filter of CPU sub-NUMA location\n");
		goto fail;
	}
#else
	// use current thread affinity
	pthread_t mythread = pthread_self();
	if (hwloc_get_thread_cpubind(psm3_hwloc_topology, mythread,
					cpu_bind_set, HWLOC_CPUBIND_THREAD)) {
		_HFI_DBG("Unable to determine thread CPU binding, skipping filter of CPU sub-NUMA location\n");
		goto fail;
	}
#endif

	for (i=0; i < ninfo; i++)
	{
		if (nic_info[i].filtered)
			continue;
		if (nic_info[i].pci_addr.domain == UINT32_MAX
			&& psmi_hal_get_unit_pci_bus(nic_info[i].unit,
					 &nic_info[i].pci_addr.domain, &nic_info[i].pci_addr.bus,
					 &nic_info[i].pci_addr.dev, &nic_info[i].pci_addr.func)) {
			_HFI_DBG("Unable to get NIC PCIe address for unit %d (%s)\n",
				nic_info[i].unit, psm3_sysfs_unit_dev_name(nic_info[i].unit));
			// can't filter out NIC because if all fail we won't have any.
			// Unsure how to rank this NIC vs others, so assume not close
			nic_info[i].cpu_close = 0;
			continue;
		}

		hwloc_obj_t ancestor = psm3_get_non_io_ancestor_obj(
													&nic_info[i].pci_addr);
		if (! ancestor) {
			_HFI_DBG("Unable to determine NIC ancestor for unit %d (%s) at PCIe %04x:%02x:%02x.%x\n",
				nic_info[i].unit, psm3_sysfs_unit_dev_name(nic_info[i].unit),
				nic_info[i].pci_addr.domain, nic_info[i].pci_addr.bus,
				nic_info[i].pci_addr.dev, nic_info[i].pci_addr.func);
			// can't filter out NIC because if all fail we won't have any.
			// Unsure how to rank this NIC vs others, so assume not close
			nic_info[i].cpu_close = 0;
			continue;
		}

		// If any overlap of NIC and process CPU sets, consider it close
		nic_info[i].cpu_close =
				hwloc_bitmap_isincluded(cpu_bind_set, ancestor->cpuset)
				|| hwloc_bitmap_isincluded(ancestor->cpuset, cpu_bind_set);

		if (_HFI_DBG_ON) {
			char buf[256] = {0};;
			hwloc_bitmap_list_snprintf(buf, sizeof(buf), ancestor->cpuset);
			buf[sizeof(buf)-1] = '\0';	// paranoid, hwloc doc not clear
			_HFI_DBG_ALWAYS("NIC closeness to CPU for unit %d (%s) at %u:%u:%u:%u is %d, NIC close to CPUs: %s\n",
				nic_info[i].unit, psm3_sysfs_unit_dev_name(nic_info[i].unit),
				nic_info[i].pci_addr.domain, nic_info[i].pci_addr.bus,
				nic_info[i].pci_addr.dev, nic_info[i].pci_addr.func,
				nic_info[i].cpu_close, buf);
		}
		found += nic_info[i].cpu_close;
	}
	if (found) {
		if (_HFI_DBG_ON) {
			char buf[256] = {0};;
			hwloc_bitmap_list_snprintf(buf, sizeof(buf), cpu_bind_set);
			buf[sizeof(buf)-1] = '\0';	// paranoid, hwloc doc not clear
			_HFI_DBG_ALWAYS("Found %d unfiltered NICs close to CPUs: %s\n", found, buf);
		}
		// filter out NICs not close
		for (i=0; i < ninfo; i++)
		{
			if (nic_info[i].filtered)
				continue;
			nic_info[i].filtered = ! nic_info[i].cpu_close;
		}
	} else {
		if (_HFI_DBG_ON) {
			char buf[256] = {0};;
			hwloc_bitmap_list_snprintf(buf, sizeof(buf), cpu_bind_set);
			buf[sizeof(buf)-1] = '\0';	// paranoid, hwloc doc not clear
			_HFI_DBG_ALWAYS("No NICs found close to CPUs: %s\n", buf);
		}
	}
fail:
	hwloc_bitmap_free(cpu_bind_set);
#else
	//_HFI_DBG("Filtering based on CPU closeness to NIC disabled\n");
#endif
}

#ifdef PSM_HAVE_GPU_CENTRIC_AFFINITY
/* Find the closest NIC to the current GPU and then fiter out all NICs
 * which are further from the GPU than that closest NIC
 * If no GPU for the process yet, or PSM3 GPU support not enabled, noop.
 * Also initializes nic_info.pci_addr and nic_info.gpu_distance
 */
void nic_info_filter_gpu_distance(struct nic_info *nic_info, unsigned ninfo)
{
	unsigned i;
	int min_distance = INT_MAX;	// smallest distance found
	unsigned found = 0;
	struct pci_addr gpu_pci_addr = { 0 };

	if (! PSM3_GPU_IS_ENABLED)
		return;

	psm3_deferred_hwloc_topology_init();
	if (psm3_hwloc_topology_init_failed)
		return;	// hwloc incorrect version
	psmi_assert(psm3_hwloc_topology_initialized);

	// Get current GPU's PCIe address to gpu_pci_addr;
	PSM3_GPU_GET_PCI_ADDR( &gpu_pci_addr.domain, &gpu_pci_addr.bus,
		&gpu_pci_addr.dev, &gpu_pci_addr.func);
	_HFI_DBG("GPU PCIe address is %04x:%02x:%02x.%x\n",
				gpu_pci_addr.domain, gpu_pci_addr.bus,
				gpu_pci_addr.dev, gpu_pci_addr.func);

	for (i=0; i < ninfo; i++) {
		if (nic_info[i].filtered)
			continue;
		if (nic_info[i].pci_addr.domain == UINT32_MAX
			&& psmi_hal_get_unit_pci_bus(nic_info[i].unit,
					 &nic_info[i].pci_addr.domain, &nic_info[i].pci_addr.bus,
					 &nic_info[i].pci_addr.dev, &nic_info[i].pci_addr.func)) {
			_HFI_DBG("Unable to get NIC PCIe address for unit %d (%s)\n",
				nic_info[i].unit, psm3_sysfs_unit_dev_name(nic_info[i].unit));
			// can't filter out NIC because if all fail we won't have any.
			// Unsure how to rank this NIC vs others, so use max distance
			nic_info[i].gpu_distance = INT_MAX;
			continue;
		}
		nic_info[i].gpu_distance = psm3_get_distance_between_pcis(
										&nic_info[i].pci_addr, &gpu_pci_addr);
		_HFI_DBG("NIC PCIe address for unit %d (%s) is %04x:%02x:%02x.%x distance to GPU: %d\n",
				nic_info[i].unit, psm3_sysfs_unit_dev_name(nic_info[i].unit),
				nic_info[i].pci_addr.domain, nic_info[i].pci_addr.bus,
				nic_info[i].pci_addr.dev, nic_info[i].pci_addr.func,
				nic_info[i].gpu_distance);
		if (nic_info[i].gpu_distance < min_distance) {
			min_distance = nic_info[i].gpu_distance;
		}
	}
	if (min_distance == INT_MAX) {
		_HFI_DBG("No NIC found with a known distance\n");
		return;	// noop
	}

	// filter out all NICs with a distance > min_distance
	for (i=0; i < ninfo; i++) {
		if (nic_info[i].filtered)
			continue;
		psmi_assert(nic_info[i].gpu_distance >= min_distance);
		nic_info[i].filtered = (nic_info[i].gpu_distance > min_distance);
		found += ! nic_info[i].filtered;
	}
	_HFI_DBG("Found %d unfiltered NICs with GPU distance of %d\n",
					found, min_distance);
}
#endif /* PSM_HAVE_GPU_CENTRIC_AFFINITY */

// filter down the list of NICs solely based on CPU NUMA locality
static void nic_info_filter_cpu_numa(struct nic_info *nic_info,
										unsigned ninfo)
{
	_HFI_DBG("Filtering NICs with CPU NUMA Strategy\n");
	nic_info_filter_numa(nic_info, ninfo);
}

// filter down the list of NICs with a CPU locality focus as priority
// if present, the GPU is considered last.  If the GPU is NUMA local
// to the CPU, the GPU filter can further limit NICs to those close to the
// GPU (same PCIe switch).  But if the GPU is not NUMA local to the CPU,
// the gpu distance filter may still limit distance or end up being a noop.
static void nic_info_filter_cpu_centric(struct nic_info *nic_info,
										unsigned ninfo)
{
	_HFI_DBG("Filtering NICs with CPU Centric Strategy\n");
	nic_info_filter_sub_numa(nic_info, ninfo);
	nic_info_filter_numa(nic_info, ninfo);
#ifdef PSM_HAVE_GPU_CENTRIC_AFFINITY
	nic_info_filter_gpu_distance(nic_info, ninfo);
#endif
}

#ifdef PSM_HAVE_GPU_CENTRIC_AFFINITY
// filter down the list of NICs with a GPU locality focus as priority
// When there is a GPU, once we have selected NICs closest to that
// GPU we are likely to have limited ourselves to NICs in the same
// NUMA as the GPU, so the CPU NUMA tests will become noops.
// For example, a GPU and NIC on the same PCIe switch will by definition
// be in the same CPU root complex and hence same CPU NUMA.
// But if there is no GPU or none of the NICs are close to the GPU
// the CPU numa tests may narrow the list of NICs.
static void nic_info_filter_gpu_centric(struct nic_info *nic_info,
										unsigned ninfo)
{
	_HFI_DBG("Filtering NICs with GPU Centric Strategy\n");
	nic_info_filter_gpu_distance(nic_info, ninfo);
	nic_info_filter_numa(nic_info, ninfo);
	nic_info_filter_sub_numa(nic_info, ninfo);
}
#endif /* PSM_HAVE_GPU_CENTRIC_AFFINITY */

// analyze the refcount table and filter out NICs with refcounts
// higher than the lowest found.
// If all NICs have equal refcounts, noop.
static void
nic_info_filter_refcount(struct nic_info *nic_info, unsigned ninfo,
					uint64_t *refcount, unsigned nunits, const char *str)
{
	unsigned i;
	uint64_t min_refcount = UINT64_MAX;	// smallest refcount found
	unsigned found = 0;

	for (i=0; i < ninfo; i++) {
		if (nic_info[i].filtered)
			continue;
		psmi_assert(nic_info[i].unit < nunits);
		_HFI_DBG("NIC %s reference count for unit %d (%s) is %"PRIu64"\n", str,
				nic_info[i].unit, psm3_sysfs_unit_dev_name(nic_info[i].unit),
				refcount[nic_info[i].unit]);
		if (refcount[nic_info[i].unit] < min_refcount) {
			min_refcount = refcount[nic_info[i].unit];
			psmi_assert(nic_info[i].unit < nunits);;
		}
	}
	if (min_refcount == UINT64_MAX) {
		// unexpected, should have found a smaller value
		_HFI_DBG("No NIC found with a low %s reference count\n", str);
		return;	// noop
	}

	// filter out all NICs with a refcount > min_refcount
	for (i=0; i < ninfo; i++) {
		if (nic_info[i].filtered)
			continue;
		psmi_assert(refcount[nic_info[i].unit] >= min_refcount);
		nic_info[i].filtered = (refcount[nic_info[i].unit] > min_refcount);
		found += ! nic_info[i].filtered;
	}
	_HFI_DBG("Found %d unfiltered NICs with %s reference count of %"PRIu64"\n",
					found, str, min_refcount);
}

// return index in nic_info of 1st unfiltered NIC
static unsigned
nic_info_get_first_unfiltered_nic(struct nic_info *nic_info, unsigned ninfo)
{
	unsigned i;
	for (i=0; i < ninfo; i++) {
		if (! nic_info[i].filtered)
			return i;
	}
	psmi_assert(0);
	return 0;
}

/*
 * Select NIC among the unfiltered NICs in nic_info while
 * scoreboarding use of each NIC and picking the one with lowest
 * unit number and least use.
 *
 * Scoreboarding starts with the local process's NIC usage across all EPs
 * This helps to ensure a given process balances itself across unfiltered NICs
 * on the assumption that all local processes will ultimately have the same
 * number of endpoints.
 *
 * After the local process scoreboarding, the shm scoreboard is checked
 * to pick a NIC based on lowest refcount within the server.  Thus balancing
 * NIC usage within the server.
 *
 * Among NICs with the lowest reference counts, the lowest entry in nic_info
 * (also lowest unit_id) is selected.
 * This assumes only one entry appears in nic_info for each unit_id
 * (eg. nic_info_init was given per_addr_index of 1) and the entries in
 * nic_info are sorted by unit_id (in order built by nic_info_init).
 *
 * Due to call sequence prior to this, nic_info list will already be sorted by
 * unit_id since it was built in that order by nic_info_init.
 * Returns index in nic_info of selected NIC.
 * On any issues, selects 1st NIC
 */
static int
psm3_open_shm_scoreboard_and_select_nic(
						struct nic_info *nic_info, unsigned ninfo,
						psm2_uuid_t const job_key, unsigned nunits)
{
	int ret, shm_location, index;

	psmi_assert(nunits > 0);
	psmi_assert(ninfo > 0);

	// balance among endpoints within current process
	nic_info_filter_refcount(nic_info, ninfo,
							psm3_nic_refcount, nunits, "local process");

	psm3_create_affinity_semaphores(job_key);
	/*
	 * Take affinity lock and open shared memory region to be able to
	 * accurately determine which NIC to pick for this process. If any
	 * issues, bail by picking first unfiltered NIC in nic_info
	 */
	if (!psm3_affinity_semaphore_open)
		goto fallback;

	ret = psm3_create_and_open_affinity_shm(job_key);
	if (ret < 0)
		goto fallback;

	// start of scoreboard area, we keep refcount for each unit_id.
	// Note that some other modes may organize the shm area differently,
	// so it's important that all processes and all endpoints use the same
	// fundamental modes for PSM3_MULTIRAIL and PSM3_NIC_SELECTION_ALG
	shm_location = AFFINITY_SHM_HFI_INDEX_LOCATION;
	if (shm_location + sizeof(*psm3_shared_affinity_ptr)*nunits > PSMI_PAGESIZE)
		goto fallback;

	// At psm3_shm_refcount_ptr in Linux shared memory is a table indexed
	// by unit_id with a reference count per NIC showing the total
	// endpoints within the job which are using the NIC.
	psm3_shared_affinity_nic_refcount_ptr =
									&psm3_shared_affinity_ptr[shm_location];

	/* Start critical section to read/write shm object */
	if (psmi_sem_timedwait(psm3_sem_affinity_shm_rw, psm3_sem_affinity_shm_rw_name)) {
		_HFI_VDBG("Could not enter critical section to update NIC index\n");
		goto fallback;
	}

	// balance among processes within current node
	nic_info_filter_refcount(nic_info, ninfo,
					psm3_shared_affinity_nic_refcount_ptr,
					nunits, "local node");

	// use lowest index among those which remain
	index = nic_info_get_first_unfiltered_nic(nic_info, ninfo);

	// update reference counts for node level and process level
	psm3_shared_affinity_nic_refcount_ptr[nic_info[index].unit]++;
	psm3_nic_refcount[nic_info[index].unit]++;

	/* End Critical Section */
	psmi_sem_post(psm3_sem_affinity_shm_rw, psm3_sem_affinity_shm_rw_name);

	psmi_assert(index >= 0 && index <= ninfo);
	_HFI_DBG("Selected NIC unit %d(%s)\n",
		nic_info[index].unit, psm3_sysfs_unit_dev_name(nic_info[index].unit));
	return index;

fallback:
	index = nic_info_get_first_unfiltered_nic(nic_info, ninfo);
	psm3_nic_refcount[nic_info[index].unit]++;	// inc process level refcount
	return index;
}

// decrement reference counts which were incremented in local process
// and in shm within node
// For modes which do not track this style of refcounts psm3_nic_refcount
// will be zero for every unit_id and psm3_shared_affinity_nic_refcount_ptr will
// be NULL (or if psm3 has been finalized)
void psm3_dec_nic_refcount(int unit_id)
{
	// in some modes we don't track refcount, in which case do nothing
	if (psm3_nic_refcount[unit_id])
		psm3_nic_refcount[unit_id]--;
	if (psm3_affinity_shared_file_opened && psm3_affinity_semaphore_open
		&& psm3_shared_affinity_nic_refcount_ptr) {
		/* Start critical section to read/write shm object */
		if (psmi_sem_timedwait(psm3_sem_affinity_shm_rw, psm3_sem_affinity_shm_rw_name)) {
			_HFI_VDBG("Could not enter critical section to update NIC refcount\n");
		} else {
			psm3_shared_affinity_nic_refcount_ptr[unit_id]--;
			/* End Critical Section */
			psmi_sem_post(psm3_sem_affinity_shm_rw, psm3_sem_affinity_shm_rw_name);
		}
	}
}

static int parse_selection_alg(const char *str)
{
	if (!strcasecmp(str, "Round Robin")
		|| !strcasecmp(str, "RoundRobin")
		|| !strcasecmp(str, "rr"))
		return PSMI_NIC_SEL_ALG_NUMA;
	else if (!strcasecmp(str, "Packed")
			 || !strcasecmp(str, "p"))
		return PSMI_NIC_SEL_ALG_FIRST;
	else if (!strcasecmp(str, "Round Robin All")
			 || !strcasecmp(str, "RoundRobinAll")
			 || !strcasecmp(str, "rra"))
		return PSMI_NIC_SEL_ALG_ANY;
	else if (!strcasecmp(str, "CPU Centric Round Robin")
			 || !strcasecmp(str, "CpuRoundRobin")
			 || !strcasecmp(str, "crr"))
		return PSMI_NIC_SEL_ALG_CPU_CENTRIC;
#ifdef PSM_HAVE_GPU_CENTRIC_AFFINITY
	else if (!strcasecmp(str, "GPU Centric Round Robin")
			 || !strcasecmp(str, "GpuRoundRobin")
			 || !strcasecmp(str, "grr"))
		return PSMI_NIC_SEL_ALG_GPU_CENTRIC;
#endif
	else
		return -1;
}

/* check for valid PSM3_SELECTION_ALG
 * returns:
 * 0 - valid
 * -1 - empty string
 * -2 - invalid syntax
 */
static int parse_check_selection_alg(int type, const union psmi_envvar_val val,
						void *ptr, size_t errstr_size, char errstr[])
{
		psmi_assert(type == PSMI_ENVVAR_TYPE_STR);
		if (! val.e_str || ! *val.e_str)
			return -1;
		if (parse_selection_alg(val.e_str) < 0)
			return -2;
		return 0;
}

static
int psmi_parse_nic_selection_algorithm(void)
{
	union psmi_envvar_val env_nic_alg;
	int nic_alg;

	const char* PSM3_NIC_SELECTION_ALG_HELP =
		    "Round Robin[RoundRobin or rr] (Default)"
		    ", Packed[p], Round Robin All[RoundRobinAll or rra],"
#ifdef PSM_HAVE_GPU_CENTRIC_AFFINITY
			" CPU Centric Round Robin [CpuRoundRobin or crr]"
			", or GPU Centric Round Robin [GpuRoundRobin or grr]";
#else
			" or CPU Centric Round Robin [CpuRoundRobin or crr]";
#endif

	psm3_getenv_range("PSM3_NIC_SELECTION_ALG",
			"NIC Device Selection Algorithm",
			PSM3_NIC_SELECTION_ALG_HELP,
			PSMI_ENVVAR_LEVEL_USER, PSMI_ENVVAR_TYPE_STR,
			(union psmi_envvar_val)"rr",
			(union psmi_envvar_val)NULL, (union psmi_envvar_val)NULL,
			parse_check_selection_alg, NULL, &env_nic_alg);

	nic_alg = parse_selection_alg(env_nic_alg.e_str);
	psmi_assert(nic_alg >= 0);
	return nic_alg;
}

// Autoselect one unit for non-multirail operation.
// caller will select 1st active port and an addr_index within unit
// returns the unit number or -1 if unable to find an active unit
int
psm3_autoselect_one(long addr_index, int nunits, psm2_uuid_t const job_key)
{
	unsigned short nic_sel_alg;
	unsigned first_active = nunits;	// invalid value. for error check
	int have_subnet = 0, unit_id;
	psmi_subnet128_t got_subnet = { };
	unsigned ninfo;
	struct nic_info nic_info[PSMI_MAX_RAILS];
	unsigned index;
	int nunitsactive = 0;

	// find first_active, also if NICs are on different planes
	// (non-routed subnets) we need to have all ranks default to the
	// same plane so force 1st active NIC in that case
	for (unit_id = 0; unit_id < nunits; unit_id++) {
		psmi_subnet128_t subnet;
		if (psmi_hal_get_unit_active(unit_id) <= 0)
			continue;
		if (0 != psmi_hal_get_port_subnet(unit_id, 1 /* VERBS_PORT*/,
						addr_index>0?addr_index:0,
						&subnet, NULL, NULL, NULL))
			continue; // can't access NIC
		// found an active viable NIC
		nunitsactive++;
		if (! have_subnet) {
			have_subnet = 1;
			got_subnet = subnet;
			first_active = unit_id;
		} else if (! psm3_subnets_match(got_subnet, subnet)) {
			// Active units have different tech (IB/OPA vs Eth)
			// or different subnets.
			// Use 1st active unit so all ranks in job can communicate
			_HFI_DBG("Multi-Plane config: Using 1st viable NIC unit= %u.\n",
					first_active);
			return first_active;
		}
	}
	if (nunitsactive == 0)
		return -1;

	nic_sel_alg = psmi_parse_nic_selection_algorithm();

	if (nunitsactive <= 1 || nic_sel_alg == PSMI_NIC_SEL_ALG_FIRST) {
		// pick 1st active unit
		_HFI_DBG("%s: Selected 1st viable NIC unit= %u.\n",
			(nic_sel_alg == PSMI_NIC_SEL_ALG_FIRST)
				?"Packed":"Only 1 viable NIC",
			first_active);
		return first_active;
	}

	ninfo = nic_info_init(nic_info, nunits, 0);
	if (! ninfo) {
		// should not happen, already confirmed there is >1 active unit
		return -1;
	}
	switch (nic_sel_alg) {
	default:
	case PSMI_NIC_SEL_ALG_NUMA: /* round-robin is selection algorithm */
		nic_info_filter_cpu_numa(nic_info, ninfo);
		break;
	case PSMI_NIC_SEL_ALG_ANY:
		// we will use any active unit
		_HFI_DBG("No further NIC filtering\n");
		break;
	case PSMI_NIC_SEL_ALG_CPU_CENTRIC:
		nic_info_filter_cpu_centric(nic_info, ninfo);
		break;
#ifdef PSM_HAVE_GPU_CENTRIC_AFFINITY
	case PSMI_NIC_SEL_ALG_GPU_CENTRIC:
		nic_info_filter_gpu_centric(nic_info, ninfo);
		break;
#endif
	}

	index = psm3_open_shm_scoreboard_and_select_nic(nic_info, ninfo,
									job_key, nunits);
	psmi_assert(index >= 0 && index < ninfo);

	return nic_info[index].unit;
}


/* parse a list of NIC rails for PSM3_MULTIRAIL_MAP
 * map is in format: unit:port-addr_index,unit:port-addr_index,...;unit....
 * where :port is optional (default of 1) and unit can be name or number
 * -addr_index is also optional and defaults to "all"
 * addr_index can be an integer between 0 and PSM3_ADDR_PER_NIC-1
 * or "any" or "all".  "any" selects a single address using the hash and
 * "all" setups a rail for each address.
 * ; may separate sets of rails.  When more than 1 set is presented, the
 * map_index selects which set is used.
 * Returns:
 * 0 - successfully parsed, config_out updated
 * -1 - str empty, config_out unchanged
 * -2 - syntax error, config_out partially updated
 */
static int psm3_parse_multirail_map(const char *str, int map_index,
		size_t errstr_size, char errstr[],
		struct multirail_config *config_out)
{
	char temp[MAX_MAP_LEN+1];
	char *s;
	char *delim;
	char delim_char = '\0';
	unsigned i;
	int ret;
	int set_index = 0;

	if (!str || ! *str)
		return -1;

	strncpy(temp, str, MAX_MAP_LEN);
	if (temp[MAX_MAP_LEN-1] != 0) {
		if (errstr_size)
			snprintf(errstr, errstr_size,
				" Value too long, limit %u characters",
				MAX_MAP_LEN-1);
		return -2;
	}
	config_out->num_rails = 0;
	s = temp;
	psmi_assert(*s);
	do {
		int u;
		unsigned int p = 1;
		int skip_port = 0;
		int skip_addr_index = 0;
		long a_index = PSM3_ADDR_INDEX_ALL;

		if (! *s) {	// trailing ',' or ';' on 2nd or later loop
			if (delim_char == ';')
				set_index--;	// never started next set
			break;
		}
		if (delim_char == ';') {
			// start of a new set
			config_out->num_rails = 0;
		}
		if (config_out->num_rails >= PSMI_MAX_RAILS) {
			if (errstr_size)
				snprintf(errstr, errstr_size,
					" Value too long, limit %u rails",
					PSMI_MAX_RAILS);
			return -2;
		}

		// find end of unit field and put in \0 as needed
		delim = strpbrk(s, ":-,;");
		if (!delim || *delim == ',' || *delim == ';') {
			skip_port = 1; skip_addr_index = 1;
		} else if (*delim == '-') {
			skip_port = 1;
		}
		if (delim) {
			delim_char = *delim;
			*delim = '\0';
		} else {
			delim_char = '\0';
		}
		// parse unit
		u = psm3_sysfs_find_unit(s);
		if (u < 0) {
			if (errstr_size)
				snprintf(errstr, errstr_size,
					" Invalid unit: '%s'", s);
			return -2;
		}
		// find next field
		if (delim)
			s = delim+1;
		if (! skip_port) {
			// find end of port field and put in \0 as needed
			delim = strpbrk(s, "-,;");
			if (!delim || *delim == ',' || *delim == ';')
				skip_addr_index = 1;
			if (delim) {
				delim_char = *delim;
				*delim = '\0';
			} else {
				delim_char = '\0';
			}
			// parse port
			if (psm3_parse_str_uint(s, &p, 0, UINT_MAX) < 0) {
				if (errstr_size)
					snprintf(errstr, errstr_size,
						" Invalid port: '%s'", s);
				return -2;
			}
			// find next field
			if (delim)
				s = delim+1;
		}
		if (! skip_addr_index) {
			// find end of addr_index field and put in \0 as needed
			delim = strpbrk(s, ",;");
			if (delim) {
				delim_char = *delim;
				*delim = '\0';
			} else {
				delim_char = '\0';
			}
			// parse addr_index
			if (0 == strcmp(s, "all"))
				a_index = PSM3_ADDR_INDEX_ALL;	// we will loop below
			else if (0 == strcmp(s, "any"))
				a_index = PSM3_ADDR_INDEX_ANY;	// caller will pick
			else if (psm3_parse_str_long(s, &a_index, 0, psm3_addr_per_nic-1)) {
				if (errstr_size)
					snprintf(errstr, errstr_size,
						" Invalid addr index: '%s'", s);
				return -2;
			}
			// find next field
			if (delim)
				s = delim+1;
		}

		if (a_index == PSM3_ADDR_INDEX_ALL) { // all
			for (a_index = 0; a_index < psm3_addr_per_nic; a_index++) {
				if (config_out->num_rails >= PSMI_MAX_RAILS) {
					if (errstr_size)
						snprintf(errstr, errstr_size,
							" Limit of %u rails exceeded due to multi-addr",
							PSMI_MAX_RAILS);
					return -2;
				}
				config_out->units[config_out->num_rails] = u;
				config_out->ports[config_out->num_rails] = p;
				config_out->addr_indexes[config_out->num_rails] = a_index;
				config_out->num_rails++;
			}
		} else {
			config_out->units[config_out->num_rails] = u;
			config_out->ports[config_out->num_rails] = p;
			config_out->addr_indexes[config_out->num_rails] = a_index;
			config_out->num_rails++;
		}
		if (delim_char == ';') {
			if (set_index == map_index)
				break;	// found it, stop parsing
			set_index++;	// start of next
		}
	} while (delim);

	// if only 1 set input, we use it, otherwise must have enough sets for us
	psmi_assert(set_index >= 0);
	if (set_index > 0 && set_index != map_index) {
		if (errstr_size)
			snprintf(errstr, errstr_size,
						" Insufficient sets specified: %d need at least %d",
						set_index+1, map_index+1);
		return -2;
	}
	psmi_assert(set_index == 0 || set_index == map_index);

	// must have at least 1 rail.  Since we caught empty string above,
	// if we get here without any rails input must be something like "," or ";"
	// and we'll treat that as a syntax error
	if (! config_out->num_rails) {
		if (errstr_size)
			snprintf(errstr, errstr_size, " No rails specified");
		return -2;
	}

	// Check if any of the ports are not usable. Just use addr_index 0 for check
	for (i = 0; i < config_out->num_rails; i++) {
		_HFI_VDBG("rail %d:  %u(%s) %u\n", i,
			config_out->units[i],
			psm3_sysfs_unit_dev_name(config_out->units[i]),
			config_out->ports[i]);

		ret = psmi_hal_get_port_active(config_out->units[i],
										config_out->ports[i]);
		if (ret <= 0) {
			if (errstr_size)
				snprintf(errstr, errstr_size,
					" Unit:port: %d(%s):%d is not active.",
					config_out->units[i],
					psm3_sysfs_unit_dev_name(config_out->units[i]),
					config_out->ports[i]);
			return -2;
		}

		ret = psmi_hal_get_port_lid(config_out->units[i],
								config_out->ports[i], 0 /* addr_index*/);
		if (ret <= 0) {
			if (errstr_size)
				snprintf(errstr, errstr_size,
					" Unit:port: %d(%s):%d was filtered out, unable to use",
					config_out->units[i],
					psm3_sysfs_unit_dev_name(config_out->units[i]),
					config_out->ports[i]);
			return -2;
		}

		ret = psmi_hal_get_port_subnet(config_out->units[i],
								config_out->ports[i], 0 /* addr_index*/,
								NULL, NULL, NULL, NULL);
		if (ret == -1) {
			if (errstr_size)
				snprintf(errstr, errstr_size,
					" Couldn't get subnet for unit %d (%s):%d",
					config_out->units[i],
					psm3_sysfs_unit_dev_name(config_out->units[i]),
					config_out->ports[i]);
			return -2;
		}
	}

	// valid input
	return 0;
}

static int psm3_parse_check_multirail_map(int type,
		const union psmi_envvar_val val, void *ptr,
		size_t errstr_size, char errstr[])
{
	struct multirail_config temp;
	int map_index = *(int*)ptr;
	psmi_assert(type == PSMI_ENVVAR_TYPE_STR);
	return psm3_parse_multirail_map(val.e_str, map_index, errstr_size, errstr,
						&temp);
}

// comparison function for qsort
// Sort by subnet 1st, then by nic unit, then by addr_index.
// Nics are already numbered in alphabetic order so this effectively
// sorts by subnet, then nic name, then addr_index..
// We simply ignore the filtered field, filtered NICs will also get sorted
// but omitted from final output list by caller
static int niccmpfunc(const void *p1, const void *p2)
{
	struct nic_info *a = ((struct nic_info *) p1);
	struct nic_info *b = ((struct nic_info *) p2);
	int ret;

	ret = psmi_subnet128_cmp(a->subnet, b->subnet);
	if (ret == 0) {
		if (a->unit < b->unit)
			return -1;
		else if (a->unit > b->unit)
			return 1;

		if (a->addr_index < b->addr_index)
			return -1;
		else if (a->addr_index > b->addr_index)
			return 1;
	}
	return ret;
}

/*
 * Sort all the ports within nic_info from small to big.
 * So, when there are multiple fabrics, and we will use fabric with the
 * smallest subnet to make the master connection.
 */
static void
psm3_copy_nic_info_to_multitrail_config(
							struct nic_info *nic_info, unsigned ninfo,
							struct multirail_config *multirail_config)
{
	unsigned i, j;

	qsort(nic_info, ninfo, sizeof(nic_info[0]), niccmpfunc);

	multirail_config->num_rails = 0;
	j = 0;
	for (i = 0; i < ninfo; i++) {
		if (nic_info[i].filtered)
			continue;
		multirail_config->units[j] = nic_info[i].unit;
		multirail_config->ports[j] = nic_info[i].port;
		multirail_config->addr_indexes[j] = nic_info[i].addr_index;
		multirail_config->num_rails++;
		j++;
	}
}

// Multirail enabled, see if PSM3_MULTIRAIL_MAP is selecting NICs
// for PSM3_MULTIRAIL=1 or 2, PSM3_MULTIRAIL_MAP can explicitly select NICs.
// returns:
//   PSM2_OK - PSM3_MULTIRAIL_MAP specified and valid, multirail_config updated
//   PSM2_EP_NO_DEVICE - PSM3_MULTIRAIL_MAP not specified or invalid
static psm2_error_t
psm3_ep_multirail_map(int multirail_mode,
							struct multirail_config *multirail_config)
{
	int ret;
	union psmi_envvar_val env_multirail_map;
	int map_index;

	// PSM3_MUTLIRAIL_MAP only allowed for PSM3_MULTIRAIL=1 or 2
	// We treat invalid input, such as bad syntax or selection of an unusable
	// port (down/missing/etc), as a fatal error instead of attempting to run
	// on the default PSM3_MULTIRAIL_MAP config.  This helps avoid
	// inconsistent NIC selections, especially for down ports, which may
	// cause confusing behaviors or errors.
	// If PSM3_MULTIRAIL_MAP contains multiple lists of NICs, then
	// if PSM3_MULTIRAIL=1 - use local rank index (0, ...) to select
	// if PSM3_MULTIRAIL=2 - use process NUMA (0, ...) to select
	if (multirail_mode == 1) {
		map_index = psm3_get_mylocalrank();
	} else if (multirail_mode == 2) {
		map_index = psm3_get_current_proc_location();
		if (map_index < 0) {
			return psm3_handle_error(PSMI_EP_NORETURN,
				PSM2_EP_DEVICE_FAILURE,
				"Unable to get NUMA location of current process\n");
		}
	} else {
		return PSM2_EP_NO_DEVICE;	// caller will ignore MULTIRAIL_MAP
	}
	ret = psm3_getenv_range("PSM3_MULTIRAIL_MAP",
		"Explicit NIC selections for each rail",
		"Specified as:\n"
		"     rail,rail,...;rail,rail,...\n"
#if 0
		"Where rail can be: unit:port-addr_index or unit\n"
#else
		"Where rail can be: unit-addr_index or unit\n"
#endif
		"unit can be device name or unit number\n"
#if 0
		"where :port is optional (default of 1)\n"
#endif
		"addr_index can be 0 to PSM3_ADDR_PER_NIC-1, or 'any' or 'all'\n"
		"When addr_index is omitted, it defaults to 'all'\n"
		"When more than 1 set of rails is present (each set is separated by ;),\n"
		"the set to use for a given process is selected based on PSM3_MULTIRAIL.\n"
		"    1 - use local rank number to select\n"
		"    2 - use local CPU NUMA to select\n"
		"When empty, PSM3 will autoselect NICs as controlled by PSM3_MULTIRAIL.",
			PSMI_ENVVAR_LEVEL_USER|PSMI_ENVVAR_FLAG_FATAL, PSMI_ENVVAR_TYPE_STR,
			(union psmi_envvar_val)"",
			(union psmi_envvar_val)NULL, (union psmi_envvar_val)NULL,
			psm3_parse_check_multirail_map, &map_index, &env_multirail_map);
	if (ret < 0) {	// syntax error in input, ret error instead of using default
		psmi_assert(0); // should not get here since specified FLAG_FATAL
		multirail_config->num_rails = 0;
		return psm3_handle_error(PSMI_EP_NORETURN,
				PSM2_EP_DEVICE_FAILURE,
				"Invalid value for PSM3_MULTIRAIL_MAP: '%s', can't proceed\n",
				env_multirail_map.e_str);
	}
	if (! ret) {
		// valid input
		if (psm3_parse_multirail_map(env_multirail_map.e_str, map_index, 0, NULL,
			multirail_config) < 0) {
			// already checked, shouldn't get parse errors nor empty strings
			psmi_assert(0);
		}
		return PSM2_OK;
	}
	return PSM2_EP_NO_DEVICE;
}

// Multirail enabled, autoselect one or more NICs for this process
// multirail_mode is PSM3_MULTIRAIL selection
// (1=all NICs, 2=NUMA local NICs, 3=cpu centric, 4=gpu centric)
static psm2_error_t
psm3_ep_multirail_autoselect(int multirail_mode,
							struct multirail_config *multirail_config)
{
	uint32_t num_units = 0;
	psm2_error_t err = PSM2_OK;
	unsigned ninfo;
	struct nic_info nic_info[PSMI_MAX_RAILS];

	if ((err = psm3_ep_num_devunits(&num_units))) {
		return err;
	}

	if (num_units > PSMI_MAX_RAILS) {
		_HFI_INFO
		    ("Found %d units, max %d units are supported, using first %d\n",
		     num_units, PSMI_MAX_RAILS, PSMI_MAX_RAILS);
		num_units = PSMI_MAX_RAILS;
	}

	// enumerate addr_index too
	ninfo = nic_info_init(nic_info, num_units, 1);
	if (! ninfo) {
		// caller will try single NIC selection next
		multirail_config->num_rails = 0;
		return PSM2_OK;
	}

	switch (multirail_mode) {
	default:
	case 1:
		// we will use all active units
		_HFI_DBG("No further NIC filtering\n");
		break;
	case 2:
		nic_info_filter_cpu_numa(nic_info, ninfo);
		break;
	case 3:
		nic_info_filter_cpu_centric(nic_info, ninfo);
		break;
#ifdef PSM_HAVE_GPU_CENTRIC_AFFINITY
	case 4:
		nic_info_filter_gpu_centric(nic_info, ninfo);
		break;
#endif /* PSM_HAVE_GPU_CENTRIC_AFFINITY */
	}

	// we will use all unfiltered units

	// ensure psm3_context_set_affinity doesn't unnecessarily narrow CPU
	// selection, it will be called per rail and if rails are in
	// different CPU NUMA could have an undesired impact
	setenv("PSM3_NO_AFFINITY", "1", 1);

	psm3_copy_nic_info_to_multitrail_config(nic_info, ninfo, multirail_config);
	return PSM2_OK;
}

// process PSM3_MULTIRAIL and PSM3_MULTIRAIL_MAP and return the
// list of unit/port/addr_index in multirail_config.
// When multirail_config->num_rails is returned as 0, multirail is not enabled
// and other mechanisms (PSM3_NIC, PSM3_NIC_SELECTION_ALG) must be
// used by the caller to select a single NIC for the process
// via psm3_autoselect_one().
// This can return num_rails==1 if exactly 1 NIC is to be used by this process
// or num_rails>1 if this process is to stripe data across multiple NICs
// in which case the 1st NIC in multirail_config should be used as the
// primary NIC for job communications setup.
psm2_error_t
psm3_ep_multirail(struct multirail_config *multirail_config)
{
	union psmi_envvar_val env_multirail;

	psm3_getenv_range("PSM3_MULTIRAIL",
			"Control use of multiple NICs",
			"-1: No PSM3 NIC autoselection (middleware selects 1 NIC per process).\n"
			" 0: (default) Middleware may select NICs or use PSM3 'autoselect_one'\n"
			"    interface. 'autoselect_one' interface will pick 1 NIC per process\n"
			"    based on PSM3_NIC_SELECTION_ALG.\n"
			" 1: Enable multirail, each process uses all available NICs. Only 'autoselect'\n"
			"    interface presented to middleware.\n"
			" 2: Enable multirail, each process uses all NUMA local NICs. Only 'autoselect'\n"
			"    interface presented to middleware. If no NUMA local NICs found for a given\n"
			"    process, PSM3 will use all available NICs for that process.\n"
			" 3: Enable multirail, each process selects only ideally located NICs with\n"
#ifdef PSM_HAVE_GPU_CENTRIC_AFFINITY
			"    consideration of NIC, CPU"
#ifdef PSM3_HAVE_CPU_SUBNUMA
									" sub-NUMA"
#endif
									" and GPU locations with priority given\n"
			"    to CPU locality. Only 'autoselect' interface presented to middleware.\n"
			"    If no NUMA local NICs are found for a given process and all NICs are equal\n"
			"    distance to the GPU, PSM3 will use all available NICs for that process.\n"
#else
			"    consideration of NIC and CPU"
#ifdef PSM3_HAVE_CPU_SUBNUMA
											"  sub-NUMA"
#endif
											" locations.\n"
			"    Only 'autoselect' interface presented to middleware.\n"
			"    If no NUMA local NICs are found for a given process, PSM3 will use all\n"
			"    available NICs for that process.\n"
#endif
#ifdef PSM_HAVE_GPU_CENTRIC_AFFINITY
			" 4: Enable multirail, each process selects only ideally located NICs with\n"
			"    consideration of NIC, GPU, and CPU"
#ifdef PSM3_HAVE_CPU_SUBNUMA
											" sub-NUMA"
#endif
											" locations with priority given\n"
			"    to GPU locality. Only 'autoselect' interface presented to middleware.\n"
			"    If no NUMA local NICs are found for a given process, PSM3 will use all\n"
			"    available NICs of equal distance to the GPU for that process."
#endif
			,
			PSMI_ENVVAR_LEVEL_USER, PSMI_ENVVAR_TYPE_INT,
			(union psmi_envvar_val)0,
#ifdef PSM_HAVE_GPU_CENTRIC_AFFINITY
			(union psmi_envvar_val)-1, (union psmi_envvar_val)4,
#else
			(union psmi_envvar_val)-1, (union psmi_envvar_val)3,
#endif
			NULL, NULL, &env_multirail);
	if (env_multirail.e_int <= 0) {
		// will pick 1 NIC per process
		multirail_config->num_rails = 0;
		return PSM2_OK;
	}

	// see if PSM3_MULTIRAIL_MAP is manually selecting NICs
	if (psm3_ep_multirail_map(env_multirail.e_int, multirail_config) == PSM2_OK)
		return PSM2_OK;

	// multirail enabled, automatically select 1 or more NICs
	return psm3_ep_multirail_autoselect(env_multirail.e_int, multirail_config);
}

// potential job start hwloc initialization.  To avoid overhead
// when hwloc is not needed, we defer to the 1st actual need for hwloc
void
psm3_hwloc_topology_init()
{
}

#if defined(PSM_HAVE_GPU_CENTRIC_AFFINITY) || defined(PSM3_HAVE_CPU_SUBNUMA)
// deferred hwloc initialization.  Caller must hold psm3_creation_lock
static void psm3_deferred_hwloc_topology_init()
{
	unsigned version;
	Dl_info info_hwloc;
	const char *location;

	// only try once
	if (psm3_hwloc_topology_initialized || psm3_hwloc_topology_init_failed)
		return;

#define SHOW_HWLOC_VERSION(ver) (ver)>>16, ((ver) >> 8) & 0xff, (ver) & 0xff
	version = hwloc_get_api_version();
	location = dladdr(hwloc_topology_init, &info_hwloc) ?
					 info_hwloc.dli_fname : "hwloc path not available";
	if ((version >> 16) != (HWLOC_API_VERSION >> 16)) {
		_HFI_ERROR("PSM3 was compiled for hwloc API %u.%u.%u but found library API %u.%u.%u at %s.\n"
		   "You may need to point LD_LIBRARY_PATH to the right hwloc library.\n"
		   "Disabling some NIC selection affinity features\n",
		   SHOW_HWLOC_VERSION(HWLOC_API_VERSION), SHOW_HWLOC_VERSION(version),
		   location);
		psm3_hwloc_topology_init_failed = 1;
		return;
	}
	// HWLOC_VERSION string mentioned in docs, but not defined in headers
	psm3_print_identify("%s %s hwloc runtime API %u.%u.%u at %s, built against API %u.%u.%u\n",
			psm3_get_mylabel(), psm3_ident_tag,
			SHOW_HWLOC_VERSION(version), location,
			SHOW_HWLOC_VERSION(HWLOC_API_VERSION));

	hwloc_topology_init(&psm3_hwloc_topology);
	// detection configuration, need all PCI devices and CPU sub-numa
	// HWLOC_API_VERSION is rev X.Y.Z as (X<<16)+(Y<<8)+Z
	// significant API changes from 1.0 to 2.0, including ABI changes
#if HWLOC_API_VERSION < 0x20000
	hwloc_topology_set_flags(psm3_hwloc_topology,
		HWLOC_TOPOLOGY_FLAG_IO_DEVICES|HWLOC_TOPOLOGY_FLAG_IO_BRIDGES);
#else
	hwloc_topology_set_io_types_filter(psm3_hwloc_topology,
		HWLOC_TYPE_FILTER_KEEP_ALL);
#endif
	hwloc_topology_load(psm3_hwloc_topology);
	psm3_hwloc_topology_initialized = 1;
}
#endif /* defined(PSM_HAVE_GPU_CENTRIC_AFFINITY) || defined(PSM3_HAVE_CPU_SUBNUMA) */

void
psm3_hwloc_topology_destroy()
{
#if defined(PSM_HAVE_GPU_CENTRIC_AFFINITY) || defined(PSM3_HAVE_CPU_SUBNUMA)
	if (psm3_hwloc_topology_initialized) {
		psm3_hwloc_topology_initialized = 0;
		hwloc_topology_destroy(psm3_hwloc_topology);
	}
#endif
}

#if defined(PSM_HAVE_GPU_CENTRIC_AFFINITY) || defined(PSM3_HAVE_CPU_SUBNUMA)
/* Get the next PCI device in the system.
 *
 * return the first PCI device if prev is NULL.
 * looping on this allows iterating through all PCIe devices
 * device=any PCIe component (root controller, bridge, switch, device, etc)
 */
static inline hwloc_obj_t
get_next_pcidev(hwloc_topology_t topology, hwloc_obj_t prev)
{
  return hwloc_get_next_obj_by_type(topology, HWLOC_OBJ_PCI_DEVICE, prev);
}

/* Find the PCI device hwloc object matching the PCI bus id
 * given domain, bus, device and func PCI bus id.
 */
static hwloc_obj_t
get_pcidev_by_busid(hwloc_topology_t topology,
						const struct pci_addr *addr)
{
  hwloc_obj_t obj = NULL;
  while ((obj = get_next_pcidev(topology, obj)) != NULL) {
    if (obj->attr->pcidev.domain == addr->domain
        && obj->attr->pcidev.bus == addr->bus
        && obj->attr->pcidev.dev == addr->dev
        && obj->attr->pcidev.func == addr->func)
      return obj;
  }
  return NULL;
}
#endif /* defined(PSM_HAVE_GPU_CENTRIC_AFFINITY) || defined(PSM3_HAVE_CPU_SUBNUMA) */

#ifdef PSM_HAVE_GPU_CENTRIC_AFFINITY
// compare two hwloc objects for equality
// 1 on match, 0 on mismatch
static int equal_hwlocobj(const hwloc_obj_t obj1, const hwloc_obj_t obj2)
{
	return (obj1->type == obj2->type
			&& obj1->depth == obj2->depth
			&& obj1->logical_index == obj2->logical_index);
}

// compute distance in between objects (PCIe devices).
// If the devices are on different PCIe controllers and/or different CPU sockets
// returns INT_MAX
static int get_distance_to_common_ancestor(const hwloc_obj_t obj1, const hwloc_obj_t obj2)
{
    int d1 = 0;
    int d2 = 0;
    hwloc_obj_t temp1 = obj1;

    while (temp1) {

        hwloc_obj_t temp2 = obj2;
        d2 = 0;

        while (temp2) {

            /* common ancestor found */
            if (equal_hwlocobj(temp1, temp2)) {
                return d1 + d2;
            }
            temp2 = temp2->parent;
            d2++;
        }
        temp1 = temp1->parent;
        d1++;
    }

    /* No common ancestor found, return INT_MAX as the distance */
    return INT_MAX;
}

// compute distance in PCIe hops between devices.  If the
// If the devices are on different PCIe controllers and/or different CPU sockets
// returns INT_MAX
static int psm3_get_distance_between_pcis(const struct pci_addr *pci_addr_1,
                                          const struct pci_addr *pci_addr_2)
{
    hwloc_obj_t obj1 = get_pcidev_by_busid(psm3_hwloc_topology, pci_addr_1);
    hwloc_obj_t obj2 = get_pcidev_by_busid(psm3_hwloc_topology, pci_addr_2);
    return get_distance_to_common_ancestor(obj1, obj2);
}
#endif /* PSM_HAVE_GPU_CENTRIC_AFFINITY */

#ifdef PSM3_HAVE_CPU_SUBNUMA
// find ancestor of a device, namely the PCIe controller in the CPU socket
static hwloc_obj_t psm3_get_non_io_ancestor_obj(
										const struct pci_addr *pci_addr)
{
	hwloc_obj_t obj = get_pcidev_by_busid(psm3_hwloc_topology, pci_addr);
	if (! obj)
		return NULL;
	return hwloc_get_non_io_ancestor_obj(psm3_hwloc_topology, obj);
}
#endif /* PSM3_HAVE_CPU_SUBNUMA */
