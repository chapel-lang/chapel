/*
 * Copyright (c) 2004, 2005 Topspin Communications.  All rights reserved.
 * Copyright (c) 2006, 2007 Cisco Systems, Inc.  All rights reserved.
 * Copyright (c) 2017-2020 Amazon.com, Inc. or its affiliates. All rights reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * OpenIB.org BSD license below:
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

#include "efa.h"
#include "efa_device.h"
#include "efa_prov_info.h"

#ifdef _WIN32
#include "efawin.h"
#endif

/**
 * @brief initialize data members of a struct of efa_device
 *
 * @param	efa_device[in,out]	pointer to a struct efa_device
 * @param	device_idx[in]		device index
 * @param	ibv_device[in]		pointer to a struct ibv_device, which is used
 * 					to query attributes of the EFA device
 * @return	0 on success
 * 		a negative libfabric error code on failure.
 */
int efa_device_construct(struct efa_device *efa_device,
			 int device_idx,
			 struct ibv_device *ibv_device)
{
	int err;

	efa_device->device_idx = device_idx;

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
		EFA_INFO_ERRNO(FI_LOG_FABRIC, "efadv_query_device", err);
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

	efa_device->ibv_pd = ibv_alloc_pd(efa_device->ibv_ctx);
	if (!efa_device->ibv_pd) {
		EFA_INFO_ERRNO(FI_LOG_DOMAIN, "ibv_alloc_pd",
		               errno);
		err = -errno;
		goto err_close;
	}

#ifdef HAVE_RDMA_SIZE
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

	if (device->ibv_pd) {
		err = ibv_dealloc_pd(device->ibv_pd);
		if (err)
			EFA_INFO_ERRNO(FI_LOG_DOMAIN, "ibv_dealloc_pd",
			               err);
	}

	device->ibv_pd = NULL;

	if (device->ibv_ctx) {
		err = ibv_close_device(device->ibv_ctx);
		if (err)
			EFA_INFO_ERRNO(FI_LOG_DOMAIN, "ibv_dealloc_pd",
			               err);
	}

	device->ibv_ctx = NULL;
}

struct efa_device *g_device_list;
int g_device_cnt;

/**
 * @brief initialize the global variables g_device_list and g_device_cnt
 * @return	0 on success.
 * 		negative libfabric error code on failure.
 */
int efa_device_list_initialize(void)
{
	struct ibv_device **ibv_device_list;
	int device_idx;
	int ret, err;
	static bool initialized = false;

	if (initialized)
		return 0;

	initialized = true;

	ibv_device_list = ibv_get_device_list(&g_device_cnt);
	if (ibv_device_list == NULL)
		return -FI_ENOMEM;

	if (g_device_cnt <= 0) {
		ibv_free_device_list(ibv_device_list);
		return -FI_ENODEV;
	}

	g_device_list = calloc(g_device_cnt, sizeof(struct efa_device));
	if (!g_device_list) {
		ret = -FI_ENOMEM;
		goto err_free;
	}

	for (device_idx = 0; device_idx < g_device_cnt; device_idx++) {
		err = efa_device_construct(&g_device_list[device_idx], device_idx, ibv_device_list[device_idx]);
		if (err) {
			ret = err;
			goto err_free;
		}
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
 * @brief release the resources in g_device_list, and set g_device_cnt to 0
 */
void efa_device_list_finalize(void)
{
	int i;

	if (g_device_list) {
		for (i = 0; i < g_device_cnt; i++)
			efa_device_destruct(&g_device_list[i]);

		free(g_device_list);
	}

	g_device_cnt = 0;
}

/**
 * @brief check whether efa device support rdma read
 *
 * @return a boolean indicating rdma read status
 */
bool efa_device_support_rdma_read(void)
{
	if (g_device_cnt <=0)
		return false;

	return g_device_list[0].device_caps & EFADV_DEVICE_ATTR_CAPS_RDMA_READ;
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
