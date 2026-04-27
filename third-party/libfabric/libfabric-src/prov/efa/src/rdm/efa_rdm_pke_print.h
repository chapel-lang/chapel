/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */

#ifndef _efa_rdm_pke_print_H
#define _efa_rdm_pke_print_H


#include "efa.h"

#if ENABLE_DEBUG
void efa_rdm_pke_print(struct efa_rdm_pke *pkt_entry, char *prefix);
#endif

#endif
