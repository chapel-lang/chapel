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
#ifndef _FI_PROV_OPX_TAGGED_H_
#define _FI_PROV_OPX_TAGGED_H_


/* Macro indirection in order to support other macros as arguments
 * C requires another indirection for expanding macros since
 * operands of the token pasting operator are not expanded */

#define FI_OPX_TAGGED_SPECIALIZED_FUNC(LOCK,AV,CAPS,RELIABILITY)		\
	FI_OPX_TAGGED_SPECIALIZED_FUNC_(LOCK,AV,CAPS,RELIABILITY)

#define FI_OPX_TAGGED_SPECIALIZED_FUNC_(LOCK,AV,CAPS,RELIABILITY)		\
    __OPX_FORCE_INLINE__ ssize_t                      \
	fi_opx_tsend_ ## LOCK	## _ ## AV ## _ ## CAPS ## _ ## RELIABILITY	\
		(struct fid_ep *ep, const void *buf, size_t len,		\
			void *desc, fi_addr_t dest_addr,			\
			uint64_t tag, void *context)				\
	{									\
		return fi_opx_ep_tx_send(ep, buf, len, desc,			\
				dest_addr, tag, context, 0,			\
				LOCK,	/* lock_required */			\
				AV,	/* av_type */				\
				1,	/* is_contiguous */			\
				0,	/* override_flags */			\
				0,	/* flags */				\
				CAPS | FI_TAGGED,				\
				RELIABILITY);					\
	}									\
	__OPX_FORCE_INLINE__ ssize_t                          \
	fi_opx_trecv_ ## LOCK	## _ ## AV ## _ ## CAPS ## _ ## RELIABILITY	\
		(struct fid_ep *ep, void *buf, size_t len,			\
			void *desc, fi_addr_t src_addr, uint64_t tag,		\
			uint64_t ignore, void *context)				\
	{									\
		return fi_opx_recv_generic(ep, buf, len, desc,		\
				src_addr, tag, ignore, context,			\
				LOCK, AV, FI_TAGGED, RELIABILITY);		\
	}									\
	__OPX_FORCE_INLINE__ ssize_t                          \
	fi_opx_tinject_ ## LOCK ## _ ## AV ## _ ## CAPS ## _ ## RELIABILITY	\
		(struct fid_ep *ep, const void *buf, size_t len,		\
			fi_addr_t dest_addr, uint64_t tag)			\
	{									\
		return fi_opx_ep_tx_inject(ep, buf, len,			\
				dest_addr, tag, 0,				\
				LOCK,	/* lock_required */			\
				AV,	/* av_type */				\
				CAPS | FI_TAGGED,				\
				RELIABILITY);					\
	}									\
	__OPX_FORCE_INLINE__ ssize_t                          \
	fi_opx_tsenddata_ ## LOCK ## _ ## AV ## _ ## CAPS ## _ ## RELIABILITY	\
		(struct fid_ep *ep, const void *buf, size_t len,		\
			void *desc, uint64_t data, fi_addr_t dest_addr,		\
			uint64_t tag, void *context)				\
	{									\
		return fi_opx_ep_tx_send(ep, buf, len, desc,			\
				dest_addr, tag, context, data,			\
				LOCK,	/* lock_required */			\
				AV,	/* av_type */				\
				1,	/* is_contiguous */			\
				0,	/* override_flags */			\
				0,	/* flags */				\
				CAPS | FI_TAGGED,				\
				RELIABILITY);					\
	}									\
	__OPX_FORCE_INLINE__ ssize_t                          \
	fi_opx_tinjectdata_ ## LOCK ## _ ## AV ## _ ## CAPS ## _ ## RELIABILITY	\
		(struct fid_ep *ep, const void *buf, size_t len,		\
			uint64_t data, fi_addr_t dest_addr,			\
			uint64_t tag)						\
	{									\
		return fi_opx_ep_tx_inject(ep, buf, len,			\
				dest_addr, tag, data,				\
				LOCK,	/* lock_required */			\
				AV,	/* av_type */				\
				CAPS | FI_TAGGED,				\
				RELIABILITY);					\
	}

#define FI_OPX_TAGGED_SPECIALIZED_FUNC_NAME(TYPE, LOCK, AV, CAPS, RELIABILITY)	\
	FI_OPX_TAGGED_SPECIALIZED_FUNC_NAME_(TYPE, LOCK, AV, CAPS, RELIABILITY)

#define FI_OPX_TAGGED_SPECIALIZED_FUNC_NAME_(TYPE, LOCK, AV, CAPS, RELIABILITY)	\
		fi_opx_ ## TYPE ## _ ## LOCK ## _ ## AV ## _ ## CAPS ## _ ## RELIABILITY

#endif /* _FI_PROV_OPX_TAGGED_H_ */
