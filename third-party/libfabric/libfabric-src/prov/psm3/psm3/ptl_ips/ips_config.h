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

#ifndef PTL_IPS_IPS_CONFIG_H
#define PTL_IPS_IPS_CONFIG_H

#include "psm_config.h"

/* Allocate new epaddrs in chunks of 128 */
#define PTL_EPADDR_ALLOC_CHUNK  128

/* Generate an expected header every 16 packets */
#define PSM_DEFAULT_EXPECTED_HEADER 16

#define DF_OPP_LIBRARY "libopasadb.so.1.0.0"
#define DATA_VFABRIC_OFFSET 8

/* Send retransmission */
#define IPS_PROTO_SPIO_RETRY_US_DEFAULT	2	/* in uS */

#define IPS_PROTO_ERRCHK_MS_MIN_DEFAULT	160	/* in millisecs */
#define IPS_PROTO_ERRCHK_MS_MAX_DEFAULT	640	/* in millisecs */
#define IPS_PROTO_ERRCHK_FACTOR_DEFAULT 2
#define PSM_TID_TIMEOUT_DEFAULT "160:640:2"	/* update from above params */

#ifdef PSM_FI
/* Fault injection, becomes parameters to psm3_faultinj_getspec so
 * a comma-delimited list of
 *   "spec_name", num, denom
 * Where num/denom means fault num out of every denom.
 * The defines set 'denum' and assume that num is set to 1
 *
 * These values are all defaults, each is overridable via
 * PSM3_FI_<spec_name> in the environment (and yes, spec_name is in lowercase
 * *in the environment* just to minimize it appearing in the wild).  The format
 * there is <num:denom:initial_seed> so the same thing except that one can set
 * a specific seed to the random number generator.
 */
#define IPS_FAULTINJ_RECVLOST	5000	/* 1 every X pkts dropped at recv */
#define IPS_FAULTINJ_SENDFULL	5000	/* 1 every X pkts no resource at send */
#define IPS_FAULTINJ_SENDFULLCTRL 5000	/* 1 every X pkts no resource at send ctrl */
#define IPS_FAULTINJ_SENDFULLCB	5000	/* 1 every X pkts no resource at send ctrl callback */
#define IPS_FAULTINJ_SENDLOST	5000	/* 1 every X pkts dropped at send */
#define IPS_FAULTINJ_SENDPART	10	/* 1 every X pkts partial send */
#define IPS_FAULTINJ_RECVPART	10	/* 1 every X pkts partial recv */
#ifdef PSM_HAVE_REG_MR
#define IPS_FAULTINJ_RQ_LKEY	5000	/* 0 every X RQ WQE bad lkey */
#define IPS_FAULTINJ_SQ_LKEY	5000	/* 0 every X SQ WQE bad lkey */
#define IPS_FAULTINJ_RC_RDMA_LKEY 5000	/* 0 every X RC SQ RDMA bad lkey */
#define IPS_FAULTINJ_RC_RDMA_RKEY 5000	/* 0 every X RC SQ RDMA bad rkey */
#define IPS_FAULTINJ_RV_RDMA_LEN 5000	/* 0 every X RV SQ RDMA bad len */
#define IPS_FAULTINJ_RV_RDMA_RKEY 5000	/* 1 every X RV SQ RDMA bad rkey */
#define IPS_FAULTINJ_REG_MR	100	/* 1 every X reg_mr ENOMEM */
#define IPS_FAULTINJ_NONPRI_REG_MR 50	/* 1 every X non-pri reg_mr ENOMEM */
#define IPS_FAULTINJ_PRI_REG_MR	1000	/* 1 every X pri reg_mr ENOMEM */
#endif /* PSM_HAVE_REG_MR */
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
#define IPS_FAULTINJ_GDRMMAP	100	/* 1 every X GPU pin and mmap ENOMEM */
#define IPS_FAULTINJ_GPU_REG_MR	100	/* 1 every X GPU reg_mr */
#endif
#endif /* PSM_FI */



/* rcv thread */
/* All in milliseconds */
#define RCVTHREAD_TO_MIN_FREQ	    10	/* min of 10 polls per sec */
#define RCVTHREAD_TO_MAX_FREQ	    100	/* max of 100 polls per sec */
#define RCVTHREAD_TO_SHIFT	    1

/* ptl.c */
#define PSMI_CONTEXT_STATUS_CHECK_INTERVAL_MSECS	250

/* ips_proto_recv.c */
#define PSM_STRAY_WARN_INTERVAL_DEFAULT_SECS	30

#define PSM_CONN_CREDITS 100

/*
 * Easy switch to (say) _HFI_INFO if debugging in the expected protocol is
 * needed
 */
#define _HFI_EXP _HFI_VDBG

#endif /* PTL_IPS_IPS_CONFIG_H */
