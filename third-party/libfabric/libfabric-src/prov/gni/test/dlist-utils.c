/*
 * Copyright (c) 2015-2016 Cray Inc. All rights reserved.
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
#include <sys/time.h>
#include <stdlib.h>

#include <ofi_list.h>

#include "gnix_util.h"

#include <criterion/criterion.h>
#include "gnix_rdma_headers.h"

struct element {
	int val;
	struct dlist_entry entry;
};

static void setup(void)
{
	srand(time(NULL));
}

static void teardown(void)
{
}

static void generate_perm(int *perm, int len)
{
	int i;
	/* good 'nuff */
	for (i = 0; i < len; i++) {
		int t = perm[i];
		int j = rand() % len;

		perm[i] = perm[j];
		perm[j] = t;
	}
}

TestSuite(dlist_utils, .init = setup, .fini = teardown);

struct dlist_test {
	char dummy[3];
	struct dlist_entry le;
	int x;
};

Test(dlist_utils, for_each)
{
	int i;
	const int n = 593;
	struct dlist_entry dl;
	struct dlist_test dt[n];
	struct dlist_test *elem;
	int perm[n];

	for (i = 0; i < n; i++)
		perm[i] = i;

	generate_perm(perm, n);

	dlist_init(&dl);
	for (i = 0; i < n; i++) {
		dt[perm[i]].x = i;
		dlist_insert_tail(&dt[perm[i]].le, &dl);
	}

	i = 0;
	dlist_for_each(&dl, elem, le) {
		cr_assert(elem->x == i);
		++i;
	}
}

Test(dlist_utils, for_each_empty)
{
	struct dlist_entry dl;
	struct dlist_test *elem;

	dlist_init(&dl);

	dlist_for_each(&dl, elem, le) {
		cr_assert(false);
	}
}

Test(dlist_utils, for_each_safe)
{
	int i;
	const int n = 1129;
	struct dlist_entry dl;
	struct dlist_test dt[n];
	struct dlist_test *elem, *next;
	int perm[n];

	for (i = 0; i < n; i++)
		perm[i] = i;

	generate_perm(perm, n);

	dlist_init(&dl);
	for (i = 0; i < n; i++) {
		dt[perm[i]].x = i;
		dlist_insert_tail(&dt[perm[i]].le, &dl);
	}

	i = 0;
	dlist_for_each_safe(&dl, elem, next, le) {
		cr_assert(elem->x == i);
		dlist_remove(&elem->le);
		++i;
	}
}

Test(dlist_utils, for_each_safe_empty)
{
	struct dlist_entry dl;
	struct dlist_test *elem, *next;

	dlist_init(&dl);

	dlist_for_each_safe(&dl, elem, next, le) {
		cr_assert(false);
	}
}

Test(dlist_utils, dlist_splice_head_test)
{
	struct dlist_entry list1, list2;
	struct element values[4], *current;
	int i;
	int expected[4] = {2, 3, 0, 1};

	for (i = 0; i < 4; i++) {
		values[i].val = i;
		dlist_init(&values[i].entry);
	}

	dlist_init(&list1);
	dlist_init(&list2);
	dlist_insert_tail(&values[0].entry, &list1);
	dlist_insert_tail(&values[1].entry, &list1);

	dlist_insert_tail(&values[2].entry, &list2);
	dlist_insert_tail(&values[3].entry, &list2);

	dlist_splice_head(&list1, &list2);

	cr_assert(dlist_empty(&list2));

	i = 0;
	dlist_for_each(&list1, current, entry)
	{
		cr_assert(current->val == expected[i]);
		i++;
	}

}

Test(dlist_utils, dlist_splice_tail_test)
{
	struct dlist_entry list1, list2;
	struct element values[4], *current;
	int i;
	int expected[4] = {0, 1, 2, 3};

	for (i = 0; i < 4; i++) {
		values[i].val = i;
		dlist_init(&values[i].entry);
	}

	dlist_init(&list1);
	dlist_init(&list2);
	dlist_insert_tail(&values[0].entry, &list1);
	dlist_insert_tail(&values[1].entry, &list1);

	dlist_insert_tail(&values[2].entry, &list2);
	dlist_insert_tail(&values[3].entry, &list2);

	dlist_splice_tail(&list1, &list2);

	cr_assert(dlist_empty(&list2));

	i = 0;
	dlist_for_each(&list1, current, entry)
	{
		cr_assert(current->val == expected[i]);
		i++;
	}


}
