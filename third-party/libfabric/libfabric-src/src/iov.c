/*
 * Copyright (c) 2016 Intel Corp., Inc.  All rights reserved.
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

#include "config.h"

#include <string.h>

#include <ofi.h>
#include <ofi_iov.h>

size_t ofi_copy_iov_buf(const struct iovec *iov, size_t iov_count, size_t iov_offset,
			void *buf, size_t bufsize, int dir)
{
	size_t done = 0, len;
	char *iov_buf;
	size_t i;

	for (i = 0; i < iov_count && bufsize; i++) {
		len = ofi_iov_bytes_to_copy(&iov[i], &bufsize, &iov_offset,
					    &iov_buf);
		if (!len)
			continue;

		if (dir == OFI_COPY_BUF_TO_IOV)
			memcpy(iov_buf, (char *) buf + done, len);
		else if (dir == OFI_COPY_IOV_TO_BUF)
			memcpy((char *) buf + done, iov_buf, len);

		done += len;
	}
	return done;
}

void ofi_consume_iov_desc(struct iovec *iov, void **desc,
			  size_t *iov_count, size_t to_consume)
{
	struct iovec *cur_iov;
	void **cur_desc;

	assert(*iov_count);
	assert(to_consume <= ofi_total_iov_len(iov, *iov_count));

	if (*iov_count == 1) {
		if (iov[0].iov_len == to_consume) {
			*iov_count = 0;
			return;
		}
		goto out;
	}

	cur_iov = iov;
	cur_desc = desc;
	while (*iov_count && to_consume >= cur_iov->iov_len) {
		to_consume -= cur_iov->iov_len;
		cur_iov++;
		cur_desc++;
		*iov_count -= 1;
	}
	if (*iov_count == 0)
		return;

	if (iov != cur_iov) {
		memmove(iov, cur_iov, sizeof(*iov) * (*iov_count));
		if (desc)
			memmove(desc, cur_desc, sizeof(*desc) * (*iov_count));
	}
out:
	iov[0].iov_base = (uint8_t *)iov[0].iov_base + to_consume;
	iov[0].iov_len -= to_consume;
}

void ofi_consume_iov(struct iovec *iov, size_t *iov_count, size_t to_consume)
{
	ofi_consume_iov_desc(iov, NULL, iov_count, to_consume);
}

void ofi_consume_rma_iov(struct fi_rma_iov *rma_iov, size_t *rma_iov_count,
			 size_t to_consume)
{
	struct fi_rma_iov *cur_iov;

	assert(*rma_iov_count);
	assert(to_consume <= ofi_total_rma_iov_len(rma_iov, *rma_iov_count));

	if (*rma_iov_count == 1) {
		 if (rma_iov[0].len == to_consume) {
			*rma_iov_count = 0;
			return;
		 }
		 goto out;
	}

	cur_iov = rma_iov;
	while (*rma_iov_count && to_consume >= cur_iov->len) {
		to_consume -= cur_iov->len;
		cur_iov++;
		*rma_iov_count -= 1;
	}
	if (*rma_iov_count == 0)
		return;

	if (rma_iov != cur_iov) {
		memmove(rma_iov, cur_iov, sizeof(*rma_iov) * (*rma_iov_count));
	}
out:
	rma_iov[0].addr += to_consume;
	rma_iov[0].len -= to_consume;
}

int ofi_truncate_iov(struct iovec *iov, size_t *iov_count, size_t new_size)
{
	size_t i;

	for (i = 0; i < *iov_count; i++) {
		if (new_size <= iov[i].iov_len) {
			iov[i].iov_len = new_size;
			*iov_count = i + 1;
			return FI_SUCCESS;
		}
		new_size -= iov[i].iov_len;
	}
	return new_size ? -FI_ETRUNC : FI_SUCCESS;
}

/* Copy 'len' bytes worth of src iovec to dst */
int ofi_copy_iov_desc(struct iovec *dst_iov, void **dst_desc, size_t *dst_count,
		      struct iovec *src_iov, void **src_desc, size_t src_count,
		      size_t *index, size_t *offset, size_t len)
{
	size_t i, j;

	assert(*index < src_count);
	assert(*offset <= src_iov[*index].iov_len);

	for (i = 0, j = *index; j < src_count; i++, j++) {
		dst_iov[i].iov_base = (uint8_t *)src_iov[j].iov_base + *offset;
		if (src_desc)
			dst_desc[i] = src_desc[j];

		if (len <= src_iov[j].iov_len - *offset) {
			dst_iov[i].iov_len = len;
			*dst_count = i + 1;

			if (len == src_iov[j].iov_len - *offset) {
				*index = j + 1;
				*offset = 0;
			} else {
				*index = j;
				*offset += len;
			}
			return 0;
		}
		dst_iov[i].iov_len = src_iov[j].iov_len - *offset;
		len -= dst_iov[i].iov_len;
		*offset = 0;
	}
	return -FI_ETOOSMALL;
}

/* Copy 'len' bytes worth of src fi_rma_iov to dst */
int ofi_copy_rma_iov(struct fi_rma_iov *dst_iov, size_t *dst_count,
		struct fi_rma_iov *src_iov, size_t src_count,
		size_t *index, size_t *offset, size_t len)
{
	size_t i, j;

	assert(*index < src_count);
	assert(*offset <= src_iov[*index].len);

	for (i = 0, j = *index; j < src_count; i++, j++) {
		dst_iov[i].addr	= src_iov[j].addr + *offset;
		dst_iov[i].key	= src_iov[j].key;

		if (len <= src_iov[j].len - *offset) {
			dst_iov[i].len = len;
			*dst_count = i + 1;

			if (len == src_iov[j].len - *offset) {
				*index = j + 1;
				*offset = 0;
			} else {
				*index = j;
				*offset += len;
			}
			return 0;
		}
		dst_iov[i].len = src_iov[j].len - *offset;
		len -= dst_iov[i].len;
		*offset = 0;
	}
	return -FI_ETOOSMALL;
}
