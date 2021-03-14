/*
 * Copyright (c) 2013-2014 Intel Corporation.  All rights reserved.
 * Copyright (c) 2016 Cisco Systems, Inc.  All rights reserved.
 *
 * This software is available to you under the BSD license below:
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

#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <rdma/fi_errno.h>

static void usage(const char *argv0)
{
	printf("Usage: %s FI_ERROR_CODE\n", argv0);
	printf("\n");
	printf("Displays the error string for the given numeric FI_ERROR_CODE.\n");
	printf("FI_ERROR_CODE may be a hexadecimal, octal, or decimal constant.\n");
	printf("For convenience, the absolute value of FI_ERROR_CODE will be used.\n");
}

int main(int argc, char *argv[])
{
	char *endptr;
	long err;

	if (argc != 2) {
		usage(argv[0]);
		return EXIT_FAILURE;
	}

	if (strcmp(argv[1], "-h") == 0) {
		usage(argv[0]);
		return EXIT_SUCCESS;
	}

	errno = 0;
	endptr = NULL;
	err = strtol(argv[1], &endptr, 0);
	if (errno || endptr == argv[1] || *endptr != '\0') {
		printf("ERROR: unable to parse '%s'\n\n", argv[1]);
		usage(argv[0]);
		return EXIT_FAILURE;
	}

	if (err < (long)INT_MIN || err > (long)INT_MAX) {
		printf("ERROR: '%s' is out of range\n\n", argv[1]);
		usage(argv[0]);
		return EXIT_FAILURE;
	}

	printf("%s\n", fi_strerror(err >= 0 ? (int)err : (int)-err));

	return EXIT_SUCCESS;
}
