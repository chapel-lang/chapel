/*
 * Copyright (c) 2015-2016, Cisco Systems, Inc. All rights reserved.
 * Copyright (c) 2015, Intel Corp., Inc. All rights reserved.
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
 *
 */

#ifndef FI_LOG_H
#define FI_LOG_H

#include <rdma/fabric.h>
#include <rdma/providers/fi_prov.h>

#ifdef __cplusplus
extern "C" {
#endif

enum fi_log_subsys {
	FI_LOG_CORE,
	FI_LOG_FABRIC,
	FI_LOG_DOMAIN,
	FI_LOG_EP_CTRL,
	FI_LOG_EP_DATA,
	FI_LOG_AV,
	FI_LOG_CQ,
	FI_LOG_EQ,
	FI_LOG_MR,
	FI_LOG_CNTR,
	FI_LOG_SUBSYS_MAX
};

enum fi_log_level {
	FI_LOG_WARN,
	FI_LOG_TRACE,
	FI_LOG_INFO,
	FI_LOG_DEBUG,
	FI_LOG_MAX
};

int fi_log_enabled(const struct fi_provider *prov, enum fi_log_level level,
		   enum fi_log_subsys subsys);
int fi_log_ready(const struct fi_provider *prov, enum fi_log_level level,
		 enum fi_log_subsys subsys, uint64_t *showtime);
void fi_log(const struct fi_provider *prov, enum fi_log_level level,
	    enum fi_log_subsys subsys, const char *func, int line,
	    const char *fmt, ...) FI_FORMAT_PRINTF(6, 7);

#define FI_LOG(prov, level, subsystem, ...)				\
	do {								\
		if (fi_log_enabled(prov, level, subsystem)) {		\
			int saved_errno = errno;			\
			fi_log(prov, level, subsystem,			\
				__func__, __LINE__, __VA_ARGS__);	\
			errno = saved_errno;				\
		}							\
	} while (0)

#define FI_LOG_SPARSE(prov, level, subsystem, ...)			\
	do {								\
		static uint64_t showtime;				\
		if (fi_log_ready(prov, level, subsystem, &showtime)) {	\
			int saved_errno = errno;			\
			fi_log(prov, level, subsystem,			\
				__func__, __LINE__, __VA_ARGS__);	\
			errno = saved_errno;				\
		}							\
	} while (0)

#define FI_WARN(prov, subsystem, ...)					\
	FI_LOG(prov, FI_LOG_WARN, subsystem, __VA_ARGS__)
#define FI_WARN_SPARSE(prov, subsystem, ...)				\
	FI_LOG_SPARSE(prov, FI_LOG_WARN, subsystem, __VA_ARGS__)

#define FI_TRACE(prov, subsystem, ...)					\
	FI_LOG(prov, FI_LOG_TRACE, subsystem, __VA_ARGS__)

#define FI_INFO(prov, subsystem, ...)					\
	FI_LOG(prov, FI_LOG_INFO, subsystem, __VA_ARGS__)

#if defined(ENABLE_DEBUG) && ENABLE_DEBUG
#define FI_DBG(prov, subsystem, ...)					\
	FI_LOG(prov, FI_LOG_DEBUG, subsystem, __VA_ARGS__)
#define FI_DBG_TRACE(prov, subsystem, ...)				\
	FI_LOG(prov, FI_LOG_TRACE, subsystem, __VA_ARGS__)
#else
#define FI_DBG(prov_name, subsystem, ...)				\
	do {} while (0)
#define FI_DBG_TRACE(prov, subsystem, ...)				\
	do {} while (0)
#endif

#define FI_WARN_ONCE(prov, subsystem, ...)  				\
	do {								\
		static int warned = 0;					\
		if (!warned &&						\
		    fi_log_enabled(prov, FI_LOG_WARN, subsystem)) {	\
			int saved_errno = errno;			\
			fi_log(prov, FI_LOG_WARN, subsystem,		\
			__func__, __LINE__, __VA_ARGS__);		\
			warned = 1;					\
			errno = saved_errno;				\
		}							\
	} while (0)

#ifdef __cplusplus
}
#endif

#endif /* FI_LOG_H */
