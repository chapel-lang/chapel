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

#include <pthread.h>

#include "fi_bgq_spi.h"


#include "cq_agent.h"
#include "l2atomic.h"
#include "fi_bgq_memfifo.h"


int cq_agent_main_test (struct l2atomic_barrier * barrier);

void test_init_fn (void *buffer, uintptr_t cookie) {

	uint64_t *ptr = (uint64_t *) buffer;
	*ptr = cookie;
}

int main (int argc, char *argv[]) {

	struct l2atomic l2atomic;

	int rc, lineno;
	rc = l2atomic_init(&l2atomic); lineno = __LINE__;
	if (rc) goto err;

	uint32_t ppn = Kernel_ProcessCount(); lineno = __LINE__;
	if (ppn==1) {
		/* check for ofi agent environment variable */
		char * envvar = NULL;
		envvar = getenv("BG_APPAGENT"); lineno = __LINE__;
		if (!envvar) { fprintf(stderr, "Required environment variable 'BG_APPAGENT' is not set\n"); goto err; }
	}

	struct l2atomic_barrier barrier;
	rc = l2atomic_barrier_alloc_generic(&l2atomic, &barrier, 2, "agent_barrier"); lineno = __LINE__;
	if (rc) goto err;

	uint32_t tcoord = Kernel_MyTcoord();
	if (tcoord==1) {

		//struct cq_agent_internal internal;
		rc = cq_agent_main_test(&barrier); lineno = __LINE__;
		if (rc) goto err;


	} else if (tcoord==0) {
		struct cq_agent_client client;
		rc = cq_agent_client_init(&client, &l2atomic); lineno = __LINE__;
		if (rc) goto err;

		union fi_bgq_addr self;
		fi_bgq_create_addr_self_cx(&self.fi, 0);

		struct memfifo mfifo;
		MUHWI_Descriptor_t model;
		rc = cq_agent_client_register(&client, &l2atomic, &self, &mfifo, 8192, &model, 1); lineno = __LINE__;
		if (rc) goto err;

		struct cq_agent_client_test_mu test;
		rc = cq_agent_client_test_mu_setup(&test); lineno = __LINE__;
		if (rc) goto err;

		uint16_t entry_id = 1234;
		rc = cq_agent_client_test_mu_inject(&test, &model, entry_id, 1); lineno = __LINE__;
		if (rc) goto err;

		if (ppn>1) l2atomic_barrier(&barrier);

		/* spin until something is received from the mfifo */
		uint16_t id = (uint16_t)-1;
		while (0 != memfifo_consume16(&mfifo.consumer, &id));

		if (ppn>1) l2atomic_barrier(&barrier);
		fprintf (stdout, "TEST SUCCESSFUL\n");
	}

	return 0;
err:
	fprintf(stderr, "%s : Error at line %d (rc=%d)\n", __FILE__, lineno, rc);
	abort();
}
