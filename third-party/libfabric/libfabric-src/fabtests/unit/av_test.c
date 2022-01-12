/*
 * Copyright (c) 2013-2015 Intel Corporation.  All rights reserved.
 * Copyright (c) 2014 Cisco Systems, Inc.  All rights reserved.
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
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <rdma/fi_errno.h>

#include "shared.h"
#include "unit_common.h"

#define MAX_ADDR 256

char *good_address;
int num_good_addr;
char *bad_address;

static enum fi_av_type av_type;

static char err_buf[512];

static int
check_eq_readerr(struct fid_eq *eq, fid_t fid, void *context, int index)
{
	int ret;
	struct fi_eq_err_entry err_entry;

	memset(&err_entry, 0, sizeof(err_entry));
	ret = fi_eq_readerr(eq, &err_entry, 0);
	if (ret != sizeof(err_entry)) {
		sprintf(err_buf, "fi_eq_readerr ret = %d, %s", ret,
				(ret < 0) ? fi_strerror(-ret) : "unknown");
		return -1;
	}
	if (err_entry.fid != fid) {
		sprintf(err_buf, "fi_eq_readerr fid = %p, should be %p",
				err_entry.fid, fid);
		return -1;
	}
	if (err_entry.context != context) {
		sprintf(err_buf, "fi_eq_readerr fid = %p, should be %p",
				err_entry.context, context);
		return -1;
	}
	if (err_entry.data != index) {
		sprintf(err_buf, "fi_eq_readerr index = %" PRIu64 ", should be %d",
				err_entry.data, index);
		return -1;
	}
	if (err_entry.err <= 0) {
		sprintf(err_buf, "fi_eq_readerr err = %d, should be > 0",
				err_entry.err);
		return -1;
	}
	return 0;
}

static int
check_eq_result(int ret, uint32_t event, struct fi_eq_entry *entry,
		fid_t fid, void *context, uint32_t count)
{
	if (ret != sizeof(*entry)) {
		sprintf(err_buf, "fi_eq_sread ret = %d, %s", ret,
				(ret < 0) ? fi_strerror(-ret) : "unknown");
		return -1;
	}
	if (event != FI_AV_COMPLETE) {
		sprintf(err_buf, "fi_eq_sread event = %u, should be %u", event,
				FI_AV_COMPLETE);
		return -1;
	}
	if (entry->fid != fid) {
		sprintf(err_buf, "fi_eq_sread fid = %p, should be %p",
				entry->fid, fid);
		return -1;
	}
	/* context == NULL means skip check */
	if (context != NULL && entry->context != context) {
		sprintf(err_buf, "fi_eq_sread fid = %p, should be %p", entry->context,
				context);
		return -1;
	}
	if (count != ~0 && entry->data != count) {
		sprintf(err_buf, "count = %" PRIu64 ", should be %u", entry->data, count);
		return -1;
	}
	return 0;
}

static int
check_eq_sread(struct fid_eq *eq, fid_t fid, void *context, uint32_t count,
		int timeout, uint64_t flags)
{
	struct fi_eq_entry entry;
	uint32_t event;
	int ret;

	event = ~0;
	memset(&entry, 0, sizeof(entry));

	ret = fi_eq_sread(eq, &event, &entry, sizeof(entry), timeout, flags);
	return check_eq_result(ret, event, &entry, fid, context, count);
}

static int
av_test_open_close(enum fi_av_type type, int count, uint64_t flags)
{
	int ret;
	struct fi_av_attr attr;
	struct fid_av *av;

	memset(&attr, 0, sizeof(attr));
	attr.type = type;
	attr.count = count;
	attr.flags = flags;

	ret = fi_av_open(domain, &attr, &av, NULL);
	if (ret != 0) {
		sprintf(err_buf, "fi_av_open(%d, %s) = %d, %s",
				count, fi_tostr(&type, FI_TYPE_AV_TYPE),
				ret, fi_strerror(-ret));
		return ret;
	}

	ret = fi_close(&av->fid);
	if (ret != 0) {
		sprintf(err_buf, "close(av) = %d, %s", ret, fi_strerror(-ret));
		return ret;
	}
	return 0;
}

/*
 * Tests:
 * - test open and close of AV
 */
static int
av_open_close()
{
	int i;
	int testret;
	int ret;
	int count;

	testret = FAIL;

	for (i = 0; i < 17; ++i) {
		count = 1 << i;
		ret = av_test_open_close(av_type, count, 0);
		if (ret != 0) {
			goto fail;
		}
	}
	testret = PASS;
fail:
	return TEST_RET_VAL(ret, testret);
}

static int
av_create_addr_sockaddr_in(char *first_address, int index, void *addr)
{
	struct addrinfo hints;
	struct addrinfo *ai;
	struct sockaddr_in *sin;
	uint32_t tmp;
	int ret;

	memset(&hints, 0, sizeof(hints));

	/* return all 0's for invalid address */
	if (first_address == NULL) {
		memset(addr, 0, sizeof(*sin));
		return 0;
	}

	hints.ai_family = AF_INET;
	/* port doesn't matter, set port to discard port */
	ret = getaddrinfo(first_address, "discard", &hints, &ai);
	if (ret != 0) {
		sprintf(err_buf, "getaddrinfo: %s", gai_strerror(ret));
		return -1;
	}

	sin = (struct sockaddr_in *)addr;
	*sin = *(struct sockaddr_in *)ai->ai_addr;

	tmp = ntohl(sin->sin_addr.s_addr);
	tmp += index;
	sin->sin_addr.s_addr = htonl(tmp);

	freeaddrinfo(ai);
	return 0;
}

ssize_t av_get_addrlen(struct fi_info *info)
{
	switch (info->addr_format) {
	case FI_SOCKADDR:
	case FI_SOCKADDR_IN:
		return sizeof(struct sockaddr_in);
	default:
		sprintf(err_buf, "test does not yet support %s",
				fi_tostr(&info->addr_format, FI_TYPE_ADDR_FORMAT));
		return -FI_ENOSYS;
	}
}

/*
 * Create an address list
 */
static int
av_create_address_list(char *first_address, int base, int num_addr,
		void *addr_array, int offset, int len)
{
	int (*add_address)(char *, int, void *);
	uint8_t *cur_addr;
	int addrlen;
	int ret;
	int i;

	switch (fi->addr_format) {
	case FI_SOCKADDR:
	case FI_SOCKADDR_IN:
		addrlen = sizeof(struct sockaddr_in);
		add_address = av_create_addr_sockaddr_in;
		break;
	default:
		sprintf(err_buf, "test does not yet support %s",
				fi_tostr(&fi->addr_format, FI_TYPE_ADDR_FORMAT));
		return -FI_ENOSYS;
	}

	if (len < addrlen * (offset + num_addr)) {
		sprintf(err_buf, "internal error, not enough room for %d addresses",
				num_addr);
		return -FI_ENOMEM;
	}

	cur_addr = addr_array;
	cur_addr += offset * addrlen;
	for (i = 0; i < num_addr; ++i) {
		ret = add_address(first_address, base + i, cur_addr);
		if (ret != 0) {
			return ret;
		}
		cur_addr += addrlen;
	}

	return cur_addr - (uint8_t *)addr_array;
}

/*
 * Tests:
 * - synchronous resolution of good address
 */
static int
av_good_sync()
{
	int testret;
	int ret;
	struct fid_av *av;
	struct fi_av_attr attr;
	uint8_t addrbuf[4096];
	int buflen;
	fi_addr_t fi_addr;

	testret = FAIL;

	memset(&attr, 0, sizeof(attr));
	attr.type = av_type;
	attr.count = 32;

	av = NULL;
	ret = fi_av_open(domain, &attr, &av, NULL);
	if (ret != 0) {
		sprintf(err_buf, "fi_av_open(%s) = %d, %s",
				fi_tostr(&av_type, FI_TYPE_AV_TYPE),
				ret, fi_strerror(-ret));
		goto fail;
	}

	fi_addr = FI_ADDR_NOTAVAIL;

	buflen = sizeof(addrbuf);
	ret = av_create_address_list(good_address, 0, 1, addrbuf, 0, buflen);
	if (ret < 0) {
		goto fail;		// av_create_address_list filled err_buf
	}

	ret = fi_av_insert(av, addrbuf, 1, &fi_addr, 0, NULL);
	if (ret != 1) {
		sprintf(err_buf, "fi_av_insert ret=%d, %s", ret, fi_strerror(-ret));
		goto fail;
	}
	if (fi_addr == FI_ADDR_NOTAVAIL) {
		sprintf(err_buf, "fi_addr == FI_ADDR_NOTAVAIL");
		goto fail;
	}

	testret = PASS;
fail:
	FT_CLOSE_FID(av);
	return TEST_RET_VAL(ret, testret);
}

static int
av_null_fi_addr()
{
	int testret;
	int ret;
	struct fid_av *av;
	struct fi_av_attr attr;
	uint8_t addrbuf[4096];
	int buflen;

	testret = FAIL;

	if (av_type != FI_AV_TABLE) {
		ret = 0;
		testret = SKIPPED;
		sprintf(err_buf, "test not valid for AV type FI_AV_MAP");
		goto out1;
	}

	memset(&attr, 0, sizeof(attr));
	attr.type = av_type;
	attr.count = 32;

	av = NULL;
	ret = fi_av_open(domain, &attr, &av, NULL);
	if (ret != 0) {
		sprintf(err_buf, "fi_av_open(%s) = %d, %s",
				fi_tostr(&av_type, FI_TYPE_AV_TYPE),
				ret, fi_strerror(-ret));
		goto out1;
	}

	buflen = sizeof(addrbuf);
	ret = av_create_address_list(good_address, 0, 1, addrbuf, 0, buflen);
	if (ret < 0) {
		goto out2;		// av_create_address_list filled err_buf
	}

	ret = fi_av_insert(av, addrbuf, 1, NULL, 0, NULL);
	if (ret != 1) {
		sprintf(err_buf, "fi_av_insert ret=%d, %s", ret, fi_strerror(-ret));
		goto out2;
	}
	testret = PASS;
out2:
	FT_CLOSE_FID(av);
out1:
	return TEST_RET_VAL(ret, testret);
}

/*
 * Tests:
 * - synchronous resolution of bad address
 */
static int
av_bad_sync()
{
	int testret;
	int ret;
	struct fid_av *av;
	struct fi_av_attr attr;
	uint8_t addrbuf[4096];
	int buflen;
	fi_addr_t fi_addr;

	testret = FAIL;

	memset(&attr, 0, sizeof(attr));
	attr.type = av_type;
	attr.count = 32;

	av = NULL;
	ret = fi_av_open(domain, &attr, &av, NULL);
	if (ret != 0) {
		sprintf(err_buf, "fi_av_open(%s) = %d, %s",
				fi_tostr(&av_type, FI_TYPE_AV_TYPE),
				ret, fi_strerror(-ret));
		goto fail;
	}

	fi_addr = ~FI_ADDR_NOTAVAIL;

	buflen = sizeof(addrbuf);
	ret = av_create_address_list(bad_address, 0, 1, addrbuf, 0, buflen);
	if (ret < 0) {
		goto fail;		// av_create_address_list filled err_buf
	}

	ret = fi_av_insert(av, addrbuf, 1, &fi_addr, 0, NULL);
	if (ret != 0) {
		sprintf(err_buf, "fi_av_insert ret=%d, should be 0", ret);
		goto fail;
	}
	if (fi_addr != FI_ADDR_NOTAVAIL) {
		sprintf(err_buf,
				"fi_addr = 0x%" PRIx64 ", should be 0x%" PRIx64" (FI_ADDR_NOTAVAIL)",
				fi_addr, FI_ADDR_NOTAVAIL);
		goto fail;
	}

	testret = PASS;
fail:
	FT_CLOSE_FID(av);
	return TEST_RET_VAL(ret, testret);
}

/*
 * Tests:
 * - sync vector with 1 good and 1 bad
 */
static int
av_goodbad_vector_sync()
{
	int testret;
	int ret;
	int i;
	struct fid_av *av;
	struct fi_av_attr attr;
	uint8_t addrbuf[4096];
	int buflen;
	fi_addr_t fi_addr[MAX_ADDR];

	testret = FAIL;

	memset(&attr, 0, sizeof(attr));
	attr.type = av_type;
	attr.count = 32;

	av = NULL;
	ret = fi_av_open(domain, &attr, &av, NULL);
	if (ret != 0) {
		sprintf(err_buf, "fi_av_open(%s) = %d, %s",
				fi_tostr(&av_type, FI_TYPE_AV_TYPE),
				ret, fi_strerror(-ret));
		goto fail;
	}

	for (i = 0; i < MAX_ADDR; ++i) {
		fi_addr[i] = FI_ADDR_NOTAVAIL;
	}
	fi_addr[1] = ~FI_ADDR_NOTAVAIL;

	buflen = sizeof(addrbuf);

	/* vector is good address + bad address */
	ret = av_create_address_list(good_address, 0, 1, addrbuf, 0, buflen);
	if (ret < 0) {
		goto fail;		// av_create_address_list filled err_buf
	}
	ret = av_create_address_list(bad_address, 0, 1, addrbuf, 1, buflen);
	if (ret < 0) {
		goto fail;		// av_create_address_list filled err_buf
	}
	ret = fi_av_insert(av, addrbuf, 2, fi_addr, 0, NULL);
	if (ret != 1) {
		sprintf(err_buf, "fi_av_insert ret=%d, should be 1", ret);
		goto fail;
	}

	/*
	 * Check returned fi_addrs
	 */
	if (fi_addr[0] == FI_ADDR_NOTAVAIL) {
		sprintf(err_buf, "fi_addr[0] = FI_ADDR_NOTAVAIL");
		goto fail;
	}
	if (fi_addr[1] != FI_ADDR_NOTAVAIL) {
		sprintf(err_buf, "fi_addr[1] != FI_ADDR_NOTAVAIL");
		goto fail;
	}

	testret = PASS;
fail:
	FT_CLOSE_FID(av);
	return TEST_RET_VAL(ret, testret);
}

/*
 * Tests:
 * - sync vector with 1 good and 1 bad using FI_SYNC_ERR
 */
static int
av_goodbad_vector_sync_err()
{
	int testret, ret;
	struct fid_av *av;
	struct fi_av_attr attr;
	uint8_t addrbuf[4096];
	int buflen;
	int sync_err[2];

	if (av_type != FI_AV_TABLE) {
		ret = 0;
		testret = SKIPPED;
		sprintf(err_buf, "test not valid for AV type FI_AV_MAP");
		goto out;
	}

	testret = FAIL;

	memset(&attr, 0, sizeof(attr));
	attr.type = av_type;
	attr.count = 32;

	av = NULL;
	ret = fi_av_open(domain, &attr, &av, NULL);
	if (ret != 0) {
		sprintf(err_buf, "fi_av_open(%s) = %d, %s",
				fi_tostr(&av_type, FI_TYPE_AV_TYPE),
				ret, fi_strerror(-ret));
		goto fail;
	}

	sync_err[0] = -1;
	sync_err[1] = 0;

	buflen = sizeof(addrbuf);

	/* vector is good address + bad address */
	ret = av_create_address_list(good_address, 0, 1, addrbuf, 0, buflen);
	if (ret < 0) {
		goto fail;		// av_create_address_list filled err_buf
	}
	ret = av_create_address_list(bad_address, 0, 1, addrbuf, 1, buflen);
	if (ret < 0) {
		goto fail;		// av_create_address_list filled err_buf
	}
	ret = fi_av_insert(av, addrbuf, 2, NULL, FI_SYNC_ERR, sync_err);
	if (ret != 1) {
		if (ret == -FI_EBADFLAGS) {
			sprintf(err_buf, "FI_SYNC_ERR not supported\n");
			ret = -FI_ENOSYS;
		}
		sprintf(err_buf, "fi_av_insert ret=%d, should be 1", ret);
		goto fail;
	}

	if (sync_err[0] != 0) {
		sprintf(err_buf, "sync_err[0] != 0");
		goto fail;
	}
	if (sync_err[1] == 0) {
		sprintf(err_buf, "sync_err[1] = 0");
		goto fail;
	}

	testret = PASS;
fail:
	FT_CLOSE_FID(av);
out:
	return TEST_RET_VAL(ret, testret);
}

/*
 * Tests:
 * - async good vector
 */
static int
av_good_vector_async()
{
	int testret;
	int ret;
	int i;
	struct fid_av *av;
	struct fi_av_attr attr;
	uint8_t addrbuf[4096];
	uint32_t ctx;
	int buflen;
	fi_addr_t fi_addr[MAX_ADDR];

	testret = FAIL;

	memset(&attr, 0, sizeof(attr));
	attr.type = av_type;
	attr.count = 32;
	attr.flags = FI_EVENT;

	av = NULL;
	ret = fi_av_open(domain, &attr, &av, NULL);
	if (ret != 0) {
		sprintf(err_buf, "fi_av_open(%s) = %d, %s",
				fi_tostr(&av_type, FI_TYPE_AV_TYPE),
				ret, fi_strerror(-ret));
		goto fail;
	}
	ret = fi_av_bind(av, &eq->fid, 0);
	if (ret != 0) {
		sprintf(err_buf, "fi_av_bind() = %d, %s", ret, fi_strerror(-ret));
		goto fail;
	}

	for (i = 0; i < MAX_ADDR; ++i) {
		fi_addr[i] = FI_ADDR_NOTAVAIL;
	}

	buflen = sizeof(addrbuf);
	ret = av_create_address_list(good_address, 0, num_good_addr,
			addrbuf, 0, buflen);
	if (ret < 0) {
		goto fail;		// av_create_address_list filled err_buf
	}

	for (i = 0; i < num_good_addr; ++i) {
		fi_addr[i] = FI_ADDR_NOTAVAIL;
	}
	ret = fi_av_insert(av, addrbuf, num_good_addr, fi_addr, 0, &ctx);
	if (ret) {
		sprintf(err_buf, "fi_av_insert ret=%d, %s", ret, fi_strerror(-ret));
		goto fail;
	}

	if (check_eq_sread(eq, &av->fid, &ctx, num_good_addr, 20000, 0) != 0) {
		goto fail;
	}
	for (i = 0; i < num_good_addr; ++i) {
		if (fi_addr[i] == FI_ADDR_NOTAVAIL) {
			sprintf(err_buf, "fi_addr[%d] = FI_ADDR_NOTAVAIL", i);
			goto fail;
		}
	}

	testret = PASS;
fail:
	FT_CLOSE_FID(av);
	return TEST_RET_VAL(ret, testret);
}

/*
 * Tests:
 * - async good vector
 */
static int
av_zero_async()
{
	int testret;
	int ret;
	struct fid_av *av;
	struct fi_av_attr attr;
	uint8_t addrbuf[4096];
	uint32_t ctx;
	fi_addr_t fi_addr[MAX_ADDR];

	testret = FAIL;

	memset(&attr, 0, sizeof(attr));
	attr.type = av_type;
	attr.count = 32;
	attr.flags = FI_EVENT;

	av = NULL;
	ret = fi_av_open(domain, &attr, &av, NULL);
	if (ret != 0) {
		sprintf(err_buf, "fi_av_open(%s) = %d, %s",
				fi_tostr(&av_type, FI_TYPE_AV_TYPE),
				ret, fi_strerror(-ret));
		goto fail;
	}
	ret = fi_av_bind(av, &eq->fid, 0);
	if (ret != 0) {
		sprintf(err_buf, "fi_av_bind() = %d, %s", ret, fi_strerror(-ret));
		goto fail;
	}

	ret = fi_av_insert(av, addrbuf, 0, fi_addr, 0, &ctx);
	if (ret != 0) {
		sprintf(err_buf, "fi_av_insert ret=%d, %s", ret, fi_strerror(-ret));
		goto fail;
	}

	if (check_eq_sread(eq, &av->fid, &ctx, 0, 20000, 0) != 0) {
		goto fail;
	}

	testret = PASS;
fail:
	FT_CLOSE_FID(av);
	return TEST_RET_VAL(ret, testret);
}

/*
 * Tests:
 * - async 2 good vectors
 */
static int
av_good_2vector_async()
{
	int testret;
	int ret;
	int i;
	struct fid_av *av;
	struct fi_av_attr attr;
	uint8_t addrbuf[4096];
	uint32_t event;
	struct fi_eq_entry entry;
	uint32_t ctx[2];
	int buflen;
	fi_addr_t fi_addr[MAX_ADDR];

	testret = FAIL;

	memset(&attr, 0, sizeof(attr));
	attr.type = av_type;
	attr.count = 32;
	attr.flags = FI_EVENT;

	av = NULL;
	ret = fi_av_open(domain, &attr, &av, NULL);
	if (ret != 0) {
		sprintf(err_buf, "fi_av_open(%s) = %d, %s",
				fi_tostr(&av_type, FI_TYPE_AV_TYPE),
				ret, fi_strerror(-ret));
		goto fail;
	}
	ret = fi_av_bind(av, &eq->fid, 0);
	if (ret != 0) {
		sprintf(err_buf, "fi_av_bind() = %d, %s", ret, fi_strerror(-ret));
		goto fail;
	}

	for (i = 0; i < MAX_ADDR; ++i) {
		fi_addr[i] = FI_ADDR_NOTAVAIL;
	}

	buflen = sizeof(addrbuf);

	/* 1st vector is just first address */
	ret = av_create_address_list(good_address, 0, 1, addrbuf, 0, buflen);
	if (ret < 0) {
		goto fail;		// av_create_address_list filled err_buf
	}
	ret = fi_av_insert(av, addrbuf, 1, fi_addr, FI_MORE, &ctx[0]);
	if (ret) {
		sprintf(err_buf, "fi_av_insert ret=%d, %s", ret, fi_strerror(-ret));
		goto fail;
	}
	ctx[0] = 1;

	/* 2nd vector is remaining addresses */
	ret = av_create_address_list(good_address, 1, num_good_addr-1,
			addrbuf, 0, buflen);
	if (ret < 0) {
		goto fail;		// av_create_address_list filled err_buf
	}
	ret = fi_av_insert(av, addrbuf, num_good_addr-1, &fi_addr[1], 0, &ctx[1]);
	if (ret != num_good_addr-1) {
		sprintf(err_buf, "fi_av_insert ret=%d, %s", ret, fi_strerror(-ret));
		goto fail;
	}
	ctx[1] = num_good_addr-1;

	/*
	 * Handle completions in either order
	 */
	for (i = 0; i < 2; ++i) {
		ret = fi_eq_sread(eq, &event, &entry, sizeof(entry), 20000, 0);
		ret = check_eq_result(ret, event, &entry, &av->fid, NULL, ~0);
		if (ret != 0) {
			goto fail;
		}
		if (entry.context != &ctx[0] && entry.context != &ctx[1]) {
			sprintf(err_buf, "bad context: %p", entry.context);
			goto fail;
		}
		if (*(uint32_t *)(entry.context) == ~0) {
			sprintf(err_buf, "duplicate context: %p", entry.context);
			goto fail;
		}
		if (*(uint32_t *)(entry.context) != entry.data) {
			sprintf(err_buf, "count = %" PRIu64 ", should be %d", entry.data,
					*(uint32_t *)(entry.context));
			goto fail;
		}
		*(uint32_t *)(entry.context) = ~0;
	}
	for (i = 0; i < num_good_addr; ++i) {
		if (fi_addr[i] == FI_ADDR_NOTAVAIL) {
			sprintf(err_buf, "fi_addr[%d] = FI_ADDR_NOTAVAIL", i);
			goto fail;
		}
	}

	testret = PASS;
fail:
	FT_CLOSE_FID(av);
	return TEST_RET_VAL(ret, testret);
}

/*
 * Tests:
 * - async vector with 1 good and 1 bad
 */
static int
av_goodbad_vector_async()
{
	int testret;
	int ret;
	int i;
	struct fid_av *av;
	struct fi_av_attr attr;
	uint8_t addrbuf[4096];
	uint32_t event;
	uint32_t ctx;
	struct fi_eq_entry entry;
	int buflen;
	fi_addr_t fi_addr[MAX_ADDR];

	testret = FAIL;

	memset(&attr, 0, sizeof(attr));
	attr.type = av_type;
	attr.count = 32;
	attr.flags = FI_EVENT;

	av = NULL;
	ret = fi_av_open(domain, &attr, &av, NULL);
	if (ret != 0) {
		sprintf(err_buf, "fi_av_open(%s) = %d, %s",
				fi_tostr(&av_type, FI_TYPE_AV_TYPE),
				ret, fi_strerror(-ret));
		goto fail;
	}
	ret = fi_av_bind(av, &eq->fid, 0);
	if (ret != 0) {
		sprintf(err_buf, "fi_av_bind() = %d, %s", ret, fi_strerror(-ret));
		goto fail;
	}

	for (i = 0; i < MAX_ADDR; ++i) {
		fi_addr[i] = FI_ADDR_NOTAVAIL;
	}
	fi_addr[1] = ~FI_ADDR_NOTAVAIL;

	buflen = sizeof(addrbuf);

	/* vector is good address + bad address */
	ret = av_create_address_list(good_address, 0, 1, addrbuf, 0, buflen);
	if (ret < 0) {
		goto fail;		// av_create_address_list filled err_buf
	}
	ret = av_create_address_list(bad_address, 0, 1, addrbuf, 1, buflen);
	if (ret < 0) {
		goto fail;		// av_create_address_list filled err_buf
	}
	ret = fi_av_insert(av, addrbuf, 2, fi_addr, 0, &ctx);
	if (ret) {
		sprintf(err_buf, "fi_av_insert ret=%d, %s", ret, fi_strerror(-ret));
		goto fail;
	}

	/*
	 * Read event after sync, verify we get FI_EAVAIL, then read and
	 * verify the error completion
	 */
	ret = fi_eq_sread(eq, &event, &entry, sizeof(entry), 20000, 0);
	if (ret != -FI_EAVAIL) {
		sprintf(err_buf, "fi_eq_sread ret = %d, should be -FI_EAVAIL", ret);
		goto fail;
	}
	ret = check_eq_readerr(eq, &av->fid, &ctx, 1);
	if (ret != 0) {
		goto fail;
	}

	/*
	 * Now we should get a good completion, and all fi_addr except fd_addr[1]
	 * should have good values.
	 */
	if (check_eq_sread(eq, &av->fid, &ctx, 1, 20000, 0) != 0) {
		goto fail;
	}
	if (fi_addr[0] == FI_ADDR_NOTAVAIL) {
		sprintf(err_buf, "fi_addr[0] = FI_ADDR_NOTAVAIL");
		goto fail;
	}
	if (fi_addr[1] != FI_ADDR_NOTAVAIL) {
		sprintf(err_buf, "fi_addr[1] != FI_ADDR_NOTAVAIL");
		goto fail;
	}

	testret = PASS;
fail:
	FT_CLOSE_FID(av);
	return TEST_RET_VAL(ret, testret);
}

/*
 * Test AV insert at different stages
 */
static int
av_insert_stages(void)
{
	int testret, ret, i, count = 0;
	struct fid_av *av;
	struct fi_av_attr attr;
	uint8_t addrbuf[4096];
	uint32_t ctx;
	int buflen;
	fi_addr_t fi_addr[MAX_ADDR], *fi_addr_cur = NULL;
	ssize_t addrlen;

	testret = FAIL;

	memset(&attr, 0, sizeof(attr));
	attr.type = av_type;
	attr.count = 32;

	av = NULL;
	ret = fi_av_open(domain, &attr, &av, NULL);
	if (ret != 0) {
		sprintf(err_buf, "fi_av_open(%s) = %d, %s",
				fi_tostr(&av_type, FI_TYPE_AV_TYPE),
				ret, fi_strerror(-ret));
		goto fail;
	}

	addrlen = av_get_addrlen(fi);
	if (addrlen < 0) {
		ret = addrlen;
		goto fail;
	}

	for (i = 0; i < MAX_ADDR; ++i) {
		fi_addr[i] = FI_ADDR_NOTAVAIL;
	}

	buflen = sizeof(addrbuf);

	ret = av_create_address_list(good_address, 0, 6, addrbuf, 0, buflen);
	if (ret < 0)
		goto fail;

	if (av_type != FI_AV_TABLE)
		fi_addr_cur = fi_addr + count;

	ret = fi_av_insert(av, addrbuf + count * addrlen, 2,
			   fi_addr_cur, 0, &ctx);
	if (ret != 2) {
		sprintf(err_buf, "fi_av_insert ret=%d, %s", ret, fi_strerror(-ret));
		goto fail;
	}
	count += 2;

	if (av_type != FI_AV_TABLE) {
		for (i = 0; i < count; ++i) {
			if (fi_addr[i] == FI_ADDR_NOTAVAIL) {
				sprintf(err_buf, "fi_addr[%d] == FI_ADDR_NOTAVAIL", i);
				goto fail;
			}
		}
	}

	ret = fi_endpoint(domain, fi, &ep, NULL);
	if (ret) {
		sprintf(err_buf, "fi_endpoint=%d, %s", ret, fi_strerror(-ret));
		goto fail;
	}

	ret = fi_ep_bind(ep, &av->fid, 0);
	if (ret) {
		sprintf(err_buf, "fi_ep_bind=%d, %s", ret, fi_strerror(-ret));
		goto fail;
	}

	if (av_type != FI_AV_TABLE)
		fi_addr_cur = fi_addr + count;

	ret = fi_av_insert(av, addrbuf + count * addrlen, 2,
			   fi_addr_cur, 0, &ctx);
	if (ret != 2) {
		sprintf(err_buf, "fi_av_insert ret=%d, %s", ret, fi_strerror(-ret));
		goto fail;
	}
	count += 2;

	if (av_type != FI_AV_TABLE) {
		for (i = 2; i < count; ++i) {
			if (fi_addr[i] == FI_ADDR_NOTAVAIL) {
				sprintf(err_buf, "fi_addr[%d] == FI_ADDR_NOTAVAIL", i);
				goto fail;
			}
		}
	}

	// TODO test av insert after endpoint enable
	testret = PASS;
fail:
	FT_CLOSE_FID(ep);
	FT_CLOSE_FID(av);
	return TEST_RET_VAL(ret, testret);
}

struct test_entry test_array_good[] = {
	TEST_ENTRY(av_open_close, "Test open and close AVs of varying sizes"),
	TEST_ENTRY(av_good_sync, "Test sync AV insert with good address"),
	TEST_ENTRY(av_null_fi_addr, "Test AV insert without specifying fi_addr"),
	TEST_ENTRY(av_good_vector_async,
		   "Test async AV insert with vector of good addresses"),
	TEST_ENTRY(av_zero_async, "Test async insert AV insert of zero addresses"),
	TEST_ENTRY(av_good_2vector_async,
		   "Test async AV inserts with two address vectors"),
	TEST_ENTRY(av_insert_stages, "Test AV insert at various stages"),
	{ NULL, "" }
};

struct test_entry test_array_bad[] = {
	TEST_ENTRY(av_bad_sync, "Test sync AV insert of bad address"),
	TEST_ENTRY(av_goodbad_vector_sync,
		   "Test sync AV insert of 1 good and 1 bad address"),
	TEST_ENTRY(av_goodbad_vector_async,
		   "Test async AV insert with good and bad address"),
	TEST_ENTRY(av_goodbad_vector_sync_err,
		   "Test AV insert of 1 good, 1 bad address using FI_SYNC_ERR"),
	{ NULL, "" }
};

int
run_test_set()
{
	int failed;

	failed = 0;

	failed += run_tests(test_array_good, err_buf);
	if (bad_address != NULL) {
		printf("\nTesting with bad_address = \"%s\"\n", bad_address);
		failed += run_tests(test_array_bad, err_buf);
	}

	bad_address = NULL;
	printf("\nTesting with invalid address\n");
	failed += run_tests(test_array_bad, err_buf);

	return failed;
}

static void usage(void)
{
	ft_unit_usage("av_test", "Unit test for Address Vector (AV)");
	FT_PRINT_OPTS_USAGE("-g <good_address>", "");
	FT_PRINT_OPTS_USAGE("-G <bad_address>]", "");
	fprintf(stderr, FT_OPTS_USAGE_FORMAT " (max=%d)\n", "-n <num_good_addr>",
			"Number of good addresses", MAX_ADDR - 1);
	FT_PRINT_OPTS_USAGE("-s <source_address>", "");
}

int main(int argc, char **argv)
{
	int op, ret;
	int failed;

	opts = INIT_OPTS;
	opts.options |= FT_OPT_SIZE;

	hints = fi_allocinfo();
	if (!hints)
		return EXIT_FAILURE;

	while ((op = getopt(argc, argv, FAB_OPTS "g:G:n:s:h")) != -1) {
		switch (op) {
		case 'g':
			good_address = optarg;
			break;
		case 'G':
			bad_address = optarg;
			break;
		case 'n':
			num_good_addr = atoi(optarg);
			break;
		case 's':
			opts.src_addr = optarg;
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

	if (good_address == NULL ||  num_good_addr == 0) {
		printf("Test requires -g and -n\n");
		return EXIT_FAILURE;
	}

	if (num_good_addr > MAX_ADDR - 1) {
		printf("num_good_addr = %d is too big, dropped to %d\n",
				num_good_addr, MAX_ADDR);
		num_good_addr = MAX_ADDR - 1;
	}

	hints->mode = ~0;
	hints->domain_attr->mode = ~0;
	hints->domain_attr->mr_mode = ~(FI_MR_BASIC | FI_MR_SCALABLE);
	hints->addr_format = FI_SOCKADDR;

	// TODO make this test accept endpoint type argument
	hints->ep_attr->type = FI_EP_RDM;
	ret = fi_getinfo(FT_FIVERSION, opts.src_addr, 0, FI_SOURCE, hints, &fi);

	if (ret && ret != -FI_ENODATA) {
		FT_PRINTERR("fi_getinfo", ret);
		goto err;
	}

	if (ret == -FI_ENODATA) {
		hints->ep_attr->type = FI_EP_DGRAM;
		ret = fi_getinfo(FT_FIVERSION, opts.src_addr, 0, FI_SOURCE, hints, &fi);
		if (ret) {
			FT_PRINTERR("fi_getinfo", ret);
			goto err;
		}
	}

	ret = ft_open_fabric_res();
	if (ret)
		goto err;

	printf("Testing AVs on fabric %s\n", fi->fabric_attr->name);
	failed = 0;

	if (fi->domain_attr->av_type == FI_AV_UNSPEC ||
	    fi->domain_attr->av_type == FI_AV_MAP) {
		av_type = FI_AV_MAP;
		printf("\nTesting with type = FI_AV_MAP\n");
		failed += run_test_set();
	}

	if (fi->domain_attr->av_type == FI_AV_UNSPEC ||
	    fi->domain_attr->av_type == FI_AV_TABLE) {
		av_type = FI_AV_TABLE;
		printf("\nTesting with type = FI_AV_TABLE\n");
		failed += run_test_set();
	}

	if (failed > 0) {
		printf("\nSummary: %d tests failed\n", failed);
	} else {
		printf("\nSummary: all tests passed\n");
	}

err:
	ft_free_res();
	return ret ? ft_exit_code(ret) : (failed > 0) ? EXIT_FAILURE : EXIT_SUCCESS;
}
