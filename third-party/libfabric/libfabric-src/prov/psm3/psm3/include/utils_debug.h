/*

  This file is provided under a dual BSD/GPLv2 license.  When using or
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2015 Intel Corporation.

  This program is free software; you can redistribute it and/or modify
  it under the terms of version 2 of the GNU General Public License as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  Contact Information:
  Intel Corporation, www.intel.com

  BSD LICENSE

  Copyright(c) 2015 Intel Corporation.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in
      the documentation and/or other materials provided with the
      distribution.
    * Neither the name of Intel Corporation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

/* Copyright (c) 2003-2014 Intel Corporation. All rights reserved. */

#ifndef UTILS_DEBUG_H
#define UTILS_DEBUG_H

#include <stdio.h>
#include <time.h>

// See below for macros and comments about these settings

#ifndef _HFI_DEBUGGING		/* debugging enabled or not */
#define _HFI_DEBUGGING 1
#endif

#if _HFI_DEBUGGING

/*
 * Mask values for debugging.  The scheme allows us to compile out any
 * of the debug tracing stuff, and if compiled in, to enable or disable
 * dynamically.  This can be set at modprobe time also:
 *      modprobe hfi.ko psm3_dbgmask=7
 */

#define __HFI_INFO        0x1	/* generic low verbosity stuff */
#define __HFI_DBG         0x2	/* generic debug */
/* leave some low verbosity spots open */
/* Debug messages related to the connection protocol. */
#define __HFI_CONNDBG     0x20
#define __HFI_VERBDBG     0x40	/* very verbose debug */
#define __HFI_PKTDBG      0x80	/* print packet data */
/* print process startup (init)/exit messages and important env vars */
#define __HFI_PROCDBG     0x100
/* print MR, mmap/nopage stuff, not using VDBG any more */
#define __HFI_MMDBG       0x200
/* low-level environment variables */
#define __HFI_ENVDBG	    0x400

#define __HFI_DEBUG_DEFAULT __HFI_INFO
#define __HFI_DEBUG_DEFAULT_STR "0x0001"

#else /* _HFI_DEBUGGING */

/*
 * define all of these even with debugging off, for the few places that do
 * if(psm3_dbgmask & _HFI_xyzzy), but in a way that will make the
 * compiler eliminate the code
 */

#define __HFI_INFO      0x0	/* generic low verbosity stuff */
#define __HFI_DBG       0x0	/* generic debug */
#define __HFI_CONNDBG   0x0
#define __HFI_VERBDBG   0x0	/* very verbose debug */
#define __HFI_PKTDBG    0x0	/* print packet data */
#define __HFI_PROCDBG   0x0	/* print process startup (init)/exit messages */
/* print MR, mmap/nopage stuff, not using VDBG any more */
#define __HFI_MMDBG     0x0

#define __HFI_DEBUG_DEFAULT __HFI_INFO
#define __HFI_DEBUG_DEFAULT_STR "0x0000"

#endif /* _HFI_DEBUGGING */

#define __HFI_VERBOSEDBG __HFI_VERBDBG

// To have a a message be unconditionally output for all builds, regardless of
// env variables, use _HFI_ERROR or _HFI_UNIT_ERROR
// All other logging macros are under the control of the user via env variables
// and build options can disable them
//
// Other logging calls are only enabled if _HFI_DEBUGGING is defined,
// in which case _HFI_INFO is also enabled by default (but env can disable it).
// All others controlled by env variable.
//
// _HFI_DEBUGGING is always defined so logging is presently enabled in all
// builds.  At some point
// may want to explore a performance optimization and disable logging macros
// for lower level debug messages in non-debug builds.
//
// See psm3_handle_error in psm_error.h.  Use of it's PSMI_EP_NO_RETURN option
// can unconditionally output a message and abort.

extern unsigned psm3_dbgmask;
extern char *__progname;

static const char psm3_ident_tag[] = "PSM3_IDENTIFY";
char *psm3_get_mylabel();
int psm3_get_myrank();	// -1 if unknown
int psm3_get_myrank_count();	// -1 if unknown
int psm3_get_mylocalrank();	// -1 if unknown
int psm3_get_mylocalrank_count();	// -1 if unknown

#ifdef PSM_TS_USER
#define PSM3_TIME_FMT "[%s.%09lu] "
#define PSM3_TIME_ARG \
	tbuf, ts.tv_nsec
#define PSM3_GETTIME \
	struct timespec ts; \
	struct tm tm; \
	char tbuf[40]; \
		      \
	clock_gettime(CLOCK_REALTIME, &ts); \
	tzset(); \
	localtime_r(&ts.tv_sec, &tm); \
	strftime(tbuf, 40, "%F %T", &tm);
#else
#define PSM3_TIME_FMT "[%lu.%09lu] "
#define PSM3_TIME_ARG \
	ts.tv_sec, ts.tv_nsec
#define PSM3_GETTIME \
	struct timespec ts;	\
				\
	clock_gettime(CLOCK_REALTIME, &ts);

#endif

#if _HFI_DEBUGGING

extern char psm3_mylabel[];
void psm3_set_mylabel(char *);
extern FILE *psm3_dbgout;
extern void psm3_dump_buf(uint8_t *buf, uint32_t len);
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
extern void psm3_dump_gpu_buf(uint8_t *buf, uint32_t len);
#endif

#define _HFI_UNIT_ERROR(unit, fmt, ...) \
	do { \
		_Pragma_unlikely \
		PSM3_GETTIME \
		printf(PSM3_TIME_FMT "%s.%s: " fmt, PSM3_TIME_ARG, psm3_mylabel, __progname, \
		       ##__VA_ARGS__); \
	} while (0)

#define _HFI_ERROR(fmt, ...) \
	do { \
		_Pragma_unlikely \
		PSM3_GETTIME \
		printf(PSM3_TIME_FMT "%s.%s: " fmt, PSM3_TIME_ARG, psm3_mylabel, __progname, \
		       ##__VA_ARGS__); \
	} while (0)

#define _HFI_INFO(fmt, ...) \
	do { \
		_Pragma_unlikely \
		if (unlikely(psm3_dbgmask&__HFI_INFO)) {  \
			PSM3_GETTIME \
			printf(PSM3_TIME_FMT "%s.%s: " fmt, PSM3_TIME_ARG, psm3_mylabel, __func__, \
			       ##__VA_ARGS__); \
		} \
	} while (0)

#define _HFI_ENV_ERROR(fmt, ...) \
	do { \
		_Pragma_unlikely \
		if (unlikely(psm3_dbgmask&__HFI_INFO)) {  \
			printf("%s: env " fmt, psm3_mylabel, ##__VA_ARGS__); \
		} \
	} while (0)

#define __HFI_PKTDBG_ON unlikely(psm3_dbgmask & __HFI_PKTDBG)

#define __HFI_DBG_WHICH(which, fmt, ...) \
	do { \
		_Pragma_unlikely \
		if (unlikely(psm3_dbgmask&(which))) {\
			PSM3_GETTIME \
			fprintf(psm3_dbgout, PSM3_TIME_FMT "%s.%s: " fmt, PSM3_TIME_ARG, psm3_mylabel, \
				__func__, ##__VA_ARGS__); \
		} \
	} while (0)

#define __HFI_DBG_WHICH_NOFUNC(which, fmt, ...) \
	do { \
		_Pragma_unlikely \
		if (unlikely(psm3_dbgmask&(which))) { \
			fprintf(psm3_dbgout, "%s: " fmt, psm3_mylabel, \
			       ##__VA_ARGS__); \
		} \
	} while (0)

#define _HFI_DBG(fmt, ...) __HFI_DBG_WHICH(__HFI_DBG, fmt, ##__VA_ARGS__)
#define _HFI_CONNDBG(fmt, ...) __HFI_DBG_WHICH(__HFI_CONNDBG, fmt, ##__VA_ARGS__)
#define _HFI_VDBG(fmt, ...) __HFI_DBG_WHICH(__HFI_VERBDBG, fmt, ##__VA_ARGS__)
#define _HFI_PDBG(fmt, ...) __HFI_DBG_WHICH(__HFI_PKTDBG, fmt, ##__VA_ARGS__)
#define _HFI_PRDBG(fmt, ...) __HFI_DBG_WHICH(__HFI_PROCDBG, fmt, ##__VA_ARGS__)
#define _HFI_ENVDBG(lev, fmt, ...) \
	__HFI_DBG_WHICH_NOFUNC(					    \
		(lev == 0) ? __HFI_INFO : __HFI_ENVDBG,\
		"env " fmt, ##__VA_ARGS__)
#define _HFI_MMDBG(fmt, ...) __HFI_DBG_WHICH(__HFI_MMDBG, fmt, ##__VA_ARGS__)

/*
 * Use these macros (_HFI_DBG_ON and _HFI_DBG_ALWAYS) together
 * for a scope of code preparing debug info for printing; e.g.
 * if (_HFI_DBG_ON) {
 *     // put your code here
 *     _HFI_DBG_ALWAYS(print your results here);
 * }
 */
#define _HFI_DBG_ON unlikely(psm3_dbgmask & __HFI_DBG)
#define _HFI_DBG_ALWAYS(fmt, ...) \
	do { \
		_Pragma_unlikely \
		PSM3_GETTIME \
		fprintf(psm3_dbgout, PSM3_TIME_FMT "%s.%s: " fmt, PSM3_TIME_ARG, psm3_mylabel, __func__, \
			       ##__VA_ARGS__); \
	} while (0)

#define _HFI_CONNDBG_ON unlikely(psm3_dbgmask & __HFI_CONNDBG)
#define _HFI_CONNDBG_ALWAYS(fmt, ...) _HFI_DBG_ALWAYS(fmt, ##__VA_ARGS__)

#define _HFI_VDBG_ON unlikely(psm3_dbgmask & __HFI_VERBDBG)
#define _HFI_VDBG_ALWAYS(fmt, ...) _HFI_DBG_ALWAYS(fmt, ##__VA_ARGS__)

#define _HFI_PDBG_ON unlikely(psm3_dbgmask & __HFI_PKTDBG)
#define _HFI_PDBG_ALWAYS(fmt, ...) _HFI_DBG_ALWAYS(fmt, ##__VA_ARGS__)
#define _HFI_PDBG_DUMP_ALWAYS(buf, len) psm3_dump_buf(buf, len)
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
#define _HFI_PDBG_DUMP_GPU_ALWAYS(buf, len) psm3_dump_gpu_buf(buf, len)
#endif

#define _HFI_PRDBG_ON unlikely(psm3_dbgmask & __HFI_PROCDBG)
#define _HFI_PRDBG_ALWAYS(fmt, ...) _HFI_DBG_ALWAYS(fmt, ##__VA_ARGS__)

#define _HFI_MMDBG_ON unlikely(psm3_dbgmask & __HFI_MMDBG)
#define _HFI_MMDBG_ALWAYS(fmt, ...) _HFI_DBG_ALWAYS(fmt, ##__VA_ARGS__)


#define _HFI_INFO_ON unlikely(psm3_dbgmask & __HFI_INFO)
#define _HFI_INFO_ALWAYS(fmt, ...) _HFI_DBG_ALWAYS(fmt, ##__VA_ARGS__)

#else /* ! _HFI_DEBUGGING */

#define _HFI_UNIT_ERROR(unit, fmt, ...) \
	do { \
		PSM3_GETTIME \
		printf(PSM3_TIME_FMT "%s: " fmt, PSM3_TIME_ARG, "", ##__VA_ARGS__); \
	} while (0)

#define _HFI_ERROR(fmt, ...) \
	do { \
		PSM3_GETTIME \
		printf(PSM3_TIME_FMT "%s: " fmt, PSM3_TIME_ARG, "", ##__VA_ARGS__); \
	} while (0)

#define _HFI_INFO(fmt, ...)

#define _HFI_ENV_ERROR(fmt, ...)

#define __HFI_PKTDBG_ON 0

#define _HFI_DBG(fmt, ...)
#define _HFI_PDBG(fmt, ...)
#define _HFI_PRDBG(fmt, ...)
#define _HFI_ENVDBG(lev, fmt, ...)
#define _HFI_CONNDBG(fmt, ...)
#define _HFI_VDBG(fmt, ...)
#define _HFI_MMDBG(fmt, ...)

#define _HFI_DBG_ON 0
#define _HFI_DBG_ALWAYS(fmt, ...)
#define _HFI_CONNDBG_ON 0
#define _HFI_CONNDBG_ALWAYS(fmt, ...)
#define _HFI_VDBG_ON 0
#define _HFI_VDBG_ALWAYS(fmt, ...)
#define _HFI_PRDBG_ON 0
#define _HFI_PRDBG_ALWAYS(fmt, ...)
#define _HFI_MMDBG_ON 0
#define _HFI_MMDBG_ALWAYS(fmt, ...)
#define _HFI_PDBG_DUMP_ALWAYS(buf, len)
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
#define _HFI_PDBG_DUMP_GPU_ALWAYS(buf, len)
#endif
#define _HFI_INFO_ON 0
#define _HFI_INFO_ALWAYS(fmt, ...)

#endif /* _HFI_DEBUGGING */

#endif /* UTILS_DEBUG_H */
