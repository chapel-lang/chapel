/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */

#ifndef EFA_FORK_SUPPORT_H
#define EFA_FORK_SUPPORT_H

#include <rdma/fabric.h>

/*
 * ON will avoid using huge pages for bounce buffers, so that the libibverbs
 * fork support can be used safely.
 */
enum efa_fork_support_status {
	EFA_FORK_SUPPORT_OFF = 0,
	EFA_FORK_SUPPORT_ON,
	EFA_FORK_SUPPORT_UNNEEDED,
};
extern enum efa_fork_support_status g_efa_fork_status;

int efa_fork_support_enable_if_requested(struct fid_domain *domain_fid);

void efa_fork_support_request_initialize();

#endif
