/*
 * Copyright (c) 2019 Amazon.com, Inc. or its affiliates.
 * All rights reserved.
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

#include <ofi_prov.h>
#include "efa.h"
#include "rxr.h"
#include "rxr_env.h"
#include "efa_prov_info.h"

#ifdef ENABLE_EFA_POISONING
const uint32_t rxr_poison_value = 0xdeadbeef;
#endif

static void rxr_fini();

struct fi_provider rxr_prov = {
	.name = "efa",
	.version = OFI_VERSION_DEF_PROV,
	.fi_version = OFI_VERSION_LATEST,
	.getinfo = efa_getinfo,
	.fabric = efa_fabric,
	.cleanup = rxr_fini
};

struct util_prov rxr_util_prov = {
	.prov = &rxr_prov,
	.flags = 0,
};

static int rxr_util_prov_initialize()
{
	int i, err;
	struct fi_info *head, *tail, *prov_info_rxr;

	head = NULL;
	tail = NULL;
	for (i = 0; i < g_device_cnt; ++i) {
		err = efa_prov_info_alloc_for_rxr(&prov_info_rxr, &g_device_list[i]);
		if (err) {
			EFA_WARN(FI_LOG_DOMAIN, "Failed to allocate prov_info for RxR. error: %d\n",
				 err);
			continue;
		}

		if (!head) {
			head = prov_info_rxr;
		} else {
			assert(tail);
			tail->next = prov_info_rxr;
		}

		tail = prov_info_rxr;
	}

	if (!head)
		return -FI_ENODATA;

	rxr_util_prov.info = head;
	return 0;
}

static void rxr_util_prov_finalize()
{
	struct fi_info *prov_info_rxr;

	prov_info_rxr = (struct fi_info *)rxr_util_prov.info;
	fi_freeinfo(prov_info_rxr);
	rxr_util_prov.info = NULL;
}

EFA_INI
{
#if HAVE_EFA_DL
	ofi_mem_init();
	ofi_hmem_init();
	ofi_monitors_init();
#endif

	if (efa_prov_initialize())
		return NULL;

	rxr_env_initialize();

	if (rxr_util_prov_initialize())
		return NULL;

	return &rxr_prov;
}

static void rxr_fini(void)
{
	rxr_util_prov_finalize();

	efa_prov_finalize();

	efa_shm_info_finalize();

#if HAVE_EFA_DL
	ofi_monitors_cleanup();
	ofi_hmem_cleanup();
	ofi_mem_fini();
#endif
}

