/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */

#ifndef _EFA_RDM_SRX_H
#define _EFA_RDM_SRX_H

#include "efa.h"

int efa_rdm_peer_srx_construct(struct efa_rdm_ep *efa_rdm_ep);

void efa_rdm_srx_update_rxe(struct fi_peer_rx_entry *peer_rxe,
			    struct efa_rdm_ope *rxe);

static inline struct util_srx_ctx *efa_rdm_srx_get_srx_ctx(struct fi_peer_rx_entry *peer_rxe)
{
	return (struct util_srx_ctx *) peer_rxe->srx->ep_fid.fid.context;
}

#endif /* _EFA_RDM_SRX_H */
