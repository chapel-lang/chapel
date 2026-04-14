/*
 * Copyright (C) 2024 Cornelis Networks.
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
#ifndef _OPX_TRACER_H_
#define _OPX_TRACER_H_

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "rdma/providers/fi_log.h"
#include "rdma/opx/fi_opx_compiler.h"

#include <rdma/fi_errno.h>
#include <rdma/fi_ext.h>

#include "ofi.h"
#include "ofi_str.h"
#include "ofi_enosys.h"
#include "ofi_util.h"

struct opx_tracer_info {
	FILE *logptr;
	pid_t pid;
};

extern struct opx_tracer_info opx_tracer;

enum opx_tracer_status {
	OPX_TRACER_BEGIN,
	OPX_TRACER_INSTANT,
	OPX_TRACER_END_SUCCESS,
	OPX_TRACER_END_EAGAIN,
	OPX_TRACER_END_EAGAIN_SDMA_QUEUE_FULL,
	OPX_TRACER_END_EAGAIN_SDMA_NO_WE,
	OPX_TRACER_END_EAGAIN_SDMA_PSNS,
	OPX_TRACER_END_EAGAIN_SDMA_PSNS_THROTTLE,
	OPX_TRACER_END_EAGAIN_SDMA_PSNS_MAX_NACKS,
	OPX_TRACER_END_EAGAIN_SDMA_PSNS_MAX_OUT,
	OPX_TRACER_END_EAGAIN_SDMA_REPLAY_BUFFER,
	OPX_TRACER_END_ACK_IGNORED,
	OPX_TRACER_END_ENOBUFS,
	OPX_TRACER_END_ERROR
};

static const char *const OPX_TRACER_STATUS_STR[] = {
	[OPX_TRACER_INSTANT]			    = "INSTANT",
	[OPX_TRACER_BEGIN]			    = "BEGIN",
	[OPX_TRACER_END_SUCCESS]		    = "END_SUCCESS",
	[OPX_TRACER_END_EAGAIN]			    = "END_EAGAIN",
	[OPX_TRACER_END_EAGAIN_SDMA_QUEUE_FULL]	    = "OPX_TRACER_END_EAGAIN_SDMA_QUEUE_FULL",
	[OPX_TRACER_END_EAGAIN_SDMA_NO_WE]	    = "OPX_TRACER_END_EAGAIN_SDMA_NO_WE",
	[OPX_TRACER_END_EAGAIN_SDMA_PSNS]	    = "OPX_TRACER_END_EAGAIN_SDMA_PSNS",
	[OPX_TRACER_END_EAGAIN_SDMA_PSNS_THROTTLE]  = "OPX_TRACER_END_EAGAIN_SDMA_PSNS_THROTTLE",
	[OPX_TRACER_END_EAGAIN_SDMA_PSNS_MAX_NACKS] = "OPX_TRACER_END_EAGAIN_SDMA_PSNS_MAX_NACKS",
	[OPX_TRACER_END_EAGAIN_SDMA_PSNS_MAX_OUT]   = "OPX_TRACER_END_EAGAIN_SDMA_PSNS_MAX_OUT",
	[OPX_TRACER_END_EAGAIN_SDMA_REPLAY_BUFFER]  = "OPX_TRACER_END_EAGAIN_SDMA_REPLAY_BUFFER",
	[OPX_TRACER_END_ACK_IGNORED]		    = "OPX_TRACER_END_ACK_IGNORED",
	[OPX_TRACER_END_ENOBUFS]		    = "END_ENOBUFS",
	[OPX_TRACER_END_ERROR]			    = "END_ERROR"};

__OPX_FORCE_INLINE__
void opx_tracer_init()
{
	opx_tracer.pid = getpid();

	char *env = NULL;
	if (FI_SUCCESS == fi_param_get_str(NULL, "opx_tracer_out_path", &env)) {
		char buf[0x100];
		snprintf(buf, sizeof(buf), "%s/pid%d.log", env, opx_tracer.pid);
		opx_tracer.logptr = fopen(buf, "a");
	}
}

__OPX_FORCE_INLINE__
void opx_tracer_exit()
{
	if (opx_tracer.logptr) {
		fclose(opx_tracer.logptr);
	}
}

__OPX_FORCE_INLINE__
int opx_tracer_enabled()
{
	return opx_tracer.logptr ? 1 : 0;
}

__OPX_FORCE_INLINE__
void opx_tracer_trace(enum opx_tracer_status status, const char *func, int line, const char *msg)
{
	struct timespec ts;
	if (clock_gettime(CLOCK_REALTIME, &ts) == -1) {
		perror("clock gettime");
		exit(EXIT_FAILURE);
	}
	long unsigned timestamp = ts.tv_sec * 1000000000ul + ts.tv_nsec;

	fprintf(opx_tracer.logptr, "%ld:%d:%s():%d:%s:%s:\n", timestamp, opx_tracer.pid, func, line,
		OPX_TRACER_STATUS_STR[status], msg);
}

#if defined(OPX_TRACER) || defined(OPX_TRACER_SDMA) || defined(OPX_TRACER_RELI) || defined(OPX_TRACER_LOCK_IF_REQUIRED)

#define OPX_TRACER_INIT() opx_tracer_init()

#define OPX_TRACER_TRACE(status, fmt, ...)                                 \
	do {                                                               \
		if (opx_tracer_enabled()) {                                \
			int  saved_errno = errno;                          \
			char msg[1024];                                    \
			snprintf(msg, sizeof(msg), fmt, ##__VA_ARGS__);    \
			opx_tracer_trace(status, __func__, __LINE__, msg); \
			errno = saved_errno;                               \
		}                                                          \
	} while (0)

#define OPX_TRACER_EXIT() opx_tracer_exit()

#else
#define OPX_TRACER_INIT()
#define OPX_TRACER_TRACE(status, ...)
#define OPX_TRACER_EXIT()
#endif

#if defined(OPX_TRACER_SDMA)

#define OPX_TRACER_TRACE_SDMA(status, fmt, ...)                            \
	do {                                                               \
		if (opx_tracer_enabled()) {                                \
			int  saved_errno = errno;                          \
			char msg[1024];                                    \
			snprintf(msg, sizeof(msg), fmt, ##__VA_ARGS__);    \
			opx_tracer_trace(status, __func__, __LINE__, msg); \
			errno = saved_errno;                               \
		}                                                          \
	} while (0)

#else
#define OPX_TRACER_TRACE_SDMA(status, ...)
#endif

#if defined(OPX_TRACER_RELI)

#define OPX_TRACER_TRACE_RELI(status, fmt, ...)                            \
	do {                                                               \
		if (opx_tracer_enabled()) {                                \
			int  saved_errno = errno;                          \
			char msg[1024];                                    \
			snprintf(msg, sizeof(msg), fmt, ##__VA_ARGS__);    \
			opx_tracer_trace(status, __func__, __LINE__, msg); \
			errno = saved_errno;                               \
		}                                                          \
	} while (0)

#else
#define OPX_TRACER_TRACE_RELI(status, ...)
#endif

#if defined(OPX_TRACER_LOCK_IF_REQUIRED)

#define OPX_TRACER_TRACE_LOCK_IF_REQUIRED(status, fmt, ...)                \
	do {                                                               \
		if (opx_tracer_enabled()) {                                \
			int  saved_errno = errno;                          \
			char msg[1024];                                    \
			snprintf(msg, sizeof(msg), fmt, ##__VA_ARGS__);    \
			opx_tracer_trace(status, __func__, __LINE__, msg); \
			errno = saved_errno;                               \
		}                                                          \
	} while (0)

#else
#define OPX_TRACER_TRACE_LOCK_IF_REQUIRED(status, ...)
#endif

#endif
