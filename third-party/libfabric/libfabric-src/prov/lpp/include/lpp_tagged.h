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

#ifndef _LPP_TAGGED_H_
#define _LPP_TAGGED_H_

ssize_t lpp_fi_trecv(struct fid_ep *ep, void *buf, size_t len, void *desc,
		     fi_addr_t src_addr, uint64_t tag, uint64_t ignore, void *context);

ssize_t lpp_fi_trecvmsg(struct fid_ep *ep, const struct fi_msg_tagged *msg,
			uint64_t flags);

ssize_t lpp_fi_tsend(struct fid_ep *ep, const void *buf, size_t len,
		     void *desc, fi_addr_t dest_addr, uint64_t tag, void *context);

ssize_t lpp_fi_tsendmsg(struct fid_ep *ep, const struct fi_msg_tagged *msg,
			uint64_t flags);

ssize_t lpp_fi_tinject(struct fid_ep *ep, const void *buf, size_t len,
		       fi_addr_t dest_addr, uint64_t tag);

ssize_t lpp_fi_tsenddata(struct fid_ep *ep, const void *buf, size_t len,
			 void *desc, uint64_t data, fi_addr_t dest_addr,
			 uint64_t tag, void *context);

ssize_t lpp_fi_tinjectdata(struct fid_ep *ep, const void *buf, size_t len,
			   uint64_t data, fi_addr_t dest_addr, uint64_t tag);

#endif // _LPP_TAGGED_H_
