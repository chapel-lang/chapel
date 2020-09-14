/*
 * Copyright (c) 2015 Los Alamos National Security, LLC. All rights reserved.
 * Copyright (c) 2015-2016 Cray Inc.  All rights reserved.
 * Copyright (c) 2019 Triad National Security, LLC. All rights reserved.
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

#ifndef _GNIX_MBOX_ALLOCATOR_
#define _GNIX_MBOX_ALLOCATOR_

#include <stddef.h>

#include "gni_pub.h"

#include "gnix.h"
#include "gnix_bitmap.h"

enum gnix_page_size {
	GNIX_PAGE_2MB   = 2,
	GNIX_PAGE_4MB   = 4,
	GNIX_PAGE_8MB   = 8,
	GNIX_PAGE_16MB  = 16,
	GNIX_PAGE_32MB  = 32,
	GNIX_PAGE_64MB  = 64,
	GNIX_PAGE_128MB = 128,
	GNIX_PAGE_256MB = 256,
	GNIX_PAGE_512MB = 512
};

/**
 * Structure representing mailbox allocated by mbox_allocator.
 *
 * @var memory_handle	Memory handle returned by GNI_MemRegister.
 * @var slab		Slab from which mbox was allocated.
 * @var base		Pointer to the start of the memory returned by mmap.
 * @var offset		Offset from the base pointer where mailbox is located.
 */
struct gnix_mbox {
	gni_mem_handle_t *memory_handle;
	struct gnix_slab *slab;

	void *base;
	ptrdiff_t offset;
};

/**
 * Structure representing a slab of memory allocated by the mbox_allocator.
 *
 * @var base		The pointer to the start of memory returned by mmap.
 * @var allocator	Pointer to the parent allocator.
 * @var used		Bitmap of slab usage.
 * @var memory_handle	The handle returned by GNI_MemRegister.
 * @var list_entry	Entry for storing structure in slist structs.
 */
struct gnix_slab {
	void *base;
	struct gnix_mbox_alloc_handle *allocator;

	gnix_bitmap_t *used;
	gni_mem_handle_t memory_handle;

	struct slist_entry list_entry;
};

/**
 * Structure representing an mbox_allocator.
 *
 * @var nic_handle	Gnix_nic that the memory is associated with.
 * @var cq_handle	CQ handle that the memory is associated with.
 * @var last_offset	Last offset within file with name filename.
 * @var filename	File name created in the HUGETLBFS.
 * @var fd		Opened descriptor for file with name filename.
 * @var page_size	The page size used for HUGETLBFS and mmap.
 * @var mbox_size	Size per each mailbox.
 * @var mpmmap		Mailboxes per mmap slab.
 * @var slab_list	List of slab objects.
 *
 * @note  If HUGETLBFS is not available, memory is allocated via ANON mmap.
 */
struct gnix_mbox_alloc_handle {
	struct gnix_nic *nic_handle;
	fastlock_t lock;
	gni_cq_handle_t cq_handle;

	size_t last_offset;
	char *filename;
	int fd;

	size_t page_size;
	size_t mbox_size;
	size_t mpmmap;

	struct slist slab_list;
};

/**
 * Creates an allocator that can be used with mbox_alloc to allocate mailboxes.
 *
 * @param nic		IN	Gnix_nic memory is associated with.
 * @param cq_handle	IN	CQ handle memory is associated with.
 * @param page_size	IN	Page size used for HUGETLBFS and mmap.
 * @param mbox_size	IN	Size per each mailbox.
 * @param mpmmap	IN	Mailboxes per mmap slab.
 * @param alloc_handle	IN/OUT	Handle needed for use with mbox_alloc.
 *
 * @return FI_SUCCESS	Upon successfully creating an allocator.
 * @return -FI_EINVAL	Upon getting an invalid nic, mbox_size, mpmmap, or
 * alloc_handle.
 * @return -FI_ENOMEM	Upon failure to allocate a handle using calloc.
 * @return -FI_EIO	Upon failure to open a huge page.
 * @return [Unspec]	If failure in GNI_MemRegister. Converts gni_return_t
 * status code to FI_ERRNO value.
 */
int _gnix_mbox_allocator_create(struct gnix_nic *nic,
				gni_cq_handle_t cq_handle,
				enum gnix_page_size page_size,
				size_t mbox_size,
				size_t mpmmap,
				struct gnix_mbox_alloc_handle **alloc_handle);

/**
 * Releases all resources associated with an allocator handle.
 *
 * @param alloc_handle	IN	Alloc handle to destroy.
 *
 * @return FI_SUCCESS	Upon successfully destroying an allocator.
 * @return -FI_EINVAL	Upon receiving an invalid alloc handle.
 * @return -FI_EBUSY	Upon finding that there are still mailboxes allocated
 * that haven't been freed using gnix_mbox_free.
 */
int _gnix_mbox_allocator_destroy(struct gnix_mbox_alloc_handle *alloc_handle);

/**
 * Allocate a new mailbox.
 *
 * @param alloc_handle	IN	Gnix_mbox_alloc_handle to use as allocator.
 * @param ptr		IN/OUT	Pointer to gnix_mbox to be allocated.
 *
 * @return FI_SUCCESS	Upon successfully allocating a mailbox.
 * @return -FI_ENOMEM	Upon not being able to allocate memory for a slab or
 * bitmap.
 * @return -FI_EINVAL	Upon finding that input generates invalid location for
 * mbox.
 * @return [Unspec]	Upon failure in alloc_bitmap. Returns error code from
 * alloc_bitmap.
 * @return [Unspec]	Upon failure in GNI_MemRegister. Converts gni_return_t
 * to FI_ERRNO value.
 */
int _gnix_mbox_alloc(struct gnix_mbox_alloc_handle *alloc_handle,
		     struct gnix_mbox **ptr);

/**
 * Mark mailbox as free.
 *
 * @param ptr		IN	Pointer to allocated gnix_mbox to free.
 *
 * @return FI_SUCCESS	Upon successful free.
 * @return -FI_EINVAL	Upon an invalid parameter, or finding that the bitmap
 * is in a corrupted state.
 */
int _gnix_mbox_free(struct gnix_mbox *ptr);

/*
 * Initialized in gnix_init.c, used for updating filename when creating
 * hugepages.
 */
extern ofi_atomic32_t file_id_counter;

/*
 * safety valve for disabling mbox allocator fallback to base pages
 */

extern bool gnix_mbox_alloc_allow_fallback;
#endif /* _GNIX_MBOX_ALLOCATOR_ */
