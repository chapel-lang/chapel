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

#include "psm_user.h"
#include "psm2_hal.h"
#include "verbs_user.h"

#if PSMI_HAL_INST_CNT > 1 || defined(PSM_DEBUG)
#define PSMI_HAL_CAT_INL_SYM(KERNEL) psm3_hfp_verbs_ ## KERNEL
// declare all the HAL_INLINE functions and pull in implementation as non-inline
#include "psm2_hal_inline_t.h"
#include "verbs_hal_inline_i.h"
#endif

static int psm3_hfp_verbs_initialize(psmi_hal_instance_t *phi,
										int devid_enabled[PTL_MAX_INIT])
{
	/* we initialize a few HAL software specific capabilities which
	 * are known before context_open can open RV or parse HAL specific
	 * env variables.  Additional flags may be added to cap_mask by
	 * context_open.
	 * Any flags which influence PSM env variable parsing prior to
	 * context_open must be set here
	 */
	// UD and RC can handle odd length packets
	// We can support SDMA and RDMA 
	phi->params.cap_mask = PSM_HAL_CAP_NON_DW_PKT_SIZE
			| PSM_HAL_CAP_SDMA
			| PSM_HAL_CAP_RDMA
			;
	// GPUDIRECT, GPUDIRECT_SDMA, GPUDIRECT_RDMA set in context_open based
	// on env variables and RV capabilities
	// USER_MR capability set in context_open based on RV capability
	return 0;
}

/* functions called via DISPATCH_FUNC */
static int psm3_hfp_verbs_finalize_(void)
{
	return 0;
}

static const char* psm3_hfp_verbs_identify(void)
{
	static char buf[100];

#ifdef RNDV_MOD
/* we test NVIDIA_GPU_DIRECT here instead of PSM_CUDA since that define
 * controls the rv module ioctl header file interface
 */
#ifdef NVIDIA_GPU_DIRECT
	snprintf(buf, sizeof(buf), "HAL: %s (%s) built against rv interface v%d.%d gpu v%d.%d cuda",
			psmi_hal_get_hal_instance_name(),
			psmi_hal_get_hal_instance_description(),
			psm3_rv_get_user_major_bldtime_version(),
			psm3_rv_get_user_minor_bldtime_version(),
			psm3_rv_get_gpu_user_major_bldtime_version(),
			psm3_rv_get_gpu_user_minor_bldtime_version());
#elif defined(INTEL_GPU_DIRECT)
	snprintf(buf, sizeof(buf), "HAL: %s (%s) built against rv interface v%d.%d gpu v%d.%d oneapi-ze",
			psmi_hal_get_hal_instance_name(),
			psmi_hal_get_hal_instance_description(),
			psm3_rv_get_user_major_bldtime_version(),
			psm3_rv_get_user_minor_bldtime_version(),
			psm3_rv_get_gpu_user_major_bldtime_version(),
			psm3_rv_get_gpu_user_minor_bldtime_version());
#else
	snprintf(buf, sizeof(buf), "HAL: %s (%s) built against rv interface v%d.%d",
			psmi_hal_get_hal_instance_name(),
			psmi_hal_get_hal_instance_description(),
			psm3_rv_get_user_major_bldtime_version(),
			psm3_rv_get_user_minor_bldtime_version());
#endif
#else /* RNDV_MOD */
	snprintf(buf, sizeof(buf), "HAL: %s (%s)",
			psmi_hal_get_hal_instance_name(),
			psmi_hal_get_hal_instance_description());
#endif /* RNDV_MOD */
	return buf;
}

// used as domain.name for fi_info
static const char *psm3_hfp_verbs_get_unit_name(int unit)
{
	return psm3_sysfs_unit_dev_name(unit);
}

// used as fabric.name for fi_info
static int psm3_hfp_verbs_get_port_subnet_name(int unit, int port, int addr_index, char *buf, size_t bufsize)
{
	psmi_subnet128_t subnet;

	if (psm3_hfp_verbs_get_port_subnet(unit, 1 /* VERBS_PORT */, addr_index,
							 &subnet, NULL, NULL, NULL))
		return -1;

	/* protocol argument ignored for PSMI_ADDR_FMT_IB */
	psm3_subnet128_fmt_name(PSMI_ETH_PROTO_ROCE, subnet,
					buf, bufsize);
	return 0;
}

static int psm3_hfp_verbs_get_port_lid(int unit, int port, int addr_index)
{
	return psm3_verbs_get_port_lid(unit, port, addr_index, VIMS_FILTER);
}

// initialize default MQ thresholds
// This is called prior to parsing PSM3_ env variables for MQ and
// also prior to the EP being opened (eg. NIC not yet selected).
static void psm3_hfp_verbs_mq_init_defaults(struct psm2_mq *mq)
{
	/* These values may be changed by initialize_params if user specifies
	 * corresponding PSM3_* env variables.
	 * Otherwise these defaults are used.
	 */
	unsigned rdmamode = psm3_verbs_parse_rdmamode(1);
	mq->hfi_thresh_rv = 64000;
	mq->hfi_base_window_rv = 131072;
	if (! (rdmamode & IPS_PROTOEXP_FLAG_ENABLED)) {
		// TBD - when RDMA is disabled do we want to disable rendezvous?
		// even without RDMA, the receiver controlled pacing helps scalability
		mq->hfi_thresh_rv = (~(uint32_t)0); // disable rendezvous
	}
	mq->hfi_thresh_tiny = PSM_MQ_NIC_MAX_TINY;
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
	if (PSMI_IS_GPU_ENABLED)
		mq->hfi_base_window_rv = 2097152;
#endif
	// we parse mr_cache_mode and rv_gpu_cache_size here so we can cache it
	// once per EP open, even if multi-rail or multi-QP
	(void)psm3_verbs_parse_mr_cache_mode(rdmamode, 1);
#ifdef RNDV_MOD
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
	(void)psmi_parse_gpudirect_rv_gpu_cache_size(1);
#endif
#endif
}

// initialize default EP Open options
// This is called in psm3_ep_open_internal prior to parsing PSM3_ env variables
// and also prior to the EP being opened (eg. NIC not yet selected).
static void psm3_hfp_verbs_ep_open_opts_get_defaults(struct psm3_ep_open_opts *opts)
{
	opts->imm_size = VERBS_SEND_MAX_INLINE; // PSM header size is 56
}

#ifdef PSM_CUDA
static void psm3_hfp_verbs_gdr_open(void)
{
}
#endif

/* functions called via DISPATCH_PI */
static int psm3_hfp_verbs_get_num_ports(void)
{
	return HFI_NUM_PORTS_VERBS;
}

static int psm3_hfp_verbs_get_unit_active(int unit)
{
	return psm3_verbs_get_unit_active(unit, VIMS_FILTER);
}

static int psm3_hfp_verbs_get_num_contexts(int unit)
{
	return 1024;
}

static int psm3_hfp_verbs_get_num_free_contexts(int unit)
{
	return 1024;
}

static int psm3_hfp_verbs_get_default_pkey(void)
{
	return 0;	/* use slot 0 as default */
}

static int psm3_hfp_verbs_get_unit_pci_bus(int unit, uint32_t *domain,
	uint32_t *bus, uint32_t *device, uint32_t *function)
{
	return psm3_sysfs_get_unit_pci_bus(unit, domain, bus, device, function);
}

static int psm3_hfp_verbs_get_unit_device_id(int unit, char *buf, size_t bufsize)
{
	return psm3_sysfs_get_unit_device_id(unit, buf, bufsize);
}

static int psm3_hfp_verbs_get_unit_device_version(int unit, char *buf, size_t bufsize)
{
	return psm3_sysfs_get_unit_device_version(unit, buf, bufsize);
}

static int psm3_hfp_verbs_get_unit_vendor_id(int unit, char *buf, size_t bufsize)
{
	return psm3_sysfs_get_unit_vendor_id(unit, buf, bufsize);
}

static int psm3_hfp_verbs_get_unit_driver(int unit, char *buf, size_t bufsize)
{
	return psm3_sysfs_get_unit_driver(unit, buf, bufsize);
}

/* define the singleton that implements hal for verbs */
static hfp_verbs_t psm3_verbs_hi = {
	/* start of public psmi_hal_instance_t data */
	.phi = {
		.hal_index				  = PSM_HAL_INDEX_VERBS,
		.description				  = "RDMA Verbs"
#ifdef PSM_CUDA
								" (cuda)"
#elif defined(PSM_ONEAPI)
								" (oneapi-ze)"
#endif
									,
		.nic_sys_class_path			  = "/sys/class/infiniband",
		.nic_sys_port_path_fmt			  = PSM3_PORT_PATH_TYPE_IB,
		.params					  = {0},

	/* functions called directly, no DISPATCH macro */
		.hfp_initialize				  = psm3_hfp_verbs_initialize,
		.hfp_have_active_unit			  = psm3_hfp_verbs_have_active_unit,

	/* called via DISPATCH_FUNC */
		.hfp_finalize_				  = psm3_hfp_verbs_finalize_,
		.hfp_identify				  = psm3_hfp_verbs_identify,
		.hfp_get_unit_name			  = psm3_hfp_verbs_get_unit_name,
		.hfp_get_port_subnet_name		  = psm3_hfp_verbs_get_port_subnet_name,
		.hfp_get_port_speed			  = psm3_hfp_verbs_get_port_speed,
		.hfp_get_port_lid			  = psm3_hfp_verbs_get_port_lid,
		.hfp_mq_init_defaults			  = psm3_hfp_verbs_mq_init_defaults,
		.hfp_ep_open_opts_get_defaults		  = psm3_hfp_verbs_ep_open_opts_get_defaults,
		.hfp_context_initstats			  = psm3_hfp_verbs_context_initstats,
#ifdef PSM_CUDA
		.hfp_gdr_open				  = psm3_hfp_verbs_gdr_open,
#endif

	/* called via DISPATCH_PI */
		.hfp_get_num_units			  = psm3_hfp_verbs_get_num_units,
		.hfp_get_num_ports			  = psm3_hfp_verbs_get_num_ports,
		.hfp_get_unit_active			  = psm3_hfp_verbs_get_unit_active,
		.hfp_get_port_active			  = psm3_hfp_verbs_get_port_active,
		.hfp_get_num_contexts			  = psm3_hfp_verbs_get_num_contexts,
		.hfp_get_num_free_contexts		  = psm3_hfp_verbs_get_num_free_contexts,
		.hfp_get_default_pkey			  = psm3_hfp_verbs_get_default_pkey,
		.hfp_get_port_subnet			  = psm3_hfp_verbs_get_port_subnet,
		.hfp_get_unit_pci_bus			  = psm3_hfp_verbs_get_unit_pci_bus,
		.hfp_get_unit_device_id			  = psm3_hfp_verbs_get_unit_device_id,
		.hfp_get_unit_device_version		  = psm3_hfp_verbs_get_unit_device_version,
		.hfp_get_unit_vendor_id			  = psm3_hfp_verbs_get_unit_vendor_id,
		.hfp_get_unit_driver			  = psm3_hfp_verbs_get_unit_driver,

	/* called via DISPATCH, may be inline */
#if PSMI_HAL_INST_CNT > 1 || defined(PSM_DEBUG)
		.hfp_context_open			  = psm3_hfp_verbs_context_open,
		.hfp_close_context			  = psm3_hfp_verbs_close_context,
		.hfp_context_check_status		  = psm3_hfp_verbs_context_check_status,
#ifdef PSM_FI
		.hfp_faultinj_allowed			  = psm3_hfp_verbs_faultinj_allowed,
#endif
		.hfp_ips_ptl_init_pre_proto_init	  = psm3_hfp_verbs_ips_ptl_init_pre_proto_init,
		.hfp_ips_ptl_init_post_proto_init	  = psm3_hfp_verbs_ips_ptl_init_post_proto_init,
		.hfp_ips_ptl_fini			  = psm3_hfp_verbs_ips_ptl_fini,
		.hfp_ips_proto_init			  = psm3_hfp_verbs_ips_proto_init,
		.hfp_ips_proto_update_linkinfo		  = psm3_hfp_verbs_ips_proto_update_linkinfo,
		.hfp_ips_fully_connected		  = psm3_hfp_verbs_ips_fully_connected,
		.hfp_ips_ipsaddr_set_req_params		  = psm3_hfp_verbs_ips_ipsaddr_set_req_params,
		.hfp_ips_ipsaddr_process_connect_reply	  = psm3_hfp_verbs_ips_ipsaddr_process_connect_reply,
		.hfp_ips_proto_build_connect_message	  = psm3_hfp_verbs_ips_proto_build_connect_message,
		.hfp_ips_ipsaddr_init_addressing	  = psm3_hfp_verbs_ips_ipsaddr_init_addressing,
		.hfp_ips_ipsaddr_init_connections	  = psm3_hfp_verbs_ips_ipsaddr_init_connections,
		.hfp_ips_ipsaddr_free			  = psm3_hfp_verbs_ips_ipsaddr_free,
		.hfp_ips_flow_init			  = psm3_hfp_verbs_ips_flow_init,
		.hfp_ips_ipsaddr_disconnect		  = psm3_hfp_verbs_ips_ipsaddr_disconnect,
		.hfp_ips_ibta_init			  = psm3_hfp_verbs_ips_ibta_init,
		.hfp_ips_path_rec_init			  = psm3_hfp_verbs_ips_path_rec_init,
		.hfp_ips_ptl_pollintr			  = psm3_hfp_verbs_ips_ptl_pollintr,
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
		.hfp_gdr_close				  = psm3_hfp_verbs_gdr_close,
		.hfp_gdr_convert_gpu_to_host_addr	  = psm3_hfp_verbs_gdr_convert_gpu_to_host_addr,
#ifdef PSM_ONEAPI
		.hfp_gdr_munmap_gpu_to_host_addr	  = psm3_hfp_verbs_gdr_munmap_gpu_to_host_addr,
#endif
#endif /* PSM_CUDA || PSM_ONEAPI */
		.hfp_get_port_index2pkey		  = psm3_hfp_verbs_get_port_index2pkey,
		.hfp_poll_type				  = psm3_hfp_verbs_poll_type,
		.hfp_spio_transfer_frame		  = psm3_hfp_verbs_spio_transfer_frame,
		.hfp_transfer_frame			  = psm3_hfp_verbs_transfer_frame,
		.hfp_drain_sdma_completions		  = psm3_hfp_verbs_drain_sdma_completions,
		.hfp_get_node_id			  = psm3_hfp_verbs_get_node_id,
#endif /* PSMI_HAL_INST_CNT > 1 || defined(PSM_DEBUG) */
	},
};

static void __attribute__ ((constructor)) __psmi_hal_verbs_constructor(void)
{
	psm3_hal_register_instance((psmi_hal_instance_t*)&psm3_verbs_hi);
}
#endif /* PSM_VERBS */
