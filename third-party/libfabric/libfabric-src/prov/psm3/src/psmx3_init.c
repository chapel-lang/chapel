/*
 * Copyright (c) 2013-2020 Intel Corporation. All rights reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * BSD license below:
 *
 *     Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *      - Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "ofi_prov.h"
#include "psmx3.h"
#include <glob.h>
#include <dlfcn.h>

static int psmx3_init_count = 0;
static int psmx3_lib_initialized = 0;
static pthread_mutex_t psmx3_lib_mutex;

struct psmx3_domain_info psmx3_domain_info;

static const char* FI_PSM3_NAME_SERVER_HELP =
			"Whether to turn on the name server or not (default: yes)";
static const char* FI_PSM3_TAGGED_RMA_HELP =
			"Whether to use tagged messages for large size RMA or not " \
			"(default: no)";
static const char* FI_PSM3_UUID_HELP =
			"Unique Job ID required by the fabric";
static const char* FI_PSM3_DELAY_HELP =
			"Delay (seconds) before finalization (for debugging)";
static const char* FI_PSM3_TIMEOUT_HELP =
			"Timeout (seconds) for gracefully closing the PSM3 endpoint";
static const char* FI_PSM3_PROG_INTERVAL_HELP =
			"Interval (microseconds) between progress calls made in the " \
			"progress thread (default: 1 if affinity is set, 1000 if not)";
static const char* FI_PSM3_PROG_AFFINITY_HELP =
			"When set, specify the set of CPU cores to set the progress " \
			"thread affinity to. The format is " \
			"<start>[:<end>[:<stride>]][,<start>[:<end>[:<stride>]]]*, " \
			"where each triplet <start>:<end>:<stride> defines a block " \
			"of core_ids. Both <start> and <end> can be either the core_id " \
			"(when >=0) or core_id - num_cores (when <0). " \
			"(default: affinity not set)";
static const char* FI_PSM3_INJECT_SIZE_HELP =
			"Maximum message size for fi_inject and fi_tinject (default: 64).";
static const char* FI_PSM3_LOCK_LEVEL_HELP =
			"How internal locking is used. 0 means no locking. (default: 2).";
static const char* FI_PSM3_LAZY_CONN_HELP =
			"Whether to force lazy connection mode. (default: no).";
static const char* FI_PSM3_CONN_TIMEOUT_HELP =
			"Timeout (seconds) for establishing connection between two PSM3 endpoints";
static const char* FI_PSM3_DISCONNECT_HELP =
			"Whether to issue disconnect request when process ends (default: no).";
#if (PSMX3_TAG_LAYOUT == PSMX3_TAG_LAYOUT_RUNTIME)
static const char* FI_PSM3_TAG_LAYOUT_HELP =
			"How the 96 bit PSM3 tag is organized: " \
			"tag60 means 32/4/60 for data/flags/tag;" \
			"tag64 means 4/28/64 for flags/data/tag (default: tag60).";
#endif
static const char* FI_PSM3_YIELD_MODE_HELP =
			"Enabled interrupt driven operation with fi_wait. (default: no).";
static const char* FI_PSM3_WAIT_ENABLE_HELP =
			"Enabled use of wait semantics outside of yield mode. (default: no).";

#define FI_PSM3_PREFIX "FI_PSM3_"
#define FI_PSM3_PREFIX_LEN strlen(FI_PSM3_PREFIX)

// logging and behavior similar to fi_param_get_bool
int psmx3_param_get_bool(struct fi_provider *provider, const char *env_var_name,
					const char *descr, int visible, int *value)
{
	assert(0 == strncmp(env_var_name, FI_PSM3_PREFIX, FI_PSM3_PREFIX_LEN));
	if (psm3_getenv_bool(env_var_name, descr, visible, value)) {
		PSMX3_INFO(provider, FI_LOG_CORE, "variable %s=<not set>\n", env_var_name);
		return -FI_ENODATA;
	} else {
		PSMX3_INFO(provider, FI_LOG_CORE, "read bool var %s=%d\n", env_var_name,
				*value);
		return FI_SUCCESS;
	}
}

// logging and behavior similar to fi_param_get_int
int psmx3_param_get_int(struct fi_provider *provider, const char *env_var_name,
					const char *descr, int visible, int *value)
{
	assert(0 == strncmp(env_var_name, FI_PSM3_PREFIX, FI_PSM3_PREFIX_LEN));
	if (psm3_getenv_int(env_var_name, descr, visible, value)) {
		PSMX3_INFO(provider, FI_LOG_CORE, "variable %s=<not set>\n", env_var_name);
		return -FI_ENODATA;
	} else {
		PSMX3_INFO(provider, FI_LOG_CORE, "read int var %s=%d\n", env_var_name,
				*value);
		return FI_SUCCESS;
	}
}

// logging and behavior similar to fi_param_get_str
int psmx3_param_get_str(struct fi_provider *provider, const char *env_var_name,
					const char *descr, int visible, char **value)
{
	assert(0 == strncmp(env_var_name, FI_PSM3_PREFIX, FI_PSM3_PREFIX_LEN));
	if (psm3_getenv_str(env_var_name, descr, visible, value)) {
		PSMX3_INFO(provider, FI_LOG_CORE, "variable %s=<not set>\n", env_var_name);
		return -FI_ENODATA;
	} else {
		PSMX3_INFO(provider, FI_LOG_CORE, "read string var %s=%s\n", env_var_name,
				*value);
		return FI_SUCCESS;
	}
}

struct psmx3_env psmx3_env = {
	.name_server	= 1,
	.tagged_rma	= 0,
	.uuid		= PSMX3_DEFAULT_UUID,
	.uuid_override  = 0,
	.delay		= 0,
	.timeout	= 10,
	.conn_timeout	= 10,
	.prog_interval	= -1,
	.prog_affinity	= NULL,
	.multi_ep	= 1,
	.inject_size	= 64,
	.lock_level	= 2,
	.lazy_conn	= 0,
	.disconnect	= 0,
#if (PSMX3_TAG_LAYOUT == PSMX3_TAG_LAYOUT_RUNTIME)
	.tag_layout	= "auto",
#endif
	.yield_mode	= 0,
	.wait_enable	= 0,
};

#if (PSMX3_TAG_LAYOUT == PSMX3_TAG_LAYOUT_RUNTIME)
uint64_t psmx3_tag_mask;
uint32_t psmx3_tag_upper_mask;
uint32_t psmx3_data_mask;
int	 psmx3_flags_idx;
int	 psmx3_tag_layout_locked = 0;
#endif

static void psmx3_init_env(void)
{
	uint32_t uid = getuid();
	char *uuid = NULL;

	psm3_env_initialize();

	if (getenv("OMPI_COMM_WORLD_RANK") || getenv("PMI_RANK") || getenv("PMIX_RANK"))
		psmx3_env.name_server = 0;

	//fi_param_get_bool(&psmx3_prov, "name_server", &psmx3_env.name_server);
	psmx3_param_get_bool(&psmx3_prov, "FI_PSM3_NAME_SERVER",
					FI_PSM3_NAME_SERVER_HELP, 0, &psmx3_env.name_server);
	//fi_param_get_bool(&psmx3_prov, "tagged_rma", &psmx3_env.tagged_rma);
	psmx3_param_get_bool(&psmx3_prov, "FI_PSM3_TAGGED_RMA",
					FI_PSM3_TAGGED_RMA_HELP, 0, &psmx3_env.tagged_rma);
	//if (FI_SUCCESS != fi_param_get_str(&psmx3_prov, "uuid", &psmx3_env.uuid)) {
	if (FI_SUCCESS != psmx3_param_get_str(&psmx3_prov, "FI_PSM3_UUID",
					FI_PSM3_UUID_HELP, 1, &psmx3_env.uuid)) {
		/*
		 * For OpenMPI 4.x only:
		 * The job key is passed via the environment variable, but the format
		 * is different. Perform format conversion and use it as the default
		 * uuid. This will be overridden if FI_PSM3_UUID is set.
		 */
		psm2_uuid_t ompi_uuid = {};
		unsigned long long int *u = (unsigned long long int *)ompi_uuid;
		char *ompi_job_key = getenv("OMPI_MCA_orte_precondition_transports");
		if (ompi_job_key) {
			PSMX3_INFO(&psmx3_prov, FI_LOG_CORE,
				"Open MPI job key: %s.\n", ompi_job_key);
			if (sscanf(ompi_job_key, "%016llx-%016llx", &u[0], &u[1]) == 2)
				uuid = strdup(psmx3_uuid_to_string(ompi_uuid));
			else {
				PSMX3_INFO(&psmx3_prov, FI_LOG_CORE,
					"Invalid Open MPI job key format.\n");
			}
		}

		/* Set Default UUID if none supplied through environment variable */
		if (!uuid) { /* If ompi_job_key is not set or invalid */
			uuid = strdup(PSMX3_DEFAULT_UUID);
			if (uuid) {
				/* fill in uid as bytes 9-11 (XXXX-XXXX) in format:
				 * xxxxxxxx-xxxx-XXXX-XXXX-xxxxxxxxxxxx
				 */
				snprintf(&uuid[14], 10, "%02hhX%02hhX-%02hhX%02hhX",
					(uint8_t)((uid >> 24) & 0xff), (uint8_t)((uid >> 16) & 0xff),
					(uint8_t)((uid >> 8) & 0xff), (uint8_t)(uid & 0xff));
				uuid[23] = '-';	/* restore */
			}
		}
		psmx3_env.uuid = uuid;
	} else {
		/* FI_PSM3_UUID has highest priority, so it can override auth_key from fi_info */
		psmx3_env.uuid_override = 1;
	}
	//fi_param_get_int(&psmx3_prov, "delay", &psmx3_env.delay);
	psmx3_param_get_int(&psmx3_prov, "FI_PSM3_DELAY",
				FI_PSM3_DELAY_HELP, 0, &psmx3_env.delay);
	//fi_param_get_int(&psmx3_prov, "timeout", &psmx3_env.timeout);
	psmx3_param_get_int(&psmx3_prov, "FI_PSM3_TIMEOUT",
				FI_PSM3_TIMEOUT_HELP, 0, &psmx3_env.timeout);
	//fi_param_get_int(&psmx3_prov, "prog_interval", &psmx3_env.prog_interval);
	psmx3_param_get_int(&psmx3_prov, "FI_PSM3_PROG_INTERVAL",
				FI_PSM3_PROG_INTERVAL_HELP, 0, &psmx3_env.prog_interval);
	//fi_param_get_str(&psmx3_prov, "prog_affinity", &psmx3_env.prog_affinity);
	psmx3_param_get_str(&psmx3_prov, "FI_PSM3_PROG_AFFINITY",
				FI_PSM3_PROG_AFFINITY_HELP, 0, &psmx3_env.prog_affinity);
	//fi_param_get_int(&psmx3_prov, "inject_size", &psmx3_env.inject_size);
	psmx3_param_get_int(&psmx3_prov, "FI_PSM3_INJECT_SIZE",
				FI_PSM3_INJECT_SIZE_HELP, 1, &psmx3_env.inject_size);
	//fi_param_get_int(&psmx3_prov, "lock_level", &psmx3_env.lock_level);
	psmx3_param_get_int(&psmx3_prov, "FI_PSM3_LOCK_LEVEL",
				FI_PSM3_LOCK_LEVEL_HELP, 0, &psmx3_env.lock_level);
	//fi_param_get_bool(&psmx3_prov, "lazy_conn", &psmx3_env.lazy_conn);
	psmx3_param_get_bool(&psmx3_prov, "FI_PSM3_LAZY_CONN",
				FI_PSM3_LAZY_CONN_HELP, 1, &psmx3_env.lazy_conn);
	if (psmx3_env.lazy_conn)
		psmx3_env.conn_timeout = 30;	// more headroom since app may be busy
	//fi_param_get_int(&psmx3_prov, "conn_timeout", &psmx3_env.conn_timeout);
	psmx3_param_get_int(&psmx3_prov, "FI_PSM3_CONN_TIMEOUT",
				FI_PSM3_CONN_TIMEOUT_HELP, 0, &psmx3_env.conn_timeout);
	//fi_param_get_bool(&psmx3_prov, "disconnect", &psmx3_env.disconnect);
	psmx3_param_get_bool(&psmx3_prov, "FI_PSM3_DISCONNECT",
				FI_PSM3_DISCONNECT_HELP, 0, &psmx3_env.disconnect);
#if (PSMX3_TAG_LAYOUT == PSMX3_TAG_LAYOUT_RUNTIME)
	//fi_param_get_str(&psmx3_prov, "tag_layout", &psmx3_env.tag_layout);
	psmx3_param_get_str(&psmx3_prov, "FI_PSM3_TAG_LAYOUT",
				FI_PSM3_TAG_LAYOUT_HELP, 0, &psmx3_env.tag_layout);
#endif
	//fi_param_get_bool(&psmx3_prov, "yield_mode", &psmx3_env.yield_mode);
	psmx3_param_get_bool(&psmx3_prov, "FI_PSM3_YIELD_MODE",
				FI_PSM3_YIELD_MODE_HELP, 0, &psmx3_env.yield_mode);
	psmx3_param_get_bool(&psmx3_prov, "FI_PSM3_WAIT_ENABLE",
				FI_PSM3_WAIT_ENABLE_HELP, 0, &psmx3_env.wait_enable);
}

void psmx3_init_tag_layout(struct fi_info *info)
{
	int use_tag64;

#if (PSMX3_TAG_LAYOUT == PSMX3_TAG_LAYOUT_RUNTIME)
	use_tag64 = (psmx3_tag_mask == PSMX3_TAG_MASK_64);

	if (psmx3_tag_layout_locked) {
		PSMX3_INFO(&psmx3_prov, FI_LOG_CORE,
			"tag layout already set opened domain.\n");
		goto out;
	}

	if (strcasecmp(psmx3_env.tag_layout, "tag60") == 0) {
		psmx3_tag_upper_mask = PSMX3_TAG_UPPER_MASK_60;
		psmx3_tag_mask = PSMX3_TAG_MASK_60;
		psmx3_data_mask = PSMX3_DATA_MASK_60;
		psmx3_flags_idx = PSMX3_FLAGS_IDX_60;
		use_tag64 = 0;
	} else if (strcasecmp(psmx3_env.tag_layout, "tag64") == 0) {
		psmx3_tag_upper_mask = PSMX3_TAG_UPPER_MASK_64;
		psmx3_tag_mask = PSMX3_TAG_MASK_64;
		psmx3_data_mask = PSMX3_DATA_MASK_64;
		psmx3_flags_idx = PSMX3_FLAGS_IDX_64;
		use_tag64 = 1;
	} else {
		if (strcasecmp(psmx3_env.tag_layout, "auto") != 0) {
			PSMX3_INFO(&psmx3_prov, FI_LOG_CORE,
				"Invalid tag layout '%s', using 'auto'.\n",
				psmx3_env.tag_layout);
			psmx3_env.tag_layout = "auto";
		}
		if ((info->caps & (FI_TAGGED | FI_MSG)) &&
		    info->domain_attr->cq_data_size) {
			psmx3_tag_upper_mask = PSMX3_TAG_UPPER_MASK_60;
			psmx3_tag_mask = PSMX3_TAG_MASK_60;
			psmx3_data_mask = PSMX3_DATA_MASK_60;
			psmx3_flags_idx = PSMX3_FLAGS_IDX_60;
			use_tag64 = 0;
		} else {
			psmx3_tag_upper_mask = PSMX3_TAG_UPPER_MASK_64;
			psmx3_tag_mask = PSMX3_TAG_MASK_64;
			psmx3_data_mask = PSMX3_DATA_MASK_64;
			psmx3_flags_idx = PSMX3_FLAGS_IDX_64;
			use_tag64 = 1;
		}
	}

	psmx3_tag_layout_locked = 1;
out:
#elif (PSMX3_TAG_LAYOUT == PSMX3_TAG_LAYOUT_TAG64)
	use_tag64 = 1;
#else
	use_tag64 = 0;
#endif
	PSMX3_INFO(&psmx3_prov, FI_LOG_CORE,
		"use %s: tag_mask: %016" PRIX64 ", data_mask: %08" PRIX32 "\n",
		use_tag64 ? "tag64" : "tag60", (uint64_t)PSMX3_TAG_MASK,
		PSMX3_DATA_MASK);
}

static int psmx3_check_multi_ep_cap(void)
{
	uint64_t caps = PSM2_MULTI_EP_CAP;
	char *s = NULL;
	int val = 1; /* if parses as empty (-1) or invalid (-2), use default of 1 */

	s = psm3_env_get("PSM3_MULTI_EP");
	/* psm3 below us will provide warning as needed when it parses it again */
	if (psm3_get_capability_mask(caps) == caps && (psm3_parse_str_yesno(s, &val) || val))
		psmx3_env.multi_ep = 1;
	else
		psmx3_env.multi_ep = 0;

	return psmx3_env.multi_ep;
}

/* indicate if lock_level variable was set (1 if set, 0 if defaulted) */
int psmx3_check_lock_level(void)
{
	int tmp = 2;

	//err = fi_param_get_int(&psmx3_prov, "lock_level", &tmp);
	return (0 == psmx3_param_get_int(&psmx3_prov, "FI_PSM3_LOCK_LEVEL",
				FI_PSM3_LOCK_LEVEL_HELP, 0, &tmp));
}

static int psmx3_init_lib(void)
{
	int major, minor;
	int ret = 0, err;

	if (psmx3_lib_initialized)
		return 0;

	pthread_mutex_lock(&psmx3_lib_mutex);

	if (psmx3_lib_initialized)
		goto out;

	/* turn on multi-ep feature, but don't overwrite existing setting */
	/*setenv("PSM3_MULTI_EP", "1", 0); - not needed, PSM3 default=1*/

	psm3_error_register_handler(NULL, PSM2_ERRHANDLER_NO_HANDLER);

	major = PSM2_VERNO_MAJOR;
	minor = PSM2_VERNO_MINOR;

	err = psm3_init(&major, &minor);
	if (err != PSM2_OK) {
		PSMX3_WARN(&psmx3_prov, FI_LOG_CORE,
			"psm3_init failed: %s\n", psm3_error_get_string(err));
		ret = err;
		goto out;
	}

	PSMX3_INFO(&psmx3_prov, FI_LOG_CORE,
		"PSM3 header version = (%d, %d)\n", PSM2_VERNO_MAJOR, PSM2_VERNO_MINOR);
	PSMX3_INFO(&psmx3_prov, FI_LOG_CORE,
		"PSM3 library version = (%d, %d)\n", major, minor);

	if (psmx3_check_multi_ep_cap())
		PSMX3_INFO(&psmx3_prov, FI_LOG_CORE, "PSM3 multi-ep feature enabled.\n");
	else
		PSMX3_INFO(&psmx3_prov, FI_LOG_CORE, "PSM3 multi-ep feature not available or disabled.\n");

	psmx3_lib_initialized = 1;

out:
	pthread_mutex_unlock(&psmx3_lib_mutex);
	return ret;
}

static int psmx3_update_hfi_info(void)
{
	unsigned short i, j, psmx3_unit;
	int multirail = 0;
	char *s = NULL;
	char unit_name[NAME_MAX];
	char fabric_name[NAME_MAX];
	uint32_t cnt = 0;
	uint32_t addr_cnt = 0;
	int unit_active;
	int ret;
	psm2_info_query_arg_t args[4];
	char first_active_unit_name[NAME_MAX];
	char first_active_unit_fabric_name[NAME_MAX];

	if (psmx3_domain_info.num_units > 0)
		return 0;

	if (psm3_info_query(PSM2_INFO_QUERY_NUM_UNITS, &cnt, 0, NULL)
			|| !cnt) {
		PSMX3_INFO(&psmx3_prov, FI_LOG_CORE,
			"no PSM3 device is found.\n");
		return -FI_ENODEV;
	} else if (cnt > PSMX3_MAX_UNITS) {
		PSMX3_WARN(&psmx3_prov, FI_LOG_CORE,
			"Discovered %u devices, limited to 1st %u\n", cnt, PSMX3_MAX_UNITS);
		cnt = PSMX3_MAX_UNITS;
	}
	if (psm3_info_query(PSM2_INFO_QUERY_NUM_ADDR_PER_UNIT, &addr_cnt, 0, NULL) || !addr_cnt)
	{
		PSMX3_INFO(&psmx3_prov, FI_LOG_CORE,
			"no PSM3 device is found (no addr per unit).\n");
		return -FI_ENODEV;
	}
	psmx3_domain_info.num_units = cnt * addr_cnt;
	if (psmx3_domain_info.num_units > PSMX3_MAX_UNITS) {
		PSMX3_WARN(&psmx3_prov, FI_LOG_CORE,
			"Too many addresses per PSM3 device. For %d physical "
			"devices only  %d addresses per device are supported.",
			cnt, PSMX3_MAX_UNITS/cnt);
		return-FI_ENODEV;
	}

	// if parses as empty or invalid (-1), use default of 0 */
	// PSM3 below us will provide warning as needed when it parses it
	s = psm3_env_get("PSM3_MULTIRAIL");
	(void)psm3_parse_str_int(s, &multirail, INT_MIN, INT_MAX);

	psmx3_domain_info.num_reported_units = 0;
	psmx3_domain_info.num_active_units = 0;
	for (i = 0; i < cnt; i++) {
		args[0].unit = i;
		ret = psm3_info_query(PSM2_INFO_QUERY_UNIT_STATUS, &unit_active, 1, args);
		if (ret != PSM2_OK) {
			PSMX3_WARN(&psmx3_prov, FI_LOG_CORE,
				"Failed to check active state of HFI unit_id %d\n",
				i);
			continue;
		}

		if (unit_active<=0) {
			PSMX3_WARN(&psmx3_prov, FI_LOG_CORE,
				"NIC unit_id %d STATE = INACTIVE\n",
				i);
			continue;
		}

		for (j=0; j < addr_cnt; j++) {
			psmx3_unit = i * addr_cnt + j;
			args[1].port = 1;	// VERBS_PORT
			args[2].addr_index = j;

			args[3].length = sizeof(unit_name);
			if (PSM2_OK != psm3_info_query(PSM2_INFO_QUERY_UNIT_ADDR_NAME,
							unit_name, 4, args))
			{
				PSMX3_WARN(&psmx3_prov, FI_LOG_CORE,
					"Failed to read name of HFI unit_id %d addr_index %d\n",
					i, j);
				continue;
			}

			// IB/OPA ports only support addr_index==0 for PSM3_ADDR_PER_NIC>1
			// in which case this will fail for addr_index>0 and prevent
			// them from being reported in fi_info
			args[3].length = sizeof(fabric_name);
			if (PSM2_OK != psm3_info_query(PSM2_INFO_QUERY_UNIT_SUBNET_NAME,
							fabric_name, 4, args))
			{
				PSMX3_WARN(&psmx3_prov, FI_LOG_CORE,
					"Failed to read name of HFI unit_id %d addr_index %d subnet\n",
					i, j);
				continue;
			}

			psmx3_domain_info.num_active_units++;

			/* for PSM3_MULTIRAIL only report 1 "autoselect" unit */
			if (psmx3_domain_info.num_reported_units < 1 || multirail <= 0) {
				psmx3_domain_info.unit_is_active[psmx3_unit] = 1;
				psmx3_domain_info.unit_id[psmx3_unit] = i;
				psmx3_domain_info.addr_index[psmx3_unit] = j;
				psmx3_domain_info.active_units[psmx3_domain_info.num_reported_units++] = psmx3_unit;
			}
			if (psmx3_domain_info.num_active_units == 1) {
				if (multirail < 0) {
					strcpy(psmx3_domain_info.default_domain_name, "");
					strcpy(psmx3_domain_info.default_fabric_name, "");
				} else if (multirail) {
					strcpy(psmx3_domain_info.default_domain_name, "autoselect:");
					strcpy(psmx3_domain_info.default_fabric_name, "autoselect:");
				} else {
					strcpy(psmx3_domain_info.default_domain_name, "autoselect_one:");
					strcpy(psmx3_domain_info.default_fabric_name, "autoselect_one:");
				}
				strcpy(first_active_unit_name, unit_name);
				strcpy(first_active_unit_fabric_name, fabric_name);
			}
			if (multirail >= 0) {
				if (psmx3_domain_info.num_active_units > 1) {
					strcat(psmx3_domain_info.default_domain_name, ";");
					strcat(psmx3_domain_info.default_fabric_name, ";");
				}
				strcat(psmx3_domain_info.default_domain_name, unit_name);
				strcat(psmx3_domain_info.default_fabric_name, fabric_name);
			}
		}
	}
	/* replace "autoselect:" when only 1 choice */
	if (psmx3_domain_info.num_active_units == 1 && multirail >= 0) {
		strcpy(psmx3_domain_info.default_domain_name, first_active_unit_name);
		strcpy(psmx3_domain_info.default_fabric_name, first_active_unit_fabric_name);
	}

	PSMX3_INFO(&psmx3_prov, FI_LOG_CORE,
		"psm3 units: total %d, reported %d, active %d\n",
		psmx3_domain_info.num_units, psmx3_domain_info.num_reported_units,
		psmx3_domain_info.num_active_units);

	if (psmx3_env.multi_ep) {
		psmx3_domain_info.max_trx_ctxt = PSMX3_MAX_EPS;
	} else {
		psmx3_domain_info.max_trx_ctxt = 1;
	}

	PSMX3_INFO(&psmx3_prov, FI_LOG_CORE,
		"Tx/Rx contexts: %d allowed per process.\n",
		psmx3_domain_info.max_trx_ctxt);

	return 0;
}

static psm2_error_t psmx3_get_link_speed(int unit_id, int port, uint64_t *speed,
					const struct fi_info *info)
{
	psm2_error_t ret;
	psm2_info_query_arg_t args[2];

	args[0].unit = unit_id;
	args[1].port = port == PSMX3_DEFAULT_PORT ? 1 : port;
	ret = psm3_info_query(PSM2_INFO_QUERY_PORT_SPEED, speed, 2, args);
	if (PSM2_OK != ret) {
		PSMX3_WARN(&psmx3_prov, FI_LOG_CORE,
			"Failed to query link_attr (speed) for unit %d/%d: %s\n",
			unit_id, port, info->nic && info->nic->device_attr?
					info->nic->device_attr->name:"(null)");
	}
	return ret;
}

static void psmx3_update_nic_info(struct fi_info *info)
{
	uint32_t pci_bus[4] = {};
	int unit, port, unit_id;
	psm2_info_query_arg_t args[2];
	char buffer[PATH_MAX] = {};
	psm2_error_t err;

	for ( ; info; info = info->next) {
		unit = ((struct psmx3_ep_name *)info->src_addr)->unit;
		port = ((struct psmx3_ep_name *)info->src_addr)->port;
		uint64_t speed = 0;

		if (unit == PSMX3_DEFAULT_UNIT) {
			// only can get speed (max of available) for default
			// as the specific device is not yet selected
			if (PSM2_OK != psmx3_get_link_speed(unit, port, &speed, info))
				continue;
			if (!info->nic) {
				info->nic = ofi_nic_dup(NULL);
				if (!info->nic) {
					PSMX3_WARN(&psmx3_prov, FI_LOG_CORE,
						"Failed to allocate nic info for unit %d\n", unit);
					continue;
				}
			}
			info->nic->link_attr->speed = (size_t)speed;
			continue;
		}

		unit_id = psmx3_domain_info.unit_id[unit];
		args[0].unit = unit_id;
		args[1].length = sizeof(uint32_t) * 4;
		err = psm3_info_query(PSM2_INFO_QUERY_UNIT_PCI_BUS, pci_bus, 2, args);
		if (err == PSM2_EP_NO_DEVICE)
			continue;	/* no NIC to report, probably single node job */
		if (err != PSM2_OK) {
			PSMX3_WARN(&psmx3_prov, FI_LOG_CORE,
				"Failed to query bus_attr (pci bus) for unit %d\n", unit);
			continue;
		}

		if (!info->nic) {
			info->nic = ofi_nic_dup(NULL);
			if (!info->nic) {
				PSMX3_WARN(&psmx3_prov, FI_LOG_CORE,
					"Failed to allocate nic info for unit %d\n", unit);
				continue;
			}
		}

		info->nic->bus_attr->bus_type = FI_BUS_PCI;
		info->nic->bus_attr->attr.pci.domain_id = (uint16_t)pci_bus[0];
		info->nic->bus_attr->attr.pci.bus_id = (uint8_t)pci_bus[1];
		info->nic->bus_attr->attr.pci.device_id = (uint8_t)pci_bus[2];
		info->nic->bus_attr->attr.pci.function_id = (uint8_t)pci_bus[3];

		/* ----------------------------------------------------------------------- */
		/* Device Attr */
		args[1].length = sizeof(buffer);
		if (PSM2_OK != psm3_info_query(PSM2_INFO_QUERY_UNIT_NAME, buffer, 2, args)) {
			PSMX3_WARN(&psmx3_prov, FI_LOG_CORE,
				"Failed to query device_attr (name) for unit %d\n", unit);
			continue;
		}
		info->nic->device_attr->name = strdup(buffer);

		if (PSM2_OK != psm3_info_query(PSM2_INFO_QUERY_UNIT_DEVICE_ID, buffer, 2, args)) {
			PSMX3_WARN(&psmx3_prov, FI_LOG_CORE,
				"Failed to query device_attr (device_id) for unit %d: %s\n",
				unit, info->nic->device_attr->name);
			continue;
		}
		info->nic->device_attr->device_id = strdup(buffer);

		if (PSM2_OK != psm3_info_query(PSM2_INFO_QUERY_UNIT_DEVICE_VERSION, buffer, 2, args)) {
			PSMX3_WARN(&psmx3_prov, FI_LOG_CORE,
				"Failed to query device_attr (device_version) for unit %d: %s\n",
				unit, info->nic->device_attr->name);
			continue;
		}
		info->nic->device_attr->device_version = strdup(buffer);

		if (PSM2_OK != psm3_info_query(PSM2_INFO_QUERY_UNIT_VENDOR_ID, buffer, 2, args)) {
			PSMX3_WARN(&psmx3_prov, FI_LOG_CORE,
				"Failed to query device_attr (vendor_id) for unit %d: %s\n",
				unit, info->nic->device_attr->name);
			continue;
		}
		info->nic->device_attr->vendor_id = strdup(buffer);

		if (PSM2_OK != psm3_info_query(PSM2_INFO_QUERY_UNIT_DRIVER, buffer, 2, args)) {
			PSMX3_WARN(&psmx3_prov, FI_LOG_CORE,
				"Failed to query device_attr (driver) for unit %d: %s\n",
				unit, info->nic->device_attr->name);
			continue;
		}
		info->nic->device_attr->driver = strdup(buffer);

		/* ----------------------------------------------------------------------- */
		/* Link Attr */
		if (PSM2_OK != psmx3_get_link_speed(unit_id, port, &speed, info))
			continue;
		info->nic->link_attr->speed = (size_t)speed;
	}
}

static int init_calls;
static int psmx3_getinfo(uint32_t api_version, const char *node,
			 const char *service, uint64_t flags,
			 const struct fi_info *hints, struct fi_info **info)
{
	struct fi_info *prov_info = NULL;
	struct psmx3_ep_name *dest_addr = NULL;
	struct psmx3_ep_name *src_addr = NULL;
	size_t len;
	void *addr;
	uint32_t fmt;

	PSMX3_INFO(&psmx3_prov, FI_LOG_CORE,"\n");

	if (psmx3_init_prov_info(hints, &prov_info))
		goto err_out;

	if (psmx3_init_lib())
		goto err_out;

	if (psmx3_update_hfi_info())
		goto err_out;

	if (!psmx3_domain_info.num_reported_units) {
		PSMX3_INFO(&psmx3_prov, FI_LOG_CORE,
			"no PSM3 device is active.\n");
		goto err_out;
	}

	init_calls += 1;

	/* when available, default domain and fabric names are a superset
	 * of all individual names, so we can do a substr search as a 1st level
	 * filter
	 */
	if (hints && hints->domain_attr && hints->domain_attr->name &&
		psmx3_domain_info.default_domain_name[0] &&
		NULL == strcasestr(psmx3_domain_info.default_domain_name, hints->domain_attr->name)) {
		PSMX3_INFO(&psmx3_prov, FI_LOG_CORE, "Unknown domain name\n");
		OFI_INFO_STR(&psmx3_prov, psmx3_domain_info.default_domain_name,
					   hints->domain_attr->name, "Supported", "Requested");
		goto err_out;
	}

	if (hints && hints->fabric_attr && hints->fabric_attr->name &&
		psmx3_domain_info.default_fabric_name[0] &&
		NULL == strcasestr(psmx3_domain_info.default_fabric_name, hints->fabric_attr->name)) {
		PSMX3_INFO(&psmx3_prov, FI_LOG_CORE, "Unknown fabric name\n");
		OFI_INFO_STR(&psmx3_prov, psmx3_domain_info.default_fabric_name,
					   hints->fabric_attr->name, "Supported", "Requested");
		goto err_out;
	}

	/* Set src or dest to used supplied address in native format */
	if (node &&
	    !ofi_str_toaddr(node, &fmt, &addr, &len) &&
	    fmt == FI_ADDR_PSMX3) {
		if (flags & FI_SOURCE) {
			src_addr = addr;
			PSMX3_INFO(&psmx3_prov, FI_LOG_CORE,
				"'%s' is taken as src_addr: <unit=%d, port=%d, service=%d>\n",
				node, src_addr->unit, src_addr->port, src_addr->service);
		} else {
			dest_addr = addr;
			PSMX3_INFO(&psmx3_prov, FI_LOG_CORE,
				"'%s' is taken as dest_addr: <epid=%s>\n",
				node, psm3_epid_fmt(dest_addr->epid, 0));
		}
		node = NULL;
	}

	/* Initialize src address based on the "host:unit:port" format */
	if (!src_addr) {
		src_addr = calloc(1, sizeof(*src_addr));
		if (!src_addr) {
			PSMX3_INFO(&psmx3_prov, FI_LOG_CORE,
				"failed to allocate src addr.\n");
			goto err_out;
		}
		src_addr->type = PSMX3_EP_SRC_ADDR;
		src_addr->epid = psm3_epid_zeroed();
		src_addr->unit = PSMX3_DEFAULT_UNIT;
		src_addr->port = PSMX3_DEFAULT_PORT;
		src_addr->service = PSMX3_ANY_SERVICE;

		if (flags & FI_SOURCE) {
			if (node) {
				if (sscanf(node, "%*[^:]:%" SCNi8 ":%" SCNu8,
				           &src_addr->unit, &src_addr->port) != 2) {
					PSMX3_INFO(&psmx3_prov, FI_LOG_CORE, "Invalid string representation for node.\n");
				}
			}
			if (service) {
				if (sscanf(service, "%" SCNu32, &src_addr->service) != 1) {
					PSMX3_INFO(&psmx3_prov, FI_LOG_CORE, "Invalid string representation for service.\n");
				}
			}
			PSMX3_INFO(&psmx3_prov, FI_LOG_CORE,
				"node '%s' service '%s' converted to <unit=%d, port=%d, service=%d>\n",
				node, service, src_addr->unit, src_addr->port, src_addr->service);
		}
	}

	/* Check that the src address contains valid unit */
	if (src_addr->unit != PSMX3_DEFAULT_UNIT) {
		if (src_addr->unit < 0 || src_addr->unit >= PSMX3_MAX_UNITS) {
			PSMX3_INFO(&psmx3_prov, FI_LOG_CORE,
				"invalid source address: unit %d out of range\n", src_addr->unit);
			goto err_out;
		}
		if (!psmx3_domain_info.unit_is_active[src_addr->unit]) {
			PSMX3_INFO(&psmx3_prov, FI_LOG_CORE,
				"invalid source address: unit %d is inactive\n", src_addr->unit);
			goto err_out;
		}
	}

	/* Resovle dest address using "node", "service" pair */
	if (!dest_addr && node && !(flags & FI_SOURCE)) {
		dest_addr = psmx3_lookup(node, service);
		if (!dest_addr)
			goto err_out;
	}

	/* Update prov info with resovled addresses and hfi info */
	psmx3_update_prov_info(prov_info, src_addr, dest_addr);

	/* Remove prov info that don't match the hints */
	if (psmx3_check_prov_info(api_version, hints, &prov_info))
		goto err_out;

	/* Apply hints to the prov info */
	psmx3_alter_prov_info(api_version, hints, prov_info);

	/* Set fi_nic struture */
	psmx3_update_nic_info(prov_info);

	*info = prov_info;
	free(src_addr);
	free(dest_addr);
	if (hints || init_calls >= 2) {
		psm3_turn_off_init_cache();
	}
	return 0;

err_out:
	free(src_addr);
	free(dest_addr);
	fi_freeinfo(prov_info);
	*info = NULL;
	return -FI_ENODATA;
}

static void psmx3_fini(void)
{
	PSMX3_INFO(&psmx3_prov, FI_LOG_CORE, "\n");

	if (! --psmx3_init_count) {
		/* psm3_env_finalize is called by psm3_finailize, only need to call
		 * when we don't call psm3_finalize
		 */
		if (psmx3_lib_initialized) {
			/* This function is called from a library destructor, which is
			 * called automatically when exit() is called. The call to
			 * psm3_finalize() might cause deadlock if the applicaiton is
			 * terminated with Ctrl-C
			 * -- the application could be inside a PSM3 call, holding a lock
			 * that psm3_finalize() tries to acquire. This can be avoided by
			 * only calling psm3_finalize() when PSM3 is guaranteed to be
			 * unused.
			 */
			if (psmx3_active_fabric) {
				PSMX3_INFO(&psmx3_prov, FI_LOG_CORE,
					"psmx3_active_fabric != NULL, skip psm3_finalize\n");
			} else {
				psm3_finalize();
				psmx3_lib_initialized = 0;
			}
		} else {
			psm3_env_finalize();
		}
	}
}

struct fi_provider psmx3_prov = {
	.name = PSMX3_PROV_NAME,
	.fi_version = OFI_VERSION_LATEST,
	.getinfo = psmx3_getinfo,
	.fabric = psmx3_fabric,
	.cleanup = psmx3_fini
};

PROVIDER_INI
{
	psmx3_prov.version = get_psm3_provider_version();

	PSMX3_INFO(&psmx3_prov, FI_LOG_CORE, "build options: VERSION=%u.%u=%u.%u.%u.%u, "
			"HAVE_PSM3_SRC=%d, PSM3_CUDA=%d\n",
			(psmx3_prov.version >> 16), (psmx3_prov.version & 0xFFFF),
			(psmx3_prov.version >> 16) / 100, (psmx3_prov.version >> 16) % 100,
			(psmx3_prov.version & 0xFFFF) / 1000, ((psmx3_prov.version & 0xFFFF) % 1000) / 10,
			HAVE_PSM3_SRC, PSM3_CUDA);

	fi_param_define(&psmx3_prov, "name_server", FI_PARAM_BOOL,
			FI_PSM3_NAME_SERVER_HELP);

	fi_param_define(&psmx3_prov, "tagged_rma", FI_PARAM_BOOL,
			FI_PSM3_TAGGED_RMA_HELP);

	fi_param_define(&psmx3_prov, "uuid", FI_PARAM_STRING,
			FI_PSM3_UUID_HELP);

	fi_param_define(&psmx3_prov, "delay", FI_PARAM_INT,
			FI_PSM3_DELAY_HELP);

	fi_param_define(&psmx3_prov, "timeout", FI_PARAM_INT,
			FI_PSM3_TIMEOUT_HELP);

	fi_param_define(&psmx3_prov, "conn_timeout", FI_PARAM_INT,
			FI_PSM3_CONN_TIMEOUT_HELP);

	fi_param_define(&psmx3_prov, "prog_interval", FI_PARAM_INT,
			FI_PSM3_PROG_INTERVAL_HELP);

	fi_param_define(&psmx3_prov, "prog_affinity", FI_PARAM_STRING,
			FI_PSM3_PROG_AFFINITY_HELP);

	fi_param_define(&psmx3_prov, "inject_size", FI_PARAM_INT,
			FI_PSM3_INJECT_SIZE_HELP);

	fi_param_define(&psmx3_prov, "lock_level", FI_PARAM_INT,
			FI_PSM3_LOCK_LEVEL_HELP);

	fi_param_define(&psmx3_prov, "lazy_conn", FI_PARAM_BOOL,
			FI_PSM3_LAZY_CONN_HELP);

	fi_param_define(&psmx3_prov, "disconnect", FI_PARAM_BOOL,
			FI_PSM3_DISCONNECT_HELP);

#if (PSMX3_TAG_LAYOUT == PSMX3_TAG_LAYOUT_RUNTIME)
	fi_param_define(&psmx3_prov, "tag_layout", FI_PARAM_STRING,
			FI_PSM3_TAG_LAYOUT_HELP);
#endif
	fi_param_define(&psmx3_prov, "yield_mode", FI_PARAM_BOOL,
			FI_PSM3_YIELD_MODE_HELP);
	fi_param_define(&psmx3_prov, "wait_enable", FI_PARAM_BOOL,
			FI_PSM3_WAIT_ENABLE_HELP);

	psmx3_init_env();

	pthread_mutex_init(&psmx3_lib_mutex, NULL);
	psmx3_init_count++;
	return (&psmx3_prov);
}

