/*
 * SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only
 *
 * Copyright (c) 2019-2024 Hewlett Packard Enterprise Development LP
 */

/* Fault injection. */

struct cxip_fault {
	char *env;	/* Configuration env. var. name */
	int prop;	/* Proportion of rand() values */
	size_t count;	/* Count of injected faults */
};

extern struct cxip_fault dma_fault;
extern struct cxip_fault malloc_fault;

void cxip_fault_inject_fini(void);
void cxip_fault_inject_init(void);

#if ENABLE_DEBUG
#define INJECT_FAULT(fault) \
	((fault).prop && rand() < (fault).prop && (fault).count++)
#else
#define INJECT_FAULT(fault) 0
#endif

#define cxi_cq_emit_dma_f(...)			\
	(INJECT_FAULT(dma_fault) ? -ENOSPC :	\
	 cxi_cq_emit_dma(__VA_ARGS__))

#define cxip_pte_unlink_f(...)			\
	(INJECT_FAULT(dma_fault) ? -FI_EAGAIN :	\
	 cxip_pte_unlink(__VA_ARGS__))

#define malloc_f(...)				\
	(INJECT_FAULT(malloc_fault) ? NULL :	\
	 malloc(__VA_ARGS__))

/**
 * Collective traps, can be extended for other uses.
 *
 * This creates a dlist of "traps" that are keyed to an index, and a trap
 * identifier. When the search results in a match of both index and trap, this
 * sets the *err variable to the specified trap error, and returns true.
 * Otherwise it returns false.
 *
 * The close, and set functions are generally called in the test code. The
 * search function is generally embedded in the provider.
 *
 * If the trap logic branches on search returning true, search should be a no-op
 * with no performance penalty when ENABLE_DEBUG is FALSE.
 *
 * This will slow operations if ENABLE_DEBUG is TRUE, and there is a large list
 * of traps. Normally, the test case will set only one trap, since the objective
 * is to force a controlled fault and observe the result.
 */
enum {
	CXIP_TRAP_NONE = 0,
	CXIP_TRAP_GETGRP,
	CXIP_TRAP_BCAST,
	CXIP_TRAP_REDUCE,
	CXIP_TRAP_CURL_FM_URL,
	CXIP_TRAP_CURL_TOKEN,
	CXIP_TRAP_HWROOT_INVAL,
	CXIP_TRAP_HWROOT_INUSE,
	CXIP_TRAP_MCAST_INUSE,
	CXIP_TRAP_INITPTE,
};

void cxip_trap_close(void);
void cxip_trap_set(int index, int trap, int err, int prov_errno);
bool cxip_trap_search(int index, int trap, int *err, int *prov_errno);
