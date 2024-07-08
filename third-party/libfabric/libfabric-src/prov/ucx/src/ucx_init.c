/*
 * Copyright (c) 2016-2023 Intel Corporation. All rights reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * OpenFabrics.org BSD license below:
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

#include "ucx.h"
#include <dirent.h>

#define UCX_VENDOR_ID 0x15b3

struct ucx_global_descriptor ucx_descriptor = {
	.config = NULL,
	.use_ns = 0,
	.ns_port = FI_UCX_DEFAULT_NS_PORT,
	.localhost = NULL,
	.ep_flush = 1,
	.check_req_leak = 0,
};

/*
 * UCS status values are mostly negative, except for 0 (UCS_OK) and
 * 1 (UCS_INPROGRESS). Need to offset the table by 1.
 */
#define UCX_ERRCODE_ENTRY(X) ucx_errcode_translation_table[(-X)+1]

int ucx_errcode_translation_table[(-UCS_ERR_LAST)+1];

static void ucx_init_errcodes()
{
	int i;

	for (i = 0; i < (-UCS_ERR_LAST) + 1; i++)
		ucx_errcode_translation_table[i] = -FI_EOTHER;

	UCX_ERRCODE_ENTRY(UCS_OK)                  	= -FI_SUCCESS;
	UCX_ERRCODE_ENTRY(UCS_INPROGRESS)          	= -FI_EINPROGRESS;
	UCX_ERRCODE_ENTRY(UCS_ERR_NO_MESSAGE)      	= -FI_ENOMSG;
	UCX_ERRCODE_ENTRY(UCS_ERR_NO_RESOURCE)     	= -FI_EINVAL;
	UCX_ERRCODE_ENTRY(UCS_ERR_IO_ERROR)        	= -FI_EIO;
	UCX_ERRCODE_ENTRY(UCS_ERR_NO_MEMORY)       	= -FI_ENOMEM;
	UCX_ERRCODE_ENTRY(UCS_ERR_INVALID_PARAM)   	= -FI_EINVAL;
	UCX_ERRCODE_ENTRY(UCS_ERR_UNREACHABLE)     	= -FI_ENETUNREACH;
	UCX_ERRCODE_ENTRY(UCS_ERR_INVALID_ADDR)    	= -FI_EINVAL;
	UCX_ERRCODE_ENTRY(UCS_ERR_NOT_IMPLEMENTED) 	= -FI_ENOSYS;
	UCX_ERRCODE_ENTRY(UCS_ERR_MESSAGE_TRUNCATED)	= -FI_ETRUNC;
	UCX_ERRCODE_ENTRY(UCS_ERR_NO_PROGRESS)     	= -FI_EAGAIN;
	UCX_ERRCODE_ENTRY(UCS_ERR_BUFFER_TOO_SMALL)	= -FI_ETOOSMALL;
	UCX_ERRCODE_ENTRY(UCS_ERR_NO_ELEM)         	= -FI_ENOENT;
	UCX_ERRCODE_ENTRY(UCS_ERR_SOME_CONNECTS_FAILED)	= -FI_EIO;
	UCX_ERRCODE_ENTRY(UCS_ERR_NO_DEVICE)       	= -FI_ENODEV;
	UCX_ERRCODE_ENTRY(UCS_ERR_BUSY)            	= -FI_EBUSY;
	UCX_ERRCODE_ENTRY(UCS_ERR_CANCELED)        	= -FI_ECANCELED;
	UCX_ERRCODE_ENTRY(UCS_ERR_SHMEM_SEGMENT)   	= -FI_EINVAL;
	UCX_ERRCODE_ENTRY(UCS_ERR_ALREADY_EXISTS)  	= -EEXIST;
	UCX_ERRCODE_ENTRY(UCS_ERR_OUT_OF_RANGE)    	= -FI_EINVAL;
	UCX_ERRCODE_ENTRY(UCS_ERR_TIMED_OUT)       	= -FI_ETIMEDOUT;
	UCX_ERRCODE_ENTRY(UCS_ERR_EXCEEDS_LIMIT)   	= -FI_E2BIG;
	UCX_ERRCODE_ENTRY(UCS_ERR_UNSUPPORTED)     	= -FI_ENOSYS;
}

static struct fi_domain_attr ucx_domain_attrs = {
	.domain = NULL,
	.name = FI_UCX_FABRIC_NAME,
	.threading = FI_THREAD_SAFE,
	.control_progress = FI_PROGRESS_AUTO,
	.data_progress = FI_PROGRESS_MANUAL,
	.resource_mgmt = FI_RM_ENABLED,
	.av_type = FI_AV_UNSPEC,
	.mr_mode = OFI_MR_BASIC_MAP | FI_MR_BASIC | FI_MR_RAW,
	.mr_key_size = FI_UCX_MAX_KEY_SIZE,
	.tx_ctx_cnt = 1,
	.rx_ctx_cnt = 1,
	.max_ep_tx_ctx = 1,
	.max_ep_rx_ctx = 1,
	.mr_cnt = FI_UCX_DEF_MR_CNT,
	.mr_iov_limit = 1,
	.caps = FI_UCX_DOM_CAPS,
};

static struct fi_rx_attr ucx_rx_attrs = {
	.caps = FI_UCX_CAPS,
	.mode = FI_UCX_MODE,
	.op_flags = FI_UCX_RX_FLAGS,
	.msg_order = FI_ORDER_SAS,
	.comp_order = FI_ORDER_NONE,
	.total_buffered_recv = ~(0ULL),
	.size = 384,
	.iov_limit = 4,
};

static struct fi_tx_attr ucx_tx_attrs = {
	.caps = FI_UCX_CAPS,
	.mode = FI_UCX_MODE,
	.op_flags = FI_UCX_TX_FLAGS,
	.msg_order = FI_ORDER_SAS,
	.comp_order = FI_ORDER_NONE,
	.inject_size = FI_UCX_DEFAULT_INJECT_SIZE, /* Should be setup after init */
	.size = 384,
	.iov_limit = 1,
	.rma_iov_limit = 1,
};

struct fi_fabric_attr ucx_fabric_attrs = {
	.name = FI_UCX_FABRIC_NAME,
	.prov_version = OFI_VERSION_DEF_PROV,
	.fabric = NULL
};

static struct fi_ep_attr ucx_ep_attrs = {
	.type = FI_EP_RDM,
	.protocol = FI_PROTO_UCX,
	.protocol_version = ((FI_UCX_VERSION_MAJOR << UCP_VERSION_MAJOR_SHIFT) |
			     (FI_UCX_VERSION_MINOR << UCP_VERSION_MINOR_SHIFT)),
	.max_msg_size = SIZE_MAX,
	.mem_tag_format = FI_TAG_GENERIC,
	.tx_ctx_cnt = 1,
	.rx_ctx_cnt = 1,
};

static struct fi_info ucx_info = {
	.caps = FI_UCX_CAPS,
	.mode = FI_UCX_MODE,
	.addr_format = FI_ADDR_UCX,
	.src_addrlen = 0,
	.dest_addr = 0,
	.tx_attr = &ucx_tx_attrs,
	.rx_attr = &ucx_rx_attrs,
	.ep_attr = &ucx_ep_attrs,
	.domain_attr = &ucx_domain_attrs,
	.fabric_attr = &ucx_fabric_attrs
};

struct util_prov ucx_util_prov = {
	.prov = &ucx_prov,
	.info = &ucx_info,
	.flags = 0,
};

static int ucx_getinfo(uint32_t version, const char *node,
		       const char *service, uint64_t flags,
		       const struct fi_info *hints, struct fi_info **info)
{
	int status = -ENODATA;
	char *configfile_name = NULL;
	int inject_thresh = -1;
	ucx_descriptor.config = NULL;
	char *tls = NULL;
	char *devices = NULL;
	static char *auto_val = "auto";
	int speed_gbps = 0;

	/* check the existance of ucx infiniband device */

	status = fi_param_get(&ucx_prov, "devices", &devices);
	if (status != FI_SUCCESS)
		devices = auto_val;

	if (strncmp(devices, auto_val, strlen(auto_val)) != 0) {
		setenv("UCX_NET_DEVICES", devices, 0);
	} else {
		char sysfs_path[1024] = { 0 };
		DIR *devices_dir = NULL;
		int ucx_device_found = 0;

		devices_dir = opendir("/sys/class/infiniband");
		if (devices_dir) {
			struct dirent *dir;
			FILE *fp;
			char *line;
			size_t len;
			int value;

			while ((dir = readdir(devices_dir)) != NULL) {
				if (!strcmp(dir->d_name, ".") ||
				    !strcmp(dir->d_name, ".."))
					continue;

				/* NVIDIA device is required */

				snprintf(sysfs_path, sizeof(sysfs_path),
					 "/sys/class/infiniband/%s/device/vendor",
					 dir->d_name);
				fp = fopen(sysfs_path, "r");
				if (!fp)
					continue;

				line = NULL;
				len = 0;
				value = 0;

				if (getline(&line, &len, fp) != -1 &&
				    sscanf(line, "%x", &value) == 1 &&
				    value == UCX_VENDOR_ID)
					ucx_device_found = 1;

				fclose(fp);

				if (!ucx_device_found)
					continue;

				FI_INFO(&ucx_prov, FI_LOG_CORE,
					"primary detected device: %s \n",
					dir->d_name);

				/* identify device speed */
				snprintf(sysfs_path, sizeof(sysfs_path),
					 "/sys/class/infiniband/%s/ports/1/rate",
					 dir->d_name);

				fp = fopen(sysfs_path, "r");
				if (fp) {
					line = NULL;
					len = 0;
					value = 0;

					if (getline(&line, &len, fp) != -1 &&
					    sscanf(line, "%d", &value) == 1 &&
					    value > 0)
						speed_gbps = value;

					fclose(fp);
				}
				break;
			}
			closedir(devices_dir);
		}

		if (!ucx_device_found) {
			FI_INFO(&ucx_prov, FI_LOG_CORE,
				"no ucx device is found.\n");
			status  = -FI_ENODEV;
			goto out;
		}
	}

	status = fi_param_get(&ucx_prov, "inject_limit", &inject_thresh);
	if (status != FI_SUCCESS)
		inject_thresh = FI_UCX_DEFAULT_INJECT_SIZE;

	FI_INFO(&ucx_prov, FI_LOG_CORE, "used inject size = %d \n",
		inject_thresh);
	ucx_tx_attrs.inject_size = inject_thresh;

	status = fi_param_get(&ucx_prov, "config", &configfile_name);
	if (status != FI_SUCCESS)
		configfile_name = NULL;

	/* NS is disabled by default */
	status = fi_param_get(&ucx_prov, "ns_enable", &ucx_descriptor.use_ns);
	if (status != FI_SUCCESS)
		ucx_descriptor.use_ns = 0;

	status = fi_param_get(&ucx_prov, "ns_port", &ucx_descriptor.ns_port);
	if (status != FI_SUCCESS)
		ucx_descriptor.ns_port = FI_UCX_DEFAULT_NS_PORT;

	status = fi_param_get(&ucx_prov, "tls", &tls);
	if (status != FI_SUCCESS)
		tls = auto_val;

	if (strncmp(tls, auto_val, strlen(auto_val)) != 0 &&
	    getenv("UCX_TLS"))
		setenv("UCX_TLS", tls, 0);

	status = fi_param_get(&ucx_prov, "ep_flush", &ucx_descriptor.ep_flush);
	if (status != FI_SUCCESS)
		ucx_descriptor.ep_flush = 1;

	status = fi_param_get(&ucx_prov, "check_req_leak", &ucx_descriptor.check_req_leak);
	if (status != FI_SUCCESS)
		ucx_descriptor.check_req_leak = 0;

	status = ucp_config_read(NULL, configfile_name, &ucx_descriptor.config);
	if (status != UCS_OK)
		FI_WARN(&ucx_prov, FI_LOG_CORE,
			"UCX error: invalid config file\n\t%d (%s)\n",
			status, ucs_status_string(status));

	FI_INFO(&ucx_prov, FI_LOG_CORE, "Loaded UCX version %s\n",
		ucp_get_version_string());

#if ENABLE_DEBUG
	{
		int extra_debug = 0;

		status = fi_param_get( &ucx_prov, "extra_debug", &extra_debug);
		if (status != FI_SUCCESS)
			extra_debug = 0;

		if (ucx_descriptor.config && extra_debug &&
		    fi_log_enabled(&ucx_prov, FI_LOG_INFO, FI_LOG_CORE))
			ucp_config_print(ucx_descriptor.config, stderr,
					 "Used UCX configuration", (1<<4)-1);
	}
#endif

	*info = NULL;
	if (node || service) {
		FI_WARN(&ucx_prov, FI_LOG_CORE,
			"fi_getinfo with non-NULL node or service is unsupported\n");
		node = service = NULL;
		flags = 0;
	}

	/* Only Pure UCX address and IPv4 are supported */
	if (hints) {
		if (!ucx_descriptor.use_ns ||
		    hints->addr_format == FI_ADDR_UCX ||
		    hints->addr_format == FI_FORMAT_UNSPEC) {
			ucx_info.addr_format = FI_ADDR_UCX;
		} else if (hints->addr_format <= FI_SOCKADDR_IN) {
			ucx_descriptor.use_ns = 1;
			ucx_info.addr_format = FI_SOCKADDR_IN;
		} else {
			FI_WARN(&ucx_prov, FI_LOG_CORE,
				"invalid addr_format requested\n");
			return -ENODATA;
		}
	} else {
		ucx_info.addr_format = ucx_descriptor.use_ns ?
						FI_SOCKADDR_IN : FI_ADDR_UCX;
	}

	status = fi_param_get(&ucx_prov, "enable_spawn",
			      &ucx_descriptor.enable_spawn);
	if (status != FI_SUCCESS)
		ucx_descriptor.enable_spawn = 0;

	FI_WARN(&ucx_prov, FI_LOG_CORE,
		"UCX: spawn support %d \n", ucx_descriptor.enable_spawn);

	status = util_getinfo(&ucx_util_prov, version, service, node, flags,
			      hints, info);
	if (*info) {
		(*info)->addr_format = ucx_info.addr_format;
		(*info)->nic = ofi_nic_dup(NULL);
		if ((*info)->nic)
			(*info)->nic->link_attr->speed =
				(size_t) speed_gbps * 1000 * 1000 * 1000;

		if (hints && hints->domain_attr &&
		    (hints->domain_attr->mr_mode & FI_MR_HMEM))
			(*info)->domain_attr->mr_mode |= FI_MR_HMEM;
	}

	/* make sure the memery hooks are installed for memory type cache */
	ucs_memtype_cache_lookup(0, 0, NULL);
out:
	return status;
}

static void ucx_cleanup(void)
{
#if HAVE_UCX_DL
        ofi_hmem_cleanup();
#endif

	FI_DBG(&ucx_prov, FI_LOG_CORE, "provider goes cleanup sequence\n");
	if (ucx_descriptor.config) {
		ucp_config_release(ucx_descriptor.config);
		ucx_descriptor.config = NULL;
	}
}

struct fi_provider ucx_prov = {
	.name = FI_UCX_FABRIC_NAME,
	.version = OFI_VERSION_DEF_PROV,
	.fi_version = OFI_VERSION_LATEST,
	.getinfo = ucx_getinfo,
	.fabric = ucx_fabric_open,
	.cleanup = ucx_cleanup,
};

UCX_INI
{
#if HAVE_UCX_DL
        ofi_hmem_init();
#endif

	ucx_init_errcodes();

	fi_param_define(&ucx_prov,
			"config", FI_PARAM_STRING,
			"UCX configuration file name");

	fi_param_define(&ucx_prov,
			"inject_limit", FI_PARAM_INT,
			"Maximal tinject/inject message size");

	fi_param_define(&ucx_prov,
			"ns_port", FI_PARAM_INT,
			"UCX Name server port");

	fi_param_define(&ucx_prov,
			"ns_enable",FI_PARAM_BOOL,
			"Enforce usage of name server for UCX provider");

	fi_param_define(&ucx_prov,
			"ep_flush",FI_PARAM_BOOL,
			"Use EP flush (Disabled by default)");

	fi_param_define(&ucx_prov,
			"ns_iface",FI_PARAM_STRING,
			"Specify IPv4 network interface for UCX provider's name server'");

	fi_param_define(&ucx_prov,
			"extra_debug",FI_PARAM_BOOL,
			"Output transport-level debug information");

	fi_param_define(&ucx_prov,
			"enable_spawn",FI_PARAM_BOOL,
			"Enable dynamic process support (Disabled by default)");

	fi_param_define(&ucx_prov,
			"tls",FI_PARAM_STRING,
			"Specifies transports available for UCX provider (Default: auto)");

	fi_param_define(&ucx_prov,
			"devices", FI_PARAM_STRING,
			"Specifies devices available for UCX provider (Default: auto)");

	fi_param_define(&ucx_prov,
			"check_req_leak", FI_PARAM_BOOL,
			"Check request leak (Default: false)");

	return &ucx_prov;
}
