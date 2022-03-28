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

#include "ptl_ips/ips_scb.h"

static SLIST_HEAD(, _psmi_hal_instance) head_hi;

/* define the current hal instance pointer */
psmi_hal_instance_t *psmi_hal_current_hal_instance = NULL;

/* psmi_hal_register_instance */
void psmi_hal_register_instance(psmi_hal_instance_t *psm_hi)
{
#define REJECT_IMPROPER_HI(MEMBER) if (!psm_hi->MEMBER) return

	/* If an attempt to register a hal instance contains a NULL func ptr, reject it. */
	/* To allow fast lookups, please keep this code segment alphabetized by hfp_*
	   func ptr member name: */
#if PSMI_HAL_INST_CNT > 1

	REJECT_IMPROPER_HI(hfp_close_context);
	REJECT_IMPROPER_HI(hfp_context_open);
	REJECT_IMPROPER_HI(hfp_context_initstats);


	REJECT_IMPROPER_HI(hfp_finalize_);


	REJECT_IMPROPER_HI(hfp_get_jkey);


	REJECT_IMPROPER_HI(hfp_get_node_id);
	REJECT_IMPROPER_HI(hfp_get_num_contexts);
	REJECT_IMPROPER_HI(hfp_get_num_free_contexts);


	REJECT_IMPROPER_HI(hfp_get_port_active);
	REJECT_IMPROPER_HI(hfp_get_port_subnet);


	REJECT_IMPROPER_HI(hfp_get_port_lid);


	REJECT_IMPROPER_HI(hfp_get_port_rate);


	REJECT_IMPROPER_HI(hfp_get_unit_active);


	REJECT_IMPROPER_HI(hfp_spio_process_events);
	REJECT_IMPROPER_HI(hfp_spio_transfer_frame);

#endif // PSMI_HAL_INST_CNT > 1
	REJECT_IMPROPER_HI(hfp_get_default_pkey);
	REJECT_IMPROPER_HI(hfp_get_num_ports);
	REJECT_IMPROPER_HI(hfp_get_num_units);
	REJECT_IMPROPER_HI(hfp_initialize);

#ifndef PSM2_MOCK_TESTING
	if (!sysfs_init(psm_hi->hfi_sys_class_path))
#endif
		SLIST_INSERT_HEAD(&head_hi, psm_hi, next_hi);
}

static struct _psmi_hal_instance *psmi_hal_get_pi_inst(void);

int psmi_hal_pre_init_cache_func(enum psmi_hal_pre_init_cache_func_krnls k, ...)
{
	va_list ap;
	va_start(ap, k);

	int rv = 0;
	struct _psmi_hal_instance *p = psmi_hal_get_pi_inst();

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
						if (!p->params.port_active_valid[unit*port]) {
							p->params.port_active_valid[unit*port] = 1;
							p->params.port_active[unit*port] = p->hfp_get_port_active(unit,port);
						}
						rv = p->params.port_active[unit*port];
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
						if (!p->params.port_subnet_valid[unit*port]) {
							rv = p->hfp_get_port_subnet(unit, port,
									&p->params.port_subnet[unit*port],
									&p->params.port_subnet_addr[unit*port],
									&p->params.port_ip_addr[unit*port],
									&p->params.port_netmask[unit*port],
									&p->params.port_subnet_idx[unit*port],
									&p->params.port_subnet_gid_hi[unit*port],
									&p->params.port_subnet_gid_lo[unit*port]);
							if (rv == 0)
								p->params.port_subnet_valid[unit*port] = 1;
							else
								p->params.port_subnet_valid[unit*port] = -1;
						}
						uint64_t* subnet = va_arg(ap,uint64_t*);
						uint64_t* addr = va_arg(ap,uint64_t*);
						uint32_t* ip_addr = va_arg(ap,uint32_t*);
						uint32_t* netmask = va_arg(ap,uint32_t*);
						int* idx = va_arg(ap,int*);
						uint64_t* hi = va_arg(ap,uint64_t*);
						uint64_t* lo = va_arg(ap,uint64_t*);
						rv = (p->params.port_subnet_valid[unit*port] ==1)? 0: -1;
						if (subnet) *subnet = p->params.port_subnet[unit*port];
						if (addr) *addr = p->params.port_subnet_addr[unit*port];
						if (ip_addr) *ip_addr = p->params.port_ip_addr[unit*port];
						if (netmask) *netmask = p->params.port_netmask[unit*port];
						if (idx) *idx = p->params.port_subnet_idx[unit*port];
						if (hi) *hi = p->params.port_subnet_gid_hi[unit*port];
						if (lo) *lo = p->params.port_subnet_gid_lo[unit*port];
					}
					else
						rv = -1;
				}
				else
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

static void psmi_hal_free_cache(struct _psmi_hal_instance *p)
{
#define FREE_HAL_CACHE(field) \
	do { \
		if (p->params.field) \
			psmi_free(p->params.field); \
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
	FREE_HAL_CACHE(port_ip_addr);
	FREE_HAL_CACHE(port_netmask);
	FREE_HAL_CACHE(port_subnet_idx);
	FREE_HAL_CACHE(port_subnet_gid_hi);
	FREE_HAL_CACHE(port_subnet_gid_lo);
#undef FREE_HAL_CACHE
	p->params.sw_status = 0;
}

static struct _psmi_hal_instance *psmi_hal_get_pi_inst(void)
{

	if (psmi_hal_current_hal_instance)
		return psmi_hal_current_hal_instance;

	if (SLIST_EMPTY(&head_hi))
		return NULL;

	/* At this point, assuming there are multiple HAL INSTANCES that are
	   registered, and two or more of the HAL INSTANCES are capable
	   of initialization on a host, the environment variable PSM3_HAL_PREF
	   allows the user to identify the one HAL INSTANCE that is desired to
	   be used. The default policy is, when the PSM3_HAL_PREF is not set, the
	   first hal instance that successfully initializes is used. */

	union psmi_envvar_val env_hi_pref; /* HAL instance preference */
	psmi_getenv("PSM3_HAL_PREF",
		    "Indicate preference for HAL instance (Default is use first HAL"
		    " instance to successfully initialize))",
		    PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_INT,
		    (union psmi_envvar_val)PSM_HAL_INSTANCE_ANY_GEN, &env_hi_pref);

	/* The hfp_get_num_units() call below, will not wait for the HFI driver
	   to come up and create device nodes in /dev/.) */
	struct _psmi_hal_instance *p;
	SLIST_FOREACH(p, &head_hi, next_hi)
	{
		if ((env_hi_pref.e_int == PSM_HAL_INSTANCE_ANY_GEN) ||
		    (p->type == env_hi_pref.e_int))
		{
			const int valid_flags = PSM_HAL_PARAMS_VALID_DEFAULT_PKEY |
				PSM_HAL_PARAMS_VALID_NUM_UNITS |
				PSM_HAL_PARAMS_VALID_NUM_PORTS | PSM_HAL_PARAMS_VALID_CACHE;

			if ((p->params.sw_status & valid_flags) == valid_flags)
				return p;

			int nunits = p->hfp_get_num_units();
			int nports = p->hfp_get_num_ports();
			int dflt_pkey = p->hfp_get_default_pkey();
			if (nunits > 0 && nports > 0
#ifndef PSM2_MOCK_TESTING
			    && (0 == sysfs_init(p->hfi_sys_class_path))
#endif
				)
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

				ALLOC_HAL_CACHE(unit_active, int8_t, nunits);
				ALLOC_HAL_CACHE(unit_active_valid, int8_t, nunits);
				ALLOC_HAL_CACHE(port_active, int8_t, nunits*(nports+1));
				ALLOC_HAL_CACHE(port_active_valid, int8_t, nunits*(nports+1));
				ALLOC_HAL_CACHE(num_contexts, uint16_t, nunits);
				ALLOC_HAL_CACHE(num_contexts_valid, uint16_t, nunits);
				ALLOC_HAL_CACHE(num_free_contexts, uint16_t, nunits);
				ALLOC_HAL_CACHE(num_free_contexts_valid, uint16_t, nunits);
				ALLOC_HAL_CACHE(port_subnet_valid, int8_t, nunits*(nports+1));
				ALLOC_HAL_CACHE(port_subnet, uint64_t, nunits*(nports+1));
				ALLOC_HAL_CACHE(port_subnet_addr, uint64_t, nunits*(nports+1));
				ALLOC_HAL_CACHE(port_ip_addr, uint32_t, nunits*(nports+1));
				ALLOC_HAL_CACHE(port_netmask, uint32_t, nunits*(nports+1));
				ALLOC_HAL_CACHE(port_subnet_idx, int, nunits*(nports+1));
				ALLOC_HAL_CACHE(port_subnet_gid_hi, uint64_t, nunits*(nports+1));
				ALLOC_HAL_CACHE(port_subnet_gid_lo, uint64_t, nunits*(nports+1));
				p->params.sw_status |= valid_flags;
#undef ALLOC_HAL_CACHE
				return p;
			}
		}
	}
	return NULL;

fail_cache_alloc:
	psmi_hal_free_cache(p);
	return NULL;
}

/* psmi_hal_initialize */
int psmi_hal_initialize(void)
{
	struct _psmi_hal_instance *p = psmi_hal_get_pi_inst();

	if (!p)
		return -PSM_HAL_ERROR_INIT_FAILED;

	int rv = p->hfp_initialize(p);

	if (!rv)
	{
		psmi_hal_current_hal_instance = p;


		return rv;
	}
	return -PSM_HAL_ERROR_INIT_FAILED;
}

int psmi_hal_finalize(void)
{
	struct _psmi_hal_instance *p = psmi_hal_current_hal_instance;

	int rv = psmi_hal_finalize_();
	psmi_hal_free_cache(p);
	psmi_hal_current_hal_instance = NULL;
	sysfs_fini();
	return rv;
}


#ifdef PSM2_MOCK_TESTING

#include "psm_hal_gen1/opa_user_gen1.h"



#endif
