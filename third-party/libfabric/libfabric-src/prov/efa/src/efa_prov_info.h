/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */

#ifndef EFA_DEVICE_INFO_H
#define EFA_DEVICE_INFO_H

#include "efa.h"

int efa_prov_info_alloc(struct fi_info **prov_info,
			struct efa_device *device,
			enum fi_ep_type ep_type);

int efa_prov_info_alloc_for_rdm(struct fi_info **prov_info_rdm,
				struct efa_device *device);

int efa_prov_info_compare_src_addr(const char *node, uint64_t flags, const struct fi_info *hints,
				   const struct fi_info *fi);

int efa_prov_info_compare_domain_name(const struct fi_info *hints,
                                      const struct fi_info *info);

int efa_prov_info_compare_pci_bus_id(const struct fi_info *hints,
                                     const struct fi_info *info);

#endif
