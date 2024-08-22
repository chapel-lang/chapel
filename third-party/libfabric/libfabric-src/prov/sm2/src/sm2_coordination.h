/*
 * Copyright (c) Amazon.com, Inc. or its affiliates. All rights reserved.
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

#ifndef _SM2_COORDINATION_H_
#define _SM2_COORDINATION_H_

#include "config.h"

#include <stddef.h>
#include <stdint.h>
#include <sys/un.h>

#include <ofi_atom.h>
#include <ofi_hmem.h>
#include <ofi_mem.h>
#include <ofi_proto.h>
#include <ofi_rbuf.h>
#include <ofi_tree.h>

#include <rdma/providers/fi_prov.h>

#define SM2_XFER_ENTRY_SIZE   4096
#define SM2_MAX_UNIVERSE_SIZE 256
/* TODO: Tune max GDRCopy size for SM2 */
#define SM2_MAX_GDRCOPY_SIZE 3072
/* TODO: Make the number of XFER ENTRY's configurable */
#define SM2_NUM_XFER_ENTRY_PER_PEER 1024

typedef unsigned int sm2_gid_t;

struct sm2_mmap {
	char *base;
	size_t size;
	int fd;
};

struct sm2_ep_allocation_entry {
	int pid; /* This is for allocation startup */
	char ep_name[FI_NAME_MAX];
	bool startup_ready; /* TODO Do I need to make atomic */
};

struct sm2_coord_file_header {
	int file_version;
	pthread_mutex_t write_lock;
	/* TODO enforce that all procs in the file use this */
	int64_t ep_region_size;

	ptrdiff_t ep_allocation_offset; /* struct sm2_ep_allocation_entry */
	ptrdiff_t ep_regions_offset; /* struct ep_region */
};

struct sm2_attr {
	const char *name;
	uint16_t flags;
};

struct sm2_region {
	uint8_t version;
	uint8_t resv;
	uint16_t flags;

	/* offsets from start of sm2_region */
	ptrdiff_t recv_queue_offset;
	ptrdiff_t freestack_offset;
};

size_t sm2_calculate_size_offsets(ptrdiff_t *rq_offset, ptrdiff_t *fs_offset);
int sm2_create(const struct fi_provider *prov, const struct sm2_attr *attr,
	       struct sm2_mmap *sm2_mmap, sm2_gid_t *gid);

ssize_t sm2_mmap_cleanup(struct sm2_mmap *map);

ssize_t sm2_entry_allocate(const char *name, struct sm2_mmap *map,
			   sm2_gid_t *gid, bool self);
void sm2_entry_free(struct sm2_mmap *map, sm2_gid_t gid);

ssize_t sm2_file_open_or_create(struct sm2_mmap *map_shared);
void sm2_file_lock(struct sm2_mmap *map);
void sm2_file_unlock(struct sm2_mmap *map);

static inline struct sm2_ep_allocation_entry *
sm2_mmap_entries(struct sm2_mmap *map)
{
	struct sm2_coord_file_header *header = (void *) map->base;
	char *alloc_offset = map->base + header->ep_allocation_offset;
	return (struct sm2_ep_allocation_entry *) alloc_offset;
}

static inline struct sm2_region *sm2_mmap_ep_region(struct sm2_mmap *map,
						    sm2_gid_t gid)
{
	struct sm2_coord_file_header *header = (void *) map->base;
	return (struct sm2_region *) (map->base + header->ep_regions_offset +
				      header->ep_region_size * gid);
}

#endif /* _SM2_COORDINATION_H_ */
