/*
 * Copyright (c) 2016-2018 Intel Corporation, Inc.  All rights reserved.
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

#ifndef FI_DIRECT_CM_H
#define FI_DIRECT_CM_H

#define FABRIC_DIRECT_CM

int psmx2_cm_getname(fid_t fid, void *addr, size_t *addrlen);


static inline int fi_setname(fid_t fid, void *addr, size_t addrlen)
{
	return -FI_ENOSYS;
}

static inline int fi_getname(fid_t fid, void *addr, size_t *addrlen)
{
	return psmx2_cm_getname(fid, addr, addrlen);
}

static inline int fi_getpeer(struct fid_ep *ep, void *addr, size_t *addrlen)
{
	return -FI_ENOSYS;
}

static inline int fi_listen(struct fid_pep *pep)
{
	return -FI_ENOSYS;
}

static inline int
fi_connect(struct fid_ep *ep, const void *addr,
	   const void *param, size_t paramlen)
{
	return -FI_ENOSYS;
}

static inline int
fi_accept(struct fid_ep *ep, const void *param, size_t paramlen)
{
	return -FI_ENOSYS;
}

static inline int
fi_reject(struct fid_pep *pep, fid_t handle,
	  const void *param, size_t paramlen)
{
	return -FI_ENOSYS;
}

static inline int fi_shutdown(struct fid_ep *ep, uint64_t flags)
{
	return -FI_ENOSYS;
}

static inline int fi_join(struct fid_ep *ep, const void *addr, uint64_t flags,
			  struct fid_mc **mc, void *context)
{
	return -FI_ENOSYS;
}

static inline fi_addr_t fi_mc_addr(struct fid_mc *mc)
{
	return -FI_ENOSYS;
}

#endif /* FI_DIRECT_CM_H */
