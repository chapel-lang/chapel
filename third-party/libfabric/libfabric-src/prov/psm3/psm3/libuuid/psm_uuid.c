/*

  This file is provided under a dual BSD/GPLv2 license.  When using or
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2016 Intel Corporation.

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

  Copyright(c) 2016 Intel Corporation.

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

/* Copyright (c) 2003-2016 Intel Corporation. All rights reserved. */

#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include "psm_user.h"
#include "psm_uuid.h"

static void psmi_make_drand_uuid(psm2_uuid_t uuid_out)
{
	struct drand48_data drand48_data;
	int i;
	long int rnum;
	srand48_r((get_cycles() + getpid()) % LONG_MAX, &drand48_data);
	for(i=0; i < 16; i++) {
		lrand48_r(&drand48_data, &rnum);
		uuid_out[i] = rnum % UCHAR_MAX;
	}
}

/* Since libuuid can call srand, we will generate our own uuids */
void
__psm2_uuid_generate(psm2_uuid_t uuid_out)
{
	PSM2_LOG_MSG("entering");
	/* Prefer using urandom, fallback to drand48_r */
	struct stat urandom_stat;
	size_t nbytes;
	int fd;
	if(stat("/dev/urandom", &urandom_stat) != 0) {
		psmi_make_drand_uuid(uuid_out);
		return;
	}

	fd = open("/dev/urandom", O_RDONLY);
	if(fd == -1) {
		psmi_make_drand_uuid(uuid_out);
	} else {
		nbytes = read(fd, (char *) uuid_out, 16);
		if(nbytes != 16) {
			psmi_make_drand_uuid(uuid_out);
		}
		close(fd);
	}
	PSM2_LOG_MSG("leaving");
	return;
}
PSMI_API_DECL(psm2_uuid_generate)

void
psmi_uuid_unparse(const uuid_t uu, char *out)
{
	uuid_unparse_lower(uu, out);
}

int
psmi_uuid_parse(const char *in, uuid_t uu)
{
	return uuid_parse(in, uu);
}

