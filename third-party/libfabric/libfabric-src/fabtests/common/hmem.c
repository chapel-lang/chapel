/*
 * Copyright (c) 2020 Intel Corporation.  All rights reserved.
 *
 * This software is available to you under the BSD license below:
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
 */

#if HAVE_CONFIG_H
	#include <config.h>
#endif

#include <inttypes.h>
#include <stdbool.h>
#include "hmem.h"

static bool hmem_initialized = false;

struct ft_hmem_ops {
	int (*init)(void);
	int (*cleanup)(void);
	int (*alloc)(uint64_t device, void **buf, size_t size);
	int (*free)(void *buf);
	int (*memset)(uint64_t device, void *buf, int value, size_t size);
	int (*copy_to_hmem)(uint64_t device, void *dst, const void *src,
			    size_t size);
	int (*copy_from_hmem)(uint64_t device, void *dst, const void *src,
			      size_t size);
};

static struct ft_hmem_ops hmem_ops[] = {
	[FI_HMEM_SYSTEM] = {
		.init = ft_host_init,
		.cleanup = ft_host_cleanup,
		.alloc = ft_host_alloc,
		.free = ft_host_free,
		.memset = ft_host_memset,
		.copy_to_hmem = ft_host_memcpy,
		.copy_from_hmem = ft_host_memcpy,
	},
	[FI_HMEM_CUDA] = {
		.init = ft_cuda_init,
		.cleanup = ft_cuda_cleanup,
		.alloc = ft_cuda_alloc,
		.free = ft_cuda_free,
		.memset = ft_cuda_memset,
		.copy_to_hmem = ft_cuda_copy_to_hmem,
		.copy_from_hmem = ft_cuda_copy_from_hmem,
	},
	[FI_HMEM_ROCR] = {
		.init = ft_rocr_init,
		.cleanup = ft_rocr_cleanup,
		.alloc = ft_rocr_alloc,
		.free = ft_rocr_free,
		.memset = ft_rocr_memset,
		.copy_to_hmem = ft_rocr_memcpy,
		.copy_from_hmem = ft_rocr_memcpy,
	},
	[FI_HMEM_ZE] = {
		.init = ft_ze_init,
		.cleanup = ft_ze_cleanup,
		.alloc = ft_ze_alloc,
		.free = ft_ze_free,
		.memset = ft_ze_memset,
		.copy_to_hmem = ft_ze_copy,
		.copy_from_hmem = ft_ze_copy,
	},
};

int ft_hmem_init(enum fi_hmem_iface iface)
{
	int ret;

	ret = hmem_ops[iface].init();
	if (ret == FI_SUCCESS)
		hmem_initialized = true;

	return ret;
}

int ft_hmem_cleanup(enum fi_hmem_iface iface)
{
	int ret = FI_SUCCESS;

	if (hmem_initialized) {
		ret = hmem_ops[iface].cleanup();
		if (ret == FI_SUCCESS)
			hmem_initialized = false;
	}

	return ret;
}

int ft_hmem_alloc(enum fi_hmem_iface iface, uint64_t device, void **buf,
		  size_t size)
{
	return hmem_ops[iface].alloc(device, buf, size);
}

int ft_hmem_free(enum fi_hmem_iface iface, void *buf)
{
	return hmem_ops[iface].free(buf);
}

int ft_hmem_memset(enum fi_hmem_iface iface, uint64_t device, void *buf,
		   int value, size_t size)
{
	return hmem_ops[iface].memset(device, buf, value, size);
}

int ft_hmem_copy_to(enum fi_hmem_iface iface, uint64_t device, void *dst,
		    const void *src, size_t size)
{
	return hmem_ops[iface].copy_to_hmem(device, dst, src, size);
}

int ft_hmem_copy_from(enum fi_hmem_iface iface, uint64_t device, void *dst,
		      const void *src, size_t size)
{
	return hmem_ops[iface].copy_from_hmem(device, dst, src, size);
}
