/*
 * Copyright (C) 2021 by Cornelis Networks.
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

/*
 * Used by signal handler to free shared memory files.
 */
struct opx_shm_tx *shm_tx_head = NULL;
struct opx_shm_rx *shm_rx_head = NULL;

struct sigaction SIGSEGV_old_action;
struct sigaction SIGBUS_old_action;
struct sigaction SIGINT_old_action;
struct sigaction SIGTERM_old_action;
struct sigaction SIGABRT_old_action;

// Frees the shared memory files then calls the old
// signal handler.
static void opx_shm_signal(int signum, siginfo_t *info, void *ucontext)
{
	while (shm_tx_head != NULL) {
		struct opx_shm_tx *tmp = shm_tx_head;
		shm_tx_head = shm_tx_head->next;

		opx_shm_tx_fini(tmp);
	}

	while (shm_rx_head != NULL) {
		struct opx_shm_rx *tmp = shm_rx_head;
		shm_rx_head = shm_rx_head->next;

		opx_shm_rx_fini(tmp);
	}

	// Restore old signal handlers and re-raise signal.
	(void)sigaction(SIGINT, &SIGINT_old_action, NULL);
	(void)sigaction(SIGTERM, &SIGTERM_old_action, NULL);
	(void)sigaction(SIGBUS, &SIGBUS_old_action, NULL);
	(void)sigaction(SIGSEGV, &SIGSEGV_old_action, NULL);
	(void)sigaction(SIGABRT, &SIGABRT_old_action, NULL);

	raise(signum);

}

static int registered = 0;
void opx_register_shm_handler()
{
	struct sigaction act; 

	if (registered) return;

	memset(&act, 0, sizeof(act));

	act.sa_sigaction = opx_shm_signal;
	act.sa_flags = SA_SIGINFO;

	(void)sigaction(SIGINT, &act, &SIGINT_old_action);
	(void)sigaction(SIGTERM, &act, &SIGTERM_old_action);
	(void)sigaction(SIGBUS, &act, &SIGBUS_old_action);
	(void)sigaction(SIGSEGV, &act, &SIGSEGV_old_action);
	(void)sigaction(SIGABRT, &act, &SIGABRT_old_action);

	registered = 1;

}

