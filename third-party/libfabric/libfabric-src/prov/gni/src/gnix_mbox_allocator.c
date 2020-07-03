/*
 * Copyright (c) 2015-2016 Los Alamos National Security, LLC.
 *                         All rights reserved.
 * Copyright (c) 2015,2017-2018 Cray Inc.  All rights reserved.
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

#include <stdio.h>
#include <sys/statfs.h>
#include <sys/mman.h>
#include <mntent.h>

#include "gnix_mbox_allocator.h"
#include "gnix_nic.h"
#include "fi_ext_gni.h"

bool gnix_mbox_alloc_allow_fallback = true;

/**
 * Will attempt to find a directory in the hugetlbfs with the given page size.
 *
 * @param[in] page_size	 Page size to look for in the hugetlbfs
 * @param[out] directory Double pointer to string that will point to
 * directory name.
 *
 * @return FI_SUCCESS	on successfully finding a huge page. The directory
 * pointer contains the string that represents the directory name.
 *
 * @return -FI_EINVAL	if an invalid parameter was given
 * @return -FI_EIO	if an error occurred while opening the /proc/mounts
 * file.
 */
static int __find_huge_page(size_t page_size, char **directory)
{
	int rc = -FI_EINVAL;
	struct statfs pg_size;
	struct mntent *mntent;
	FILE *fd;

	if (!directory || !page_size) {
		GNIX_WARN(FI_LOG_EP_CTRL,
			  "Invalid page_size or directory provided.\n");
		return -FI_EINVAL;
	}

	fd = setmntent ("/proc/mounts", "r");
	if (fd == NULL) {
		GNIX_WARN(FI_LOG_EP_CTRL,
			  "Unable to open /proc/mounts - %s.\n",
			  strerror(errno));
		return -FI_EIO;
	}

	while ((mntent = getmntent(fd)) != NULL) {

		if (strcmp (mntent->mnt_type, "hugetlbfs") != 0) {
			continue;
		}

		if (statfs(mntent->mnt_dir, &pg_size) == 0) {
			if (pg_size.f_bsize == page_size) {
				*directory = strdup(mntent->mnt_dir);
				rc = FI_SUCCESS;
				break;
			}
		}
	}

	endmntent(fd);

	return rc;
}

/**
 * Will attempt to find a directory in hugetlbfs using the given page size and
 * create a filename to use for backing an mmap.
 *
 * @param[in] page_size Page size to look for in the hugetlbfs
 * @param[out] filename Pointer containing filename after generation.
 *
 * @return FI_SUCCESS	On successfully finding a huge page and generating a
 * file name.
 *
 * @return -FI_EINVAL	if an invalid parameter was given
 * @return -FI_EIO	if an error occurred while opening the /proc/mounts
 * file. This is propagated from __find_huge_page.
 * @return -FI_ENOMEM	if an error occurred while allocating space for the
 * filename.
 */
static int __generate_file_name(size_t page_size, char **filename)
{
	static const char basename[] = "gnix_map";
	char *full_filename = NULL;
	char *huge_page = NULL;
	char *error;
	char error_buf[256];
	int my_file_id;
	int size;
	int ret;

	if (!filename) {
		GNIX_WARN(FI_LOG_EP_CTRL, "filename pointer is NULL.\n");
		ret = -FI_EINVAL;
		goto err_invalid;
	}

	ret = __find_huge_page(page_size, &huge_page);
	if (ret != FI_SUCCESS) {
		GNIX_WARN(FI_LOG_EP_CTRL,
			  "Find huge page returned error %s\n",
			  fi_strerror(-ret));
		goto err_invalid;
	}

	my_file_id = ofi_atomic_inc32(&file_id_counter);
	size = snprintf(NULL, 0, "%s/%s.%d.%d", huge_page, basename, getpid(),
			my_file_id);
	if (size < 0) {
		error = strerror_r(errno, error_buf, sizeof(error_buf));
		GNIX_WARN(FI_LOG_EP_CTRL,
			  "Error while gathering size for snprintf: %s\n",
			  error);
		goto err_snprintf;
	}

	full_filename = malloc(size + 1);
	if (!full_filename) {
		error = strerror_r(errno, error_buf, sizeof(error_buf));
		GNIX_WARN(FI_LOG_EP_CTRL,
			  "Error allocating full_filename: %s\n",
			  error);
		ret = -FI_ENOMEM;
		goto err_snprintf;
	}

	sprintf(full_filename, "%s/%s.%d.%d", huge_page, basename, getpid(),
		my_file_id);

	GNIX_DEBUG(FI_LOG_EP_CTRL, "Generated filename: %s\n", full_filename);

	*filename = full_filename;

err_snprintf:
	free(huge_page);
err_invalid:
	return ret;
}

/**
 * Find huge page, generate filename, open huge page, and attach huge page
 * descriptor to handle.
 *
 * @param[in] handle	Handle to the allocator being used.
 *
 * @return FI_SUCCESS	On successfully opening a huge page.
 *
 * @return -FI_EINVAL	if an invalid parameter was given. Propagated from
 * __generate_file_name.
 * @return -FI_EIO	if an error occurred while opening the hugepage
 * @return -FI_ENOMEM	if an error in space allocation occurred. Propagated
 * from __generate_file_name.
 */
static int __open_huge_page(struct gnix_mbox_alloc_handle *handle)
{
	char *filename = NULL;
	char error_buf[256];
	char *error;
	int ret;
	int fd;

	GNIX_TRACE(FI_LOG_EP_CTRL, "\n");

	handle->fd = -1;
	handle->filename = NULL;

	ret = __generate_file_name(handle->page_size, &filename);
	if (ret < 0) {
		GNIX_WARN(FI_LOG_EP_CTRL, "Error in generating file name.\n");
		goto err_filename;
	}

	fd = open(filename, O_CREAT | O_RDWR | O_EXCL, 0700);
	if (fd < 0) {
		error = strerror_r(errno, error_buf, sizeof(error_buf));
		GNIX_WARN(FI_LOG_EP_CTRL, "IO Error: %s\n", error);
		ret = -FI_EIO;
		goto err_open;
	}

	handle->fd = fd;
	handle->filename = filename;

	GNIX_DEBUG(FI_LOG_EP_CTRL,
		   "Successfully opened: %s with handle : %d\n.",
		   handle->filename, handle->fd);

	unlink(handle->filename);

	return ret;

err_open:
	free(filename);
err_filename:
	return ret;
}

/**
 * Determine how many pages need to be allocated.
 *
 * @param[in] handle	Handle to the allocator being used.
 *
 * @return Number of pages that need to be allocated rounded up to the nearest
 * multiple of the page size.
 */
static size_t __page_count(struct gnix_mbox_alloc_handle *handle)
{
	size_t total_size = CEILING((handle->mbox_size * handle->mpmmap),
				     handle->page_size);
	size_t page_count;

	page_count = total_size / handle->page_size;

	GNIX_DEBUG(FI_LOG_EP_CTRL,
		   "Mbox_size: %zu, mpmmap: %zu, page_size: %zu\n",
		   handle->mbox_size, handle->mpmmap, handle->page_size);

	GNIX_DEBUG(FI_LOG_EP_CTRL,
		   "Total size: %zu, page_count: %zu\n", total_size,
		   page_count);

	if (page_count <= 0) {
		GNIX_WARN(FI_LOG_EP_CTRL,
			  "Invalid size requested, truncating to single page.\n");
		page_count = 1;
	}

	return page_count;
}

/**
 * Determine how many mboxes are in a requested allocation size.
 *
 * @param[in] handle	Handle to the allocator being used.
 *
 * @return Number of mail boxes being allocated.
 */
static size_t __mbox_count(struct gnix_mbox_alloc_handle *handle)
{
	size_t mbox_count = (__page_count(handle) * handle->page_size) /
			    handle->mbox_size;

	GNIX_DEBUG(FI_LOG_EP_CTRL,
		   "Mbox_count: %zu.\n", mbox_count);
	return mbox_count;
}

/**
 * Create a slab from a handle and append to the slab list.
 *
 * @param[in] handle	Handle to the allocator being used.
 *
 * @return FI_SUCCESS	On successful slab creation.
 *
 * @return -FI_ENOMEM	if failure to allocate memory for slab or bitmap.
 * @return [Unspec]	if failure in alloc_bitmap. Will return error code from
 * alloc_bitmap.
 * @return [Unspec]	if failure in GNI_MemRegister. Converts gni_return_t
 * status code to FI_ERRNO value.
 */
static int __create_slab(struct gnix_mbox_alloc_handle *handle)
{
	struct gnix_slab *slab;
	gni_return_t status;
	char error_buf[256];
	char *error;
	size_t total_size;
	int ret, mflags;
	int vmdh_index = -1;
	int flags = GNI_MEM_READWRITE;
	struct gnix_auth_key *info;
	struct fi_gni_auth_key key;

	GNIX_TRACE(FI_LOG_EP_CTRL, "\n");

	slab = calloc(1, sizeof(*slab));
	if (!slab) {
		error = strerror_r(errno, error_buf, sizeof(error_buf));
		GNIX_WARN(FI_LOG_EP_CTRL,
			  "Error allocating slab: %s\n",
			  error);
		ret = -FI_ENOMEM;
		goto err_slab_calloc;
	}

	total_size = handle->page_size * __page_count(handle);
	GNIX_DEBUG(FI_LOG_EP_CTRL, "total_size requested for mmap: %zu.\n",
		   total_size);

	slab->used = calloc(1, sizeof(*(slab->used)));
	if (!slab->used) {
		error = strerror_r(errno, error_buf, sizeof(error_buf));
		GNIX_WARN(FI_LOG_EP_CTRL,
			  "Error allocating bitmap: %s\n",
			  error);
		ret = -FI_ENOMEM;
		goto err_bitmap_calloc;
	}

	mflags = MAP_SHARED;
	if (handle->fd == -1)
		mflags |= MAP_ANONYMOUS;

	slab->base = mmap(0, total_size, (PROT_READ | PROT_WRITE), mflags,
			  handle->fd, handle->last_offset);
	if (slab->base == MAP_FAILED) {
		error = strerror_r(errno, error_buf, sizeof(error_buf));
		GNIX_WARN(FI_LOG_EP_CTRL, "%s\n", error);
		ret = -FI_ENOMEM;
		goto err_mmap;
	}

	ret = _gnix_alloc_bitmap(slab->used, __mbox_count(handle), NULL);
	if (ret) {
		GNIX_WARN(FI_LOG_EP_CTRL, "Error allocating bitmap.\n");
		goto err_alloc_bitmap;
	}

	COND_ACQUIRE(handle->nic_handle->requires_lock, &handle->nic_handle->lock);
	if (handle->nic_handle->using_vmdh) {
		key.type = GNIX_AKT_RAW;
		key.raw.protection_key = handle->nic_handle->cookie;

		info = _gnix_auth_key_lookup((uint8_t *) &key, sizeof(key));
		assert(info);

		if (!handle->nic_handle->mdd_resources_set) {
			/* check to see if the ptag registration limit was set
			 * yet or not -- becomes read-only after success */
			_gnix_auth_key_enable(info);

			status = GNI_SetMddResources(
				handle->nic_handle->gni_nic_hndl,
				(info->attr.prov_key_limit +
				 info->attr.user_key_limit));
			assert(status == GNI_RC_SUCCESS);

			handle->nic_handle->mdd_resources_set = 1;
		}

		vmdh_index = _gnix_get_next_reserved_key(info);
		if (vmdh_index <= 0) {
			GNIX_FATAL(FI_LOG_DOMAIN,
				"failed to get reserved key for mbox "
				"registration, rc=%d\n",
				vmdh_index);
		}
		flags |= GNI_MEM_USE_VMDH;
	}

	status = GNI_MemRegister(handle->nic_handle->gni_nic_hndl,
				 (uint64_t) slab->base, total_size,
				 handle->cq_handle,
				 flags, vmdh_index,
				 &slab->memory_handle);
	COND_RELEASE(handle->nic_handle->requires_lock, &handle->nic_handle->lock);
	if (status != GNI_RC_SUCCESS) {
		GNIX_WARN(FI_LOG_EP_CTRL, "GNI_MemRegister failed: %s\n",
			  gni_err_str[status]);
		ret = gnixu_to_fi_errno(status);
		goto err_memregister;
	}

	slab->allocator = handle;

	gnix_slist_insert_tail(&slab->list_entry, &handle->slab_list);

	handle->last_offset += total_size;

	return ret;

err_memregister:
	_gnix_free_bitmap(slab->used);
err_alloc_bitmap:
	munmap(slab->base, total_size);
err_mmap:
	free(slab->used);
err_bitmap_calloc:
	free(slab);
err_slab_calloc:
	return ret;
}

/**
 * Destroy a slab.
 *
 * @param[in] handle	Handle to the allocator being used.
 * @param[in] slab	Slab to be destroyed.
 *
 * @return FI_SUCCESS	On successful slab destruction.
 *
 * @return -FI_EINVAL	On invalid handle or slab being given as parameters.
 */
static int __destroy_slab(struct gnix_mbox_alloc_handle *handle,
			  struct gnix_slab *slab)
{
	size_t total_size;

	GNIX_TRACE(FI_LOG_EP_CTRL, "\n");

	if (!handle || !slab) {
		GNIX_WARN(FI_LOG_EP_CTRL,
			  "Invalid argument handle or slab.\n");
		return -FI_EINVAL;
	}

	total_size = handle->page_size * __page_count(handle);

	_gnix_free_bitmap(slab->used);
	free(slab->used);

	COND_ACQUIRE(handle->nic_handle->requires_lock, &handle->nic_handle->lock);
	GNI_MemDeregister(handle->nic_handle->gni_nic_hndl,
			  &slab->memory_handle);
	COND_RELEASE(handle->nic_handle->requires_lock, &handle->nic_handle->lock);

	munmap(slab->base, total_size);

	free(slab);

	return FI_SUCCESS;
}

/**
 * Iterate over all slab bitmaps associated with an allocation handle and run
 * action function on each bitmap.
 *
 * @param[in] handle	Handle to the allocator being used.
 * @param[out] slab	Contains slab which made action evaluate to true.
 *
 * @return Index into slab which made action evaluate to true.
 * @return -FI_EAGAIN	On failure of action across all slab bitmaps.
 */
static int __check_bitmap(struct gnix_mbox_alloc_handle *handle,
			  struct gnix_slab **slab,
			  int (*action)(gnix_bitmap_t *))
{
	struct slist_entry *entry;
	struct gnix_slab *temp;
	int ret = FI_SUCCESS;

	*slab = NULL;

	for (entry = handle->slab_list.head; entry; entry = entry->next) {
		temp = container_of(entry, struct gnix_slab, list_entry);
		ret = action(temp->used);
		if (ret >= 0) {
			*slab = temp;
			break;
		}
	}

	return ret;
}

/**
 * Iterate over all slab bitmaps associated with an allocation handle and
 * search for the first free piece of memory.
 *
 * @param[in] handle	Handle to the allocator being used.
 * @param[out] slab	Contains slab which contained a free piece of memory.
 *
 * @return Index into slab which is free.
 * @return -FI_EAGAIN	Upon not finding any free memory.
 */
static int __find_free(struct gnix_mbox_alloc_handle *handle,
		       struct gnix_slab **slab)
{
	return __check_bitmap(handle, slab, _gnix_find_first_zero_bit);
}

/**
 * Iterate over all slab bitmaps associated with an allocation handle and
 * search for the first used piece of memory.
 *
 * @param[in] handle	Handle to the allocator being used.
 * @param[out] slab	Contains slab which contained a free piece of memory.
 *
 * @return Index into slab which is used.
 * @return -FI_EAGAIN	Upon not finding any used memory.
 */
static int __find_used(struct gnix_mbox_alloc_handle *handle,
		       struct gnix_slab **slab)
{
	return __check_bitmap(handle, slab, _gnix_find_first_set_bit);
}

/**
 * Fill all of the fields of an mbox to be returned to the requester.
 *
 * @param[in] handle	Handle to the allocator being used.
 * @param[in] slab	Slab which the mbox is allocated from.
 * @param[in] position	Position of the mbox in the slab.
 * @param[out] ptr	Contains the allocated mbox upon success.
 *
 * @return FI_SUCCESS	Upon successfully filling an mbox with relevant data.
 * @return -FI_EINVAL	Upon receiving invalid input, or finding the bitmap in
 * a corrupted state.
 * @return -FI_ENOMEM	Upon failure to create the mbox structure using calloc.
 */
static int __fill_mbox(struct gnix_mbox_alloc_handle *handle,
		       struct gnix_slab *slab, size_t position,
		       struct gnix_mbox **ptr)
{
	struct gnix_mbox *out;
	int ret = FI_SUCCESS;
	char error_buf[256];
	size_t mapped_size;
	char *error;

	out = calloc(1, sizeof(*out));
	if (!out) {
		error = strerror_r(errno, error_buf, sizeof(error_buf));
		GNIX_WARN(FI_LOG_EP_CTRL,
			  "Error allocating mbox: %s\n",
			  error);
		ret = -FI_ENOMEM;
		goto err_mbox_calloc;
	}

	mapped_size = handle->page_size * __page_count(handle);

	out->slab = slab;
	out->base = slab->base;
	out->offset = (position * handle->mbox_size);
	out->memory_handle = &slab->memory_handle;

	if (out->offset > mapped_size) {
		GNIX_WARN(FI_LOG_EP_CTRL, "Mbox out of bounds.\n");
		ret = -FI_EINVAL;
		goto err_invalid;
	}

	/* On some systems, the page may not be zero'd from first use.
		Memset it here */
	memset((void *) ((uint64_t) out->base + out->offset),
		0x0, handle->mbox_size);

	ret = _gnix_test_and_set_bit(slab->used, position);
	if (ret != 0) {
		GNIX_WARN(FI_LOG_EP_CTRL,
			  "Bit already set when creating mbox.\n");
		ret = -FI_EINVAL;
		goto err_invalid;
	}

	*ptr = out;

	return ret;

err_invalid:
	free(out);
err_mbox_calloc:
	return ret;
}

int _gnix_mbox_allocator_create(struct gnix_nic *nic,
				gni_cq_handle_t cq_handle,
				enum gnix_page_size page_size,
				size_t mbox_size,
				size_t mpmmap,
				struct gnix_mbox_alloc_handle **alloc_handle)
{
	struct gnix_mbox_alloc_handle *handle;
	char error_buf[256];
	char *error;
	int ret;

	GNIX_TRACE(FI_LOG_EP_CTRL, "\n");

	if (!nic || !mbox_size || !mpmmap || !alloc_handle) {
		GNIX_WARN(FI_LOG_EP_CTRL,
			  "Invalid parameter to allocator_create.\n");
		return -FI_EINVAL;
	}

	*alloc_handle = NULL;

	handle = calloc(1, sizeof(*handle));
	if (!handle) {
		error = strerror_r(errno, error_buf, sizeof(error_buf));
		GNIX_WARN(FI_LOG_EP_CTRL,
			  "Error allocating alloc handle: %s\n",
			  error);
		return -FI_ENOMEM;
	}

	handle->page_size = page_size * 1024 * 1024;
	handle->mbox_size = mbox_size;
	handle->mpmmap = mpmmap;
	handle->nic_handle = nic;
	handle->cq_handle = cq_handle;
	fastlock_init(&handle->lock);

	ret = __open_huge_page(handle);
	if (ret == FI_SUCCESS) {
		ret = __create_slab(handle);
		if (ret != FI_SUCCESS) {
			GNIX_WARN(FI_LOG_EP_CTRL, "Slab creation failed.\n");
		}
	} else {
		GNIX_WARN(FI_LOG_EP_CTRL, "Error opening huge page.\n");
	}

	/*
	 * try plan B - try to use anonymous mapping (base page size).
	 * If a file was successfully opened, close fd and free filename
	 * field in the handle.
	 */

	if ((ret != FI_SUCCESS) &&
		(gnix_mbox_alloc_allow_fallback == true)) {
		if (handle->filename != NULL) {
			free(handle->filename);
			handle->filename = NULL;
		}
		if (handle->fd != -1) {
			ret = close(handle->fd);
			handle->fd = -1;
			if (ret) {
				GNIX_WARN(FI_LOG_EP_CTRL,
				 "Error closing huge page - %d\n",
				  ret);
			}
		}

		ret = __create_slab(handle);
		if (ret != FI_SUCCESS) {
			GNIX_WARN(FI_LOG_EP_CTRL,
				"Slab(anon) creation failed.\n");
		}
	}

	if (ret == FI_SUCCESS) {
		*alloc_handle = handle;
	} else {
		free(handle);
	}

	return ret;
}

int _gnix_mbox_allocator_destroy(struct gnix_mbox_alloc_handle *alloc_handle)
{
	struct slist_entry *entry;
	struct gnix_slab *temp;
	char error_buf[256];
	int position;
	char *error;
	int ret = FI_SUCCESS;

	GNIX_TRACE(FI_LOG_EP_CTRL, "\n");

	if (!alloc_handle) {
		GNIX_WARN(FI_LOG_EP_CTRL, "Invalid alloc handle.\n");
		return -FI_EINVAL;
	}

	position = __find_used(alloc_handle, &temp);
	if (position >= 0) {
		GNIX_WARN(FI_LOG_EP_CTRL,
			  "Can't destroy, not all mailboxes have been returned (pos = %d).\n",
			  position);
		return -FI_EBUSY;
	}

	while (!slist_empty(&alloc_handle->slab_list)) {
		entry = slist_remove_head(&alloc_handle->slab_list);

		temp = container_of(entry, struct gnix_slab, list_entry);

		ret = __destroy_slab(alloc_handle, temp);
		if (ret)
			GNIX_WARN(FI_LOG_EP_CTRL,
				  "Error destroying slab.\n");
	}

	if (alloc_handle->filename != NULL)
		free(alloc_handle->filename);

	if (alloc_handle->fd != -1)
		ret = close(alloc_handle->fd);

	if (ret) {
		error = strerror_r(errno, error_buf, sizeof(error_buf));
		GNIX_WARN(FI_LOG_EP_CTRL,
			  "Error closing map file: %s\n",
			  error);
	}

	fastlock_destroy(&alloc_handle->lock);

	free(alloc_handle);

	return FI_SUCCESS;
}

int _gnix_mbox_alloc(struct gnix_mbox_alloc_handle *alloc_handle,
		     struct gnix_mbox **ptr)
{
	struct gnix_slab *slab;
	int position;
	int ret;

	GNIX_TRACE(FI_LOG_EP_CTRL, "\n");

	if (!alloc_handle || !ptr) {
		GNIX_WARN(FI_LOG_EP_CTRL, "Invalid alloc_handle or ptr.\n");
		ret = -FI_EINVAL;
		goto err;
	}

	fastlock_acquire(&alloc_handle->lock);
	position = __find_free(alloc_handle, &slab);
	if (position < 0) {
		GNIX_DEBUG(FI_LOG_EP_CTRL, "Creating new slab.\n");
		ret = __create_slab(alloc_handle);
		if (ret) {
			GNIX_WARN(FI_LOG_EP_CTRL, "Slab creation failed.\n");
			goto err;
		}

		slab = container_of(alloc_handle->slab_list.tail,
				    struct gnix_slab, list_entry);
		position = ret;
	}

	ret = __fill_mbox(alloc_handle, slab, (size_t) position, ptr);
	if (ret)
		GNIX_WARN(FI_LOG_EP_CTRL, "Creating mbox failed.\n");

	fastlock_release(&alloc_handle->lock);
err:
	return ret;
}

int _gnix_mbox_free(struct gnix_mbox *ptr)
{
	size_t position;
	int ret;
	fastlock_t *lock;

	GNIX_TRACE(FI_LOG_EP_CTRL, "\n");

	if (!ptr || !ptr->slab || !ptr->slab->allocator) {
		GNIX_WARN(FI_LOG_EP_CTRL, "Invalid mbox given to free.\n");
		return -FI_EINVAL;
	}

	lock = &ptr->slab->allocator->lock;
	fastlock_acquire(lock);
	position = ptr->offset / ptr->slab->allocator->mbox_size;

	ret = _gnix_test_and_clear_bit(ptr->slab->used, position);
	if (ret != 1) {
		GNIX_WARN(FI_LOG_EP_CTRL,
			  "Bit already cleared while freeing mbox.\n");
		fastlock_release(lock);
		return -FI_EINVAL;
	}

	free(ptr);
	fastlock_release(lock);

	return FI_SUCCESS;
}
