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

#ifndef _LPP_EQ_H_
#define _LPP_EQ_H_

struct lpp_eq {
	struct fid_eq		eq_fid;

	struct lpp_fabric	*fabric;
	struct fi_eq_attr	attr;
};

/* User visible functions. */
int		lpp_fi_eq_open(struct fid_fabric *fabric, struct fi_eq_attr *attr, struct fid_eq **eq, void *context);
int		lpp_fi_eq_close(struct fid *eq);
ssize_t		lpp_fi_eq_read(struct fid_eq *eq, uint32_t *event, void *buf, size_t len, uint64_t flags);
ssize_t		lpp_fi_eq_readerr(struct fid_eq *eq, struct fi_eq_err_entry *buf, uint64_t flags);
ssize_t		lpp_fi_eq_sread(struct fid_eq *eq, uint32_t *event, void *buf, size_t len, int timeout, uint64_t flags);
const char	*lpp_fi_eq_strerror(struct fid_eq *eq, int prov_errno, const void *err_data, char *buf, size_t len);

#endif // _LPP_EQ_H_
