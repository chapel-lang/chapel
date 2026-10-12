/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */

#ifndef EFA_DEVICE_H
#define EFA_DEVICE_H

#include <rdma/fabric.h>
#include <infiniband/verbs.h>
#include <infiniband/efadv.h>
#include <stdbool.h>
#include "ofi_lock.h"

struct efa_qp;

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
	/* QP table and lock for device-level QP management */
	struct efa_qp		**qp_table;
	uint8_t			*qp_gen_table;
	size_t			qp_table_sz_m1;
	struct ofi_genlock		qp_table_lock;
	int				urandom_fd;
};

int efa_device_list_initialize(void);

void efa_device_list_finalize(void);

void efa_device_destruct(struct efa_device *device);

int efa_device_construct_gid(struct efa_device *efa_device,
			     struct ibv_device *ibv_device);

int efa_device_construct_data(struct efa_device *efa_device,
			      struct ibv_device *ibv_device);

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
