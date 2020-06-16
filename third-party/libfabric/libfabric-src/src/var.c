/*
 * Copyright (c) 2015-2016, Cisco Systems, Inc. All rights reserved.
 * Copyright (c) 2015, Intel Corp., Inc.  All rights reserved.
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
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include <rdma/fi_errno.h>

#include "ofi.h"
#include "ofi_list.h"


extern int ofi_init;
extern void fi_ini(void);

struct fi_param_entry {
	const struct fi_provider *provider;
	char *name;
	enum fi_param_type type;
	char *help_string;
	char *env_var_name;
	struct dlist_entry entry;
};

/* TODO: Add locking around param_list when adding dynamic removal */
static DEFINE_LIST(param_list);


static struct fi_param_entry *
fi_find_param(const struct fi_provider *provider, const char *param_name)
{
	struct fi_param_entry *param;
	struct dlist_entry *entry;

	for (entry = param_list.next; entry != &param_list; entry = entry->next) {
		param = container_of(entry, struct fi_param_entry, entry);
		if (param->provider == provider &&
		    strcmp(param->name, param_name) == 0) {
			return param;
		}
	}

	FI_DBG(provider, FI_LOG_CORE,
		"Failed to find parameter %s: was not defined\n", param_name);
	return NULL;
}

__attribute__((visibility ("default"),EXTERNALLY_VISIBLE))
int DEFAULT_SYMVER_PRE(fi_getparams)(struct fi_param **params, int *count)
{
	struct fi_param *vhead = NULL;
	struct fi_param_entry *param;
	struct dlist_entry *entry;
	int cnt, i;
	char *tmp;

	if (!ofi_init)
		fi_ini();

	for (entry = param_list.next, cnt = 0; entry != &param_list;
	     entry = entry->next)
		cnt++;

	if (cnt == 0)
		goto out;

	// last extra entry will be all NULL
	vhead = calloc(cnt + 1, sizeof (*vhead));
	if (!vhead)
		return -FI_ENOMEM;

	for (entry = param_list.next, i = 0; entry != &param_list;
	     entry = entry->next, i++) {
		param = container_of(entry, struct fi_param_entry, entry);
		vhead[i].name = strdup(param->env_var_name);
		vhead[i].type = param->type;
		vhead[i].help_string = strdup(param->help_string);

		tmp = getenv(param->env_var_name);
		if (tmp)
			vhead[i].value = strdup(tmp);

		if (!vhead[i].name || !vhead[i].help_string) {
			fi_freeparams(vhead);
			return -FI_ENOMEM;
		}
	}

out:
	*count = cnt;
	*params = vhead;
	return FI_SUCCESS;
}
DEFAULT_SYMVER(fi_getparams_, fi_getparams, FABRIC_1.0);

__attribute__((visibility ("default"),EXTERNALLY_VISIBLE))
void DEFAULT_SYMVER_PRE(fi_freeparams)(struct fi_param *params)
{
	int i;
	for (i = 0; params[i].name; ++i) {
		free((void*) params[i].name);
		free((void*) params[i].help_string);
		free((void*) params[i].value);
	}
	free(params);
}
DEFAULT_SYMVER(fi_freeparams_, fi_freeparams, FABRIC_1.0);

static void fi_free_param(struct fi_param_entry *param)
{
	free(param->name);
	free(param->help_string);
	free(param->env_var_name);
	free(param);
}

void fi_param_undefine(const struct fi_provider *provider)
{
	struct fi_param_entry *param;
	struct dlist_entry *entry;
	struct dlist_entry *next;

	for (entry = param_list.next; entry != &param_list; entry = next) {
		next = entry->next;
		param = container_of(entry, struct fi_param_entry, entry);
		if (param->provider == provider) {
			FI_DBG(provider, FI_LOG_CORE, "Removing param: %s\n", param->name);
			dlist_remove(entry);
			fi_free_param(param);
		}
	}
}

__attribute__((visibility ("default"),EXTERNALLY_VISIBLE))
int DEFAULT_SYMVER_PRE(fi_param_define)(const struct fi_provider *provider,
		const char *param_name, enum fi_param_type type,
		const char *help_string_fmt, ...)
{
	int i, ret;
	struct fi_param_entry *v;
	char *tmp_str;
	va_list vargs;

	if (!provider)
		provider = &core_prov;

	// Check for bozo cases
	if (param_name == NULL || help_string_fmt == NULL || *help_string_fmt == '\0') {
		FI_DBG(provider, FI_LOG_CORE,
			"Failed to register %s variable: provider coding error\n",
			param_name);
		return -FI_EINVAL;
	}

	v = calloc(1, sizeof(*v));
	if (!v) {
		FI_DBG(provider, FI_LOG_CORE,
			"Failed to register %s variable: ENOMEM\n", param_name);
		return -FI_ENOMEM;
	}

	v->provider = provider;
	v->name = strdup(param_name);
	v->type = type;

	va_start(vargs, help_string_fmt);
	ret = vasprintf(&v->help_string, help_string_fmt, vargs);
	va_end(vargs);
	if (ret < 0)
		v->help_string = NULL;

	if (provider != &core_prov) {
		ret = asprintf(&tmp_str, "%s: %s", provider->name, v->help_string);
		free(v->help_string);
		if (ret < 0)
			v->help_string = NULL;
		v->help_string = tmp_str;
		ret = asprintf(&v->env_var_name, "FI_%s_%s", provider->name, param_name);
		if (ret < 0)
			v->env_var_name = NULL;
	} else {
		ret = asprintf(&v->env_var_name, "FI_%s", param_name);
		if (ret < 0)
			v->env_var_name = NULL;
	}
	if (!v->name || !v->help_string || !v->env_var_name) {
		fi_free_param(v);
		FI_DBG(provider, FI_LOG_CORE,
			"Failed to register %s variable: ENOMEM\n", param_name);
		return -FI_ENOMEM;
	}

	for (i = 0; v->env_var_name[i]; ++i)
		v->env_var_name[i] = (char) toupper(v->env_var_name[i]);

	dlist_insert_tail(&v->entry, &param_list);

	FI_DBG(provider, FI_LOG_CORE, "registered var %s\n", param_name);
	return FI_SUCCESS;
}
DEFAULT_SYMVER(fi_param_define_, fi_param_define, FABRIC_1.0);

static int fi_parse_bool(const char *str_value)
{
	if (strcmp(str_value, "0") == 0 ||
	    strcasecmp(str_value, "false") == 0 ||
	    strcasecmp(str_value, "no") == 0 ||
	    strcasecmp(str_value, "off") == 0) {
		return 0;
	}

	if (strcmp(str_value, "1") == 0 ||
	    strcasecmp(str_value, "true") == 0 ||
	    strcasecmp(str_value, "yes") == 0 ||
	    strcasecmp(str_value, "on") == 0) {
		return 1;
	}

	return -1;
}

__attribute__((visibility ("default"),EXTERNALLY_VISIBLE))
int DEFAULT_SYMVER_PRE(fi_param_get)(struct fi_provider *provider,
		const char *param_name, void *value)
{
	struct fi_param_entry *param;
	char *str_value;
	int ret = FI_SUCCESS;

	if (!provider)
		provider = &core_prov;

	if (!param_name || !value) {
		FI_DBG(provider, FI_LOG_CORE,
			"Failed to read %s variable: provider coding error\n",
			param_name);
		return -FI_EINVAL;
	}

	param = fi_find_param(provider, param_name);
	if (!param)
		return -FI_ENOENT;

	str_value = getenv(param->env_var_name);
	if (!str_value) {
		FI_INFO(provider, FI_LOG_CORE,
			"variable %s=<not set>\n", param_name);
		ret = -FI_ENODATA;
		goto out;
	}

	switch (param->type) {
	case FI_PARAM_STRING:
		* ((char **) value) = str_value;
		FI_INFO(provider, FI_LOG_CORE,
			"read string var %s=%s\n", param_name, *(char **) value);
		break;
	case FI_PARAM_INT:
		* ((int *) value) = strtol(str_value, NULL, 0);
		FI_INFO(provider, FI_LOG_CORE,
			"read int var %s=%d\n", param_name, *(int *) value);
		break;
	case FI_PARAM_BOOL:
		* ((int *) value) = fi_parse_bool(str_value);
		FI_INFO(provider, FI_LOG_CORE,
			"read bool var %s=%d\n", param_name, *(int *) value);
		if (*(int *) value == -1)
			ret = -FI_EINVAL;
		break;
	case FI_PARAM_SIZE_T:
		* ((size_t *) value) = strtol(str_value, NULL, 0);
		FI_INFO(provider, FI_LOG_CORE,
			"read long var %s=%zu\n", param_name, *(size_t *) value);
		break;
	}

out:
	return ret;
}
DEFAULT_SYMVER(fi_param_get_, fi_param_get, FABRIC_1.0);


void fi_param_init(void)
{
	dlist_init(&param_list);
}

void fi_param_fini(void)
{
	struct fi_param_entry *param;
	struct dlist_entry *entry;

	while (!dlist_empty(&param_list)) {
		entry = param_list.next;
		param = container_of(entry, struct fi_param_entry, entry);
		dlist_remove(entry);
		fi_free_param(param);
	}
}
