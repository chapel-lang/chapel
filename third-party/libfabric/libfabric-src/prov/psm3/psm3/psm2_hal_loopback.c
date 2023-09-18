/*

  This file is provided under a dual BSD/GPLv2 license.  When using or
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2021 Intel Corporation.

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

  Copyright(c) 2021 Intel Corporation.

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
#include "psm_mq_internal.h"
#include "psm2_hal_loopback.h"

/* This is a loopback HAL suitable only for single node jobs
 * When PSM3_DEVICES does not include IPS (aka NIC), we may use this
 * HAL so that fi_getinfo and other queries early in job startup
 * work sensibly.  However this HAL does not open any real devices
 * and does not move any data via "ptl_ips" so many of it's
 * function pointers are intentionally left NULL.  It is purposely
 * not registered so that PSMI_HAL_INST_CNT can be 1 when only 1 real
 * HAL is built in, in which case the real HAL's datapath functions can
 * be inline.
 *
 * We purposely do not register this HAL at job startup.  It is handled
 * specially in psm3_hal_initialized and manually registered when needed.
 */

static inline struct hfp_loopback *get_psm_loopback_hi(void)
{
        return (struct hfp_loopback*) psm3_hal_current_hal_instance;
}


static int psm3_hfp_loopback_initialize(psmi_hal_instance_t *phi,
										int devid_enabled[PTL_MAX_INIT])
{
	hfp_loopback_t *loopback_hi = (hfp_loopback_t*)phi;
	phi->params.cap_mask = 0;
	if (psm3_device_is_enabled(devid_enabled, PTL_DEVID_AMSH)) {
		loopback_hi->hfp_private.unit_name = "shm";
		loopback_hi->hfp_private.subnet_name = "shm";
	} else {
		loopback_hi->hfp_private.unit_name = "self";
		loopback_hi->hfp_private.subnet_name = "self";
	}
	return 0;
}

static int psm3_hfp_loopback_finalize_(void)
{
	return 0;
}

const char* psm3_hfp_loopback_identify(void)
{
	static char buf[100];

	snprintf(buf, sizeof(buf), "HAL: %s (%s)",
			psmi_hal_get_hal_instance_name(),
			psmi_hal_get_hal_instance_description());
	return buf;
}

static int psm3_hfp_loopback_get_num_units(void)
{
	return 1;
}

static int psm3_hfp_loopback_get_num_ports(void)
{
	return 1;
}

static int psm3_hfp_loopback_have_active_unit(int num_units)
{
	return 1;	// should never be called
}

static int psm3_hfp_loopback_get_unit_active(int unit)
{
	return (unit == 0);
}

static int psm3_hfp_loopback_get_port_active(int unit, int port)
{
	return (unit == 0) && (port == 1);
}

static int psm3_hfp_loopback_get_num_contexts(int unit)
{
	return 1024;
}

static int psm3_hfp_loopback_get_num_free_contexts(int unit)
{
	return 1024;
}

static int psm3_hfp_loopback_get_port_subnet(int unit, int port, int addr_index,
	psmi_subnet128_t *subnet, psmi_naddr128_t *addr,
	int *idx, psmi_gid128_t *gid)
{
	/* required function, but should never be called because
	 * QUERY_UNIT_SUBNET_NAME specially handles loopback HAL */
	return -1;
}

static PSMI_HAL_INLINE const char *psm3_hfp_loopback_get_unit_name(int unit)
{
	return get_psm_loopback_hi()->hfp_private.unit_name;
}

static PSMI_HAL_INLINE int psm3_hfp_loopback_get_port_subnet_name(int unit, int port, int addr_index, char *buf, size_t bufsize)
{
	snprintf(buf, bufsize, "%s", get_psm_loopback_hi()->hfp_private.subnet_name);
	return 0;
}

static int psm3_hfp_loopback_get_unit_pci_bus(int unit, uint32_t *domain,
	uint32_t *bus, uint32_t *device, uint32_t *function)
{
	if (unit != 0) return -1;
	return -3;	/* no NIC to report details for */
}

static int psm3_hfp_loopback_get_unit_device_id(int unit, char *buf, size_t bufsize)
{
	if (unit != 0) return -1;
	return -3;	/* no NIC to report details for */
}

static int psm3_hfp_loopback_get_unit_device_version(int unit, char *buf, size_t bufsize)
{
	if (unit != 0) return -1;
	return -3;	/* no NIC to report details for */
}

static int psm3_hfp_loopback_get_unit_vendor_id(int unit, char *buf, size_t bufsize)
{
	if (unit != 0) return -1;
	return -3;	/* no NIC to report details for */
}

static int psm3_hfp_loopback_get_unit_driver(int unit, char *buf, size_t bufsize)
{
	if (unit != 0) return -1;
	return -3;	/* no NIC to report details for */
}

static int psm3_hfp_loopback_get_port_speed(int unit, int port, uint64_t *speed)
{
	if (unit != 0) return -1;
	return -3;	/* no NIC to report details for */
}

static int psm3_hfp_loopback_get_port_lid(int unit, int port, int addr_index)
{
	if (unit != 0 || port != 1 || addr_index != 0) return -1;
	return 1;
}

// initialize default MQ thresholds
// This is called prior to parsing PSM3_ env variables for MQ and
// also prior to the EP being opened
static void psm3_hfp_loopback_mq_init_defaults(struct psm2_mq *mq)
{
	/* these are only used by ptl_ips */
	mq->hfi_base_window_rv =  (~(uint32_t)0); // no rendezvous
	mq->hfi_thresh_rv = (~(uint32_t)0); // disable rendezvous
	mq->hfi_thresh_tiny = PSM_MQ_NIC_MAX_TINY;
	// RDMA and MR cache N/A, leave ep->rdmamode, ep->mr_cache_mode and
	// ep->rv_gpu_cache_size as set by caller (0, NONE, 0)
}

// initialize default EP Open options
// This is called in psm3_ep_open_internal prior to parsing PSM3_ env variables
// and also prior to the EP being opened
static void psm3_hfp_loopback_ep_open_opts_get_defaults(struct psm3_ep_open_opts *opts)
{
	opts->imm_size = 128;
}

static int psm3_hfp_loopback_get_default_pkey(void)
{
	return 0x8001;	// not used (only used in ptl_ips), pick a safe value
}

#ifdef PSM_CUDA
static void psm3_hfp_loopback_gdr_open(void)
{
	/* disable GPU Direct copy, no driver to help us */
	is_gdr_copy_enabled = gdr_copy_limit_send = gdr_copy_limit_recv = 0;
}
#endif

/* define the singleton that implements loopback hal
 * we only need the functions which are used when IPS is not enabled
 * all others can be NULL and will cause a segfault if mistakenly used
 */
hfp_loopback_t psm3_loopback_hi = {
	/* start of public psmi_hal_instance_t data */
	.phi = {
		.hal_index				  = PSM_HAL_INDEX_LOOPBACK,
		.description				  = "loopback",
		.nic_sys_class_path			  = NULL,
		.nic_sys_port_path_fmt			  = 0,
		.params					  = {0},

	/* functions called directly, no DISPATCH macro */
		.hfp_initialize				  = psm3_hfp_loopback_initialize,
		.hfp_have_active_unit			  = psm3_hfp_loopback_have_active_unit,

	/* called via DISPATCH_FUNC */
		.hfp_finalize_				  = psm3_hfp_loopback_finalize_,
		.hfp_identify				  = psm3_hfp_loopback_identify,
		.hfp_get_unit_name				  = psm3_hfp_loopback_get_unit_name,
		.hfp_get_port_subnet_name		  = psm3_hfp_loopback_get_port_subnet_name,
		.hfp_get_port_speed			  = psm3_hfp_loopback_get_port_speed,
		.hfp_get_port_lid			  = psm3_hfp_loopback_get_port_lid,
		.hfp_mq_init_defaults			  = psm3_hfp_loopback_mq_init_defaults,
		.hfp_ep_open_opts_get_defaults		  = psm3_hfp_loopback_ep_open_opts_get_defaults,
		.hfp_context_initstats			  = NULL, // ptl_ips only
#ifdef PSM_CUDA
		.hfp_gdr_open			  	= psm3_hfp_loopback_gdr_open,
#endif

	/* called vis DISPATCH_PI */
		.hfp_get_num_units			  = psm3_hfp_loopback_get_num_units,
		.hfp_get_num_ports			  = psm3_hfp_loopback_get_num_ports,
		.hfp_get_unit_active			  = psm3_hfp_loopback_get_unit_active,
		.hfp_get_port_active			  = psm3_hfp_loopback_get_port_active,
		.hfp_get_num_contexts			  = psm3_hfp_loopback_get_num_contexts,
		.hfp_get_num_free_contexts		  = psm3_hfp_loopback_get_num_free_contexts,
		.hfp_get_default_pkey			  = psm3_hfp_loopback_get_default_pkey,
		.hfp_get_port_subnet			  = psm3_hfp_loopback_get_port_subnet,
		.hfp_get_unit_pci_bus			  = psm3_hfp_loopback_get_unit_pci_bus,
		.hfp_get_unit_device_id			  = psm3_hfp_loopback_get_unit_device_id,
		.hfp_get_unit_device_version		  = psm3_hfp_loopback_get_unit_device_version,
		.hfp_get_unit_vendor_id			  = psm3_hfp_loopback_get_unit_vendor_id,
		.hfp_get_unit_driver			  = psm3_hfp_loopback_get_unit_driver,

	/* called via DISPATCH, may be inline */
	/* none of these will be called for loopback since they are
	 * only used when a NIC has been opened for ptl_ips
	 */
#if PSMI_HAL_INST_CNT > 1 || defined(PSM_DEBUG)
		.hfp_context_open			  = NULL,
		.hfp_close_context			  = NULL,
		.hfp_context_check_status		  = NULL,
#ifdef PSM_FI
		.hfp_faultinj_allowed		  = NULL,
#endif
		.hfp_ips_ptl_init_pre_proto_init	  = NULL,
		.hfp_ips_ptl_init_post_proto_init	  = NULL,
		.hfp_ips_ptl_fini			  = NULL,
		.hfp_ips_proto_init			  = NULL,
		.hfp_ips_proto_update_linkinfo		  = NULL,
		.hfp_ips_fully_connected		  = NULL,
		.hfp_ips_ipsaddr_set_req_params		  = NULL,
		.hfp_ips_ipsaddr_process_connect_reply	  = NULL,
		.hfp_ips_proto_build_connect_message	  = NULL,
		.hfp_ips_ipsaddr_init_addressing	  = NULL,
		.hfp_ips_ipsaddr_init_connections	  = NULL,
		.hfp_ips_ipsaddr_free			  = NULL,
		.hfp_ips_flow_init			  = NULL,
		.hfp_ips_ipsaddr_disconnect		  = NULL,
		.hfp_ips_ibta_init			  = NULL,
		.hfp_ips_path_rec_init			  = NULL,
		.hfp_ips_ptl_pollintr			  = NULL,
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
		.hfp_gdr_close				  = NULL,
		.hfp_gdr_convert_gpu_to_host_addr	  = NULL,
#ifdef PSM_ONEAPI
		.hfp_gdr_munmap_gpu_to_host_addr	  = NULL,
#endif
#endif /* PSM_CUDA || PSM_ONEAPI */
		.hfp_get_port_index2pkey		  = NULL,
		.hfp_poll_type				  = NULL,
		.hfp_spio_transfer_frame		  = NULL,
		.hfp_transfer_frame			  = NULL,
		.hfp_get_node_id			  = NULL,
#endif /* PSMI_HAL_INST_CNT > 1 || defined(PSM_DEBUG) */
	},

	/* start of private hfp_loopback_private data */
	.hfp_private = {
		.unit_name   = NULL,
		.subnet_name   = NULL,
	}
};
