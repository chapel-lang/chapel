/*

  This file is provided under a dual BSD/GPLv2 license.  When using or
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2016 Intel Corporation.
  Copyright(c) 2021 Cornelis Networks.

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

  Copyright(c) 2016 Intel Corporation.
  Copyright(c) 2021 Cornelis Networks.

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

/*

  hfi1_deprecated_gen1.h

  Contains certain features of the hfi1 module that have been deprecated.

  These features may still need to be supported by the psm library for
  reasons of backwards compatibility.
 */

#ifndef __HFI1_DEPRECATED_GEN1_H__

#define __HFI1_DEPRECATED_GEN1_H__

/* First, include the current hfi1_user.h file: */

#include <rdma/hfi/hfi1_user.h>

/* Determine if we need to define and declare deprecated
   entities based on the IB_IOCTL_MAGIC macro. */

#if defined( IB_IOCTL_MAGIC )

/* The macro: PSM2_SUPPORT_IW_CMD_API is used to stipulate
   adding compile-time support of either the ioctl() or write()
   command interfaces to the driver.  Note though that the
   final decision whether to support this depends on factors
   only known at runtime. */
#define PSM2_SUPPORT_IW_CMD_API 1
/* IOCTL_CMD_API_MODULE_MAJOR defines the first version of the hfi1
 * module that supports the ioctl() command interface.  Prior to this
 * (IOCTL_CMD_API_MODULE_MAJOR - 1 and smaller), the module used
 * write() for the command interface. */
#define IOCTL_CMD_API_MODULE_MAJOR        6

/*
 * round robin contexts across HFIs, then
 * ports; this is the default.
 * This option spreads the HFI selection within the local socket.
 * If it is preferred to spread job over over entire set of
 * HFIs within the system, see ALG_ACROSS_ALL below.
 */
#define HFI1_ALG_ACROSS_DEP 0

/*
 * use all contexts on an HFI (round robin
 * active ports within), then next HFI
 */
#define HFI1_ALG_WITHIN_DEP 1

struct hfi1_cmd_deprecated {
	__u32 type;        /* command type */
	__u32 len;         /* length of struct pointed to by add */
	__u64 addr;        /* pointer to user structure */
};

#define hfi1_cmd hfi1_cmd_deprecated

#define HFI1_ALG_ACROSS		HFI1_ALG_ACROSS_DEP
#define HFI1_ALG_WITHIN		HFI1_ALG_WITHIN_DEP

#else

#define HFI1_SWMAJOR_SHIFT 16

#endif /* defined( IB_IOCTL_MAGIC )*/

#define HFI1_ALG_ACROSS_ALL_DEP 2
#define HFI1_ALG_ACROSS_ALL	HFI1_ALG_ACROSS_ALL_DEP

/* Note that struct hfi1_user_info_dep declaration is identical to
   the struct hfi1_user_info declaration from MAJOR version 5 of the
   hfi1_user.h file. */
struct hfi1_user_info_dep {
	/*
	 * version of user software, to detect compatibility issues.
	 * Should be set to HFI1_USER_SWVERSION.
	 */
	__u32 userversion;
	__u16 pad;
	/* HFI selection algorithm, if unit has not selected */
	__u16 hfi1_alg;
	/*
	 * If two or more processes wish to share a context, each process
	 * must set the subcontext_cnt and subcontext_id to the same
	 * values.  The only restriction on the subcontext_id is that
	 * it be unique for a given node.
	 */
	__u16 subctxt_cnt;
	__u16 subctxt_id;
	/* 128bit UUID passed in by PSM. */
	__u8 uuid[16];
};

/*
 * We assume here that we have the hfi1_user.h file installed in the system path
 * with the 'flags' field defined in struct sdma_req_info. (At least, when the
 * user needs to run GPU workloads, this _should_ be the version of hfi1_user.h
 * file installed by the IFS.)
 */
struct sdma_req_info_v6_3 {
	/*
	 * bits 0-3 - version (currently unused)
	 * bits 4-7 - opcode (enum sdma_req_opcode)
	 * bits 8-15 - io vector count
	 */
	__u16 ctrl;
	/*
	 * Number of fragments contained in this request.
	 * User-space has already computed how many
	 * fragment-sized packet the user buffer will be
	 * split into.
	 */
	__u16 npkts;
	/*
	 * Size of each fragment the user buffer will be
	 * split into.
	 */
	__u16 fragsize;
	/*
	 * Index of the slot in the SDMA completion ring
	 * this request should be using. User-space is
	 * in charge of managing its own ring.
	 */
	__u16 comp_idx;
} __attribute__((packed));

#endif /* #ifndef __HFI1_DEPRECATED_GEN1_H__ */
