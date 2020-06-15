/*
 * Copyright (C) 2016 by Argonne National Laboratory.
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

#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "rdma/bgq/fi_bgq_hwi.h"
#include "rdma/bgq/fi_bgq_spi.h"

#include "rdma/bgq/fi_bgq_mu.h"
#include "rdma/bgq/fi_bgq_node.h"

uint64_t fi_bgq_node_bat_allocate_id (struct fi_bgq_node * node, struct l2atomic_lock * lock, uint64_t index);

#define FI_BGQ_NODE_COUNTER_SIZE ((1 << 13))	/* 8192 counters per node */
#define FI_BGQ_NODE_LOCK_SIZE ((1 << 13))	/* 8192 locks per node */

enum bat_variable {
	BAT_VARIABLE_GLOBAL = 0,
	BAT_VARIABLE_COUNTER,
	BAT_VARIABLE_ONE,
	BAT_VARIABLE_ZERO,
	BAT_VARIABLE_BLACKHOLE,
	BAT_VARIABLE_NUM
};

struct fi_bgq_node_shared {
	volatile uint64_t	init_counter;
	volatile uint64_t	is_initialized;
	uint64_t		pad;
	volatile uint64_t	global_variables[BAT_VARIABLE_NUM];
	struct {
		struct l2atomic_lock_data	lock_data;
	} mu;
	struct {
		struct l2atomic_counter_data	allocator_data;
		struct l2atomic_counter_data	counter_data[FI_BGQ_NODE_COUNTER_SIZE];
	} counter;
	struct {
		struct l2atomic_counter_data	allocator_data;
		struct l2atomic_lock_data	lock_data[FI_BGQ_NODE_LOCK_SIZE];
	} lock;
	struct l2atomic_barrier_data		barrier_data;
	uint32_t				leader_tcoord;
	volatile uint64_t	bat_shadow[FI_BGQ_NODE_BAT_SIZE];
	volatile uint64_t	bat_cntr[FI_BGQ_NODE_APPLICATION_BAT_SIZE];
};


#define FI_BGQ_NODE_SHM_FILENAME "/fi_bgq_node"
#define FI_BGQ_NODE_SHM_FILESIZE (sizeof(struct fi_bgq_node_shared) + L2_CACHE_LINE_SIZE)

void calculate_local_process_count (uint64_t * local_process_count, uint32_t * leader_tcoord) {

	int cnk_rc __attribute__ ((unused));

	Personality_t personality;
	cnk_rc = Kernel_GetPersonality(&personality, sizeof(Personality_t));
	assert(cnk_rc==0);

	BG_CoordinateMapping_t local_coords;
	local_coords.a = personality.Network_Config.Acoord;
	local_coords.b = personality.Network_Config.Bcoord;
	local_coords.c = personality.Network_Config.Ccoord;
	local_coords.d = personality.Network_Config.Dcoord;
	local_coords.e = personality.Network_Config.Ecoord;
	local_coords.t = 0;
	local_coords.reserved = 0;
	const uint32_t * const local_coords_uint32 = (uint32_t *)&local_coords;

	size_t node_count = personality.Network_Config.Anodes *
		personality.Network_Config.Bnodes *
		personality.Network_Config.Cnodes *
		personality.Network_Config.Dnodes *
		personality.Network_Config.Enodes;

	uint32_t ppn = Kernel_ProcessCount();

	/*
	 * read the ranks2coords mapping on to the stack
	 */
	size_t mapsize = node_count * ppn;
	BG_CoordinateMapping_t map[mapsize];
	uint64_t numentries = 0;
	cnk_rc = Kernel_RanksToCoords(mapsize*sizeof(BG_CoordinateMapping_t), map, &numentries);
	assert(cnk_rc==0);

	/*
	 * scan the mapping for all ranks on the local node
	 *
	 * the last rank encountered, the highest global rank, will be the
	 * node "leader" regardless of its t coordinate
	 *
	 * calculate the number of active processes on the local node
	 */
	*local_process_count = 0;
	uint64_t n;
	const uint32_t * const map_uint32 = (uint32_t *)map;
	for (n = 0; n < numentries; ++n) {
		const uint32_t bg_coordinatemapping = map_uint32[n];
		if ((bg_coordinatemapping & 0xBFFFFFC0) == *local_coords_uint32) {
			*local_process_count += 1;
			*leader_tcoord = map[n].t;
		}
	}
}

int fi_bgq_node_init (struct fi_bgq_node * node) {

	/* open and create the shared memory segment */
	int _fd = -1;
	_fd = shm_open(FI_BGQ_NODE_SHM_FILENAME, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
	if (_fd == -1) goto err;

	/* set this shared memory as l2 atomic */
	uint64_t foo=1;
	int rc;
	rc = ioctl(_fd, FIOBGQATOMIC, &foo);
	if (rc) goto err;

	/* set the size of the shared memory segment */
	size_t nbytes = FI_BGQ_NODE_SHM_FILESIZE;
	if (ftruncate(_fd, nbytes) == -1) goto err;

	/* map the shared memory segment and get the virtual address */
	void * _ptr = MAP_FAILED;
	_ptr = mmap(NULL, nbytes, PROT_READ | PROT_WRITE, MAP_SHARED, _fd, 0);
	if (_ptr == MAP_FAILED) goto err;

	/* align to L2 cache */
	node->abs_ptr = _ptr;
	_ptr = (void *)(((uint64_t)_ptr+L2_CACHE_LINE_SIZE) & ~(L2_CACHE_LINE_SIZE-1));
	node->shm_ptr = _ptr;

	struct fi_bgq_node_shared * shared = (struct fi_bgq_node_shared *) node->shm_ptr;

	uint32_t cnk_rc __attribute__ ((unused));
	cnk_rc = Kernel_L2AtomicsAllocate((void *)shared, FI_BGQ_NODE_SHM_FILESIZE);
	assert(cnk_rc==0);

	uint64_t value = L2_AtomicLoadIncrement(&shared->init_counter);
	if (value == 0) {

		/* initialize the mu lock */
		L2_AtomicStore(&shared->mu.lock_data.ticket, 0);
		L2_AtomicStore(&shared->mu.lock_data.serving, 0);

		/* initialize the counter allocater .. er .. counter */
		l2atomic_counter_initialize(&node->counter.allocator, &shared->counter.allocator_data);

		/* initialize the lock allocator counter */
		l2atomic_counter_initialize(&node->lock.allocator, &shared->lock.allocator_data);

		uint64_t local_process_count = 0;
		shared->leader_tcoord = (uint32_t)-1;
		calculate_local_process_count(&local_process_count, &shared->leader_tcoord);

		l2atomic_barrier_initialize(&node->barrier, &shared->barrier_data, local_process_count);

#ifdef TODO
		/* verify that the MU and ND are not in reset. they must be out
		 * of reset in order to set up the MU resources or a machine
		 * check will occur - this would impact a future job using ofi-bgq. */
		uint64_t val1, val2, val3;
		val1 = DCRReadUser(MU_DCR(RESET));
		val2 = DCRReadUser(ND_X2_DCR(RESET));
		val3 = DCRReadUser(ND_500_DCR(RESET));
		if ((MU_DCR__RESET__DCRS_OUT_get(val1)) ||
			(ND_X2_DCR__RESET__DCRS_OUT_get(val2)) ||
			(ND_500_DCR__RESET__DCRS_OUT_get(val3))) {
			assert(0);
		}
#endif

		/* Initialize the Base Address Table shadow */
		{
			node->bat.shadow = &shared->bat_shadow[0];
			unsigned index;
			for (index=0; index<FI_BGQ_NODE_BAT_SIZE; ++index) {
				node->bat.shadow[index] = 0xFFFFFFFFFFFFFFFFull;
			}
		}

		/*
		 * Initialize the FI_BGQ_MU_BAT_ID_GLOBAL entry to the base
		 * physical address 0x00 which will be used by MU operations
		 * that specify the actual physical addresss
		 *
		 * Initialize the FI_BGQ_MU_BAT_ID_COUNTER entry to a global
		 * variable which will be used by MU "direct put" operations
		 * that choose to disregard reception counter completions.
		 *
		 * Initialize the FI_BGQ_MU_BAT_ID_ZERO entry to a global
		 * variable which is set to the constant value 'zero'.
		 *
		 * Initialize the FI_BGQ_MU_BAT_ID_ONE entry to a global
		 * variable which is set to the constant value 'one'.
		 *
		 * Initialize the FI_BGQ_MU_BAT_ID_BLACKHOLE entry to a global
		 * variable which is used as a 'garbage' location to write
		 * data that is to be ignored.
		 */
		uint64_t rc __attribute__ ((unused));

		rc = fi_bgq_node_bat_allocate_id(node, NULL, FI_BGQ_MU_BAT_ID_GLOBAL);
		assert(rc == 0);
		fi_bgq_node_bat_write(node, NULL, FI_BGQ_MU_BAT_ID_GLOBAL, 0);

		rc = fi_bgq_node_bat_allocate_id(node, NULL, FI_BGQ_MU_BAT_ID_COUNTER);
		assert(rc == 0);

		rc = fi_bgq_node_bat_allocate_id(node, NULL, FI_BGQ_MU_BAT_ID_ZERO);
		assert(rc == 0);

		rc = fi_bgq_node_bat_allocate_id(node, NULL, FI_BGQ_MU_BAT_ID_ONE);
		assert(rc == 0);

		rc = fi_bgq_node_bat_allocate_id(node, NULL, FI_BGQ_MU_BAT_ID_BLACKHOLE);
		assert(rc == 0);

		uint64_t base_paddr = (uint64_t)-1;
		{
			void * vaddr = (void *)shared->global_variables;

			Kernel_MemoryRegion_t cnk_mr;
			Kernel_CreateMemoryRegion(&cnk_mr, vaddr, sizeof(uint64_t)*5);
			uint64_t offset = (uint64_t)vaddr - (uint64_t)cnk_mr.BaseVa;
			base_paddr = (uint64_t)cnk_mr.BasePa + offset;
		}

		shared->global_variables[BAT_VARIABLE_COUNTER] = 0;
		uint64_t mu_atomic_paddr =
			MUSPI_GetAtomicAddress(base_paddr + sizeof(uint64_t) * BAT_VARIABLE_COUNTER,
				MUHWI_ATOMIC_OPCODE_STORE_ADD);
		fi_bgq_node_bat_write(node, NULL, FI_BGQ_MU_BAT_ID_COUNTER, mu_atomic_paddr);

		shared->global_variables[BAT_VARIABLE_ZERO] = 0;
		fi_bgq_node_bat_write(node, NULL, FI_BGQ_MU_BAT_ID_ZERO,
			base_paddr + sizeof(uint64_t) * BAT_VARIABLE_ZERO);

		shared->global_variables[BAT_VARIABLE_ONE] = 1;
		fi_bgq_node_bat_write(node, NULL, FI_BGQ_MU_BAT_ID_ONE,
			base_paddr + sizeof(uint64_t) * BAT_VARIABLE_ONE);

		shared->global_variables[BAT_VARIABLE_BLACKHOLE] = 0;
		fi_bgq_node_bat_write(node, NULL, FI_BGQ_MU_BAT_ID_BLACKHOLE,
			base_paddr + sizeof(uint64_t) * BAT_VARIABLE_BLACKHOLE);


		/* finally, update the shared state to "initialized" */
		L2_AtomicStore(&shared->is_initialized, 1);

	} else {
		/* all other processes will wait until the first process
		 * updates the shared state to "initialized" */
		while (L2_AtomicLoad(&shared->is_initialized) == 0) {
			usleep(1);
		}

		/* clone the counter allocator counter ... <sigh> */
		node->counter.allocator.value_l2vaddr = (uintptr_t)&shared->counter.allocator_data.value;

		/* clone the lock allocator counter */
		node->lock.allocator.value_l2vaddr = (uintptr_t)&shared->lock.allocator_data.value;

		/* set the pointer to the shared base address table shadow */
		node->bat.shadow = &shared->bat_shadow[0];

		l2atomic_barrier_clone(&node->barrier, &shared->barrier_data);
	}

	node->leader_tcoord = shared->leader_tcoord;
	node->is_leader = node->leader_tcoord == Kernel_MyTcoord();

	/* get the paddr of the bat counters in l2 atomic shared memory */
	void * vaddr = (void *)&shared->bat_cntr[0];
	Kernel_MemoryRegion_t cnk_mr;
	Kernel_CreateMemoryRegion(&cnk_mr, vaddr, sizeof(uint64_t)*FI_BGQ_NODE_APPLICATION_BAT_SIZE);
	uint64_t offset = (uint64_t)vaddr - (uint64_t)cnk_mr.BaseVa;
	uint64_t paddr = (uint64_t)cnk_mr.BasePa + offset;
	unsigned i;
	for (i=0; i<FI_BGQ_NODE_APPLICATION_BAT_SIZE; ++i) {
		node->bat.l2_cntr_paddr[i] = paddr + sizeof(uint64_t)*i;
	}

	l2atomic_barrier_enter(&node->barrier);

	return 0;
err:
	if (_fd != -1) close(_fd);
	return -errno;
}

int fi_bgq_node_mu_lock_init (struct fi_bgq_node * node, struct l2atomic_lock * lock) {

	struct fi_bgq_node_shared * shared = (struct fi_bgq_node_shared *) node->shm_ptr;

	/* do not use 'l2atomic_lock_initialize()' because it clears the
	 * 'ticket' and 'serving' values each time */
	lock->ticket_l2vaddr = (uintptr_t)&shared->mu.lock_data.ticket;
	lock->serving_l2vaddr = (uintptr_t)&shared->mu.lock_data.serving;

	return 0;
}

int fi_bgq_node_counter_allocate (struct fi_bgq_node * node, struct l2atomic_counter * counter) {

	uint64_t index = l2atomic_counter_increment(&node->counter.allocator);
	if (index == 0x8000000000000000ull)
		return -1;

	struct fi_bgq_node_shared * shared = (struct fi_bgq_node_shared *) node->shm_ptr;

	counter->value_l2vaddr = (uintptr_t)&shared->counter.counter_data[index].value;
	l2atomic_counter_set(counter, 0);

	return 0;
}

int fi_bgq_node_lock_allocate (struct fi_bgq_node * node, struct l2atomic_lock * lock) {

	uint64_t index = l2atomic_counter_increment(&node->lock.allocator);
	if (index == 0x8000000000000000ull)
		return -1;

	struct fi_bgq_node_shared * shared = (struct fi_bgq_node_shared *) node->shm_ptr;
	l2atomic_lock_initialize(lock, &shared->lock.lock_data[index]);

	return 0;
}


void fi_bgq_node_bat_write (struct fi_bgq_node * node, struct l2atomic_lock * lock, uint64_t index, uint64_t offset) {

	assert(index < FI_BGQ_NODE_BAT_SIZE);

	uint32_t requested_bat_id = index & 0x07;

	if (lock) l2atomic_lock_acquire(lock);

	int32_t cnk_rc __attribute__ ((unused));
	cnk_rc = MUSPI_SetBaseAddress(&node->bat.subgroup[index], requested_bat_id, offset);
	assert(cnk_rc == 0);

	node->bat.shadow[index] = offset;

	{	/* this "l1p flush" hack is only needed to flush *writes* from a processor cache to the memory system */
		volatile uint64_t *mu_register =
			(volatile uint64_t *)(BGQ_MU_STATUS_CONTROL_REGS_START_OFFSET(0, 0) +
			0x030 - PHYMAP_PRIVILEGEDOFFSET);
		*mu_register = 0;
	}
	ppc_msync();

	if (lock) l2atomic_lock_release(lock);
}

void fi_bgq_node_bat_clear (struct fi_bgq_node * node, struct l2atomic_lock * lock, uint64_t index) {
	fi_bgq_node_bat_write(node, lock, index, 0xFFFFFFFFFFFFFFFFull);
}


uint64_t fi_bgq_node_bat_allocate (struct fi_bgq_node * node, struct l2atomic_lock * lock) {

	if (lock) l2atomic_lock_acquire(lock);

	uint32_t subgroup_id;
	for (subgroup_id = 0; subgroup_id < FI_BGQ_NODE_NUM_USER_SUBGROUPS; ++subgroup_id) {

		uint32_t nbatids;
		uint32_t batids[BGQ_MU_NUM_DATA_COUNTERS_PER_SUBGROUP];
		int32_t cnk_rc __attribute__ ((unused));
		cnk_rc = Kernel_QueryBaseAddressTable(subgroup_id, &nbatids, batids);
		assert(cnk_rc == 0);

		if (nbatids > 0) {

			uint64_t index = (subgroup_id << 3) | batids[0];

			cnk_rc = Kernel_AllocateBaseAddressTable(subgroup_id,
				&node->bat.subgroup[index], 1, &batids[0], 0);
			assert(cnk_rc == 0);

			uint64_t bat_offset __attribute__ ((unused));
			bat_offset = fi_bgq_node_bat_read(node, index);
			assert(bat_offset == 0xFFFFFFFFFFFFFFFFull);

			if (lock) l2atomic_lock_release(lock);

			return index;
		}
	}

	if (lock) l2atomic_lock_release(lock);

	return 0xFFFFFFFFFFFFFFFFull;	/* error! */
}

uint64_t fi_bgq_node_bat_allocate_id (struct fi_bgq_node * node, struct l2atomic_lock * lock, uint64_t index) {

	assert(index < FI_BGQ_NODE_BAT_SIZE);

	uint32_t requested_subgroup_id = index >> 3;
	uint32_t requested_bat_id = index & 0x07;

	if (lock) l2atomic_lock_acquire(lock);

	uint32_t nbatids;
	uint32_t batids[BGQ_MU_NUM_DATA_COUNTERS_PER_SUBGROUP];
	int32_t cnk_rc __attribute__ ((unused));
	cnk_rc = Kernel_QueryBaseAddressTable(requested_subgroup_id, &nbatids, batids);
	assert(cnk_rc == 0);
	assert(nbatids > 0);

	unsigned i;
	for (i=0; i<nbatids; ++i) {
		if (batids[i] == requested_bat_id) {

			uint64_t index = (requested_subgroup_id << 3) | batids[i];

			cnk_rc = Kernel_AllocateBaseAddressTable(requested_subgroup_id,
				&node->bat.subgroup[index], 1, &batids[i], 0);
			assert(cnk_rc == 0);

			uint64_t bat_offset __attribute__ ((unused));
			bat_offset = fi_bgq_node_bat_read(node, index);
			assert(bat_offset == 0xFFFFFFFFFFFFFFFFull);

			if (lock) l2atomic_lock_release(lock);

			return 0;
		}
	}

	if (lock) l2atomic_lock_release(lock);

	return 0xFFFFFFFFFFFFFFFFull;	/* error! */
}

void fi_bgq_node_bat_free (struct fi_bgq_node * node, struct l2atomic_lock * lock, uint64_t index) {

	assert(index < FI_BGQ_NODE_APPLICATION_BAT_SIZE);

	if (lock) l2atomic_lock_acquire(lock);

	fi_bgq_node_bat_clear(node, NULL, index);

	uint32_t batid = index & 0x07;

	int32_t cnk_rc  __attribute__ ((unused));
	cnk_rc = Kernel_DeallocateBaseAddressTable(&node->bat.subgroup[index], 1, &batid);
	assert(cnk_rc == 0);

	if (lock) l2atomic_lock_release(lock);
}
