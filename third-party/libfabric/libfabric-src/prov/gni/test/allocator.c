/*
 * Copyright (c) 2015-2018 Los Alamos National Security, LLC. All rights reserved.
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
#include <stddef.h>

#include "gnix.h"
#include "gnix_mbox_allocator.h"

#include <criterion/criterion.h>
#include "gnix_rdma_headers.h"
#include "common.h"

#define ALLOCD_WITH_NIC 0

/* Note: Set to ~FI_NOTIFY_FLAGS_ONLY since this was written before api 1.5 */
static uint64_t mode_bits = ~FI_NOTIFY_FLAGS_ONLY;
static struct fid_fabric *fab;
static struct fid_domain *dom;
static struct fid_ep *ep;
static struct fi_info *hints;
static struct fi_info *fi;
static struct gnix_fid_ep *ep_priv;
static struct gnix_mbox_alloc_handle *allocator;

static void __allocator_setup(uint32_t version, int mr_mode)
{
	int ret = 0;

	hints = fi_allocinfo();
	cr_assert(hints, "fi_allocinfo");

	hints->domain_attr->cq_data_size = 4;
	hints->mode = mode_bits;
	hints->domain_attr->mr_mode = mr_mode;

	hints->fabric_attr->prov_name = strdup("gni");

	ret = fi_getinfo(version, NULL, 0, 0, hints, &fi);
	cr_assert_eq(ret, FI_SUCCESS, "fi_getinfo");

	ret = fi_fabric(fi->fabric_attr, &fab, NULL);
	cr_assert_eq(ret, FI_SUCCESS, "fi_fabric");

	ret = fi_domain(fab, fi, &dom, NULL);
	cr_assert_eq(ret, FI_SUCCESS, "fi_domain");

	ret = fi_endpoint(dom, fi, &ep, NULL);
	cr_assert_eq(ret, FI_SUCCESS, "fi_endpoint");

	ep_priv = container_of(ep, struct gnix_fid_ep, ep_fid);
}

static void allocator_setup_basic(void)
{
	__allocator_setup(fi_version(), GNIX_MR_BASIC);
}

static void allocator_setup_scalable(void)
{
	__allocator_setup(fi_version(), GNIX_MR_SCALABLE);
}

void allocator_teardown(void)
{
	int ret = 0;

	ret = fi_close(&ep->fid);
	cr_assert_eq(ret, FI_SUCCESS, "failure in closing ep.");
	ret = fi_close(&dom->fid);
	cr_assert_eq(ret, FI_SUCCESS, "failure in closing domain.");
	ret = fi_close(&fab->fid);
	cr_assert_eq(ret, FI_SUCCESS, "failure in closing fabric.");
	fi_freeinfo(fi);
	fi_freeinfo(hints);
}

/*
 * Count how many slabs are present in an allocator.
 */
static size_t count_slabs(struct gnix_mbox_alloc_handle *handle)
{
	size_t count = 0;

	for (struct slist_entry *entry = handle->slab_list.head; entry;
	     entry = entry->next) {
		count++;
	}

	return count;
}

/*
 * Absolute value function that returns a ptrdiff_t.
 */
static ptrdiff_t abs_value(ptrdiff_t x)
{
	return x * ((x > 0) - (x < 0));
}

#ifndef __aarch64__
/*
 * Open /proc/self/maps and count the number of times the hugetlbfs
 * string is present. Return value is the count;
 *
 * TODO: this approach doesn't work on Cray ARM systems.  Large
 *       page regions don't show being backed by files in 
 *       /var/lib/hugetlbfs.  Need to fix with something better.
 */
static int verify_hugepages(void)
{
	int ret = 0;
	FILE *fd;
	char *line;
	size_t size = 1024;

	fd = fopen("/proc/self/maps", "r");
	if (!fd) {
		fprintf(stderr, "error opening /proc/self/maps.\n");
		return ret;
	}

	line = malloc(size);
	if (!line) {
		fprintf(stderr, "error mallocing space for line.\n");
		return ret;
	}

	while (getline(&line, &size, fd) != -1) {
		if (strstr(line, "hugetlbfs")) {
			ret++;
		}
	}

	free(line);
	fclose(fd);

	return ret;
}
#endif

/*
 * Open an allocator with the given parameters and immediately close it. Verify
 * that everything returned a successful error code.  Note that for large
 * page sizes over ~64 MB, it can be iffy whether or not large pages can
 * be synthesized if the linux page cache has become highly fragmented, so
 * we have a may fail parameter that checks to see if the error return is
 * -FI_ENOMEM, in which case don't treat as fatal.
 */
static void open_close_allocator(enum gnix_page_size page_size,
				 size_t mbox_size,
				 size_t mpmmap, bool may_fail)
{
	int ret;

	ret = _gnix_mbox_allocator_create(ep_priv->nic, NULL, page_size,
					  mbox_size, mpmmap, &allocator);
	if ((ret == -FI_ENOMEM) && (may_fail == true)) {
		fprintf(stderr, "Allocation of page size %d MB failed -"
				"skipping\n", page_size);
		return;
	}
	cr_assert_eq(ret, FI_SUCCESS, "_gnix_mbox_allocator_create failed5.");
#ifndef __aarch64__
	cr_expect_eq(verify_hugepages(), 2 + ALLOCD_WITH_NIC,
			 "memory not found in /proc/self/maps.");
#endif

	ret = _gnix_mbox_allocator_destroy(allocator);
	cr_assert_eq(ret, FI_SUCCESS, "_gnix_mbox_allocator_destroy failed.");
#ifndef __aarch64__
	cr_expect_eq(verify_hugepages(), 1 + ALLOCD_WITH_NIC,
			 "memory not released in /proc/self/maps.");
#endif
}


TestSuite(mbox_creation_basic,
	  .init = allocator_setup_basic,
	  .fini = allocator_teardown);

TestSuite(mbox_creation_scalable,
	  .init = allocator_setup_scalable,
	  .fini = allocator_teardown);

static inline void __alloc_single_page(void)
{
	/*
	 * Test creation of all predefined page sizes.
	 */
	open_close_allocator(GNIX_PAGE_2MB, 100, 100, false);
	open_close_allocator(GNIX_PAGE_4MB, 100, 100, false);
	open_close_allocator(GNIX_PAGE_8MB, 100, 100, false);
	open_close_allocator(GNIX_PAGE_16MB, 100, 100, false);
	open_close_allocator(GNIX_PAGE_32MB, 100, 100, false);
	open_close_allocator(GNIX_PAGE_64MB, 100, 100, true);
	open_close_allocator(GNIX_PAGE_128MB, 100, 100, true);
	open_close_allocator(GNIX_PAGE_256MB, 100, 100, true);
	open_close_allocator(GNIX_PAGE_512MB, 100, 100, true);
}

Test(mbox_creation_basic, alloc_single_page)
{
	__alloc_single_page();
}

Test(mbox_creation_scalable, alloc_single_page)
{
	__alloc_single_page();
}


Test(mbox_creation_basic, alloc_three_pages)
{
	/*
	 * This should allocate a single slab that's 3 pages in size.
	 */
	open_close_allocator(GNIX_PAGE_4MB, 1000, 12000, false);
}

Test(mbox_creation_scalable, alloc_three_pages)
{
	/*
	 * This should allocate a single slab that's 3 pages in size.
	 */
	open_close_allocator(GNIX_PAGE_4MB, 1000, 12000, false);
}

static inline void __alloc_mbox(void)
{
	int ret;

	struct gnix_mbox *mail_box;
	struct slist_entry *entry;
	struct gnix_slab *slab;

	char test_string[] = "hello allocator.";

	ret = _gnix_mbox_allocator_create(ep_priv->nic, NULL, GNIX_PAGE_4MB,
					  1000, 12000, &allocator);
	cr_assert_eq(ret, FI_SUCCESS, "_gnix_mbox_allocator_create failed1.");

	/*
	 *value is 4 because the provider has internally already opened
	 * an mbox allocator and 2 rdma slabs at this point.
	 */
#ifndef __aarch64__
	cr_expect_eq(verify_hugepages(), 2 + ALLOCD_WITH_NIC,
		  "memory not found in /proc/self/maps.");
#endif

	ret = _gnix_mbox_alloc(allocator, &mail_box);
	cr_expect_eq(ret, FI_SUCCESS, "_gnix_mbox_alloc failed.");

	cr_expect(mail_box);

	entry = allocator->slab_list.head;
	cr_assert(entry);

	slab = container_of(entry, struct gnix_slab, list_entry);

	cr_expect_eq(mail_box->slab, slab,
		  "slab list head and mail box slab pointer are not equal.");
	cr_expect_eq(mail_box->memory_handle, &mail_box->slab->memory_handle,
		 "mail_box memory handle not equal to slab memory handle.");
	cr_expect_eq(mail_box->offset, 0, "offset is not 0.");
	cr_expect_eq(mail_box->base, mail_box->slab->base,
		     "mail_box base not equal to slab base.");

	/*
	 * Write our test strings and make sure they're equal.
	 */
	memcpy(mail_box->base, test_string, sizeof(test_string));
	cr_expect_str_eq((char *) mail_box->base, test_string);

	/*
	 * Mailboxes haven't been returned so destroy will return -FI_EBUSY.
	 */
	ret = _gnix_mbox_allocator_destroy(allocator);
	cr_assert_eq(ret, -FI_EBUSY,
		  "_gnix_mbox_allocator_destroy should have returned -FI_EBUSY.");

	/*
	 * Free allocated mailboxes so we can destroy cleanly.
	 */
	ret = _gnix_mbox_free(mail_box);
	cr_expect_eq(ret, FI_SUCCESS, "_gnix_mbox_free failed.");

	ret = _gnix_mbox_allocator_destroy(allocator);
	cr_assert_eq(ret, FI_SUCCESS, "_gnix_mbox_allocator_destroy failed.");

#ifndef __aarch64__
	cr_expect_eq(verify_hugepages(), 1 + ALLOCD_WITH_NIC,
		     "memory not released in /proc/self/maps.");
#endif

}

Test(mbox_creation_basic, alloc_mbox)
{
	__alloc_mbox();
}

Test(mbox_creation_scalable, alloc_mbox)
{
	__alloc_mbox();
}

/*
 * Page size needs to be one of the predefined enums. 2200 is not a valid page
 * size. This actually gets expanded to 2200 * 1024 * 1024.
 */
static inline void __page_size_fail(void)
{
	int ret;

	ret = _gnix_mbox_allocator_create(ep_priv->nic, NULL, 2200,
					  1000, 12000, &allocator);
	cr_assert_eq(ret, -FI_EINVAL,
		     "Creating allocator with bogus page size succeeded.");
	cr_assert_eq(allocator, NULL);
	/*
	 *value is 3 because the provider has internally already opened
	 * an mbox allocator and two other slabs at this point.
	 */
#ifndef __aarch64__
	cr_expect_eq(verify_hugepages(), 1 + ALLOCD_WITH_NIC,
		     "Huge page open, but shouldn't be");
#endif

	ret = _gnix_mbox_allocator_destroy(allocator);
	cr_assert_eq(ret, -FI_EINVAL,
		     "_gnix_mbox_allocator_destroy succeeded on NULL handle.");
}

Test(mbox_creation_basic, page_size_fail)
{
	__page_size_fail();
}

Test(mbox_creation_scalable, page_size_fail)
{
	__page_size_fail();
}

static inline void __mbox_size_fail(void)
{
	int ret;

	/*
	 * mbox_size can't be zero.
	 */
	ret = _gnix_mbox_allocator_create(ep_priv->nic, NULL, GNIX_PAGE_4MB,
					  0, 12000, &allocator);
	cr_assert_eq(ret, -FI_EINVAL,
		     "Creating allocator with zero mbox size succeeded.");

	cr_assert_eq(allocator, NULL);
#ifndef __aarch64__
	cr_expect_eq(verify_hugepages(), 1 + ALLOCD_WITH_NIC,
		     "Huge page open, but shouldn't be");
#endif

	ret = _gnix_mbox_allocator_destroy(allocator);
	cr_assert_eq(ret, -FI_EINVAL,
		     "_gnix_mbox_allocator_destroy succeeded on NULL handle.");
}

Test(mbox_creation_basic, mbox_size_fail)
{
	__mbox_size_fail();
}

Test(mbox_creation_scalable, mbox_size_fail)
{
	__mbox_size_fail();
}

static inline void __mpmmap_size_fail(void)
{
	int ret;

	/*
	 * Can't have zero mailboxes per mmap.
	 */
	ret = _gnix_mbox_allocator_create(ep_priv->nic, NULL, GNIX_PAGE_4MB,
					  1000, 0, &allocator);
	cr_assert_eq(ret, -FI_EINVAL,
		  "Creating allocator with zero mailboxes per mmap succeeded.");
	cr_assert_eq(allocator, NULL);
#ifndef __aarch64__
	cr_expect_eq(verify_hugepages(), 1 + ALLOCD_WITH_NIC,
		     "Huge page open, but shouldn't be");
#endif

	ret = _gnix_mbox_allocator_destroy(allocator);
	cr_assert_eq(ret, -FI_EINVAL,
		     "_gnix_mbox_allocator_destroy succeeded on NULL handle.");
}

Test(mbox_creation_basic, mpmmap_size_fail)
{
	__mpmmap_size_fail();
}

Test(mbox_creation_scalable, mpmmap_size_fail)
{
	__mpmmap_size_fail();
}

static inline void __null_allocator_fail(void)
{
	int ret;

	/*
	 * Can't have a NULL allocator.
	 */
	ret = _gnix_mbox_allocator_create(ep_priv->nic, NULL, GNIX_PAGE_4MB,
					  1000, 100, NULL);
	cr_assert_eq(ret, -FI_EINVAL,
		     "Creating allocator with null allocator succeeded.");
#ifndef __aarch64__
	cr_expect_eq(verify_hugepages(), 1 + ALLOCD_WITH_NIC,
		     "Huge page open, but shouldn't be");
#endif

	ret = _gnix_mbox_allocator_destroy(allocator);
	cr_assert_eq(ret, -FI_EINVAL,
		     "_gnix_mbox_allocator_destroy succeeded on NULL handle.");
}

Test(mbox_creation_basic, null_allocator_fail)
{
	__null_allocator_fail();
}

Test(mbox_creation_scalable, null_allocator_fail)
{
	__null_allocator_fail();
}

static inline void __multi_allocation(void)
{
	int ret;

	size_t array_size = 5;
	size_t mbox_size = 1000;

	ptrdiff_t expected;
	ptrdiff_t actual;

	struct gnix_mbox *mbox_arr[array_size];

	ret = _gnix_mbox_allocator_create(ep_priv->nic, NULL, GNIX_PAGE_4MB,
					  mbox_size, array_size, &allocator);
	cr_assert_eq(ret, FI_SUCCESS, "_gnix_mbox_allocator_create failed2.");
#ifndef __aarch64__
	cr_expect_eq(verify_hugepages(), 2 + ALLOCD_WITH_NIC,
		     "memory not found in /proc/self/maps.");
#endif

	/*
	 * Create an array of mailboxes of size array_size.
	 */
	for (int i = 0; i < array_size; i++) {
		ret = _gnix_mbox_alloc(allocator, &(mbox_arr[i]));
		cr_expect_eq(ret, FI_SUCCESS, "_gnix_mbox_alloc failed.");
		cr_expect(mbox_arr[i]);
	}

	/*
	 * Compare each mailbox to each other mailbox excluding the diagonal.
	 * The expected base should be a function of the mbox_size and the
	 * difference between their positions in the array. We can verify this
	 * against the offset inside the mailbox object.
	 */
	for (int i = 0; i < array_size; i++) {
		for (int j = 0; j < array_size; j++) {
			if (i == j)
				continue;

			actual = abs_value(mbox_arr[i]->offset -
					   mbox_arr[j]->offset);
			expected = abs_value(i - j) * mbox_size;

			cr_expect_eq(actual, expected,
				     "Expected offsets and actual base offsets are not equal.");
		}
	}

	for (int i = 0; i < array_size; i++) {
		ret = _gnix_mbox_free(mbox_arr[i]);
		cr_expect_eq(ret, FI_SUCCESS, "_gnix_mbox_free failed.");
	}

	ret = _gnix_mbox_allocator_destroy(allocator);
	cr_assert_eq(ret, FI_SUCCESS, "_gnix_mbox_allocator_destroy failed.");

#ifndef __aarch64__
	cr_expect_eq(verify_hugepages(), 1 + ALLOCD_WITH_NIC,
		     "memory not released in /proc/self/maps.");
#endif
}

Test(mbox_creation_basic, multi_allocation)
{
	__multi_allocation();
}

Test(mbox_creation_scalable, multi_allocation)
{
	__multi_allocation();
}

static inline void __check_errors(void)
{
	int ret;

	struct gnix_mbox_alloc_handle *allocator;
	struct gnix_slab *slab;
	struct gnix_mbox *mail_box;
	size_t position;

	ret = _gnix_mbox_allocator_create(ep_priv->nic, NULL, GNIX_PAGE_4MB,
					  1000, 12000, &allocator);
	cr_assert_eq(ret, FI_SUCCESS, "_gnix_mbox_allocator_create failed3");
#ifndef __aarch64__
	cr_expect_eq(verify_hugepages(), 2 + ALLOCD_WITH_NIC,
		     "memory not found in /proc/self/maps.");
#endif

	ret = _gnix_mbox_alloc(allocator, &mail_box);
	cr_expect_eq(ret, FI_SUCCESS, "_gnix_mbox_alloc failed.");

	cr_expect(mail_box);

	/* Force various error paths */
	slab = mail_box->slab;
	mail_box->slab = NULL;
	ret = _gnix_mbox_free(mail_box);
	cr_expect_eq(ret, -FI_EINVAL, "_gnix_mbox_free did not fail.");
	mail_box->slab = slab;

	allocator = mail_box->slab->allocator;
	mail_box->slab->allocator = NULL;
	ret = _gnix_mbox_free(mail_box);
	cr_expect_eq(ret, -FI_EINVAL, "_gnix_mbox_free did not fail.");
	mail_box->slab->allocator = allocator;

	position = mail_box->offset / mail_box->slab->allocator->mbox_size;
	ret = _gnix_test_and_clear_bit(mail_box->slab->used, position);
	cr_expect_eq(ret, 1, "bitmap clear failed.");
	ret = _gnix_mbox_free(mail_box);
	cr_expect_eq(ret, -FI_EINVAL, "_gnix_mbox_free did not fail.");
	ret = _gnix_test_and_set_bit(mail_box->slab->used, position);
	cr_expect_eq(ret, 0, "bitmap set failed.");

	/*
	 * Free allocated mailboxes so we can destroy cleanly.
	 */
	ret = _gnix_mbox_free(mail_box);
	cr_expect_eq(ret, FI_SUCCESS, "_gnix_mbox_free failed.");

	ret = _gnix_mbox_allocator_destroy(allocator);
	cr_assert_eq(ret, FI_SUCCESS, "_gnix_mbox_allocator_destroy failed.");

#ifndef __aarch64__
	cr_expect_eq(verify_hugepages(), 1 + ALLOCD_WITH_NIC,
		     "memory not released in /proc/self/maps.");
#endif
}

Test(mbox_creation_basic, check_errors)
{
	__check_errors();
}

Test(mbox_creation_scalable, check_errors)
{
	__check_errors();
}

/*
 * Force the creation of two slabs by setting mpmmap to 1 and making a mailbox
 * the size of the entire page.
 */
static inline void __two_slabs(void)
{
	int ret;

	/*
	 * Only have one mail box per slab.
	 */
	size_t mbox_size = GNIX_PAGE_4MB * 1024 * 1024;
	size_t mpmmap = 1;

	struct gnix_mbox *mbox_arr[2];

	ret = _gnix_mbox_allocator_create(ep_priv->nic, NULL, GNIX_PAGE_4MB,
					  mbox_size, mpmmap, &allocator);
	cr_assert_eq(ret, FI_SUCCESS, "_gnix_mbox_allocator_create failed4.");
#ifndef __aarch64__
	cr_expect_eq(verify_hugepages(), 2 + ALLOCD_WITH_NIC,
		     "memory not found in /proc/self/maps.");
#endif

	/*
	 * Should use previously allocated slab
	 */
	ret = _gnix_mbox_alloc(allocator, &(mbox_arr[0]));
	cr_expect_eq(ret, FI_SUCCESS, "_gnix_mbox_alloc failed.");

	/*
	 * Will need another slab. Allocation will occur.
	 */
	ret = _gnix_mbox_alloc(allocator, &(mbox_arr[1]));
	cr_expect_eq(ret, FI_SUCCESS, "_gnix_mbox_alloc failed.");

	/*
	 * The bases should be different. The base is a per slab concept.
	 */
	cr_expect_neq(mbox_arr[0]->base, mbox_arr[1]->base,
		      "Bases are the same.");

	/*
	 * The linked list should contain two slabs.
	 */
	cr_expect_eq(2, count_slabs(allocator));

	ret = _gnix_mbox_free(mbox_arr[0]);
	cr_expect_eq(ret, FI_SUCCESS, "_gnix_mbox_free failed.");

	ret = _gnix_mbox_free(mbox_arr[1]);
	cr_expect_eq(ret, FI_SUCCESS, "_gnix_mbox_free failed.");

	ret = _gnix_mbox_allocator_destroy(allocator);
	cr_assert_eq(ret, FI_SUCCESS, "_gnix_mbox_allocator_destroy failed.");

#ifndef __aarch64__
	cr_expect_eq(verify_hugepages(), 1 + ALLOCD_WITH_NIC,
		     "memory not released in /proc/self/maps.");
#endif
}

Test(mbox_creation_basic, two_slabs)
{
	__two_slabs();
}

Test(mbox_creation_scalable, two_slabs)
{
	__two_slabs();
}
