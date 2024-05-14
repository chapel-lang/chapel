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

#ifndef OFI_CMA_H
#define OFI_CMA_H

#if HAVE_CONFIG_H
#  include <config.h>
#endif /* HAVE_CONFIG_H */

#include <ofi.h>
#include <ofi_iov.h>

static inline int cma_copy(struct iovec *local, unsigned long local_cnt,
			   struct iovec *remote, unsigned long remote_cnt,
			   size_t total, pid_t pid, bool write,
			   void *user_data)
{
	ssize_t ret;

	while (1) {
		if (write)
			ret = ofi_process_vm_writev(pid, local, local_cnt, remote,
						    remote_cnt, 0);
		else
			ret = ofi_process_vm_readv(pid, local, local_cnt, remote,
						   remote_cnt, 0);
		if (ret < 0) {
			FI_WARN(&core_prov, FI_LOG_CORE,
				"CMA error %d\n", errno);
			return -FI_EIO;
		}

		total -= ret;
		if (!total)
			return FI_SUCCESS;

		ofi_consume_iov(local, &local_cnt, (size_t) ret);
		ofi_consume_iov(remote, &remote_cnt, (size_t) ret);
	}
}

#endif /* OFI_CMA_H */
