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

#include "cq_agent.h"
#include "fi_bgq_memfifo.h"

#define ITERATIONS 10

void test_init_fn (void * buffer, uintptr_t cookie) {

	uint64_t * ptr = (uint64_t *) buffer;
	*ptr = cookie;
}

int main (int argc, char *argv[]) {

	struct l2atomic l2atomic;
	memset((void*)&l2atomic, 0, sizeof(l2atomic));

	int rc, lineno;
	rc = l2atomic_init(&l2atomic); lineno = __LINE__;
	if (rc) goto err;

	struct memfifo mfifo;
	rc = memfifo_initialize(&l2atomic, "some name", &mfifo, 0); lineno = __LINE__;
	if (rc) goto err;

	unsigned production_count = 0;
	while (0 == memfifo_produce16(&mfifo.producer, production_count)) production_count++;
	if (production_count != CQ_MFIFO_SIZE) { lineno = __LINE__; goto err; }

	uint16_t entry_id;
	unsigned consumption_count = 0;
	while (0 == memfifo_consume16(&mfifo.consumer, &entry_id)) {
		if (entry_id != consumption_count++) { lineno = __LINE__; goto err; }
	}
	if (consumption_count != CQ_MFIFO_SIZE) { lineno = __LINE__; goto err; }

	while (0 == memfifo_produce16(&mfifo.producer, production_count)) production_count++;
	if (production_count != (CQ_MFIFO_SIZE*2)) { lineno = __LINE__; goto err; }

	rc = memfifo_consume16(&mfifo.consumer, &entry_id); lineno = __LINE__;
	if (rc) goto err;
	if (entry_id != (0x7FFF & consumption_count)) { lineno = __LINE__; goto err; }
	consumption_count++;

	rc = memfifo_produce16(&mfifo.producer, production_count++); lineno = __LINE__;
	if (rc) goto err;

	rc = memfifo_consume16(&mfifo.consumer, &entry_id); lineno = __LINE__;
	if (rc) goto err;
	if (entry_id != (0x7FFF & consumption_count)) { lineno = __LINE__; goto err; }
	consumption_count++;


	fprintf (stdout, "TEST SUCCESSFUL\n");
	return 0;
err:
	fprintf (stderr, "%s: Error at line %d\n", __FILE__, lineno);
	return 1;
}
