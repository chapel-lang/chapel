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

#include <unistd.h>

#include "rdma/bgq/fi_bgq_hwi.h"
#include "rdma/bgq/fi_bgq_spi.h"

#include "l2atomic.h"

#define ITERATIONS 1000000


void test_init_fn (void * buffer, uintptr_t cookie) {
	uint64_t * ptr = (uint64_t *) buffer;
	*ptr = cookie;
}

int main (int argc, char *argv[]) {

	struct l2atomic l2atomic;
	memset((void*)&l2atomic, 0, sizeof(l2atomic));

	uint32_t tcoord = Kernel_MyTcoord();

	int rc, lineno;
	rc = l2atomic_init(&l2atomic); lineno = __LINE__;
	if (rc) goto err;

	/* race condition! how to determine the number of *active* ranks on the node
	 * without using Kernel_RanksToCoords() ? */
	usleep(5000);
	ppc_msync();
	int participants = l2atomic.shared->counter;
	fprintf(stderr, "%s:%d participants=%d\n", __FILE__, __LINE__, participants);
	/* end: race */

	struct l2atomic_barrier barrier;
	rc = l2atomic_barrier_alloc_generic(&l2atomic, &barrier, participants, "barrier_test"); lineno = __LINE__;
	if (rc) goto err; 
	uint64_t start_time = GetTimeBase();
	l2atomic_barrier(&barrier);
	if (tcoord == 0) usleep(1);
	l2atomic_barrier(&barrier);
	uint64_t end_time = GetTimeBase();

	fprintf(stdout, "barrier cycles: %lu\n", end_time - start_time);

	if (tcoord==0) fprintf(stdout, "TEST SUCCESSFUL\n");
	return 0;
err:
	fprintf(stderr, "Error at line %d\n", lineno);
	return 1;
}
