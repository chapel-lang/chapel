/*

  This file is provided under a dual BSD/GPLv2 license.  When using or
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2015 Intel Corporation.

  This program is free software; you can redistribute it and/or modify
  it under the terms of version 2 of the GNU General Public License as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  Contact Information:
  Intel Corporation, www.intel.com

  BSD LICENSE

  Copyright(c) 2015 Intel Corporation.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in
      the documentation and/or other materials provided with the
      distribution.
    * Neither the name of Intel Corporation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

/* Copyright (c) 2003-2014 Intel Corporation. All rights reserved. */

#include "psm_user.h"
#include "ips_recvq.h"

/* We return a table of pointer indexes.
 *
 * From the point of view of the returned pointer, index -1 always points to
 * the address to call psmi_free on (since we force page-alignment).
 */
void **psm3_ips_recvq_egrbuf_table_alloc(psm2_ep_t ep, void *baseptr,
				    uint32_t bufnum, uint32_t bufsize)
{
	unsigned i;
	void *ptr_alloc;
	uintptr_t *buft;
	uintptr_t base = (uintptr_t) baseptr;

	ptr_alloc = psmi_malloc(ep, UNDEFINED,
				PSMI_PAGESIZE + sizeof(uintptr_t) * (bufnum +
								     1));
	if (ptr_alloc == NULL)
		return NULL;
	/* First pointer is to the actual allocated address, so we can free it but
	 * buft[1] is first on the page boundary
	 */
	buft = (uintptr_t *) PSMI_ALIGNUP((uint8_t *)ptr_alloc + 1, PSMI_PAGESIZE);
	buft[-1] = (uintptr_t) ptr_alloc;
	for (i = 0; i < bufnum; i++)
		buft[i] = (uintptr_t) ((char *)base + i * bufsize);
	return (void **)buft;
}

void psm3_ips_recvq_egrbuf_table_free(void **buftable)
{
	uintptr_t *buft = (uintptr_t *) buftable;
	void *ptr_alloc = (void *)buft[-1];
	psmi_free(ptr_alloc);
}
