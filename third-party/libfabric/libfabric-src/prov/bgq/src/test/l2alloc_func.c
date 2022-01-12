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

#define ITERATIONS 1000000

void test_init_fn (void * buffer, uintptr_t cookie) {

	uint64_t * ptr = (uint64_t *) buffer;
	ptr[0] = cookie;
	ptr[1] = cookie;
	ptr[2] = cookie;
}

int main (int argc, char *argv[]) {

	struct l2atomic l2atomic;
	memset((void*)&l2atomic, 0, sizeof(l2atomic));

	uint32_t tcoord = Kernel_MyTcoord();
	int rc, lineno;

	rc = l2atomic_init(&l2atomic); lineno = __LINE__;
	if (rc) goto err;

	volatile uint64_t * buffer = NULL;
	rc = l2atomic_alloc(&l2atomic, "simple", sizeof(uint64_t)*3, (void**)&buffer, test_init_fn, 0); lineno = __LINE__;
	if (rc) goto err;

	volatile uint64_t * entered = buffer+1;
	volatile uint64_t * exited = buffer+2;
	L2_AtomicLoadIncrement(entered);

	uint32_t i;
	if (tcoord % 2 == 0) {
		for (i=0; i<ITERATIONS; ++i) L2_AtomicLoadIncrement(buffer);
	} else {
		for (i=0; i<ITERATIONS; ++i) L2_AtomicLoadDecrement(buffer);
	}

	L2_AtomicLoadIncrement(exited);
	while (L2_AtomicLoad(exited) != *entered);

	uint64_t np = *entered;
	uint64_t expected = 0;
	if (np % 2 == 1) expected = ITERATIONS;

	if (tcoord == 0) fprintf (stdout, "l2atomic value: %lu (expected %lu)\n", *buffer, expected);
	if (*buffer == expected) {
		if (tcoord == 0) fprintf (stdout, "TEST SUCCESSFUL\n");
	} else {
		if (tcoord == 0) fprintf (stdout, "TEST FAILED\n");
		return 1;
	}
	return 0;
err:
	fprintf (stderr, "Error at line %d\n", lineno);
	return 1;
}
