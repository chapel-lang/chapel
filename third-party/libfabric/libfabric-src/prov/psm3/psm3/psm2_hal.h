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

#ifndef __PSM2_HAL_H__

#define __PSM2_HAL_H__

#include "psm_user.h"

/* Forward declaration of PSM structs: */
struct psm2_mq;
struct ips_recvhdrq_event;
struct ips_scb_pendlist;
struct ips_flow;
struct ips_scb;
struct ips_epinfo;
struct ips_message_header;
struct ips_connect_reqrep;
struct ips_path_rec;
struct _ibta_path_rec;
struct ptl_ips;
struct ips_recvhdrq;
struct psm3_ep_open_opts;

#include "psm2_hal_inlines_d.h"	// get PSMI_HAL_INST_CNT

/* Declare types: */

/* Each HAL has a specific slot in the HAL registration table (psm3_hal_table).
 * The order in the table is the order by which we will examine HALs.
 * Hence preferred or more device specific HALs are listed before more generic
 * or less desirable HALs.
 */
typedef enum
{
	PSM_HAL_INDEX_VERBS	=  1,
	PSM_HAL_INDEX_SOCKETS	=  2,
	PSM_HAL_INDEX_LOOPBACK	=  3,
	PSM_HAL_INDEX_MAX	=  3,
} psmi_hal_instance_index_t;

/* This string is used as the hal_name for both log messages
 * and PSM3_HAL env parsing
 */
static inline const char* psm3_hal_index_to_str(int index)
{
	switch (index) {
	case PSM_HAL_INDEX_VERBS: return "verbs";
	case PSM_HAL_INDEX_SOCKETS: return "sockets";
	case PSM_HAL_INDEX_LOOPBACK: return "loopback";
	default:	return "unknown";
	}
}

typedef enum
{
	/* Operation was successful.  No error occurred. */
	PSM_HAL_ERROR_OK			= 0,
	/* The operation can not be done unless HAL is initialized first. */
	PSM_HAL_ERROR_NOT_INITIALIZED		= 1,
	/* No HAL INSTANCE has been registered.  Initialization is impossible. */
	PSM_HAL_ERROR_NO_HI_REGISTERED		= 2,
	/* Initialization failure. */
	PSM_HAL_ERROR_INIT_FAILED		= 3,
	/* Can't open device file. */
	PSM_HAL_ERROR_CANNOT_OPEN_DEVICE	= 4,
	/* Can't open context. */
	PSM_HAL_ERROR_CANNOT_OPEN_CONTEXT	= 5,
	/* Context is not open. */
	PSM_HAL_ERROR_CONTEXT_IS_NOT_OPEN	= 6,
	/* General error. */
	PSM_HAL_ERROR_GENERAL_ERROR		= 7,
	/* Not implemented. */
	PSM_HAL_ERROR_NOT_IMPLEMENTED		= 8,
	/* Internal error. */
	PSM_HAL_ERROR_INTERNAL_ERROR		= 9,

	/* HAL instances should not return errors less than the value
	   PSM_HAL_ERROR_RESERVED_BY_HAL_API.  These errors are reserved by
	   the HAL API layer. */
	PSM_HAL_ERROR_RESERVED_BY_HAL_API	= 1000,
} psmi_hal_errors;



/* The following enum constants correspond to the bits in the
 * cap_mask member of the psmi_hal_params_t.
 * These represent capabilities of the HAL itself.  While many
 * of these are not known until the device is opened via context_open
 * care must be taken to use these capability bits to reflect general
 * capabilities of the HAL itself, as opposed to EP or device specific
 * capabilities.
 * Note that for multi-rail and multi-EP configurations, more than one
 * context_open may occur (same or different devices) and upper level code
 * which tests these capability flags assumes all devioes in the process
 * have the same capability.
 */
typedef enum
{
		/* can spio_transfer_frame handle a non 32b multiple
		 * payload length for both single packets and PIO
		 * segmentation (UDP GSO)
		 */
	PSM_HAL_CAP_NON_DW_PKT_SIZE		= (1UL << 0),
#ifdef PSM_HAVE_REG_MR
		/* has RV been loaded with enable_user_mr=1 module param */
	PSM_HAL_CAP_USER_MR			= (1UL << 1),
		/* Is Send DMA (using MRs) supported by the HAL.
		 * When supported, PSM3_SDMA env will control enablement.
		 * This is distinct from RDMA and Send segmentation (GSO)
		 */
	PSM_HAL_CAP_SDMA			= (1UL << 2),
		/* Is RDMA (and hence protoexp) supported by the HAL.
		 * When supported, PSM3_RDMA env will control enablement.
		 * This is distinct from Send DMA or Send segmentation (GSO)
		 */
	PSM_HAL_CAP_RDMA			= (1UL << 3),
#endif
		/* Are any GPUDIRECT features (Copy, Send DMA or RDMA)
		 * currently available for the given HAL.  Otherwise
		 * PSM3_GPUDIRECT=1 is disallowed (fatal).
		 * For UD and UDP, only true if RV also enabled for GPU.
		 * At a minimum when this is set, GPUDirect Copy must be allowed
		 */
	PSM_HAL_CAP_GPUDIRECT			= (1UL << 4),
		/* Is GPUDIRECT Send DMA (using MRs) currently available for
		 * the given HAL.  Otherwise we ignore PSM3_GPUDIRECT_SDMA.
		 * Upper level will only attempt to use GPUDIRECT SDMA if both
		 * this and PSM_HAL_CAP_GPUDIRECT are true.
		 * This should only be true if PSM_HAL_CAP_SDMA also true.
		 */
	PSM_HAL_CAP_GPUDIRECT_SDMA		= (1UL << 5),
		/* Is GPUDIRECT RDMA (send and recv) currently available for
		 * the given HAL.  Otherwise we ignore
		 * PSM3_GPUDIRECT_RDMA_SEND_LIMIT
		 * and PSM3_GPUDIRECT_RDMA_RECV_LIMIT.
		 * Upper level will only attempt to use GPUDIRECT RDMA if both
		 * this and PSM_HAL_CAP_GPUDIRECT are true.
		 * This should only be true if PSM_HAL_CAP_RDMA also true.
		 */
	PSM_HAL_CAP_GPUDIRECT_RDMA              = (1UL << 6),

} psmi_hal_capability_bits;

/* The following enum constants correspond to the bits in the
   sw_status member of the psmi_hal_params_t. */
typedef enum
{
	/* Request to start rx thread. */
	PSM_HAL_PSMI_RUNTIME_RTS_RX_THREAD	= (1UL <<  0),
	/* Rx thread is started. */
	PSM_HAL_PSMI_RUNTIME_RX_THREAD_STARTED	= (1UL <<  1),
	PSM_HAL_PSMI_RUNTIME_INTR_ENABLED       = (1UL <<  2),
	PSM_HAL_PARAMS_VALID_NUM_UNITS          = (1UL <<  4),
	PSM_HAL_PARAMS_VALID_NUM_PORTS          = (1UL <<  5),
	PSM_HAL_PARAMS_VALID_DEFAULT_PKEY       = (1UL <<  6),
	PSM_HAL_PARAMS_VALID_CACHE              = (1UL <<  7),

} psmi_hal_sw_status;

/* The _psmi_hal_params structure stores values that remain constant for the entire life of
   the process and this structure resides in the hal instance structure (below).
   The values are settled after the context is opened. */
typedef struct _psmi_hal_params
{
	uint32_t   cap_mask;
	uint32_t   sw_status;
	/* start cached members */
	uint16_t   num_units;
	uint16_t   num_ports;
	uint16_t   default_pkey;
	int8_t     *unit_active,*unit_active_valid;
	int8_t     *port_active,*port_active_valid;
	uint16_t   *num_contexts,*num_contexts_valid;
	uint16_t   *num_free_contexts,*num_free_contexts_valid;
		// information from port_get_subnet
	int8_t     *port_subnet_valid;
	uint8_t    *port_subnet_addr_fmt;
	psmi_subnet128_t   *port_subnet;
	psmi_naddr128_t   *port_subnet_addr;
	int        *port_subnet_idx;
	psmi_gid128_t   *port_subnet_gid;

	int8_t     *unit_pci_bus_valid;
	uint32_t   *unit_pci_bus_domain;
	uint32_t   *unit_pci_bus_bus;
	uint32_t   *unit_pci_bus_device;
	uint32_t   *unit_pci_bus_function;

	char **unit_device_id;
	char **unit_device_version;
	char **unit_vendor_id;
	char **unit_driver;
} psmi_hal_params_t;


#define PSM_HAL_ALG_ACROSS     0
#define PSM_HAL_ALG_WITHIN     1
#define PSM_HAL_ALG_ACROSS_ALL 2


typedef enum {
	PSMI_HAL_POLL_TYPE_URGENT = 1
} psmi_hal_poll_type;

/* Forward declaration of incomplete struct type _psmi_hal_instance and
 * psmi_hal_instance_t typedef: */

struct _psmi_hal_instance;
typedef struct _psmi_hal_instance psmi_hal_instance_t;

struct _psmi_hal_instance
{
	psmi_hal_instance_index_t	hal_index;
	const char			*description;
	const char			*nic_sys_class_path;
	/* format args: char *unitpath, uint32_t port, char *attr */
	const psm3_port_path_type	nic_sys_port_path_fmt;
	/* The params member should be read-only for HIC, and
	   written only by the HAL instance. */
	psmi_hal_params_t		params;

/* functions called directly, no DISPATCH macro */
	/* Initialize the HAL INSTANCE. */
	int (*hfp_initialize)(psmi_hal_instance_t *, int devid_enabled[PTL_MAX_INIT]);
	/* Given a number of units (as obtained via HAL's get_num_units),
	 * returns 1 if any port on any of the units is active
	 * returns <= 0 if no port on any of the units is active
	 * NOTE: hfp_have_active_unit is a function that must
	 * be callable before the hal instance is initialized. */
	int (*hfp_have_active_unit)(int num_units);

/* functions called via DISPATCH_FUNC */
	/* Finalize the HAL INSTANCE. */
	int (*hfp_finalize_)(void);

	/* Identify the HAL instance and what it was built against */
	const char* (*hfp_identify)(void);

	const char *(*hfp_get_unit_name)(int unit);
	int (*hfp_get_port_subnet_name)(int unit, int port, int addr_index, char *buf, size_t bufsize);
	int (*hfp_get_port_speed)(int unit, int port, uint64_t *speed);
	int (*hfp_get_port_lid)(int unit, int port, int addr_index);

	void (*hfp_mq_init_defaults)(struct psm2_mq *mq);
	void (*hfp_ep_open_opts_get_defaults)(struct psm3_ep_open_opts *opts);

	/* Initialize PSM3_PRINT_STATS stats for given ep */
	void (*hfp_context_initstats)(psm2_ep_t ep);

#ifdef PSM_CUDA
	void (*hfp_gdr_open)(void);
#endif

/* functions called via DISPATCH_PI */

	/* Returns the number of hfi units installed on ths host:
	   NOTE: hfp_get_num_units is a function that must
	   be callable before the hal instance is initialized. */
	int (*hfp_get_num_units)(void);

	/* Returns the number of ports on each hfi unit installed.
	   on ths host.
	   NOTE: hfp_get_num_ports is a function that must
	   be callable before the hal instance is initialized. */
	int (*hfp_get_num_ports)(void);

	/* Given a unit number, returns 1 if any port on the unit is active.
	   returns <=0 if no port on the unit is active.
	   NOTE: hfp_get_unit_active is a function that must
	   be callable before the hal instance is initialized. */
	int (*hfp_get_unit_active)(int unit);
	int (*hfp_get_port_active)(int unit,int port);

	/* NOTE: hfp_get_num_contexts is a function that must
	   be callable before the hal instance is initialized. */
	int (*hfp_get_num_contexts)(int unit);
	/* NOTE: hfp_get_num_free_contexts is a function that must
	   be callable before the hal instance is initialized. */
	int (*hfp_get_num_free_contexts)(int unit);

	/* Returns the default pkey:
	   NOTE: hfp_get_default_pkey is a function that must
	   be callable before the hal instance is initialized. */
	int (*hfp_get_default_pkey)(void);

	int (*hfp_get_port_subnet)(int unit, int port, int addr_index,
				psmi_subnet128_t *subnet, psmi_naddr128_t *addr,
				int *idx, psmi_gid128_t *gid);

	int (*hfp_get_unit_pci_bus)(int unit, uint32_t *domain, uint32_t *bus,
				uint32_t *device, uint32_t *function);
	int (*hfp_get_unit_device_id)(int unit, char *buf, size_t bufsize);
	int (*hfp_get_unit_device_version)(int unit, char *buf, size_t bufsize);
	int (*hfp_get_unit_vendor_id)(int unit, char *buf, size_t bufsize);
	int (*hfp_get_unit_driver)(int unit, char *buf, size_t bufsize);

/* functions which are inline unless > 1 HAL or debug build */
#if PSMI_HAL_INST_CNT > 1 || defined(PSM_DEBUG)
	/* Context open includes opening the device file, and get hw params. */
	int (*hfp_context_open)(int unit,
				int port, int addr_index,
				uint64_t open_timeout,
				psm2_ep_t ep,
				psm2_uuid_t const job_key,
				unsigned retryCnt);

	/* Close the context, including the device file. */
	int (*hfp_close_context)(psm2_ep_t ep);
	psm2_error_t (*hfp_context_check_status)(struct ptl_ips *ptl);

#ifdef PSM_FI
	int (*hfp_faultinj_allowed)(const char *name, psm2_ep_t ep);
#endif
	psm2_error_t (*hfp_ips_ptl_init_pre_proto_init)(struct ptl_ips *ptl);
	psm2_error_t (*hfp_ips_ptl_init_post_proto_init)(struct ptl_ips *ptl);
	psm2_error_t (*hfp_ips_ptl_fini)(struct ptl_ips *ptl);
	psm2_error_t (*hfp_ips_proto_init)(struct ips_proto *proto,
				uint32_t cksum_sz);
	psm2_error_t (*hfp_ips_proto_update_linkinfo)(struct ips_proto *proto);

	/* PSM connection establishment */
	int (*hfp_ips_fully_connected)(ips_epaddr_t *ipsaddr);
	psm2_error_t (*hfp_ips_ipsaddr_set_req_params)(struct ips_proto *proto,
				ips_epaddr_t *ipsaddr,
				const struct ips_connect_reqrep *req);
	psm2_error_t (*hfp_ips_ipsaddr_process_connect_reply)(
                                struct ips_proto *proto,
                                ips_epaddr_t *ipsaddr,
                                const struct ips_connect_reqrep *req);
	void (*hfp_ips_proto_build_connect_message)(struct ips_proto *proto,
				ips_epaddr_t *ipsaddr, uint8_t opcode,
				struct ips_connect_reqrep *req);
	void (*hfp_ips_ipsaddr_init_addressing)(struct ips_proto *proto,
				psm2_epid_t epid, ips_epaddr_t *ipsaddr,
				uint16_t *lidp
				, psmi_gid128_t *gidp
				);
	psm2_error_t (*hfp_ips_ipsaddr_init_connections)(
				struct ips_proto *proto,
				psm2_epid_t epid, ips_epaddr_t *ipsaddr);
	void (*hfp_ips_ipsaddr_free)(ips_epaddr_t *ipsaddr,
				struct ips_proto *proto);
	void (*hfp_ips_flow_init)(struct ips_flow *flow,
				struct ips_proto *proto);
	void (*hfp_ips_ipsaddr_disconnect)(struct ips_proto *proto,
				ips_epaddr_t *ipsaddr);
	psm2_error_t (*hfp_ips_ibta_init)(struct ips_proto *proto);
	psm2_error_t (*hfp_ips_path_rec_init)(struct ips_proto *proto,
				struct ips_path_rec *path_rec,
				struct _ibta_path_rec *response);
	psm2_error_t (*hfp_ips_ptl_pollintr)(psm2_ep_t ep,
				struct ips_recvhdrq *recvq, int fd_pipe,
				int next_timeout,
				uint64_t *pollok, uint64_t *pollcyc);
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
	void (*hfp_gdr_close)(void);
	void* (*hfp_gdr_convert_gpu_to_host_addr)(unsigned long buf,
                                size_t size, int flags, psm2_ep_t ep);
#ifdef PSM_ONEAPI
	void (*hfp_gdr_munmap_gpu_to_host_addr)(unsigned long buf,
                                size_t size, int flags, psm2_ep_t ep);
#endif
#endif /* PSM_CUDA || PSM_ONEAPI */
	/* Given an open context and index, return an error, or the
	 * corresponding pkey for the index as programmed by the SM */
	/* Returns an int, so -1 indicates an error. */
	int (*hfp_get_port_index2pkey)(psm2_ep_t ep, int index);
	int (*hfp_poll_type)(uint16_t poll_type, psm2_ep_t ep);


	psm2_error_t (*hfp_spio_transfer_frame)(struct ips_proto *proto,
				       struct ips_flow *flow, struct ips_scb *scb,
				       uint32_t *payload, uint32_t length,
				       uint32_t isCtrlMsg, uint32_t cksum_valid,
				       uint32_t cksum
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
				, uint32_t is_gpu_payload
#endif
		);
	psm2_error_t (*hfp_transfer_frame)(struct ips_proto *proto,
				       struct ips_flow *flow, struct ips_scb *scb,
				       uint32_t *payload, uint32_t length,
				       uint32_t isCtrlMsg, uint32_t cksum_valid,
				       uint32_t cksum
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
				, uint32_t is_gpu_payload
#endif
		);
	psm2_error_t (*hfp_drain_sdma_completions)(struct ips_proto *proto);
	int (*hfp_get_node_id)(int unit, int *nodep);

#endif /* PSMI_HAL_INST_CNT > 1 || defined(PSM_DEBUG) */
};

/* This is the current psmi_hal_instance, or, NULL if not initialized.
   The HIC should not modify the contents of the HAL instance directly. */
extern psmi_hal_instance_t *psm3_hal_current_hal_instance;

/* Declare functions called by the HAL INSTANCES. */
void psm3_hal_register_instance(psmi_hal_instance_t *);

/* Declare functions that are called by the HIC: */
/* All of these functions return a negative int value to
   indicate failure, or >= 0 for success. */

/* Chooses one of the the psmi_hal_instances that have been
    registered and then initializes it.
    Returns: -PSM_HAL_ERROR_NOT_REGISTERED_HI if no HAL
    INSTANCES are registered, or PSM_HAL_ERROR_INIT_FAILED when
    another failure has occured during initialization. */
int psm3_hal_initialize(int devid_enabled[PTL_MAX_INIT]);

int psm3_hal_finalize(void);

enum psmi_hal_pre_init_cache_func_krnls
{
	psmi_hal_pre_init_cache_func_get_num_units,
	psmi_hal_pre_init_cache_func_get_num_ports,
	psmi_hal_pre_init_cache_func_get_unit_active,
	psmi_hal_pre_init_cache_func_get_port_active,
	psmi_hal_pre_init_cache_func_get_num_contexts,
	psmi_hal_pre_init_cache_func_get_num_free_contexts,
	psmi_hal_pre_init_cache_func_get_default_pkey,
	psmi_hal_pre_init_cache_func_get_port_subnet,
	psmi_hal_pre_init_cache_func_get_unit_pci_bus,
	psmi_hal_pre_init_cache_func_get_unit_device_id,
	psmi_hal_pre_init_cache_func_get_unit_device_version,
	psmi_hal_pre_init_cache_func_get_unit_vendor_id,
	psmi_hal_pre_init_cache_func_get_unit_driver,
};

int psm3_hal_pre_init_cache_func(enum psmi_hal_pre_init_cache_func_krnls k, ...);

/* There are 3 classes of HAL functions:
 * DISPATCH_FUNC - always called via function table, even if only 1 HAL in build
 * DISPATCH_PI - called via pre_init_cache_func which may return a cached
 * 			value and will call via function table only on 1st call
 * DISPATCH - will be inline if only 1 HAL in build, otherwise called via
 * 			function table.
 * Note: functions provided by loopback must be DISPATCH_FUNC or DISPATCH_PI */

/* HAL functons which must be real functions such as those for loopback */
#define PSMI_HAL_DISPATCH_FUNC(KERNEL,...)    ( psm3_hal_current_hal_instance->hfp_ ## KERNEL ( __VA_ARGS__ ))

/* HAL functions whose answers are cached via "pre_init_cache_func" */
#define PSMI_HAL_DISPATCH_PI(KERNEL,...) ( psm3_hal_pre_init_cache_func(psmi_hal_pre_init_cache_func_ ## KERNEL , ##__VA_ARGS__ ) )

/* used for datapath functions which can become inline when only 1 HAL
 * In a debug build we force function calls to confirm not called for loopback
 * HAL (function pointer will be NULL) and aid backtrace for debug
 */
#if PSMI_HAL_INST_CNT == 1 && ! defined(PSM_DEBUG)
#define PSMI_HAL_DISPATCH(KERNEL,...)    ( PSMI_HAL_CAT_INL_SYM(KERNEL) ( __VA_ARGS__ ) )
#else
#define PSMI_HAL_DISPATCH(KERNEL,...)    ( psm3_hal_current_hal_instance->hfp_ ## KERNEL ( __VA_ARGS__ ))

#endif

	/* initialize called directly during psm3_hal_initialize without a
	 * DISPATCH macro needed
	 */

	/* DISPATCH_FUNC */
#define psmi_hal_get_unit_name(...)                          PSMI_HAL_DISPATCH_FUNC(get_unit_name,__VA_ARGS__)
#define psmi_hal_get_port_subnet_name(...)                          PSMI_HAL_DISPATCH_FUNC(get_port_subnet_name,__VA_ARGS__)
#define psmi_hal_get_port_speed(...)                            PSMI_HAL_DISPATCH_FUNC(get_port_speed,__VA_ARGS__)
#define psmi_hal_get_port_lid(...)				PSMI_HAL_DISPATCH_FUNC(get_port_lid,__VA_ARGS__)
#define psmi_hal_mq_init_defaults(...)		                PSMI_HAL_DISPATCH_FUNC(mq_init_defaults,__VA_ARGS__)
#define psmi_hal_ep_open_opts_get_defaults(...)	                PSMI_HAL_DISPATCH_FUNC(ep_open_opts_get_defaults,__VA_ARGS__)
#define psmi_hal_context_initstats(...)				PSMI_HAL_DISPATCH_FUNC(context_initstats,__VA_ARGS__)
#ifdef PSM_CUDA
#define psmi_hal_gdr_open(...)                                  PSMI_HAL_DISPATCH_FUNC(gdr_open,__VA_ARGS__)
#endif
#define psmi_hal_finalize_(...)                                 PSMI_HAL_DISPATCH_FUNC(finalize_,__VA_ARGS__)
#define psmi_hal_identify(...)                                 PSMI_HAL_DISPATCH_FUNC(identify,__VA_ARGS__)

	/* DISPATCH_PI */
#define psmi_hal_get_num_units_(...)                            PSMI_HAL_DISPATCH_PI(get_num_units,##__VA_ARGS__)
#define psmi_hal_get_num_ports_(...)                            PSMI_HAL_DISPATCH_PI(get_num_ports,##__VA_ARGS__)
#define psmi_hal_get_unit_active(...)                           PSMI_HAL_DISPATCH_PI(get_unit_active,__VA_ARGS__)
#define psmi_hal_get_port_active(...)                           PSMI_HAL_DISPATCH_PI(get_port_active,__VA_ARGS__)
#define psmi_hal_get_num_contexts(...)                          PSMI_HAL_DISPATCH_PI(get_num_contexts,__VA_ARGS__)
#define psmi_hal_get_num_free_contexts(...)                     PSMI_HAL_DISPATCH_PI(get_num_free_contexts,__VA_ARGS__)
#define psmi_hal_get_default_pkey(...)			        PSMI_HAL_DISPATCH_PI(get_default_pkey,##__VA_ARGS__)
#define psmi_hal_get_port_subnet(...)				PSMI_HAL_DISPATCH_PI(get_port_subnet,__VA_ARGS__)
#define psmi_hal_get_unit_pci_bus(...)                          PSMI_HAL_DISPATCH_PI(get_unit_pci_bus,__VA_ARGS__)
#define psmi_hal_get_unit_device_id(...)                        PSMI_HAL_DISPATCH_PI(get_unit_device_id,__VA_ARGS__)
#define psmi_hal_get_unit_device_version(...)                   PSMI_HAL_DISPATCH_PI(get_unit_device_version,__VA_ARGS__)
#define psmi_hal_get_unit_vendor_id(...)                        PSMI_HAL_DISPATCH_PI(get_unit_vendor_id,__VA_ARGS__)
#define psmi_hal_get_unit_driver(...)                           PSMI_HAL_DISPATCH_PI(get_unit_driver,__VA_ARGS__)

	/* DISPATCH  - potentially inline */
#define psmi_hal_context_open(...)				PSMI_HAL_DISPATCH(context_open,__VA_ARGS__)
#define psmi_hal_close_context(...)				PSMI_HAL_DISPATCH(close_context,__VA_ARGS__)
#define psmi_hal_context_check_status(...)				PSMI_HAL_DISPATCH(context_check_status,__VA_ARGS__)

#ifdef PSM_FI
#define psmi_hal_faultinj_allowed(...)				PSMI_HAL_DISPATCH(faultinj_allowed,__VA_ARGS__)
#endif
#define psmi_hal_ips_ptl_init_pre_proto_init(...)               PSMI_HAL_DISPATCH(ips_ptl_init_pre_proto_init,__VA_ARGS__)
#define psmi_hal_ips_ptl_init_post_proto_init(...)              PSMI_HAL_DISPATCH(ips_ptl_init_post_proto_init,__VA_ARGS__)
#define psmi_hal_ips_ptl_fini(...)                              PSMI_HAL_DISPATCH(ips_ptl_fini,__VA_ARGS__)
#define psmi_hal_ips_proto_init(...)                            PSMI_HAL_DISPATCH(ips_proto_init,__VA_ARGS__)
#define psmi_hal_ips_proto_update_linkinfo(...)                 PSMI_HAL_DISPATCH(ips_proto_update_linkinfo,__VA_ARGS__)
#define psmi_hal_ips_fully_connected(...)                       PSMI_HAL_DISPATCH(ips_fully_connected,__VA_ARGS__)
#define psmi_hal_ips_ipsaddr_set_req_params(...)                PSMI_HAL_DISPATCH(ips_ipsaddr_set_req_params,__VA_ARGS__)
#define psmi_hal_ips_ipsaddr_process_connect_reply(...)         PSMI_HAL_DISPATCH(ips_ipsaddr_process_connect_reply,__VA_ARGS__)
#define psmi_hal_ips_proto_build_connect_message(...)           PSMI_HAL_DISPATCH(ips_proto_build_connect_message,__VA_ARGS__)
#define psmi_hal_ips_ipsaddr_init_addressing(...)	        PSMI_HAL_DISPATCH(ips_ipsaddr_init_addressing,__VA_ARGS__)
#define psmi_hal_ips_ipsaddr_init_connections(...)	        PSMI_HAL_DISPATCH(ips_ipsaddr_init_connections,__VA_ARGS__)
#define psmi_hal_ips_ipsaddr_free(...)			        PSMI_HAL_DISPATCH(ips_ipsaddr_free,__VA_ARGS__)
#define psmi_hal_ips_flow_init(...)	                        PSMI_HAL_DISPATCH(ips_flow_init,__VA_ARGS__)
#define psmi_hal_ips_ipsaddr_disconnect(...)                    PSMI_HAL_DISPATCH(ips_ipsaddr_disconnect,__VA_ARGS__)
#define psmi_hal_ips_ibta_init(...)                             PSMI_HAL_DISPATCH(ips_ibta_init,__VA_ARGS__)
#define psmi_hal_ips_path_rec_init(...)                         PSMI_HAL_DISPATCH(ips_path_rec_init,__VA_ARGS__)
#define psmi_hal_ips_ptl_pollintr(...)                          PSMI_HAL_DISPATCH(ips_ptl_pollintr,__VA_ARGS__)
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
#define psmi_hal_gdr_close(...)                                 PSMI_HAL_DISPATCH(gdr_close,__VA_ARGS__)
#define psmi_hal_gdr_convert_gpu_to_host_addr(...)              PSMI_HAL_DISPATCH(gdr_convert_gpu_to_host_addr,__VA_ARGS__)
#ifdef PSM_ONEAPI
#define psmi_hal_gdr_munmap_gpu_to_host_addr(...)               PSMI_HAL_DISPATCH(gdr_munmap_gpu_to_host_addr,__VA_ARGS__)
#endif
#endif /* PSM_CUDA || PSM_ONEAPI */

#define psmi_hal_get_port_index2pkey(...)			PSMI_HAL_DISPATCH(get_port_index2pkey,__VA_ARGS__)
#define psmi_hal_poll_type(...)					PSMI_HAL_DISPATCH(poll_type,__VA_ARGS__)


#define psmi_hal_spio_transfer_frame(...)			PSMI_HAL_DISPATCH(spio_transfer_frame,__VA_ARGS__)
#define psmi_hal_transfer_frame(...)				PSMI_HAL_DISPATCH(transfer_frame,__VA_ARGS__)
#define psmi_hal_drain_sdma_completions(...)			PSMI_HAL_DISPATCH(drain_sdma_completions,__VA_ARGS__)
#define psmi_hal_get_node_id(...)				PSMI_HAL_DISPATCH(get_node_id,__VA_ARGS__)


#define psmi_hal_get_hal_instance_index()			psm3_hal_current_hal_instance->hal_index
#define psmi_hal_get_hal_instance_name()			psm3_hal_index_to_str(psm3_hal_current_hal_instance->hal_index)
#define psmi_hal_get_hal_instance_description()			psm3_hal_current_hal_instance->description
#define psmi_hal_get_num_units()				psm3_hal_current_hal_instance->params.num_units
#define psmi_hal_get_num_ports()				psm3_hal_current_hal_instance->params.num_ports
#define psmi_hal_get_cap_mask()					psm3_hal_current_hal_instance->params.cap_mask
#define psmi_hal_set_cap_mask(NEW_MASK)				(psm3_hal_current_hal_instance->params.cap_mask = (NEW_MASK))
#define psmi_hal_add_cap(CAP)					(psm3_hal_current_hal_instance->params.cap_mask |= (CAP))
#define psmi_hal_sub_cap(CAP)					(psm3_hal_current_hal_instance->params.cap_mask &= (~(CAP)))
#define psmi_hal_has_cap(CAP)                                   ((psmi_hal_get_cap_mask() & (CAP)) == (CAP))

#define psmi_hal_get_sw_status()				psm3_hal_current_hal_instance->params.sw_status
#define psmi_hal_set_sw_status(NEW_STATUS)			(psm3_hal_current_hal_instance->params.sw_status = (NEW_STATUS))
#define psmi_hal_add_sw_status(STATUS)				(psm3_hal_current_hal_instance->params.sw_status |= (STATUS))
#define psmi_hal_sub_sw_status(STATUS)				(psm3_hal_current_hal_instance->params.sw_status &= (~(STATUS)))
#define psmi_hal_has_sw_status(STATUS)				((psmi_hal_get_sw_status() & (STATUS)) == (STATUS))


// Defines PSMI_HAL_INLINE.
// If only 1 HAL and not PSM_DEBUG, this will also include the HAL's inline_i.h
// file to declare all the inline HAL functions
#include "psm2_hal_inlines_i.h"

#endif /* #ifndef __PSM2_HAL_H__ */
