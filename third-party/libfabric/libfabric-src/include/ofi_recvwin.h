/*
 * Copyright (c) 2018 Amazon.com, Inc. or its affiliates. All rights reserved.
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

/*
 * This utility provides a sliding receive window implementation. It uses a
 * circular queue to order incoming messages based on the message ID in the
 * transport protocol.
 */

#if !defined(FI_RECVWIN_H)
#define FI_RECVWIN_H

#include "config.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <ofi.h>
#include <ofi_rbuf.h>

#define OFI_DECL_RECVWIN_BUF(entrytype, name, id_type)			\
OFI_DECLARE_CIRQUE(entrytype, recvwin_cirq);				\
struct name {								\
	id_type exp_msg_id;						\
	id_type win_size;						\
	struct recvwin_cirq *pending;					\
};									\
									\
static inline struct name *						\
ofi_recvwin_buf_alloc(struct name *recvq, unsigned int size)		\
{									\
	assert(size == roundup_power_of_two(size));			\
	recvq->exp_msg_id = 0;						\
	recvq->win_size	= size;						\
	recvq->pending	= recvwin_cirq_create(recvq->win_size);		\
	return recvq;							\
}									\
									\
static inline void							\
ofi_recvwin_free(struct name *recvq)					\
{									\
	recvwin_cirq_free(recvq->pending);				\
}									\
									\
static inline int							\
ofi_recvwin_id_valid(struct name *recvq, id_type id)			\
{									\
	return ofi_recvwin_id_valid_ ## id_type (recvq, id);		\
}									\
									\
static inline int							\
ofi_recvwin_queue_msg(struct name *recvq, entrytype * msg, id_type id)	\
{									\
	size_t write_idx;						\
									\
	assert(ofi_recvwin_id_valid(recvq, id));			\
	write_idx = (ofi_cirque_rindex(recvq->pending)			\
		    + (id - recvq->exp_msg_id))				\
		    & recvq->pending->size_mask;			\
	recvq->pending->buf[write_idx] = *msg;				\
	ofi_cirque_commit(recvq->pending);				\
	return 0;							\
}									\
				                                        \
static inline entrytype *						\
ofi_recvwin_get_msg(struct name *recvq, id_type id)			\
{		                                           		\
	size_t read_idx;						\
									\
	assert(ofi_recvwin_id_valid(recvq, id));			\
	read_idx = (ofi_cirque_rindex(recvq->pending)			\
		    + (id - recvq->exp_msg_id))				\
		    & recvq->pending->size_mask;			\
	return &recvq->pending->buf[read_idx];				\
}									\
									\
static inline entrytype *						\
ofi_recvwin_get_next_msg(struct name *recvq)				\
{									\
	if (ofi_cirque_head(recvq->pending)) {				\
		ofi_recvwin_exp_inc(recvq);				\
		return ofi_cirque_remove(recvq->pending);		\
	} else {							\
		return NULL;						\
	}								\
}									\
									\
static inline void							\
ofi_recvwin_slide(struct name *recvq)					\
{									\
	ofi_recvwin_exp_inc(recvq);					\
	ofi_cirque_discard(recvq->pending);				\
	ofi_cirque_commit(recvq->pending);				\
}

#define ofi_recvwin_peek(rq)		(ofi_cirque_head(rq->pending))
#define ofi_recvwin_is_empty(rq)	(ofi_cirque_isempty(rq->pending))
#define ofi_recvwin_exp_inc(rq)		((rq)->exp_msg_id++)
#define ofi_recvwin_is_exp(rq, id)	((rq)->exp_msg_id == id)
#define ofi_recvwin_next_exp_id(rq)	((rq)->exp_msg_id)
/*
 * When exp_msg_id on the receiver has not wrapped around but the sender ID has
 * we need to allow the IDs starting from 0 that are valid. These macros use
 * the overflow of exp_msg_id to validate that.
 */
#define ofi_recvwin_id_valid_uint32_t(rq, id) \
	ofi_val32_inrange(rq->exp_msg_id, rq->win_size, id)
#define ofi_recvwin_id_valid_uint64_t(rq, id) \
	ofi_val64_inrange(rq->exp_msg_id, rq->win_size, id)

#endif /* FI_RECVWIN_H */
