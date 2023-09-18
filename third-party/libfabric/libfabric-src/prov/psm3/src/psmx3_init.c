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

struct psmx3_env psmx3_env = {
	.name_server	= 1,
	.tagged_rma	= 1,
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

	if (getenv("OMPI_COMM_WORLD_RANK") || getenv("PMI_RANK") || getenv("PMIX_RANK"))
		psmx3_env.name_server = 0;

	fi_param_get_bool(&psmx3_prov, "name_server", &psmx3_env.name_server);
	fi_param_get_bool(&psmx3_prov, "tagged_rma", &psmx3_env.tagged_rma);

	if (FI_SUCCESS != fi_param_get_str(&psmx3_prov, "uuid", &psmx3_env.uuid)) {
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
			FI_INFO(&psmx3_prov, FI_LOG_CORE,
				"Open MPI job key: %s.\n", ompi_job_key);
			if (sscanf(ompi_job_key, "%016llx-%016llx", &u[0], &u[1]) == 2)
				uuid = strdup(psmx3_uuid_to_string(ompi_uuid));
			else {
				FI_INFO(&psmx3_prov, FI_LOG_CORE,
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
	fi_param_get_int(&psmx3_prov, "delay", &psmx3_env.delay);
	fi_param_get_int(&psmx3_prov, "timeout", &psmx3_env.timeout);
	fi_param_get_int(&psmx3_prov, "prog_interval", &psmx3_env.prog_interval);
	fi_param_get_str(&psmx3_prov, "prog_affinity", &psmx3_env.prog_affinity);
	fi_param_get_int(&psmx3_prov, "inject_size", &psmx3_env.inject_size);
	fi_param_get_bool(&psmx3_prov, "lock_level", &psmx3_env.lock_level);
	fi_param_get_bool(&psmx3_prov, "lazy_conn", &psmx3_env.lazy_conn);
	if (psmx3_env.lazy_conn)
		psmx3_env.conn_timeout = 30;	// more headroom since app may be busy
	fi_param_get_int(&psmx3_prov, "conn_timeout", &psmx3_env.conn_timeout);
	fi_param_get_bool(&psmx3_prov, "disconnect", &psmx3_env.disconnect);
#if (PSMX3_TAG_LAYOUT == PSMX3_TAG_LAYOUT_RUNTIME)
	fi_param_get_str(&psmx3_prov, "tag_layout", &psmx3_env.tag_layout);
#endif
}

void psmx3_init_tag_layout(struct fi_info *info)
{
	int use_tag64;

#if (PSMX3_TAG_LAYOUT == PSMX3_TAG_LAYOUT_RUNTIME)
	use_tag64 = (psmx3_tag_mask == PSMX3_TAG_MASK_64);

	if (psmx3_tag_layout_locked) {
		FI_INFO(&psmx3_prov, FI_LOG_CORE,
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
			FI_INFO(&psmx3_prov, FI_LOG_CORE,
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
	FI_INFO(&psmx3_prov, FI_LOG_CORE,
		"use %s: tag_mask: %016" PRIX64 ", data_mask: %08" PRIX32 "\n",
		use_tag64 ? "tag64" : "tag60", (uint64_t)PSMX3_TAG_MASK,
		PSMX3_DATA_MASK);
}

static int psmx3_get_yes_no(char *s, int default_value)
{
	unsigned long value;
	char *end_ptr;

	if (!s || s[0] == '\0')
		return default_value;

	if (s[0] == 'Y' || s[0] == 'y')
		return 1;

	if (s[0] == 'N' || s[0] == 'n')
		return 0;

	value = strtoul(s, &end_ptr, 0);
	if (end_ptr == s)
		return default_value;

	return value ? 1 : 0;
}

static int psmx3_check_multi_ep_cap(void)
{
	uint64_t caps = PSM2_MULTI_EP_CAP;
	char *s = getenv("PSM3_MULTI_EP");

	if (psm3_get_capability_mask(caps) == caps && psmx3_get_yes_no(s, 1))
		psmx3_env.multi_ep = 1;
	else
		psmx3_env.multi_ep = 0;

	return psmx3_env.multi_ep;
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
		FI_WARN(&psmx3_prov, FI_LOG_CORE,
			"psm3_init failed: %s\n", psm3_error_get_string(err));
		ret = err;
		goto out;
	}

	FI_INFO(&psmx3_prov, FI_LOG_CORE,
		"PSM3 header version = (%d, %d)\n", PSM2_VERNO_MAJOR, PSM2_VERNO_MINOR);
	FI_INFO(&psmx3_prov, FI_LOG_CORE,
		"PSM3 library version = (%d, %d)\n", major, minor);

	if (psmx3_check_multi_ep_cap())
		FI_INFO(&psmx3_prov, FI_LOG_CORE, "PSM3 multi-ep feature enabled.\n");
	else
		FI_INFO(&psmx3_prov, FI_LOG_CORE, "PSM3 multi-ep feature not available or disabled.\n");

	psmx3_lib_initialized = 1;

out:
	pthread_mutex_unlock(&psmx3_lib_mutex);
	return ret;
}

static int psmx3_update_hfi_info(void)
{
	unsigned short i, j, psmx3_unit;
	int nctxts = 0;
	int nfreectxts = 0;
	int multirail = 0;
	int counted_unit;
	char *s;
	char unit_name[NAME_MAX];
	char fabric_name[NAME_MAX];
	uint32_t cnt = 0;
	uint32_t addr_cnt = 0;
	int tmp_nctxts, tmp_nfreectxts;
	int unit_active;
	int ret;
	psm2_info_query_arg_t args[4];
	char first_active_unit_name[NAME_MAX];
	char first_active_unit_fabric_name[NAME_MAX];

	if (psmx3_domain_info.num_units > 0)
		return 0;

	if (psm3_info_query(PSM2_INFO_QUERY_NUM_UNITS, &cnt, 0, NULL)
			|| !cnt) {
		FI_INFO(&psmx3_prov, FI_LOG_CORE,
			"no PSM3 device is found.\n");
		return -FI_ENODEV;
	} else if (cnt > PSMX3_MAX_UNITS) {
		FI_WARN(&psmx3_prov, FI_LOG_CORE,
			"Discovered %u devices, limited to 1st %u\n", cnt, PSMX3_MAX_UNITS);
		cnt = PSMX3_MAX_UNITS;
	}
	if (psm3_info_query(PSM2_INFO_QUERY_NUM_ADDR_PER_UNIT, &addr_cnt, 0, NULL) || !addr_cnt)
	{
		FI_INFO(&psmx3_prov, FI_LOG_CORE,
			"no PSM3 device is found (no addr per unit).\n");
		return -FI_ENODEV;
	}
	psmx3_domain_info.num_units = cnt * addr_cnt;
	if (psmx3_domain_info.num_units > PSMX3_MAX_UNITS) {
		FI_WARN(&psmx3_prov, FI_LOG_CORE,
			"Too many addresses per PSM3 device. For %d physical "
			"devices only  %d addresses per device are supported.",
			cnt, PSMX3_MAX_UNITS/cnt);
		return-FI_ENODEV;
	}

	s = getenv("PSM3_MULTIRAIL");
	if (s)
		multirail = atoi(s);

	psmx3_domain_info.num_reported_units = 0;
	psmx3_domain_info.num_active_units = 0;
	for (i = 0; i < cnt; i++) {
		args[0].unit = i;
		ret = psm3_info_query(PSM2_INFO_QUERY_UNIT_STATUS, &unit_active, 1, args);
		if (ret != PSM2_OK) {
			FI_WARN(&psmx3_prov, FI_LOG_CORE,
				"Failed to check active state of HFI unit_id %d\n",
				i);
			continue;
		}

		if (unit_active<=0) {
			FI_WARN(&psmx3_prov, FI_LOG_CORE,
				"NIC unit_id %d STATE = INACTIVE\n",
				i);
			continue;
		}

		if (PSM2_OK != psm3_info_query(PSM2_INFO_QUERY_NUM_FREE_CONTEXTS,
						&tmp_nfreectxts, 1, args) || (tmp_nfreectxts < 0))
		{
			FI_WARN(&psmx3_prov, FI_LOG_CORE,
				"Failed to read number of free contexts from HFI unit_id %d\n",
				i);
			continue;
		}

		if (PSM2_OK != psm3_info_query(PSM2_INFO_QUERY_NUM_CONTEXTS,
						&tmp_nctxts, 1, args) || (tmp_nctxts < 0))
		{
			FI_WARN(&psmx3_prov, FI_LOG_CORE,
				"Failed to read number of contexts from HFI unit_id %d\n",
				i);
			continue;
		}

		counted_unit = 0;
		for (j=0; j < addr_cnt; j++) {
			psmx3_unit = i * addr_cnt + j;
			args[1].port = 1;	// VERBS_PORT
			args[2].addr_index = j;

			args[3].length = sizeof(unit_name);
			if (PSM2_OK != psm3_info_query(PSM2_INFO_QUERY_UNIT_ADDR_NAME,
							unit_name, 4, args))
			{
				FI_WARN(&psmx3_prov, FI_LOG_CORE,
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
				FI_WARN(&psmx3_prov, FI_LOG_CORE,
					"Failed to read name of HFI unit_id %d addr_index %d subnet\n",
					i, j);
				continue;
			}

			if (! counted_unit) {
				nctxts += tmp_nctxts;
				nfreectxts += tmp_nfreectxts;
				counted_unit = 1;
			}

			psmx3_domain_info.num_active_units++;

			/* for PSM3_MULTIRAIL only report 1 "autoselect" unit */
			if (psmx3_domain_info.num_reported_units < 1 || multirail <= 0) {
				psmx3_domain_info.unit_is_active[psmx3_unit] = 1;
				psmx3_domain_info.unit_id[psmx3_unit] = i;
				psmx3_domain_info.addr_index[psmx3_unit] = j;
				psmx3_domain_info.unit_nctxts[psmx3_unit] = tmp_nctxts;
				psmx3_domain_info.unit_nfreectxts[psmx3_unit] = tmp_nfreectxts;
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

	FI_INFO(&psmx3_prov, FI_LOG_CORE,
		"hfi1 units: total %d, reported %d, active %d; "
		"hfi1 contexts: total %d, free %d\n",
		psmx3_domain_info.num_units, psmx3_domain_info.num_reported_units,
		psmx3_domain_info.num_active_units, nctxts, nfreectxts);

	if (psmx3_env.multi_ep) {
		psmx3_domain_info.max_trx_ctxt = nctxts;
		psmx3_domain_info.free_trx_ctxt = nfreectxts;
	} else {
		psmx3_domain_info.max_trx_ctxt = 1;
		psmx3_domain_info.free_trx_ctxt = (nfreectxts == 0) ? 0 : 1;
	}

	FI_INFO(&psmx3_prov, FI_LOG_CORE,
		"Tx/Rx contexts: %d in total, %d available.\n",
		psmx3_domain_info.max_trx_ctxt, psmx3_domain_info.free_trx_ctxt);

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
		FI_WARN(&psmx3_prov, FI_LOG_CORE,
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
					FI_WARN(&psmx3_prov, FI_LOG_CORE,
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
			FI_WARN(&psmx3_prov, FI_LOG_CORE,
				"Failed to query bus_attr (pci bus) for unit %d\n", unit);
			continue;
		}

		if (!info->nic) {
			info->nic = ofi_nic_dup(NULL);
			if (!info->nic) {
				FI_WARN(&psmx3_prov, FI_LOG_CORE,
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
			FI_WARN(&psmx3_prov, FI_LOG_CORE,
				"Failed to query device_attr (name) for unit %d\n", unit);
			continue;
		}
		info->nic->device_attr->name = strdup(buffer);

		if (PSM2_OK != psm3_info_query(PSM2_INFO_QUERY_UNIT_DEVICE_ID, buffer, 2, args)) {
			FI_WARN(&psmx3_prov, FI_LOG_CORE,
				"Failed to query device_attr (device_id) for unit %d: %s\n",
				unit, info->nic->device_attr->name);
			continue;
		}
		info->nic->device_attr->device_id = strdup(buffer);

		if (PSM2_OK != psm3_info_query(PSM2_INFO_QUERY_UNIT_DEVICE_VERSION, buffer, 2, args)) {
			FI_WARN(&psmx3_prov, FI_LOG_CORE,
				"Failed to query device_attr (device_version) for unit %d: %s\n",
				unit, info->nic->device_attr->name);
			continue;
		}
		info->nic->device_attr->device_version = strdup(buffer);

		if (PSM2_OK != psm3_info_query(PSM2_INFO_QUERY_UNIT_VENDOR_ID, buffer, 2, args)) {
			FI_WARN(&psmx3_prov, FI_LOG_CORE,
				"Failed to query device_attr (vendor_id) for unit %d: %s\n",
				unit, info->nic->device_attr->name);
			continue;
		}
		info->nic->device_attr->vendor_id = strdup(buffer);

		if (PSM2_OK != psm3_info_query(PSM2_INFO_QUERY_UNIT_DRIVER, buffer, 2, args)) {
			FI_WARN(&psmx3_prov, FI_LOG_CORE,
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

static int psmx3_getinfo(uint32_t api_version, const char *node,
			 const char *service, uint64_t flags,
			 const struct fi_info *hints, struct fi_info **info)
{
	struct fi_info *prov_info = NULL;
	struct psmx3_ep_name *dest_addr = NULL;
	struct psmx3_ep_name *src_addr = NULL;
	int svc0, svc = PSMX3_ANY_SERVICE;
	size_t len;
	void *addr;
	uint32_t fmt;

	FI_INFO(&psmx3_prov, FI_LOG_CORE,"\n");

	if (psmx3_init_prov_info(hints, &prov_info))
		goto err_out;

	if (psmx3_init_lib())
		goto err_out;

	if (psmx3_update_hfi_info())
		goto err_out;

	if (!psmx3_domain_info.num_reported_units) {
		FI_INFO(&psmx3_prov, FI_LOG_CORE,
			"no PSM3 device is active.\n");
		goto err_out;
	}

	/* when available, default domain and fabric names are a superset
	 * of all individual names, so we can do a substr search as a 1st level
	 * filter
	 */
	if (hints && hints->domain_attr && hints->domain_attr->name &&
		psmx3_domain_info.default_domain_name[0] &&
		NULL == strcasestr(psmx3_domain_info.default_domain_name, hints->domain_attr->name)) {
		FI_INFO(&psmx3_prov, FI_LOG_CORE, "Unknown domain name\n");
		OFI_INFO_STR(&psmx3_prov, psmx3_domain_info.default_domain_name,
					   hints->domain_attr->name, "Supported", "Requested");
		goto err_out;
	}

	if (hints && hints->fabric_attr && hints->fabric_attr->name &&
		psmx3_domain_info.default_fabric_name[0] &&
		NULL == strcasestr(psmx3_domain_info.default_fabric_name, hints->fabric_attr->name)) {
		FI_INFO(&psmx3_prov, FI_LOG_CORE, "Unknown fabric name\n");
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
			FI_INFO(&psmx3_prov, FI_LOG_CORE,
				"'%s' is taken as src_addr: <unit=%d, port=%d, service=%d>\n",
				node, src_addr->unit, src_addr->port, src_addr->service);
		} else {
			dest_addr = addr;
			FI_INFO(&psmx3_prov, FI_LOG_CORE,
				"'%s' is taken as dest_addr: <epid=%s>\n",
				node, psm3_epid_fmt(dest_addr->epid, 0));
		}
		node = NULL;
	}

	/* Initialize src address based on the "host:unit:port" format */
	if (!src_addr) {
		src_addr = calloc(1, sizeof(*src_addr));
		if (!src_addr) {
			FI_INFO(&psmx3_prov, FI_LOG_CORE,
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
					FI_INFO(&psmx3_prov, FI_LOG_CORE, "Invalid string representation for node.\n");
				}
			}
			if (service) {
				if (sscanf(service, "%" SCNu32, &src_addr->service) != 1) {
					FI_INFO(&psmx3_prov, FI_LOG_CORE, "Invalid string representation for service.\n");
				}
			}
			FI_INFO(&psmx3_prov, FI_LOG_CORE,
				"node '%s' service '%s' converted to <unit=%d, port=%d, service=%d>\n",
				node, service, src_addr->unit, src_addr->port, src_addr->service);
		}
	}

	/* Check that the src address contains valid unit */
	if (src_addr->unit != PSMX3_DEFAULT_UNIT) {
		if (src_addr->unit < 0 || src_addr->unit >= PSMX3_MAX_UNITS) {
			FI_INFO(&psmx3_prov, FI_LOG_CORE,
				"invalid source address: unit %d out of range\n", src_addr->unit);
			goto err_out;
		}
		if (!psmx3_domain_info.unit_is_active[src_addr->unit]) {
			FI_INFO(&psmx3_prov, FI_LOG_CORE,
				"invalid source address: unit %d is inactive\n", src_addr->unit);
			goto err_out;
		}
	}

	/* Resovle dest address using "node", "service" pair */
	if (!dest_addr && node && !(flags & FI_SOURCE)) {
		psm2_uuid_t uuid;

		psmx3_get_uuid(uuid);
		struct util_ns ns = {
			.port = psmx3_uuid_to_port(uuid),
			.name_len = sizeof(*dest_addr),
			.service_len = sizeof(svc),
			.service_cmp = psmx3_ns_service_cmp,
			.is_service_wildcard = psmx3_ns_is_service_wildcard,
		};
		ofi_ns_init(&ns);

		if (service)
			svc = atoi(service);
		svc0 = svc;
		dest_addr = (struct psmx3_ep_name *)
			ofi_ns_resolve_name(&ns, node, &svc);
		if (dest_addr) {
			FI_INFO(&psmx3_prov, FI_LOG_CORE,
				"'%s:%u' resolved to <epid=%s>:%d\n",
				node, svc0, psm3_epid_fmt(dest_addr->epid, 0), svc);
		} else {
			FI_INFO(&psmx3_prov, FI_LOG_CORE,
				"failed to resolve '%s:%u'.\n", node, svc);
			goto err_out;
		}
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
	FI_INFO(&psmx3_prov, FI_LOG_CORE, "\n");

	if (! --psmx3_init_count && psmx3_lib_initialized) {
		/* This function is called from a library destructor, which is called
		 * automatically when exit() is called. The call to psm3_finalize()
		 * might cause deadlock if the applicaiton is terminated with Ctrl-C
		 * -- the application could be inside a PSM3 call, holding a lock that
		 * psm3_finalize() tries to acquire. This can be avoided by only
		 * calling psm3_finalize() when PSM3 is guaranteed to be unused.
		 */
		if (psmx3_active_fabric) {
			FI_INFO(&psmx3_prov, FI_LOG_CORE,
				"psmx3_active_fabric != NULL, skip psm3_finalize\n");
		} else {
			psm3_finalize();
			psmx3_lib_initialized = 0;
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

	FI_INFO(&psmx3_prov, FI_LOG_CORE, "build options: VERSION=%u.%u=%u.%u.%u.%u, "
			"HAVE_PSM3_SRC=%d, PSM3_CUDA=%d\n",
			(psmx3_prov.version >> 16), (psmx3_prov.version & 0xFFFF),
			(psmx3_prov.version >> 16) / 100, (psmx3_prov.version >> 16) % 100,
			(psmx3_prov.version & 0xFFFF) / 1000, ((psmx3_prov.version & 0xFFFF) % 1000) / 10,
			HAVE_PSM3_SRC, PSM3_CUDA);

	fi_param_define(&psmx3_prov, "name_server", FI_PARAM_BOOL,
			"Whether to turn on the name server or not "
			"(default: yes)");

	fi_param_define(&psmx3_prov, "tagged_rma", FI_PARAM_BOOL,
			"Whether to use tagged messages for large size "
			"RMA or not (default: yes)");

	fi_param_define(&psmx3_prov, "uuid", FI_PARAM_STRING,
			"Unique Job ID required by the fabric");

	fi_param_define(&psmx3_prov, "delay", FI_PARAM_INT,
			"Delay (seconds) before finalization (for debugging)");

	fi_param_define(&psmx3_prov, "timeout", FI_PARAM_INT,
			"Timeout (seconds) for gracefully closing the PSM3 endpoint");

	fi_param_define(&psmx3_prov, "conn_timeout", FI_PARAM_INT,
			"Timeout (seconds) for establishing connection between two PSM3 endpoints");

	fi_param_define(&psmx3_prov, "prog_interval", FI_PARAM_INT,
			"Interval (microseconds) between progress calls made in the "
			"progress thread (default: 1 if affinity is set, 1000 if not)");

	fi_param_define(&psmx3_prov, "prog_affinity", FI_PARAM_STRING,
			"When set, specify the set of CPU cores to set the progress "
			"thread affinity to. The format is "
			"<start>[:<end>[:<stride>]][,<start>[:<end>[:<stride>]]]*, "
			"where each triplet <start>:<end>:<stride> defines a block "
			"of core_ids. Both <start> and <end> can be either the core_id "
			"(when >=0) or core_id - num_cores (when <0). "
			"(default: affinity not set)");

	fi_param_define(&psmx3_prov, "inject_size", FI_PARAM_INT,
			"Maximum message size for fi_inject and fi_tinject (default: 64).");

	fi_param_define(&psmx3_prov, "lock_level", FI_PARAM_INT,
			"How internal locking is used. 0 means no locking. (default: 2).");

	fi_param_define(&psmx3_prov, "lazy_conn", FI_PARAM_BOOL,
			"Whether to force lazy connection mode. (default: no).");

	fi_param_define(&psmx3_prov, "disconnect", FI_PARAM_BOOL,
			"Whether to issue disconnect request when process ends (default: no).");

#if (PSMX3_TAG_LAYOUT == PSMX3_TAG_LAYOUT_RUNTIME)
	fi_param_define(&psmx3_prov, "tag_layout", FI_PARAM_STRING,
			"How the 96 bit PSM3 tag is organized: "
			"tag60 means 32/4/60 for data/flags/tag;"
			"tag64 means 4/28/64 for flags/data/tag (default: tag60).");
#endif

	psmx3_init_env();

	pthread_mutex_init(&psmx3_lib_mutex, NULL);
	psmx3_init_count++;
	return (&psmx3_prov);
}

