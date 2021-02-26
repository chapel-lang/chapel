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

#ifndef _EFA_CUDA_H_
#define _EFA_CUDA_H_

#include "efa.h"
#include "rxr.h"

#ifdef HAVE_LIBCUDA

#include <ofi_cuda.h>
#include <cuda.h>
#include <cuda_runtime.h>

static inline bool rxr_ep_is_cuda_mr(struct efa_mr *efa_mr)
{
	return efa_mr ? (efa_mr->peer.iface == FI_HMEM_CUDA): false;
}

#else

static inline bool rxr_ep_is_cuda_mr(struct efa_mr *efa_mr)
{
	return false;
}

#endif /* HAVE_LIBCUDA */

static inline
size_t rxr_copy_from_tx(void *buf, size_t tocopy,
			struct rxr_tx_entry *tx_entry, size_t offset)
{
	size_t data_size;

#ifdef HAVE_LIBCUDA
	if (rxr_ep_is_cuda_mr(tx_entry->desc[0]))
		data_size = ofi_copy_from_cuda_iov(buf,
						   tocopy,
						   tx_entry->iov,
						   tx_entry->iov_count,
						   offset);
       else
#endif
		data_size = ofi_copy_from_iov(buf,
					      tocopy,
					      tx_entry->iov,
					      tx_entry->iov_count,
					      offset);
	return data_size;
}

static inline
size_t rxr_copy_to_rx(void *data, size_t tocopy, struct rxr_rx_entry *rx_entry, size_t offset)
{
	size_t data_size;
#ifdef HAVE_LIBCUDA
	if (rxr_ep_is_cuda_mr(rx_entry->desc[0]))
		data_size = ofi_copy_to_cuda_iov(rx_entry->iov,
						 rx_entry->iov_count,
						 offset,
						 data,
						 tocopy);
	else
#endif
		data_size = ofi_copy_to_iov(rx_entry->iov,
					    rx_entry->iov_count,
					    offset,
					    data,
					    tocopy);

	return data_size;
}


#endif /* _EFA_CUDA_H_ */
