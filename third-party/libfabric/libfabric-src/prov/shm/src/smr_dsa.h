/*
 * Copyright (c) 2022 Intel Corporation. All rights reserved
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

#ifndef _DSA_SHM_H_
#define _DSA_SHM_H_

#ifdef __cplusplus
extern "C" {
#endif

#if HAVE_CONFIG_H
#  include <config.h>
#endif /* HAVE_CONFIG_H */

#include <stddef.h>
#include <stdint.h>
#include "smr.h"

/* SMR FUNCTIONS FOR DSA SUPPORT */
void smr_dsa_init(void);
void smr_dsa_cleanup(void);
size_t smr_dsa_copy_to_sar(struct smr_ep *ep, struct smr_freestack *sar_pool,
		struct smr_resp *resp, struct smr_cmd *cmd,
		const struct iovec *iov, size_t count, size_t *bytes_done,
		void *entry_ptr);
size_t smr_dsa_copy_from_sar(struct smr_ep *ep, struct smr_freestack *sar_pool,
		struct smr_resp *resp, struct smr_cmd *cmd,
		const struct iovec *iov, size_t count, size_t *bytes_done,
		void *entry_ptr);
void smr_dsa_context_init(struct smr_ep *ep);
void smr_dsa_context_cleanup(struct smr_ep *ep);
void smr_dsa_progress(struct smr_ep *ep);

#ifdef __cplusplus
}
#endif
#endif /* _DSA_SHM_H_ */
