/*
 * (C) Copyright 2023 UT-Battelle, LLC. All rights reserved.
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

#ifndef OFI_XPMEM_H
#define OFI_XPMEM_H

#include <ofi_mr.h>
#include <ofi_util.h>

#if HAVE_XPMEM
#include <xpmem.h>
#else
/* define XPMEM specific types to allow the code to
 * compile on different platforms and to allow smr_region to compile
 * without xpmem available.
 */
typedef int64_t xpmem_apid_t;
typedef int64_t xpmem_segid_t;
#endif /* HAVE_XPMEM */

struct xpmem_client {
	uint8_t cap;
	xpmem_apid_t apid;
	uintptr_t addr_max;
};

struct xpmem_pinfo {
	/* XPMEM segment id for this process */
	xpmem_segid_t seg_id;
	/* maximum attachment address for this process. attempts to attach
	 * past this value may fail. */
	uintptr_t address_max;
};

struct xpmem {
	struct xpmem_pinfo pinfo;
	/* maximum size that will be used with a single memcpy call.
	 * On some systems we see better peformance if we chunk the
	 * copy into multiple memcpy calls. */
	uint64_t memcpy_chunk_size;
};

extern struct xpmem *xpmem;

int ofi_xpmem_cache_search(struct ofi_mr_cache *cache,
			   struct iovec *iov, uint64_t peer_id,
			   struct ofi_mr_entry **mr_entry,
			   struct xpmem_client *xpmem);

int ofi_xpmem_cache_open(struct ofi_mr_cache **cache);
void ofi_xpmem_cache_destroy(struct ofi_mr_cache *cache);

int xpmem_init(void);
int xpmem_cleanup(void);
int xpmem_copy(struct iovec *local, unsigned long local_cnt,
	       struct iovec *remote, unsigned long remote_cnt,
	       size_t total, pid_t pid, bool write, void *user_data);
int ofi_xpmem_enable(struct xpmem_pinfo *peer,
		     struct xpmem_client *xpmem);
void ofi_xpmem_release(struct xpmem_client *xpmem);

#endif /* OFI_XPMEM_H */
