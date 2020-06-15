/*
 * Copyright (c) 2015-2016 Cray Inc.  All rights reserved.
 * Copyright (c) 2015 Los Alamos National Security, LLC. All rights reserved.
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

#ifndef _GNIX_NAMESERVER_H_
#define _GNIX_NAMESERVER_H_

#include "gnix.h"

/*
 * defines, data structs, and prototypes for gnix nameserver
 */

/*
 * prototypes
 */

int _gnix_local_ipaddr(struct sockaddr_in *sin);

int _gnix_pe_to_ip(const struct gnix_ep_name *ep_name,
		   struct sockaddr_in *saddr);

int _gnix_resolve_name(IN const char *node, IN const char *service,
		       IN uint64_t flags, INOUT struct gnix_ep_name
		       *resolved_addr);

int _gnix_src_addr(struct gnix_ep_name *resolved_addr);

#endif /* _GNIX_NAMESERVER_H_ */
