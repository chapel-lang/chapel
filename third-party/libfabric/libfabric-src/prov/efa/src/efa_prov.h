/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */

#ifndef EFA_PROV_H
#define EFA_PROV_H

extern struct fi_provider efa_prov;
extern struct util_prov efa_util_prov;

#define EFA_WARN(subsys, ...) FI_WARN(&efa_prov, subsys, __VA_ARGS__)
#define EFA_WARN_ONCE(subsys, ...) FI_WARN_ONCE(&efa_prov, subsys, __VA_ARGS__)
#define EFA_TRACE(subsys, ...) FI_TRACE(&efa_prov, subsys, __VA_ARGS__)
#define EFA_INFO(subsys, ...) FI_INFO(&efa_prov, subsys, __VA_ARGS__)
#define EFA_INFO_ERRNO(subsys, fn, errno) \
	EFA_INFO(subsys, fn ": %s(%d)\n", strerror(errno), errno)
#define EFA_WARN_ERRNO(subsys, fn, errno) \
	EFA_WARN(subsys, fn ": %s(%d)\n", strerror(errno), errno)
#define EFA_DBG(subsys, ...) FI_DBG(&efa_prov, subsys, __VA_ARGS__)

static inline
int efa_prov_info_set_fabric_name(struct fi_info *prov_info, char *fabric_name)
{
	prov_info->fabric_attr->name = calloc(1, strlen(fabric_name) + 1);
	if (!prov_info->fabric_attr->name)
		return -FI_ENOMEM;
	strcpy(prov_info->fabric_attr->name, fabric_name);
	return FI_SUCCESS;
}

#endif