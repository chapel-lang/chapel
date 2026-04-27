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

#ifndef _LPP_AV_H_
#define _LPP_AV_H_

#include "lpp.h"

struct lpp_av {
	struct fid_av           av_fid;
	struct fi_av_attr       attr;
	void                    *context;
	char                    *map_name;
	struct lpp_domain	*domain;
	struct lpp_ep		*ep;
	ofi_atomic32_t          ref_count;
};

/* Prototypes. */
void		lpp_av_bind_ep(struct lpp_av *, struct lpp_ep *);
int		lpp_fi_av_open(struct fid_domain *, struct fi_av_attr *, struct fid_av **, void *);
int		lpp_fi_av_lookup(struct fid_av *av, fi_addr_t fi_addr, void *addr, size_t *addrlen);
int		lpp_fi_av_close(struct fid *);
int		lpp_fi_av_insert(struct fid_av *, const void *, size_t, fi_addr_t *, uint64_t, void *);
const char *	lpp_fi_av_straddr(struct fid_av *, const void *, char *, size_t *);

void lpp_make_sockaddr(uint8_t fabric_index, uint32_t host_index, uint16_t port,
		       struct sockaddr_in *sin, size_t *addrlen);
int lpp_from_sockaddr(const struct sockaddr_in *sin, uint32_t *host_index,
		      uint16_t *port, bool checkaddr);

#endif // _LPP_AV_H_
