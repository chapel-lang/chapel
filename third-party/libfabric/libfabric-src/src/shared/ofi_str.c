/*
 * Copyright (c) 2018 Intel Corp., Inc.  All rights reserved.
 * Copyright (c) 2018 Cisco Systems, Inc.  All rights reserved.
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
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include <rdma/fi_errno.h>

/*
 * TODO remove this and include ofi_osd.h which would require merging osd.h
 * files in libfabric and fabtests
 */
#if defined _WIN32
#define strdup _strdup

static inline char* strsep(char **stringp, const char *delim)
{
	char* ptr = *stringp;
	char* p;

	p = ptr ? strpbrk(ptr, delim) : NULL;

	if(!p)
		*stringp = NULL;
	else
	{
		*p = 0;
		*stringp = p + 1;
	}

	return ptr;
}

char *strcasestr(const char *haystack, const char *needle)
{
	char *uneedle, *uhaystack, *pos = NULL;
	int i;

	uneedle = malloc(strlen(needle) + 1);
	uhaystack = malloc(strlen(haystack) + 1);
	if (!uneedle || !uhaystack)
		goto out;

	for (i = 0; i < strlen(needle); i++)
		uneedle[i] = toupper(needle[i]);
	uneedle[i] = '\0';

	for (i = 0; i < strlen(haystack); i++)
		uhaystack[i] = toupper(haystack[i]);
	uhaystack[i] = '\0';

	pos = strstr(uhaystack, uneedle);
	if (pos)
		pos = (char *) ((uintptr_t) haystack + (uintptr_t) pos -
				(uintptr_t) uhaystack);
out:
	free(uneedle);
	free(uhaystack);
	return pos;
}

#endif

/* String utility functions */

int ofi_rm_substr(char *str, const char *substr)
{
	char *dest, *src;

	dest = strstr(str, substr);
	if (!dest)
		return -FI_EINVAL;

	src = dest + strlen(substr);
	memmove(dest, src, strlen(src) + 1);
	return 0;
}

int ofi_rm_substr_delim(char *str, const char *substr, const char delim)
{
	char *pattern;
	size_t len = strlen(substr) + 2; // account for delim and null char
	int ret;

	pattern = malloc(len);
	if (!pattern)
		return -FI_ENOMEM;

	snprintf(pattern, len, "%c%s", delim, substr);
	ret = ofi_rm_substr(str, pattern);
	if (!ret)
		goto out;

	snprintf(pattern, len, "%s%c", substr, delim);
	ret = ofi_rm_substr(str, pattern);
	if (!ret)
		goto out;

	ret = ofi_rm_substr(str, substr);
out:
	free(pattern);
	return ret;
}

/* Split the given string "s" using the specified delimiter(s) in the string
 * "delim" and return an array of strings. The array is terminated with a NULL
 * pointer. Returned array should be freed with ofi_free_string_array().
 *
 * Returns NULL on failure.
 */

char **ofi_split_and_alloc(const char *s, const char *delim, size_t *count)
{
	int i, n;
	char *tmp;
	char *dup = NULL;
	char **arr = NULL;

	if (!s || !delim)
		return NULL;

	dup = strdup(s);
	if (!dup)
		return NULL;

	/* compute the array size */
	n = 1;
	for (tmp = dup; *tmp != '\0'; ++tmp) {
		for (i = 0; delim[i] != '\0'; ++i) {
			if (*tmp == delim[i]) {
				++n;
				break;
			}
		}
	}

	/* +1 to leave space for NULL terminating pointer */
	arr = calloc(n + 1, sizeof(*arr));
	if (!arr)
		goto cleanup;

	/* set array elts to point inside the dup'ed string */
	for (tmp = dup, i = 0; tmp != NULL; ++i) {
		arr[i] = strsep(&tmp, delim);
	}
	assert(i == n);

	if (count)
		*count = n;
	return arr;

cleanup:
	free(dup);
	free(arr);
	return NULL;
}

/* see ofi_split_and_alloc() */
void ofi_free_string_array(char **s)
{
	/* all strings are allocated from the same strdup'ed slab, so just free
	 * the first element */
	if (s != NULL)
		free(s[0]);

	/* and then the actual array of pointers */
	free(s);
}
