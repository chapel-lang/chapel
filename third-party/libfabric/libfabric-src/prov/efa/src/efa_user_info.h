/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */

#ifndef _EFA_USER_INFO_H
#define _EFA_USER_INFO_H

int efa_user_info_set_dest_addr(const char *node, const char *service, uint64_t flags,
				const struct fi_info *hints, struct fi_info *fi);

int efa_user_info_check_hints_addr(const char *node, const char *service,
				   uint64_t flags, const struct fi_info *hints);

int efa_user_info_check_fabric_object(const struct fi_info *hints,
				      struct fi_info *dupinfo,
				      const struct fi_info *prov_info);

int efa_user_info_check_domain_object(const struct fi_info *hints,
				      struct fi_info *dupinfo);

int efa_getinfo(uint32_t version, const char *node, const char *service,
		uint64_t flags, const struct fi_info *hints, struct fi_info **info);

#endif
