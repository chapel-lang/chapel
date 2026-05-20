/*
 * Copyright (c) 2018-2024 GigaIO, Inc. All Rights Reserved.
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

#ifndef _LPP_KLPP_H_
#define _LPP_KLPP_H_

// Ioctl interface calls.
int klpp_open(int);
void klpp_close(int);

int klpp_getdevice(int klpp_fd, struct klppioc_lf *klpp_devinfo);

int klpp_av_resolve(struct lpp_av *lpp_avp, uint32_t host_index, uint16_t port,
		    struct lpp_fi_addr *lpp_fi_addr);
int klpp_av_lookup(struct lpp_av *lpp_avp, struct lpp_fi_addr lpp_fi_addr,
		   uint32_t *host_index, uint16_t *port);

int klpp_mr_register(struct lpp_mr *, uint64_t flags);
int klpp_mr_close(struct lpp_mr *);

int klpp_cntr_create(struct lpp_cntr *);
int klpp_cntr_close(struct lpp_cntr *);

int klpp_cq_create(struct lpp_cq *);
int klpp_cq_close(struct lpp_cq *);

int klpp_ctx_create(struct lpp_ctx *);
int klpp_ctx_close(struct lpp_ctx *);

int klpp_ep_create(struct lpp_ep *);
int klpp_ep_close(struct lpp_ep *);
int klpp_ep_enable(struct lpp_ep *);
int klpp_ep_disable(struct lpp_ep *);

void *klpp_mmap(struct lpp_domain *, size_t, int);
int klpp_munmap(void *, size_t);

int klpp_rmr_label(struct lpp_domain *);

int klpp_progress(struct lpp_domain *lpp_domainp, int umc_id);
int klpp_readwrite(struct lpp_ep *lpp_epp, fi_addr_t addr, uint64_t length,
		   uint64_t flags, void *context, void *local_uaddr,
		   uint64_t remote_uaddr, uint64_t remote_key);

int klpp_umc_acquire(struct lpp_domain *lpp_domain, klpp_id_t *umc_id,
		     uint8_t *src_id, uint64_t *generation, void *rx_base,
		     void *tx_base, void *k2u_base, void *u2k_base,
		     struct lpp_fi_addr ep_addr, uint16_t *port);
void klpp_umc_release(struct lpp_domain *lpp_domain, klpp_id_t umc_id);
int klpp_kmc_send_one(struct lpp_ep *lpp_epp, struct lpp_fi_addr dst_addr,
		      struct klpp_msg_hdr *hdr, const struct iovec *iov,
		      size_t iov_count);
int klpp_kmc_send_list(struct lpp_ep *lpp_epp, struct klpp_ioc_kmc_send *head);

#endif	// _LPP_KLPP_H_
