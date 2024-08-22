/*
 * SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only
 *
 * Copyright (c) 2019 Hewlett Packard Enterprise Development LP
 */

/* Fault injection. */

#include <stdlib.h>
#include <time.h>

#include "cxip.h"

#define CXIP_DBG(...) _CXIP_DBG(FI_LOG_EP_CTRL, __VA_ARGS__)
#define CXIP_WARN(...) _CXIP_WARN(FI_LOG_EP_CTRL, __VA_ARGS__)

#if ENABLE_DEBUG

struct cxip_fault dma_fault = { .env = "DMA_FAULT_RATE" };
struct cxip_fault malloc_fault = { .env = "MALLOC_FAULT_RATE" };

static void fault_init(struct cxip_fault *fault)
{
	char *var;
	float rate;
	int ret;

	var = getenv(fault->env);
	if (var) {
		ret = sscanf(var, "%f", &rate);
		if (ret == 1) {
			if (rate < 0)
				rate = 0;
			if (rate > 1)
				rate = 1;

			fault->prop = rate * RAND_MAX;
			CXIP_DBG("%s: %f\n", fault->env, rate);
		}
	}
}

static void fault_fini(struct cxip_fault *fault)
{
	if (fault->prop)
		CXIP_WARN("%s: %ld faults injected\n",
			  fault->env, fault->count);
}

void cxip_fault_inject_init(void)
{
	time_t t = time(NULL);

	CXIP_DBG("Rand seed: %lu\n", t);
	srand(t);

	fault_init(&dma_fault);
	fault_init(&malloc_fault);
}

void cxip_fault_inject_fini(void)
{
	fault_fini(&dma_fault);
	fault_fini(&malloc_fault);
}

#else
void cxip_fault_inject_init(void) {}
void cxip_fault_inject_fini(void) {}
#endif
