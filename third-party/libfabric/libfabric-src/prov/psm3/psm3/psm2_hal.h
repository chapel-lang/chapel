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
struct ips_recvhdrq_event;
struct ips_flow;
struct ips_scb;
struct ips_epinfo;
struct ips_message_header;

/* Declare types: */
typedef enum
{
	PSM_HAL_INSTANCE_ANY_GEN =  0,
	PSM_HAL_INSTANCE_GEN1    =  1,
	PSM_HAL_INSTANCE_GEN2    =  2,
	PSM_HAL_INSTANCE_GEN3    =  3,

#ifdef PSM2_MOCK_TESTING
	PSM_HAL_INSTANCE_MOCK    = 99,
#endif
} psmi_hal_instance_type;

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
   cap_mask member of the psmi_hal_params_t. */
typedef enum
{
	PSM_HAL_CAP_GPUDIRECT_OT		= (1UL << 16),
	PSM_HAL_CAP_USER_MR			= (1UL << 17),
	PSM_HAL_CAP_EVICT			= (1UL << 18),
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
	uint64_t   *port_subnet;
	uint64_t   *port_subnet_addr;
	uint32_t   *port_ip_addr;
	uint32_t   *port_netmask;
	int        *port_subnet_idx;
	uint64_t   *port_subnet_gid_hi;
	uint64_t   *port_subnet_gid_lo;
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
	SLIST_ENTRY(_psmi_hal_instance) next_hi;
	psmi_hal_instance_type		type;
	const char			*description;
	const char			*hfi_name;
	const char			*hfi_sys_class_path;
	/* The params member should be read-only for HIC, and
	   written only by the HAL instance. */
	psmi_hal_params_t		params;
	/* Initialize the HAL INSTANCE. */
	int (*hfp_initialize)(psmi_hal_instance_t *);
	/* Finalize the HAL INSTANCE. */
	int (*hfp_finalize_)(void);

	/* Returns the number of hfi units installed on ths host:
	   NOTE: hfp_get_num_units is a function that must
	   be callable before the hal instance is initialized. */
	int (*hfp_get_num_units)(void);

	/* Returns the number of ports on each hfi unit installed.
	   on ths host.
	   NOTE: hfp_get_num_ports is a function that must
	   be callable before the hal instance is initialized. */
	int (*hfp_get_num_ports)(void);

	/* Returns the default pkey:
	   NOTE: hfp_get_default_pkey is a function that must
	   be callable before the hal instance is initialized. */
	int (*hfp_get_default_pkey)(void);

	/* Given a unit number, returns 1 if any port on the unit is active.
	   returns 0 if no port on the unit is active.
	   returns -1 when an error occurred.
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

	/* Context open includes opening the device file, and get hw params. */
	int (*hfp_context_open)(int unit,
				int port,
				uint64_t open_timeout,
				psm2_ep_t ep,
				psm2_uuid_t const job_key,
				psmi_context_t *psm_ctxt,
				uint32_t cap_mask,
				unsigned retryCnt);

	/* Initialize PSM3_PRINT_STATS stats for given ep */
	void (*hfp_context_initstats)(psm2_ep_t ep);

	/* Close the context, including the device file. */
	int (*hfp_close_context)(psmi_hal_hw_context *);


	int (*hfp_get_port_rate)(int unit, int port);


	int (*hfp_get_port_lid)(int unit, int port);
	int (*hfp_get_port_subnet)(int unit, int port,
				uint64_t *subnet, uint64_t *addr,
				uint32_t *ip_addr, uint32_t *netmask,
				int *idx, uint64_t *hi, uint64_t *lo);


	/* End of receive functions. */


	int (*hfp_spio_transfer_frame)(struct ips_proto *proto,
				       struct ips_flow *flow, struct ips_scb *scb,
				       uint32_t *payload, uint32_t length,
				       uint32_t isCtrlMsg, uint32_t cksum_valid,
				       uint32_t cksum, psmi_hal_hw_context
#ifdef PSM_CUDA
				, uint32_t is_cuda_payload
#endif
		);
	int (*hfp_spio_process_events)(const struct ptl *ptl);
	int (*hfp_get_node_id)(int unit, int *nodep);


	int      (*hfp_get_jkey)(psmi_hal_hw_context);

};

/* This is the current psmi_hal_instance, or, NULL if not initialized.
   The HIC should not modify the contents of the HAL instance directly. */
extern psmi_hal_instance_t *psmi_hal_current_hal_instance;

/* Declare functions called by the HAL INSTANCES. */
void psmi_hal_register_instance(psmi_hal_instance_t *);

/* Declare functions that are called by the HIC: */
/* All of these functions return a negative int value to
   indicate failure, or >= 0 for success. */

/* Chooses one of the the psmi_hal_instances that have been
    registered and then initializes it.
    Returns: -PSM_HAL_ERROR_NOT_REGISTERED_HI if no HAL
    INSTANCES are registered, or PSM_HAL_ERROR_INIT_FAILED when
    another failure has occured during initialization. */
int psmi_hal_initialize(void);

int psmi_hal_finalize(void);

#include "psm2_hal_inlines_d.h"

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
};

int psmi_hal_pre_init_cache_func(enum psmi_hal_pre_init_cache_func_krnls k, ...);

#define PSMI_HAL_DISPATCH_PI(KERNEL,...) ( psmi_hal_pre_init_cache_func(psmi_hal_pre_init_cache_func_ ## KERNEL , ##__VA_ARGS__ ) )

#if PSMI_HAL_INST_CNT == 1

#define PSMI_HAL_DISPATCH(KERNEL,...)    ( PSMI_HAL_CAT_INL_SYM(KERNEL) ( __VA_ARGS__ ) )

#else

#define PSMI_HAL_DISPATCH(KERNEL,...)    ( psmi_hal_current_hal_instance->hfp_ ## KERNEL ( __VA_ARGS__ ))

#endif

#define psmi_hal_get_num_units_(...)                           PSMI_HAL_DISPATCH_PI(get_num_units,##__VA_ARGS__)
#define psmi_hal_get_num_ports_(...)                           PSMI_HAL_DISPATCH_PI(get_num_ports,##__VA_ARGS__)
#define psmi_hal_get_unit_active(...)                          PSMI_HAL_DISPATCH_PI(get_unit_active,__VA_ARGS__)
#define psmi_hal_get_port_active(...)                          PSMI_HAL_DISPATCH_PI(get_port_active,__VA_ARGS__)
#define psmi_hal_get_num_contexts(...)                         PSMI_HAL_DISPATCH_PI(get_num_contexts,__VA_ARGS__)
#define psmi_hal_get_num_free_contexts(...)                    PSMI_HAL_DISPATCH_PI(get_num_free_contexts,__VA_ARGS__)
#define psmi_hal_get_default_pkey(...)			       PSMI_HAL_DISPATCH_PI(get_default_pkey,##__VA_ARGS__)
#define psmi_hal_get_port_subnet(...)				PSMI_HAL_DISPATCH_PI(get_port_subnet,__VA_ARGS__)
#define psmi_hal_context_open(...)				PSMI_HAL_DISPATCH(context_open,__VA_ARGS__)
#define psmi_hal_context_initstats(...)				PSMI_HAL_DISPATCH(context_initstats,__VA_ARGS__)
#define psmi_hal_close_context(...)				PSMI_HAL_DISPATCH(close_context,__VA_ARGS__)


#define psmi_hal_get_port_rate(...)				PSMI_HAL_DISPATCH(get_port_rate,__VA_ARGS__)


#define psmi_hal_get_port_lid(...)				PSMI_HAL_DISPATCH(get_port_lid,__VA_ARGS__)


#define psmi_hal_finalize_(...)                                 PSMI_HAL_DISPATCH(finalize_,__VA_ARGS__)


#define psmi_hal_get_user_major_bldtime_version(...)		PSMI_HAL_DISPATCH(get_user_major_bldtime_version,__VA_ARGS__)
#define psmi_hal_get_user_minor_bldtime_version(...)		PSMI_HAL_DISPATCH(get_user_minor_bldtime_version,__VA_ARGS__)


#define psmi_hal_spio_transfer_frame(...)			PSMI_HAL_DISPATCH(spio_transfer_frame,__VA_ARGS__)
#define psmi_hal_spio_process_events(...)			PSMI_HAL_DISPATCH(spio_process_events,__VA_ARGS__)
#define psmi_hal_get_node_id(...)				PSMI_HAL_DISPATCH(get_node_id,__VA_ARGS__)


#define psmi_hal_get_jkey(...)					PSMI_HAL_DISPATCH(get_jkey,__VA_ARGS__)


#define psmi_hal_get_hal_instance_type()			psmi_hal_current_hal_instance->type
#define psmi_hal_get_hal_instance_description()			psmi_hal_current_hal_instance->description
#define psmi_hal_get_hfi_name()					psmi_hal_current_hal_instance->hfi_name
#define psmi_hal_get_num_units()				psmi_hal_current_hal_instance->params.num_units
#define psmi_hal_get_num_ports()				psmi_hal_current_hal_instance->params.num_ports
#define psmi_hal_get_cap_mask()					psmi_hal_current_hal_instance->params.cap_mask
#define psmi_hal_set_cap_mask(NEW_MASK)				(psmi_hal_current_hal_instance->params.cap_mask = (NEW_MASK))
#define psmi_hal_add_cap(CAP)					(psmi_hal_current_hal_instance->params.cap_mask |= (CAP))
#define psmi_hal_sub_cap(CAP)					(psmi_hal_current_hal_instance->params.cap_mask &= (~(CAP)))
#define psmi_hal_has_cap(CAP)                                   ((psmi_hal_get_cap_mask() & (CAP)) == (CAP))

#define psmi_hal_get_sw_status()				psmi_hal_current_hal_instance->params.sw_status
#define psmi_hal_set_sw_status(NEW_STATUS)			(psmi_hal_current_hal_instance->params.sw_status = (NEW_STATUS))
#define psmi_hal_add_sw_status(STATUS)				(psmi_hal_current_hal_instance->params.sw_status |= (STATUS))
#define psmi_hal_sub_sw_status(STATUS)				(psmi_hal_current_hal_instance->params.sw_status &= (~(STATUS)))
#define psmi_hal_has_sw_status(STATUS)				((psmi_hal_get_sw_status() & (STATUS)) == (STATUS))


#include "psm2_hal_inlines_i.h"

#endif /* #ifndef __PSM2_HAL_H__ */
