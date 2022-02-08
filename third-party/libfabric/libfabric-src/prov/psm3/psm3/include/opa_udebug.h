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

#ifndef OPA_UDEBUG_H
#define OPA_UDEBUG_H

#include <stdio.h>
#include "opa_debug.h"

// To have a a message be unconditionally output for all builds, regardless of
// env variables, use _HFI_ERROR or _HFI_UNIT_ERROR
// All other logging macros are under the control of the user via env variables
// and build options can disable them
//
// Other logging calls are only enabled if _HFI_DEBUGGING is defined,
// in which case _HFI_INFO is also enabled by default (but env can disable it).
// All others controlled by env variable.
//
// Currently opa_debug.h always defines _HFI_DEBUGGING and it is included by
// opa_udebug.h, so logging is presently enabled in all builds.  At some point
// may want to explore a performance optimization and disable logging macros
// for lower level debug messages in non-debug builds.
//
// See psmi_handle_error in psm_error.h.  Use of it's PSMI_EP_NO_RETURN option
// can unconditionally output a message and abort.

extern unsigned hfi_debug;
const char *hfi_get_unit_name(int unit);
extern char *__progname;

static const char hfi_ident_tag[] = "PSM3_IDENTIFY";
char *hfi_get_mylabel();
int hfi_get_myrank();	// -1 if unknown
int hfi_get_myrank_count();	// -1 if unknown
int hfi_get_mylocalrank();	// -1 if unknown
int hfi_get_mylocalrank_count();	// -1 if unknown

#if _HFI_DEBUGGING

extern char __hfi_mylabel[];
void hfi_set_mylabel(char *);
extern FILE *__hfi_dbgout;
extern void hfi_dump_buf(uint8_t *buf, uint32_t len);
#ifdef PSM_CUDA
extern void hfi_dump_gpu_buf(uint8_t *buf, uint32_t len);
#endif

#define _HFI_UNIT_ERROR(unit, fmt, ...) \
	do { \
		_Pragma_unlikely \
		printf("%s.%s: " fmt, __hfi_mylabel, __progname, \
		       ##__VA_ARGS__); \
	} while (0)

#define _HFI_ERROR(fmt, ...) \
	do { \
		_Pragma_unlikely \
		printf("%s.%s: " fmt, __hfi_mylabel, __progname, \
		       ##__VA_ARGS__); \
	} while (0)

#define _HFI_INFO(fmt, ...) \
	do { \
		_Pragma_unlikely \
		if (unlikely(hfi_debug&__HFI_INFO))  \
			printf("%s.%s: " fmt, __hfi_mylabel, __func__, \
			       ##__VA_ARGS__); \
	} while (0)

#define __HFI_PKTDBG_ON unlikely(hfi_debug & __HFI_PKTDBG)

#define __HFI_DBG_WHICH(which, fmt, ...) \
	do { \
		_Pragma_unlikely \
		if (unlikely(hfi_debug&(which))) \
			fprintf(__hfi_dbgout, "%s.%s: " fmt, __hfi_mylabel, __func__, \
			       ##__VA_ARGS__); \
	} while (0)

#define __HFI_DBG_WHICH_NOFUNC(which, fmt, ...) \
	do { \
		_Pragma_unlikely \
		if (unlikely(hfi_debug&(which))) \
			fprintf(__hfi_dbgout, "%s: " fmt, __hfi_mylabel, \
			       ##__VA_ARGS__); \
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
#define _HFI_DBG_ON unlikely(hfi_debug & __HFI_DBG)
#define _HFI_DBG_ALWAYS(fmt, ...) \
	do { \
		_Pragma_unlikely \
		fprintf(__hfi_dbgout, "%s: " fmt, __hfi_mylabel, \
			##__VA_ARGS__); \
	} while (0)

#define _HFI_CONNDBG_ON unlikely(hfi_debug & __HFI_CONNDBG)
#define _HFI_CONNDBG_ALWAYS(fmt, ...) _HFI_DBG_ALWAYS(fmt, ##__VA_ARGS__)

#define _HFI_VDBG_ON unlikely(hfi_debug & __HFI_VERBDBG)
#define _HFI_VDBG_ALWAYS(fmt, ...) _HFI_DBG_ALWAYS(fmt, ##__VA_ARGS__)

#define _HFI_PDBG_ON unlikely(hfi_debug & __HFI_PKTDBG)
#define _HFI_PDBG_ALWAYS(fmt, ...) _HFI_DBG_ALWAYS(fmt, ##__VA_ARGS__)
#define _HFI_PDBG_DUMP(buf, len) hfi_dump_buf(buf, len)
#ifdef PSM_CUDA
#define _HFI_PDBG_DUMP_GPU(buf, len) hfi_dump_gpu_buf(buf, len)
#endif

#define _HFI_PRDBG_ON unlikely(hfi_debug & __HFI_PROCDBG)
#define _HFI_PRDBG_ALWAYS(fmt, ...) _HFI_DBG_ALWAYS(fmt, ##__VA_ARGS__)

#define _HFI_CCADBG_ON unlikely(hfi_debug & __HFI_CCADBG)
#define _HFI_CCADBG_ALWAYS(fmt, ...) _HFI_DBG_ALWAYS(fmt, ##__VA_ARGS__)

#define _HFI_INFO_ON unlikely(hfi_debug & __HFI_INFO)
#define _HFI_INFO_ALWAYS(fmt, ...) _HFI_DBG_ALWAYS(fmt, ##__VA_ARGS__)

#else /* ! _HFI_DEBUGGING */

#define _HFI_UNIT_ERROR(unit, fmt, ...) \
	do { \
		printf("%s: " fmt, "", ##__VA_ARGS__); \
	} while (0)

#define _HFI_ERROR(fmt, ...) \
	do { \
		printf("%s: " fmt, "", ##__VA_ARGS__); \
	} while (0)

#define _HFI_INFO(fmt, ...)

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
#define _HFI_PDBG_DUMP(buf, len)
#ifdef PSM_CUDA
#define _HFI_PDBG_DUMP_GPU(buf, len)
#endif
#define _HFI_CCADBG_ON 0
#define _HFI_CCADBG_ALWAYS(fmt, ...)
#define _HFI_INFO_ON 0
#define _HFI_INFO_ALWAYS(fmt, ...)

#endif /* _HFI_DEBUGGING */

#endif /* OPA_UDEBUG_H */
