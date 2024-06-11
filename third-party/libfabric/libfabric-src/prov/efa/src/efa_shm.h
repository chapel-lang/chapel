/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */

#ifndef _EFA_SHM_H
#define _EFA_SHM_H

#include <rdma/fabric.h>

struct efa_ep_addr;

int efa_shm_ep_name_construct(char *smr_name, size_t *smr_name_len, struct efa_ep_addr *raw_addr);

void efa_shm_info_create(const struct fi_info *app_info, struct fi_info **shm_info);

/** maximum name length for shm endpoint */
#define EFA_SHM_NAME_MAX	   (256)

#endif
