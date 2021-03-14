/*
 * Copyright (c) 2018 Intel Corporation. All rights reserved.
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


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <inttypes.h>

#include <rdma/fi_errno.h>
#include <ofi_perf.h>
#include <rdma/providers/fi_log.h>


enum ofi_perf_domain	perf_domain = OFI_PMU_CPU;
uint32_t		perf_cntr = OFI_PMC_CPU_INSTR;
uint32_t		perf_flags;


void ofi_perf_init(void)
{
	char *param_val = NULL;

	fi_param_define(NULL, "perf_cntr", FI_PARAM_STRING,
			"Performance counter to analyze (default: cpu_instr). "
			"Options: cpu_instr, cpu_cycles.");
	fi_param_get_str(NULL, "perf_cntr", &param_val);
	if (!param_val)
		return;

	if (!strcasecmp(param_val, "cpu_cycles")) {
		perf_domain = OFI_PMU_CPU;
		perf_cntr = OFI_PMC_CPU_CYCLES;
	}
}

int ofi_perfset_create(const struct fi_provider *prov,
		       struct ofi_perfset *set, size_t size,
		       enum ofi_perf_domain domain, uint32_t cntr_id,
		       uint32_t flags)
{
	int ret;

	ret = ofi_pmu_open(&set->ctx, domain, cntr_id, flags);
	if (ret) {
		FI_WARN(prov, FI_LOG_CORE, "Unable to open PMU %d (%s)\n",
			ret, fi_strerror(ret));
		return ret;
	}

	set->data = calloc(size, sizeof(*set->data));
	if (!set->data) {
		ofi_pmu_close(set->ctx);
		return -FI_ENOMEM;
	}

	set->prov = prov;
	set->size = size;
	return 0;
}

void ofi_perfset_close(struct ofi_perfset *set)
{
	ofi_pmu_close(set->ctx);
	free(set->data);
}

static const char *ofi_perf_name(void)
{
	switch (perf_domain) {
	case OFI_PMU_CPU:
		switch (perf_cntr) {
		case OFI_PMC_CPU_CYCLES:
			return "CPU cycles";
		case OFI_PMC_CPU_INSTR:
			return "CPU instr";
		}
		break;
	case OFI_PMU_CACHE:
		switch (perf_cntr) {
		case OFI_PMC_CACHE_L1_DATA:
			return "L1 data cache";
		case OFI_PMC_CACHE_L1_INSTR:
			return "L1 instr cache";
		case OFI_PMC_CACHE_TLB_DATA:
			return "TLB data cache";
		case OFI_PMC_CACHE_TLB_INSTR:
			return "TLB instr cache";
		}
		break;
	case OFI_PMU_OS:
		switch (perf_cntr) {
		case OFI_PMC_OS_PAGE_FAULT:
			return "page faults";
		}
		break;
	case OFI_PMU_NIC:
		break;
	}
	return "unknown";
}

void ofi_perfset_log(struct ofi_perfset *set, const char *names[])
{
	size_t i;

	FI_TRACE(set->prov, FI_LOG_CORE, "\n");
	FI_TRACE(set->prov, FI_LOG_CORE, "\tPERF: %s\n", ofi_perf_name());
	FI_TRACE(set->prov, FI_LOG_CORE, "\t%-20s%-10s%s\n", "Name", "Avg", "Events");

	for (i = 0; i < set->size; i++) {
		if (!set->data[i].events)
			continue;

		FI_TRACE(set->prov, FI_LOG_CORE, "\t%-20s%-10g%" PRIu64 "\n",
			names && names[i] ? names[i] : "unknown",
			(double) set->data[i].sum / set->data[i].events,
			set->data[i].events);
	}
}
