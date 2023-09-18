/*
 * Copyright (c) 2022 Amazon.com, Inc. or its affiliates. All rights reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * BSD license below:
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

#ifndef EFA_DEVICE_H
#define EFA_DEVICE_H

#include <infiniband/verbs.h>
#include <infiniband/efadv.h>

struct efa_device {
	int			device_idx;
	struct ibv_context	*ibv_ctx;
	struct ibv_device_attr	ibv_attr;
	struct efadv_device_attr efa_attr;
	struct ibv_port_attr	ibv_port_attr;
	union ibv_gid		ibv_gid;
	uint32_t		device_caps;
	uint32_t		max_rdma_size;
	struct ibv_pd		*ibv_pd;
	struct fi_info		*rdm_info;
	struct fi_info		*dgram_info;
};

int efa_device_list_initialize(void);

void efa_device_list_finalize(void);

extern struct efa_device *g_device_list;

extern int g_device_cnt;

bool efa_device_support_rdma_read(void);

int efa_device_get_driver(struct efa_device *efa_device,
			  char **efa_driver);

int efa_device_get_version(struct efa_device *efa_device,
			   char **device_version);

int efa_device_get_pci_attr(struct efa_device *device,
			    struct fi_pci_attr *pci_attr);

#endif
