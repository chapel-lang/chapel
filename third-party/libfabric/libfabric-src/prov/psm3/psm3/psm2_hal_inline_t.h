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

/* The psm2_hal_inline_t.h file serves as a template to allow all HAL
   instances to easily and conveniently declare their HAL methods. */

static PSMI_HAL_INLINE int PSMI_HAL_CAT_INL_SYM(initialize)
				(psmi_hal_instance_t *);
static PSMI_HAL_INLINE int PSMI_HAL_CAT_INL_SYM(finalize_)
				(void);
static PSMI_HAL_INLINE int PSMI_HAL_CAT_INL_SYM(get_num_units)
				(void);
static PSMI_HAL_INLINE int PSMI_HAL_CAT_INL_SYM(get_num_ports)
				(void);
static PSMI_HAL_INLINE int PSMI_HAL_CAT_INL_SYM(get_unit_active)
				(int unit);
static PSMI_HAL_INLINE int PSMI_HAL_CAT_INL_SYM(get_node_id)
				(int unit, int *nodep);
static PSMI_HAL_INLINE int PSMI_HAL_CAT_INL_SYM(get_port_active)
				(int unit, int port);
static PSMI_HAL_INLINE int PSMI_HAL_CAT_INL_SYM(get_num_contexts)
				(int unit);
static PSMI_HAL_INLINE int PSMI_HAL_CAT_INL_SYM(get_num_free_contexts)
				(int unit);
static PSMI_HAL_INLINE int PSMI_HAL_CAT_INL_SYM(close_context)
				(psmi_hal_hw_context *);
static PSMI_HAL_INLINE int PSMI_HAL_CAT_INL_SYM(context_open)
				(int unit,
				 int port,
				 uint64_t open_timeout,
				 psm2_ep_t ep,
				 psm2_uuid_t const job_key,
				 psmi_context_t *psm_ctxt,
				 uint32_t cap_mask,
				 unsigned);
static PSMI_HAL_INLINE void PSMI_HAL_CAT_INL_SYM(context_initstats)
				 (psm2_ep_t ep);


static PSMI_HAL_INLINE int PSMI_HAL_CAT_INL_SYM(get_port_rate)
				(int unit, int port);


static PSMI_HAL_INLINE int PSMI_HAL_CAT_INL_SYM(get_port_lid)
				(int unit, int port);
static PSMI_HAL_INLINE int PSMI_HAL_CAT_INL_SYM(get_port_subnet)
				(int unit, int port, uint64_t *subnet, uint64_t *addr,
				uint32_t *ip_addr, uint32_t *netmask,
				int *idx, uint64_t *hi, uint64_t *lo);


static PSMI_HAL_INLINE int PSMI_HAL_CAT_INL_SYM(get_default_pkey)
				(void);


static PSMI_HAL_INLINE int PSMI_HAL_CAT_INL_SYM(spio_transfer_frame)
				(struct ips_proto *proto,
				 struct ips_flow *flow, struct ips_scb *scb,
				 uint32_t *payload, uint32_t length,
				 uint32_t isCtrlMsg, uint32_t cksum_valid,
				 uint32_t cksum, psmi_hal_hw_context
#ifdef PSM_CUDA
				 , uint32_t is_cuda_payload
#endif
					);
static PSMI_HAL_INLINE int PSMI_HAL_CAT_INL_SYM(spio_process_events)
				(const struct ptl *ptl);


static PSMI_HAL_INLINE int      PSMI_HAL_CAT_INL_SYM(get_jkey)
				(psmi_hal_hw_context ctxt);

