/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */

#ifndef EFA_DEVICE_H
#define EFA_DEVICE_H

#include <rdma/fabric.h>
#include <infiniband/verbs.h>
#include <infiniband/efadv.h>
#include <stdbool.h>

struct efa_device {
	struct ibv_context	*ibv_ctx;
	struct ibv_device_attr	ibv_attr;
	struct efadv_device_attr efa_attr;
	struct ibv_port_attr	ibv_port_attr;
	union ibv_gid		ibv_gid;
	uint32_t		device_caps;
	uint32_t		max_rdma_size;
	struct fi_info		*rdm_info;
	struct fi_info		*dgram_info;
};

int efa_device_list_initialize(void);

void efa_device_list_finalize(void);

extern struct efa_device *g_efa_selected_device_list;

extern int g_efa_selected_device_cnt;

extern union ibv_gid *g_efa_ibv_gid_list;

extern int g_efa_ibv_gid_cnt;

bool efa_device_support_rdma_read(void);

bool efa_device_support_rdma_write(void);

bool efa_device_support_unsolicited_write_recv(void);

bool efa_device_support_cq_with_ext_mem_dmabuf(void);

bool efa_device_use_sub_cq(void);

int efa_device_get_driver(struct efa_device *efa_device,
			  char **efa_driver);

int efa_device_get_version(struct efa_device *efa_device,
			   char **device_version);

int efa_device_get_pci_attr(struct efa_device *device,
			    struct fi_pci_attr *pci_attr);

#endif
