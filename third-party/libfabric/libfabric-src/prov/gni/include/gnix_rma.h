/*
 * Copyright (c) 2015-2016 Cray Inc.  All rights reserved.
 * Copyright (c) 2015 Los Alamos National Security, LLC. All rights reserved.
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

#ifndef _GNIX_RMA_H_
#define _GNIX_RMA_H_

#define GNIX_RMA_READ_FLAGS_DEF  (FI_RMA | FI_READ)
#define GNIX_RMA_WRITE_FLAGS_DEF (FI_RMA | FI_WRITE)

ssize_t _gnix_rma(struct gnix_fid_ep *ep, enum gnix_fab_req_type fr_type,
		  uint64_t loc_addr, size_t len, void *mdesc,
		  uint64_t dest_addr, uint64_t rem_addr, uint64_t mkey,
		  void *context, uint64_t flags, uint64_t data);

/**
 * @brief try to deliver an IRQ to peer
 *
 * This routine can be used to deliver an IRQ to the remote peer
 * via a GNI_PostCqWrite.
 *
 * @param[in] vc       pointer to previously allocated gnix_vc struct which
 *                     is in connected state
 * @return FI_SUCCESS  GNI_PostCqWrite successfully posted.
 * @return -FI_INVALID vc in invalid state or incorrect memory handle used
 * @return -FI_ENOSPC  no free tx descriptors
 */
int _gnix_rma_post_irq(struct gnix_vc *vc);

/* SMSG callback for RMA data control message. */
int __smsg_rma_data(void *data, void *msg);

#endif /* _GNIX_RMA_H_ */

