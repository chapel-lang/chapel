/*
 * Copyright (c) 2022 Canon Medical Research USA, Inc.  All rights reserved.
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

#ifndef _VERBS_OSD_H_
#define _VERBS_OSD_H_

#include <stdio.h>

static inline int vrb_os_ini()
{
    return 0;
}

static inline void vrb_os_fini()
{
}

static inline void vrb_os_mem_support(bool *peer_mem, bool *dmabuf)
{
	char *line = NULL;
	size_t line_size = 0;
	ssize_t bytes;
	FILE *kallsyms_fd;

	kallsyms_fd = fopen("/proc/kallsyms", "r");
	if (!kallsyms_fd)
		return;

	while ((bytes = getline(&line, &line_size, kallsyms_fd)) != -1) {
		if (strstr(line, "ib_register_peer_memory_client"))
			*peer_mem = true;
		else if (strstr(line, "ib_umem_dmabuf_get"))
			*dmabuf = true;
		if (*peer_mem && *dmabuf)
			break;
	}

	free(line);
	fclose(kallsyms_fd);
}

#endif
