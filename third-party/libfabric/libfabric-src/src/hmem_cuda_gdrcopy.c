/*
 * Copyright (c) 2020 Amazon.com, Inc. or its affiliates.
 * All rights reserved.
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

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include "ofi_hmem.h"
#include "ofi.h"
#include "ofi_iov.h"

#if HAVE_GDRCOPY

#include <pthread.h>
#include <gdrapi.h>

struct gdrcopy_handle {
	gdr_mh_t mh; /* memory handler */
	void *cuda_ptr; /* page aligned gpu pointer */
	void *user_ptr; /* user space ptr mapped to GPU memory */
	size_t length; /* page aligned length */
};

struct gdrcopy_ops {
	gdr_t (*gdr_open)();
	int (*gdr_close)(gdr_t g);
	int (*gdr_pin_buffer)(gdr_t g, unsigned long addr, size_t size,
			      uint64_t p2p_token, uint32_t va_space,
			      gdr_mh_t *handle);
	int (*gdr_unpin_buffer)(gdr_t g, gdr_mh_t handle);
	int (*gdr_map)(gdr_t g, gdr_mh_t handle, void **va, size_t size);
	int (*gdr_unmap)(gdr_t g, gdr_mh_t handle, void *va, size_t size);
	int (*gdr_copy_to_mapping)(gdr_mh_t handle, void *map_d_ptr,
				   const void *h_ptr, size_t size);
	int (*gdr_copy_from_mapping)(gdr_mh_t handle, void *map_d_ptr,
				     const void *h_ptr, size_t size);
};

enum gdrcopy_dir {
	GDRCOPY_TO_DEVICE,
	GDRCOPY_FROM_DEVICE,
};

static gdr_t global_gdr;
static pthread_spinlock_t global_gdr_lock;

#if ENABLE_GDRCOPY_DLOPEN

#include <dlfcn.h>

static void *gdrapi_handle;
static struct gdrcopy_ops global_gdrcopy_ops;

static int cuda_gdrcopy_dl_hmem_init(void)
{
	gdrapi_handle = dlopen("libgdrapi.so", RTLD_NOW);
	if (!gdrapi_handle) {
		FI_INFO(&core_prov, FI_LOG_CORE,
			"Failed to dlopen libgdrapi.so\n");
		return -FI_ENOSYS;
	}

	global_gdrcopy_ops.gdr_open = dlsym(gdrapi_handle, "gdr_open");
	if (!global_gdrcopy_ops.gdr_open) {
		FI_WARN(&core_prov, FI_LOG_CORE, "Failed to find gdr_open\n");
		goto err_dlclose_gdrapi;
	}

	global_gdrcopy_ops.gdr_close = dlsym(gdrapi_handle, "gdr_close");
	if (!global_gdrcopy_ops.gdr_close) {
		FI_WARN(&core_prov, FI_LOG_CORE, "Failed to find gdr_close\n");
		goto err_dlclose_gdrapi;
	}

	global_gdrcopy_ops.gdr_pin_buffer = dlsym(gdrapi_handle, "gdr_pin_buffer");
	if (!global_gdrcopy_ops.gdr_pin_buffer) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"Failed to find gdr_pin_buffer\n");
		goto err_dlclose_gdrapi;
	}

	global_gdrcopy_ops.gdr_unpin_buffer = dlsym(gdrapi_handle, "gdr_unpin_buffer");
	if (!global_gdrcopy_ops.gdr_unpin_buffer) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"Failed to find gdr_unpin_buffer\n");
		goto err_dlclose_gdrapi;
	}

	global_gdrcopy_ops.gdr_map = dlsym(gdrapi_handle, "gdr_map");
	if (!global_gdrcopy_ops.gdr_map) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"Failed to find gdr_map\n");
		goto err_dlclose_gdrapi;
	}

	global_gdrcopy_ops.gdr_unmap = dlsym(gdrapi_handle, "gdr_unmap");
	if (!global_gdrcopy_ops.gdr_unmap) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"Failed to find gdr_unmap\n");
		goto err_dlclose_gdrapi;
	}

	global_gdrcopy_ops.gdr_copy_to_mapping = dlsym(gdrapi_handle, "gdr_copy_to_mapping");
	if (!global_gdrcopy_ops.gdr_copy_to_mapping) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"Failed to find gdr_copy_to_mapping\n");
		goto err_dlclose_gdrapi;
	}

	global_gdrcopy_ops.gdr_copy_from_mapping = dlsym(gdrapi_handle, "gdr_copy_from_mapping");
	if (!global_gdrcopy_ops.gdr_copy_from_mapping) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"Failed to find gdr_copy_from_mapping\n");
		goto err_dlclose_gdrapi;
	}

	return FI_SUCCESS;

err_dlclose_gdrapi:
	memset(&global_gdrcopy_ops, 0, sizeof(global_gdrcopy_ops));
	dlclose(gdrapi_handle);
	return -FI_ENODATA;
}

static int cuda_gdrcopy_dl_hmem_cleanup(void)
{
	dlclose(gdrapi_handle);
	return FI_SUCCESS;
}

#else

static struct gdrcopy_ops global_gdrcopy_ops = {
	.gdr_open = gdr_open,
	.gdr_close = gdr_close,
	.gdr_pin_buffer = gdr_pin_buffer,
	.gdr_unpin_buffer = gdr_unpin_buffer,
	.gdr_map = gdr_map,
	.gdr_unmap = gdr_unmap,
	.gdr_copy_to_mapping = gdr_copy_to_mapping,
	.gdr_copy_from_mapping = gdr_copy_from_mapping
};

static int cuda_gdrcopy_dl_hmem_init(void)
{
	return FI_SUCCESS;
}

static int cuda_gdrcopy_dl_hmem_cleanup(void)
{
	return FI_SUCCESS;
}

#endif /* ENABLE_CUDA_DLOPEN */

int cuda_gdrcopy_hmem_init(void)
{
	int err;

	err = cuda_gdrcopy_dl_hmem_init();
	if (err) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"gdrcopy_dl_hmem_init failed!\n");
		return -FI_ENOSYS;
	}

	assert(global_gdrcopy_ops.gdr_open);

	global_gdr = global_gdrcopy_ops.gdr_open();
	if (!global_gdr) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"gdr_open failed!\n");
		err = -FI_ENOMEM;
		goto error_exit;
	}

	err = pthread_spin_init(&global_gdr_lock, 0);
	if (err) {
		assert(global_gdrcopy_ops.gdr_close);
		global_gdrcopy_ops.gdr_close(global_gdr);
		err = -err;
		goto error_exit;
	}

	return 0;

error_exit:
	cuda_gdrcopy_dl_hmem_cleanup();
	return err;
}

int cuda_gdrcopy_hmem_cleanup(void)
{
	int err, ret = 0;

	err = pthread_spin_destroy(&global_gdr_lock);
	if (err) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"destroy global_gdr_lock failed! err: %s\n",
			strerror(err));
		ret = err;
	}

	assert(global_gdrcopy_ops.gdr_close);
	err = global_gdrcopy_ops.gdr_close(global_gdr);
	if (err) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"close global_gdr failed! err: %s\n",
			strerror(err));
		ret = err;
	}

	err = cuda_gdrcopy_dl_hmem_cleanup();
	if (err) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"cuda_gdrcopy_dl_hmem_cleaup() failed! err: %s\n",
			strerror(err));
		ret = err;
	}

	return ret;
}

void cuda_gdrcopy_impl(uint64_t handle, void *devptr,
		       void *hostptr, size_t len,
		       enum gdrcopy_dir dir)
{
	ssize_t off;
	struct gdrcopy_handle *gdrcopy;
	void *gdrcopy_user_ptr;

	assert(global_gdrcopy_ops.gdr_copy_to_mapping);
	assert(handle);

	gdrcopy = (struct gdrcopy_handle *)handle;
	off = (char *)devptr - (char *)gdrcopy->cuda_ptr;
	assert(off >= 0 && off + len <= gdrcopy->length);
	gdrcopy_user_ptr = (char *)gdrcopy->user_ptr + off;
	if (dir == GDRCOPY_TO_DEVICE) {
		global_gdrcopy_ops.gdr_copy_to_mapping(gdrcopy->mh,
						       gdrcopy_user_ptr,
						       hostptr, len);
	} else {
		assert(dir == GDRCOPY_FROM_DEVICE);
		global_gdrcopy_ops.gdr_copy_from_mapping(gdrcopy->mh,
							 hostptr,
							 gdrcopy_user_ptr,
							 len);
	}
}

void cuda_gdrcopy_to_dev(uint64_t handle, void *devptr,
			 const void *hostptr, size_t len)
{
	cuda_gdrcopy_impl(handle, devptr, (void *)hostptr, len,
			  GDRCOPY_TO_DEVICE);
}

void cuda_gdrcopy_from_dev(uint64_t handle, void *hostptr,
			   const void *devptr, size_t len)
{
	cuda_gdrcopy_impl(handle, (void *)devptr, hostptr, len,
			  GDRCOPY_FROM_DEVICE);
}

int cuda_gdrcopy_dev_register(const void *buf, size_t len, uint64_t *handle)
{
	int err;
	uintptr_t regbgn, regend;
	size_t reglen;
	struct gdrcopy_handle *gdrcopy;

	assert(global_gdr);
	assert(global_gdrcopy_ops.gdr_pin_buffer);
	assert(global_gdrcopy_ops.gdr_map);

	regbgn = (uintptr_t)ofi_get_page_start(buf, GPU_PAGE_SIZE);
	regend = (uintptr_t)buf + len;
	reglen = ofi_get_aligned_size(regend - regbgn, GPU_PAGE_SIZE);

	gdrcopy = malloc(sizeof(struct gdrcopy_handle));
	if (!gdrcopy)
		return -FI_ENOMEM;

	assert(global_gdr);
	pthread_spin_lock(&global_gdr_lock);
	err = global_gdrcopy_ops.gdr_pin_buffer(global_gdr, regbgn,
					 reglen, 0, 0, &gdrcopy->mh);
	if (err) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"gdr_pin_buffer failed! error: %s ptr: %p len: %ld\n",
			strerror(err), buf, len);
		free(gdrcopy);
		goto exit;
	}

	gdrcopy->cuda_ptr = (void *)regbgn;
	gdrcopy->length = reglen;

	err = global_gdrcopy_ops.gdr_map(global_gdr, gdrcopy->mh,
					 &gdrcopy->user_ptr, gdrcopy->length);
	if (err) {
		FI_WARN(&core_prov, FI_LOG_CORE, "gdr_map failed! error: %s\n",
			strerror(err));
		global_gdrcopy_ops.gdr_unpin_buffer(global_gdr, gdrcopy->mh);
		free(gdrcopy);
		goto exit;
	}

	*handle = (uint64_t)gdrcopy;
exit:
	pthread_spin_unlock(&global_gdr_lock);
	return err;
}

int cuda_gdrcopy_dev_unregister(uint64_t handle)
{
	int err;
	struct gdrcopy_handle *gdrcopy;

	assert(global_gdr);
	assert(global_gdrcopy_ops.gdr_unmap);
	assert(global_gdrcopy_ops.gdr_unpin_buffer);

	gdrcopy = (struct gdrcopy_handle *)handle;
	assert(gdrcopy);

	pthread_spin_lock(&global_gdr_lock);
	err = global_gdrcopy_ops.gdr_unmap(global_gdr, gdrcopy->mh,
					   gdrcopy->user_ptr, gdrcopy->length);
	if (err) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"gdr_unmap failed! error: %s\n",
			strerror(err));
		goto exit;
	}

	err = global_gdrcopy_ops.gdr_unpin_buffer(global_gdr, gdrcopy->mh);
	if (err) {
		FI_WARN(&core_prov, FI_LOG_MR,
			"gdr_unpin_buffer failed! error: %s\n",
			strerror(err));
		goto exit;
	}

exit:
	pthread_spin_unlock(&global_gdr_lock);
	free(gdrcopy);
	return err;
}

#else

int cuda_gdrcopy_hmem_init(void)
{
	return -FI_ENOSYS;
}

int cuda_gdrcopy_hmem_cleanup(void)
{
	return FI_SUCCESS;
}

void cuda_gdrcopy_to_dev(uint64_t devhandle, void *devptr,
			 const void *hostptr, size_t len)
{
}

void cuda_gdrcopy_from_dev(uint64_t devhandle, void *hostptr,
			   const void *devptr, size_t len)
{
}

int cuda_gdrcopy_dev_register(const void *buf, size_t len, uint64_t *handle)
{
	return FI_SUCCESS;
}

int cuda_gdrcopy_dev_unregister(uint64_t handle)
{
	return FI_SUCCESS;
}

ssize_t ofi_gdrcopy_to_cuda_iov(uint64_t handle, const struct iovec *iov,
                                size_t iov_count, uint64_t iov_offset,
                                const void *host, size_t len)
{
	return -FI_ENOSYS;
}

ssize_t ofi_gdrcopy_from_cuda_iov(uint64_t handle, void *host,
                                  const struct iovec *iov, size_t iov_count,
                                  uint64_t iov_offset, size_t len)
{
	return -FI_ENOSYS;
}

#endif /* HAVE_GDRCOPY */
