/*
 * Copyright (C) 2021-2023 by Cornelis Networks.
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

#ifndef _FI_PROV_OPX_OPS_TABLE_H_
#define _FI_PROV_OPX_OPS_TABLE_H_

#include <ofi_enosys.h>
#include "rdma/fi_eq.h"
#include "rdma/opx/fi_opx_eq.h"

#define FI_OPX_COMMS_NONE 0x0000000000000000ull
#define FI_OPX_COMMS_LOCAL 0x0008000000000000ull
#define FI_OPX_COMMS_REMOTE 0x0010000000000000ull
#define FI_OPX_COMMS_LOCAL_REMOTE 0x0018000000000000ull
#define FI_OPX_COMMS_COUNT 4

/* Number of types in enum fi_cq_format */
#define FI_CQ_FORMAT_COUNT 5

typedef struct fi_ops_cq op_matrix_t[FI_CQ_FORMAT_COUNT][OFI_RELIABILITY_KIND_COUNT][FI_OPX_COMMS_COUNT];

static ssize_t
fi_opx_cq_readerr(struct fid_cq *cq, struct fi_cq_err_entry *buf, uint64_t flags)
{
	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_CQ, "(begin)\n");

	struct fi_opx_cq *opx_cq = container_of(cq, struct fi_opx_cq, cq_fid);

	if (IS_PROGRESS_MANUAL(opx_cq->domain)) {

		struct fi_opx_context_ext * ext =
			 (struct fi_opx_context_ext *) opx_cq->err.head;

		if ((ext == NULL) || (ext->opx_context.byte_counter != 0)) {
			/* perhaps an in-progress truncated rendezvous receive? */
			errno = FI_EAGAIN;
			return -errno;
		}

		assert(ext->opx_context.flags & FI_OPX_CQ_CONTEXT_EXT);	/* DEBUG */

		const enum fi_threading threading = opx_cq->domain->threading;
		const int lock_required = fi_opx_threading_lock_required(threading, fi_opx_global.progress);

		fi_opx_lock_if_required(&opx_cq->lock, lock_required);
		ofi_cq_err_memcpy(opx_cq->domain->fabric->fabric_fid.api_version,
				  buf, &ext->err_entry);
		slist_remove_head((struct slist *)&opx_cq->err);
		OPX_BUF_FREE(ext);
		ext = NULL;
		fi_opx_unlock_if_required(&opx_cq->lock, lock_required);

	} else {
		FI_WARN(fi_opx_global.prov, FI_LOG_CQ, "unimplemented\n");
		abort();
	}

	FI_DBG_TRACE(fi_opx_global.prov, FI_LOG_CQ, "(end)\n");
	return 1;
}

static ssize_t
fi_opx_cq_sread(struct fid_cq *cq, void *buf, size_t len, const void *cond, int timeout)
{
	FI_WARN(fi_opx_global.prov, FI_LOG_CQ, "unimplemented\n");
	abort();

	errno = FI_EAGAIN;
	return -errno;
}

static ssize_t
fi_opx_cq_sreadfrom(struct fid_cq *cq, void *buf, size_t len,
		   fi_addr_t *src_addr, const void *cond, int timeout)
{
	FI_WARN(fi_opx_global.prov, FI_LOG_CQ, "unimplemented\n");
	abort();

	errno = FI_EAGAIN;
	return -errno;
}

static const char *
fi_opx_cq_strerror(struct fid_cq *cq, int prov_errno, const void *err_data,
	       char *buf, size_t len)
{
	FI_WARN(fi_opx_global.prov, FI_LOG_CQ, "unimplemented\n");
	errno = FI_ENOSYS;
	return NULL;
}

#define FI_OPX_CQ_OPS_STRUCT_NAME(FORMAT, LOCK, RELIABILITY, MASK, CAPS)			\
  fi_opx_ops_cq_ ## FORMAT ## _ ## LOCK ## _ ## RELIABILITY ## _ ## MASK ## _ ## CAPS			\

#define FI_OPX_CQ_OPS_STRUCT_INIT(FORMAT, LOCK, RELIABILITY, MASK, CAPS)			\
  {										\
    .size    = sizeof(struct fi_ops_cq),					\
    .read      = FI_OPX_CQ_SPECIALIZED_FUNC_NAME(cq_read, FORMAT, LOCK, RELIABILITY, MASK, CAPS),		\
    .readfrom  = FI_OPX_CQ_SPECIALIZED_FUNC_NAME(cq_readfrom, FORMAT, LOCK, RELIABILITY, MASK, CAPS),	\
    .readerr   = fi_opx_cq_readerr,						\
    .sread     = fi_opx_cq_sread,						\
    .sreadfrom = fi_opx_cq_sreadfrom,						\
    .signal    = fi_no_cq_signal,						\
    .strerror  = fi_opx_cq_strerror,						\
  }

#define FI_OPX_CQ_OPS_STRUCT(FORMAT, LOCK, RELIABILITY, MASK, CAPS)				\
static struct fi_ops_cq								\
	FI_OPX_CQ_OPS_STRUCT_NAME(FORMAT, LOCK, RELIABILITY, MASK, CAPS) = 			\
		FI_OPX_CQ_OPS_STRUCT_INIT(FORMAT, LOCK, RELIABILITY, MASK, CAPS)

#endif
