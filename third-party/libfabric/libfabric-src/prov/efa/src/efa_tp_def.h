/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */

#undef LTTNG_UST_TRACEPOINT_PROVIDER
#define LTTNG_UST_TRACEPOINT_PROVIDER EFA_TP_PROV

#undef LTTNG_UST_TRACEPOINT_INCLUDE
#define LTTNG_UST_TRACEPOINT_INCLUDE "efa_tp_def.h"

#if !defined(_EFA_TP_DEF_H) || defined(LTTNG_UST_TRACEPOINT_HEADER_MULTI_READ)
#define _EFA_TP_DEF_H

#include <lttng/tracepoint.h>

#define EFA_TP_PROV efa

/* Pre-defined tracepoints */

#define X_PKT_ARGS \
	size_t, wr_id, \
	size_t, efa_rdm_ope, \
	size_t, context

#define X_PKT_FIELDS \
	lttng_ust_field_integer_hex(size_t, wr_id, wr_id) \
	lttng_ust_field_integer_hex(size_t, efa_rdm_ope, efa_rdm_ope) \
	lttng_ust_field_integer_hex(size_t, context, context)

LTTNG_UST_TRACEPOINT_EVENT_CLASS(EFA_TP_PROV, post_wr_id,
	LTTNG_UST_TP_ARGS(X_PKT_ARGS),
	LTTNG_UST_TP_FIELDS(X_PKT_FIELDS))

LTTNG_UST_TRACEPOINT_EVENT_INSTANCE(EFA_TP_PROV, post_wr_id, EFA_TP_PROV,
	post_send,
	LTTNG_UST_TP_ARGS(X_PKT_ARGS))
LTTNG_UST_TRACEPOINT_LOGLEVEL(EFA_TP_PROV, post_send, LTTNG_UST_TRACEPOINT_LOGLEVEL_INFO)

LTTNG_UST_TRACEPOINT_EVENT_INSTANCE(EFA_TP_PROV, post_wr_id, EFA_TP_PROV,
	post_recv,
	LTTNG_UST_TP_ARGS(X_PKT_ARGS))
LTTNG_UST_TRACEPOINT_LOGLEVEL(EFA_TP_PROV, post_recv, LTTNG_UST_TRACEPOINT_LOGLEVEL_INFO)

#endif /* _EFA_TP_DEF_H */

#include <lttng/tracepoint-event.h>
