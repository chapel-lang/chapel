/*
 * Copyright (c) 2018-2024 GigaIO, Inc. All Rights Reserved.
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

#ifndef _LPP_RMA_H_
#define _LPP_RMA_H_

ssize_t lpp_rma_common(struct fid_ep *ep, const struct iovec *local_iovp,
		       size_t local_iov_count,
		       const struct fi_rma_iov *remote_iovp,
		       size_t remote_iov_count,
		       fi_addr_t addr, void *context, uint64_t flags);

ssize_t lpp_fi_read(struct fid_ep *ep, void *buf, size_t len, void *desc,
		    fi_addr_t src_addr, uint64_t addr, uint64_t key,
		    void *context);
ssize_t lpp_fi_readmsg(struct fid_ep *ep, const struct fi_msg_rma *msg,
		       uint64_t flags);
ssize_t lpp_fi_write(struct fid_ep *ep, const void *buf, size_t len, void *desc,
		     fi_addr_t dest_addr, uint64_t addr, uint64_t key,
		     void *context);
ssize_t lpp_fi_writemsg(struct fid_ep *ep, const struct fi_msg_rma *msg,
			uint64_t flags);
ssize_t lpp_fi_inject_write(struct fid_ep *ep, const void *buf, size_t len,
			    fi_addr_t dest_addr, uint64_t addr, uint64_t key);

#endif // _LPP_RMA_H_
