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

#define __USE_GNU
#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>
#include <syslog.h>
#include <stdio.h>

#include "utils_user.h"

#define SYSLOG_MAXLEN	512

extern char psm3_mylabel[];

void
psm3_vsyslog(const char *prefix, int to_console, int level,
	    const char *format, va_list ap)
{
	char logprefix[SYSLOG_MAXLEN];
	size_t len;

	if (to_console) {
		char hostname[80];
		va_list ap_cons;
		va_copy(ap_cons, ap);
		len = strlen(format);
		gethostname(hostname, sizeof(hostname));
		hostname[sizeof(hostname) - 1] = '\0';

		if (psm3_mylabel[0])
			fprintf(stderr, "%s: ", psm3_mylabel);
		else
			fprintf(stderr, "%s: ", hostname);

		vfprintf(stderr, format, ap_cons);
		if (format[len] != '\n')
			fprintf(stderr, "\n");
		fflush(stderr);
		va_end(ap_cons);
	}

	len = snprintf(logprefix, sizeof(logprefix),
		       "(nic/%s)[%d]: %s", prefix ? prefix : "nic",
		       (int)getpid(), format);

	vsyslog(level | LOG_USER, logprefix, ap);

	return;
}

void
psm3_syslog_internal(const char *prefix, int to_console, int level,
	   const char *format, ...)
{
	va_list ap;
	va_start(ap, format);
	psm3_vsyslog(prefix, to_console, level, format, ap);
	va_end(ap);
}
