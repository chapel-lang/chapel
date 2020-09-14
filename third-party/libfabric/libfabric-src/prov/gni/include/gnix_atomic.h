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

#ifndef _GNIX_ATOMIC_H_
#define _GNIX_ATOMIC_H_

#define GNIX_ATOMIC_WRITE_FLAGS_DEF	(FI_ATOMIC | FI_WRITE)
#define GNIX_ATOMIC_READ_FLAGS_DEF	(FI_ATOMIC | FI_READ)

int _gnix_atomic_cmd(enum fi_datatype dt, enum fi_op op,
		     enum gnix_fab_req_type fr_type);
ssize_t _gnix_atomic(struct gnix_fid_ep *ep,
		     enum gnix_fab_req_type fr_type,
		     const struct fi_msg_atomic *msg,
		     const struct fi_ioc *comparev,
		     void **compare_desc,
		     size_t compare_count,
		     struct fi_ioc *resultv,
		     void **result_desc,
		     size_t result_count,
		     uint64_t flags);

/* SMSG callback for AMO remote counter control message. */
int __smsg_amo_cntr(void *data, void *msg);

#endif /* _GNIX_ATOMIC_H_ */

