/*
 * Copyright (c) 2016 Intel Corporation.  All rights reserved.
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
 *
 */

#ifndef _OFI_IOV_H_
#define _OFI_IOV_H_

#include "config.h"

#include <ofi.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/uio.h>
#include <inttypes.h>
#include <rdma/fabric.h>


static inline size_t ofi_total_iov_len(const struct iovec *iov, size_t iov_count)
{
	size_t i, len = 0;
	for (i = 0; i < iov_count; i++)
		len += iov[i].iov_len;
	return len;
}

static inline size_t ofi_total_ioc_cnt(const struct fi_ioc *ioc, size_t ioc_count)
{
	size_t i, cnt = 0;
	for (i = 0; i < ioc_count; i++)
		cnt += ioc[i].count;
	return cnt;
}

static inline size_t ofi_total_rma_iov_len(const struct fi_rma_iov *rma_iov,
					   size_t iov_count)
{
	size_t i, len = 0;

	for (i = 0; i < iov_count; i++)
		len += rma_iov[i].len;
	return len;
}

static inline size_t ofi_total_rma_ioc_cnt(const struct fi_rma_ioc *rma_ioc,
					   size_t ioc_count)
{
	size_t i, cnt = 0;
	for (i = 0; i < ioc_count; i++)
		cnt += rma_ioc[i].count;
	return cnt;
}

#define OFI_COPY_IOV_TO_BUF 0
#define OFI_COPY_BUF_TO_IOV 1

uint64_t ofi_copy_iov_buf(const struct iovec *iov, size_t iov_count, uint64_t iov_offset,
			  void *buf, uint64_t bufsize, int dir);

static inline uint64_t
ofi_copy_to_iov(const struct iovec *iov, size_t iov_count, uint64_t iov_offset,
		void *buf, uint64_t bufsize)
{
	if (iov_count == 1) {
		uint64_t size = ((iov_offset > iov[0].iov_len) ?
				 0 : MIN(bufsize, iov[0].iov_len - iov_offset));

		memcpy((char *)iov[0].iov_base + iov_offset, buf, size);
		return size;
	} else {
		return ofi_copy_iov_buf(iov, iov_count, iov_offset, buf, bufsize,
					OFI_COPY_BUF_TO_IOV);
	}
}

static inline uint64_t
ofi_copy_from_iov(void *buf, uint64_t bufsize,
		  const struct iovec *iov, size_t iov_count, uint64_t iov_offset)
{
	if (iov_count == 1) {
		uint64_t size = ((iov_offset > iov[0].iov_len) ?
				 0 : MIN(bufsize, iov[0].iov_len - iov_offset));

		memcpy(buf, (char *)iov[0].iov_base + iov_offset, size);
		return size;
	} else {
		return ofi_copy_iov_buf(iov, iov_count, iov_offset, buf, bufsize,
					OFI_COPY_IOV_TO_BUF);
	}
}

static inline void ofi_ioc_to_iov(const struct fi_ioc *ioc, struct iovec *iov,
				  size_t count, size_t size)
{
	int i;
	for (i = 0; i < count; i++) {
		iov[i].iov_base = ioc[i].addr;
		iov[i].iov_len = ioc[i].count * size;
	}
}

static inline void ofi_rma_ioc_to_iov(const struct fi_rma_ioc *ioc,
				      struct fi_rma_iov *iov,
				      size_t count, size_t size)
{
	int i;
	for (i = 0; i < count; i++) {
		iov[i].addr = ioc[i].addr;
		iov[i].len = ioc[i].count * size;
		iov[i].key = ioc[i].key;
	}
}

static inline void *
ofi_iov_end(const struct iovec *iov)
{
	return ((char *) iov->iov_base) + iov->iov_len - 1;
}

static inline bool
ofi_iov_left(const struct iovec *iov1, const struct iovec *iov2)
{
	return ofi_iov_end(iov1) < iov2->iov_base;
}

static inline bool
ofi_iov_right(const struct iovec *iov1, const struct iovec *iov2)
{
	return iov1->iov_base > ofi_iov_end(iov2);
}

static inline bool
ofi_iov_shifted_left(const struct iovec *iov1, const struct iovec *iov2)
{
	return ((iov1->iov_base < iov2->iov_base) &&
		(ofi_iov_end(iov1) < ofi_iov_end(iov2)));
}

static inline bool
ofi_iov_shifted_right(const struct iovec *iov1, const struct iovec *iov2)
{
	return ((iov1->iov_base > iov2->iov_base) &&
		(ofi_iov_end(iov1) > ofi_iov_end(iov2)));
}

static inline bool
ofi_iov_within(const struct iovec *iov1, const struct iovec *iov2)
{
	return (iov1->iov_base >= iov2->iov_base) &&
	       (ofi_iov_end(iov1) <= ofi_iov_end(iov2));
}

void ofi_consume_iov(struct iovec *iovec, size_t *iovec_count, size_t offset);

void ofi_consume_iov_desc(struct iovec *iovec, void **desc,
			  size_t *iovec_count, size_t offset);

void ofi_consume_rma_iov(struct fi_rma_iov *rma_iov, size_t *rma_iov_count,
			 size_t length);

int ofi_truncate_iov(struct iovec *iov, size_t *iov_count, size_t new_size);

/* Copy 'len' bytes worth of src iovec to dst */
int ofi_copy_iov_desc(struct iovec *dst_iov, void **dst_desc, size_t *dst_count,
		      struct iovec *src_iov, void **src_desc, size_t src_count,
		      size_t *index, size_t *offset, size_t len);

/* Copy 'len' bytes worth of src fi_rma_iov to dst */
int ofi_copy_rma_iov(struct fi_rma_iov *dst_iov, size_t *dst_count,
		struct fi_rma_iov *src_iov, size_t src_count,
		size_t *index, size_t *offset, size_t len);
#endif /* _OFI_IOV_H_ */
