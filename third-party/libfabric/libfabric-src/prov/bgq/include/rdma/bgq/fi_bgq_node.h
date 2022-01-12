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
#ifndef _FI_PROV_BGQ_NODE_H_
#define _FI_PROV_BGQ_NODE_H_

#include "rdma/bgq/fi_bgq_l2atomic.h"

#include "rdma/bgq/fi_bgq_spi.h"

#define FI_BGQ_NODE_NUM_USER_SUBGROUPS (BGQ_MU_NUM_FIFO_SUBGROUPS_PER_NODE-2)					/* subgroups 66 and 67 are privileged */
#define FI_BGQ_NODE_BAT_SIZE (FI_BGQ_NODE_NUM_USER_SUBGROUPS * BGQ_MU_NUM_DATA_COUNTERS_PER_SUBGROUP)
#define FI_BGQ_NODE_APPLICATION_BAT_SIZE ((BGQ_MU_NUM_FIFO_GROUPS-1) * BGQ_MU_NUM_DATA_COUNTERS_PER_GROUP)	/* cnk and agents use group 16 */

struct fi_bgq_node {
	void *shm_ptr;
	void *abs_ptr;
	struct {
		struct l2atomic_counter	allocator;
	} counter;
	struct {
		struct l2atomic_counter	allocator;
	} lock;
	struct l2atomic_barrier		barrier;
	uint32_t			leader_tcoord;
	uint32_t			is_leader;
	struct {
		volatile uint64_t			*shadow;	/* in shared memory */
		volatile uint64_t			l2_cntr_paddr[FI_BGQ_NODE_APPLICATION_BAT_SIZE];
		MUSPI_BaseAddressTableSubGroup_t	subgroup[FI_BGQ_NODE_BAT_SIZE];
	} bat;
};

int fi_bgq_node_init (struct fi_bgq_node * node);

int fi_bgq_node_mu_lock_init (struct fi_bgq_node * node, struct l2atomic_lock * lock);

int fi_bgq_node_counter_allocate (struct fi_bgq_node * node, struct l2atomic_counter * counter);

int fi_bgq_node_lock_allocate (struct fi_bgq_node * node, struct l2atomic_lock * lock);

uint64_t fi_bgq_node_bat_allocate (struct fi_bgq_node * node, struct l2atomic_lock * lock);

void fi_bgq_node_bat_free (struct fi_bgq_node * node, struct l2atomic_lock * lock, uint64_t index);

void fi_bgq_node_bat_write (struct fi_bgq_node * node, struct l2atomic_lock * lock, uint64_t index, uint64_t offset);

void fi_bgq_node_bat_clear (struct fi_bgq_node * node, struct l2atomic_lock * lock, uint64_t index);

static inline
uint64_t fi_bgq_node_bat_read (struct fi_bgq_node * node, uint64_t index) {

	assert(index < FI_BGQ_NODE_BAT_SIZE);
	return node->bat.shadow[index];
}

#endif /* _FI_PROV_BGQ_NODE_H_ */
