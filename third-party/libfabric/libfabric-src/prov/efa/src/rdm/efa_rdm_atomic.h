/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */

#if HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#ifndef _EFA_RDM_ATOMIC_H_
#define _EFA_RDM_ATOMIC_H_

int efa_rdm_atomic_query(struct fid_domain *domain,
			 enum fi_datatype datatype, enum fi_op op,
			 struct fi_atomic_attr *attr, uint64_t flags);

ssize_t efa_rdm_atomic_post_atomic(struct efa_rdm_ep *efa_rdm_ep, struct efa_rdm_ope *txe);

extern struct fi_ops_atomic efa_rdm_atomic_ops;

#endif
