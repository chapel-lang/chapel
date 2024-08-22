/*

  This file is provided under a dual BSD/GPLv2 license.  When using or
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2015 Intel Corporation.
  Copyright(c) 2021-2023 Cornelis Networks.

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
  Copyright(c) 2021-2023 Cornelis Networks.

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
/* Copyright (C) 2021-2023 by Cornelis Networks.                    */

#ifndef OPA_UDEBUG_H
#define OPA_UDEBUG_H

#include <stdio.h>
#include "rdma/providers/fi_log.h"
#include "rdma/opx/fi_opx.h"

#define _HFI_UNIT_ERROR(unit, fmt, ...) \
	do { if (fi_opx_global.prov) { \
		FI_WARN(fi_opx_global.prov, FI_LOG_FABRIC, fmt, ##__VA_ARGS__); \
	} else { \
		fprintf(stderr, fmt, ##__VA_ARGS__); \
	} } while (0)

#define _HFI_ERROR(fmt, ...) \
	do { if (fi_opx_global.prov) { \
		FI_WARN(fi_opx_global.prov, FI_LOG_FABRIC, fmt, ##__VA_ARGS__); \
	} else { \
		fprintf(stderr, fmt, ##__VA_ARGS__); \
	} } while (0)

#define _HFI_INFO(fmt, ...) \
	do { if (fi_opx_global.prov) { \
		FI_INFO(fi_opx_global.prov, FI_LOG_FABRIC, fmt, ##__VA_ARGS__); \
	} } while (0)

#define _HFI_DBG(fmt, ...) \
	do { if (fi_opx_global.prov) { \
		FI_WARN(fi_opx_global.prov, FI_LOG_FABRIC, fmt, ##__VA_ARGS__); \
	} } while (0)

#define _HFI_PDBG(fmt, ...) \
	do { if (fi_opx_global.prov) { \
		FI_DBG(fi_opx_global.prov, FI_LOG_FABRIC, fmt, ##__VA_ARGS__); \
	} } while (0)

#define _HFI_EPDBG(fmt, ...) \
	do { if (fi_opx_global.prov) { \
		FI_DBG(fi_opx_global.prov, FI_LOG_FABRIC, fmt, ##__VA_ARGS__); \
	} } while (0)

#define _HFI_PRDBG(fmt, ...) \
	do { if (fi_opx_global.prov) { \
		FI_DBG(fi_opx_global.prov, FI_LOG_FABRIC, fmt, ##__VA_ARGS__); \
	} } while (0)

#define _HFI_ENVDBG(lev, fmt, ...) \
	do { if (fi_opx_global.prov) { \
		FI_DBG(fi_opx_global.prov, FI_LOG_FABRIC, fmt, ##__VA_ARGS__); \
	} } while (0)

#define _HFI_VDBG(fmt, ...) \
	do { if (fi_opx_global.prov) { \
		FI_DBG(fi_opx_global.prov, FI_LOG_FABRIC, fmt, ##__VA_ARGS__); \
	} } while (0)

#define _HFI_MMDBG(fmt, ...) \
	do { if (fi_opx_global.prov) { \
		FI_DBG(fi_opx_global.prov, FI_LOG_FABRIC, fmt, ##__VA_ARGS__); \
	} } while (0)

#define _HFI_CCADBG(fmt, ...) \
	do { if (fi_opx_global.prov) { \
		FI_DBG(fi_opx_global.prov, FI_LOG_FABRIC, fmt, ##__VA_ARGS__); \
	} } while (0)


/******************** Logging best practices for OPX *********************************
--Use FI_WARN() to log messages that are important and need the most visiblity
	* These logs will be seen on any build of libfabric where FI_LOG_LEVEL is
	set to anything valid.
	FI_WARN() has a slight performance hit and should not be used in the crit path

--Use FI_INFO() to log messages that are useful information but not repetitive
	* These logs will be seen on any build of libfabric where FI_LOG_LEVEL is
	set to info or debug (not warn)
	FI_INFO() has a slight performance hit and should not be used in the crit path

--Use OPX_LOG_OBSERVABLE() to log messages that may be documented for service
	* These messages may be monitored by external supervisors, log harvesters, 
	or other customer applications, so don't change their text or format without 
	checking to see that you're not breaking something.  This is just a FI_INFO() 
	in a 'special' wrapper so devs are warned.
	* Do not use this on the crit path or for overly repetitive messages

--Use OPX_LOG() or one of its subclasses to log verbose messages on high-use paths.  
    This enables developers to quickly disable versbose logs that aren't needed 
	when debugging.  These can also be used on the non-critical path, however 
	these messages probably won't show up without a special/debug build.
*************************************************************************************/
#define OPX_LOG_OBSERVABLE(subsystem, ...)	   	 \
	FI_INFO(fi_opx_global.prov, subsystem, __VA_ARGS__);


/************************************************************************************
 * To enable one or more sub-classes of verbose logs in OPX, uncomment 
 * at least one of the following #define OPX_ENABLE_LOG*** lines and re-compile 
 * libfabric, OR use a -D parm to the compliler to define one more of the OPX verbose 
 * log class #defines below. Verbose logging will then be enabled on all build types
 * 
 * IF this is a debug type of build, AND none of the verbose log classes are enabled, 
 * THEN enable them all.  This is the default libfabric behavior, debug builds get 
 * all verbose logs.  However, this can get quite large when processing  traffic, 
 * therefore these log classes have been provided to reduce the volume of output.
 ************************************************************************************/

//#define OPX_ENABLE_LOG			//Enable verbose logs for general Opx 
//#define OPX_ENABLE_LOG_SHM		//Enable verbose logs for Shared-memory (Intranode)
//#define OPX_ENABLE_LOG_PKT		//Enable verbose logs for Packet processing/packet headers
//#define OPX_ENABLE_LOG_REL		//Enable verbose logs for Reliablity

//Debug builds with no verbose logging #define get all verbose logs (default libfabric behavior)
#if					\
defined(ENABLE_DEBUG) && ENABLE_DEBUG &&	\
!defined(OPX_ENABLE_LOG) &&		\
!defined(OPX_ENABLE_LOG_SHM) &&		\
!defined(OPX_ENABLE_LOG_PKT) &&		\
!defined(OPX_ENABLE_LOG_REL)	
	#define OPX_ENABLE_LOG
	#define OPX_ENABLE_LOG_SHM
	#define OPX_ENABLE_LOG_PKT
	#define OPX_ENABLE_LOG_REL
#endif

/* 
 * OPX_LOG() will by default be a nop/no code on optimzed builds and not affect performance.  
 * Use OPX_LOG() to safely handle message logging on critical/high use paths.
 * This log class is for general messages in Opx that don't fit into a particular class
*/
#ifdef OPX_ENABLE_LOG
#define OPX_LOG(level, subsystem, ...)	    \
	FI_LOG(fi_opx_global.prov, level, subsystem, __VA_ARGS__)
#else
#define OPX_LOG(level, subsystem, ...)
#endif

/* 
 * OPX_LOG_SHM() will by default be a nop/no code on optimzed builds and not affect performance.  
 * Use OPX_LOG_SHM() to safely handle message logging on critical/high use paths.
 * This log class is for shared memory/intranode tracing
*/
#ifdef OPX_ENABLE_LOG_SHM
#define OPX_LOG_SHM(level, subsystem, ...)	    \
	FI_LOG(fi_opx_global.prov, level, subsystem, __VA_ARGS__)
#else
#define OPX_LOG_SHM(level, subsystem, ...)
#endif

/* 
 * OPX_LOG_PKT() will by default be a nop/no code on optimzed builds and not affect performance.  
 * Use OPX_LOG_PKT() to safely handle message logging on critical/high use paths.
 * This log class is for packet processing tracing and packet header debug
*/
#ifdef OPX_ENABLE_LOG_PKT
#define OPX_LOG_PKT(level, subsystem, ...)	    \
	FI_LOG(fi_opx_global.prov, level, subsystem, __VA_ARGS__)
#else
#define OPX_LOG_PKT(level, subsystem, ...)
#endif

/* 
 * OPX_LOG_REL() will by default be a nop/no code on optimzed builds and not affect performance.  
 * Use OPX_LOG_REL() to safely handle message logging on critical/high use paths.
 * This log class is for tracing issues with reliability
*/
#ifdef OPX_ENABLE_LOG_REL
#define OPX_LOG_REL(level, subsystem, ...)	    \
	FI_LOG(fi_opx_global.prov, level, subsystem, __VA_ARGS__)
#else
#define OPX_LOG_REL(level, subsystem, ...)
#endif

#endif /* OPA_UDEBUG_H */
