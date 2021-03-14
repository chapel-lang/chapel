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

#include "rdma/bgq/fi_bgq_spi.h"

#include <stdlib.h>

/* internal function */
int fi_bgq_spi_injfifo_subgrp_init (struct fi_bgq_spi_injfifo *f,
		MUSPI_InjFifoSubGroup_t *subgrp,
		unsigned num_fifos_to_allocate,
		const size_t injfifo_size,
		const unsigned immediate_payload_sizeof,
		const unsigned is_remote_get,
		const int subgrp_id) {

	assert(num_fifos_to_allocate > 0);
	assert(is_remote_get == 0 || is_remote_get == 1);

	uint32_t available;
	uint32_t fifo_ids[BGQ_MU_NUM_INJ_FIFOS_PER_SUBGROUP];
	Kernel_MemoryRegion_t mregion;
	int rc = 0;

	memset((void*)f, 0x00, sizeof(*f));
	f->node_scoped_fifo_id = ~(0ull);

	Kernel_InjFifoAttributes_t attr;
	memset(&attr, 0x00, sizeof(attr));
	attr.RemoteGet = is_remote_get;
	attr.System = 0;

	rc = Kernel_QueryInjFifos(subgrp_id, &available, fifo_ids);
	if (rc) {
		goto err;
	}
	if (!available) {
		return 0;
	}

	uint32_t subgrp_fifo_id = fifo_ids[0];

	if ((rc = Kernel_AllocateInjFifos(subgrp_id,
			subgrp,
			1,
			&subgrp_fifo_id,
			&attr)) != 0) {
		goto err;
	}

	size_t bytes = injfifo_size * sizeof(MUHWI_Descriptor_t);
	if (posix_memalign((void**) &f->memory, 64, bytes + 64 /* force alignment */)) {
		errno = ENOMEM;
		goto err;
	}

	/* FORCE a 64-byte alignment (?!?!) */
	void * injfifo_memory = (void *)(((uintptr_t)f->memory+64) & (~63));

	if (Kernel_CreateMemoryRegion(&mregion, injfifo_memory, bytes)) {
		goto err;
	}

	if (Kernel_InjFifoInit(subgrp, subgrp_fifo_id, &mregion,
			(uint64_t)injfifo_memory - (uint64_t)mregion.BaseVa,
			bytes-1)) {
		goto err;
	}

	if (Kernel_InjFifoActivate(subgrp, 1, &subgrp_fifo_id,
			KERNEL_INJ_FIFO_ACTIVATE)) {
		goto err;
	}

	f->muspi_injfifo = MUSPI_IdToInjFifo(subgrp_fifo_id, subgrp);
	f->sw_freeSpace = &f->muspi_injfifo->freeSpace;
	f->sw_tailva = (uint64_t*)&f->muspi_injfifo->_fifo.va_tail;
	f->hw_injfifo = f->muspi_injfifo->hw_injfifo;
	f->node_scoped_fifo_id = subgrp_id * BGQ_MU_NUM_INJ_FIFO_SUBGROUPS + subgrp_fifo_id;


	if (!is_remote_get && immediate_payload_sizeof > 0) {

		f->immediate_payload_sizeof = immediate_payload_sizeof;

		bytes = injfifo_size * immediate_payload_sizeof;
		if (posix_memalign((void**) &f->immediate_payload_memory, 64, bytes + 64 /* force alignment */)) {
			errno = ENOMEM;
			goto err;
		}

		/* FORCE a 64-byte alignment (?!?!) */
		f->immediate_payload_base_vaddr = (uintptr_t)f->immediate_payload_memory & ~63ull;

		Kernel_MemoryRegion_t cnk_mr;
		uint32_t cnk_rc = 0;
		cnk_rc = Kernel_CreateMemoryRegion(&cnk_mr, (void*)f->immediate_payload_base_vaddr, bytes);
		if (cnk_rc) {
			goto err;
		}

		f->immediate_payload_base_paddr =
			(uint64_t)cnk_mr.BasePa +
			((uint64_t)f->immediate_payload_base_vaddr - (uint64_t)cnk_mr.BaseVa);

		f->va_start = (uintptr_t)f->muspi_injfifo->_fifo.va_start;

	}

	return 1;
err:

	if (f->memory) free(f->memory);
	if (f->immediate_payload_memory) free(f->immediate_payload_memory);
	memset((void*)f, 0x00, sizeof(*f));
	f->node_scoped_fifo_id = ~(0ull);
	return 0;
}

int fi_bgq_spi_injfifo_init (struct fi_bgq_spi_injfifo *f,
		MUSPI_InjFifoSubGroup_t *injfifo_subgroup,
		unsigned num_fifos_to_allocate,
		const size_t injfifo_size,
		const unsigned immediate_payload_sizeof,
		const unsigned is_remote_get,
		const unsigned is_top_down) {

	assert(num_fifos_to_allocate > 0);
	assert(is_top_down == 0 || is_top_down == 1);

	int subgrp_id;

	unsigned n, total_fifos_allocated = 0;
	if (is_top_down) {
		for (subgrp_id = BGQ_MU_NUM_FIFO_SUBGROUPS_PER_NODE-BGQ_MU_NUM_FIFO_SUBGROUPS-1; (subgrp_id >= 0) && (num_fifos_to_allocate > 0); --subgrp_id) {
			n = fi_bgq_spi_injfifo_subgrp_init(f,
				injfifo_subgroup,
				num_fifos_to_allocate,
				injfifo_size,
				immediate_payload_sizeof,
				is_remote_get,
				subgrp_id);
			num_fifos_to_allocate -= n;
			total_fifos_allocated += n;
		}
	} else {
		for (subgrp_id = 0; (subgrp_id < (BGQ_MU_NUM_FIFO_SUBGROUPS_PER_NODE-BGQ_MU_NUM_FIFO_SUBGROUPS)) && (num_fifos_to_allocate > 0); ++subgrp_id) {
			n = fi_bgq_spi_injfifo_subgrp_init(f,
				injfifo_subgroup,
				num_fifos_to_allocate,
				injfifo_size,
				immediate_payload_sizeof,
				is_remote_get,
				subgrp_id);
			num_fifos_to_allocate -= n;
			total_fifos_allocated += n;
		}
	}

	return total_fifos_allocated;
}

void fi_bgq_spi_injfifo_clone (struct fi_bgq_spi_injfifo *dst, struct fi_bgq_spi_injfifo *src) {

	assert(dst);
	assert(src);

	/* TODO - set an "is clone" variable to remeber not to free this */
	*dst = *src;
}

int fi_bgq_spi_injfifo_fini (struct fi_bgq_spi_injfifo *f) {

	/* TODO ..... */

	if (f->memory) free(f->memory);
	if (f->immediate_payload_memory) free(f->immediate_payload_memory);
	memset((void*)f, 0x00, sizeof(*f));
	f->node_scoped_fifo_id = ~(0ull);

	return 0;
}
