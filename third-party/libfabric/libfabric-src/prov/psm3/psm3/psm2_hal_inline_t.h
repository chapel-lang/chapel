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
#ifndef  _PSM2_HAL_INLINE_T_H_
#define  _PSM2_HAL_INLINE_T_H_

/* The psm2_hal_inline_t.h file serves as a template to allow all HAL
   instances to easily and conveniently declare their HAL_INLINE functions
   This also helps guarantee that inline functions are properly declared
   since the DISPATCH macro won't enforce the function API signature
 */

/* functions which are inline unless > 1 HAL or debug build */
static PSMI_HAL_INLINE int PSMI_HAL_CAT_INL_SYM(context_open)
				(int unit,
				 int port, int addr_index,
				 uint64_t open_timeout,
				 psm2_ep_t ep,
				 psm2_uuid_t const job_key,
				 unsigned retryCnt);
static PSMI_HAL_INLINE int PSMI_HAL_CAT_INL_SYM(close_context)
				(psm2_ep_t ep);
#ifdef PSM_FI
static PSMI_HAL_INLINE int PSMI_HAL_CAT_INL_SYM(faultinj_allowed)
				(const char *name, psm2_ep_t ep);
#endif
static PSMI_HAL_INLINE psm2_error_t PSMI_HAL_CAT_INL_SYM(ips_ptl_init_pre_proto_init)
				(struct ptl_ips *ptl);
static PSMI_HAL_INLINE psm2_error_t PSMI_HAL_CAT_INL_SYM(ips_ptl_init_post_proto_init)
				(struct ptl_ips *ptl);
static PSMI_HAL_INLINE psm2_error_t PSMI_HAL_CAT_INL_SYM(ips_ptl_fini)
				(struct ptl_ips *ptl);
static PSMI_HAL_INLINE psm2_error_t PSMI_HAL_CAT_INL_SYM(ips_proto_init)
				(struct ips_proto *proto, uint32_t cksum_sz);
static PSMI_HAL_INLINE psm2_error_t PSMI_HAL_CAT_INL_SYM(ips_proto_update_linkinfo)
				(struct ips_proto *proto);
static PSMI_HAL_INLINE int PSMI_HAL_CAT_INL_SYM(ips_fully_connected)
				(ips_epaddr_t *ipsaddr);
static PSMI_HAL_INLINE psm2_error_t PSMI_HAL_CAT_INL_SYM(ips_ipsaddr_set_req_params)
				(struct ips_proto *proto,
					ips_epaddr_t *ipsaddr,
					const struct ips_connect_reqrep *req);
static PSMI_HAL_INLINE psm2_error_t PSMI_HAL_CAT_INL_SYM(ips_ipsaddr_process_connect_reply)
                                (struct ips_proto *proto,
                                	ips_epaddr_t *ipsaddr,
                                	const struct ips_connect_reqrep *req);
static PSMI_HAL_INLINE void PSMI_HAL_CAT_INL_SYM(ips_proto_build_connect_message)
				(struct ips_proto *proto,
					ips_epaddr_t *ipsaddr, uint8_t opcode,
					struct ips_connect_reqrep *req);
static PSMI_HAL_INLINE void PSMI_HAL_CAT_INL_SYM(ips_ipsaddr_init_addressing)
				(struct ips_proto *proto, psm2_epid_t epid,
					ips_epaddr_t *ipsaddr, uint16_t *lidp
					, psmi_gid128_t *gidp
					);

static PSMI_HAL_INLINE psm2_error_t PSMI_HAL_CAT_INL_SYM(ips_ipsaddr_init_connections)
				(struct ips_proto *proto, psm2_epid_t epid,
					ips_epaddr_t *ipsaddr);
static PSMI_HAL_INLINE void PSMI_HAL_CAT_INL_SYM(ips_ipsaddr_free)
				(ips_epaddr_t *ipsaddr,
					struct ips_proto *proto);
static PSMI_HAL_INLINE void PSMI_HAL_CAT_INL_SYM(ips_flow_init)
				(struct ips_flow *flow,
					struct ips_proto *proto);
static PSMI_HAL_INLINE void PSMI_HAL_CAT_INL_SYM(ips_ipsaddr_disconnect)
				(struct ips_proto *proto,
					ips_epaddr_t *ipsaddr);
static PSMI_HAL_INLINE psm2_error_t PSMI_HAL_CAT_INL_SYM(ips_ibta_init)
				(struct ips_proto *proto);
static PSMI_HAL_INLINE psm2_error_t PSMI_HAL_CAT_INL_SYM(ips_path_rec_init)
				(struct ips_proto *proto,
					struct ips_path_rec *path_rec,
					struct _ibta_path_rec *response);
static PSMI_HAL_INLINE psm2_error_t PSMI_HAL_CAT_INL_SYM(ips_ptl_pollintr)
				(psm2_ep_t ep, struct ips_recvhdrq *recvq,
					int fd_pipe, int next_timeout,
					uint64_t *pollok, uint64_t *pollcyc, uint64_t *pollintr);
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
static PSMI_HAL_INLINE void PSMI_HAL_CAT_INL_SYM(gdr_close)
				(void);
static PSMI_HAL_INLINE void* PSMI_HAL_CAT_INL_SYM(gdr_convert_gpu_to_host_addr)
				(unsigned long buf,
					size_t size, int flags, psm2_ep_t ep);
#endif /* PSM_CUDA || PSM_ONEAPI */
static PSMI_HAL_INLINE int PSMI_HAL_CAT_INL_SYM(get_port_index2pkey)
				(psm2_ep_t ep, int index);
static PSMI_HAL_INLINE int PSMI_HAL_CAT_INL_SYM(poll_type)
				(uint16_t, psm2_ep_t ep);

static PSMI_HAL_INLINE psm2_error_t PSMI_HAL_CAT_INL_SYM(spio_transfer_frame)
				(struct ips_proto *proto,
				 struct ips_flow *flow, struct ips_scb *scb,
				 uint32_t *payload, uint32_t length,
				 uint32_t isCtrlMsg, uint32_t cksum_valid,
				 uint32_t cksum
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
				 , uint32_t is_gpu_payload
#endif
					);
static PSMI_HAL_INLINE psm2_error_t PSMI_HAL_CAT_INL_SYM(transfer_frame)
				(struct ips_proto *proto,
				 struct ips_flow *flow, struct ips_scb *scb,
				 uint32_t *payload, uint32_t length,
				 uint32_t isCtrlMsg, uint32_t cksum_valid,
				 uint32_t cksum
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
				 , uint32_t is_gpu_payload
#endif
					);
static PSMI_HAL_INLINE psm2_error_t PSMI_HAL_CAT_INL_SYM(drain_sdma_completions)
				(struct ips_proto *proto);
static PSMI_HAL_INLINE int PSMI_HAL_CAT_INL_SYM(get_node_id)
				(int unit, int *nodep);


#endif /*  _PSM2_HAL_INLINE_T_H_ */
