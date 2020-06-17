/*
 * Copyright (c) 2017 Cray Inc.  All rights reserved.
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

#ifndef _GNIX_SHMEM_H
#define _GNIX_SHMEM_H

#include <stdint.h>


struct gnix_shared_memory {
	void *addr;
	uint32_t size;
};

/**
 * @brief Creates a shared memory region
 *
 * @param path       path to shared memory region
 * @param size       size of shared memory region in bytes
 * @param init_func  initialization function for region, if created
 * @param region     pointer to returned memory region
 */
int _gnix_shmem_create(
	const char *path,
	uint32_t size,
	int (*init_func)(const char *path, uint32_t size, void *region),
	struct gnix_shared_memory *region);

/**
 * @brief Destroys a shared memory region
 *
 * @param region  memory region to destroy
 */
int _gnix_shmem_destroy(struct gnix_shared_memory *region);

#endif
