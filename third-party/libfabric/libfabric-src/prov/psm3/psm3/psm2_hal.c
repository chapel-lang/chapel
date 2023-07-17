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
#include "psm2_hal_loopback.h"

#include "ptl_ips/ips_scb.h"
#include <fnmatch.h>

static psmi_hal_instance_t *psm3_hal_table[PSM_HAL_INDEX_MAX+1] = { NULL };

/* number of registered HALs (not including loopback */
static unsigned psm3_num_hal = 0;

/* define the current hal instance pointer */
psmi_hal_instance_t *psm3_hal_current_hal_instance = NULL;

/* the loopback instance, for pure intra-node jobs */
extern hfp_loopback_t psm3_loopback_hi;

/* for internal sanity checks, this indicates if we are examinging a HAL
 * in psm3_hal_get_pi_inst so we can detect invalid recursive calls
 * to psmi_hal_* during sysfs_init or have_active_unit.
 */
static int psm3_examining_hal = 0;

/* psm3_hal_register_instance */
void psm3_hal_register_instance(psmi_hal_instance_t *psm_hi)
{
#define REJECT_IMPROPER_HI(MEMBER) if (!psm_hi->MEMBER) return

	/* enforce that HAL function table is fully initialized */
	/* we ignore (aka REJECT) registration of a HAL which lacks a function */

	/* functions called directly, no DISPATCH macro */
	REJECT_IMPROPER_HI(hfp_initialize);
	REJECT_IMPROPER_HI(hfp_have_active_unit);

	/* functions called via DISPATCH_FUNC and must always be in table */
	REJECT_IMPROPER_HI(hfp_finalize_);
	REJECT_IMPROPER_HI(hfp_identify);
	REJECT_IMPROPER_HI(hfp_get_unit_name);
	REJECT_IMPROPER_HI(hfp_get_port_subnet_name);
	REJECT_IMPROPER_HI(hfp_get_port_speed);
	REJECT_IMPROPER_HI(hfp_get_port_lid);
	REJECT_IMPROPER_HI(hfp_mq_init_defaults);
	REJECT_IMPROPER_HI(hfp_ep_open_opts_get_defaults);
	REJECT_IMPROPER_HI(hfp_context_initstats);
#ifdef PSM_CUDA
	REJECT_IMPROPER_HI(hfp_gdr_open);
#endif

	/* functions called via DISPATCH_PI and must always be in table
	 * Note: for loopback HAL a few should never be called and may be NULL
	 * in loopback HAL table.  But loopback HAL never calls
	 * psm3_hal_register_instance
	 */
	REJECT_IMPROPER_HI(hfp_get_num_units);
	REJECT_IMPROPER_HI(hfp_get_num_ports);
	REJECT_IMPROPER_HI(hfp_get_unit_active);
	REJECT_IMPROPER_HI(hfp_get_port_active);
	REJECT_IMPROPER_HI(hfp_get_num_contexts);
	REJECT_IMPROPER_HI(hfp_get_num_free_contexts);
	REJECT_IMPROPER_HI(hfp_get_default_pkey);
	REJECT_IMPROPER_HI(hfp_get_port_subnet);
	REJECT_IMPROPER_HI(hfp_get_unit_pci_bus);
	REJECT_IMPROPER_HI(hfp_get_unit_device_id);
	REJECT_IMPROPER_HI(hfp_get_unit_device_version);
	REJECT_IMPROPER_HI(hfp_get_unit_vendor_id);
	REJECT_IMPROPER_HI(hfp_get_unit_driver);

	/* functions which are inline (not in table) unless > 1 HAL in build 
	 * or a debug build
	 * None of these are used nor provided for loopback HAL as they are
	 * specific to use of ptl_ips with an opened NIC
	 */
#if PSMI_HAL_INST_CNT > 1 || defined(PSM_DEBUG)
	REJECT_IMPROPER_HI(hfp_context_open);
	REJECT_IMPROPER_HI(hfp_close_context);
	REJECT_IMPROPER_HI(hfp_context_check_status);
#ifdef PSM_FI
	REJECT_IMPROPER_HI(hfp_faultinj_allowed);
#endif
	REJECT_IMPROPER_HI(hfp_ips_ptl_init_pre_proto_init);
	REJECT_IMPROPER_HI(hfp_ips_ptl_init_post_proto_init);
	REJECT_IMPROPER_HI(hfp_ips_ptl_fini);
	REJECT_IMPROPER_HI(hfp_ips_proto_init);
	REJECT_IMPROPER_HI(hfp_ips_proto_update_linkinfo);
	REJECT_IMPROPER_HI(hfp_ips_fully_connected);
	REJECT_IMPROPER_HI(hfp_ips_ipsaddr_set_req_params);
	REJECT_IMPROPER_HI(hfp_ips_ipsaddr_process_connect_reply);
	REJECT_IMPROPER_HI(hfp_ips_proto_build_connect_message);
	REJECT_IMPROPER_HI(hfp_ips_ipsaddr_init_addressing);
	REJECT_IMPROPER_HI(hfp_ips_ipsaddr_init_connections);
	REJECT_IMPROPER_HI(hfp_ips_ipsaddr_free);
	REJECT_IMPROPER_HI(hfp_ips_flow_init);
	REJECT_IMPROPER_HI(hfp_ips_ipsaddr_disconnect);
	REJECT_IMPROPER_HI(hfp_ips_ibta_init);
	REJECT_IMPROPER_HI(hfp_ips_path_rec_init);
	REJECT_IMPROPER_HI(hfp_ips_ptl_pollintr);
#if defined(PSM_CUDA) || defined(PSM_ONEAPI)
	REJECT_IMPROPER_HI(hfp_gdr_close);
	REJECT_IMPROPER_HI(hfp_gdr_convert_gpu_to_host_addr);
#ifdef PSM_ONEAPI
	REJECT_IMPROPER_HI(hfp_gdr_munmap_gpu_to_host_addr);
#endif
#endif /* PSM_CUDA || PSM_ONEAPI */
	REJECT_IMPROPER_HI(hfp_get_port_index2pkey);
	REJECT_IMPROPER_HI(hfp_poll_type);

	REJECT_IMPROPER_HI(hfp_spio_transfer_frame);
	REJECT_IMPROPER_HI(hfp_transfer_frame);
	REJECT_IMPROPER_HI(hfp_drain_sdma_completions);
	REJECT_IMPROPER_HI(hfp_get_node_id);


#endif /* PSMI_HAL_INST_CNT > 1 || defined(PSM_DEBUG) */

	psmi_assert(! psm3_hal_table[psm_hi->hal_index]);
	psm3_hal_table[psm_hi->hal_index] = psm_hi;
	psm3_num_hal++;
}

static struct _psmi_hal_instance *psm3_hal_get_pi_inst(void);

int psm3_hal_pre_init_cache_func(enum psmi_hal_pre_init_cache_func_krnls k, ...)
{
	va_list ap;
	va_start(ap, k);

	int rv = 0;
	struct _psmi_hal_instance *p = psm3_hal_get_pi_inst();

	if (!p)
		rv = -1;
	else
	{
		switch(k)
		{
		case psmi_hal_pre_init_cache_func_get_num_units:
			rv = p->params.num_units;
			break;
		case psmi_hal_pre_init_cache_func_get_num_ports:
			rv = p->params.num_ports;
			break;
		case psmi_hal_pre_init_cache_func_get_unit_active:
			{
				int unit = va_arg(ap,int);

				if ((unit >= 0) && (unit < p->params.num_units))
				{
					if (!p->params.unit_active_valid[unit]) {
						p->params.unit_active_valid[unit] = 1;
						p->params.unit_active[unit] = p->hfp_get_unit_active(unit);
					}
					rv = p->params.unit_active[unit];
				}
				else
					rv = -1;
			}
			break;
		case psmi_hal_pre_init_cache_func_get_port_active:
			{
				int unit = va_arg(ap,int);

				if ((unit >= 0) && (unit < p->params.num_units))
				{
					int port = va_arg(ap,int);
					if ((port >= 1) && (port <= p->params.num_ports))
					{
						int i = unit * (p->params.num_ports+1) + port;
						if (!p->params.port_active_valid[i]) {
							p->params.port_active_valid[i] = 1;
							p->params.port_active[i] = p->hfp_get_port_active(unit,port);
						}
						rv = p->params.port_active[i];
					}
					else
						rv = -1;
				}
				else
					rv = -1;
			}
			break;
		case psmi_hal_pre_init_cache_func_get_num_contexts:
			{
				int unit = va_arg(ap,int);
				if ((unit >= 0) && (unit < p->params.num_units))
				{
					if (!p->params.num_contexts_valid[unit]) {
						p->params.num_contexts_valid[unit] = 1;
						p->params.num_contexts[unit] = p->hfp_get_num_contexts(unit);
					}
					rv = p->params.num_contexts[unit];
				}
				else
					rv = -1;
			}
			break;
		case psmi_hal_pre_init_cache_func_get_num_free_contexts:
			{
				int unit = va_arg(ap,int);

				if ((unit >= 0) && (unit < p->params.num_units))
				{
					if (!p->params.num_free_contexts_valid[unit]) {
						p->params.num_free_contexts_valid[unit] = 1;
						p->params.num_free_contexts[unit] = p->hfp_get_num_free_contexts(unit);
					}
					rv = p->params.num_free_contexts[unit];
				}
				else
					rv = -1;
			}
			break;
		case psmi_hal_pre_init_cache_func_get_default_pkey:
			rv = p->params.default_pkey;
			break;
		case psmi_hal_pre_init_cache_func_get_port_subnet:
			{
				int unit = va_arg(ap,int);

				if ((unit >= 0) && (unit < p->params.num_units))
				{
					int port = va_arg(ap,int);
					if ((port >= 1) && (port <= p->params.num_ports))
					{
						int addr_index = va_arg(ap,int);
						if (addr_index >= 0 && addr_index < psm3_addr_per_nic)
						{
							int i = unit * ((p->params.num_ports+1) * psm3_addr_per_nic) + port * psm3_addr_per_nic + addr_index;
							if (!p->params.port_subnet_valid[i]) {
								rv = p->hfp_get_port_subnet(unit, port, addr_index,
									&p->params.port_subnet[i],
									&p->params.port_subnet_addr[i],
									&p->params.port_subnet_idx[i],
									&p->params.port_subnet_gid[i]);
								if (rv == 0)
									p->params.port_subnet_valid[i] = 1;
								else
									p->params.port_subnet_valid[i] = -1;
							}
							psmi_subnet128_t* subnet = va_arg(ap,psmi_subnet128_t*);
							psmi_subnet128_t* addr = va_arg(ap,psmi_subnet128_t*);
							int* idx = va_arg(ap,int*);
							psmi_gid128_t* gid = va_arg(ap,psmi_gid128_t*);
							rv = (p->params.port_subnet_valid[i] ==1)? 0: -1;
							if (rv == 0) {
								if (subnet) *subnet = p->params.port_subnet[i];
								if (addr) *addr = p->params.port_subnet_addr[i];
								if (idx) *idx = p->params.port_subnet_idx[i];
								if (gid) *gid = p->params.port_subnet_gid[i];
							}
						}
						else
							rv = -1;
					}
					else
						rv = -1;
				}
				else
					rv = -1;
			}
			break;
		case psmi_hal_pre_init_cache_func_get_unit_pci_bus:
			{
				int unit = va_arg(ap,int);

				if ((unit >= 0) && (unit < p->params.num_units))
				{
					if (!p->params.unit_pci_bus_valid[unit]) {
						rv = p->hfp_get_unit_pci_bus(unit,
							&p->params.unit_pci_bus_domain[unit],
							&p->params.unit_pci_bus_bus[unit],
							&p->params.unit_pci_bus_device[unit],
							&p->params.unit_pci_bus_function[unit]);

						p->params.unit_pci_bus_valid[unit] = rv == 0 ? 1 : -1;
					}
					uint32_t *domain   = va_arg(ap, uint32_t*);
					uint32_t *bus      = va_arg(ap, uint32_t*);
					uint32_t *device   = va_arg(ap, uint32_t*);
					uint32_t *function = va_arg(ap, uint32_t*);
					rv = p->params.unit_pci_bus_valid[unit] == 1 ? 0 : -1;
					if (rv == 0) {
						if (domain)     *domain = p->params.unit_pci_bus_domain[unit];
						if (bus)           *bus = p->params.unit_pci_bus_bus[unit];
						if (device)     *device = p->params.unit_pci_bus_device[unit];
						if (function) *function = p->params.unit_pci_bus_function[unit];
					}
				}
				else
					rv = -1;
			}
			break;
		case psmi_hal_pre_init_cache_func_get_unit_device_id:
			{
				int unit = va_arg(ap,int);

				if ((unit >= 0) && (unit < p->params.num_units))
				{
					if (!p->params.unit_device_id[unit]) {
						char buffer[PATH_MAX] = {};
						rv = p->hfp_get_unit_device_id(unit, buffer, sizeof(buffer));
						if (rv == 0) {
							p->params.unit_device_id[unit] = psmi_strdup(PSMI_EP_NONE, buffer);
						}
					}
					char *buf      = va_arg(ap, char*);
					size_t bufsize = va_arg(ap, size_t);
					rv = p->params.unit_device_id[unit] ? 0 : -1;
					if (rv == 0 && buf) {
						(void)snprintf(buf, bufsize, "%s", p->params.unit_device_id[unit]);
					}
				} else
					rv = -1;
			}
			break;
		case psmi_hal_pre_init_cache_func_get_unit_device_version:
			{
				int unit = va_arg(ap,int);

				if ((unit >= 0) && (unit < p->params.num_units))
				{
					if (!p->params.unit_device_version[unit]) {
						char buffer[PATH_MAX] = {};
						rv = p->hfp_get_unit_device_version(unit, buffer, sizeof(buffer));
						if (rv == 0) {
							p->params.unit_device_version[unit] = psmi_strdup(PSMI_EP_NONE, buffer);
						}
					}
					char *buf      = va_arg(ap, char*);
					size_t bufsize = va_arg(ap, size_t);
					rv = p->params.unit_device_version[unit] ? 0 : -1;
					if (rv == 0 && buf) {
						(void)snprintf(buf, bufsize, "%s", p->params.unit_device_version[unit]);
					}
				} else
					rv = -1;
			}
			break;
		case psmi_hal_pre_init_cache_func_get_unit_vendor_id:
			{
				int unit = va_arg(ap,int);

				if ((unit >= 0) && (unit < p->params.num_units))
				{
					if (!p->params.unit_vendor_id[unit]) {
						char buffer[PATH_MAX] = {};
						rv = p->hfp_get_unit_vendor_id(unit, buffer, sizeof(buffer));
						if (rv == 0) {
							p->params.unit_vendor_id[unit] = psmi_strdup(PSMI_EP_NONE, buffer);
						}
					}
					char *buf      = va_arg(ap, char*);
					size_t bufsize = va_arg(ap, size_t);
					rv = p->params.unit_vendor_id[unit] ? 0 : -1;
					if (rv == 0 && buf) {
						(void)snprintf(buf, bufsize, "%s", p->params.unit_vendor_id[unit]);
					}
				} else
					rv = -1;
			}
			break;
		case psmi_hal_pre_init_cache_func_get_unit_driver:
			{
				int unit = va_arg(ap,int);

				if ((unit >= 0) && (unit < p->params.num_units))
				{
					if (!p->params.unit_driver[unit]) {
						char buffer[PATH_MAX] = {};
						rv = p->hfp_get_unit_driver(unit, buffer, sizeof(buffer));
						if (rv == 0) {
							p->params.unit_driver[unit] = psmi_strdup(PSMI_EP_NONE, buffer);
						}
					}
					char *buf      = va_arg(ap, char*);
					size_t bufsize = va_arg(ap, size_t);
					rv = p->params.unit_driver[unit] ? 0 : -1;
					if (rv == 0 && buf) {
						(void)snprintf(buf, bufsize, "%s", p->params.unit_driver[unit]);
					}
				} else
					rv = -1;
			}
			break;
		default:
			rv = -1;
			break;
		}
	}

	va_end(ap);
	return rv;
}

static void psm3_hal_free_cache(struct _psmi_hal_instance *p)
{
#define FREE_HAL_CACHE(field) \
	do { \
		if (p->params.field) \
			psmi_free(p->params.field); \
		p->params.field = NULL; \
	} while (0)
#define FREE_HAL_CACHE_ARRAY(field, num) \
	do { \
		if (p->params.field) { \
			int i; \
			for (i = 0; i < num; i++) { \
				if (p->params.field[i]) \
					psmi_free(p->params.field[i]); \
				p->params.field[i] = NULL; \
			} \
			psmi_free(p->params.field); \
		} \
		p->params.field = NULL; \
	} while (0)

	FREE_HAL_CACHE(unit_active);
	FREE_HAL_CACHE(unit_active_valid);
	FREE_HAL_CACHE(port_active);
	FREE_HAL_CACHE(port_active_valid);
	FREE_HAL_CACHE(num_contexts);
	FREE_HAL_CACHE(num_contexts_valid);
	FREE_HAL_CACHE(num_free_contexts);
	FREE_HAL_CACHE(num_free_contexts_valid);
	FREE_HAL_CACHE(port_subnet_valid);
	FREE_HAL_CACHE(port_subnet);
	FREE_HAL_CACHE(port_subnet_addr);
	FREE_HAL_CACHE(port_subnet_idx);
	FREE_HAL_CACHE(port_subnet_gid);

	FREE_HAL_CACHE(unit_pci_bus_valid);
	FREE_HAL_CACHE(unit_pci_bus_domain);
	FREE_HAL_CACHE(unit_pci_bus_bus);
	FREE_HAL_CACHE(unit_pci_bus_device);
	FREE_HAL_CACHE(unit_pci_bus_function);

	FREE_HAL_CACHE_ARRAY(unit_device_id, p->params.num_units);
	FREE_HAL_CACHE_ARRAY(unit_device_version, p->params.num_units);
	FREE_HAL_CACHE_ARRAY(unit_vendor_id, p->params.num_units);
	FREE_HAL_CACHE_ARRAY(unit_driver, p->params.num_units);

#undef FREE_HAL_CACHE_ARRAY
#undef FREE_HAL_CACHE
	p->params.sw_status = 0;
}

/* on success returns p, if unable to select HAL p, returns NULL */
static psmi_hal_instance_t *psm3_hal_select_hal(psmi_hal_instance_t *p,
								 int nunits, int nports, int dflt_pkey)
{
	p->params.num_units = nunits;
	p->params.num_ports = nports;
	p->params.default_pkey = dflt_pkey;

	// unit is 0 to nunits-1
	// port is 1 to nports
	// size extra entry for ports below, entry 0 unused
#define ALLOC_HAL_CACHE(field, type, cnt) \
	do { \
		p->params.field = (type *)psmi_calloc(PSMI_EP_NONE, UNDEFINED, cnt, sizeof(type)); \
		if (! p->params.field) goto fail_cache_alloc; \
	} while (0)
#define ALLOC_HAL_CACHE_ARRAY(field, type, cnt) \
	do { \
		p->params.field = (type **)psmi_calloc(PSMI_EP_NONE, UNDEFINED, cnt, sizeof(type *)); \
		if (! p->params.field) goto fail_cache_alloc; \
	} while (0)

	ALLOC_HAL_CACHE(unit_active, int8_t, nunits);
	ALLOC_HAL_CACHE(unit_active_valid, int8_t, nunits);
	ALLOC_HAL_CACHE(port_active, int8_t, nunits*(nports+1));
	ALLOC_HAL_CACHE(port_active_valid, int8_t, nunits*(nports+1));
	ALLOC_HAL_CACHE(num_contexts, uint16_t, nunits);
	ALLOC_HAL_CACHE(num_contexts_valid, uint16_t, nunits);
	ALLOC_HAL_CACHE(num_free_contexts, uint16_t, nunits);
	ALLOC_HAL_CACHE(num_free_contexts_valid, uint16_t, nunits);
	ALLOC_HAL_CACHE(port_subnet_valid, int8_t, nunits*(nports+1)*psm3_addr_per_nic);
	ALLOC_HAL_CACHE(port_subnet, psmi_subnet128_t, nunits*(nports+1)*psm3_addr_per_nic);
	ALLOC_HAL_CACHE(port_subnet_addr, psmi_naddr128_t, nunits*(nports+1)*psm3_addr_per_nic);
	ALLOC_HAL_CACHE(port_subnet_idx, int, nunits*(nports+1)*psm3_addr_per_nic);
	ALLOC_HAL_CACHE(port_subnet_gid, psmi_gid128_t, nunits*(nports+1)*psm3_addr_per_nic);

	ALLOC_HAL_CACHE(unit_pci_bus_valid, int8_t, nunits);
	ALLOC_HAL_CACHE(unit_pci_bus_domain, uint32_t, nunits);
	ALLOC_HAL_CACHE(unit_pci_bus_bus, uint32_t, nunits);
	ALLOC_HAL_CACHE(unit_pci_bus_device, uint32_t, nunits);
	ALLOC_HAL_CACHE(unit_pci_bus_function, uint32_t, nunits);

	/* char [] NULL means not valid */
	ALLOC_HAL_CACHE_ARRAY(unit_device_id, char, nunits);
	ALLOC_HAL_CACHE_ARRAY(unit_device_version, char, nunits);
	ALLOC_HAL_CACHE_ARRAY(unit_vendor_id, char, nunits);
	ALLOC_HAL_CACHE_ARRAY(unit_driver, char, nunits);
#undef ALLOC_HAL_CACHE_ARRAY
#undef ALLOC_HAL_CACHE

	p->params.sw_status |= PSM_HAL_PARAMS_VALID_DEFAULT_PKEY |
				PSM_HAL_PARAMS_VALID_NUM_UNITS |
				PSM_HAL_PARAMS_VALID_NUM_PORTS | PSM_HAL_PARAMS_VALID_CACHE;
	_HFI_DBG("Selected HAL: %s\n", psm3_hal_index_to_str(p->hal_index));
	return p;

fail_cache_alloc:
	_HFI_ERROR("Unable to allocate memory for HAL cache\n");
	psm3_hal_free_cache(p);
	return NULL;
}

static struct _psmi_hal_instance *psm3_hal_get_pi_inst(void)
{
	int i;
	psmi_hal_instance_t *p = NULL;

	// calls to HAL via psmi_hal_* within sysfs_init or have_active_unit
	// are not allowed.  The assert catches the resulting recursive call.
	psmi_assert(! psm3_examining_hal);
	if (psm3_hal_current_hal_instance)
		return psm3_hal_current_hal_instance;

	if (!psm3_num_hal) {
		_HFI_ERROR("No HALs registered\n");
		return NULL;
	}

	/* At this point, assuming there are multiple HAL INSTANCES that are
	 * registered, and two or more of the HAL INSTANCES are capable
	 * of initialization on a host, the environment variable PSM3_HAL
	 * allows the user to identify the one HAL INSTANCE that is desired to
	 * be used. The default policy is, when the PSM3_HAL is not set, the
	 * first hal instance that successfully initializes is used.
	 * Note: even if there is only 1 HAL, if it doesn't match PSM3_HAL
	 * it will not be used.
	 */

	union psmi_envvar_val env_hal; /* HAL instance preference */
	psm3_getenv("PSM3_HAL",
		    "Hardware Abstraction Layer to use (Default is first HAL"
		    " to find a valid, unfiltered NIC [any])",
		    PSMI_ENVVAR_LEVEL_USER, PSMI_ENVVAR_TYPE_STR,
		    (union psmi_envvar_val)"any", &env_hal);

	for (i=0; i <= PSM_HAL_INDEX_MAX; i++)
	{
		p = psm3_hal_table[i];
		psmi_assert(!p || i != PSM_HAL_INDEX_LOOPBACK); // never pre-registered
		if (! p) {
			if (i != PSM_HAL_INDEX_LOOPBACK)
				_HFI_DBG("Skipping HAL index %d (%s): not registered\n", i,
						psm3_hal_index_to_str(i));
			continue;
		}

		if (0 == strcmp(env_hal.e_str, "any") ||
		    0 == fnmatch(env_hal.e_str,
					psm3_hal_index_to_str(p->hal_index), 0
#ifdef FNM_EXTMATCH
								| FNM_EXTMATCH
#endif
			 ))
		{
			const int valid_flags = PSM_HAL_PARAMS_VALID_DEFAULT_PKEY |
				PSM_HAL_PARAMS_VALID_NUM_UNITS |
				PSM_HAL_PARAMS_VALID_NUM_PORTS | PSM_HAL_PARAMS_VALID_CACHE;

			// TBD - can this ever happen?  we haven't yet picked a HAL
			// but we find its cache has already been initialized?
			if ((p->params.sw_status & valid_flags) == valid_flags)
				return p;

			_HFI_DBG("Examining HAL for active units: %s\n",
					psm3_hal_index_to_str(p->hal_index));
			psm3_examining_hal = 1;
			int nunits = p->hfp_get_num_units();
			int nports = p->hfp_get_num_ports();
			int dflt_pkey = p->hfp_get_default_pkey();
			if (nunits > 0 && nports > 0
			    && (0 == psm3_sysfs_init(p->nic_sys_class_path,
						p->nic_sys_port_path_fmt))
			    && p->hfp_have_active_unit(nunits)
				)
			{
				p = psm3_hal_select_hal(p, nunits, nports, dflt_pkey);
				psm3_examining_hal = 0;
				return p;
			}
			_HFI_DBG("Skipping HAL: %s: No unfiltered active unit\n",
					psm3_hal_index_to_str(p->hal_index));
			psm3_examining_hal = 0;
		}
	}
	_HFI_DBG("No HAL found with unfiltered active units\n");
	return NULL;
}

/* psm3_hal_initialize */
int psm3_hal_initialize(int devid_enabled[PTL_MAX_INIT])
{
	struct _psmi_hal_instance *p = NULL;

	if (! psm3_hal_current_hal_instance) {
		if (! psm3_device_is_enabled(devid_enabled, PTL_DEVID_IPS)) {
			// register the loopback HAL and select it.  Unlike normal HALs
			// we don't call psm3_hal_register_instance because it would enforce
			// all functions must be non-NULL.  But loopback only needs the
			// subset of functions which are called when ptl_ips is disabled
			p = psm3_hal_table[PSM_HAL_INDEX_LOOPBACK] = &psm3_loopback_hi.phi;
			psm3_examining_hal = 1;
			p = psm3_hal_select_hal(p, p->hfp_get_num_units(),
						p->hfp_get_num_ports(), p->hfp_get_default_pkey());
			psm3_examining_hal = 0;
		} else {
			_HFI_DBG("Skipping HAL index %d (%s): PSM3_DEVICES includes 'nic'\n", PSM_HAL_INDEX_LOOPBACK,
				psm3_hal_index_to_str(PSM_HAL_INDEX_LOOPBACK));
		}
	}
	if (!p)
		p = psm3_hal_get_pi_inst();
	if (!p)
		return -PSM_HAL_ERROR_INIT_FAILED;

	int rv = p->hfp_initialize(p, devid_enabled);

	if (!rv)
	{
		psm3_hal_current_hal_instance = p;
		return rv;
	}
	return -PSM_HAL_ERROR_INIT_FAILED;
}

int psm3_hal_finalize(void)
{
	struct _psmi_hal_instance *p = psm3_hal_current_hal_instance;

	int rv = psmi_hal_finalize_();
	psm3_hal_free_cache(p);
	psm3_hal_current_hal_instance = NULL;
	psm3_sysfs_fini();
	return rv;
}
