/*
 * Copyright (c) 2013-2017 Intel Corporation.  All rights reserved.
 * Copyright (c) 2014-2016 Cisco Systems, Inc.  All rights reserved.
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

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <sched.h>

#include <rdma/fi_errno.h>

#include "unit_common.h"
#include "shared.h"


static char err_buf[512];
#define MAX_COUNTER_CHECK 100


static int cntr_loop()
{
	size_t i, opened, cntr_cnt;
	uint64_t value, expected;
	struct timespec start, stop;
	int ret, testret = FAIL, timeout = 5000;

	cntr_cnt = MIN(fi->domain_attr->cntr_cnt, MAX_COUNTER_CHECK);
	struct fid_cntr **cntrs = calloc(cntr_cnt, sizeof(struct fid_cntr *));
	if (!cntrs) {
		perror("calloc");
		return -FI_ENOMEM;
	}

	for (opened = 0; opened < cntr_cnt; opened++) {
		ret = ft_cntr_open(&cntrs[opened]);
		if (ret) {
			FT_PRINTERR("fi_cntr_open", ret);
			goto close;
		}
	}

	for (i = 0; i < opened; i++) {
		ret = fi_cntr_set(cntrs[i], i);
		if (ret) {
			FT_PRINTERR("fi_cntr_set", ret);
			goto close;
		}

		ret = fi_cntr_seterr(cntrs[i], i << 1);
		if (ret) {
			FT_PRINTERR("fi_cntr_seterr", ret);
			goto close;
		}
	}

	for (i = 0; i < opened; i++) {
		ret = fi_cntr_add(cntrs[i], i);
		if (ret) {
			FT_PRINTERR("fi_cntr_add", ret);
			goto close;
		}

		ret = fi_cntr_adderr(cntrs[i], i);
		if (ret) {
			FT_PRINTERR("fi_cntr_adderr", ret);
			goto close;
		}
	}

	for (i = 0; i < opened; i++) {
		clock_gettime(CLOCK_MONOTONIC, &start);
		expected = i + i;
		do {
			value = fi_cntr_read(cntrs[i]);
			clock_gettime(CLOCK_MONOTONIC, &stop);
			sched_yield();
		} while ((value != expected) &&
			((stop.tv_sec - start.tv_sec) > timeout));
		if (value != expected) {
			FT_PRINTERR("fi_cntr_read", value);
			goto close;
		}

		clock_gettime(CLOCK_MONOTONIC, &start);
		expected = (i << 1) + i;
		do {
			value = fi_cntr_readerr(cntrs[i]);
			clock_gettime(CLOCK_MONOTONIC, &stop);
			sched_yield();
		} while ((value != expected) &&
			((stop.tv_sec - start.tv_sec) > timeout));
		if (value != expected) {
			FT_PRINTERR("fi_cntr_readerr", value);
			goto close;
		}
	}
	testret = PASS;

close:
	for (i = 0; i < opened; i++) {
		ret = fi_close(&(cntrs[i])->fid);
		if (ret) {
			FT_PRINTERR("fi_cntr_close", ret);
			break;
		}
	}

	if (i < cntr_cnt)	
		testret = FAIL;

	free(cntrs);
	return TEST_RET_VAL(ret, testret);
}

struct test_entry test_array[] = {
	TEST_ENTRY(cntr_loop, "Test counter open/set/read/close operations"),
	{ NULL, "" }
};

static void usage(void)
{
	ft_unit_usage("cntr_test", "Unit test for counter (cntr)");
}

int main(int argc, char **argv)
{
	int op, ret;
	int failed = 0;

	hints = fi_allocinfo();
	if (!hints)
		return EXIT_FAILURE;

	while ((op = getopt(argc, argv, FAB_OPTS "h")) != -1) {
		switch (op) {
		default:
			ft_parseinfo(op, optarg, hints, &opts);
			break;
		case '?':
		case 'h':
			usage();
			return EXIT_FAILURE;
		}
	}

	hints->mode = ~0;
	hints->domain_attr->mode = ~0;
	hints->domain_attr->mr_mode = ~(FI_MR_BASIC | FI_MR_SCALABLE);

	ret = fi_getinfo(FT_FIVERSION, NULL, 0, 0, hints, &fi);
	if (ret) {
		FT_PRINTERR("fi_getinfo", ret);
		goto out;
	}

	if (!fi->domain_attr->cntr_cnt)
		goto out;

	ret = ft_open_fabric_res();
	if (ret)
		goto out;

	printf("Testing CNTRS on fabric %s\n", fi->fabric_attr->name);

	failed = run_tests(test_array, err_buf);
	if (failed > 0)
		printf("Summary: %d tests failed\n", failed);
	else
		printf("Summary: all tests passed\n");

out:
	ft_free_res();
	return ret ? ft_exit_code(ret) : (failed > 0) ? EXIT_FAILURE : EXIT_SUCCESS;
}
