/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */

#ifndef _EFA_TP_H
#define _EFA_TP_H

#include <config.h>

#if HAVE_LTTNG

#include "efa_tp_def.h"
#include "rdm/efa_rdm_ope.h"

#include <lttng/tracef.h>
#include <lttng/tracelog.h>

#define efa_tracepoint(...)	lttng_ust_tracepoint(EFA_TP_PROV, __VA_ARGS__)

/*
 * Simple printf()-style tracepoints
 * Tracing events will be labeled `lttng_ust_tracef:*`
 */
#define efa_tracef	lttng_ust_tracef

/* tracelog() is similar to tracef(), but with a log level param */
#define efa_tracelog	lttng_ust_tracelog

static inline void efa_tracepoint_wr_id_post_send(const void *wr_id)
{
	struct efa_rdm_pke *pkt_entry = (struct efa_rdm_pke *) wr_id;
	struct efa_rdm_ope *ope = pkt_entry->ope;
	if (!ope)
		return;
	efa_tracepoint(post_send, (size_t) wr_id, (size_t) ope, (size_t) ope->cq_entry.op_context);
}

static inline void efa_tracepoint_wr_id_post_recv(const void *wr_id)
{
	struct efa_rdm_pke *pkt_entry = (struct efa_rdm_pke *) wr_id;
	struct efa_rdm_ope *ope = pkt_entry->ope;
	if (!ope)
		return;
	efa_tracepoint(post_recv, (size_t) wr_id, (size_t) ope, (size_t) ope->cq_entry.op_context);
}

#else

#define efa_tracepoint(...)	do {} while(0)
#define efa_tracef(...)	do {} while(0)
#define efa_tracelog(...)	do {} while(0)

#endif /* HAVE_LTTNG */

#endif /* _EFA_TP_H */
