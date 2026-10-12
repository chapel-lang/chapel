/*
* Copyright (c) 2015-2016 Intel Corporation, Inc.  All rights reserved.
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

#include <ws2tcpip.h>
#include <ws2def.h>
#include <assert.h>


int nd_is_valid_addr(const SOCKADDR *addr)
{
	assert(addr);

	switch (addr->sa_family) {
	case AF_INET:
	{
		struct sockaddr_in *addr4 = (struct sockaddr_in*)addr;
		/* HACK-alert: reject local or MS default IPv4 addrs */
		return !(addr4->sin_addr.S_un.S_un_b.s_b1 == 169 ||
			 addr4->sin_addr.S_un.S_un_b.s_b1 == 127);
	}
	case AF_INET6:
		return 1;
	default:
		break;
	}

	return 0;
}

int nd_addr_cmp(const void* vaddr1, const void* vaddr2)
{
	assert(vaddr1);
	assert(vaddr2);

	const struct sockaddr *addr1 = vaddr1;
	const struct sockaddr *addr2 = vaddr2;

	if (addr1->sa_family < addr2->sa_family)
		return -1;
	else if (addr1->sa_family > addr2->sa_family)
		return 1;

	assert(addr1->sa_family == addr2->sa_family);

	switch (addr1->sa_family) {
	case AF_INET:
		return memcmp(&((struct sockaddr_in*)addr1)->sin_addr,
			      &((struct sockaddr_in*)addr2)->sin_addr,
			      sizeof(((struct sockaddr_in*)addr2)->sin_addr.S_un));
	case AF_INET6:
		return memcmp(&((struct sockaddr_in6*)addr1)->sin6_addr.u,
			      &((struct sockaddr_in6*)addr2)->sin6_addr.u,
			      sizeof(((struct sockaddr_in6*)addr1)->sin6_addr.u));
	default:
		assert(0); /* should never go here */
		break;
	}

	return 0;
}
