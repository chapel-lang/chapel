/*

  This file is provided under a dual BSD/GPLv2 license.  When using or
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2015 Intel Corporation.
  Copyright(c) 2021 Cornelis Networks.

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
  Copyright(c) 2021 Cornelis Networks.

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

/* This file contains hfi service routine interface used by the low */
/* level hfi protocol code. */

#include <sys/poll.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <malloc.h>
#include <time.h>

#include "opa_user_gen1.h"
#include "ofi_prov.h"
#include "rdma/opx/fi_opx.h"

/* touch the pages, with a 32 bit read */
void opx_hfi_touch_mmap(void *m, size_t bytes)
{
	volatile uint32_t *b = (volatile uint32_t *)m, c;
	size_t i;		/* m is always page aligned, so pgcnt exact */
	int __hfi_pg_sz;

	/* First get the page size */
	__hfi_pg_sz = sysconf(_SC_PAGESIZE);

	_HFI_VDBG("Touch %lu mmap'ed pages starting at %p\n",
		  (unsigned long)bytes / __hfi_pg_sz, m);
	bytes /= sizeof(c);
	for (i = 0; i < bytes; i += __hfi_pg_sz / sizeof(c))
		c = b[i];
}

/* set the send context pkey to check BTH pkey in each packet.
   driver should check its pkey table to see if it can find
   this pkey, if not, driver should return error. */
int opx_hfi_set_pkey(struct _hfi_ctrl *ctrl, uint16_t pkey)
{
	struct hfi1_cmd cmd;
	struct hfi1_base_info tbinfo;

	cmd.type = OPX_HFI_CMD_SET_PKEY;
	cmd.len = 0;
	cmd.addr = (uint64_t) pkey;

	_HFI_VDBG("Setting context pkey to 0x%04x.\n", pkey);
	if (opx_hfi_cmd_write(ctrl->fd, &cmd, sizeof(cmd)) == -1) {
		_HFI_INFO("Setting context pkey to 0x%04x failed: %s\n",
			  pkey, strerror(errno));
		return -1;
	} else {
		_HFI_VDBG("Successfully set context pkey to 0x%04x.\n", pkey);
	}

	int selinux = 0;
	fi_param_get_bool(fi_opx_global.prov,"selinux",&selinux);
	if (selinux) {
		/*
		 * If SELinux is in use the kernel may have changed our JKey based on
		 * what we supply for the PKey so go ahead and interrogate the user info
		 * again and update our saved copy. In the future there may be a new
		 * IOCTL to get the JKey only. For now, this temporary workaround works.
		 */
		cmd.type = OPX_HFI_CMD_USER_INFO;
		cmd.len = sizeof(tbinfo);
		cmd.addr = (uint64_t) &tbinfo;

		if (opx_hfi_cmd_write(ctrl->fd, &cmd, sizeof(cmd)) == -1) {
			_HFI_VDBG("BASE_INFO command failed in setpkey: %s\n",
				  strerror(errno));
			return -1;
		}
		_HFI_VDBG("FI_OPX_SELINUX is set, updating jkey to 0x%04x\n", tbinfo.jkey);
		ctrl->base_info.jkey = tbinfo.jkey;
	}

	return 0;
}

/* These have been fixed to read the values, but they are not
 * compatible with the hfi driver, they return new info with
 * the qib driver
 */
static int opx_hfi_count_names(const char *namep)
{
	int n = 0;
	while (*namep != '\0') {
		if (*namep == '\n')
			n++;
		namep++;
	}
	return n;
}

const char *opx_hfi_get_next_name(char **names)
{
char *p, *start;

	p = start = *names;
	while (*p != '\0' && *p != '\n') {
		p++;
	}   
	if (*p == '\n') {
		*p = '\0';
		p++;
		*names = p;
		return start;
	} else
		return NULL;
}

int opx_hfi_lookup_stat(const char *attr, char *namep, uint64_t *stats,
		    uint64_t *s)
{
	const char *p;
	int i, ret = -1, len = strlen(attr);
	int nelem = opx_hfi_count_names(namep);

	for (i = 0; i < nelem; i++) {
		p = opx_hfi_get_next_name(&namep);
		if (p == NULL)
			break;
		if (strncasecmp(p, attr, len + 1) == 0) {
			ret = i;
			*s = stats[i];
		}
	}
	return ret;
}
