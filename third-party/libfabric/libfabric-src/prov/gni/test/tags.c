/*
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


/*
 *  Created on: July 17, 2015
 *      Author: jswaro
 */
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/time.h>

#include <gnix_tags.h>
#include <gnix.h>

#include <criterion/criterion.h>
#include "gnix_rdma_headers.h"
#include <criterion/parameterized.h>
#include "common.h"

/* Note: Set to ~FI_NOTIFY_FLAGS_ONLY since this was written before api 1.5 */
static uint64_t mode_bits = ~FI_NOTIFY_FLAGS_ONLY;
static struct fi_info *hints;
static struct fi_info *fi;

#define DEFAULT_FORMAT  0x00000000FFFFFFFF
#define SINGLE_FORMAT   0xFFFFFFFFFFFFFFFF
#define DOUBLE_FORMAT   0xFFFFFFFF00000000
#define TRIPLE_FORMAT   0xFFFF0000FFFFFFFF
#define MULTI_FORMAT    0xFF00FFFF0000FFFF
#define BAD_FORMAT      0xFFFFFFFFFFFFFF00
#define SIMPLE_FORMAT   0x0000000000FF00FF

#define TEST_TAG        0x00000000DEADBEEF

enum {
	TEST_ORDER_INORDER = 0,
	TEST_ORDER_RANDOM,
	TEST_ORDER_REVERSE,
};

enum {
	TEST_OVERLAY_DEF = 0,
	TEST_OVERLAY_SINGLE,
	TEST_OVERLAY_DOUBLE,
	TEST_OVERLAY_TRIPLE,
	TEST_OVERLAY_MULTI,
	TEST_OVERLAY_BAD,
	TEST_OVERLAY_SIMPLE,
	TEST_OVERLAY_MAX,
};

struct __test_mask {
	uint64_t mask;
	uint64_t format;
	int type;
	int fields;
	int field_width[5];
	int reserved_bits;
};

struct gnix_fr_element {
	struct gnix_fab_req req;
	int claimed;
	uint64_t ignore;
	uint64_t addr_ignore;
	uint64_t peek_flags;
	uint64_t remove_flags;
	void *context;
};

struct __test_mask test_masks[TEST_OVERLAY_MAX] = {
		{
				.mask = DEFAULT_FORMAT,
				.format = DEFAULT_FORMAT,
				.type = TEST_OVERLAY_DEF,
				.fields = 1,
				.field_width = {32, 0, 0, 0, 0},
				.reserved_bits = 32,
		},
		{
				.mask = ~0,
				.format = SINGLE_FORMAT,
				.type = TEST_OVERLAY_SINGLE,
				.fields = 1,
				.field_width = {64, 0, 0, 0, 0},
				.reserved_bits = 0,
		},
		{
				.mask = ~0,
				.format = DOUBLE_FORMAT,
				.type = TEST_OVERLAY_DOUBLE,
				.fields = 2,
				.field_width = {32, 32, 0, 0, 0},
				.reserved_bits = 0,
		},
		{
				.mask = ~0,
				.format = TRIPLE_FORMAT,
				.type = TEST_OVERLAY_TRIPLE,
				.fields = 3,
				.field_width = {16, 16, 32, 0, 0},
				.reserved_bits = 0,
		},
		{
				.mask = ~0,
				.format = MULTI_FORMAT,
				.type = TEST_OVERLAY_MULTI,
				.fields = 5,
				.field_width = {8, 8, 16, 16, 16},
				.reserved_bits = 0,
		},
		{
				.mask = ~0,
				.format = BAD_FORMAT,
				.type = TEST_OVERLAY_BAD,
				.fields = 5,
				.field_width = {16, 16, 16, 8, 8},
				.reserved_bits = 0,
		},
		{
				.mask = 0x0000000000ffffff,
				.format = SIMPLE_FORMAT,
				.type = TEST_OVERLAY_SIMPLE,
				.fields = 3,
				.field_width = {8, 8, 8, 0, 0},
				.reserved_bits = 40,
		}
};

static struct gnix_tag_storage_attr default_list_attr = {
	.type = GNIX_TAG_LIST,
};

static struct gnix_tag_storage_attr default_hlist_attr = {
	.type = GNIX_TAG_HLIST,
};

static struct gnix_tag_storage_attr default_kdtree_attr = {
	.type = GNIX_TAG_KDTREE,
};

static struct gnix_tag_storage_attr default_auto_attr = {
	.type = GNIX_TAG_AUTOSELECT,
};

static struct gnix_fr_element default_reqs[8] = {
		{
			.req = {
				.msg = {
					.tag = 0x00005555,
					.ignore = 0x11111111
				},
			},
			.peek_flags = FI_PEEK,
			.remove_flags = 0,
			.context = NULL,

		},
		{
			.req = {
				.msg = {
						.tag = 0x0000AAAA,
						.ignore = 0x11111111
				},
			},
			.peek_flags = FI_PEEK,
			.remove_flags = 0,
			.context = NULL,
		},
		{
			.req = {
				.msg = {
					.tag = 0xAAAA5555,
					.ignore = 0x11111111
				},
			},
			.peek_flags = FI_PEEK,
			.remove_flags = 0,
			.context = NULL,
		},
		{
			.req = {
				.msg = {
					.tag = 0x5555AAAA,
					.ignore = 0x11111111
				},
			},
			.peek_flags = FI_PEEK,
			.remove_flags = 0,
			.context = NULL,
		},
		{
			.req = {
				.msg = {
					.tag = 0xAAAA5555,
					.ignore = 0x11111111
				},
			},
			.peek_flags = FI_PEEK,
			.remove_flags = 0,
			.context = NULL,
		},
		{
			.req = {
				.msg = {
					.tag = 0x00005555,
					.ignore = 0x11111111
				},
			},
			.peek_flags = FI_PEEK,
			.remove_flags = 0,
			.context = NULL,
		},
		{
			.req = {
				.msg = {
					.tag = 0x00005555,
					.ignore = 0x11111111
				},
			},
			.peek_flags = FI_PEEK,
			.remove_flags = 0,
			.context = NULL,
		},
		{
			.req = {
				.msg = {
					.tag = 0x0000AAAA,
					.ignore = 0x11111111
				},
			},
			.peek_flags = FI_PEEK,
			.remove_flags = 0,
			.context = NULL,
		},
};

static struct gnix_fr_element *make_evenly_distributed_tags(
		int requests,
		struct __test_mask *mask);

static struct gnix_fr_element *make_random_tags(
		int requests,
		struct __test_mask *mask);

struct ipr_test_params {
	int elements;
	struct gnix_fr_element *(*make_requests)(
					int,
					struct __test_mask *);
};

static struct ipr_test_params ipr_params[6] = {
		{
			.elements = 16,
			.make_requests = make_random_tags,
		},
		{
			.elements = 128,
			.make_requests = make_random_tags,
		},
		{
			.elements = 1024,
			.make_requests = make_random_tags,
		},
		{
			.elements = 16,
			.make_requests = make_evenly_distributed_tags,
		},
		{
			.elements = 128,
			.make_requests = make_evenly_distributed_tags,
		},
		{
			.elements = 1024,
			.make_requests = make_evenly_distributed_tags,
		},
};

static struct gnix_tag_storage *test_tag_storage;
static int call_destruct;
static int (*match_func)(
		struct dlist_entry *entry,
		const void *arg) = _gnix_match_posted_tag;


static inline void reset_test_fr_metadata(struct gnix_fr_element *reqs,
		int requests)
{
	int i;

	for (i = 0; i < requests; i++) {
		reqs[i].claimed = 0;
		reqs[i].req.msg.tle.context = NULL;
	}
}
static inline void reset_test_tag_storage(
		struct gnix_tag_storage *ts,
		struct gnix_tag_storage_attr *attr)
{
	int ret;

	ret = _gnix_tag_storage_destroy(ts);
	cr_assert(ret == FI_SUCCESS,
			"failed to destroy tag storage on reset");

	ret = _gnix_tag_storage_init(ts, attr, match_func);
	cr_assert(ret == FI_SUCCESS,
			"failed to initialize tag storage on reset");
}

static void __gnix_tags_bare_test_setup(void)
{
	int ret = 0;

	hints = fi_allocinfo();
	cr_assert(hints, "fi_allocinfo");

	hints->domain_attr->mr_mode = GNIX_DEFAULT_MR_MODE;
	hints->domain_attr->cq_data_size = 4;
	hints->mode = mode_bits;

	hints->fabric_attr->prov_name = strdup("gni");

	ret = fi_getinfo(fi_version(), NULL, 0, 0, hints, &fi);
	cr_assert_eq(ret, FI_SUCCESS, "fi_getinfo");

	cr_assert(test_tag_storage == NULL,
			"test_tag_storage was not freed prior to setup");
	test_tag_storage = calloc(1, sizeof(*test_tag_storage));
	cr_assert(test_tag_storage != NULL,
			"could not allocate test_tag_storage");

	call_destruct = 0;

	srand(0xDEADBEEF);
}

static void __gnix_tags_bare_test_teardown(void)
{
	fi_freeinfo(fi);
	fi_freeinfo(hints);

	cr_assert(test_tag_storage != NULL,
			"test_tag_storage pointer "
			"deallocated or overwritten during test");
	free(test_tag_storage);
	test_tag_storage = NULL;
}

static void __gnix_tags_basic_test_setup(void)
{
	__gnix_tags_bare_test_setup();
}

static void __gnix_tags_basic_test_teardown(void)
{
	int ret;

	ret = _gnix_tag_storage_destroy(test_tag_storage);
	cr_assert(ret == FI_SUCCESS,
			"failed to destroy tag storage "
			"during basic teardown");

	__gnix_tags_bare_test_teardown();
}

static void __gnix_tags_basic_list_test_setup(void)
{
	int ret;

	__gnix_tags_basic_test_setup();

	ret = _gnix_tag_storage_init(test_tag_storage, &default_list_attr,
		match_func);
	cr_assert(ret == FI_SUCCESS, "failed to initialize tag storage "
			"during basic list setup");
}

static void __gnix_tags_basic_hlist_test_setup(void)
{
	int ret;

	__gnix_tags_basic_test_setup();

	ret = _gnix_tag_storage_init(test_tag_storage, &default_hlist_attr,
			match_func);
	cr_assert(ret == FI_SUCCESS, "failed to initialize tag storage "
				"during basic hlist setup");
}

static void __gnix_tags_basic_kdtree_test_setup(void)
{
	int ret;

	__gnix_tags_basic_test_setup();

	ret = _gnix_tag_storage_init(test_tag_storage, &default_kdtree_attr,
			match_func);
	cr_assert(ret == FI_SUCCESS, "failed to initialize tag storage "
				"during basic kdtree setup");
}

static void __gnix_tags_basic_posted_list_test_setup(void)
{
	match_func = _gnix_match_posted_tag;
	__gnix_tags_basic_list_test_setup();
}

static void __gnix_tags_basic_posted_hlist_test_setup(void)
{
	match_func = _gnix_match_posted_tag;
	__gnix_tags_basic_hlist_test_setup();
}

static void __gnix_tags_basic_posted_kdtree_test_setup(void)
{
	match_func = _gnix_match_posted_tag;
	__gnix_tags_basic_kdtree_test_setup();
}

static void __gnix_tags_basic_unexpected_list_test_setup(void)
{
	match_func = _gnix_match_unexpected_tag;
	__gnix_tags_basic_list_test_setup();
}

static void __gnix_tags_basic_unexpected_hlist_test_setup(void)
{
	match_func = _gnix_match_unexpected_tag;
	__gnix_tags_basic_hlist_test_setup();
}

static void __gnix_tags_basic_unexpected_kdtree_test_setup(void)
{
	match_func = _gnix_match_unexpected_tag;
	__gnix_tags_basic_kdtree_test_setup();
}

__attribute__((unused))
static void __gnix_tags_basic_auto_test_setup(void)
{
	int ret;

	__gnix_tags_basic_test_setup();

	ret = _gnix_tag_storage_init(test_tag_storage, &default_auto_attr,
			match_func);
	cr_assert(ret == FI_SUCCESS,
			"failed to initialize tag storage during basic auto setup");
}

/* multi-mode tests */

static inline uint64_t make_test_tag(struct __test_mask *mask, uint64_t val)
{
	return val & mask->mask;
}


static struct gnix_fr_element *make_evenly_distributed_tags(
		int requests,
		struct __test_mask *mask)
{
	int i, j;
	uint64_t offset, field_width, tmp;
	struct gnix_fr_element *reqs;

	reqs = calloc(requests, sizeof(*reqs));
	cr_assert(reqs);

	offset = 64 - mask->reserved_bits;

	for (i = 0; i < mask->fields; i++) {
		field_width = 1ull << mask->field_width[i];
		if (mask->field_width[i] == 64)
			field_width = ~0ull;

		offset -= mask->field_width[i];
		for (j = 0; j < requests; j++) {
			tmp = (j % field_width) << offset;
			reqs[j].req.msg.tag |= tmp;
		}
	}

	for (i = 0; i < requests; i++) {
		tmp = make_test_tag(mask, reqs[i].req.msg.tag);
		reqs[i].req.msg.ignore = 0;
		reqs[i].req.msg.tag = tmp;
		reqs[i].peek_flags = FI_PEEK;
	}

	return reqs;
}

static struct gnix_fr_element *make_random_tags(
		int requests,
		struct __test_mask *mask)
{
	int i;
	uint64_t tag;
	struct gnix_fr_element *reqs;

	reqs = calloc(requests, sizeof(*reqs));
	cr_assert(reqs, "failed to allocate requests for random tag creation");

	for (i = 0; i < requests; i++) {
		tag = rand();
		tag <<= 32;
		tag += rand();
		reqs[i].req.msg.tag = make_test_tag(mask, tag);
		reqs[i].req.msg.ignore = 0;
		reqs[i].peek_flags = FI_PEEK;
	}

	return reqs;
}

static void print_request_lists(
		struct gnix_fr_element *reqs,
		int requests,
		int *correct_order,
		int *removal_order,
		int ordering_type)
{
	int i;
	char *type;
	struct gnix_fr_element *current;

	if (ordering_type == TEST_ORDER_INORDER)
		type = "INORDER";
	else if (ordering_type == TEST_ORDER_RANDOM)
		type = "RANDOM";
	else if (ordering_type == TEST_ORDER_REVERSE)
		type = "REVERSE";
	else
		type = "UNKNOWN";


	fprintf(stderr, "FAILED %s\n", type);
	fprintf(stderr, "insertion order:\n");
	for (i = 0; i < requests; i++) {
		current = &reqs[i];
		fprintf(stderr, "  req=%p req.msg.tag=0x%.16llx index=%i\n",
				&current->req,
				(unsigned long long int) current->req.msg.tag,
				i);
	}

	fprintf(stderr, "removal order:\n");
	for (i = 0; i < requests; i++) {
		current = &reqs[removal_order[i]];
		fprintf(stderr,
				"  req=%p req.msg.tag=0x%.16llx ignore=0x%.16llx index=%i\n",
				&current->req,
				(unsigned long long int) current->req.msg.tag,
				(unsigned long long int) current->ignore,
				removal_order[i]);
	}

	fprintf(stderr, "correct order:\n");
	for (i = 0; i < requests; i++) {
		current = &reqs[correct_order[i]];
		fprintf(stderr, "  req=%p req.msg.tag=0x%.16llx index=%i\n",
				&current->req,
				(unsigned long long int) current->req.msg.tag,
				correct_order[i]);
	}
}

static void multiple_insert_peek_remove_by_order(
		struct gnix_tag_storage *ts,
		struct __test_mask *mask,
		int requests,
		struct gnix_fr_element *reqs,
		int *correct_order,
		int *removal_order,
		int ordering_type)
{
	int ret;
	int i, j, ignore_bits;
	struct gnix_fr_element *to_remove, *current;
	struct gnix_fab_req *correct, *found;
	int is_posted = ts->match_func == _gnix_match_posted_tag;

	/* clear claimed flags */
	for (i = 0; i < requests; i++)
		reqs[i].claimed = 0;

	/* reset the dlist state as appropriate for a new request */
	for (i = 0; i < requests; i++)
		dlist_init(&reqs[i].req.msg.tle.free);

	/* establish correct removal order based on passed in removal order */
	for (i = 0; i < requests; i++) {
		to_remove = &reqs[removal_order[i]];

		for (j = 0; j < requests; j++) {
			current = &reqs[j];

			if (current->claimed)
				continue;

			/* when the tag store is a posted tag store,
			 * always use the ignore bits from the stored request
			 *
			 * otherwise, we are attempting to remove from
			 * a unexpected tag store and we should use the
			 * provided ignore bits
			 */
			if (is_posted)
				ignore_bits = current->req.msg.ignore;
			else
				ignore_bits = to_remove->ignore;

			/* applying peek flags - This assumes the appropriate
			 * peek was performed prior to removal
			 */
			if ((to_remove->peek_flags & FI_PEEK) &&
					(to_remove->peek_flags & FI_CLAIM)) {
				to_remove->req.msg.tle.context =
						to_remove->context;
			}

			if (_gnix_req_matches_params(&to_remove->req,
					current->req.msg.tag,
					ignore_bits,
					to_remove->remove_flags,
					to_remove->context,
					test_tag_storage->attr.use_src_addr_matching,
					&to_remove->req.addr, is_posted))
				break;

			to_remove->req.msg.tle.context = NULL;
		}

		cr_assert(j != requests,
				"failed to find a match for every entry");
		correct_order[i] = j;
		current->claimed = 1;
	}

	/* clear alterations to the req structure
	 * during creation of correct list
	 */
	for (i = 0; i < requests; i++)
		reqs[i].req.msg.tle.context = NULL;

	/* clear claimed flags */
	for (i = 0; i < requests; i++)
		reqs[i].claimed = 0;

	for (i = 0; i < requests; i++) {
		ret = _gnix_insert_tag(ts, reqs[i].req.msg.tag, &reqs[i].req,
				reqs[i].ignore);
		if (ret) {
			print_request_lists(reqs, requests, correct_order,
					removal_order, ordering_type);
		}
		cr_assert(ret == FI_SUCCESS,
				"failed to insert tag into storage");
	}

	for (i = 0; i < requests; i++) {
		to_remove = &reqs[removal_order[i]];
		correct = &reqs[correct_order[i]].req;

		found = _gnix_match_tag(ts,
				to_remove->req.msg.tag, to_remove->ignore,
				to_remove->peek_flags | FI_PEEK, to_remove->context,
				NULL);
		if (found != correct) {
			print_request_lists(reqs, requests, correct_order,
					removal_order, ordering_type);
			fprintf(stderr,
					"failed to find request, to_remove=%p "
					"correct=%p found=%p",
					&to_remove->req,
					found,
					correct);
		}
		cr_assert(found == correct,
				"failed to find tag in storage");


		found = _gnix_match_tag(ts,
				to_remove->req.msg.tag, to_remove->ignore,
				to_remove->remove_flags, to_remove->context,
				NULL);
		if (found != correct) {
			print_request_lists(reqs, requests, correct_order,
					removal_order, ordering_type);
		}
		cr_assert(found == correct,
				"failed to remove tag from storage");
	}
}

static void multiple_insert_peek_remove_inorder(
		struct gnix_tag_storage *ts,
		struct __test_mask *mask,
		int requests,
		struct gnix_fr_element *reqs,
		int *correct_order,
		int *removal_order)
{
	int i;

	for (i = 0; i < requests; i++)
		removal_order[i] = i;

	multiple_insert_peek_remove_by_order(
			ts, mask, requests, reqs,
			correct_order, removal_order,
			TEST_ORDER_INORDER);
}

static void multiple_insert_peek_remove_reverse(
		struct gnix_tag_storage *ts,
		struct __test_mask *mask,
		int requests,
		struct gnix_fr_element *reqs,
		int *correct_order,
		int *removal_order)
{
	int i;

	for (i = requests - 1; i >= 0; i--)
		removal_order[requests - 1 - i] = i;

	multiple_insert_peek_remove_by_order(ts,
			mask, requests, reqs,
			correct_order, removal_order,
			TEST_ORDER_REVERSE);
}

static void multiple_insert_peek_remove_random(
		struct gnix_tag_storage *ts,
		struct __test_mask *mask,
		int requests,
		struct gnix_fr_element *reqs,
		int *correct_order,
		int *removal_order)
{
	int i, j, rand_index, rand_j;

	for (i = 0; i < requests; i++) {
		rand_index = rand() % requests;
		for (j = 0; j < requests; j++) {
			rand_j = (rand_index + j) % requests;
			if (!reqs[rand_j].claimed)
				break;
		}

		removal_order[i] = rand_j;
		reqs[rand_j].claimed = 1;
	}

	/* clear claimed flags */
	for (i = 0; i < requests; i++)
		reqs[i].claimed = 0;

	multiple_insert_peek_remove_by_order(ts,
			mask, requests, reqs,
			correct_order, removal_order,
			TEST_ORDER_RANDOM);
}

static inline void __test_multiple_ipr_reqs_by_function(
		int requests,
		struct gnix_tag_storage_attr *test_attr,
		void (*order_func)(
				struct gnix_tag_storage*,
				struct __test_mask*,
				int,
				struct gnix_fr_element*,
				int*,
				int*),
		struct gnix_fr_element *(make_requests)(
				int,
				struct __test_mask *),
		int *correct_order,
		int *removal_order)
{
	int i;
	struct gnix_fr_element *reqs;
	struct gnix_tag_storage_attr attr;

	memcpy(&attr, test_attr, sizeof(struct gnix_tag_storage_attr));

	for (i = 0; i < TEST_OVERLAY_MAX; i++) {
		reqs = make_requests(requests, &test_masks[i]);
		cr_assert(reqs != NULL, "failed to make random tags");

		order_func(test_tag_storage, &test_masks[i], requests, reqs,
				correct_order, removal_order);

		/* make necessary alterations to the structure
		 * for the next test mask
		 */
		reset_test_tag_storage(test_tag_storage, &attr);

		free(reqs);
	}
}

static inline void __test_multiple_inorder_ipr_reqs(
		int requests,
		struct gnix_tag_storage_attr *test_attr,
		struct gnix_fr_element *(make_requests)(
				int,
				struct __test_mask *))

{
	int *correct_order, *removal_order;

	correct_order = calloc(requests, sizeof(*correct_order));
	cr_assert(correct_order != NULL);

	removal_order = calloc(requests, sizeof(*removal_order));
	cr_assert(removal_order != NULL);

	__test_multiple_ipr_reqs_by_function(requests, test_attr,
			multiple_insert_peek_remove_inorder,
			make_requests, correct_order, removal_order);

	free(correct_order);
	free(removal_order);
}

static inline void __test_multiple_reverse_ipr_reqs(
		int requests,
		struct gnix_tag_storage_attr *test_attr,
		struct gnix_fr_element *(make_requests)(
				int,
				struct __test_mask *))

{
	int *correct_order, *removal_order;

	correct_order = calloc(requests, sizeof(*correct_order));
	cr_assert(correct_order != NULL);

	removal_order = calloc(requests, sizeof(*removal_order));
	cr_assert(removal_order != NULL);

	__test_multiple_ipr_reqs_by_function(requests, test_attr,
			multiple_insert_peek_remove_reverse,
			make_requests, correct_order, removal_order);

	free(correct_order);
	free(removal_order);
}

static inline void __test_multiple_random_ipr_reqs(
		int requests,
		struct gnix_tag_storage_attr *test_attr,
		struct gnix_fr_element *(make_requests)(
				int,
				struct __test_mask *))

{
	int *correct_order, *removal_order;

	correct_order = calloc(requests, sizeof(*correct_order));
	cr_assert(correct_order != NULL);

	removal_order = calloc(requests, sizeof(*removal_order));
	cr_assert(removal_order != NULL);

	__test_multiple_ipr_reqs_by_function(requests, test_attr,
			multiple_insert_peek_remove_random,
			make_requests, correct_order, removal_order);

	free(correct_order);
	free(removal_order);
}

static inline void __test_multiple_type_ipr_reqs(
		int requests,
		struct gnix_tag_storage_attr *test_attr,
		struct gnix_fr_element *(make_requests)(
				int,
				struct __test_mask *))
{
	__test_multiple_inorder_ipr_reqs(requests, test_attr, make_requests);

	reset_test_tag_storage(test_tag_storage, test_attr);

	__test_multiple_reverse_ipr_reqs(requests, test_attr, make_requests);

	reset_test_tag_storage(test_tag_storage, test_attr);

	__test_multiple_random_ipr_reqs(requests, test_attr, make_requests);

	reset_test_tag_storage(test_tag_storage, test_attr);
}

static inline void __test_multiple_8_duplicate_tags(
		struct gnix_tag_storage_attr *test_attr)
{
	int i;
	struct gnix_fr_element reqs[8];
	int requests = 8;
	int *correct_order, *removal_order;

	correct_order = calloc(8, sizeof(*correct_order));
	cr_assert(correct_order != NULL);

	removal_order = calloc(8, sizeof(*removal_order));
	cr_assert(removal_order != NULL);

	memcpy(reqs, default_reqs, sizeof(struct gnix_fr_element) * requests);

	for (i = 0; i < TEST_OVERLAY_MAX; i++) {
		multiple_insert_peek_remove_inorder(
				test_tag_storage, &test_masks[i],
				requests, reqs, correct_order,
				removal_order);

		reset_test_tag_storage(test_tag_storage, test_attr);
		reset_test_fr_metadata(reqs, requests);

		multiple_insert_peek_remove_reverse(
				test_tag_storage, &test_masks[i],
				requests, reqs, correct_order, removal_order);

		reset_test_tag_storage(test_tag_storage, test_attr);
		reset_test_fr_metadata(reqs, requests);

		multiple_insert_peek_remove_random(
				test_tag_storage, &test_masks[i],
				requests, reqs, correct_order, removal_order);

		reset_test_tag_storage(test_tag_storage, test_attr);
		reset_test_fr_metadata(reqs, requests);
	}

	free(correct_order);
	free(removal_order);
}

static inline void __test_not_found_empty(void)
{
	struct gnix_fab_req *found;

	found = _gnix_match_tag(
			test_tag_storage, 0xCAFEBABE, 0, FI_PEEK, NULL, NULL);
	cr_assert(found == NULL,
			"something in this storage should not exist");
}

static inline void __test_not_found_non_empty(void)
{
	int ret;
	int i, requests = 8;
	struct gnix_fab_req *correct, *found;
	struct gnix_fr_element *to_remove;
	struct gnix_fr_element reqs[8];

	memcpy(reqs, default_reqs, sizeof(struct gnix_fr_element) * 8);

	/* reset the dlist state as appropriate for a new request */
	for (i = 0; i < requests; i++)
		dlist_init(&reqs[i].req.msg.tle.free);

	for (i = 0; i < requests; i++) {
		ret = _gnix_insert_tag(test_tag_storage,
				reqs[i].req.msg.tag, &reqs[i].req,
				reqs[i].ignore);
		cr_assert(ret == FI_SUCCESS,
				"failed to insert tag into storage");
	}

	found = _gnix_match_tag(test_tag_storage, 0xCAFEBABE, 0, FI_PEEK,
			NULL, NULL);
	cr_assert(found == NULL,
			"something in this storage should not exist");

	/* only doing in-order removal */
	for (i = 0; i < requests; i++) {
		to_remove = &reqs[i];
		correct = &reqs[i].req;

		found = _gnix_match_tag(
				test_tag_storage, to_remove->req.msg.tag,
				to_remove->ignore, to_remove->peek_flags,
				to_remove->context, NULL);
		cr_assert(found == correct,
				"failed to find tag in storage");

		found = _gnix_match_tag(test_tag_storage,
				to_remove->req.msg.tag, to_remove->ignore,
				to_remove->remove_flags, to_remove->context,
				NULL);
		cr_assert(found == correct,
				"failed to remove tag from storage");
	}
}

static inline void __test_ignore_mask_set(
		struct gnix_tag_storage_attr *test_attr)
{
	int i;
	struct gnix_fr_element reqs[8];
	int requests = 8;
	int correct_order[8], removal_order[8];

	memcpy(reqs, default_reqs, sizeof(struct gnix_fr_element) * requests);

	for (i = requests - 1; i >= 0 ; i--)
		removal_order[i] = (requests - 1) - i;

	reqs[7].ignore = 0xffff;
	reqs[6].ignore = 0xffff0000;
	reqs[5].ignore = 0xffff0000;
	reqs[4].ignore = 0xffff0000;
	reqs[3].ignore = 0xffff0000;
	reqs[2].ignore = 0xffff0000;
	reqs[1].ignore = 0x0;
	reqs[0].ignore = 0xffffffff;

	for (i = 0; i < requests; i++)
		reqs[i].req.msg.ignore = reqs[i].ignore;

	multiple_insert_peek_remove_by_order(test_tag_storage,
			&test_masks[TEST_OVERLAY_DEF], requests, reqs,
			correct_order, removal_order, TEST_ORDER_REVERSE);
}

static inline void __test_claim_pass(
		struct gnix_tag_storage_attr *test_attr)
{
	int ret;
	struct gnix_fr_element request = {
		.req = {
			.msg = {
				.tag = 0xA5A5A5A5,
				.ignore = 0xFFFFFFFF
			},
		},
		.peek_flags = FI_PEEK | FI_CLAIM,
		.remove_flags = FI_CLAIM,
		.context = (void *) 0xDEADBEEF,
	};
	struct gnix_fab_req *found;

	dlist_init(&request.req.msg.tle.free);

	ret = _gnix_insert_tag(
			test_tag_storage, request.req.msg.tag,
			&request.req, request.ignore);
	cr_assert(ret == FI_SUCCESS, "failed to insert tag into storage");

	found = _gnix_match_tag(
			test_tag_storage, request.req.msg.tag, request.ignore,
			request.peek_flags, request.context, NULL);
	cr_assert(found == &request.req, "failed to find tag in storage");

	found = _gnix_match_tag(
			test_tag_storage, request.req.msg.tag, request.ignore,
			request.remove_flags, request.context, NULL);
	cr_assert(found == &request.req, "failed to remove tag from storage");
}

/* tests to ensure you cannot remove an unclaimed messge when FI_CLAIM is set */
static inline void __test_fail_no_claimed_tags(
		struct gnix_tag_storage_attr *test_attr)
{
	int ret;
	struct gnix_fab_req *found;
	struct gnix_fr_element request = {
		.req = {
			.msg = {
				.tag = 0xA5A5A5A5,
				.ignore = 0xFFFFFFFF
			},
		},
		.peek_flags = FI_PEEK,
		.remove_flags = FI_CLAIM,
		.context = (void *) 0xDEADBEEF,
	};

	dlist_init(&request.req.msg.tle.free);

	ret = _gnix_insert_tag(
			test_tag_storage, request.req.msg.tag,
			&request.req, request.ignore);
	cr_assert(ret == FI_SUCCESS, "failed to insert tag into storage");

	found = _gnix_match_tag(
			test_tag_storage, request.req.msg.tag, request.ignore,
			request.peek_flags, request.context, NULL);
	cr_assert(found == &request.req, "failed to find tag in storage");

	found = _gnix_match_tag(
			test_tag_storage, request.req.msg.tag, request.ignore,
			request.remove_flags, request.context, NULL);
	cr_assert(found == NULL, "found an unexpected tag in remove");

	/* use the peek tags this time to remove the entry */
	found = _gnix_match_tag(
			test_tag_storage, request.req.msg.tag, request.ignore,
			0, request.context, NULL);
	cr_assert(found == &request.req, "failed to find tag in storage");
}

/* test to ensure you cannot remove a message that has been claimed */
static inline void __test_fail_all_claimed_tags(
		struct gnix_tag_storage_attr *test_attr)
{
	int ret;
	struct gnix_fr_element request = {
		.req = {
			.msg = {
				.tag = 0xA5A5A5A5,
				.ignore = 0xFFFFFFFF
			},
		},
		.peek_flags = FI_PEEK | FI_CLAIM,
		.remove_flags = 0,
		.context = (void *) 0xDEADBEEF,
	};
	struct gnix_fab_req *found;

	/* reset the dlist state as appropriate for a new request */
	dlist_init(&request.req.msg.tle.free);


	ret = _gnix_insert_tag(
			test_tag_storage, request.req.msg.tag,
			&request.req, request.ignore);
	cr_assert(ret == FI_SUCCESS, "failed to insert tag into storage");

	found = _gnix_match_tag(
			test_tag_storage, request.req.msg.tag, request.ignore,
			request.peek_flags, request.context, NULL);
	cr_assert(found == &request.req, "failed to find tag in storage");

	found = _gnix_match_tag(
			test_tag_storage, request.req.msg.tag, request.ignore,
			request.remove_flags, request.context, NULL);
	cr_assert(found == NULL, "found an unexpected tag during remove");

	/* use the peek tags this time to remove the entry */
	found = _gnix_match_tag(
			test_tag_storage, request.req.msg.tag, request.ignore,
			FI_CLAIM, request.context, NULL);
	cr_assert(found == &request.req, "failed to find tag in storage");
}

static inline void __test_fail_peek_all_claimed(
		struct gnix_tag_storage_attr *test_attr)
{
	int ret;
	struct gnix_fr_element request = {
		.req = {
			.msg = {
				.tag = 0xA5A5A5A5,
				.ignore = 0xFFFFFFFF
			},
		},
		.peek_flags = FI_PEEK,
		.remove_flags = FI_CLAIM,
		.context = (void *) 0xDEADBEEF,
	};
	struct gnix_fab_req *found;

	dlist_init(&request.req.msg.tle.free);

	ret = _gnix_insert_tag(
			test_tag_storage, request.req.msg.tag,
			&request.req, request.ignore);
	cr_assert(ret == FI_SUCCESS, "failed to insert tag into storage");

	found = _gnix_match_tag(
			test_tag_storage, request.req.msg.tag, request.ignore,
				request.peek_flags | FI_CLAIM, request.context, NULL);
	cr_assert(found == &request.req, "fail to claim tag");

	found = _gnix_match_tag(
			test_tag_storage, request.req.msg.tag, request.ignore,
			request.peek_flags, request.context, NULL);
	cr_assert(found == NULL, "unexpectedly found a tag");

	found = _gnix_match_tag(
			test_tag_storage, request.req.msg.tag, request.ignore,
			request.remove_flags, request.context, NULL);
	cr_assert(found == &request.req, "failed to remove tag from storage");
}

static inline void __test_src_addr_match(
		struct gnix_tag_storage_attr *test_attr)
{
	int ret;
	struct gnix_fr_element request = {
		.req = {
			.msg = {
					.tag = 0xA5A5A5A5,
			},
			.addr = {
				.cdm_id = 1,
				.device_addr = 1,
			},
		},
		.peek_flags = FI_PEEK,
	};
	struct gnix_fab_req *found;

	dlist_init(&request.req.msg.tle.free);

	ret = _gnix_insert_tag(
			test_tag_storage, request.req.msg.tag,
			&request.req, request.ignore);
	cr_assert(ret == FI_SUCCESS, "failed to insert tag into storage");

	found = _gnix_match_tag(
			test_tag_storage, request.req.msg.tag,
			request.ignore, request.peek_flags,
			request.context, &request.req.addr);
	cr_assert(found == &request.req, "failed to find tag in storage");

	found = _gnix_match_tag(
			test_tag_storage, request.req.msg.tag,
			request.ignore, request.remove_flags,
			request.context, &request.req.addr);
	cr_assert(found == &request.req, "failed to remove tag from storage");
}

/* tests to ensure you cannot remove an unclaimed messge when FI_CLAIM is set */
static inline void __test_src_addr_fail_wrong_src_addr(
		struct gnix_tag_storage_attr *test_attr)
{
	int ret;
	struct gnix_fab_req *found;
	struct gnix_fr_element request = {
		.req = {
			.msg = {
					.tag = 0xA5A5A5A5,
			},
			.addr = {
				.cdm_id = 1,
				.device_addr = 1,
			},
		},
		.ignore = 0,
		.addr_ignore = 0,
		.peek_flags = FI_PEEK,
	};
	struct gnix_address addr_to_find = {
			.cdm_id = 1,
			.device_addr = 2,
	};

	dlist_init(&request.req.msg.tle.free);

	/* hack, don't actually do this */
	test_tag_storage->attr.use_src_addr_matching = 1;

	ret = _gnix_insert_tag(
			test_tag_storage, request.req.msg.tag,
			&request.req, request.ignore);
	cr_assert(ret == FI_SUCCESS, "failed to insert tag into storage");

	found = _gnix_match_tag(
			test_tag_storage, request.req.msg.tag,
			request.ignore, request.peek_flags,
			request.context, &addr_to_find);
	cr_assert(found == NULL, "found unexpected tag");

	found = _gnix_match_tag(
			test_tag_storage, request.req.msg.tag,
				request.ignore, request.peek_flags,
				request.context, &request.req.addr);
	cr_assert(found == &request.req, "failed to find tag in storage");

	found = _gnix_match_tag(
			test_tag_storage, request.req.msg.tag,
			request.ignore, request.remove_flags,
			request.context, &request.req.addr);
	cr_assert(found == &request.req, "failed to find tag in storage");
}

/* test to ensure you cannot remove a message that has been claimed */
static inline void __test_src_addr_match_unspec(
		struct gnix_tag_storage_attr *test_attr)
{
	int ret;
	struct gnix_fr_element request = {
		.req = {
			.msg = {
					.tag = 0xA5A5A5A5,
			},
			.addr = {
				.cdm_id = 1,
				.device_addr = 1,
			},
		},
		.peek_flags = FI_PEEK,
		.ignore = 0,
		.addr_ignore = 0,
	};
	struct gnix_fab_req *found;
	struct gnix_address to_find = {
			.cdm_id = -1,
			.device_addr = -1,
	};

	dlist_init(&request.req.msg.tle.free);

	if (test_tag_storage->match_func == _gnix_match_posted_tag) {
		/* swap addresses because posted tag receives check
		 * the address in the request for unspec rather than
		 * looking at the address in the parameters
		 */
		request.req.addr.cdm_id = -1;
		request.req.addr.device_addr = -1;

		to_find.cdm_id = 1;
		to_find.device_addr = 1;
	}



	/* hack, don't actually do this */
	test_tag_storage->attr.use_src_addr_matching = 1;

	ret = _gnix_insert_tag(
			test_tag_storage, request.req.msg.tag,
			&request.req, request.ignore);
	cr_assert(ret == FI_SUCCESS, "failed to insert tag into storage");



	found = _gnix_match_tag(
			test_tag_storage, request.req.msg.tag,
			request.ignore, request.peek_flags,
			request.context, &to_find);
	cr_assert(found == &request.req, "failed to find tag in storage");

	found = _gnix_match_tag(
			test_tag_storage, request.req.msg.tag,
			request.ignore, request.remove_flags,
			request.context, &to_find);
	cr_assert(found == &request.req, "failed to find tag in storage");
}

static void single_insert_peek_remove(
		struct gnix_tag_storage *ts,
		struct __test_mask *mask,
		struct gnix_fr_element *reqs)
{
	int correct_order, removal_order;

	multiple_insert_peek_remove_inorder(ts, mask, 1, reqs, &correct_order,
			&removal_order);
}

/*
 * Basic functionality tests for the gnix_bitmap_t object
 */

TestSuite(gnix_tags_bare,
		.init = __gnix_tags_bare_test_setup,
		.fini = __gnix_tags_bare_test_teardown);

TestSuite(gnix_tags_basic_posted_list,
		.init = __gnix_tags_basic_posted_list_test_setup,
		.fini = __gnix_tags_basic_test_teardown);

TestSuite(gnix_tags_basic_posted_hlist,
		.init = __gnix_tags_basic_posted_hlist_test_setup,
		.fini = __gnix_tags_basic_test_teardown);

TestSuite(gnix_tags_basic_posted_kdtree,
		.init = __gnix_tags_basic_posted_kdtree_test_setup,
		.fini = __gnix_tags_basic_test_teardown,
		.disabled = true);

TestSuite(gnix_tags_basic_unexpected_list,
		.init = __gnix_tags_basic_unexpected_list_test_setup,
		.fini = __gnix_tags_basic_test_teardown);

TestSuite(gnix_tags_basic_unexpected_hlist,
		.init = __gnix_tags_basic_unexpected_hlist_test_setup,
		.fini = __gnix_tags_basic_test_teardown);

TestSuite(gnix_tags_basic_unexpected_kdtree,
		.init = __gnix_tags_basic_unexpected_kdtree_test_setup,
		.fini = __gnix_tags_basic_test_teardown,
		.disabled = true);

Test(gnix_tags_bare, uninitialized)
{
	cr_assert(test_tag_storage->gen == 0);
	cr_assert(test_tag_storage->attr.type == GNIX_TAG_AUTOSELECT);
	cr_assert(test_tag_storage->state == GNIX_TS_STATE_UNINITIALIZED);
}

Test(gnix_tags_bare, initialize_list)
{
	int ret;

	ret = _gnix_tag_storage_init(test_tag_storage, &default_list_attr,
			match_func);
	cr_assert(ret == FI_SUCCESS);

	ret = _gnix_tag_storage_destroy(test_tag_storage);
	cr_assert(ret == FI_SUCCESS);
}

Test(gnix_tags_bare, simple)
{
	int ret;
	struct gnix_fab_req req, *tmp;
	dlist_init(&req.msg.tle.free);

	ret = _gnix_tag_storage_init(test_tag_storage, &default_list_attr,
			match_func);
	cr_assert(ret == FI_SUCCESS);

	ret = _gnix_insert_tag(test_tag_storage, 0xdeadbeef, &req, 0);
	cr_assert(ret == FI_SUCCESS);

	ret = _gnix_insert_tag(test_tag_storage, 0xdeadbeef, &req, 0);
	cr_assert(ret == -FI_EALREADY);

	tmp = _gnix_match_tag(test_tag_storage, 0xdeadbeef, 0, 0, 0, 0);
	cr_assert(&req == tmp);

	ret = _gnix_tag_storage_destroy(test_tag_storage);
	cr_assert(ret == FI_SUCCESS);
}



Test(gnix_tags_bare, initialize_hlist)
{
	int ret;

	ret = _gnix_tag_storage_init(test_tag_storage, &default_hlist_attr,
			match_func);
	cr_assert(ret == FI_SUCCESS);

	ret = _gnix_tag_storage_destroy(test_tag_storage);
	cr_assert(ret == FI_SUCCESS);
}

Test(gnix_tags_bare, initialize_kdtree)
{
	int ret;

	ret = _gnix_tag_storage_init(test_tag_storage, &default_kdtree_attr,
			match_func);
	cr_assert(ret == -FI_ENOSYS);

	//ret = _gnix_tag_storage_destroy(test_tag_storage);
	//cr_assert(ret == FI_SUCCESS);
}


Test(gnix_tags_bare, initialize_auto)
{
	int ret;

	ret = _gnix_tag_storage_init(test_tag_storage, &default_auto_attr,
			match_func);
	cr_assert(ret == FI_SUCCESS);

	ret = _gnix_tag_storage_destroy(test_tag_storage);
	cr_assert(ret == FI_SUCCESS);
}

Test(gnix_tags_bare, already_initialized)
{
	int ret;

	ret = _gnix_tag_storage_init(test_tag_storage, &default_list_attr,
			match_func);
	cr_assert(ret == FI_SUCCESS);

	ret = _gnix_tag_storage_init(test_tag_storage, &default_list_attr,
			match_func);
	cr_assert(ret == -FI_EINVAL);

	ret = _gnix_tag_storage_destroy(test_tag_storage);
	cr_assert(ret == FI_SUCCESS);
}

Test(gnix_tags_basic_posted_list, single_insert_remove)
{
	int i;
	struct gnix_fr_element request = {
		.req = {
			.msg = {
				.tag = 0xA5A5A5A5,
				.ignore = 0xFFFFFFFF
			},
		},
		.peek_flags = FI_PEEK,
	};
	struct gnix_tag_storage_attr attr;

	memcpy(&attr, &default_list_attr, sizeof(struct gnix_tag_storage_attr));

	for (i = 0; i < TEST_OVERLAY_MAX; i++) {

		single_insert_peek_remove(test_tag_storage,
				&test_masks[i], &request);

		/* make necessary alterations to the structure
		 * for the next test mask
		 */
		reset_test_tag_storage(test_tag_storage, &attr);
	}
}

ParameterizedTestParameters(gnix_tags_basic_posted_list, multiple_ipr_tags)
{
	size_t nb_params = sizeof (ipr_params) / sizeof (struct ipr_test_params);
	return cr_make_param_array(struct ipr_test_params, ipr_params, nb_params);
}

ParameterizedTest(struct ipr_test_params *params,
		gnix_tags_basic_posted_list, multiple_ipr_tags)
{
	__test_multiple_type_ipr_reqs(params->elements, &default_list_attr,
			params->make_requests);
}


Test(gnix_tags_basic_posted_list, multiple_8_duplicate_tags)
{
	__test_multiple_8_duplicate_tags(&default_list_attr);
}

Test(gnix_tags_basic_posted_list, not_found_non_empty)
{
	__test_not_found_non_empty();
}

Test(gnix_tags_basic_posted_list, not_found_empty)
{
	__test_not_found_empty();
}

Test(gnix_tags_basic_posted_list, ignore_mask_set)
{
	__test_ignore_mask_set(&default_list_attr);
}

Test(gnix_tags_basic_posted_list, fi_claim_pass)
{
	__test_claim_pass(&default_list_attr);
}

Test(gnix_tags_basic_posted_list, fi_claim_fail_no_claimed_tags)
{
	__test_fail_no_claimed_tags(&default_list_attr);
}

Test(gnix_tags_basic_posted_list, fi_claim_fail_all_claimed_tags)
{
	__test_fail_all_claimed_tags(&default_list_attr);
}

Test(gnix_tags_basic_posted_list, fi_claim_fail_peek_all_claimed)
{
	__test_fail_peek_all_claimed(&default_list_attr);
}

/* unexpected list src address matching tests */
Test(gnix_tags_basic_posted_list, src_addr_match_success)
{
	__test_src_addr_match(&default_list_attr);
}

Test(gnix_tags_basic_posted_list, src_addr_no_match_wrong_addr)
{
	__test_src_addr_fail_wrong_src_addr(&default_list_attr);
}

Test(gnix_tags_basic_posted_list, src_addr_match_unspec)
{
	__test_src_addr_match_unspec(&default_list_attr);
}


/*
 * unexpected list tests
 */

Test(gnix_tags_basic_unexpected_list, single_insert_remove)
{
	int i;
	struct gnix_fr_element request = {
		.req = {
			.msg = {
				.tag = 0xA5A5A5A5,
				.ignore = 0xFFFFFFFF
			},
		},
		.peek_flags = FI_PEEK,
	};
	struct gnix_tag_storage_attr attr;

	memcpy(&attr, &default_list_attr, sizeof(struct gnix_tag_storage_attr));

	for (i = 0; i < TEST_OVERLAY_MAX; i++) {

		single_insert_peek_remove(test_tag_storage,
				&test_masks[i], &request);

		/* make necessary alterations to the structure
		 * for the next test mask
		 */
		reset_test_tag_storage(test_tag_storage, &attr);
	}
}

ParameterizedTestParameters(gnix_tags_basic_unexpected_list, multiple_ipr_tags)
{
	size_t nb_params = sizeof (ipr_params) / sizeof (struct ipr_test_params);
	return cr_make_param_array(struct ipr_test_params, ipr_params, nb_params);
}

ParameterizedTest(struct ipr_test_params *params,
		gnix_tags_basic_unexpected_list, multiple_ipr_tags)
{
	__test_multiple_type_ipr_reqs(params->elements, &default_list_attr,
			params->make_requests);
}

Test(gnix_tags_basic_unexpected_list, multiple_8_duplicate_tags)
{
	__test_multiple_8_duplicate_tags(&default_list_attr);
}

Test(gnix_tags_basic_unexpected_list, not_found_non_empty)
{
	__test_not_found_non_empty();
}

Test(gnix_tags_basic_unexpected_list, not_found_empty)
{
	__test_not_found_empty();
}

Test(gnix_tags_basic_unexpected_list, ignore_mask_set)
{
	__test_ignore_mask_set(&default_list_attr);
}

Test(gnix_tags_basic_unexpected_list, fi_claim_pass)
{
	__test_claim_pass(&default_list_attr);
}

Test(gnix_tags_basic_unexpected_list, fi_claim_fail_no_claimed_tags)
{
	__test_fail_no_claimed_tags(&default_list_attr);
}

Test(gnix_tags_basic_unexpected_list, fi_claim_fail_all_claimed_tags)
{
	__test_fail_all_claimed_tags(&default_list_attr);
}

Test(gnix_tags_basic_unexpected_list, fi_claim_fail_peek_all_claimed)
{
	__test_fail_peek_all_claimed(&default_list_attr);
}

/* unexpected list src address matching tests */
Test(gnix_tags_basic_unexpected_list, src_addr_match_success)
{
	__test_src_addr_match(&default_list_attr);
}

Test(gnix_tags_basic_unexpected_list, src_addr_no_match_wrong_addr)
{
	__test_src_addr_fail_wrong_src_addr(&default_list_attr);
}

Test(gnix_tags_basic_unexpected_list, src_addr_match_unspec)
{
	__test_src_addr_match_unspec(&default_list_attr);
}

/*
 * hlist tests
 */

Test(gnix_tags_basic_posted_hlist, single_insert_remove)
{
	int i;
	struct gnix_fr_element request = {
		.req = {
			.msg = {
				.tag = 0xA5A5A5A5,
				.ignore = 0xFFFFFFFF
			},
		},
		.peek_flags = FI_PEEK,
	};
	struct gnix_tag_storage_attr attr;

	memcpy(&attr, &default_hlist_attr, sizeof(struct gnix_tag_storage_attr));

	for (i = 0; i < TEST_OVERLAY_MAX; i++) {

		single_insert_peek_remove(test_tag_storage,
				&test_masks[i], &request);

		/* make necessary alterations to the structure
		 * for the next test mask
		 */
		reset_test_tag_storage(test_tag_storage, &attr);
	}
}

ParameterizedTestParameters(gnix_tags_basic_posted_hlist, multiple_ipr_tags)
{
	size_t nb_params = sizeof (ipr_params) / sizeof (struct ipr_test_params);
	return cr_make_param_array(struct ipr_test_params, ipr_params, nb_params);
}

ParameterizedTest(struct ipr_test_params *params,
		gnix_tags_basic_posted_hlist, multiple_ipr_tags)
{
	__test_multiple_type_ipr_reqs(params->elements, &default_hlist_attr,
			params->make_requests);
}


Test(gnix_tags_basic_posted_hlist, multiple_8_duplicate_tags)
{
	__test_multiple_8_duplicate_tags(&default_hlist_attr);
}

Test(gnix_tags_basic_posted_hlist, not_found_non_empty)
{
	__test_not_found_non_empty();
}

Test(gnix_tags_basic_posted_hlist, not_found_empty)
{
	__test_not_found_empty();
}

Test(gnix_tags_basic_posted_hlist, ignore_mask_set)
{
	__test_ignore_mask_set(&default_hlist_attr);
}

Test(gnix_tags_basic_posted_hlist, fi_claim_pass)
{
	__test_claim_pass(&default_hlist_attr);
}

Test(gnix_tags_basic_posted_hlist, fi_claim_fail_no_claimed_tags)
{
	__test_fail_no_claimed_tags(&default_hlist_attr);
}

Test(gnix_tags_basic_posted_hlist, fi_claim_fail_all_claimed_tags)
{
	__test_fail_all_claimed_tags(&default_hlist_attr);
}

Test(gnix_tags_basic_posted_hlist, fi_claim_fail_peek_all_claimed)
{
	__test_fail_peek_all_claimed(&default_hlist_attr);
}

/* unexpected list src address matching tests */
Test(gnix_tags_basic_posted_hlist, src_addr_match_success)
{
	__test_src_addr_match(&default_hlist_attr);
}

Test(gnix_tags_basic_posted_hlist, src_addr_no_match_wrong_addr)
{
	__test_src_addr_fail_wrong_src_addr(&default_hlist_attr);
}

Test(gnix_tags_basic_posted_hlist, src_addr_match_unspec)
{
	__test_src_addr_match_unspec(&default_hlist_attr);
}


/*
 * unexpected hlist tests
 */

Test(gnix_tags_basic_unexpected_hlist, single_insert_remove)
{
	int i;
	struct gnix_fr_element request = {
		.req = {
			.msg = {
				.tag = 0xA5A5A5A5,
				.ignore = 0xFFFFFFFF
			},
		},
		.peek_flags = FI_PEEK,
	};
	struct gnix_tag_storage_attr attr;

	memcpy(&attr, &default_hlist_attr, sizeof(struct gnix_tag_storage_attr));

	for (i = 0; i < TEST_OVERLAY_MAX; i++) {

		single_insert_peek_remove(test_tag_storage,
				&test_masks[i], &request);

		/* make necessary alterations to the structure
		 * for the next test mask
		 */
		reset_test_tag_storage(test_tag_storage, &attr);
	}
}

ParameterizedTestParameters(gnix_tags_basic_unexpected_hlist, multiple_ipr_tags)
{
	size_t nb_params = sizeof (ipr_params) / sizeof (struct ipr_test_params);
	return cr_make_param_array(struct ipr_test_params, ipr_params, nb_params);
}

ParameterizedTest(struct ipr_test_params *params,
		gnix_tags_basic_unexpected_hlist, multiple_ipr_tags)
{
	__test_multiple_type_ipr_reqs(params->elements, &default_hlist_attr,
			params->make_requests);
}

Test(gnix_tags_basic_unexpected_hlist, multiple_8_duplicate_tags)
{
	__test_multiple_8_duplicate_tags(&default_hlist_attr);
}

Test(gnix_tags_basic_unexpected_hlist, not_found_non_empty)
{
	__test_not_found_non_empty();
}

Test(gnix_tags_basic_unexpected_hlist, not_found_empty)
{
	__test_not_found_empty();
}

Test(gnix_tags_basic_unexpected_hlist, ignore_mask_set)
{
	__test_ignore_mask_set(&default_hlist_attr);
}

Test(gnix_tags_basic_unexpected_hlist, fi_claim_pass)
{
	__test_claim_pass(&default_hlist_attr);
}

Test(gnix_tags_basic_unexpected_hlist, fi_claim_fail_no_claimed_tags)
{
	__test_fail_no_claimed_tags(&default_hlist_attr);
}

Test(gnix_tags_basic_unexpected_hlist, fi_claim_fail_all_claimed_tags)
{
	__test_fail_all_claimed_tags(&default_hlist_attr);
}

Test(gnix_tags_basic_unexpected_hlist, fi_claim_fail_peek_all_claimed)
{
	__test_fail_peek_all_claimed(&default_hlist_attr);
}

/* unexpected list src address matching tests */
Test(gnix_tags_basic_unexpected_hlist, src_addr_match_success)
{
	__test_src_addr_match(&default_hlist_attr);
}

Test(gnix_tags_basic_unexpected_hlist, src_addr_no_match_wrong_addr)
{
	__test_src_addr_fail_wrong_src_addr(&default_hlist_attr);
}

Test(gnix_tags_basic_unexpected_hlist, src_addr_match_unspec)
{
	__test_src_addr_match_unspec(&default_hlist_attr);
}

