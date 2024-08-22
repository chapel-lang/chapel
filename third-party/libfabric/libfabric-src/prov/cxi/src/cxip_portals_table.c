/*
 * Copyright (c) 2023 Hewlett Packard Enterprise Development LP
 * SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only
 */
#include "cxip.h"

#define CXIP_DBG(...) _CXIP_DBG(FI_LOG_DOMAIN, __VA_ARGS__)
#define CXIP_INFO(...) _CXIP_INFO(FI_LOG_DOMAIN, __VA_ARGS__)
#define CXIP_WARN(...) _CXIP_WARN(FI_LOG_DOMAIN, __VA_ARGS__)

int cxip_portals_table_alloc(struct cxip_lni *lni, uint16_t *vni,
			     size_t vni_count, uint32_t pid,
			     struct cxip_portals_table **ptable)
{
	struct cxip_portals_table *table;
	int ret;
	int i;


	if (!vni_count) {
		CXIP_WARN("Invalid VNI count\n");
		return -FI_EINVAL;
	}

	table = calloc(1, sizeof(*table));
	if (!table) {
		CXIP_WARN("Failed to allocate IF domain\n");
		return -FI_ENOMEM;
	}

	table->doms = calloc(vni_count, sizeof(*table->doms));
	if (!table->doms) {
		CXIP_WARN("Failed to allocate domain array\n");
		ret = -FI_ENOMEM;
		goto err_free_table;
	}

	for (i = 0; i < vni_count; i++) {
		ret = cxil_alloc_domain(lni->lni, vni[i], pid, &table->doms[i]);
		if (ret) {
			CXIP_WARN("Failed to allocate CXI Domain, ret: %d\n",
				  ret);
			ret = -FI_ENOSPC;
			goto err_free_doms;
		}

		/* To handle C_PID_ANY correctly, the same PID needs to be used
		 * for each domain. Thus, update PID after the first domain
		 * is allocated to a valid value.
		 */
		pid = table->doms[i]->pid;
	}

	table->pid = pid;
	table->doms_count = vni_count;
	table->lni = lni;

	CXIP_DBG("Allocated portals table, %s PID: %u\n",
		 lni->iface->info->device_name, table->pid);

	*ptable = table;

	return FI_SUCCESS;

err_free_doms:
	for (i--; i >= 0; i--) {
		ret = cxil_destroy_domain(table->doms[i]);
		if (ret)
			CXIP_WARN("Failed to destroy domain: %d\n", ret);
	}

	free(table->doms);
err_free_table:
	free(table);

	return ret;
}

/*
 * cxip_free_if_domain() - Free an IF Domain.
 */
void cxip_portals_table_free(struct cxip_portals_table *ptable)
{
	int ret;
	int i;

	CXIP_DBG("Freeing portals table, %s PID: %u\n",
		 ptable->lni->iface->info->device_name, ptable->pid);

	for (i = 0; i < ptable->doms_count; i++) {
		ret = cxil_destroy_domain(ptable->doms[i]);
		if (ret)
			CXIP_WARN("Failed to destroy domain: %d\n", ret);
	}

	free(ptable->doms);
	free(ptable);
}
