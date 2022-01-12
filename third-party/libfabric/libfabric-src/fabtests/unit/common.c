/*
 * Copyright (c) 2013-2014 Intel Corporation.  All rights reserved.
 * Copyright (c) 2014-2016 Cisco Systems, Inc.  All rights reserved.
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

#include <stdio.h>

#include "unit_common.h"

void ft_unit_usage(char *name, char *desc)
{
	fprintf(stderr, "Usage:\n");
	fprintf(stderr, "  %s [OPTIONS]\n", name);

	if (desc)
		fprintf(stderr, "\n%s\n", desc);

	fprintf(stderr, "\nOptions:\n");
	FT_PRINT_OPTS_USAGE("-f <fabric_name>", "specific fabric to use");
	FT_PRINT_OPTS_USAGE("-d <domain>", "domain name");
	FT_PRINT_OPTS_USAGE("-p <provider_name>", "specific provider name eg sockets, verbs");
	FT_PRINT_OPTS_USAGE("-h", "display this help output");
}

int
run_tests(struct test_entry *test_array, char *err_buf)
{
	int ret;
	struct test_entry *tep;
	int failed;

	failed = 0;

	tep = test_array;
	while (tep->test != NULL) {
		printf("Running %s [%s]...", tep->name, tep->desc);
		fflush(stdout);
		ret = tep->test();
		switch (ret) {
		case PASS:
			printf("PASS!\n");
			break;
		case FAIL:
			printf("FAIL: %s\n", err_buf);
			failed++;
			break;
		case SKIPPED:
			printf("skipped because: %s\n", err_buf);
			break;
		case NOTSUPP:
			printf("requires unsupported feature: %s\n", err_buf);
			break;
		default:
			printf("FATAL: unexpected code: %d\n", ret);
			return failed + 1;
			break;
		}

		++tep;
	}

	return failed;
}
