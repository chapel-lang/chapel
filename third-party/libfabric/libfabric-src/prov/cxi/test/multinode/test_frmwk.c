/*
 * SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only
 *
 * Copyright (c) 2022-2023 Hewlett Packard Enterprise Development LP
 */

/*
 * Validation test for the pmi_frmwk implementation.
 *
 * Launch using: srun -N4 ./test_frmwk
 *
 * This can be used as a prototype for test applications.
 *
 * This activates libfabric, populates the AV, and then frees the libfabric
 * instance.
 */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <malloc.h>
#include <time.h>
#include <cxip.h>
#include <ofi.h>
#include "multinode_frmwk.h"

#define	TRACE(fmt, ...)	CXIP_COLL_TRACE(CXIP_TRC_TEST_CODE, fmt, ##__VA_ARGS__)

int main(int argc, char **argv)
{
	fi_addr_t *fiaddr = NULL;
	size_t size = 0;
	int i, j, ret;

	frmwk_init(false);
	printf("[%d|%d] initialized\n", frmwk_rank, frmwk_numranks);

	ret = frmwk_gather_nics();
	for (i = 0; i < frmwk_numranks; i++) {
		printf("[%d|%d] rank %d HSNS [", frmwk_rank, frmwk_numranks, i);
		for (j = 0; j < frmwk_nics_per_rank; j++)
			printf(" %05x", frmwk_nic_addr(i, j));
		printf("]\n");
	}

	frmwk_barrier();

	ret = frmwk_init_libfabric();
	if (frmwk_errmsg(ret, "frmwk_init_libfabric()\n"))
		return ret;

	ret = frmwk_populate_av(&fiaddr, &size);
	if (frmwk_errmsg(ret, "frmwk_populate_av()\n"))
		return ret;

	printf("[%d|%d] fiaddrs\n", frmwk_rank, frmwk_numranks);
	for (i = 0; i < size; i++) {
		printf("[%d|%d] %ld\n", frmwk_rank, frmwk_numranks,
			fiaddr[i]);
	}

	cxip_coll_trace_muted = false;
	TRACE("Trace message test %d\n", 0);
	TRACE("Trace message test %d\n", 1);
	cxip_coll_trace_muted = true;
	TRACE("This message should not appear\n");
	cxip_coll_trace_muted = false;
	TRACE("This message should appear\n");

	frmwk_free_libfabric();
	free(fiaddr);

	frmwk_term();
	return ret;
}
