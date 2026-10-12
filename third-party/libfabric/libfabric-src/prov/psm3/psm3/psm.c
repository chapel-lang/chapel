/*

  This file is provided under a dual BSD/GPLv2 license.  When using or
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2016 Intel Corporation.

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

/* Copyright (c) 2003-2016 Intel Corporation. All rights reserved. */

#include <ctype.h>
#include "psm_user.h"
#include "psm2_hal.h"
#include "psm_mq_internal.h"

static int psm3_verno_major = PSM2_VERNO_MAJOR;
static int psm3_verno_minor = PSM2_VERNO_MINOR;
static int psm3_verno = PSMI_VERNO_MAKE(PSM2_VERNO_MAJOR, PSM2_VERNO_MINOR);
static int psm3_verno_client_val;
uint8_t  psm3_addr_fmt;	// PSM3_ADDR_FMT
int psm3_allow_routers;	// PSM3_ALLOW_ROUTERS

char *psm3_allow_subnets[PSMI_MAX_SUBNETS];	// PSM3_SUBNETS
int psm3_num_allow_subnets;
unsigned int psm3_addr_per_nic = 1;
unsigned int psm3_reg_mr_fail_limit = 100;
unsigned int psm3_reg_mr_warn_cnt = 10;

const char *psm3_nic_wildcard = NULL;

const char *psm3_nic_speed_wildcard = NULL;
uint64_t psm3_nic_speed_max_found = 0;

// Special psmi_refcount values
#define PSMI_NOT_INITIALIZED    0
#define PSMI_FINALIZED         -1

// PSM2 doesn't support transitioning out of the PSMI_FINALIZED state
// once psmi_refcount is set to PSMI_FINALIZED, any further attempts to change
// psmi_refcount should be treated as an error
static int psmi_refcount = PSMI_NOT_INITIALIZED;

/* Global lock used for endpoint creation and destroy
 * (in functions psm3_ep_open and psm3_ep_close) and also
 * for synchronization with recv_thread (so that recv_thread
 * will not work on an endpoint which is in a middle of closing). */
psmi_lock_t psm3_creation_lock;

int psm3_affinity_semaphore_open = 0;
char *psm3_sem_affinity_shm_rw_name;
sem_t *psm3_sem_affinity_shm_rw = NULL;

int psm3_affinity_shared_file_opened = 0;
char *psm3_affinity_shm_name;
uint64_t *psm3_shared_affinity_ptr;
uint64_t *psm3_shared_affinity_nic_refcount_ptr;

/*
 * Bit field that contains capability set.
 * Each bit represents different capability.
 * It is supposed to be filled with logical OR
 * on conditional compilation basis
 * along with future features/capabilities.
 */
uint64_t psm3_capabilities_bitset = PSM2_MULTI_EP_CAP | PSM2_LIB_REFCOUNT_CAP;

int psm3_verno_client()
{
	return psm3_verno_client_val;
}

/* This function is used to determine whether the current library build can
 * successfully communicate with another library that claims to be version
 * 'verno'.
 *
 * PSM 2.x is always ABI compatible, but this checks to see if two different
 * versions of the library can coexist.
 */
int psm3_verno_isinteroperable(uint16_t verno)
{
	if (PSMI_VERNO_GET_MAJOR(verno) != PSM2_VERNO_MAJOR)
		return 0;

	return 1;
}

int MOCKABLE(psm3_isinitialized)()
{
	return (psmi_refcount > 0);
}
MOCK_DEF_EPILOGUE(psm3_isinitialized);

static
void
psmi_free_subnets(void)
{
	int i;

	// test pointer but not psm3_num_allow_subnets so can cleanup failed parse
	for (i=0; i < PSMI_MAX_SUBNETS && psm3_allow_subnets[i]; i++) {
		psmi_free(psm3_allow_subnets[i]);
		psm3_allow_subnets[i] = NULL;
	}
	psm3_num_allow_subnets = 0;
}

/* parse PSM3_SUBNETS to get a list of subnets we'll consider */
static
psm2_error_t
psmi_parse_subnets(const char *subnets)
{
	char *tempstr = NULL;
	char *e, *ee, *b;
	psm2_error_t err = PSM2_OK;
	int len;
	int i = 0;

	psmi_assert_always(subnets != NULL);
	len = strlen(subnets) + 1;

	tempstr = (char *)psmi_calloc(PSMI_EP_NONE, UNDEFINED, 1, len);
	if (tempstr == NULL)
		goto fail;

	strncpy(tempstr, subnets, len);
	ee = tempstr + len;	// very end of subnets string
	for (e=tempstr, i=0; e < ee && *e && i < PSMI_MAX_SUBNETS; e++) {
		char *p;

		while (*e && isspace(*e))
			e++;
		b = e;	// begining of subnet
		while (*e && *e != ',' )
			e++;
		*e = '\0';	// mark end
		// skip whitespace at end of subnet
		for (p = e-1; p >= b && isspace(*p); p--)
			*p = '\0';
		if (*b) {
			psm3_allow_subnets[i] = psmi_strdup(PSMI_EP_NONE, b);
			if (! psm3_allow_subnets[i]) {
				err = PSM2_NO_MEMORY;
				psmi_free_subnets();
				goto fail;
			}
			_HFI_DBG("PSM3_SUBNETS Entry %d = '%s'\n",
					i, psm3_allow_subnets[i]);
			i++;
		}
	}
	if ( e < ee && *e)
		_HFI_INFO("More than %d entries in PSM3_SUBNETS, ignoring extra entries\n", PSMI_MAX_SUBNETS);
	psm3_num_allow_subnets = i;
	_HFI_DBG("PSM3_SUBNETS Num subnets = %d\n", psm3_num_allow_subnets);
fail:
	if (tempstr != NULL)
		psmi_free(tempstr);
	return err;
}

static
void psmi_parse_nic_var()
{
	union psmi_envvar_val env_nic;
	psm3_getenv("PSM3_NIC",
		"Device(s) to consider for use.  By name ("
#ifdef FNM_EXTMATCH
		"extended "
#endif
		"glob pattern), unit number or 'any'",
		PSMI_ENVVAR_LEVEL_USER, PSMI_ENVVAR_TYPE_STR,
		(union psmi_envvar_val)"any", &env_nic);
	//autodetect
	if (0 == strcasecmp(env_nic.e_str, "any")) {
		//so this disables filtering
		psm3_nic_wildcard = NULL;
		return;
	}
	char *endptr = NULL;
	int unit = strtol(env_nic.e_str, &endptr, 10);
	//Unit decimal number
	if ((env_nic.e_str != endptr)&&(*endptr == '\0'))
	{
		//filter equals device name
		psm3_nic_wildcard = psm3_sysfs_unit_dev_name(unit);
		return;
	}
	unit = strtol(env_nic.e_str, &endptr, 16);
	//Unit hex number
	if ((env_nic.e_str != endptr)&&(*endptr == '\0'))
	{
		//filter equals device name
		psm3_nic_wildcard = psm3_sysfs_unit_dev_name(unit);
		return;
	}
	//Unit name or wildcard
	psm3_nic_wildcard = env_nic.e_str;
}

#if defined(PSM_DEBUG) || defined(PSM_PROFILE) || defined(PSM_FI)
static int psm3_parse_no_warn(void)
{
	union psmi_envvar_val envval;
	static int have_value = 0;
	static int saved;

	// only parse once so doesn't appear in PSM3_VERBOSE_ENV multiple times
	if (have_value)
		return saved;

	psm3_getenv("PSM3_NO_WARN", "Suppress warnings for debug builds and features",
            PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_YESNO,
            (union psmi_envvar_val)0 /* Disabled by default */, &envval);
	saved = envval.e_int;
	have_value = 1;
	return saved;
}
#endif

int init_cache_on = 1;
void psm3_turn_off_init_cache() {
	init_cache_on = 0;
}

psm2_error_t psm3_init(int *major, int *minor)
{
	psm2_error_t err = PSM2_OK;
	union psmi_envvar_val env_tmask;
	int devid_enabled[PTL_MAX_INIT];

	if (psm3_env_initialize())	// no need to uninitialize
		goto fail;

	psm3_stats_initialize();

	psm3_mem_stats_register();

	psmi_log_initialize();		// no need to uninitialize

	PSM2_LOG_MSG("entering");

	/* When PSM_PERF is enabled, the following code causes the
	   PMU to be programmed to measure instruction cycles of the
	   TX/RX speedpaths of PSM. */
	GENERIC_PERF_INIT();
	GENERIC_PERF_SET_SLOT_NAME(PSM_TX_SPEEDPATH_CTR, "TX");
	GENERIC_PERF_SET_SLOT_NAME(PSM_RX_SPEEDPATH_CTR, "RX");

	if (psmi_refcount > 0) {
		psmi_refcount++;
		goto update;
	}

	if (psmi_refcount == PSMI_FINALIZED) {
		err = PSM2_IS_FINALIZED;
		goto fail_stats;
	}

	if (major == NULL || minor == NULL) {
		err = PSM2_PARAM_ERR;
		goto fail_stats;
	}

	psmi_init_lock(&psm3_creation_lock);

#ifdef PSM_DEBUG
	if (!psm3_parse_no_warn()) {
		_HFI_ERROR(
			"!!! WARNING !!! YOU ARE RUNNING AN INTERNAL-ONLY PSM *DEBUG* BUILD.\n");
		fprintf(stderr,
			"!!! WARNING !!! YOU ARE RUNNING AN INTERNAL-ONLY PSM *DEBUG* BUILD.\n");
	}
	psm3_stats_print_msg(
			"!!! WARNING !!! YOU ARE RUNNING AN INTERNAL-ONLY PSM *DEBUG* BUILD.\n");
#endif

#ifdef PSM_PROFILE
	if (!psm3_parse_no_warn()) {
		_HFI_ERROR(
			"!!! WARNING !!! YOU ARE RUNNING AN INTERNAL-ONLY PSM *PROFILE* BUILD.\n");
		fprintf(stderr,
			"!!! WARNING !!! YOU ARE RUNNING AN INTERNAL-ONLY PSM *PROFILE* BUILD.\n");
	}
	psm3_stats_print_msg(
			"!!! WARNING !!! YOU ARE RUNNING AN INTERNAL-ONLY PSM *PROFILE* BUILD.\n");
#endif

	/* Make sure, as an internal check, that this version knows how to detect
	 * compatibility with other library versions it may communicate with */
	if (psm3_verno_isinteroperable(psm3_verno) != 1) {
		err = psm3_handle_error(PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,
					"psm3_verno_isinteroperable() not updated for current version!");
		goto fail_stats;
	}

	/* The only way to not support a client is if the major number doesn't
	 * match */
	if (*major != PSM2_VERNO_MAJOR && *major != PSM2_VERNO_COMPAT_MAJOR) {
		err = psm3_handle_error(NULL, PSM2_INIT_BAD_API_VERSION,
					"This library does not implement version %d.%d",
					*major, *minor);
		goto fail_stats;
	}

	/* Make sure we don't keep track of a client that claims a higher version
	 * number than we are */
	psm3_verno_client_val =
	    min(PSMI_VERNO_MAKE(*major, *minor), psm3_verno);

	psmi_refcount++;
	/* psm3_dbgmask lives in libhfi.so */
	psm3_getenv("PSM3_TRACEMASK",
		    "Mask flags for tracing",
		    PSMI_ENVVAR_LEVEL_USER,
		    PSMI_ENVVAR_TYPE_STR_VAL_PAT_UINT_FLAGS,
		    (union psmi_envvar_val)__HFI_DEBUG_DEFAULT_STR, &env_tmask);
	(void)psm3_parse_val_pattern_uint(env_tmask.e_str, __HFI_DEBUG_DEFAULT,
		    &psm3_dbgmask, PSMI_ENVVAR_FLAG_NOMIN_NOMAX, 0, UINT_MAX);

	/* The "real thing" is done in utils_mallopt.c as constructor function, but
	 * we getenv it here to report what we're doing with the setting */
	{
		extern int psm3_malloc_no_mmap;
		union psmi_envvar_val env_mmap;
		// real parsing was in a constructor so can't use psm3_env_get() here.
		// psm3_getenv call is just for logging and checking if mallopt failed
		psm3_getenv("PSM3_DISABLE_MMAP_MALLOC",
			    "Disable mmap for malloc()",
			    PSMI_ENVVAR_LEVEL_USER,
			    PSMI_ENVVAR_TYPE_YESNO,
			    (union psmi_envvar_val)0, &env_mmap);
		if (env_mmap.e_int != psm3_malloc_no_mmap)
			_HFI_ERROR
			    ("Couldn't successfully disable mmap in mallocs "
			     "with mallopt()\n");
	}

	{
		union psmi_envvar_val env_addr_fmt;
		psm3_getenv("PSM3_ADDR_FMT",
					"Select address format for NICs and EPID",
					PSMI_ENVVAR_LEVEL_USER, PSMI_ENVVAR_TYPE_INT,
					(union psmi_envvar_val)PSMI_ADDR_FMT_DEFAULT, &env_addr_fmt);
		if (env_addr_fmt.e_int > PSMI_MAX_ADDR_FMT_SUPPORTED) {
			psm3_handle_error(PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,
					  " The max epid version supported in this version of PSM3 is %u \n"
					  "Please upgrade PSM3 \n",
					  PSMI_MAX_ADDR_FMT_SUPPORTED);
			goto fail_unref;
		} else if ( env_addr_fmt.e_int != PSMI_ADDR_FMT_DEFAULT
			&& (env_addr_fmt.e_int < PSMI_MIN_ADDR_FMT_SUPPORTED
				|| ! PSMI_IPS_ADDR_FMT_IS_VALID(env_addr_fmt.e_int))
			) {
			psm3_handle_error(PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,
					  " Invalid value provided through PSM3_ADDR_FMT %d\n", env_addr_fmt.e_int);
			goto fail_unref;
		}
		psm3_addr_fmt = env_addr_fmt.e_int;
	}
	{
		union psmi_envvar_val env_addr_per_nic;
		psm3_getenv("PSM3_ADDR_PER_NIC",
					"Number of addresses per NIC to present",
					PSMI_ENVVAR_LEVEL_USER, PSMI_ENVVAR_TYPE_INT,
					(union psmi_envvar_val)1, &env_addr_per_nic);
		if (env_addr_per_nic.e_uint > PSMI_MAX_RAILS) {
			psm3_handle_error(PSMI_EP_NORETURN, PSM2_INTERNAL_ERR,
					  "PSM3_ADDR_PER_NIC exceeds %u\n",
					  PSMI_MAX_RAILS);
			goto fail_unref;
		}
		psm3_addr_per_nic = env_addr_per_nic.e_uint;
	}
	{
		union psmi_envvar_val env_reg_mr_fail_limit;
		psm3_getenv("PSM3_REG_MR_FAIL_LIMIT",
					"Max number of consecutive reg_mr failures",
					PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_INT,
					(union psmi_envvar_val)100, &env_reg_mr_fail_limit);
		if (env_reg_mr_fail_limit.e_uint >= 1) {
			psm3_reg_mr_fail_limit = env_reg_mr_fail_limit.e_uint;
			if (psm3_reg_mr_warn_cnt > psm3_reg_mr_fail_limit)
				psm3_reg_mr_warn_cnt = psm3_reg_mr_fail_limit;
		}
	}
	{
		union psmi_envvar_val env_allow_routers;
		psm3_getenv("PSM3_ALLOW_ROUTERS",
					"Disable check for Ethernet subnet equality between nodes\n"
					" allows routers between nodes and assumes single network plane for multi-rail\n",
					PSMI_ENVVAR_LEVEL_USER, PSMI_ENVVAR_TYPE_INT,
					(union psmi_envvar_val)0, &env_allow_routers);
		psm3_allow_routers = env_allow_routers.e_int;
	}
	{
		union psmi_envvar_val env_subnets;
		psm3_getenv("PSM3_SUBNETS",
			"List of comma separated patterns for IPv4 and IPv6 subnets to consider",
			PSMI_ENVVAR_LEVEL_USER,
			PSMI_ENVVAR_TYPE_STR,
			(union psmi_envvar_val)PSMI_SUBNETS_DEFAULT, &env_subnets);

		if ((err = psmi_parse_subnets(env_subnets.e_str)))
			goto fail_unref;
	}
	psmi_parse_nic_var();	// no need to uninitialize


	{
		/* get PSM3_NIC_SPEED
		 * "any" - allow any and all NIC speeds
		 * "max" - among the non-filtered NICs, identify fastest and
		 *	filter all NICs with lower speeds (default)
		 * # - select only NICs which match the given speed
		 *		(in bits/sec)
		 * pattern - a pattern which may contain one or more numeric
		 *		speed values
		 */
		union psmi_envvar_val env_speed;
		psm3_getenv("PSM3_NIC_SPEED",
			"NIC speed selection criteria ('any', 'max' or pattern of exact speeds)",
			PSMI_ENVVAR_LEVEL_USER, PSMI_ENVVAR_TYPE_STR,
			(union psmi_envvar_val)"max", &env_speed);
		psm3_nic_speed_wildcard = env_speed.e_str;
	}

	{
		union psmi_envvar_val env_diags;
		psm3_getenv("PSM3_DIAGS",
			"Run internal PSM3 resource alloc/dealloc self tests",
			PSMI_ENVVAR_LEVEL_HIDDEN, PSMI_ENVVAR_TYPE_YESNO,
			(union psmi_envvar_val)0, &env_diags);
		if (env_diags.e_int) {
			_HFI_INFO("Running diags...\n");
			if (psm3_diags()) {
				psm3_handle_error(PSMI_EP_NORETURN, PSM2_INTERNAL_ERR, " diags failure \n");
				goto fail_unref;
			}
		}
	}

	psm3_parse_multi_ep();	// no need to uninitialize

#ifdef PSM_FI
	psm3_parse_faultinj();	// no need to uninitialize
	/* Make sure we complain if fault injection is enabled */
	if (psm3_faultinj_enabled) {
		if (!psm3_parse_no_warn())
			fprintf(stderr,
				"!!! WARNING !!! YOU ARE RUNNING WITH FAULT INJECTION ENABLED!\n");
		psm3_stats_print_msg(
			"!!! WARNING !!! YOU ARE RUNNING WITH FAULT INJECTION ENABLED!\n");
	}
#endif /* #ifdef PSM_FI */

	psm3_ep_init();

	psm3_epid_init();

	if ((err = psm3_parse_devices(devid_enabled)))	// ok to not uninit
		goto fail_epid;

	int rc = psm3_hal_initialize(devid_enabled);

	if (rc)
	{
		err = PSM2_INTERNAL_ERR;
		goto fail_epid;
	}

	if (psm3_device_is_enabled(devid_enabled, PTL_DEVID_IPS)) {
		psm3_hwloc_topology_init();
	}

#ifdef PSM_DSA
	if (psm3_device_is_enabled(devid_enabled, PTL_DEVID_AMSH)) {
		if (psm3_dsa_init()) {
			err = PSM2_INTERNAL_ERR;
			goto fail_dsa;
		}
	}
#endif

#ifdef PSM_HAVE_GPU
	if ( (err = PSM3_GPU_INITIALIZE()) != PSM2_OK) {
		goto fail_gpu;
	}
#endif

update:
	*major = (int)psm3_verno_major;
	*minor = (int)psm3_verno_minor;
fail:
	_HFI_DBG("psmi_refcount=%d,err=%u\n", psmi_refcount, err);

	PSM2_LOG_MSG("leaving");
	return err;

#ifdef PSM_HAVE_GPU
fail_gpu:
#if defined(PSM_DSA)
	psm3_dsa_fini();
#endif
#endif
#if defined(PSM_DSA)
fail_dsa:
#endif
	psm3_hwloc_topology_destroy();	// always safe to call
	psm3_hal_finalize();
fail_epid:
	psm3_epid_fini();
fail_unref:
	psmi_refcount--;
fail_stats:
	psm3_stats_finalize();	// also undoes psm3_mem_stats_register
	goto fail;
}

/* convert return value for various device queries into
 * a psm2_error_t.  Only used for functions requesting NIC details
 * -3 -> PSM2_EP_NO_DEVICE, 0 -> PSM2_OK, other -> PSM2_INTERNAL_ERR
 */
static inline psm2_error_t unit_query_ret_to_err(int ret)
{
	switch (ret) {
	case -3:
		return PSM2_EP_NO_DEVICE;
		break;
	case 0:
		return PSM2_OK;
		break;
	default:
		return PSM2_INTERNAL_ERR;
		break;
	}
}

static uint64_t nics_max_speed;
psm2_error_t psm3_info_query(psm2_info_query_t q, void *out,
			       size_t nargs, psm2_info_query_arg_t args[])
{
	static const size_t expected_arg_cnt[PSM2_INFO_QUERY_LAST] =
	{
		0, /* PSM2_INFO_QUERY_NUM_UNITS         */
		0, /* PSM2_INFO_QUERY_NUM_PORTS         */
		1, /* PSM2_INFO_QUERY_UNIT_STATUS       */
		2, /* PSM2_INFO_QUERY_UNIT_PORT_STATUS  */
		0, /* was PSM2_INFO_QUERY_NUM_FREE_CONTEXTS */
		0, /* was PSM2_INFO_QUERY_NUM_CONTEXTS      */
		0, /* was PSM2_INFO_QUERY_CONFIG        */
		0, /* was PSM2_INFO_QUERY_THRESH        */
		0, /* was PSM2_INFO_QUERY_DEVICE_NAME   */
		0, /* was PSM2_INFO_QUERY_MTU           */
		0, /* was PSM2_INFO_QUERY_LINK_SPEED    */
		0, /* was PSM2_INFO_QUERY_NETWORK_TYPE  */
		0, /* PSM2_INFO_QUERY_FEATURE_MASK      */
		2, /* PSM2_INFO_QUERY_UNIT_NAME         */
		0, /* was PSM2_INFO_QUERY_UNIT_SYS_PATH */
		2, /* PSM2_INFO_QUERY_UNIT_PCI_BUS      */
		4, /* PSM2_INFO_QUERY_UNIT_SUBNET_NAME  */
		2, /* PSM2_INFO_QUERY_UNIT_DEVICE_ID    */
		2, /* PSM2_INFO_QUERY_UNIT_DEVICE_VERSION */
		2, /* PSM2_INFO_QUERY_UNIT_VENDOR_ID    */
		2, /* PSM2_INFO_QUERY_UNIT_DRIVER       */
		2, /* PSM2_INFO_QUERY_PORT_SPEED        */
		0, /* PSM2_INFO_QUERY_NUM_ADDR_PER_UNIT */
		4, /* PSM2_INFO_QUERY_UNIT_ADDR_NAME    */
		0, /* PSM2_INFO_QUERY_GPU_THRESH_RNDV   */
		0, /* PSM2_INFO_QUERY_MQ_RNDV_SHM_GPU_THRESH_DEFAULT */
	};
	psm2_error_t rv = PSM2_INTERNAL_ERR;

	if (q >= 0 && q < PSM2_INFO_QUERY_LAST && nargs != expected_arg_cnt[q])
		return PSM2_PARAM_ERR;

	switch (q)
	{
	case PSM2_INFO_QUERY_NUM_UNITS:
		*((uint32_t*)out) = psmi_hal_get_num_units_();
		rv = PSM2_OK;
		break;
	case PSM2_INFO_QUERY_NUM_PORTS:
		*((uint32_t*)out) = psmi_hal_get_num_ports_();
		rv = PSM2_OK;
		break;
	case PSM2_INFO_QUERY_UNIT_STATUS:
		*((uint32_t*)out) = psmi_hal_get_unit_active(args[0].unit);
		rv = PSM2_OK;
		break;
	case PSM2_INFO_QUERY_UNIT_PORT_STATUS:
		*((uint32_t*)out) = psmi_hal_get_port_active(args[0].unit,
								args[1].port);
		rv = PSM2_OK;
		break;
	case PSM2_INFO_QUERY_FEATURE_MASK:
		*((uint32_t*)out) = PSM3_GPU_QUERY_FEATURE_MASK();
		rv = PSM2_OK;
		break;
	case PSM2_INFO_QUERY_UNIT_NAME:
		{
			char         *hfiName       = (char*)out;
			uint32_t      unit          = args[0].unit;
			size_t        hfiNameLength = args[1].length;

			snprintf(hfiName, hfiNameLength, "%s", psmi_hal_get_unit_name(unit));
			rv = PSM2_OK;
		}
		break;
	case PSM2_INFO_QUERY_UNIT_PCI_BUS:
		{
			uint32_t     *pciBus        = (uint32_t *)out;
			uint32_t      unit          = args[0].unit;

			if (args[1].length != (sizeof(uint32_t)*4)) break;

			rv = unit_query_ret_to_err(psmi_hal_get_unit_pci_bus(unit, pciBus,
							pciBus+1, pciBus+2, pciBus+3));
		}
		break;
	case PSM2_INFO_QUERY_UNIT_SUBNET_NAME:
		{
			char         *subnetName       = (char*)out;
			uint32_t      unit          = args[0].unit;
			uint32_t      port          = args[1].port;
			uint32_t      addr_index    = args[2].addr_index;
			size_t        subnetNameLength = args[3].length;

			if (psmi_hal_get_unit_active(unit) <= 0) break;

			if (psmi_hal_get_port_subnet_name(unit, port, addr_index,
						subnetName, subnetNameLength))
				break;
			rv = PSM2_OK;
		}
		break;
	case PSM2_INFO_QUERY_UNIT_DEVICE_ID:
		{
			char         *devId         = (char*)out;
			uint32_t      unit          = args[0].unit;
			size_t        len           = args[1].length;

			rv = unit_query_ret_to_err(psmi_hal_get_unit_device_id(unit, devId, len));
		}
		break;
	case PSM2_INFO_QUERY_UNIT_DEVICE_VERSION:
		{
			char         *devVer        = (char*)out;
			uint32_t      unit          = args[0].unit;
			size_t        len           = args[1].length;

			rv = unit_query_ret_to_err(psmi_hal_get_unit_device_version(unit, devVer, len));
		}
		break;
	case PSM2_INFO_QUERY_UNIT_VENDOR_ID:
		{
			char         *venId         = (char*)out;
			uint32_t      unit          = args[0].unit;
			size_t        len           = args[1].length;

			rv = unit_query_ret_to_err(psmi_hal_get_unit_vendor_id(unit, venId, len));
		}
		break;
	case PSM2_INFO_QUERY_UNIT_DRIVER:
		{
			char         *driver        = (char*)out;
			uint32_t      unit          = args[0].unit;
			size_t        len           = args[1].length;

			rv = unit_query_ret_to_err(psmi_hal_get_unit_driver(unit, driver, len));
		}
		break;
	case PSM2_INFO_QUERY_PORT_SPEED:
		{
			uint64_t *speed = (uint64_t *)out;
			uint32_t  unit  = args[0].unit;
			uint32_t  port  = args[1].port;

			if (port == 0) port = 1; /* VERBS_PORT */

			if (unit == -1) {
				if (init_cache_on && nics_max_speed) {
					*speed = nics_max_speed;
					rv = PSM2_OK;
					break;
				}
				// query for unit -1 returns max speed of all candidate NICs
				*speed = 0;
				for (unit = 0; unit < psmi_hal_get_num_units_(); unit++) {
					uint64_t unit_speed;
					if (psmi_hal_get_port_lid(unit, port, 0/*addr_index*/) <= 0)
						continue;
					if (0 <= psmi_hal_get_port_speed(unit, port, &unit_speed))
						*speed = max(*speed, unit_speed);
				}
				if (*speed) {
					nics_max_speed = *speed;
					rv = PSM2_OK;
				} else {
					rv = PSM2_EP_NO_DEVICE;
				}
			} else {
				if (psmi_hal_get_port_active(unit, port) <= 0) break;

				rv = unit_query_ret_to_err(psmi_hal_get_port_speed(unit, port, speed));
			}
		}
		break;
	case PSM2_INFO_QUERY_NUM_ADDR_PER_UNIT:
		*((uint32_t*)out) = psm3_addr_per_nic;
		rv = PSM2_OK;
		break;
	case PSM2_INFO_QUERY_UNIT_ADDR_NAME:
		{
			char         *addrName       = (char*)out;
			uint32_t      unit          = args[0].unit;
			//uint32_t      port          = args[1].port;	/* ignored, should be VERBS_PORT */
			uint32_t      addr_index    = args[2].addr_index;
			size_t        addrNameLength = args[3].length;

			if (psm3_addr_per_nic > 1)
				snprintf(addrName, addrNameLength, "%s-%u", psmi_hal_get_unit_name(unit), addr_index);
			else
				snprintf(addrName, addrNameLength, "%s", psmi_hal_get_unit_name(unit));
			rv = PSM2_OK;
		}
		break;
	case PSM2_INFO_QUERY_GPU_THRESH_RNDV:
#ifdef PSM_HAVE_GPU
		*((uint32_t*)out) = psm3_gpu_thresh_rndv;
		rv = PSM2_OK;
#endif
		break;
	case PSM2_INFO_QUERY_MQ_RNDV_SHM_GPU_THRESH_DEFAULT:
#ifdef PSM_HAVE_GPU
		*((uint32_t*)out) = psm3_gpu_mq_rndv_shm_gpu_thresh_default;
		rv = PSM2_OK;
#endif
		break;
	default:
		return 	PSM2_IQ_INVALID_QUERY;
	}

	return rv;
}

/*
 * Function that allows the wrapper provider to get PSM env variables
 * including checks of the /etc/psm3.conf file.  Purposely structured
 * similar to fi_param_get_* for easier use in psmx3 wrapper
 */
int psm3_getenv_int(const char *name, const char *descr, int visible,
				int *value)
{
	union psmi_envvar_val env_val;

	int ret = psm3_getenv(name, descr,
			visible?PSMI_ENVVAR_LEVEL_USER:PSMI_ENVVAR_LEVEL_HIDDEN,
			PSMI_ENVVAR_TYPE_INT, (union psmi_envvar_val)*value,
			&env_val);
	*value = env_val.e_int;
	return ret;
}

int psm3_getenv_bool(const char *name, const char *descr, int visible,
				int *value)
{
	union psmi_envvar_val env_val;

	int ret = psm3_getenv(name, descr,
			visible?PSMI_ENVVAR_LEVEL_USER:PSMI_ENVVAR_LEVEL_HIDDEN,
			PSMI_ENVVAR_TYPE_YESNO, (union psmi_envvar_val)*value,
			&env_val);
	*value = env_val.e_int;
	return ret;
}

int psm3_getenv_str(const char *name, const char *descr, int visible,
				char **value)
{
	union psmi_envvar_val env_val;

	int ret = psm3_getenv(name, descr,
			visible?PSMI_ENVVAR_LEVEL_USER:PSMI_ENVVAR_LEVEL_HIDDEN,
			PSMI_ENVVAR_TYPE_STR, (union psmi_envvar_val)*value,
			&env_val);
	*value = env_val.e_str;
	return ret;
}

uint64_t psm3_get_capability_mask(uint64_t req_cap_mask)
{
	return (psm3_capabilities_bitset & req_cap_mask);
}

psm2_error_t psm3_finalize(void)
{
	struct psmi_eptab_iterator itor;
	char *hostname;
	psm2_ep_t ep;

	PSM2_LOG_MSG("entering");

	_HFI_DBG("psmi_refcount=%d\n", psmi_refcount);
	PSMI_ERR_UNLESS_INITIALIZED(NULL);
	psmi_assert(psmi_refcount > 0);
	psmi_refcount--;

	if (psmi_refcount > 0) {
		return PSM2_OK;
	}

	/* When PSM_PERF is enabled, the following line causes the
	   instruction cycles gathered in the current run to be dumped
	   to stderr. */
	GENERIC_PERF_DUMP(stderr);
	ep = psm3_opened_endpoint;
	while (ep != NULL) {
		psm2_ep_t saved_ep = ep->user_ep_next;
		psm3_ep_close(ep, PSM2_EP_CLOSE_GRACEFUL,
			     2 * PSMI_MIN_EP_CLOSE_TIMEOUT);
		psm3_opened_endpoint = ep = saved_ep;
	}

#ifdef PSM_DSA
	psm3_dsa_fini();	// noop if didn't successfully psm3_dsa_init
#endif

#ifdef PSM_FI
	psm3_faultinj_fini();
#endif /* #ifdef PSM_FI */

	/* De-allocate memory for any allocated space to store hostnames */
	psm3_epid_itor_init(&itor, PSMI_EP_HOSTNAME);
	while ((hostname = psm3_epid_itor_next(&itor)))
		psmi_free(hostname);
	psm3_epid_itor_fini(&itor);

	psm3_epid_fini();

	psm3_ep_fini();

	/* unmap shared mem object for affinity */
	if (psm3_affinity_shared_file_opened) {
		/*
		 * Start critical section to decrement ref count and unlink
		 * affinity shm file.
		 */
		if (psmi_sem_timedwait(psm3_sem_affinity_shm_rw, psm3_sem_affinity_shm_rw_name)) {
			_HFI_ERROR("unable to get NIC affinity semaphone, proceeding anyway\n");
		}

		psm3_shared_affinity_ptr[AFFINITY_SHM_REF_COUNT_LOCATION] -= 1;
		if (psm3_shared_affinity_ptr[AFFINITY_SHM_REF_COUNT_LOCATION] <= 0) {
			_HFI_VDBG("Unlink shm file for NIC affinity as there are no more users\n");
			shm_unlink(psm3_affinity_shm_name);
		} else {
			_HFI_VDBG("Number of affinity shared memory users left=%ld\n",
				  psm3_shared_affinity_ptr[AFFINITY_SHM_REF_COUNT_LOCATION]);
		}

		msync(psm3_shared_affinity_ptr, PSMI_PAGESIZE, MS_SYNC);

		/* End critical section */
		psmi_sem_post(psm3_sem_affinity_shm_rw, psm3_sem_affinity_shm_rw_name);

		munmap(psm3_shared_affinity_ptr, PSMI_PAGESIZE);
		psm3_shared_affinity_ptr = NULL;
		psm3_shared_affinity_nic_refcount_ptr = NULL;
		psmi_free(psm3_affinity_shm_name);
		psm3_affinity_shm_name = NULL;
		psm3_affinity_shared_file_opened = 0;
	}

	if (psm3_affinity_semaphore_open) {
		_HFI_VDBG("Closing and Unlinking Semaphore: %s.\n", psm3_sem_affinity_shm_rw_name);
		sem_close(psm3_sem_affinity_shm_rw);
		psm3_sem_affinity_shm_rw = NULL;
		sem_unlink(psm3_sem_affinity_shm_rw_name);
		psmi_free(psm3_sem_affinity_shm_rw_name);
		psm3_sem_affinity_shm_rw_name = NULL;
		psm3_affinity_semaphore_open = 0;
	}

	psm3_hwloc_topology_destroy();	// always safe to call
	psm3_hal_finalize();

	PSM3_GPU_FINALIZE();

	psmi_refcount = PSMI_FINALIZED;
	PSM2_LOG_MSG("leaving");

	psmi_free_subnets();

	psmi_log_fini();

	psm3_stats_finalize();

	psm3_env_finalize();

	psmi_heapdebug_finalize();

	return PSM2_OK;
}

/*
 * Function exposed in >= 1.05
 */
psm2_error_t
psm3_map_nid_hostname(int num, const psm2_nid_t *nids, const char **hostnames)
{
	int i;
	psm2_error_t err = PSM2_OK;

	PSM2_LOG_MSG("entering");

	PSMI_ERR_UNLESS_INITIALIZED(NULL);

	if (nids == NULL || hostnames == NULL) {
		err = PSM2_PARAM_ERR;
		goto fail;
	}

	for (i = 0; i < num; i++) {
		if ((err = psm3_epid_set_hostname(nids[i], hostnames[i], 1)))
			break;
	}

fail:
	PSM2_LOG_MSG("leaving");
	return err;
}

void psm3_epaddr_setlabel(psm2_epaddr_t epaddr, char const *epaddr_label)
{
	PSM2_LOG_MSG("entering");
	PSM2_LOG_MSG("leaving");
	return;			/* ignore this function */
}

void psm3_epaddr_setctxt(psm2_epaddr_t epaddr, void *ctxt)
{
	uint64_t optlen = sizeof(void *);
	/* Eventually deprecate this API to use set/get opt as this is unsafe. */
	PSM2_LOG_MSG("entering");
	psm3_setopt(PSM2_COMPONENT_CORE, (const void *)epaddr,
		   PSM2_CORE_OPT_EP_CTXT, (const void *)ctxt, optlen);
	PSM2_LOG_MSG("leaving");
}

void *psm3_epaddr_getctxt(psm2_epaddr_t epaddr)
{
	psm2_error_t err;
	uint64_t optlen = sizeof(void *);
	void *result = NULL;

	PSM2_LOG_MSG("entering");
	/* Eventually deprecate this API to use set/get opt as this is unsafe. */
	err = psm3_getopt(PSM2_COMPONENT_CORE, (const void *)epaddr,
			 PSM2_CORE_OPT_EP_CTXT, (void *)&result, &optlen);

	PSM2_LOG_MSG("leaving");

	if (err == PSM2_OK)
		return result;
	else
		return NULL;
}

psm2_error_t
psm3_setopt(psm2_component_t component, const void *component_obj,
	     int optname, const void *optval, uint64_t optlen)
{
	psm2_error_t rv;
	PSM2_LOG_MSG("entering");
	switch (component) {
	case PSM2_COMPONENT_CORE:
		rv = psm3_core_setopt(component_obj, optname, optval, optlen);
		PSM2_LOG_MSG("leaving");
		return rv;
		break;
	case PSM2_COMPONENT_MQ:
		/* Use the deprecated MQ set/get opt for now which does not use optlen */
		rv = psm3_mq_setopt((psm2_mq_t) component_obj, optname, optval);
		PSM2_LOG_MSG("leaving");
		return rv;
		break;
	case PSM2_COMPONENT_AM:
		/* Hand off to active messages */
		rv = psm3_am_setopt(component_obj, optname, optval, optlen);
		PSM2_LOG_MSG("leaving");
		return rv;
		break;
	case PSM2_COMPONENT_IB:
		/* Hand off to IPS ptl to set option */
		rv = psm3_ptl_ips.setopt(component_obj, optname, optval,
					   optlen);
		PSM2_LOG_MSG("leaving");
		return rv;
		break;
	}

	/* Unrecognized/unknown component */
	rv = psm3_handle_error(NULL, PSM2_PARAM_ERR, "Unknown component %u",
				 component);
	PSM2_LOG_MSG("leaving");
	return rv;
}

psm2_error_t
psm3_getopt(psm2_component_t component, const void *component_obj,
	     int optname, void *optval, uint64_t *optlen)
{
	psm2_error_t rv;

	PSM2_LOG_MSG("entering");
	switch (component) {
	case PSM2_COMPONENT_CORE:
		rv = psm3_core_getopt(component_obj, optname, optval, optlen);
		PSM2_LOG_MSG("leaving");
		return rv;
		break;
	case PSM2_COMPONENT_MQ:
		/* Use the deprecated MQ set/get opt for now which does not use optlen */
		rv = psm3_mq_getopt((psm2_mq_t) component_obj, optname, optval);
		PSM2_LOG_MSG("leaving");
		return rv;
		break;
	case PSM2_COMPONENT_AM:
		/* Hand off to active messages */
		rv = psm3_am_getopt(component_obj, optname, optval, optlen);
		PSM2_LOG_MSG("leaving");
		return rv;
		break;
	case PSM2_COMPONENT_IB:
		/* Hand off to IPS ptl to set option */
		rv = psm3_ptl_ips.getopt(component_obj, optname, optval,
					   optlen);
		PSM2_LOG_MSG("leaving");
		return rv;
		break;
	}

	/* Unrecognized/unknown component */
	rv = psm3_handle_error(NULL, PSM2_PARAM_ERR, "Unknown component %u",
				 component);
	PSM2_LOG_MSG("leaving");
	return rv;
}

psm2_error_t psm3_poll_noop(ptl_t *ptl, int replyonly, bool force)
{
	PSM2_LOG_MSG("entering");
	PSM2_LOG_MSG("leaving");
	return PSM2_OK_NO_PROGRESS;
}

psm2_error_t psm3_poll(psm2_ep_t ep)
{
	psm2_error_t err1 = PSM2_OK, err2 = PSM2_OK;
	psm2_ep_t tmp;

	PSM2_LOG_MSG("entering");

	PSMI_ASSERT_INITIALIZED();

	PSMI_LOCK(ep->mq->progress_lock);

	tmp = ep;
	do {
		err1 = ep->ptl_amsh.ep_poll(ep->ptl_amsh.ptl, 0, 0);	/* poll reqs & reps */
		if (err1 > PSM2_OK_NO_PROGRESS) {	/* some error unrelated to polling */
			PSMI_UNLOCK(ep->mq->progress_lock);
			PSM2_LOG_MSG("leaving");
			return err1;
		}

		err2 = ep->ptl_ips.ep_poll(ep->ptl_ips.ptl, 0, 0);	/* get into ips_do_work */
		if (err2 > PSM2_OK_NO_PROGRESS) {	/* some error unrelated to polling */
			PSMI_UNLOCK(ep->mq->progress_lock);
			PSM2_LOG_MSG("leaving");
			return err2;
		}
		ep = ep->mctxt_next;
	} while (ep != tmp);

	/* This is valid because..
	 * PSM2_OK & PSM2_OK_NO_PROGRESS => PSM2_OK
	 * PSM2_OK & PSM2_OK => PSM2_OK
	 * PSM2_OK_NO_PROGRESS & PSM2_OK => PSM2_OK
	 * PSM2_OK_NO_PROGRESS & PSM2_OK_NO_PROGRESS => PSM2_OK_NO_PROGRESS */
	PSMI_UNLOCK(ep->mq->progress_lock);
	PSM2_LOG_MSG("leaving");
	return (err1 & err2);
}

psm2_error_t psm3_poll_internal(psm2_ep_t ep, int poll_amsh, bool force)
{
	psm2_error_t err1 = PSM2_OK_NO_PROGRESS;
	psm2_error_t err2;
	psm2_ep_t tmp;

	PSM2_LOG_MSG("entering");
	PSMI_LOCK_ASSERT(ep->mq->progress_lock);

	tmp = ep;
	do {
		if (poll_amsh) {
			err1 = ep->ptl_amsh.ep_poll(ep->ptl_amsh.ptl, 0, force);	/* poll reqs & reps */
			if (err1 > PSM2_OK_NO_PROGRESS) { /* some error unrelated to polling */
				PSM2_LOG_MSG("leaving");
				return err1;
			}
		}

		err2 = ep->ptl_ips.ep_poll(ep->ptl_ips.ptl, 0, force);	/* get into ips_do_work */
		if (err2 > PSM2_OK_NO_PROGRESS) { /* some error unrelated to polling */
			PSM2_LOG_MSG("leaving");
			return err2;
		}

		ep = ep->mctxt_next;
	} while (ep != tmp);
	PSM2_LOG_MSG("leaving");
	return (err1 & err2);
}
#ifdef PSM_PROFILE
/* These functions each have weak symbols */
void psmi_profile_block()
{
	;			/* empty for profiler */
}

void psmi_profile_unblock()
{
	;			/* empty for profiler */
}

void psmi_profile_reblock(int did_no_progress)
{
	;			/* empty for profiler */
}
#endif
