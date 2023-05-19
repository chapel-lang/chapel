/*
 * Copyright (C) 2016 by Argonne National Laboratory.
 * Copyright (C) 2021 Cornelis Networks.
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
#ifndef _FI_PROV_OPX_DIRECT_TAGGED_H_
#define _FI_PROV_OPX_DIRECT_TAGGED_H_

#define FABRIC_DIRECT_TAGGED 1

#ifdef FABRIC_DIRECT

#ifdef __cplusplus
extern "C" {
#endif

#define fi_tsend(ep, buf, len, desc, dest_addr, tag, context)		\
	(fi_opx_tsend_FABRIC_DIRECT(ep, buf, len, desc, dest_addr, tag, context))

#define fi_trecv(ep, buf, len, desc, src_addr, tag, ignore, context)	\
	(fi_opx_trecv_FABRIC_DIRECT(ep, buf, len, desc, src_addr, tag, ignore, context))

#define fi_tinject(ep, buf, len, dest_addr, tag)			\
	(fi_opx_tinject_FABRIC_DIRECT(ep, buf, len, dest_addr, tag))

#define fi_tsenddata(ep, buf, len, desc, data, dest_addr, tag, context)	\
	(fi_opx_tsenddata_FABRIC_DIRECT(ep, buf, len, desc, data, dest_addr, tag, context))

#define fi_tinjectdata(ep, buf, len, data, dest_addr, tag)		\
	(fi_opx_tinjectdata_FABRIC_DIRECT(ep, buf, len, data, dest_addr, tag))


static inline ssize_t
fi_trecvmsg(struct fid_ep *ep, const struct fi_msg_tagged *msg, uint64_t flags)
{
        return ep->tagged->recvmsg(ep, msg, flags);
}

static inline ssize_t
fi_tsendmsg(struct fid_ep *ep, const struct fi_msg_tagged *msg, uint64_t flags)
{
        return ep->tagged->sendmsg(ep, msg, flags);
}



ssize_t
fi_opx_tinject_FABRIC_DIRECT(struct fid_ep *ep, const void *buf, size_t len,
		fi_addr_t dest_addr, uint64_t tag);

ssize_t
fi_opx_tsend_FABRIC_DIRECT(struct fid_ep *ep, const void *buf, size_t len,
		void *desc, fi_addr_t dest_addr, uint64_t tag, void *context);

ssize_t
fi_opx_tinjectdata_FABRIC_DIRECT(struct fid_ep *ep, const void *buf, size_t len,
		uint64_t data, fi_addr_t dest_addr, uint64_t tag);

ssize_t
fi_opx_tsenddata_FABRIC_DIRECT(struct fid_ep *ep, const void *buf, size_t len,
		void *desc, uint64_t data, fi_addr_t dest_addr, uint64_t tag, void *context);

ssize_t
fi_opx_trecv_FABRIC_DIRECT(struct fid_ep *ep, const void *buf, size_t len,
		void *desc, fi_addr_t src_addr, uint64_t tag, uint64_t ignore, void *context);

#ifdef __cplusplus
}
#endif

#endif /* FABRIC_DIRECT */

#endif /* _FI_PROV_OPX_DIRECT_TAGGED_H_ */
