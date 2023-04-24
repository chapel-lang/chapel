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

#ifndef EFA_DEVICE_INFO_H
#define EFA_DEVICE_INFO_H

#include "efa.h"

int efa_prov_info_alloc(struct fi_info **prov_info,
			struct efa_device *device,
			enum fi_ep_type ep_type);

int efa_prov_info_alloc_for_rxr(struct fi_info **prov_info_rxr,
				struct efa_device *device);

int efa_prov_info_compare_src_addr(const char *node, uint64_t flags, const struct fi_info *hints,
				   const struct fi_info *fi);

int efa_prov_info_compare_domain_name(const struct fi_info *hints,
                                      const struct fi_info *info);

int efa_prov_info_compare_pci_bus_id(const struct fi_info *hints,
                                     const struct fi_info *info);

#endif
