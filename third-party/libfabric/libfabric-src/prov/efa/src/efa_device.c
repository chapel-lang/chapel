/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright (c) 2004, 2005 Topspin Communications.  All rights reserved. */
/* SPDX-FileCopyrightText: Copyright (c) 2006, 2007 Cisco Systems, Inc.  All rights reserved. */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */

#if HAVE_CONFIG_H
#  include <config.h>
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#include <alloca.h>
#include <errno.h>

#include <rdma/fi_errno.h>
#include <infiniband/verbs.h>

#include "efa.h"
#include "efa_device.h"
#include "efa_prov_info.h"

#ifdef _WIN32
#include "efawin.h"
#endif

/**
 * @brief initialize data members of a struct of efa_device until the gid
 *
 * @param	efa_device[in,out]	pointer to a struct efa_device
 * @param	ibv_device[in]		pointer to a struct ibv_device, which is
 * used to query attributes of the EFA device
 * @return	0 on success
 * 		a negative libfabric error code on failure.
 */
int efa_device_construct_gid(struct efa_device *efa_device,
			 struct ibv_device *ibv_device)
{
	int err;

	efa_device->ibv_ctx = ibv_open_device(ibv_device);
	if (!efa_device->ibv_ctx) {
		EFA_WARN(FI_LOG_EP_DATA, "ibv_open_device failed! errno: %d\n",
			 errno);
		return -errno;
	}

	memset(&efa_device->ibv_attr, 0, sizeof(efa_device->ibv_attr));
	err = ibv_query_device(efa_device->ibv_ctx, &efa_device->ibv_attr);
	if (err) {
		err = -err;
		EFA_INFO_ERRNO(FI_LOG_FABRIC, "ibv_query_device", err);
		goto err_close;
	}

	memset(&efa_device->efa_attr, 0, sizeof(efa_device->efa_attr));
	err = efadv_query_device(efa_device->ibv_ctx, &efa_device->efa_attr,
				 sizeof(efa_device->efa_attr));
	if (err) {
		err = -err;
		if (err == -EOPNOTSUPP) {
			EFA_INFO(FI_LOG_FABRIC, "Not an EFA device. Will not initialize.\n");
		} else {
			EFA_INFO_ERRNO(FI_LOG_FABRIC, "efadv_query_device", err);
		}
		goto err_close;
	}

	memset(&efa_device->ibv_port_attr, 0, sizeof(efa_device->ibv_port_attr));
	err = ibv_query_port(efa_device->ibv_ctx, 1, &efa_device->ibv_port_attr);
	if (err) {
		EFA_INFO_ERRNO(FI_LOG_FABRIC, "ibv_query_port", err);
		err = -err;
		goto err_close;
	}

	memset(&efa_device->ibv_gid, 0, sizeof(efa_device->ibv_gid));
	err = ibv_query_gid(efa_device->ibv_ctx, 1, 0, &efa_device->ibv_gid);
	if (err) {
		EFA_INFO_ERRNO(FI_LOG_FABRIC, "ibv_query_gid", err);
		err = -err;
		goto err_close;
	}

	return 0;

err_close:
	ibv_close_device(efa_device->ibv_ctx);
	efa_device->ibv_ctx = NULL;

	return err;
}

/**
 * @brief initialize data members of a struct of efa_device after the gid
 * including the prov info
 *
 * @param	efa_device[in,out]	pointer to a struct efa_device
 * @param	ibv_device[in]		pointer to a struct ibv_device, which is
 * used to query attributes of the EFA device
 * @return	0 on success
 * 		a negative libfabric error code on failure.
 */
int efa_device_construct_data(struct efa_device *efa_device,
			 struct ibv_device *ibv_device)
{
	int err;

	assert(efa_device->ibv_ctx);

#if HAVE_RDMA_SIZE
	efa_device->max_rdma_size = efa_device->efa_attr.max_rdma_size;
	efa_device->device_caps = efa_device->efa_attr.device_caps;
#else
	efa_device->max_rdma_size = 0;
	efa_device->device_caps = 0;
#endif
	efa_device->rdm_info = NULL;
	err = efa_prov_info_alloc(&efa_device->rdm_info, efa_device, FI_EP_RDM);
	if (err) {
		EFA_WARN(FI_LOG_DOMAIN, "failed to allocate device info for RDM. err: %d\n",
			 -err);
		goto err_close;
	}

	efa_device->dgram_info = NULL;
	err = efa_prov_info_alloc(&efa_device->dgram_info, efa_device, FI_EP_DGRAM);
	if (err) {
		EFA_WARN(FI_LOG_DOMAIN, "failed to allocate device info for DGRAM. err: %d\n",
			 -err);
		goto err_close;
	}

	return 0;

err_close:

	ibv_close_device(efa_device->ibv_ctx);
	efa_device->ibv_ctx = NULL;

	if (efa_device->rdm_info) {
		fi_freeinfo(efa_device->rdm_info);
		efa_device->rdm_info = NULL;
	}

	if (efa_device->dgram_info) {
		fi_freeinfo(efa_device->dgram_info);
		efa_device->dgram_info = NULL;
	}

	return err;
}

/**
 * @brief release resources in an efa_device struct
 *
 * @param	device[in,out]		pointer to an efa_device struct
 */
static void efa_device_destruct(struct efa_device *device)
{
	int err;

	if (device->ibv_ctx) {
		err = ibv_close_device(device->ibv_ctx);
		if (err)
			EFA_INFO_ERRNO(FI_LOG_DOMAIN, "ibv_dealloc_pd",
			               err);
	}

	device->ibv_ctx = NULL;
}

/*
 * g_efa_selected_device_list stores the initialized efa devices
 * that match the filter in FI_EFA_IFACE
 */
struct efa_device *g_efa_selected_device_list = NULL;
int g_efa_selected_device_cnt = 0;

/*
 * Store GIDs of all EFA devices in g_efa_ibv_gid_list including the ones that
 * don't match FI_EFA_IFACE g_efa_ibv_gid_list is used in the AV insertion path
 * to check if a peer is local. Local peers are allocated SHM provider resources
 */
union ibv_gid *g_efa_ibv_gid_list = NULL;
int g_efa_ibv_gid_cnt = 0;


/**
 * @brief initialize the global variables g_efa_selected_device_list,
 * g_efa_selected_device_cnt, g_efa_ibv_gid_list and g_efa_ibv_gid_cnt
 * @return	0 on success.
 * 		negative libfabric error code on failure.
 */
int efa_device_list_initialize(void)
{
	struct efa_device cur_device = {0};
	struct ibv_device **ibv_device_list;
	int device_idx, total_device_cnt;
	int ret, err;
	static bool initialized = false;

	if (initialized)
		return 0;

	initialized = true;

	ibv_device_list = ibv_get_device_list(&total_device_cnt);
	if (ibv_device_list == NULL)
		return -FI_ENOMEM;

	if (total_device_cnt <= 0) {
		ibv_free_device_list(ibv_device_list);
		return -FI_ENODEV;
	}

	EFA_INFO(FI_LOG_FABRIC, "ibv_get_device_list returns total_device_cnt=%d\n",
                 total_device_cnt);

	g_efa_selected_device_list = calloc(total_device_cnt, sizeof(struct efa_device));
	if (!g_efa_selected_device_list) {
		ret = -FI_ENOMEM;
		goto err_free;
	}

	g_efa_ibv_gid_list = calloc(total_device_cnt, sizeof(union ibv_gid));
	if (!g_efa_ibv_gid_list) {
		ret = -FI_ENOMEM;
		goto err_free;
	}

	for (device_idx = 0; device_idx < total_device_cnt; device_idx++) {
		memset(&cur_device, 0, sizeof(struct efa_device));

		err = efa_device_construct_gid(&cur_device,
					   ibv_device_list[device_idx]);

		if (err) {
			EFA_WARN(FI_LOG_FABRIC,
				 "efa_device_construct_gid failed for device_idx %d, err=%d\n",
				 device_idx, err);

			/* efa_device_construct returns -EOPNOTSUPP for non-EFA devices */
			if (err == -EOPNOTSUPP)
				continue;

			ret = err;
			goto err_free;
		}

		memcpy(&g_efa_ibv_gid_list[g_efa_ibv_gid_cnt], cur_device.ibv_gid.raw, sizeof(union ibv_gid));
		g_efa_ibv_gid_cnt++;

		/*
		 * Read the environment variable FI_EFA_IFACE and only proceed
		 * with the rest of the initialization for device names that
		 * match the FI_EFA_IFACE filter
		 */
		if (!efa_env_allows_nic(ibv_device_list[device_idx]->name)) {
			EFA_INFO(FI_LOG_FABRIC,
				 "Device %s filtered out by FI_EFA_IFACE\n",
				 ibv_device_list[device_idx]->name);
			efa_device_destruct(&cur_device);
			continue;

		}

		err = efa_device_construct_data(&cur_device, ibv_device_list[device_idx]);
		if (err) {
			EFA_WARN(FI_LOG_FABRIC,
				 "efa_device_construct_data failed for device %s, err=%d\n",
				 ibv_device_list[device_idx]->name, err);
			ret = err;
			goto err_free;
		}

		memcpy(&g_efa_selected_device_list[g_efa_selected_device_cnt], &cur_device, sizeof(struct efa_device));
		g_efa_selected_device_cnt++;
	}

	EFA_INFO(FI_LOG_FABRIC, "g_efa_selected_device_cnt=%d, g_efa_ibv_gid_cnt=%d\n",
		 g_efa_selected_device_cnt, g_efa_ibv_gid_cnt);

	if (g_efa_selected_device_cnt == 0) {
		ret = -FI_ENODEV;
		goto err_free;
	}

	ibv_free_device_list(ibv_device_list);
	return 0;

err_free:
	efa_device_list_finalize();

	assert(ibv_device_list);
	ibv_free_device_list(ibv_device_list);

	return ret;
}

/**
 * @brief release global resources that store EFA device and GID information
 */
void efa_device_list_finalize(void)
{
	int i;

	if (g_efa_selected_device_list) {
		for (i = 0; i < g_efa_selected_device_cnt; i++)
			efa_device_destruct(&g_efa_selected_device_list[i]);

		free(g_efa_selected_device_list);
		g_efa_selected_device_list = NULL;
	}

	g_efa_selected_device_cnt = 0;

	if (g_efa_ibv_gid_list) {
		free(g_efa_ibv_gid_list);
		g_efa_ibv_gid_list = NULL;
	}
	g_efa_ibv_gid_cnt = 0;
}

/**
 * @brief Check whether EFA device supports rdma read
 *
 * @return a boolean indicating rdma read support
 */
bool efa_device_support_rdma_read(void)
{
	assert(g_efa_selected_device_cnt > 0);

	return g_efa_selected_device_list[0].device_caps & EFADV_DEVICE_ATTR_CAPS_RDMA_READ;
}

/**
 * @brief check whether efa device support rdma write
 *
 * @return a boolean indicating rdma write status
 */
#if HAVE_CAPS_RDMA_WRITE
bool efa_device_support_rdma_write(void)
{
	assert(g_efa_selected_device_cnt > 0);

	return g_efa_selected_device_list[0].device_caps & EFADV_DEVICE_ATTR_CAPS_RDMA_WRITE;
}
#else
bool efa_device_support_rdma_write(void)
{
	return false;
}
#endif

/**
 * @brief check whether efa device support unsolicited write recv
 *
 * @return a boolean indicating unsolicited write recv
 */
#if HAVE_CAPS_UNSOLICITED_WRITE_RECV
bool efa_device_support_unsolicited_write_recv(void)
{
	assert(g_efa_selected_device_cnt > 0);

	return g_efa_selected_device_list[0].device_caps & EFADV_DEVICE_ATTR_CAPS_UNSOLICITED_WRITE_RECV;
}
#else
bool efa_device_support_unsolicited_write_recv(void)
{
	return false;
}
#endif

/**
 * @brief check whether efa device has support for creating CQ with external memory
 *
 * @return a boolean indicating that creating CQs with external memory buffers
 * by passing dmabuf is supported.
 */
#if HAVE_CAPS_CQ_WITH_EXT_MEM_DMABUF
bool efa_device_support_cq_with_ext_mem_dmabuf(void)
{
	assert(g_efa_selected_device_cnt > 0);

	return !!(g_efa_selected_device_list[0].device_caps &
		  EFADV_DEVICE_ATTR_CAPS_CQ_WITH_EXT_MEM_DMABUF);
}
#else
bool efa_device_support_cq_with_ext_mem_dmabuf(void)
{
	return false;
}
#endif

/* Check whether the efa device uses a sub cq implementation */
bool efa_device_use_sub_cq(void)
{
	uint32_t vendor_part_id;
	vendor_part_id = g_efa_selected_device_list[0].ibv_attr.vendor_part_id;
	return vendor_part_id == 0xefa0;
}

#ifndef _WIN32

static char *get_sysfs_path(void)
{
	char *env = NULL;
	char *sysfs_path = NULL;
	int len;

	/*
	 * Only follow use path passed in through the calling user's
	 * environment if we're not running SUID.
	 */
	if (getuid() == geteuid())
		env = getenv("SYSFS_PATH");

	if (env) {
		sysfs_path = strndup(env, IBV_SYSFS_PATH_MAX);
		len = strlen(sysfs_path);
		while (len > 0 && sysfs_path[len - 1] == '/') {
			--len;
			sysfs_path[len] = '\0';
		}
	} else {
		sysfs_path = strdup("/sys");
	}

	return sysfs_path;
}

/**
 * @brief get efa device driver name
 *
 * @param	efa_device[in]	pointer to struct efa_device
 * @param	efa_driver[out]	EFA driver name
 * @return	0 on success
 * 		negative lifabric error code on failure
 */
int efa_device_get_driver(struct efa_device *efa_device,
			  char **efa_driver)
{
	int ret;
	char *driver_sym_path;
	char driver_real_path[PATH_MAX];
	char *driver;
	ret = asprintf(&driver_sym_path, "%s%s",
		       efa_device->ibv_ctx->device->ibdev_path, "/device/driver");
	if (ret < 0) {
		return -FI_ENOMEM;
	}

	if (!realpath(driver_sym_path, driver_real_path)) {
		ret = -errno;
		goto err_free_driver_sym;
	}

	driver = strrchr(driver_real_path, '/');
	if (!driver) {
		ret = -FI_EINVAL;
		goto err_free_driver_sym;
	}
	driver++;
	*efa_driver = strdup(driver);
	if (!*efa_driver) {
		ret = -FI_ENOMEM;
		goto err_free_driver_sym;
	}

	free(driver_sym_path);
	return 0;

err_free_driver_sym:
	free(driver_sym_path);
	return ret;
}

/**
 * @brief get efa device version string
 *
 * @param	efa_device[in]		pointer to struct efa_device
 * @param	device_version[out]	EFA device version string
 * @return	0 on success
 * 		negative lifabric error code on failure
 */
int efa_device_get_version(struct efa_device *efa_device,
			   char **device_version)
{
	char *sysfs_path;
	int ret;

	*device_version = calloc(1, EFA_ABI_VER_MAX_LEN + 1);
	if (!*device_version) {
		return -FI_ENOMEM;
	}

	sysfs_path = get_sysfs_path();
	if (!sysfs_path) {
		return -FI_ENOMEM;
	}

	ret = fi_read_file(sysfs_path, "class/infiniband_verbs/abi_version",
			   *device_version,
			   EFA_ABI_VER_MAX_LEN);
	if (ret < 0) {
		goto free_sysfs_path;
	}

	free(sysfs_path);
	return 0;

free_sysfs_path:
	free(sysfs_path);
	return ret;
}

/**
 * @brief get efa device PCI attribute
 *
 * @param	efa_device[in]		pointer to struct efa_device
 * @param	device_version[out]	EFA device PCI attribute
 * @return	0 on success
 * 		negative lifabric error code on failure
 */
int efa_device_get_pci_attr(struct efa_device *efa_device,
			    struct fi_pci_attr *pci_attr)
{
	char *dbdf_sym_path;
	char *dbdf;
	char dbdf_real_path[PATH_MAX];
	int ret;
	ret = asprintf(&dbdf_sym_path, "%s%s",
		       efa_device->ibv_ctx->device->ibdev_path, "/device");
	if (ret < 0) {
		return -FI_ENOMEM;
	}

	if (!realpath(dbdf_sym_path, dbdf_real_path)) {
		ret = -errno;
		goto err_free_dbdf_sym;
	}

	dbdf = strrchr(dbdf_real_path, '/');
	if (!dbdf) {
		ret = -FI_EINVAL;
		goto err_free_dbdf_sym;
	}
	dbdf++;

	ret = sscanf(dbdf, "%hx:%hhx:%hhx.%hhx", &pci_attr->domain_id,
		     &pci_attr->bus_id, &pci_attr->device_id,
		     &pci_attr->function_id);
	if (ret != 4) {
		ret = -FI_EINVAL;
		goto err_free_dbdf_sym;
	}

	free(dbdf_sym_path);
	return 0;

err_free_dbdf_sym:
	free(dbdf_sym_path);
	return ret;
}

#else // _WIN32

/**
 * @brief get efa device driver name
 *
 * @param	efa_device[in]	pointer to struct efa_device
 * @param	efa_driver[out]	EFA driver name
 * @return	0 on success
 * 		negative lifabric error code on failure
 */
int efa_device_get_driver(struct efa_device *efa_device,
			  char **efa_driver)
{
	int ret;
	/*
	 * On windows efa device is not exposed as infiniband device.
	 * The driver for efa device can be queried using Windows Setup API.
	 * The code required to do that is more complex than necessary in this context.
	 * We will return a hardcoded string as driver.
	 */
	ret = asprintf(efa_driver, "%s", "efa.sys");
	if (ret < 0) {
		return -FI_ENOMEM;
	}
	return 0;
}

/**
 * @brief get efa device version string
 *
 * @param	efa_device[in]		pointer to struct efa_device
 * @param	device_version[out]	EFA device version string
 * @return	0 on success
 * 		negative lifabric error code on failure
 */
int efa_device_get_version(struct efa_device *efa_device,
			   char **device_version)
{
	int ret;
	/*
	 * On Windows, there is no sysfs. We use hw_ver field of ibv_attr to obtain it
	 */
	ret = asprintf(device_version, "%u", efa_device->ibv_attr.hw_ver);
	if (ret < 0) {
		return -FI_ENOMEM;
	}
	return 0;
}

/**
 * @brief get efa device PCI attribute
 *
 * @param	efa_device[in]		pointer to struct efa_device
 * @param	device_version[out]	EFA device PCI attribute
 * @return	0 on success
 * 		negative lifabric error code on failure
 */
int efa_device_get_pci_attr(struct efa_device *device,
			    struct fi_pci_attr *pci_attr)
{
	/*
	 * pci_attr is currently not supported on Windows. We return success
	 * to let applications continue without failures.
	 */
	return 0;
}

#endif // _WIN32
