/*
 * SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only
 *
 * Copyright (c) 2019-2024 Hewlett Packard Enterprise Development LP
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

/****************************************************/

/* structure used to simulate failures */
struct cxip_trap {
	struct dlist_entry link;
	int index;
	int trap;
	int err;
	int prov_errno;
};

struct dlist_entry trap_list;
bool trap_initialized;

void cxip_trap_close(void)
{
	struct cxip_trap *trap_obj;

	if (!trap_initialized)
		return;
	while (!dlist_empty(&trap_list)) {
		dlist_pop_front(&trap_list, struct cxip_trap, trap_obj, link);
		free(trap_obj);
	}
}

void cxip_trap_set(int index, int trap, int err, int prov_errno)
{
	struct cxip_trap *trap_obj;

	if (!trap_initialized) {
		dlist_init(&trap_list);
		trap_initialized = true;
	}
	trap_obj = calloc(1, sizeof(*trap_obj));
	if (!trap_obj) {
		return;
	}
	dlist_init(&trap_obj->link);
	trap_obj->index = index;
	trap_obj->trap = trap;
	trap_obj->err = err;
	trap_obj->prov_errno = prov_errno;
	dlist_insert_tail(&trap_list, &trap_obj->link);
}

bool cxip_trap_search(int index, int trap, int *err, int *prov_errno)
{
	struct cxip_trap *trap_obj;
	struct dlist_entry *item;

	if (!trap_initialized) {
		return false;
	}

	int cnt = 0;
	dlist_foreach(&trap_list, item) {
		cnt++;
		trap_obj = container_of(item, struct cxip_trap, link);
		if (trap_obj->index != index)
			continue;
		if (trap_obj->trap != trap)
			continue;
		dlist_remove(item);
		if (err)
			*err = trap_obj->err;
		if (prov_errno) {
			if (trap_obj->err == -FI_EAVAIL)
				*prov_errno = trap_obj->prov_errno;
			else
				*prov_errno = 0;
		}
		free(trap_obj);
		return true;
	}
	return false;
}
#else
void cxip_fault_inject_init(void) {}
void cxip_fault_inject_fini(void) {}

void cxip_trap_close(void) {}
void cxip_trap_set(int index, int trap, int err, int prov_errno) {}
bool cxip_trap_search(int index, int trap, int *err, int *prov_errno)
{
	return false;
}
#endif
