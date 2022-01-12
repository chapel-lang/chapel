/*
 * Copyright (c) 2013-2016 Intel Corporation.  All rights reserved.
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
#include <string.h>

#include <rdma/fi_errno.h>

#include "unit_common.h"
#include "shared.h"

static int test_max = 1 << 15;
static char err_buf[512];

static int
create_cq(struct fid_cq **cq, size_t size, uint64_t flags,
		enum fi_cq_format format, enum fi_wait_obj wait_obj)
{
	struct fi_cq_attr cq_attr;

	memset(&cq_attr, 0, sizeof(cq_attr));
	cq_attr.size = size;
	cq_attr.flags = flags;
	cq_attr.format = format;
	cq_attr.wait_obj = wait_obj;

	return fi_cq_open(domain, &cq_attr, cq, NULL);
}

/* Try opening fi->domain_attr->cq_cnt number of completion queues
 * simultaneously using a size hint of 0 (indicating the provider should choose
 * the size)
 */
static int cq_open_close_simultaneous(void)
{
	int ret;
	int opened;
	size_t count;
	int testret = FAIL;
	struct fid_cq **cq_array;

	count = MIN(fi->domain_attr->cq_cnt, test_max);
	FT_DEBUG("testing creation of up to %zu simultaneous CQs\n", count);

	cq_array = calloc(count, sizeof(*cq_array));
	if (!cq_array)
		return -FI_ENOMEM;

	ret = 0;
	for (opened = 0; opened < count && !ret; opened++) {
		ret = create_cq(&cq_array[opened], 0, 0, FI_CQ_FORMAT_UNSPEC,
				FI_WAIT_UNSPEC);
		if (ret) {
			ret = create_cq(&cq_array[opened], 0, 0,
					FI_CQ_FORMAT_UNSPEC, FI_WAIT_NONE);
		}
	}
	if (ret) {
		FT_WARN("fi_cq_open failed after %d (cq_cnt: %zu): %s",
			opened, count, fi_strerror(-ret));
	}

	testret = PASS;

	FT_CLOSEV_FID(cq_array, opened);
	free(cq_array);

	return TEST_RET_VAL(ret, testret);
}

/*
 * Tests:
 * - test open and close of CQ over a range of sizes
 */
static int
cq_open_close_sizes()
{
	int i;
	int ret;
	int size;
	int testret;
	struct fid_cq *cq;

	testret = FAIL;

	for (i = -1; i < 17; ++i) {
		size = (i < 0) ? 0 : 1 << i;

		ret = create_cq(&cq, size, 0, FI_CQ_FORMAT_UNSPEC, FI_WAIT_UNSPEC);
		if (ret != 0) {
			ret = create_cq(&cq, size, 0, FI_CQ_FORMAT_UNSPEC,
					FI_WAIT_NONE);
		}

		if (ret == -FI_EINVAL) {
			FT_WARN("\nSuccessfully completed %d iterations up to "
				"size %d before the provider returned "
				"EINVAL...",
				i + 1, size >> 1);
			ret = 0;
			goto pass;
		}
		if (ret != 0) {
			sprintf(err_buf, "fi_cq_open with size %d returned %d, %s",
					size, ret, fi_strerror(-ret));
			goto fail;
		}

		ret = fi_close(&cq->fid);
		if (ret != 0) {
			sprintf(err_buf, "close(cq) = %d, %s", ret, fi_strerror(-ret));
			goto fail;
		}
		cq = NULL;
	}

pass:
	testret = PASS;
fail:
	cq = NULL;
	return TEST_RET_VAL(ret, testret);
}

static int
cq_signal()
{
	struct fid_cq *cq;
	struct fi_cq_tagged_entry entry;
	int64_t elapsed;
	int testret;
	int ret;

	testret = FAIL;

	ret = create_cq(&cq, 1, 0, FI_CQ_FORMAT_UNSPEC, FI_WAIT_UNSPEC);
	if (ret) {
		sprintf(err_buf, "fi_cq_open(1, 0, FI_CQ_FORMAT_UNSPEC, "
				"FI_WAIT_UNSPEC) = %d, %s",
				ret, fi_strerror(-ret));
		goto fail1;
	}

	ret = fi_cq_signal(cq);
	if (ret) {
		sprintf(err_buf, "fi_cq_signal = %d %s", ret, fi_strerror(-ret));
		goto fail2;
	}

	ft_start();
	ret = fi_cq_sread(cq, &entry, 1, NULL, 2000);
	ft_stop();
	elapsed = get_elapsed(&start, &end, MILLI);
	if (ret != -FI_EAGAIN && ret != -FI_ECANCELED) {
		sprintf(err_buf, "fi_cq_sread = %d %s", ret, fi_strerror(-ret));
		goto fail2;
	}

	if (elapsed > 1000) {
		sprintf(err_buf, "fi_cq_sread - signal ignored");
		goto fail2;
	}

	ret = fi_close(&cq->fid);
	if (ret) {
		sprintf(err_buf, "close(cq) = %d, %s", ret, fi_strerror(-ret));
		goto fail1;
	}
	cq = NULL;

	testret = PASS;
fail2:
	FT_CLOSE_FID(cq);
fail1:
	cq = NULL;
	return TEST_RET_VAL(ret, testret);
}


struct test_entry test_array[] = {
	TEST_ENTRY(cq_open_close_sizes, "Test open and close of CQ for various sizes"),
	TEST_ENTRY(cq_open_close_simultaneous, "Test opening several CQs at a time"),
	TEST_ENTRY(cq_signal, "Test fi_cq_signal"),
	{ NULL, "" }
};

static void usage(void)
{
	ft_unit_usage("cq_test", "Unit test for Completion Queue (CQ)");
	FT_PRINT_OPTS_USAGE("-L <int>", "Limit of CQs to open. Default: 32k");
}

int main(int argc, char **argv)
{
	int op, ret;
	int failed;

	hints = fi_allocinfo();
	if (!hints)
		return EXIT_FAILURE;

	while ((op = getopt(argc, argv, FAB_OPTS "hL:")) != -1) {
		switch (op) {
		case 'L':
			test_max = atoi(optarg);
			break;
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
		goto err;
	}

	ret = ft_open_fabric_res();
	if (ret)
		goto err;

	printf("Testing CQs on fabric %s\n", fi->fabric_attr->name);

	failed = run_tests(test_array, err_buf);
	if (failed > 0) {
		printf("Summary: %d tests failed\n", failed);
	} else {
		printf("Summary: all tests passed\n");
	}

err:
	ft_free_res();
	return ret ? ft_exit_code(ret) : (failed > 0) ? EXIT_FAILURE : EXIT_SUCCESS;
}
