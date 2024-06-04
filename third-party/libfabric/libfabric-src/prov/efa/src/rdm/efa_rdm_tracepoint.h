/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */

#ifndef _EFA_RDM_TRACEPOINT_H
#define _EFA_RDM_TRACEPOINT_H

#include <config.h>

#if HAVE_LTTNG

#include "efa_rdm_tracepoint_def.h"

#include <lttng/tracef.h>
#include <lttng/tracelog.h>

#define efa_rdm_tracepoint(...) \
	lttng_ust_tracepoint(EFA_RDM_TP_PROV, __VA_ARGS__)

/*
 * Simple printf()-style tracepoints
 * Tracing events will be labeled `lttng_ust_tracef:*`
 */
#define efa_rdm_tracef	lttng_ust_tracef

/* tracelog() is similar to tracef(), but with a log level param */
#define efa_rdm_tracelog	lttng_ust_tracelog

#else

#define efa_rdm_tracepoint(...)	do {} while (0)
#define efa_rdm_tracef(...)	do {} while (0)
#define efa_rdm_tracelog(...)	do {} while (0)

#endif /* HAVE_LTTNG */

#endif /* _EFA_RDM_TRACEPOINT_H */
