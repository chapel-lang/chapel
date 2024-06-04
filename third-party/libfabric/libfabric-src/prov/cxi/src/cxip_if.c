/*
 * SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only
 *
 * Copyright (c) 2018,2020-2023 Hewlett Packard Enterprise Development LP
 */

#include "config.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <dirent.h>
#include <glob.h>

#include "ofi_prov.h"
#include "ofi_osd.h"

#include "cxip.h"

#define CXIP_DBG(...) _CXIP_DBG(FI_LOG_DOMAIN, __VA_ARGS__)
#define CXIP_INFO(...) _CXIP_INFO(FI_LOG_DOMAIN, __VA_ARGS__)
#define CXIP_WARN(...) _CXIP_WARN(FI_LOG_DOMAIN, __VA_ARGS__)

struct slist cxip_if_list;
static struct cxil_device_list *cxi_dev_list;

/*
 * cxip_if_lookup_addr() - Return a provider NIC interface descriptor
 * associated with a specified NIC address, if available.
 */
struct cxip_if *cxip_if_lookup_addr(uint32_t nic_addr)
{
	struct slist_entry *entry, *prev __attribute__ ((unused));
	struct cxip_if *if_entry;

	slist_foreach(&cxip_if_list, entry, prev) {
		if_entry = container_of(entry, struct cxip_if, if_entry);
		if (if_entry->info->nic_addr == nic_addr)
			return if_entry;
	}

	return NULL;
}

/*
 * cxip_if_lookup() - Return a provider NIC interface descriptor associated
 * with a specified NIC device name, if available.
 */
struct cxip_if *cxip_if_lookup_name(const char *name)
{
	struct slist_entry *entry, *prev __attribute__ ((unused));
	struct cxip_if *if_entry;

	slist_foreach(&cxip_if_list, entry, prev) {
		if_entry = container_of(entry, struct cxip_if, if_entry);
		if (!strcmp(if_entry->info->device_name, name))
			return if_entry;
	}

	return NULL;
}

/*
 * cxip_lni_res_count() - Return usage information for LNI resource.
 */
int cxip_lni_res_cnt(struct cxip_lni *lni, char *res_str)
{
	struct dirent *de;
	char path[100];
	uint32_t c = 0;
	DIR *dr;

	sprintf(path, "/sys/kernel/debug/cxi/cxi%u/lni/%u/%s",
		lni->iface->info->dev_id, lni->lni->id, res_str);

	dr = opendir(path);
	if (!dr)
		return 0;

	while ((de = readdir(dr))) {
		if (strncmp(de->d_name, ".", 1))
			c++;
	}

	closedir(dr);

	return c;
}

/*
 * cxip_lni_res_dump() - Dump resource usage information for an LNI.
 */
void cxip_lni_res_dump(struct cxip_lni *lni)
{
	DIR *dr;
	uint32_t pt_count = 0;
	uint32_t cq_count = 0;
	uint32_t eq_count = 0;
	uint32_t ct_count = 0;
	uint32_t ac_count = 0;

	/* Check if debugfs is available. */
	dr = opendir("/sys/kernel/debug/cxi");
	if (!dr) {
		CXIP_INFO("Resource usage info unavailable: %s RGID: %u.\n",
			  lni->iface->info->device_name, lni->lni->id);
		return;
	}

	closedir(dr);

	cq_count = cxip_lni_res_cnt(lni, "cq");
	pt_count = cxip_lni_res_cnt(lni, "pt");
	eq_count = cxip_lni_res_cnt(lni, "eq");
	ct_count = cxip_lni_res_cnt(lni, "ct");
	ac_count = cxip_lni_res_cnt(lni, "ac");

	CXIP_INFO("Resource usage: %s RGID: %u CQ: %u PTE: %u EQ: %u CT: %u AC: %u\n",
		  lni->iface->info->device_name, lni->lni->id, cq_count,
		  pt_count, eq_count, ct_count, ac_count);
}

/*
 * cxip_get_if() - Get a reference to the device interface associated with a
 * provided NIC address. A process can open each interface once to support many
 * FI Domains. An IF is used to allocate the various device resources including
 * CMDQs, EVTQs, and PtlTEs.
 */
int cxip_get_if(uint32_t nic_addr, struct cxip_if **iface)
{
	int ret;
	struct cxip_if *if_entry;

	/* The IF list device info is static, no need to lock */
	if_entry = cxip_if_lookup_addr(nic_addr);
	if (!if_entry) {
		CXIP_DBG("interface not found\n");
		return -FI_ENODEV;
	}

	if (!if_entry->link) {
		CXIP_INFO("Interface %s link down.\n",
			  if_entry->info->device_name);
		return -FI_ENODEV;
	}

	/* Lock the IF to serialize opening the device */
	ofi_spin_lock(&if_entry->lock);

	if (!if_entry->dev) {
		ret = cxil_open_device(if_entry->info->dev_id, &if_entry->dev);
		if (ret) {
			CXIP_WARN("Failed to open CXI Device, ret: %d\n", ret);
			ret = -FI_ENODEV;
			goto unlock;
		}

		CXIP_DBG("Opened %s\n", if_entry->info->device_name);
	}

	ofi_atomic_inc32(&if_entry->ref);
	*iface = if_entry;

	ofi_spin_unlock(&if_entry->lock);

	return FI_SUCCESS;

unlock:
	ofi_spin_unlock(&if_entry->lock);

	return ret;
}

/*
 * cxip_put_if() - Drop a reference to the device interface.
 */
void cxip_put_if(struct cxip_if *iface)
{
	ofi_spin_lock(&iface->lock);

	if (!ofi_atomic_dec32(&iface->ref)) {
		cxil_close_device(iface->dev);
		iface->dev = NULL;

		CXIP_DBG("Closed %s\n", iface->info->device_name);
	}

	ofi_spin_unlock(&iface->lock);
}

int cxip_if_valid_rgroup_vni(struct cxip_if *iface, unsigned int rgroup_id,
			     unsigned int vni)
{
	struct cxi_svc_desc svc_desc;
	bool vni_found = false;
	int ret;
	int i;

	ret = cxil_get_svc(iface->dev, rgroup_id, &svc_desc);
	if (ret) {
		CXIP_WARN("cxil_get_svc with %s and rgroup_id %d failed: %d:%s\n",
			  iface->dev->info.device_name, rgroup_id, ret,
			  strerror(-ret));
		return -FI_EINVAL;
	}

	if (svc_desc.restricted_vnis) {
		for (i = 0; i < svc_desc.num_vld_vnis; i++) {
			if (vni == svc_desc.vnis[i]) {
				vni_found = true;
				break;
			}
		}

		if (!vni_found) {
			CXIP_WARN("Invalid VNI %d for %s and svc_id %d\n",
				  vni, iface->dev->info.device_name,
				  rgroup_id);
			return -FI_EINVAL;
		}
	}

	return FI_SUCCESS;
}

/*
 * cxip_alloc_lni() - Allocate an LNI
 */
int cxip_alloc_lni(struct cxip_if *iface, uint32_t svc_id,
		   struct cxip_lni **if_lni)
{
	struct cxip_lni *lni;
	int ret;

	lni = calloc(1, sizeof(*lni));
	if (!lni) {
		CXIP_WARN("Unable to allocate LNI\n");
		return -FI_ENOMEM;
	}

	ret = cxil_alloc_lni(iface->dev, &lni->lni, svc_id);
	if (ret) {
		CXIP_WARN("Failed to allocate LNI, ret: %d\n", ret);
		ret = -FI_ENOSPC;
		goto free_lni;
	}

	lni->iface = iface;
	ofi_spin_init(&lni->lock);
	dlist_init(&lni->remap_cps);

	CXIP_DBG("Allocated LNI, %s RGID: %u\n",
		 lni->iface->info->device_name, lni->lni->id);

	*if_lni = lni;

	return FI_SUCCESS;

free_lni:
	free(lni);

	return ret;
}

/*
 * cxip_free_lni() - Free an LNI
 */
void cxip_free_lni(struct cxip_lni *lni)
{
	int ret;
	int i;
	struct dlist_entry *tmp;
	struct cxip_remap_cp *sw_cp;

	cxip_lni_res_dump(lni);

	CXIP_DBG("Freeing LNI, %s RGID: %u\n",
		 lni->iface->info->device_name, lni->lni->id);

	dlist_foreach_container_safe(&lni->remap_cps, struct cxip_remap_cp,
				     sw_cp, remap_entry, tmp)
		free(sw_cp);

	for (i = 0; i < lni->n_cps; i++) {
		ret = cxil_destroy_cp(lni->hw_cps[i]);
		if (ret)
			CXIP_WARN("Failed to destroy CP: %d\n", ret);
	}

	ret = cxil_destroy_lni(lni->lni);
	if (ret)
		CXIP_WARN("Failed to destroy LNI: %d\n", ret);

	free(lni);
}

/*
 * netdev_ama_check - Return true if the netdev has an AMA installed.
 */
static bool netdev_ama_check(char *netdev)
{
	int rc;
	char addr_path[FI_PATH_MAX];
	FILE *f;
	int val;

	rc = snprintf(addr_path, FI_PATH_MAX,
		      "/sys/class/net/%s/addr_assign_type",
		      netdev);
	if (rc < 0)
		return false;

	f = fopen(addr_path, "r");
	if (!f)
		return false;

	rc = fscanf(f, "%d", &val);

	fclose(f);

	if (rc != 1)
		return false;

	/* Check for temporary address */
	if (val != 3)
		return false;

	rc = snprintf(addr_path, FI_PATH_MAX, "/sys/class/net/%s/address",
		      netdev);
	if (rc < 0)
		return false;

	f = fopen(addr_path, "r");
	if (!f)
		return false;

	rc = fscanf(f, "%x:%*x:%*x:%*x:%*x", &val);

	fclose(f);

	if (rc != 1)
		return false;

	/* Check for locally administered unicast address */
	if ((val & 0x3) != 0x2)
		return false;

	return true;
}

/*
 * netdev_link - Return netdev link state.
 */
static int netdev_link(char *netdev, int *link)
{
	int rc;
	char path[FI_PATH_MAX];
	FILE *f;
	char state[20];
	int carrier;

	rc = snprintf(path, FI_PATH_MAX, "/sys/class/net/%s/operstate",
		      netdev);
	if (rc < 0)
		return -1;

	f = fopen(path, "r");
	if (!f)
		return -1;

	rc = fscanf(f, "%20s", state);

	fclose(f);

	if (!strncmp(state, "up", strlen("up"))) {
		*link = 1;
		return 0;
	}

	if (strncmp(state, "unknown", strlen("unknown"))) {
		/* State is not not up or unknown, link is down. */
		*link = 0;
		return 0;
	}

	/* operstate is unknown, must check carrier. */
	rc = snprintf(path, FI_PATH_MAX, "/sys/class/net/%s/carrier",
		      netdev);
	if (rc < 0)
		return -1;

	f = fopen(path, "r");
	if (!f)
		return -1;

	rc = fscanf(f, "%d", &carrier);

	fclose(f);

	if (carrier)
		*link = 1;
	else
		*link = 0;

	return 0;
}

/*
 * netdev_speed - Return netdev interface speed.
 */
static int netdev_speed(char *netdev, int *speed)
{
	int rc;
	char path[FI_PATH_MAX];
	FILE *f;
	int val;

	rc = snprintf(path, FI_PATH_MAX, "/sys/class/net/%s/speed",
		      netdev);
	if (rc < 0)
		return -1;

	f = fopen(path, "r");
	if (!f)
		return -1;

	rc = fscanf(f, "%u", &val);

	fclose(f);

	if (rc != 1)
		return -1;

	*speed = val;

	return 0;
}

/*
 * netdev_netdev - Look up the netdev associated with an RDMA device file.
 */
static int netdev_lookup(struct cxil_devinfo *info, char **netdev)
{
	glob_t globbuf;
	int rc;
	int count;
	int i;
	char if_path[FI_PATH_MAX];
	char addr_path[FI_PATH_MAX];
	char *addr;
	unsigned int dom;
	unsigned int bus;
	unsigned int dev;
	unsigned int func;

	rc = glob("/sys/class/net/*", 0, NULL, &globbuf);
	if (rc)
		return -1;

	count = globbuf.gl_pathc;

	for (i = 0; i < count; i++) {
		rc = snprintf(if_path, FI_PATH_MAX, "%s/device",
			      globbuf.gl_pathv[i]);
		if (rc < 0)
			goto free_glob;

		rc = readlink(if_path, addr_path, FI_PATH_MAX-1);
		if (rc < 0) {
			/* A virtual device, like a bridge, doesn't have a
			 * device link.
			 */
			if (errno == ENOENT || errno == ENOTDIR)
				continue;

			goto free_glob;
		}
		addr_path[rc] = '\0';

		addr = basename(addr_path);

		rc = sscanf(addr, "%x:%x:%x.%x", &dom, &bus, &dev, &func);
		if (rc != 4)
			continue;

		if (info->pci_domain == dom &&
		    info->pci_bus == bus &&
		    info->pci_device == dev &&
		    info->pci_function == func) {
			*netdev = strdup(basename(globbuf.gl_pathv[i]));
			if (!*netdev)
				goto free_glob;

			globfree(&globbuf);
			return 0;
		}
	}

free_glob:
	globfree(&globbuf);

	return -1;
}

/*
 * cxip_query_if_list() - Populate static IF data during initialization.
 */
static void cxip_query_if_list(struct slist *if_list)
{
	struct cxip_if *if_entry;
	int ret;
	int i;
	char *netdev;
	int speed = 0;
	int link = 0;

	slist_init(if_list);

	/* The cxi_dev_list is freed in the provider IF destructor */
	ret = cxil_get_device_list(&cxi_dev_list);
	if (ret) {
		CXIP_WARN("cxil_get_device_list failed\n");
		return;
	}

	if (cxi_dev_list->count == 0) {
		CXIP_DBG("No IFs found\n");
		return;
	}

	if (cxi_dev_list->info[0].min_free_shift) {
		CXIP_WARN("Non-zero min_free_shift not supported\n");
		return;
	}

	for (i = 0; i < cxi_dev_list->count; i++) {
		/* Ignore cxi devices not included in device name string. */
		if (cxip_env.device_name &&
		    (strstr(cxip_env.device_name,
			    cxi_dev_list->info[i].device_name) == NULL))
			continue;

		if (!getenv("CXIP_SKIP_RH_CHECK") &&
		    cxi_dev_list->info[i].device_platform == C_PLATFORM_ASIC &&
		    !cxil_rh_running(&cxi_dev_list->info[i])) {
			CXIP_LOG("CXI retry handler not running for device: %s\n",
				 cxi_dev_list->info[i].device_name);
			continue;
		}

		ret = netdev_lookup(&cxi_dev_list->info[i], &netdev);
		if (ret) {
			CXIP_LOG("CXI netdev not found for device: %s\n",
				 cxi_dev_list->info[i].device_name);
			netdev = strdup("DNE");
		} else {
			ret = netdev_link(netdev, &link);
			if (ret)
				CXIP_WARN("Failed to read netdev link: %s\n",
					  netdev);

			ret = netdev_speed(netdev, &speed);
			if (ret)
				CXIP_WARN("Failed to read netdev speed: %s\n",
					  netdev);

			CXIP_DBG("Device %s has netdev %s (link: %u speed: %u)\n",
				 cxi_dev_list->info[i].device_name,
				 netdev, link, speed);
		}

		if (!getenv("CXIP_SKIP_AMA_CHECK") &&
		    !netdev_ama_check(netdev)) {
			CXIP_LOG("CXI device %s, netdev %s AMA not recognized\n",
				 cxi_dev_list->info[i].device_name,
				 netdev);
			free(netdev);
			continue;
		}

		free(netdev);

		if_entry = calloc(1, sizeof(struct cxip_if));
		if_entry->info = &cxi_dev_list->info[i];
		if_entry->link = link;
		if_entry->speed = speed;

		ofi_atomic_initialize32(&if_entry->ref, 0);
		dlist_init(&if_entry->ptes);
		ofi_spin_init(&if_entry->lock);
		slist_insert_tail(&if_entry->if_entry, if_list);
	}
}

/*
 * cxip_free_if_list() - Tears down static IF data.
 */
static void cxip_free_if_list(struct slist *if_list)
{
	struct slist_entry *entry;
	struct cxip_if *if_entry;

	while (!slist_empty(if_list)) {
		entry = slist_remove_head(if_list);
		if_entry = container_of(entry, struct cxip_if, if_entry);
		ofi_spin_destroy(&if_entry->lock);
		free(if_entry);
	}

	cxil_free_device_list(cxi_dev_list);
}

/*
 * cxip_if_init() - The provider IF constructor.  Initializes static IF data.
 */
void cxip_if_init(void)
{
	cxip_query_if_list(&cxip_if_list);
}

/*
 * cxip_if_fini() - The provider IF destructor.  Tears down IF data.
 */
void cxip_if_fini(void)
{
	cxip_free_if_list(&cxip_if_list);
}
