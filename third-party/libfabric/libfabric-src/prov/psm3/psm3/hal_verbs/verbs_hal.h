#ifdef PSM_VERBS
/*

  This file is provided under a dual BSD/GPLv2 license.  When using or
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2017 Intel Corporation.

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

  Copyright(c) 2017 Intel Corporation.

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

#ifndef _PSM_HAL_VERBS_HAL_H
#define _PSM_HAL_VERBS_HAL_H

#include "psm_user.h"
#include "ips_proto.h"
#include "ips_proto_internal.h"
#include "psm_mq_internal.h"
#include "verbs_user.h"

/* declare hfp_verbs_t struct, (combines public psmi_hal_instance_t
   together with a private struct) */
typedef struct _hfp_verbs
{
	psmi_hal_instance_t phi;
} hfp_verbs_t;

psm2_error_t psm3_verbs_ips_ptl_init_pre_proto_init(struct ptl_ips *ptl);
psm2_error_t psm3_verbs_ips_ptl_init_post_proto_init(struct ptl_ips *ptl);
psm2_error_t psm3_verbs_ips_ptl_fini(struct ptl_ips *ptl);

psm2_error_t psm3_verbs_ips_ptl_pollintr(psm2_ep_t ep,
				struct ips_recvhdrq *recvq, int fd_pipe, int next_timeout,
				uint64_t *pollok, uint64_t *pollcyc, uint64_t *pollintr);

int psm3_verbs_ips_ptl_process_unknown(const struct ips_recvhdrq_event *rcv_ev);

psm2_error_t
psm3_verbs_recvhdrq_init(const struct ips_epstate *epstate,
		  const struct ips_proto *proto,
		  const struct ips_recvhdrq_callbacks *callbacks,
		  struct ips_recvhdrq *recvq
		);

psm2_error_t psm3_verbs_recvhdrq_progress(struct ips_recvhdrq *recvq);

#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
void* psm3_verbs_gdr_convert_gpu_to_host_addr(unsigned long buf,
                                size_t size, int flags,
                                psm2_ep_t ep);
#endif /* PSM_CUDA || PSM_ONEAPI */

#endif /* _PSM_HAL_VERBS_HAL_H */
#endif /* PSM_VERBS */
