/*
 * Copyright (c) 2018 Cray Inc.  All rights reserved.
 * Copyright (c) 2019 System Fabric Works, Inc. All rights reserved.
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

#ifndef OFI_VERBS_COMPAT_H
#define OFI_VERBS_COMPAT_H

#if !VERBS_HAVE_XRC
#define IBV_QPT_XRC_SEND 9ull
#define IBV_QPT_XRC_RECV 10ull

#define IBV_DEVICE_XRC (1 << 20)

#define IBV_SRQ_INIT_ATTR_TYPE 0ull
#define IBV_SRQ_INIT_ATTR_PD 1ull
#define IBV_SRQ_INIT_ATTR_XRCD 2ull
#define IBV_SRQ_INIT_ATTR_CQ 3ull

#define IBV_SRQT_XRC 1ull
#define VRB_SET_REMOTE_SRQN(var, val) do { } while (0)
#define FI_VERBS_XRC_ONLY __attribute__((unused))

#define ibv_get_srq_num(srq, srqn) do { } while (0)
#define ibv_create_srq_ex(context, attr) (NULL)
#else /* !VERBS_HAVE_XRC */

#define VRB_SET_REMOTE_SRQN(var, val) \
	do { \
		(var).qp_type.xrc.remote_srqn = (val); \
	} while (0)

#define FI_VERBS_XRC_ONLY
#endif /* VERBS_HAVE_XRC */

#if !VERBS_HAVE_RDMA_ESTABLISH
/* If older rdma-core this function does not exist/is not needed */
#define rdma_establish(id) do { } while (0)
#endif

#endif /* OFI_VERBS_COMPAT_H */
