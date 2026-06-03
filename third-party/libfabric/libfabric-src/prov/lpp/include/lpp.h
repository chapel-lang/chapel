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

#ifndef _LPP_H_
#define _LPP_H_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <unistd.h>
#include <string.h>
#include <search.h>
#include <pthread.h>
#include <sys/ioctl.h>
#include <stdbool.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/uio.h>
#include <config.h>

#include <ofi.h>
#include <ofi_atom.h>
#include <ofi_atomic.h>
#include <ofi_mr.h>
#include <ofi_enosys.h>
#include <ofi_indexer.h>
#include <ofi_iov.h>
#include <ofi_rbuf.h>
#include <ofi_list.h>
#include <ofi_lock.h>
#include <ofi_file.h>
#include <ofi_osd.h>
#include <ofi_util.h>
#include <ofi_hmem.h>

#include "linux/klpp/klpp_shared.h"
#include "linux/klpp/klpp_ioctl.h"
#include "lpp_jhash.h"

#ifndef compiler_barrier
#define compiler_barrier() asm volatile ("" ::: "memory")
#endif

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
#endif

/* Bounds constants. */
#define LPP_MR_CNT (256)

#define LPP_CQ_MAX_COUNT (1024)
#define LPP_CQ_MAX_SIZE (65536)
#define LPP_CQ_DEFAULT_SIZE (65536)

#define LPP_EP_MAX_COUNT (256)
#define LPP_TX_CTX_MAX_COUNT (4096)
#define LPP_RX_CTX_MAX_COUNT (4096)
#define LPP_CNTR_MAX_COUNT (16 * 1024)
#define LPP_MIN_MULTI_RECV (64)
#define LPP_MAX_INJECT (64)

#if HAVE_CUDA || HAVE_ROCR
#define LPP_CAPS_HMEM FI_HMEM
#else
#define LPP_CAPS_HMEM 0
#endif

/*
 * Per fi_getinfo(3):
 *
 * Capabilities may be grouped into two general categories: primary and
 * secondary. Primary capabilities must explicitly be requested by an
 * application, and a provider must enable support for only those primary
 * capabilities which were selected. Secondary capabilities may optionally be
 * requested by an application. If requested, a provider must support the
 * capability or fail the fi_getinfo request (FI_ENODATA). A provider may
 * optionally report non-selected secondary capabilities if doing so would not
 * compromise performance or security.
 */
#define LPP_CAPS_PRIMARY		(FI_MSG |			\
					 FI_RMA |			\
					 FI_TAGGED |			\
					 FI_READ |			\
					 FI_WRITE |			\
					 FI_RECV |			\
					 FI_SEND |			\
					 FI_REMOTE_READ	|		\
					 FI_REMOTE_WRITE |		\
					 LPP_CAPS_HMEM |		\
					 FI_ATOMIC)

#define LPP_CAPS_RX_SECONDARY		(FI_MULTI_RECV |		\
					 FI_DIRECTED_RECV |		\
					 FI_REMOTE_COMM |		\
					 FI_LOCAL_COMM |		\
					 FI_SOURCE)

#define LPP_CAPS_TX_SECONDARY		(FI_REMOTE_COMM |		\
					 FI_LOCAL_COMM |		\
					 FI_SOURCE)


#define LPP_CAPS			(LPP_CAPS_PRIMARY |		\
					 LPP_CAPS_RX_SECONDARY |	\
					 LPP_CAPS_TX_SECONDARY)

#define LPP_DOMAIN_CAPS			(FI_LOCAL_COMM | FI_REMOTE_COMM)

/*
 * Most capability flags are also operational flags, but not all. This macro is
 * the list of those that are. We use this to check op flags against a
 * context's cap flags to make sure a given operation is permitted.
 */
#define LPP_CAPS_OPS			(FI_MSG |			\
					 FI_RMA |			\
					 FI_TAGGED |			\
					 FI_READ |			\
					 FI_WRITE |			\
					 FI_RECV |			\
					 FI_SEND |			\
					 FI_REMOTE_READ	|		\
					 FI_REMOTE_WRITE |		\
					 FI_ATOMIC)

#define	LPP_MODE			(0)

#define	LPP_ORDER			(FI_ORDER_SAS |			\
					 FI_ORDER_RAR |			\
					 FI_ORDER_RAW |			\
					 FI_ORDER_WAR |			\
					 FI_ORDER_WAW |			\
					 FI_ORDER_RMA_RAR |		\
					 FI_ORDER_RMA_RAW |		\
					 FI_ORDER_RMA_WAR |		\
					 FI_ORDER_RMA_WAW |		\
					 FI_ORDER_ATOMIC_RAR |		\
					 FI_ORDER_ATOMIC_RAW |		\
					 FI_ORDER_ATOMIC_WAR |		\
					 FI_ORDER_ATOMIC_WAW)

#define	LPP_OP_FLAGS			(FI_INJECT |			\
					 FI_MULTI_RECV |		\
					 FI_COMPLETION |		\
					 FI_TRANSMIT_COMPLETE |		\
					 FI_DELIVERY_COMPLETE)

#define	LPP_TAG_FORMAT			(FI_TAG_GENERIC)

/* Globals. */
extern struct fi_provider lpp_prov;
extern const struct fi_ops_mr   lpp_mr_ops;

/* Internal structs. */
struct lpp_fabric;
struct lpp_domain;
struct lpp_atomic;
struct lpp_av;
struct lpp_cntr;
struct lpp_cq;
struct lpp_endpoint;
struct lpp_ep;
struct lpp_eq;
struct lpp_mr;
struct lpp_rx;
struct lpp_stx;
struct lpp_ctx;
struct lpp_tx_entry;

#include "lpp_memcpy.h"
#include "lpp_atomic.h"
#include "lpp_cm.h"
#include "lpp_control.h"
#include "lpp_eq.h"
#include "lpp_getinfo.h"
#include "lpp_mr.h"
#include "lpp_msg.h"
#include "lpp_rma.h"
#include "lpp_tagged.h"
#include "lpp_ctx.h"
#include "lpp_stx.h"
#include "lpp_rx.h"
#include "lpp_hmem.h"
#include "lpp_domain.h"
#include "lpp_util.h"
#include "lpp_umc.h"
#include "lpp_ep.h"
#include "lpp_cntr.h"
#include "lpp_cq.h"
#include "lpp_av.h"
#include "lpp_fabric.h"
#include "lpp_completion.h"
#include "lpp_klpp.h"

#endif	/* _LPP_H_ */
