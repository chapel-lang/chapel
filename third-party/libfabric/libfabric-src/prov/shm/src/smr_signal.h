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

#ifndef _SMR_SIGNAL_H_
#define _SMR_SIGNAL_H_
#include <signal.h>
#include <ofi_shm.h>

struct sigaction *old_action;

static void smr_handle_signal(int signum, siginfo_t *info, void *ucontext)
{
	struct smr_ep_name *ep_name;
	int ret;

	dlist_foreach_container(&ep_name_list, struct smr_ep_name,
				ep_name, entry) {
		shm_unlink(ep_name->name);
	}

	/* Register the original signum handler, SIG_DFL or otherwise */
	ret = sigaction(signum, &old_action[signum], NULL);
	if (ret)
		return;

	/* Raise signum to execute the original handler */
	raise(signum);
}

static void smr_reg_sig_hander(int signum)
{
	struct sigaction action;
	int ret;

	memset(&action, 0, sizeof(action));
	action.sa_sigaction = smr_handle_signal;
	action.sa_flags |= SA_SIGINFO;

	ret = sigaction(signum, &action, &old_action[signum]);
	if (ret)
		FI_WARN(&smr_prov, FI_LOG_FABRIC,
			"Unable to register handler for sig %d\n", signum);
}

#endif /* _SMR_SIGNAL_H_ */
