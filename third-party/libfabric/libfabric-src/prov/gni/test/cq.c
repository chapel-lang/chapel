/*
 * Copyright (c) 2015-2017 Los Alamos National Security, LLC.
 *                         All rights reserved.
 * Copyright (c) 2015-2017 Cray Inc. All rights reserved.
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
#include <time.h>
#include <string.h>


#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#include "gnix_cq.h"
#include "gnix.h"

#include <criterion/criterion.h>
#include "gnix_rdma_headers.h"
#include "common.h"

/* Note: Set to ~FI_NOTIFY_FLAGS_ONLY since this was written before api 1.5 */
static uint64_t mode_bits = ~FI_NOTIFY_FLAGS_ONLY;
static uint64_t old_mode_bits;
static struct fid_fabric *fab;
static struct fid_domain *dom;
static struct gnix_fid_ep *ep;
static struct fid_ep *fid_ep;
static struct fid_cq *rcq;
static struct fi_info *hints;
static struct fi_info *fi;
static struct fi_cq_attr cq_attr;
static struct gnix_fid_cq *cq_priv;

static struct gnix_fid_wait *wait_priv;
static struct fid_wait *wait_set;
static struct fi_wait_attr wait_attr;

static void _setup(uint32_t version)
{
	int ret = 0;

	hints = fi_allocinfo();
	cr_assert(hints, "fi_allocinfo");
	if (FI_VERSION_LT(version, FI_VERSION(1, 5)))
		hints->domain_attr->mr_mode = FI_MR_BASIC;
	else
		hints->domain_attr->mr_mode = GNIX_DEFAULT_MR_MODE;
	hints->domain_attr->cq_data_size = 4;
	hints->mode = mode_bits;

	hints->fabric_attr->prov_name = strdup("gni");

	ret = fi_getinfo(version, NULL, 0, 0, hints, &fi);
	cr_assert(!ret, "fi_getinfo");

	ret = fi_fabric(fi->fabric_attr, &fab, NULL);
	cr_assert(!ret, "fi_fabric");

	ret = fi_domain(fab, fi, &dom, NULL);
	cr_assert(!ret, "fi_domain");

	cq_attr.wait_obj = FI_WAIT_NONE;
}

static void setup(void)
{
	_setup(fi_version());
}

static void setup_1_4(void)
{
	_setup(FI_VERSION(1, 4));
}

static void teardown(void)
{
	int ret = 0;

	ret = fi_close(&dom->fid);
	cr_assert(!ret, "failure in closing domain.");
	ret = fi_close(&fab->fid);
	cr_assert(!ret, "failure in closing fabric.");
	fi_freeinfo(fi);
	fi_freeinfo(hints);
}

void cq_create(enum fi_cq_format format, enum fi_wait_obj wait_obj,
	       size_t size)
{
	int ret = 0;

	cq_attr.format = format;
	cq_attr.size = size;
	cq_attr.wait_obj = wait_obj;

	ret = fi_cq_open(dom, &cq_attr, &rcq, NULL);
	cr_assert(!ret, "fi_cq_open");

	cq_priv = container_of(rcq, struct gnix_fid_cq, cq_fid);

	if (cq_priv->wait) {
		wait_priv = container_of(cq_priv->wait, struct gnix_fid_wait,
					 wait);
	}
}

void cq_setup(void)
{
	setup();
	cq_create(FI_CQ_FORMAT_UNSPEC, FI_WAIT_NONE, 0);
}

void cq_setup_1_4(void)
{
	setup_1_4();
	cq_create(FI_CQ_FORMAT_UNSPEC, FI_WAIT_NONE, 0);
}

void cq_msg_setup(void)
{
	setup();
	cq_create(FI_CQ_FORMAT_MSG, FI_WAIT_NONE, 8);
}

void cq_data_setup(void)
{
	setup();
	cq_create(FI_CQ_FORMAT_DATA, FI_WAIT_NONE, 8);
}

void cq_tagged_setup(void)
{
	setup();
	cq_create(FI_CQ_FORMAT_TAGGED, FI_WAIT_NONE, 8);
}

void cq_wait_none_setup(void)
{
	setup();
	cq_create(FI_CQ_FORMAT_MSG, FI_WAIT_NONE, 8);
}

void cq_wait_fd_setup(void)
{
	setup();
	cq_create(FI_CQ_FORMAT_MSG, FI_WAIT_FD, 8);
}

void cq_wait_unspec_setup(void)
{
	setup();
	cq_create(FI_CQ_FORMAT_MSG, FI_WAIT_UNSPEC, 8);
}

void cq_wait_mutex_cond_setup(void)
{
	setup();
	cq_create(FI_CQ_FORMAT_MSG, FI_WAIT_MUTEX_COND, 8);
}

void cq_notify_setup(void)
{
	int ret;
	old_mode_bits = mode_bits;
	mode_bits = FI_NOTIFY_FLAGS_ONLY;
	setup();

	ret = fi_endpoint(dom, fi, &fid_ep, NULL);
	cr_assert(!ret, "fi_endpoint");
	cr_assert(fid_ep != NULL);

	ep = container_of(fid_ep, struct gnix_fid_ep, ep_fid);
	cr_assert(ep, "ep not allocated");

	cq_create(FI_CQ_FORMAT_MSG, FI_WAIT_NONE, 0);
}

void cq_teardown(void)
{
	cr_assert(!fi_close(&rcq->fid), "failure in closing cq.");
	teardown();
}

void cq_notify_teardown(void)
{
	cr_assert(!fi_close(&rcq->fid), "failure in closing cq.");
	cr_assert(!fi_close(&fid_ep->fid), "failure in closing ep.");
	teardown();
	mode_bits = old_mode_bits;

}

/*******************************************************************************
 * Creation Tests:
 *
 * Create the CQ with various parameters and make sure the fields are being
 * initialized correctly.
 ******************************************************************************/

TestSuite(creation, .init = setup, .fini = cq_teardown);

Test(creation, format_unspec)
{
	int ret = 0;

	cq_attr.format = FI_CQ_FORMAT_UNSPEC;

	ret = fi_cq_open(dom, &cq_attr, &rcq, NULL);
	cr_assert(!ret, "fi_cq_open");

	cq_priv = container_of(rcq, struct gnix_fid_cq, cq_fid);
	cr_assert(cq_priv->entry_size == sizeof(struct fi_cq_entry));
}

Test(creation, format_context)
{
	int ret = 0;

	cq_attr.format = FI_CQ_FORMAT_CONTEXT;

	ret = fi_cq_open(dom, &cq_attr, &rcq, NULL);
	cr_assert(!ret, "fi_cq_open");

	cq_priv = container_of(rcq, struct gnix_fid_cq, cq_fid);
	cr_assert(cq_priv->entry_size == sizeof(struct fi_cq_entry));
}

Test(creation, format_msg)
{
	int ret = 0;

	cq_attr.format = FI_CQ_FORMAT_MSG;

	ret = fi_cq_open(dom, &cq_attr, &rcq, NULL);
	cr_assert(!ret, "fi_cq_open");

	cq_priv = container_of(rcq, struct gnix_fid_cq, cq_fid);
	cr_assert(cq_priv->entry_size == sizeof(struct fi_cq_msg_entry));
}

Test(creation, format_data)
{
	int ret = 0;

	cq_attr.format = FI_CQ_FORMAT_DATA;

	ret = fi_cq_open(dom, &cq_attr, &rcq, NULL);
	cr_assert(!ret, "fi_cq_open");

	cq_priv = container_of(rcq, struct gnix_fid_cq, cq_fid);
	cr_assert(cq_priv->entry_size == sizeof(struct fi_cq_data_entry));
}

Test(creation, format_tagged)
{
	int ret = 0;

	cq_attr.format = FI_CQ_FORMAT_TAGGED;

	ret = fi_cq_open(dom, &cq_attr, &rcq, NULL);
	cr_assert(!ret, "fi_cq_open");

	cq_priv = container_of(rcq, struct gnix_fid_cq, cq_fid);
	cr_assert(cq_priv->entry_size == sizeof(struct fi_cq_tagged_entry));
}

TestSuite(insertion, .init = cq_setup, .fini = cq_teardown);

Test(insertion, single)
{
	int ret = 0;
	char input_ctx = 'a';
	struct fi_cq_entry entry;

	cr_assert(!cq_priv->events->item_list.head);

	_gnix_cq_add_event(cq_priv, NULL, &input_ctx, 0, 0, 0, 0, 0, 0);

	cr_assert(cq_priv->events->item_list.head);
	cr_assert_eq(cq_priv->events->item_list.head,
		     cq_priv->events->item_list.tail);

	ret = fi_cq_read(rcq, &entry, 1);
	cr_assert(ret == 1);
	cr_assert(!cq_priv->events->item_list.head);

	cr_assert_eq(*(char *) entry.op_context, input_ctx,
		     "Expected same op_context as inserted.");
}

Test(insertion, limit)
{
	int ret = 0;
	char input_ctx = 'a';
	struct fi_cq_entry entry;
	const size_t cq_size = cq_priv->attr.size;

	for (size_t i = 0; i < cq_size; i++)
		_gnix_cq_add_event(cq_priv, NULL, &input_ctx, 0, 0, 0, 0, 0, 0);

	cr_assert(cq_priv->events->item_list.head);
	cr_assert(!cq_priv->events->free_list.head);

	_gnix_cq_add_event(cq_priv, NULL, &input_ctx, 0, 0, 0, 0, 0, 0);

	for (size_t i = 0; i < cq_size + 1; i++) {
		ret = fi_cq_read(rcq, &entry, 1);
		cr_assert_eq(ret, 1);
	}

	cr_assert(!cq_priv->events->item_list.head);
	cr_assert(cq_priv->events->free_list.head);
}

TestSuite(mode_bits, .init = NULL, .fini = teardown);

Test(mode_bits, fi_notify_flags_only_1_4)
{
	old_mode_bits = mode_bits;
	mode_bits = ~0;
	_setup(FI_VERSION(1, 4));
	cr_assert_eq(fi->mode & FI_NOTIFY_FLAGS_ONLY, 0, "Did not clear notify flag for version 1.4");
	mode_bits = old_mode_bits;
}

Test(mode_bits, fi_notify_flags_only)
{
	old_mode_bits = mode_bits;
	mode_bits = ~0;
	_setup(fi_version());
	cr_assert(fi->mode & FI_NOTIFY_FLAGS_ONLY, "Cleared the notify flag when we shouldn't have\n");
	mode_bits = old_mode_bits;
}

TestSuite(reading, .init = cq_setup, .fini = cq_teardown);

Test(reading, empty)
{
	int ret = 0;
	struct fi_cq_entry entry;

	ret = fi_cq_read(rcq, &entry, 1);
	cr_assert_eq(ret, -FI_EAGAIN);
}

Test(reading, error)
{
	int ret = 0;
	struct fi_cq_entry entry;
	struct fi_cq_err_entry err_entry;

	char input_ctx = 'a';
	uint64_t flags = 0xb;
	size_t len = sizeof(input_ctx);
	void *buf = &input_ctx;
	uint64_t data = 20;
	uint64_t tag = 40;
	size_t olen = 20;
	int err = 50;
	int prov_errno = 80;

	/*
	 * By default CQ start out with no error entries and no entries
	 * in the error entry free list.
	 */
	cr_assert(!cq_priv->errors->item_list.head);
	cr_assert(!cq_priv->errors->free_list.head);

	_gnix_cq_add_error(cq_priv, &input_ctx, flags, len, buf, data, tag,
			   olen, err, prov_errno, 0, 0);

	cr_assert(cq_priv->errors->item_list.head);

	ret = fi_cq_read(rcq, &entry, 1);
	cr_assert_eq(ret, -FI_EAVAIL);

	cr_assert(!cq_priv->events->item_list.head);
	cr_assert(cq_priv->errors->item_list.head);
	/* Testing err_data == NULL path set size to something
	 * other than 0 then verify it was set back to 0 */
	err_entry.err_data_size = 12;
	err_entry.err_data = malloc(12);
	ret = fi_cq_readerr(rcq, &err_entry, 0);
	cr_assert_eq(ret, 1);

	/*
	 * Item should have been removed from error queue and placed on free
	 * queue.
	 */
	cr_assert(!cq_priv->errors->item_list.head);
	cr_assert(cq_priv->errors->free_list.head);

	/*
	 * Compare structural items...
	 */
	cr_assert_eq(*(char *) err_entry.op_context, input_ctx);
	cr_assert_eq(err_entry.flags, flags);
	cr_assert_eq(err_entry.len, len);
	cr_assert_eq(err_entry.buf, buf);
	cr_assert_eq(err_entry.data, data);
	cr_assert_eq(err_entry.tag, tag);
	cr_assert_eq(err_entry.olen, olen);
	cr_assert_eq(err_entry.err, err);
	cr_assert_eq(err_entry.prov_errno, prov_errno);
	cr_assert(err_entry.err_data != NULL);
	free(err_entry.err_data);
	cr_assert(err_entry.err_data_size == 0);
}

TestSuite(reading_1_4, .init = cq_setup_1_4, .fini = cq_teardown);

Test(reading_1_4, error)
{
	int ret = 0;
	struct fi_cq_entry entry;
	struct fi_cq_err_entry err_entry;

	char input_ctx = 'a';
	uint64_t flags = 0xb;
	size_t len = sizeof(input_ctx);
	void *buf = &input_ctx;
	uint64_t data = 20;
	uint64_t tag = 40;
	size_t olen = 20;
	int err = 50;
	int prov_errno = 80;

	/*
	 * By default CQ start out with no error entries and no entries
	 * in the error entry free list.
	 */
	cr_assert(!cq_priv->errors->item_list.head);
	cr_assert(!cq_priv->errors->free_list.head);

	_gnix_cq_add_error(cq_priv, &input_ctx, flags, len, buf, data, tag,
			   olen, err, prov_errno, 0, 0);

	cr_assert(cq_priv->errors->item_list.head);

	ret = fi_cq_read(rcq, &entry, 1);
	cr_assert_eq(ret, -FI_EAVAIL);

	cr_assert(!cq_priv->events->item_list.head);
	cr_assert(cq_priv->errors->item_list.head);
	ret = fi_cq_readerr(rcq, &err_entry, 0);
	cr_assert_eq(ret, 1);

	/*
	 * Item should have been removed from error queue and placed on free
	 * queue.
	 */
	cr_assert(!cq_priv->errors->item_list.head);
	cr_assert(cq_priv->errors->free_list.head);

	/*
	 * Compare structural items...
	 */
	cr_assert_eq(*(char *) err_entry.op_context, input_ctx);
	cr_assert_eq(err_entry.flags, flags);
	cr_assert_eq(err_entry.len, len);
	cr_assert_eq(err_entry.buf, buf);
	cr_assert_eq(err_entry.data, data);
	cr_assert_eq(err_entry.tag, tag);
	cr_assert_eq(err_entry.olen, olen);
	cr_assert_eq(err_entry.err, err);
	cr_assert_eq(err_entry.prov_errno, prov_errno);
	cr_assert(err_entry.err_data == NULL);
}

#define ENTRY_CNT 5
Test(reading, issue192)
{
	int ret = 0;
	char input_ctx = 'a';
	struct fi_cq_entry entries[ENTRY_CNT];

	_gnix_cq_add_event(cq_priv, NULL, &input_ctx, 0, 0, 0, 0, 0, 0);

	ret = fi_cq_read(rcq, &entries, ENTRY_CNT);
	cr_assert_eq(ret, 1);

	ret = fi_cq_read(rcq, &entries, ENTRY_CNT);
	cr_assert_eq(ret, -FI_EAGAIN);
}


static void cq_add_read_setup(enum fi_cq_format format)
{
	switch (format) {
	default:
	case FI_CQ_FORMAT_UNSPEC:
		cq_setup();
		break;
	case FI_CQ_FORMAT_MSG:
		cq_msg_setup();
		break;
	case FI_CQ_FORMAT_DATA:
		cq_data_setup();
		break;
	case FI_CQ_FORMAT_TAGGED:
		cq_tagged_setup();
		break;
	}
}

static void cq_add_read_check(enum fi_cq_format format,
			      struct fi_cq_tagged_entry *entry,
			      struct fi_cq_tagged_entry *expected)
{
	cr_assert_eq(*(char *) entry->op_context,
		     *(char *) expected->op_context);

	if (format == FI_CQ_FORMAT_UNSPEC ||
	    format == FI_CQ_FORMAT_CONTEXT) {
		return;
	}

	cr_assert_eq(entry->flags, expected->flags);
	cr_assert_eq(entry->len, expected->len);

	if (format == FI_CQ_FORMAT_MSG) {
		return;
	}

	cr_assert_eq(entry->buf, expected->buf);
	cr_assert_eq(entry->data, expected->data);

	if (format == FI_CQ_FORMAT_DATA) {
		return;
	}

	cr_assert_eq(entry->tag, expected->tag);
}

/*
 * Add an event and read the cq.
 */
static void cq_add_read(enum fi_cq_format format)
{
	int ret = 0;
	char input_ctx = 'a';
	struct fi_cq_tagged_entry entry; /* biggest one */
	struct fi_cq_tagged_entry expected = { &input_ctx, 2, 4, (void *) 8,
					       16, 32 };

	cq_add_read_setup(format);

	cr_assert(!cq_priv->events->item_list.head);

	_gnix_cq_add_event(cq_priv, NULL, expected.op_context, expected.flags,
			   expected.len, expected.buf, expected.data,
			   expected.tag, 0x0);

	cr_assert(cq_priv->events->item_list.head);

	ret = fi_cq_read(rcq, &entry, 1);
	cr_assert_eq(ret, 1);

	cq_add_read_check(format, &entry, &expected);

	cr_assert(!cq_priv->events->item_list.head);
}

/*
 * Create up to the size events to fill it up. Check that all the properties
 * are correct, then add one more that is different. Read size items and then
 * add an error and try reading. Ensure that we get back -FI_EAVAIL. Then read
 * the last item and make sure it's the same values put in originally.
 */
static void cq_fill_test(enum fi_cq_format format)
{
	char input_ctx = 'a';
	struct fi_cq_tagged_entry entry; /* biggest one */
	struct fi_cq_tagged_entry expected = { &input_ctx, 2, 4, (void *) 8,
					       16, 32 };
	struct fi_cq_err_entry err_entry;
	int ret = 0;
	uint64_t flags = 2;
	size_t len = 4;
	size_t cq_size;

	cq_add_read_setup(format);

	cr_assert(!cq_priv->events->item_list.head);
	cr_assert(cq_priv->events->free_list.head);

	cq_size = cq_priv->attr.size;
	for (size_t i = 0; i < cq_size; i++) {
		_gnix_cq_add_event(cq_priv, NULL, expected.op_context,
				   expected.flags, expected.len,
				   expected.buf, expected.data,
				   expected.tag, 0x0);
	}

	cr_assert(cq_priv->events->item_list.head);
	cr_assert(!cq_priv->events->free_list.head);

	_gnix_cq_add_event(cq_priv, NULL, expected.op_context,
			   expected.flags, 2 * expected.len, expected.buf,
			   expected.data, expected.tag, 0x0);

	for (size_t i = 0; i < cq_size; i++) {
		ret = fi_cq_read(rcq, &entry, 1);
		cr_assert_eq(ret, 1);
		cq_add_read_check(format, &entry, &expected);
	}

	/*
	 * If we insert an error it should return -FI_EAVAIL despite having
	 * something to read.
	 */

	_gnix_cq_add_error(cq_priv, &input_ctx, flags, len, 0, 0, 0, 0, 0, 0,
			   0, 0);
	cr_assert(cq_priv->errors->item_list.head);

	ret = fi_cq_read(rcq, &entry, 1);
	cr_assert_eq(ret, -FI_EAVAIL);

	ret = fi_cq_readerr(rcq, &err_entry, 0);
	cr_assert_eq(ret, 1);

	/*
	 * Creating an error allocs an error but it is then placed in the free
	 * list after reading.
	 */
	cr_assert(cq_priv->errors->free_list.head);
	cr_assert(!cq_priv->errors->item_list.head);

	ret = fi_cq_read(rcq, &entry, 1);
	cr_assert_eq(ret, 1);

	cr_assert(cq_priv->events->free_list.head);
	cr_assert(!cq_priv->events->item_list.head);

	expected.len *= 2;
	cq_add_read_check(format, &entry, &expected);
}

/*
 * Read more than one cqe at a time.
 */
static void cq_multi_read_test(enum fi_cq_format format)
{
	int ret = 0;
	size_t count = 3;
	char input_ctx = 'a';
	struct fi_cq_tagged_entry entry[count]; /* biggest one */
	struct fi_cq_tagged_entry expected = { &input_ctx, 2, 4, (void *) 8,
					       16, 32 };

	cq_add_read_setup(format);

	cr_assert(cq_priv->events->free_list.head);
	cr_assert(!cq_priv->events->item_list.head);

	for (size_t i = 0; i < count; i++) {
		_gnix_cq_add_event(cq_priv, NULL, expected.op_context,
				   expected.flags, expected.len,
				   expected.buf, expected.data,
				   expected.tag, 0x0);
	}

	cr_assert(cq_priv->events->item_list.head);

	ret = fi_cq_read(rcq, &entry, count);
	cr_assert_eq(ret, count);

	for (size_t j = 0; j < count; j++) {
		/* This is gross */
		switch (format) {
		default:
		case FI_CQ_FORMAT_UNSPEC:
		{
			struct fi_cq_entry *e = (struct fi_cq_entry *) entry;

			cq_add_read_check(format,
					  (struct fi_cq_tagged_entry *) &e[j],
					  &expected);
			break;
		}
		case FI_CQ_FORMAT_MSG:
		{
			struct fi_cq_msg_entry *e =
				(struct fi_cq_msg_entry *) entry;

			cq_add_read_check(format,
					  (struct fi_cq_tagged_entry *) &e[j],
					  &expected);
			break;
		}
		case FI_CQ_FORMAT_DATA:
		{
			struct fi_cq_data_entry *e =
				(struct fi_cq_data_entry *) entry;

			cq_add_read_check(format,
					  (struct fi_cq_tagged_entry *) &e[j],
					  &expected);
			break;
		}
		case FI_CQ_FORMAT_TAGGED:
		{
			cq_add_read_check(format, &entry[j], &expected);
			break;
		}
		}
	}
}

TestSuite(check_cqe, .init = NULL, .fini = cq_teardown);

Test(check_cqe, context) {
	cq_add_read(FI_CQ_FORMAT_CONTEXT);
}

Test(check_cqe, context_fill) {
	cq_fill_test(FI_CQ_FORMAT_CONTEXT);
}

Test(check_cqe, context_multi_read) {
	cq_multi_read_test(FI_CQ_FORMAT_CONTEXT);
}

Test(check_cqe, msg) {
	cq_add_read(FI_CQ_FORMAT_MSG);
}

Test(check_cqe, msg_fill) {
	cq_fill_test(FI_CQ_FORMAT_MSG);
}

Test(check_cqe, msg_multi_read) {
	cq_multi_read_test(FI_CQ_FORMAT_MSG);
}

Test(check_cqe, data) {
	cq_add_read(FI_CQ_FORMAT_DATA);
}

Test(check_cqe, data_fill) {
	cq_fill_test(FI_CQ_FORMAT_DATA);
}

Test(check_cqe, data_multi_read) {
	cq_multi_read_test(FI_CQ_FORMAT_DATA);
}

Test(check_cqe, tagged) {
	cq_add_read(FI_CQ_FORMAT_TAGGED);
}

Test(check_cqe, tagged_fill) {
	cq_fill_test(FI_CQ_FORMAT_TAGGED);
}

Test(check_cqe, tagged_multi_read) {
	cq_multi_read_test(FI_CQ_FORMAT_TAGGED);
}

/* This test should be combined with cq_multi_read_test above when
 * wait object are implemented.
 */
Test(cq_msg, multi_sread, .init = cq_wait_unspec_setup, .disabled = false)
{
	int ret = 0;
	size_t count = 3;
	struct fi_cq_msg_entry entry[count];

	cr_assert(cq_priv->events->free_list.head);
	cr_assert(!cq_priv->events->item_list.head);

	ret = fi_cq_sread(rcq, &entry, count, NULL, 100);
	cr_assert_eq(ret, -FI_EAGAIN);

	for (size_t i = 0; i < count; i++)
		_gnix_cq_add_event(cq_priv, NULL, 0, (uint64_t) i, 0, 0, 0, 0, 0);

	cr_assert(cq_priv->events->item_list.head);

	ret = fi_cq_sread(rcq, &entry, count, NULL, -1);
	cr_assert_eq(ret, count);

	for (size_t j = 0; j < count; j++)
		cr_assert_eq(entry[j].flags, (uint64_t) j);
}

TestSuite(cq_wait_obj, .fini = cq_teardown);
TestSuite(cq_wait_control, .fini = cq_teardown, .disabled = true);
TestSuite(cq_wait_ops, .fini = cq_teardown);

Test(cq_wait_obj, none, .init = cq_wait_none_setup)
{
	cr_expect(!wait_priv, "wait_priv is not null.");
}

Test(cq_wait_obj, unspec, .init = cq_wait_unspec_setup)
{
	cr_expect_eq(wait_priv->type, FI_WAIT_UNSPEC);
	cr_expect_eq(wait_priv->type, cq_priv->attr.wait_obj);
	cr_expect_eq(wait_priv->type, cq_attr.wait_obj);
	cr_expect_eq(&wait_priv->fabric->fab_fid, fab);
	cr_expect_eq(wait_priv->cond_type, FI_CQ_COND_NONE);
}

Test(cq_wait_obj, fd, .init = cq_wait_fd_setup, .disabled = true)
{
	cr_expect_eq(wait_priv->type, FI_WAIT_FD);
	cr_expect_eq(wait_priv->type, cq_priv->attr.wait_obj);
	cr_expect_eq(wait_priv->type, cq_attr.wait_obj);
	cr_expect_eq(&wait_priv->fabric->fab_fid, fab);
	cr_expect_eq(wait_priv->cond_type, FI_CQ_COND_NONE);
}

Test(cq_wait_obj, mutex_cond, .init = cq_wait_mutex_cond_setup, .disabled = true)
{
	cr_expect_eq(wait_priv->type, FI_WAIT_MUTEX_COND);
	cr_expect_eq(wait_priv->type, cq_priv->attr.wait_obj);
	cr_expect_eq(wait_priv->type, cq_attr.wait_obj);
	cr_expect_eq(&wait_priv->fabric->fab_fid, fab);
	cr_expect_eq(wait_priv->cond_type, FI_CQ_COND_NONE);
}

Test(cq_wait_control, none, .init = cq_wait_none_setup)
{
	int ret;
	int fd;

	ret = fi_control(&cq_priv->cq_fid.fid, FI_GETWAIT, &fd);
	cr_expect_eq(-FI_ENOSYS, ret, "fi_control exists for none.");
}

Test(cq_wait_control, unspec, .init = cq_wait_unspec_setup, .disabled = true)
{
	int ret;
	int fd;

	ret = fi_control(&cq_priv->cq_fid.fid, FI_GETWAIT, &fd);
	cr_expect_eq(FI_SUCCESS, ret, "fi_control failed.");

	cr_expect_eq(wait_priv->fd[WAIT_READ], fd);
}

Test(cq_wait_control, fd, .init = cq_wait_fd_setup,
	.disabled = true)
{
	int ret;
	int fd;

	ret = fi_control(&cq_priv->cq_fid.fid, FI_GETWAIT, &fd);
	cr_expect_eq(FI_SUCCESS, ret, "fi_control failed.");

	cr_expect_eq(wait_priv->fd[WAIT_READ], fd);
}

Test(cq_wait_control, mutex_cond, .init = cq_wait_mutex_cond_setup,
	.disabled = true)
{
	int ret;
	struct fi_mutex_cond mutex_cond;

	ret = fi_control(&cq_priv->cq_fid.fid, FI_GETWAIT, &mutex_cond);
	cr_expect_eq(FI_SUCCESS, ret, "fi_control failed.");

	ret = memcmp(&wait_priv->mutex, mutex_cond.mutex,
		     sizeof(*mutex_cond.mutex));
	cr_expect_eq(0, ret, "mutex compare failed.");

	ret = memcmp(&wait_priv->cond, mutex_cond.cond,
		     sizeof(*mutex_cond.cond));
	cr_expect_eq(0, ret, "cond compare failed.");
}

Test(cq_wait_ops, none, .init = cq_wait_none_setup)
{
	cr_expect_eq(cq_priv->cq_fid.ops->signal, fi_no_cq_signal,
		     "signal implementation available.");
	cr_expect_eq(cq_priv->cq_fid.ops->sread, fi_no_cq_sread,
		     "sread implementation available.");
	cr_expect_eq(cq_priv->cq_fid.ops->sreadfrom, fi_no_cq_sreadfrom,
		     "sreadfrom implementation available.");
	cr_expect_eq(cq_priv->cq_fid.fid.ops->control, fi_no_control,
		     "control implementation available.");
}

Test(cq_wait_ops, fd, .init = cq_wait_fd_setup, .disabled = true)
{
	cr_expect_neq(cq_priv->cq_fid.ops->signal, fi_no_cq_signal,
		      "signal implementation not available.");
	cr_expect_neq(cq_priv->cq_fid.fid.ops->control, fi_no_control,
		      "control implementation not available.");
}

Test(cq_wait_set, fd, .init = setup, .disabled = true)
{
	int ret;
	int fd;

	wait_attr.wait_obj = FI_WAIT_FD;

	ret = fi_wait_open(fab, &wait_attr, &wait_set);
	cr_expect_eq(FI_SUCCESS, ret, "fi_wait_open failed.");

	wait_priv = container_of(wait_set, struct gnix_fid_wait, wait);

	cq_attr.format = FI_CQ_FORMAT_MSG;
	cq_attr.size = 8;
	cq_attr.wait_obj = FI_WAIT_SET;
	cq_attr.wait_set = wait_set;

	ret = fi_cq_open(dom, &cq_attr, &rcq, NULL);
	cr_expect_eq(FI_SUCCESS, ret, "fi_cq_open failed.");

	cq_priv = container_of(rcq, struct gnix_fid_cq, cq_fid);

	ret = fi_control(&cq_priv->cq_fid.fid, FI_GETWAIT, &fd);
	cr_expect_eq(FI_SUCCESS, ret, "fi_control failed.");

	cr_expect_eq(wait_priv->fd[WAIT_READ], fd);

	ret = fi_close(&rcq->fid);
	cr_expect_eq(FI_SUCCESS, ret, "failure in closing cq.");

	ret = fi_close(&wait_set->fid);
	cr_expect_eq(FI_SUCCESS, ret, "failure in closing waitset.");

	teardown();
}

TestSuite(cq_notify_flags, .init = cq_notify_setup, .fini = cq_notify_teardown);

void do_cq_notify(uint64_t flags)
{
	ssize_t ret;
	struct fi_cq_msg_entry entry;

	ret = _gnix_cq_add_event(cq_priv, ep, NULL, flags, 0, NULL,
				 0, 0, 0);
	cr_assert(ret == FI_SUCCESS, "failing in _gnix_cq_add_event");

	ret = fi_cq_read(rcq, &entry, 1);
	cr_assert(ret == 1, "failing in fi_cq_read");

	if (flags & FI_RMA_EVENT) {
		flags &= (FI_REMOTE_READ | FI_REMOTE_WRITE |
			  FI_RMA | FI_REMOTE_CQ_DATA |
			  FI_MULTI_RECV);
	} else {
		flags &= (FI_REMOTE_CQ_DATA | FI_MULTI_RECV);
	}

	cr_assert_eq(flags, entry.flags, "unexpected cq entry flags");
}

Test(cq_notify_flags, fi_rma_event)
{
	do_cq_notify(FI_REMOTE_READ | FI_REMOTE_WRITE |
		     FI_RMA | FI_REMOTE_CQ_DATA |
		     FI_MULTI_RECV | FI_RMA_EVENT);
}

Test(cq_notify_flags, not_fi_rma_event)
{
	do_cq_notify(~FI_RMA_EVENT);
}


struct test_err {
	int padding_1;
	struct fi_cq_err_entry_1_0 error;
	int padding_2;
};

Test(reading_1_4, issue_ofiwg_3227)
{
	struct test_err error_entry;
	int ret = 0;
	char input_ctx = 'a';
	uint64_t flags = 0xb;
	size_t len = sizeof(input_ctx);
	void *buf = &input_ctx;
	uint64_t data = 20;
	uint64_t tag = 40;
	size_t olen = 20;
	int err = 50;
	int prov_errno = 80;

	/*
	 * By default CQ start out with no error entries and no entries
	 * in the error entry free list.
	 */
	cr_assert(!cq_priv->errors->item_list.head);
	cr_assert(!cq_priv->errors->free_list.head);

	_gnix_cq_add_error(cq_priv, &input_ctx, flags, len, buf, data, tag,
		olen, err, prov_errno, 0, 0);

	error_entry.padding_1 = 0xcafebabe;
	error_entry.padding_2 = 0xcafed00d;

	ret = fi_cq_readerr((struct fid_cq *) cq_priv,
		(struct fi_cq_err_entry *) &error_entry.error, 0);
	cr_assert_eq(ret, 1);

	cr_assert_eq(error_entry.padding_1, 0xcafebabe);
	cr_assert_eq(error_entry.padding_2, 0xcafed00d);
}
