/*
 * Copyright (C) 2021-2025 by Cornelis Networks.
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
#include <ofi.h>
#include <stdio.h>
#include <signal.h>

#include "rdma/opx/fi_opx_hfi1.h"
#include "rdma/opx/fi_opx_internal.h"
#include "opx_shm.h"

OPX_COMPILE_TIME_ASSERT(OPX_SHM_MAX_CONN_NUM >= (OPX_MAX_HFIS * 256),
			"OPX_SHM_MAX_CONN_NUM must be at least 4096 (OPX_MAX_HFIS per node * 256 contexts per HFI)");

/*
 * Used by signal handler to free shared memory files.
 */
struct dlist_entry shm_tx_list;
struct dlist_entry shm_rx_list;

struct sigaction SIGSEGV_old_action;
struct sigaction SIGBUS_old_action;
struct sigaction SIGINT_old_action;
struct sigaction SIGTERM_old_action;
struct sigaction SIGABRT_old_action;

int opx_shm_match(struct dlist_entry *item, const void *arg)
{
	return ((void *) item) == arg;
}

// Frees the shared memory files then calls the old
// signal handler.
static void opx_shm_signal(int signum, siginfo_t *info, void *ucontext)
{
	struct opx_shm_tx *tx_entry;

	while (!dlist_empty(&shm_tx_list)) {
		dlist_pop_front(&shm_tx_list, struct opx_shm_tx, tx_entry, list_entry);

		opx_shm_tx_fini(tx_entry);
	}

	struct opx_shm_rx *rx_entry;
	while (!dlist_empty(&shm_rx_list)) {
		dlist_pop_front(&shm_rx_list, struct opx_shm_rx, rx_entry, list_entry);

		opx_shm_rx_fini(rx_entry);
	}

	// Restore old signal handlers and re-raise signal.
	(void) sigaction(SIGINT, &SIGINT_old_action, NULL);
	(void) sigaction(SIGTERM, &SIGTERM_old_action, NULL);
	(void) sigaction(SIGBUS, &SIGBUS_old_action, NULL);
	(void) sigaction(SIGSEGV, &SIGSEGV_old_action, NULL);
	(void) sigaction(SIGABRT, &SIGABRT_old_action, NULL);

	raise(signum);
}

static int registered = 0;
void	   opx_register_shm_handler()
{
	struct sigaction act;

	if (registered) {
		return;
	}

	dlist_init(&shm_tx_list);
	dlist_init(&shm_rx_list);

	memset(&act, 0, sizeof(act));

	act.sa_sigaction = opx_shm_signal;
	act.sa_flags	 = SA_SIGINFO;

	(void) sigaction(SIGINT, &act, &SIGINT_old_action);
	(void) sigaction(SIGTERM, &act, &SIGTERM_old_action);
	(void) sigaction(SIGBUS, &act, &SIGBUS_old_action);
	(void) sigaction(SIGSEGV, &act, &SIGSEGV_old_action);
	(void) sigaction(SIGABRT, &act, &SIGABRT_old_action);

	registered = 1;
}
