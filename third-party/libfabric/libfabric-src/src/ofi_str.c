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
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>

#include <rdma/fi_errno.h>

#include "ofi.h"

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

void ofi_strncatf(char *dest, size_t n, const char *fmt, ...)
{
	size_t len = strnlen(dest, n);
	va_list arglist;

	va_start(arglist, fmt);
	vsnprintf(&dest[len], n - 1 - len, fmt, arglist);
	va_end(arglist);
}

char *ofi_tostr_size(char *str, size_t len, uint64_t size)
{
	uint64_t base = 0;
	uint64_t fraction = 0;
	char mag;

	if (size >= (1 << 30)) {
		base = 1 << 30;
		mag = 'G';
	} else if (size >= (1 << 20)) {
		base = 1 << 20;
		mag = 'M';
	} else if (size >= (1 << 10)) {
		base = 1 << 10;
		mag = 'K';
	} else {
		base = 1;
		mag = '\0';
	}

	if (size / base < 10)
		fraction = (size % base) * 10 / base;

	if (fraction)
		ofi_strncatf(str, len, "%lu.%lu%c", size / base, fraction, mag);
	else
		ofi_strncatf(str, len, "%lu%c", size / base, mag);

	return str;
}

char *ofi_tostr_count(char *str, size_t len, uint64_t count)
{
	if (count >= 1000000000)
		ofi_strncatf(str, len, "%luB", count / 1000000000);
	else if (count >= 1000000)
		ofi_strncatf(str, len, "%luM", count / 1000000);
	else if (count >= 1000)
		ofi_strncatf(str, len, "%luK", count / 1000);
	else
		ofi_strncatf(str, len, "%lu", count);

	return str;
}

static void ofi_tostr_device_attr(char *buf, size_t len,
				  const struct fi_device_attr *attr)
{
	const char *prefix = TAB TAB;

	ofi_strncatf(buf, len, "%sfi_device_attr:\n", prefix);

	prefix = TAB TAB TAB;
	ofi_strncatf(buf, len, "%sname: %s\n", prefix, attr->name);
	ofi_strncatf(buf, len, "%sdevice_id: %s\n", prefix, attr->device_id);
	ofi_strncatf(buf, len, "%sdevice_version: %s\n", prefix,
		     attr->device_version);
	ofi_strncatf(buf, len, "%svendor_id: %s\n", prefix, attr->vendor_id);
	ofi_strncatf(buf, len, "%sdriver: %s\n", prefix, attr->driver);
	ofi_strncatf(buf, len, "%sfirmware: %s\n", prefix, attr->firmware);
}

static void ofi_tostr_pci_attr(char *buf, size_t len,
			       const struct fi_pci_attr *attr)
{
	const char *prefix = TAB TAB TAB;

	ofi_strncatf(buf, len, "%sfi_pci_attr:\n", prefix);

	prefix = TAB TAB TAB TAB;
	ofi_strncatf(buf, len, "%sdomain_id: %u\n", prefix, attr->domain_id);
	ofi_strncatf(buf, len, "%sbus_id: %u\n", prefix, attr->bus_id);
	ofi_strncatf(buf, len, "%sdevice_id: %u\n", prefix, attr->device_id);
	ofi_strncatf(buf, len, "%sfunction_id: %u\n", prefix, attr->function_id);
}

static void ofi_tostr_bus_type(char *buf, size_t len, int type)
{
	switch (type) {
	CASEENUMSTRN(FI_BUS_UNKNOWN, len);
	CASEENUMSTRN(FI_BUS_PCI, len);
	default:
		ofi_strncatf(buf, len, "Unknown");
		break;
	}
}

static void ofi_tostr_bus_attr(char *buf, size_t len,
			       const struct fi_bus_attr *attr)
{
	const char *prefix = TAB TAB;

	ofi_strncatf(buf, len, "%sfi_bus_attr:\n", prefix);

	prefix = TAB TAB TAB;
	ofi_strncatf(buf, len, "%sbus_type: ", prefix);
	ofi_tostr_bus_type(buf, len, attr->bus_type);
	ofi_strncatf(buf, len, "\n");

	switch (attr->bus_type) {
	case FI_BUS_PCI:
		ofi_tostr_pci_attr(buf, len, &attr->attr.pci);
		break;
	default:
		break;
	}
}

static void ofi_tostr_link_state(char *buf, size_t len, int state)
{
	switch (state) {
	CASEENUMSTRN(FI_LINK_UNKNOWN, len);
	CASEENUMSTRN(FI_LINK_DOWN, len);
	CASEENUMSTRN(FI_LINK_UP, len);
	default:
		ofi_strncatf(buf, len, "Unknown");
		break;
	}
}

static void ofi_tostr_link_attr(char *buf, size_t len,
				const struct fi_link_attr *attr)
{
	const char *prefix = TAB TAB;
	ofi_strncatf(buf, len, "%sfi_link_attr:\n", prefix);

	prefix = TAB TAB TAB;
	ofi_strncatf(buf, len, "%saddress: %s\n", prefix, attr->address);
	ofi_strncatf(buf, len, "%smtu: %zu\n", prefix, attr->mtu);
	ofi_strncatf(buf, len, "%sspeed: %zu\n", prefix, attr->speed);
	ofi_strncatf(buf, len, "%sstate: ", prefix);
	ofi_tostr_link_state(buf, len, attr->state);
	ofi_strncatf(buf, len, "\n%snetwork_type: %s\n", prefix,
		     attr->network_type);
}

int ofi_nic_tostr(const struct fid *fid_nic, char *buf, size_t len)
{
	const struct fid_nic *nic = (const struct fid_nic*) fid_nic;

	assert(fid_nic->fclass == FI_CLASS_NIC);
	ofi_strncatf(buf, len, "%snic:\n", TAB);

	ofi_tostr_device_attr(buf, len, nic->device_attr);
	ofi_tostr_bus_attr(buf, len, nic->bus_attr);
	ofi_tostr_link_attr(buf, len, nic->link_attr);
	return 0;
}
