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

#ifndef OPA_DEBUG_H
#define OPA_DEBUG_H

// See opa_udebug.h for macros and comments about these settings

#ifndef _HFI_DEBUGGING		/* debugging enabled or not */
#define _HFI_DEBUGGING 1
#endif

#if _HFI_DEBUGGING

/*
 * Mask values for debugging.  The scheme allows us to compile out any
 * of the debug tracing stuff, and if compiled in, to enable or disable
 * dynamically.  This can be set at modprobe time also:
 *      modprobe hfi.ko hfi_debug=7
 */

#define __HFI_INFO        0x1	/* generic low verbosity stuff */
#define __HFI_DBG         0x2	/* generic debug */
#define __HFI_TRSAMPLE    0x8	/* generate trace buffer sample entries */
/* leave some low verbosity spots open */
/* Debug messages specific to UD */
#define __HFI_UDDBG       0x10
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
#define __HFI_EPKTDBG     0x800	/* print error packet data */
#define __HFI_CCADBG      0x1000	/* print CCA related events */
#else /* _HFI_DEBUGGING */

/*
 * define all of these even with debugging off, for the few places that do
 * if(hfi_debug & _HFI_xyzzy), but in a way that will make the
 * compiler eliminate the code
 */

#define __HFI_INFO      0x0	/* generic low verbosity stuff */
#define __HFI_DBG       0x0	/* generic debug */
#define __HFI_TRSAMPLE  0x0	/* generate trace buffer sample entries */
#define __HFI_UDDBG     0x0
#define __HFI_CONNDBG   0x0
#define __HFI_VERBDBG   0x0	/* very verbose debug */
#define __HFI_PKTDBG    0x0	/* print packet data */
#define __HFI_PROCDBG   0x0	/* print process startup (init)/exit messages */
/* print MR, mmap/nopage stuff, not using VDBG any more */
#define __HFI_MMDBG     0x0
#define __HFI_CCADBG    0x0	/* print CCA related events */

#endif /* _HFI_DEBUGGING */

#define __HFI_VERBOSEDBG __HFI_VERBDBG

#endif /* OPA_DEBUG_H */
