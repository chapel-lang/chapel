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
#endif /* HAVE_CONFIG_H */

#ifndef _OFI_CUDA_H_
#define _OFI_CUDA_H_
#if HAVE_LIBCUDA

#include <cuda.h>
#include <cuda_runtime.h>

static uint64_t
ofi_copy_cuda_iov_buf(const struct iovec *iov, size_t iov_count,
		      uint64_t iov_offset, void *buf,
		      uint64_t bufsize, int dir)
{
	uint64_t done = 0, len;
	char *iov_buf;
	size_t i;

	for (i = 0; i < iov_count && bufsize; i++) {
		len = iov[i].iov_len;

		if (iov_offset > len) {
			iov_offset -= len;
			continue;
		}

		iov_buf = (char *)iov[i].iov_base + iov_offset;
		len -= iov_offset;

		len = MIN(len, bufsize);
		if (dir == OFI_COPY_BUF_TO_IOV)
			cudaMemcpy(iov_buf, (char *) buf + done, len, cudaMemcpyHostToDevice);
		else if (dir == OFI_COPY_IOV_TO_BUF)
			cudaMemcpy((char *) buf + done, iov_buf, len, cudaMemcpyDeviceToHost);

		iov_offset = 0;
		bufsize -= len;
		done += len;
	}
	return done;
}

static inline uint64_t
ofi_copy_from_cuda_iov(void *buf, uint64_t bufsize,
		       const struct iovec *iov, size_t iov_count, uint64_t iov_offset)
{
	if (iov_count == 1) {
		uint64_t size = ((iov_offset > iov[0].iov_len) ?
				 0 : MIN(bufsize, iov[0].iov_len - iov_offset));

		cudaMemcpy(buf, (char *) iov[0].iov_base + iov_offset,
			   size, cudaMemcpyDeviceToHost);
		return size;
	} else {
		return ofi_copy_cuda_iov_buf(iov, iov_count, iov_offset, buf,
					     bufsize, OFI_COPY_IOV_TO_BUF);
	}
}

static inline uint64_t
ofi_copy_to_cuda_iov(const struct iovec *iov, size_t iov_count, uint64_t iov_offset,
		     void *buf, uint64_t bufsize)
{
	if (iov_count == 1) {
		uint64_t size = ((iov_offset > iov[0].iov_len) ?
				 0 : MIN(bufsize, iov[0].iov_len - iov_offset));
		cudaMemcpy((char *) iov[0].iov_base + iov_offset,
			   buf, size, cudaMemcpyHostToDevice);
		return size;
	} else {
		return ofi_copy_cuda_iov_buf(iov, iov_count, iov_offset, buf,
					     bufsize, OFI_COPY_BUF_TO_IOV);
	}
}

#endif /* HAVE_LIBCUDA */
#endif /* _OFI_CUDA_H_ */
