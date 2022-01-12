/*
 * Copyright (c) 2013-2015 Intel Corporation.  All rights reserved.
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
#include <errno.h>
#include <getopt.h>
#include <poll.h>
#include <string.h>

#include <rdma/fi_errno.h>

#include "unit_common.h"
#include "shared.h"

static char err_buf[512];

static int
create_eq(size_t size, uint64_t flags, enum fi_wait_obj wait_obj)
{
	struct fi_eq_attr eq_attr;

	memset(&eq_attr, 0, sizeof(eq_attr));
	eq_attr.size = size;
	eq_attr.flags = flags;
	eq_attr.wait_obj = wait_obj;

	return fi_eq_open(fabric, &eq_attr, &eq, NULL);
}

/*
 * Tests:
 * - test open and close of EQ over a range of sizes
 */
static int
eq_open_close()
{
	int i;
	int ret;
	int size;
	int testret;

	testret = FAIL;

	for (i = 0; i < 17; ++i) {
		size = 1 << i;
		ret = create_eq(size, 0, FI_WAIT_UNSPEC);
		if (ret != 0) {
			sprintf(err_buf, "fi_eq_open(%d, 0, FI_WAIT_UNSPEC) = %d, %s",
					size, ret, fi_strerror(-ret));
			goto fail;
		}

		ret = fi_close(&eq->fid);
		if (ret != 0) {
			sprintf(err_buf, "close(eq) = %d, %s", ret, fi_strerror(-ret));
			goto fail;
		}
		eq = NULL;
	}
	testret = PASS;

fail:
	eq = NULL;
	return TEST_RET_VAL(ret, testret);
}

/*
 * Tests:
 * - writing to EQ
 * - reading from EQ with and without FI_PEEK
 * - underflow read
 */
static int
eq_write_read_self()
{
	struct fi_eq_entry entry;
	uint32_t event;
	int testret;
	int ret;
	int i;

	testret = FAIL;

	ret = create_eq(32, FI_WRITE, FI_WAIT_NONE);
	if (ret != 0) {
		sprintf(err_buf, "fi_eq_open ret=%d, %s", ret, fi_strerror(-ret));
		goto fail;
	}

	/* Insert some events */
	for (i = 0; i < 5; ++i) {
		if (i & 1) {
			entry.fid = &fabric->fid;
		} else {
			entry.fid = &eq->fid;
		}
		entry.context = (void *)(uintptr_t)i;
		ret = fi_eq_write(eq, FI_NOTIFY, &entry, sizeof(entry), 0);
		if (ret != sizeof(entry)) {
			sprintf(err_buf, "fi_eq_write ret=%d, %s", ret, fi_strerror(-ret));
			goto fail;
		}
	}

	/* Now read them back, peeking first at each one */
	for (i = 0; i < 10; ++i) {
		event = ~0;
		memset(&entry, 0, sizeof(entry));
		ret = fi_eq_read(eq, &event, &entry, sizeof(entry),
				(i & 1) ? 0 : FI_PEEK);
		if (ret != sizeof(entry)) {
			sprintf(err_buf, "fi_eq_read ret=%d, %s", ret, fi_strerror(-ret));
			goto fail;
		}

		if (event != FI_NOTIFY) {
			sprintf(err_buf, "iter %d: event = %d, should be %d\n", i, event,
					FI_NOTIFY);
			goto fail;
		}

		if ((int)(uintptr_t)entry.context != i / 2) {
			sprintf(err_buf, "iter %d: context mismatch %d != %d", i,
					(int)(uintptr_t)entry.context, i / 2);
			goto fail;
		}

		if (entry.fid != ((i & 2) ? &fabric->fid : &eq->fid)) {
			sprintf(err_buf, "iter %d: fid mismatch %p != %p", i,
					entry.fid, ((i & 2) ? &fabric->fid : &eq->fid));
			goto fail;
		}
	}

	/* queue is now empty */
	ret = fi_eq_read(eq, &event, &entry, sizeof(entry), 0);
	if (ret != -FI_EAGAIN) {
		sprintf(err_buf, "fi_eq_read of empty EQ returned %d", ret);
		goto fail;
	}
	testret = PASS;

fail:
	FT_CLOSE_FID(eq);
	return TEST_RET_VAL(ret, testret);
}

/*
 * Tests:
 * - eq size test
 */
static int
eq_size_verify()
{
	struct fi_eq_entry entry;
	int testret;
	int ret;
	int i;

	testret = FAIL;

	ret = create_eq(32, FI_WRITE, FI_WAIT_NONE);
	if (ret != 0) {
		sprintf(err_buf, "fi_eq_open ret=%d, %s", ret, fi_strerror(-ret));
		goto fail;
	}

	/* Insert some events */
	for (i = 0; i < 32; ++i) {
		entry.fid = &fabric->fid;
		entry.context = (void *)(uintptr_t)i;
		ret = fi_eq_write(eq, FI_NOTIFY, &entry, sizeof(entry), 0);
		if (ret != sizeof(entry)) {
			sprintf(err_buf, "fi_eq_write ret=%d, %s", ret, fi_strerror(-ret));
			goto fail;
		}
	}

	testret = PASS;

fail:
	FT_CLOSE_FID(eq);
	return TEST_RET_VAL(ret, testret);
}

/*
 * Tests:
 * - extracting FD from EQ with FI_WAIT_FD
 * - wait on fd with nothing pending
 * - wait on fd with event pending
 */
static int
eq_wait_fd_poll()
{
	int fd;
	struct fi_eq_entry entry;
	struct pollfd pfd;
	struct fid *fids[1];
	int testret;
	int ret;

	testret = FAIL;

	ret = create_eq(32, FI_WRITE, FI_WAIT_FD);
	if (ret != 0) {
		sprintf(err_buf, "fi_eq_open ret=%d, %s", ret, fi_strerror(-ret));
		goto fail;
	}

	ret = fi_control(&eq->fid, FI_GETWAIT, &fd);
	if (ret != 0) {
		sprintf(err_buf, "fi_control ret=%d, %s", ret, fi_strerror(-ret));
		goto fail;
	}

	fids[0] = &eq->fid;
	if (fi_trywait(fabric, fids, 1) != FI_SUCCESS) {
		sprintf(err_buf, "fi_trywait ret=%d, %s", ret, fi_strerror(-ret));
		goto fail;
	}

	pfd.fd = fd;
	pfd.events = POLLIN;
	ret = poll(&pfd, 1, 0);
	if (ret < 0) {
		sprintf(err_buf, "poll errno=%d, %s", errno, fi_strerror(-errno));
		goto fail;
	}
	if (ret > 0) {
		sprintf(err_buf, "poll returned %d, should be 0", ret);
		goto fail;
	}

	/* write an event */
	entry.fid = &eq->fid;
	entry.context = eq;
	ret = fi_eq_write(eq, FI_NOTIFY, &entry, sizeof(entry), 0);
	if (ret != sizeof(entry)) {
		sprintf(err_buf, "fi_eq_write ret=%d, %s", ret, fi_strerror(-ret));
		goto fail;
	}

	pfd.fd = fd;
	pfd.events = POLLIN;
	ret = poll(&pfd, 1, 0);
	if (ret < 0) {
		sprintf(err_buf, "poll errno=%d, %s", errno, fi_strerror(-errno));
		goto fail;
	}
	if (ret != 1) {
		sprintf(err_buf, "poll returned %d, should be 1", ret);
		goto fail;
	}

	testret = PASS;
fail:
	FT_CLOSE_FID(eq);
	return TEST_RET_VAL(ret, testret);
}

static int insert_events(size_t count)
{
	struct fi_eq_entry entry;
	size_t i;
	int ret;

	for (i = 0; i < count; i++) {
		ret = fi_eq_write(eq, FI_NOTIFY, &entry, sizeof(entry), 0);
		if (ret != sizeof(entry)) {
			sprintf(err_buf, "fi_eq_write ret=%d, %s", ret,
					fi_strerror(-ret));
			return ret;
		}
	}

	return FI_SUCCESS;
}

static int read_events(size_t count, uint64_t flags)
{
	struct fi_eq_entry entry;
	uint32_t event;
	size_t i;
	int ret;

	for (i = 0; i < count; i++) {
		memset(&entry, 0, sizeof(entry));
		event = 0;

		ret = fi_eq_read(eq, &event, &entry, sizeof(entry), flags);
		if (ret != sizeof(entry)) {
			sprintf(err_buf, "fi_eq_read ret=%d, %s", ret,
					fi_strerror(-ret));
			return ret;
		}

		if (event != FI_NOTIFY) {
			sprintf(err_buf, "iter %zu: event = %d, should be %d\n",
					i, event, FI_NOTIFY);
			return -FI_EOTHER;
		}
	}

	return FI_SUCCESS;
}

static int sread_event(int timeout, uint64_t flags)
{
	struct fi_eq_entry entry;
	int64_t elapsed;
	uint32_t event;
	int ret;

	ft_start();
	ret = fi_eq_sread(eq, &event, &entry, sizeof(entry), timeout, flags);
	ft_stop();
	if (ret != sizeof(entry)) {
		sprintf(err_buf, "fi_eq_sread returned %d, %s", ret,
				fi_strerror(-ret));
		return ret;
	}

	/* check timeout accuracy */
	elapsed = get_elapsed(&start, &end, MILLI);
	if (elapsed > (int) (timeout * 1.25)) {
		sprintf(err_buf, "fi_eq_sread slept %d ms, expected %d",
				(int) elapsed, timeout);
		return -FI_EOTHER;
	}

	return FI_SUCCESS;
}

/* Make sure the peeking works fine on normal read. */
static int eq_wait_read_peek(void)
{
	int testret;
	int ret;

	testret = FAIL;

	ret = create_eq(32, FI_WRITE, FI_WAIT_NONE);
	if (ret) {
		sprintf(err_buf, "fi_eq_open ret=%d, %s", ret,
				fi_strerror(-ret));
		goto fail;
	}

	ret = insert_events(5);
	if (ret)
		goto fail;

	ret = read_events(5, FI_PEEK);
	if (ret)
		goto fail;

	ret = read_events(5, 0);
	if (ret)
		goto fail;

	testret = PASS;
fail:
	FT_CLOSE_FID(eq);
	return TEST_RET_VAL(ret, testret);
}

/* Check that the peeking doesn't affect the waiting. If the peek invalidly
 * clears the FD, then this will fail.
 */
static int eq_wait_sread_peek(void)
{
	int testret;
	int ret;

	testret = FAIL;

	ret = create_eq(32, FI_WRITE, FI_WAIT_FD);
	if (ret) {
		sprintf(err_buf, "fi_eq_open ret=%d, %s", ret,
				fi_strerror(-ret));
		goto fail;
	}

	/* Write an event */
	ret = insert_events(1);
	if (ret)
		goto fail;

	/* Make sure we can read the event */
	ret = sread_event(2000, 0);
	if (ret)
		goto fail;

	/* Write another event */
	ret = insert_events(1);
	if (ret)
		goto fail;

	/* Peek at the event */
	ret = sread_event(2000, FI_PEEK);
	if (ret)
		goto fail;

	/* Make sure the event is still available for reading */
	ret = sread_event(2000, 0);
	if (ret)
		goto fail;

	testret = PASS;
fail:
	FT_CLOSE_FID(eq);
	return TEST_RET_VAL(ret, testret);
}

/*
 * Tests:
 * - sread with event pending
 * - sread with no event pending
 */
static int
eq_wait_fd_sread()
{
	struct fi_eq_entry entry;
	uint32_t event;
	int64_t elapsed;
	int testret;
	int ret;

	testret = FAIL;

	ret = create_eq(32, FI_WRITE, FI_WAIT_FD);
	if (ret != 0) {
		sprintf(err_buf, "fi_eq_open ret=%d, %s", ret, fi_strerror(-ret));
		goto fail;
	}

	/* timed sread on empty EQ, 2s timeout */
	ft_start();
	ret = fi_eq_sread(eq, &event, &entry, sizeof(entry), 2000, 0);
	ft_stop();
	if (ret != -FI_EAGAIN) {
		sprintf(err_buf, "fi_eq_read of empty EQ returned %d", ret);
		goto fail;
	}

	/* check timeout accuracy */
	elapsed = get_elapsed(&start, &end, MILLI);
	if (elapsed < 1500 || elapsed > 2500) {
		sprintf(err_buf, "fi_eq_sread slept %d ms, expected 2000",
				(int)elapsed);
		goto fail;
	}

	/* write an event */
	entry.fid = &eq->fid;
	entry.context = eq;
	ret = fi_eq_write(eq, FI_NOTIFY, &entry, sizeof(entry), 0);
	if (ret != sizeof(entry)) {
		sprintf(err_buf, "fi_eq_write ret=%d, %s", ret, fi_strerror(-ret));
		goto fail;
	}

	event = ~0;
	memset(&entry, 0, sizeof(entry));
	/* timed sread on EQ with event, 2s timeout */
	ft_start();
	ret = fi_eq_sread(eq, &event, &entry, sizeof(entry), 2000, 0);
	ft_stop();
	if (ret != sizeof(entry)) {
		sprintf(err_buf, "fi_eq_read ret=%d, %s", ret, fi_strerror(-ret));
		goto fail;
	}

	/* check that no undue waiting occurred */
	elapsed = get_elapsed(&start, &end, MILLI);
	if (elapsed > 5) {
		sprintf(err_buf, "fi_eq_sread slept %d ms, expected immediate return",
				(int)elapsed);
		goto fail;
	}

	if (event != FI_NOTIFY) {
		sprintf(err_buf, "fi_eq_sread: event = %d, should be %d\n", event,
				FI_NOTIFY);
		goto fail;
	}
	if (entry.fid != &eq->fid) {
		sprintf(err_buf, "fi_eq_sread: fid mismatch: %p should be %p\n",
				entry.fid, &eq->fid);
		goto fail;
	}
	if (entry.context != eq) {
		sprintf(err_buf, "fi_eq_sread: context mismatch: %p should be %p\n",
				entry.context, eq);
		goto fail;
	}

	testret = PASS;
fail:
	FT_CLOSE_FID(eq);
	return TEST_RET_VAL(ret, testret);
}

static int
eq_readerr_ret_eagain()
{
	int testret;
	int ret;
	struct fi_eq_err_entry err_entry;

	testret = FAIL;

	memset(&err_entry, 0, sizeof(err_entry));

	/* create EQ  */
	ret = create_eq(32, FI_WRITE, FI_WAIT_UNSPEC);
	if (ret) {
		sprintf(err_buf, "fi_eq_open ret=%d, %s", ret, fi_strerror(-ret));
		goto fail;
	}

	ret = insert_events(5);
	if (ret)
		goto fail;

	ret = read_events(5, 0);
	if (ret)
		goto fail;

	/* Error handling  */
	ret = fi_eq_readerr(eq, &err_entry, 0);
	if (ret == -FI_EAGAIN)
		testret = PASS;
	else
		sprintf(err_buf, ", fi_eq_readerr returned: %d: %s, ", ret,
				fi_eq_strerror(eq, err_entry.prov_errno, err_entry.err_data, NULL, 0));

fail:
	FT_CLOSE_FID(eq);
	return TEST_RET_VAL(ret, testret);
}

struct test_entry test_array[] = {
	TEST_ENTRY(eq_open_close, "Test open and close of EQ for various sizes"),
	TEST_ENTRY(eq_write_read_self, "Test writing and reading to EQ"),
	TEST_ENTRY(eq_size_verify, "Test EQ supports writing # of entries = size"),
	TEST_ENTRY(eq_wait_fd_poll, "Test polling on fd extracted from EQ"),
	TEST_ENTRY(eq_wait_fd_sread, "Test EQ sread"),
	TEST_ENTRY(eq_wait_read_peek, "Test EQ read with FI_PEEK"),
	TEST_ENTRY(eq_wait_sread_peek, "Test EQ sread with FI_PEEK"),
	TEST_ENTRY(eq_readerr_ret_eagain, "Test EQ readerr with FI_EAGAIN"),
	{ NULL, "" }
};

static void usage(void)
{
	ft_unit_usage("eq_test", "Unit test for Event Queue (EQ)");
}

int main(int argc, char **argv)
{
	int op, ret;
	int failed;

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

	hints->mode = FI_CONTEXT | FI_CONTEXT2 | FI_MSG_PREFIX | FI_ASYNC_IOV |
		FI_RX_CQ_DATA | FI_NOTIFY_FLAGS_ONLY | FI_RESTRICTED_COMP |
		FI_BUFFERED_RECV;
	hints->domain_attr->mode = FI_RESTRICTED_COMP;
	hints->domain_attr->mr_mode = ~(FI_MR_BASIC | FI_MR_SCALABLE);

	ret = fi_getinfo(FT_FIVERSION, NULL, 0, 0, hints, &fi);
	if (ret) {
		FT_PRINTERR("fi_getinfo", ret);
		goto err;
	}

	ret = fi_fabric(fi->fabric_attr, &fabric, NULL);
	if (ret) {
		FT_PRINTERR("fi_getinfo", ret);
		goto err;
	}

	printf("Testing EQs on fabric %s\n", fi->fabric_attr->name);

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
