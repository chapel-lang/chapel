/*

  This file is provided under a dual BSD/GPLv2 license.  When using or
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2018 Intel Corporation.

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

  Copyright(c) 2018 Intel Corporation.

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

#ifndef PTL_AM_AM_CONFIG_H
#define PTL_AM_AM_CONFIG_H

#include "psm_config.h"

#define AMSH_HAVE_CMA   0x1
#define AMSH_HAVE_KASSIST 0x1

/* Each block reserves some space at the beginning to store auxiliary data */
#define AMSH_BLOCK_HEADER_SIZE  4096

/* These are the defaults for shm Fifos.  Tunable via PSM3_SHM_ env */
/* Short and Long shm Fifo Queue depths */
#define AMSHORT_Q_NO_DSA 1024
#define AMLONG_Q_NO_DSA 256
/* AMLONG_SZ is the total size in memory of a bulk packet, including an
 * am_pkt_bulk_t header struct.
 */
#define AMLONG_SZ_NO_DSA   8192
// for AI workloads with limited processes and multi-ep, better to have
// large MTU and will default to CMA off for all but 1st EP
#define AMLONG_SZ_MULTIEP 32768
// This is the range we allow AMLONG_SZ to be configured as
#define AMLONG_SZ_MIN 1024
#define AMLONG_SZ_MAX (1024*1024)

#ifdef PSM_DSA
/* DSA benefits from larger bulk packets and hence larger copies */
#define AMSHORT_Q_DSA 1024
#define AMLONG_Q_DSA 256
#define AMLONG_SZ_DSA   (1024*512)
#endif

// GPU only supports GET("cma-get") or OFF("none"), so can't use PUT as default
#define PSM3_KASSIST_MODE_DEFAULT PSM3_KASSIST_CMA_GET
#define PSM3_KASSIST_MODE_DEFAULT_STRING  "cma-get"

#ifdef PSM_FI
#define SHM_FAULTINJ_CMA_ERR	10000	/* 1 every X CMA get/put error */
#define SHM_FAULTINJ_CMA_NOTAVAIL 4	/* 1 every X CMA available at init */
#endif /* PSM_FI */


#endif /* PTL_AM_AM_CONFIG_H */
