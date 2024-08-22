/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */

#ifndef EFA_PROV_H
#define EFA_PROV_H

extern struct fi_provider efa_prov;
extern struct util_prov efa_util_prov;

#define EFA_WARN(subsys, ...) FI_WARN(&efa_prov, subsys, __VA_ARGS__)
#define EFA_WARN_ONCE(subsys, ...) FI_WARN_ONCE(&efa_prov, subsys, __VA_ARGS__)
#define EFA_TRACE(subsys, ...) FI_TRACE(&efa_prov, subsys, __VA_ARGS__)
#define EFA_INFO(subsys, ...) FI_INFO(&efa_prov, subsys, __VA_ARGS__)
#define EFA_INFO_ERRNO(subsys, fn, errno) \
	EFA_INFO(subsys, fn ": %s(%d)\n", strerror(errno), errno)
#define EFA_WARN_ERRNO(subsys, fn, errno) \
	EFA_WARN(subsys, fn ": %s(%d)\n", strerror(errno), errno)
#define EFA_DBG(subsys, ...) FI_DBG(&efa_prov, subsys, __VA_ARGS__)

#endif