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

#include "fi_bgq_spi.h"

#include "l2atomic.h"

#define ITERATIONS 100000


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

	L2_Lock_t * lock;
	rc = l2atomic_lock_alloc_generic (&l2atomic, &lock, "lock"); lineno = __LINE__;
	if (rc) goto err;


	uint64_t * buffer = NULL;
	rc = l2atomic_alloc(&l2atomic, "simple", 128, (void**)&buffer, test_init_fn, 0); lineno = __LINE__;
	if (rc) goto err;

	/* race condition */
	usleep(5000);
	ppc_msync();
	int participants = l2atomic.shared->counter;
	/* race condition */

	struct l2atomic_barrier barrier;
	rc = l2atomic_barrier_alloc_generic(&l2atomic, &barrier, participants, "lock_barrier_test"); lineno = __LINE__;
	if (rc) goto err;
	l2atomic_barrier(&barrier);

	uint32_t i;
	for (i=0; i<ITERATIONS; i++) {
		L2_LockAcquire(lock);
		*buffer += (tcoord+1);
		L2_LockRelease(lock);
	}

	l2atomic_barrier(&barrier);

	uint64_t expected = (uint64_t)((participants+1) * ((double)participants/2.0)) * ITERATIONS;
	if (expected != *buffer) {
		if (tcoord==0) fprintf (stderr, "test failed. expected %lu, actual %lu\n", expected, *buffer);
		return 1;
	}


	if (tcoord==0) fprintf (stdout, "TEST SUCCESSFUL\n");
	return 0;
err:
	fprintf (stderr, "Error at line %d\n", lineno);
	return 1;
}
