/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */

#if HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#ifndef _EFA_RDM_RMA_H_
#define _EFA_RDM_RMA_H_

#include <rdma/fi_rma.h>

int efa_rdm_rma_verified_copy_iov(struct efa_rdm_ep *ep, struct efa_rma_iov *rma,
			      size_t count, uint32_t flags,
			      struct iovec *iov, void **desc);

extern struct fi_ops_rma efa_rdm_rma_ops;

struct efa_rdm_ope *
efa_rdm_rma_alloc_txe(struct efa_rdm_ep *efa_rdm_ep,
		      struct efa_rdm_peer *peer,
		      const struct fi_msg_rma *msg_rma,
		      uint32_t op,
		      uint64_t flags);

ssize_t efa_rdm_rma_post_write(struct efa_rdm_ep *ep, struct efa_rdm_ope *txe);

ssize_t efa_rdm_rma_post_read(struct efa_rdm_ep *ep, struct efa_rdm_ope *txe);

#endif
