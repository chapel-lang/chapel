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

#ifndef _LPP_FABRIC_H_
#define _LPP_FABRIC_H_

struct lpp_fabric {
	struct fid_fabric       fabric_fid;
	ofi_atomic32_t          ref_count;
};

/* LPP tunables (environment variables) */
extern int lpp_disable_osbypass;
extern int lpp_cq_overcommit;
extern int lpp_domain_cleanup;
extern int lpp_disable_dqp;
extern int lpp_system_memcpy;
extern int lpp_write_only;
extern size_t lpp_max_wr_osbypass_size;
extern size_t lpp_max_rd_osbypass_size;
extern size_t lpp_max_eager_size;
extern size_t lpp_keep_alive_msec;
extern size_t lpp_keep_alive_retries;
extern int lpp_disable_hmem_dma;

struct fi_fabric_attr lpp_fabric_default_attrs(const struct klppioc_lf *klpp_devinfo);

int	lpp_fabric_verify_attrs(const struct klppioc_lf *klpp_devinfo,
				const struct fi_fabric_attr *attrs);
int	lpp_fi_fabric(struct fi_fabric_attr *, struct fid_fabric **, void *);
int	lpp_fi_fabric_close(struct fid *);

void lpp_fabric_setup_attrs(const struct klppioc_lf *klpp_devinfo,
			    const struct fi_info *hints,
			    struct fi_fabric_attr *tmp_fabric);

#endif // _LPP_FABRIC_H_
