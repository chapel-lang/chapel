/*
 * Copyright (c) 2023 Hewlett Packard Enterprise Development LP
 * SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only
 */
#include "config.h"
#include "cxip.h"
#include "ofi.h"
#include "ofi_str.h"

#define CXIP_DBG(...) _CXIP_DBG(FI_LOG_FABRIC, __VA_ARGS__)
#define CXIP_WARN(...) _CXIP_WARN(FI_LOG_FABRIC, __VA_ARGS__)
#define CXIP_INFO(...) _CXIP_INFO(FI_LOG_FABRIC, __VA_ARGS__)

static int cxip_nic_get_ss_env_get_vni(void)
{
	char *vni_str;
	char *vni_str_dup;
	char *token;
	int vni = -FI_EINVAL;

	vni_str = getenv("SLINGSHOT_VNIS");
	if (!vni_str) {
		CXIP_INFO("SLINGSHOT_VNIS not found\n");
		return -FI_ENOSYS;
	}

	vni_str_dup = strdup(vni_str);
	if (!vni_str_dup)
		return -FI_ENOMEM;

	/* Index/token zero is the per job-step VNI. Only use this value. Index
	 * one is the inter-job-step VNI. Ignore this one.
	 */
	token = strtok(vni_str_dup, ",");
	if (token)
		vni = (uint16_t)atoi(token);
	else
		CXIP_WARN("VNI not found in SLINGSHOT_VNIS: %s\n", vni_str);

	free(vni_str_dup);

	return vni;
}

static int cxip_gen_auth_key_ss_env_get_svc_id(struct cxip_if *nic_if)
{
	char *svc_id_str;
	char *dev_str;
	char *svc_id_str_dup;
	char *dev_str_dup;
	int device_index;
	char *token;
	bool found;
	int svc_id;

	svc_id_str = getenv("SLINGSHOT_SVC_IDS");
	if (!svc_id_str) {
		CXIP_INFO("SLINGSHOT_SVC_IDS not found\n");
		return -FI_ENOSYS;
	}

	dev_str = getenv("SLINGSHOT_DEVICES");
	if (!dev_str) {
		CXIP_INFO("SLINGSHOT_DEVICES not found\n");
		return -FI_ENOSYS;
	}

	dev_str_dup = strdup(dev_str);
	if (!dev_str_dup)
		return -FI_ENOMEM;

	found = false;
	device_index = 0;
	token = strtok(dev_str_dup, ",");
	while (token != NULL) {
		if (strcmp(token, nic_if->info->device_name) == 0) {
			found = true;
			break;
		}

		device_index++;
		token = strtok(NULL, ",");
	}

	free(dev_str_dup);

	if (!found) {
		CXIP_WARN("Failed to find %s in SLINGSHOT_DEVICES: %s\n",
			  nic_if->info->device_name, dev_str);
		return -FI_ENOSYS;
	}

	svc_id_str_dup = strdup(svc_id_str);
	if (!svc_id_str_dup)
		return -FI_ENOMEM;

	found = false;
	token = strtok(svc_id_str_dup, ",");
	while (token != NULL) {
		if (device_index == 0) {
			svc_id = atoi(token);
			found = true;
			break;
		}

		device_index--;
		token = strtok(NULL, ",");
	}

	free(svc_id_str_dup);

	if (!found) {
		CXIP_WARN("Failed to find service ID in SLINGSHOT_SVC_IDS: %s\n",
			  svc_id_str);
		return -FI_EINVAL;
	}

	return svc_id;
}

static int cxip_nic_get_rgroup_vni_ss_env(struct cxip_if *nic_if,
					  unsigned int *rgroup,
					  unsigned int *vni)
{
	int ret;

	ret = cxip_nic_get_ss_env_get_vni();
	if (ret < 0)
		return ret;

	*vni = ret;

	ret = cxip_gen_auth_key_ss_env_get_svc_id(nic_if);
	if (ret < 0)
		return ret;

	*rgroup = ret;

	CXIP_INFO("Generated (%u:%u) for %s\n", *rgroup, *vni,
		  nic_if->info->device_name);

	return FI_SUCCESS;
}

static int cxip_nic_get_best_rgroup_vni(struct cxip_if *nic_if,
					unsigned int *rgroup,
					unsigned int *vni)
{
	int ret;
	struct cxil_svc_list *svc_list;
	uid_t uid;
	gid_t gid;
	int i;
	int j;
	struct cxi_svc_desc *desc;
	int found_uid;
	int found_gid;
	int found_unrestricted;

	uid = geteuid();
	gid = getegid();

	ret = cxil_get_svc_list(nic_if->dev, &svc_list);
	if (ret) {
		CXIP_WARN("cxil_get_svc_list failed: %d:%s\n", ret,
			  strerror(-ret));
		return ret;
	}

	/* Find the service indexes which can be used by this process. These are
	 * services which are unrestricted, have a matching UID, or have a
	 * matching GID. If there are multiple service IDs which could match
	 * unrestricted, UID, and GID, only the first one found is selected.
	 */
	found_uid = -1;
	found_gid = -1;
	found_unrestricted = -1;

	for (i = svc_list->count - 1; i >= 0; i--) {
		desc = svc_list->descs + i;

		if (!desc->enable || desc->is_system_svc)
			continue;

		if (!desc->restricted_members) {
			if (found_unrestricted == -1)
				found_unrestricted = i;
			continue;
		}

		for (j = 0; j < CXI_SVC_MAX_MEMBERS; j++) {
			if (desc->members[j].type == CXI_SVC_MEMBER_UID &&
			    desc->members[j].svc_member.uid == uid &&
			    found_uid == -1)
				found_uid = i;
			else if (desc->members[j].type == CXI_SVC_MEMBER_GID &&
				 desc->members[j].svc_member.gid == gid &&
				 found_gid == -1)
				found_gid = i;
		}
	}

	/* Prioritized list for matching service ID. */
	if (found_uid != -1)
		i = found_uid;
	else if (found_gid != -1) {
		i = found_gid;
	} else if (found_unrestricted != -1) {
		i = found_unrestricted;
	} else {
		cxil_free_svc_list(svc_list);
		return -FI_ENOSYS;
	}

	/* Generate auth_key using matched service ID. */
	desc = svc_list->descs + i;

	if (desc->restricted_vnis) {
		if (desc->num_vld_vnis == 0) {
			CXIP_WARN("No valid VNIs for %s service ID %u\n",
				  nic_if->info->device_name, i);

			cxil_free_svc_list(svc_list);

			return -FI_EINVAL;
		}

		*vni = (uint16_t)desc->vnis[0];
	} else {
		*vni = (uint16_t)cxip_env.default_vni;
	}

	*rgroup = desc->svc_id;

	CXIP_INFO("Found (%u:%u) for %s\n", *rgroup, *vni,
		  nic_if->info->device_name);

	return FI_SUCCESS;
}

static int cxip_nic_get_rgroup_vni(struct cxip_if *nic_if,
				   unsigned int *rgroup, unsigned int *vni)
{
	int ret;

	ret = cxip_nic_get_rgroup_vni_ss_env(nic_if, rgroup, vni);
	if (ret == FI_SUCCESS)
		return FI_SUCCESS;

	ret = cxip_nic_get_best_rgroup_vni(nic_if, rgroup, vni);
	if (ret == -FI_ENOSYS) {
		CXIP_WARN("Failed to find valid default rgroup and vni for %s\n",
			  nic_if->info->device_name);
		*rgroup = 0;
		*vni = 0;
		ret = FI_SUCCESS;
	}

	return ret;
}

static int cxip_nic_close(struct fid *fid)
{
	struct fid_nic *nic = (struct fid_nic *) fid;

	free(nic->prov_attr);
	return ofi_nic_close(fid);
}

static int cxip_nic_control(struct fid *fid, int command, void *arg)
{
	int ret;
	struct fid_nic *nic = container_of(fid, struct fid_nic, fid);
	struct cxip_nic_attr *nic_attr = nic->prov_attr;
	struct fid_nic **dup = (struct fid_nic **) arg;
	struct cxip_if *nic_if;

	if (command == FI_OPT_CXI_NIC_REFRESH_ATTR) {
		ret = cxip_get_if(nic_attr->addr, &nic_if);
		if (ret != FI_SUCCESS)
			return ret;

		ret = cxip_nic_get_rgroup_vni(nic_if,
					      (void *)&nic_attr->default_rgroup_id,
					      (void *)&nic_attr->default_vni);

		cxip_put_if(nic_if);

		return ret;
	}

	ret = ofi_nic_control(fid, command, arg);
	if (ret != FI_SUCCESS)
		return ret;

	if (command == FI_DUP) {
		(*dup)->prov_attr = mem_dup(nic->prov_attr, sizeof(struct cxip_nic_attr));
		if (!(*dup)->prov_attr) {
			cxip_nic_close(&(*dup)->fid);
			return -FI_ENOMEM;
		}
	}

	return FI_SUCCESS;
}

static int cxip_nic_tostr(const struct fid *fid_nic, char *buf, size_t len)
{
	return ofi_nic_tostr(fid_nic, buf, len);
}

static struct fi_ops cxip_nic_ops = {
	.size = sizeof(struct fi_ops),
	.close = cxip_nic_close,
	.control = cxip_nic_control,
	.tostr = cxip_nic_tostr,
};

int cxip_nic_alloc(struct cxip_if *nic_if, struct fid_nic **fid_nic)
{
	struct fid_nic *nic;
	struct cxip_nic_attr *nic_attr;
	int ret;

	/* Reuse the common fid_nic as must as possible. */
	nic = ofi_nic_dup(NULL);
	if (!nic)
		return -FI_ENOMEM;

	nic_attr = calloc(1, sizeof(*nic_attr));
	if (!nic_attr) {
		ret = -FI_ENOMEM;
		goto err_free_nic;
	}

	nic->prov_attr = nic_attr;

	ret = cxip_nic_get_rgroup_vni(nic_if,
				      (void *)&nic_attr->default_rgroup_id,
				      (void *)&nic_attr->default_vni);
	if (ret != FI_SUCCESS)
		goto err_free_nic;

	memcpy((void *)&nic_attr->addr, &nic_if->info->nic_addr,
	       sizeof(nic_attr->addr));
	nic_attr->version = FI_CXI_NIC_ATTR_VER;

	/* Update the fid_nic to point to our operations. */
	nic->fid.ops = &cxip_nic_ops;

	nic->device_attr->name = strdup(nic_if->info->device_name);
	if (!nic->device_attr->name) {
		ret = -FI_ENOMEM;
		goto err_free_nic;
	}

	ret = asprintf(&nic->device_attr->device_id, "0x%x",
		       nic_if->info->device_id);
	if (ret < 0)
		goto err_free_nic;

	ret = asprintf(&nic->device_attr->device_version, "%u",
		       nic_if->info->device_rev);
	if (ret < 0)
		goto err_free_nic;

	ret = asprintf(&nic->device_attr->vendor_id, "0x%x",
		       nic_if->info->vendor_id);
	if (ret < 0)
		goto err_free_nic;

	nic->device_attr->driver = strdup(nic_if->info->driver_name);

	nic->bus_attr->bus_type = FI_BUS_PCI;
	nic->bus_attr->attr.pci.domain_id = nic_if->info->pci_domain;
	nic->bus_attr->attr.pci.bus_id = nic_if->info->pci_bus;
	nic->bus_attr->attr.pci.device_id = nic_if->info->pci_device;
	nic->bus_attr->attr.pci.function_id = nic_if->info->pci_function;

	ret = asprintf(&nic->link_attr->address, "0x%x",
		       nic_if->info->nic_addr);
	if (ret < 0)
		goto err_free_nic;

	nic->link_attr->mtu = nic_if->info->link_mtu;
	/* Convert Mb/s to libfabric reported b/s */
	nic->link_attr->speed = (size_t)nic_if->speed * 1000000;
	nic->link_attr->state = nic_if->link ?  FI_LINK_UP : FI_LINK_DOWN;
	nic->link_attr->network_type = strdup("HPC Ethernet");

	*fid_nic = nic;

	return FI_SUCCESS;

err_free_nic:
	cxip_nic_close(&nic->fid);

	return ret;
}
