/*
 * Copyright (c) 2004, 2005 Topspin Communications.  All rights reserved.
 * Copyright (c) 2006-2016 Cisco Systems, Inc.  All rights reserved.
 * Copyright (c) 2013-2017 Intel Corp., Inc.  All rights reserved.
 * (C) Copyright 2020 Hewlett Packard Enterprise Development LP
 * Copyright (c) 2022 Intel Corporation. All rights reserved.
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

#include "config.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>

#include <rdma/fi_errno.h>
#include "ofi_util.h"
#include "ofi.h"
#include "ofi_str.h"
#include "ofi_prov.h"
#include "ofi_perf.h"
#include "ofi_hmem.h"
#include "ofi_mr.h"
#include <ofi_shm_p2p.h>
#include <rdma/fi_ext.h>

#ifdef HAVE_LIBDL
#include <dlfcn.h>
#endif


struct ofi_prov {
	struct ofi_prov		*next;
	char			*prov_name;
	struct fi_provider	*provider;
	void			*dlhandle;
	bool			hidden;
	bool			preferred;
};

enum ofi_prov_order {
	OFI_PROV_ORDER_VERSION,
	OFI_PROV_ORDER_REGISTER,
};

struct ofi_info_match {
	const char *prov_name;
	enum fi_ep_type ep_type;
};

static struct ofi_prov *prov_head, *prov_tail;
static enum ofi_prov_order prov_order = OFI_PROV_ORDER_VERSION;
static bool prov_preferred = false;
int ofi_init = 0;
extern struct ofi_common_locks common_locks;

static struct ofi_filter prov_filter;


static struct ofi_prov *
ofi_alloc_prov(const char *prov_name)
{
	struct ofi_prov *prov;

	prov = calloc(sizeof *prov, 1);
	if (!prov)
		return NULL;

	prov->prov_name = strdup(prov_name);
	if (!prov->prov_name) {
		free(prov);
		return NULL;
	}

	return prov;
}

static void
ofi_init_prov(struct ofi_prov *prov, struct fi_provider *provider,
	      void *dlhandle)
{
	prov->provider = provider;
	prov->dlhandle = dlhandle;
	prov->preferred = prov_preferred;
}

static void ofi_cleanup_prov(struct fi_provider *provider, void *dlhandle)
{
	if (provider) {
		fi_param_undefine(provider);
		if (provider->cleanup)
			provider->cleanup();
	}

#ifdef HAVE_LIBDL
	if (dlhandle)
		dlclose(dlhandle);
#else
	OFI_UNUSED(dlhandle);
#endif
}

static void ofi_free_prov(struct ofi_prov *prov)
{
	ofi_cleanup_prov(prov->provider, prov->dlhandle);
	free(prov->prov_name);
	free(prov);
}

static inline bool ofi_hide_cur_prov(struct ofi_prov *cur,
				     struct ofi_prov *new)
{
	if (cur->preferred)
		return false;

	if (new->preferred)
		return true;

	return (prov_order == OFI_PROV_ORDER_VERSION &&
		FI_VERSION_LT(cur->provider->version, new->provider->version));
}

static void ofi_insert_prov(struct ofi_prov *prov)
{
	struct ofi_prov *cur, *prev;

	for (prev = NULL, cur = prov_head; cur; prev = cur, cur = cur->next) {
		if ((strlen(prov->prov_name) == strlen(cur->prov_name)) &&
		    !strcasecmp(prov->prov_name, cur->prov_name)) {
			if (ofi_hide_cur_prov(cur, prov)) {
				cur->hidden = true;
				prov->next = cur;
				if (prev)
					prev->next = prov;
				else
					prov_head = prov;
			} else {
				prov->hidden = true;
				prov->next = cur->next;
				cur->next = prov;
				if (prov_tail == cur)
					prov_tail = prov;
			}
			return;
		}
	}

	if (prov_tail)
		prov_tail->next = prov;
	else
		prov_head = prov;
	prov_tail = prov;
}

static int ofi_find_name(char **names, const char *name)
{
	int i;

	for (i = 0; names[i]; i++) {
		if (!strcasecmp(name, names[i]))
			return i;
	}
	return -1;
}

/* matches if names[i] == "xxx;yyy" and name == "xxx" */
static int ofi_find_layered_name(char **names, const char *name)
{
	int i;
	size_t len;

	len = strlen(name);
	for (i = 0; names[i]; i++) {
		if (!strncasecmp(name, names[i], len) && names[i][len] == ';' )
			return i;
	}
	return -1;
}

/* matches if names[i] == "xxx" and name == "xxx;yyy" */
static int ofi_find_core_name(char **names, const char *name)
{
	int i;
	size_t len;

	for (i = 0; names[i]; i++) {
		len = strlen(names[i]);
		if (!strncasecmp(name, names[i], len) && name[len] == ';' )
			return i;
	}
	return -1;
}

static void ofi_closest_prov_names(char *prov_name, char* miss_prov_name, int n)
{
	if (strncasecmp( prov_name, miss_prov_name, n ) == 0 ) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"Instead misspelled provider: %s, you may want: %s?\n",
			miss_prov_name, prov_name);
	}
}

static void ofi_suggest_prov_names(char *name_to_match)
{
	struct ofi_prov *prov;
	for (prov = prov_head; prov; prov = prov->next) {
		if (strlen(prov->prov_name) != strlen(name_to_match)
		    && !strncasecmp(prov->prov_name, name_to_match,
				    strlen(name_to_match))) {
			if (strlen(name_to_match) > 5)
				ofi_closest_prov_names(prov->prov_name,
						       name_to_match, 5);
			else
				ofi_closest_prov_names(prov->prov_name,
						       name_to_match, 2);
		}
	}
}

static int ofi_is_util_prov(const struct fi_provider *provider)
{
	return ofi_prov_ctx(provider)->type == OFI_PROV_UTIL;
}

static int ofi_is_core_prov(const struct fi_provider *provider)
{
	return ofi_prov_ctx(provider)->type == OFI_PROV_CORE;
}

static int ofi_is_hook_prov(const struct fi_provider *provider)
{
	return ofi_prov_ctx(provider)->type == OFI_PROV_HOOK;
}

int ofi_apply_filter(struct ofi_filter *filter, const char *name)
{
	if (!filter->names)
		return 0;

	if (ofi_find_name(filter->names, name) >= 0)
		return filter->negated;

	return !filter->negated;
}

/*
 * The provider init filter is used to filter out unnecessary core providers
 * at the initialization time. Utility providers are not concerned.
 *
 * Special handling is needed for layered provider names:
 *
 * If the filter is not negated, a name "xxx;yyy" in the filter should match
 * input "xxx" to ensure that the core provider "xxx" is included.
 *
 * If the filter is negated, a name "xxx;yyy" in the filter should not match
 * input "xxx" otherwise the core provider "xxx" may be incorrectly filtered
 * out.
 */
static bool
ofi_apply_prov_init_filter(struct ofi_filter *filter, const char *name)
{
	if (!filter->names)
		return false;

	if (ofi_find_name(filter->names, name) >= 0)
		return filter->negated;

	if (filter->negated)
		return false;

	if (ofi_find_layered_name(filter->names, name) >= 0)
		return false;

	return true;
}

/*
 * The provider post filter is used to remove unwanted entries from the fi_info
 * list before returning from fi_getinfo().
 *
 * Layered provider names are handled in the same way as non-layered provider
 * names -- requiring full match.
 *
 * In addition, a name "xxx" in the filter should be able to match an input
 * "xxx;yyy" to allow extra layering on top of what is requested by the user.
 */
static bool
ofi_apply_prov_post_filter(struct ofi_filter *filter, const char *name)
{
	if (!filter->names)
		return false;

	if (ofi_find_name(filter->names, name) >= 0 ||
	    ofi_find_core_name(filter->names, name) >= 0)
		return filter->negated;

	return !filter->negated;
}

static bool
ofi_filter_by_names(const struct fi_info *hints, struct fi_info *info)
{
	if (ofi_apply_prov_post_filter(&prov_filter,
				       info->fabric_attr->prov_name))
		return true;

	if (!hints)
		return false;

	if (hints->domain_attr && hints->domain_attr->name &&
	    strncasecmp(hints->domain_attr->name, info->domain_attr->name,
			strlen(hints->domain_attr->name) + 1))
		return true;

	if (hints->fabric_attr && hints->fabric_attr->name &&
	    strncasecmp(hints->fabric_attr->name, info->fabric_attr->name,
			strlen(hints->fabric_attr->name) + 1))
		return true;

	return false;
}

static bool ofi_have_name_filter(const struct fi_info *hints)
{
	if (prov_filter.names)
		return true;

	if (hints && hints->domain_attr && hints->domain_attr->name)
		return true;

	if (hints && hints->fabric_attr && hints->fabric_attr->name)
		return true;

	return false;
}

static bool ofi_getinfo_filter(const struct fi_provider *provider)
{
	/* Positive filters only apply to core providers.  They must be
	 * explicitly enabled by the filter.  Other providers (i.e. utility)
	 * are automatically enabled in this case, so that they can work
	 * over any enabled core filter.  Negative filters may be used
	 * to disable any provider.
	 */
	if (!prov_filter.negated && !ofi_is_core_prov(provider))
		return false;

	return ofi_apply_prov_init_filter(&prov_filter, provider->name);
}

static void ofi_filter_info(const struct fi_info *hints, struct fi_info **info)
{
	struct fi_info *cur, *prev, *tmp;

	if (!ofi_have_name_filter(hints))
		return;

	prev = NULL;
	cur = *info;
	while (cur) {
		assert(cur->fabric_attr && cur->fabric_attr->prov_name);

		if (ofi_filter_by_names(hints, cur)) {
			tmp = cur;
			cur = cur->next;
			if (prev)
				prev->next = cur;
			else
				*info = cur;
			tmp->next = NULL;
			fi_freeinfo(tmp);
		} else {
			prev = cur;
			cur = cur->next;
		}
	}
}

static struct ofi_prov *ofi_getprov(const char *prov_name, size_t len)
{
	struct ofi_prov *prov;

	for (prov = prov_head; prov; prov = prov->next) {
		if ((strlen(prov->prov_name) == len) &&
		    !strncasecmp(prov->prov_name, prov_name, len))
			return prov;
	}

	return NULL;
}

static struct fi_provider *ofi_get_hook(const char *name)
{
	struct ofi_prov *prov;
	struct fi_provider *provider = NULL;
	char *try_name = NULL;
	int ret;

	prov = ofi_getprov(name, strlen(name));
	if (!prov) {
		ret = asprintf(&try_name, "ofi_hook_%s", name);
		if (ret > 0)
			prov = ofi_getprov(try_name, ret);
		else
			try_name = NULL;
	}

	if (prov) {
		if (prov->provider && ofi_is_hook_prov(prov->provider)) {
			provider = prov->provider;
		} else {
			FI_WARN(&core_prov, FI_LOG_CORE,
				"Specified provider is not a hook: %s\n", name);
		}
	} else {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"No hook found for: %s\n", name);
	}

	free(try_name);
	return provider;
}

/* This is the default order that providers will be accessed when available.
 * This, in turn, sets the default ordering of fi_info's reported to the user.
 * However, ofi_reorder_info() may re-arrange the list based on hard-coded
 * criteria.
 */
static void ofi_ordered_provs_init(void)
{
	char *ordered_prov_names[] = {
		"efa", "psm2", "opx", "usnic", "verbs", "cxi",
		"psm3", "ucx", "ofi_rxm", "ofi_rxd", "shm",

		/* Initialize the socket based providers last of the
		 * standard providers.  This will result in them being
		 * the least preferred providers.
		 */

		/* Before you add ANYTHING here, read the comment above!!! */
		"udp", "tcp", "sockets", "net", /* NOTHING GOES HERE! */
		/* Seriously, read it! */

		/* These are hooking providers only.  Their order
		 * doesn't matter
		 */
		"ofi_hook_perf", "ofi_hook_trace", "ofi_hook_profile", "ofi_hook_debug",
		"ofi_hook_noop", "ofi_hook_hmem", "ofi_hook_dmabuf_peer_mem",

		/* So do the offload providers. */
		"off_coll",
	};
	struct ofi_prov *prov;
	int num_provs, i;

	num_provs = sizeof(ordered_prov_names) / sizeof(ordered_prov_names[0]);

	for (i = 0; i < num_provs; i++) {
		prov = ofi_alloc_prov(ordered_prov_names[i]);
		if (prov)
			ofi_insert_prov(prov);
	}
}

static void ofi_set_prov_type(struct fi_provider *provider)
{
	if (!provider->getinfo)
		ofi_prov_ctx(provider)->type = OFI_PROV_HOOK;
	else if (ofi_has_util_prefix(provider->name))
		ofi_prov_ctx(provider)->type = OFI_PROV_UTIL;
	else if (ofi_has_offload_prefix(provider->name))
		ofi_prov_ctx(provider)->type = OFI_PROV_OFFLOAD;
	else
		ofi_prov_ctx(provider)->type = OFI_PROV_CORE;
}

static void ofi_register_provider(struct fi_provider *provider, void *dlhandle)
{
	struct ofi_prov *prov = NULL;
	bool hidden = false;

	if (!provider || !provider->name) {
		FI_DBG(&core_prov, FI_LOG_CORE,
		       "no provider structure or name\n");
		goto cleanup;
	}

	FI_INFO(&core_prov, FI_LOG_CORE,
	       "registering provider: %s (%d.%d)\n", provider->name,
	       FI_MAJOR(provider->version), FI_MINOR(provider->version));

	if (!provider->fabric) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"provider missing mandatory entry points\n");
		goto cleanup;
	}

	/* The current core implementation is not backward compatible
	 * with providers that support a release earlier than v1.3.
	 * See commit 0f4b6651.
	 */
	if (provider->fi_version < FI_VERSION(1, 3)) {
		FI_INFO(&core_prov, FI_LOG_CORE,
			"provider has unsupported FI version "
			"(provider %d.%d != libfabric %d.%d); ignoring\n",
			FI_MAJOR(provider->fi_version),
			FI_MINOR(provider->fi_version), FI_MAJOR_VERSION,
			FI_MINOR_VERSION);
		goto cleanup;
	}

	ofi_set_prov_type(provider);

	if (ofi_getinfo_filter(provider)) {
		FI_INFO(&core_prov, FI_LOG_CORE,
			"\"%s\" filtered by provider include/exclude "
			"list, skipping\n", provider->name);
		hidden = true;
	}

	if (ofi_apply_filter(&prov_log_filter, provider->name))
		ofi_prov_ctx(provider)->disable_logging = true;

	/* Prevent utility providers from layering on these core providers
	 * unless explicitly requested.
	 */
	if (!strcasecmp(provider->name, "sockets") ||
	    !strcasecmp(provider->name, "shm") ||
	    !strcasecmp(provider->name, "efa") ||
	    !strcasecmp(provider->name, "psm3") ||
	    !strcasecmp(provider->name, "ucx") ||
	    !strcasecmp(provider->name, "cxi") ||
	    ofi_is_util_prov(provider))
		ofi_prov_ctx(provider)->disable_layering = true;

	prov = ofi_getprov(provider->name, strlen(provider->name));
	if (prov && !prov->provider) {
		ofi_init_prov(prov, provider, dlhandle);
	} else {
		prov = ofi_alloc_prov(provider->name);
		if (!prov)
			goto cleanup;

		ofi_init_prov(prov, provider, dlhandle);
		ofi_insert_prov(prov);
	}

	if (hidden)
		prov->hidden = true;
	return;

cleanup:
	ofi_cleanup_prov(provider, dlhandle);
}

#ifdef HAVE_LIBDL
static int lib_filter(const struct dirent *entry)
{
	size_t l = strlen(entry->d_name);
	size_t sfx = sizeof (FI_LIB_SUFFIX) - 1;

	if (l > sfx)
		return !strcmp(&(entry->d_name[l-sfx]), FI_LIB_SUFFIX);
	else
		return 0;
}
#endif

static int verify_filter_names(char **names)
{
	int i, j;
	char** split_names;
	for (i = 0; names[i]; i++) {
		split_names = ofi_split_and_alloc(names[i], ";", NULL);
		if (!split_names) {
			FI_WARN(&core_prov, FI_LOG_CORE,
				"unable to parse given filter string\n");
			return -FI_ENODATA;
		}

		for(j = 0; split_names[j]; j++) {
			if(!ofi_getprov(split_names[j], strlen(split_names[j]))) {
				FI_WARN(&core_prov, FI_LOG_CORE,
					"provider %s is unknown, misspelled"
					" or DL provider?\n", split_names[j]);
				ofi_suggest_prov_names(split_names[j]);
			}
		}
		ofi_free_string_array(split_names);
	}

	return FI_SUCCESS;
}

void ofi_free_filter(struct ofi_filter *filter)
{
	ofi_free_string_array(filter->names);
}

void ofi_create_filter(struct ofi_filter *filter, const char *raw_filter)
{
	memset(filter, 0, sizeof *filter);
	if (raw_filter == NULL)
		return;

	if (*raw_filter == '^') {
		filter->negated = true;
		++raw_filter;
	}

	filter->names = ofi_split_and_alloc(raw_filter, ",", NULL);
	if (!filter->names) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"unable to parse filter from: %s\n", raw_filter);
		return;
	}

	if (verify_filter_names(filter->names))
		FI_WARN(&core_prov, FI_LOG_CORE,
		        "unable to verify filter name\n");
}

#ifdef HAVE_LIBDL
static void ofi_reg_dl_prov(const char *lib, bool lib_known_to_exist)
{
	void *dlhandle;
	struct fi_provider* (*inif)(void);

	FI_DBG(&core_prov, FI_LOG_CORE, "opening provider lib %s\n", lib);

	dlhandle = dlopen(lib, RTLD_NOW);
	if (dlhandle == NULL) {
		if (lib_known_to_exist) {
			FI_WARN(&core_prov, FI_LOG_CORE,
				"dlopen(%s): %s\n", lib, dlerror());
		} else {
			FI_DBG(&core_prov, FI_LOG_CORE,
				"dlopen(%s): %s\n", lib, dlerror());
		}
		return;
	}

	inif = dlsym(dlhandle, "fi_prov_ini");
	if (inif == NULL) {
		FI_WARN(&core_prov, FI_LOG_CORE, "dlsym: %s\n", dlerror());
		dlclose(dlhandle);
	} else {
		ofi_register_provider((inif)(), dlhandle);
	}
}

static void ofi_ini_dir(const char *dir)
{
	int n;
	char *lib;
	struct dirent **liblist = NULL;

	n = scandir(dir, &liblist, lib_filter, alphasort);
	if (n < 0)
		goto libdl_done;

	while (n--) {
		if (asprintf(&lib, "%s/%s", dir, liblist[n]->d_name) < 0) {
			FI_WARN(&core_prov, FI_LOG_CORE,
			       "asprintf failed to allocate memory\n");
			goto libdl_done;
		}
		ofi_reg_dl_prov(lib, true);

		free(liblist[n]);
		free(lib);
	}

libdl_done:
	while (n-- > 0)
		free(liblist[n]);
	free(liblist);
}

/* Search standard system library paths (i.e. LD_LIBRARY_PATH) for DLLs for
 * known providers.
 */
static void ofi_find_prov_libs(void)
{
	const char* lib_prefix = "lib";
	struct ofi_prov *prov;
	char* lib;
	char* short_prov_name;

	for (prov = prov_head; prov; prov = prov->next) {
		if (!prov->prov_name)
			continue;

		if (ofi_has_util_prefix(prov->prov_name)) {
			short_prov_name = prov->prov_name + strlen(OFI_UTIL_PREFIX);
		} else if (ofi_has_offload_prefix(prov->prov_name)) {
			short_prov_name = prov->prov_name + strlen(OFI_OFFLOAD_PREFIX);
		} else {
			short_prov_name = prov->prov_name;
		}

		if (asprintf(&lib, "%s%s%s%s", lib_prefix,
			short_prov_name, "-", FI_LIB_SUFFIX) < 0) {
			FI_WARN(&core_prov, FI_LOG_CORE,
				"asprintf failed to allocate memory\n");
			continue;
		}

		ofi_reg_dl_prov(lib, false);
		free(lib);
	}
}

static void ofi_load_preferred_dl_prov(const char *path)
{
	if (!path || !strlen(path))
		return;

	if (path[0] != '/') {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"invalid format for preferred provider: \"%s\"\n",
			path);
		return;
	}

	if (access(path, F_OK) != 0) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"preferred provider not found: \"%s\"\n",
			path);
		return;
	}

	FI_INFO(&core_prov, FI_LOG_CORE,
		"loading preferred provider: \"%s\"\n", path);

	prov_preferred = true;
	ofi_reg_dl_prov(path, true);
	prov_preferred = false;
}

static void ofi_load_dl_prov(void)
{
	char **dirs;
	char *provdir = NULL;
	void *dlhandle;
	int i;

	/* If dlopen fails, assume static linking and return */
	dlhandle = dlopen(NULL, RTLD_NOW);
	if (!dlhandle)
		return;
	dlclose(dlhandle);

	fi_param_define(NULL, "provider_path", FI_PARAM_STRING,
			"Search for providers in specific path.  Path is "
			"specified similar to dir1:dir2:dir3.  If the path "
			"starts with @, loaded providers are given preference "
			"based on discovery order, rather than version. "
			"Optionally any of the dir can be replaced with + "
			"followed by the full path to a provider library, "
			"which specifies a preferred provider.  If registered "
			"successfully, a preferred provider has priority over "
			"other providers with the same name. "
			"(default: " PROVDLDIR ")");

	fi_param_get_str(NULL, "provider_path", &provdir);

#if HAVE_RESTRICTED_DL
	if (!provdir || !strlen(provdir)) {
		FI_INFO(&core_prov, FI_LOG_CORE,
			"restricted_dl: setting FI_PROVIDER_PATH to \"%s\"\n", PROVDLDIR);
		provdir = PROVDLDIR;
	}
#endif

	if (!provdir || !strlen(provdir)) {
		ofi_find_prov_libs();
		dirs = ofi_split_and_alloc(PROVDLDIR, ":", NULL);
	} else if (provdir[0] == '@') {
		prov_order = OFI_PROV_ORDER_REGISTER;
		if (strlen(provdir) == 1)
			dirs = ofi_split_and_alloc(PROVDLDIR, ":", NULL);
		else
			dirs = ofi_split_and_alloc(&provdir[1], ":", NULL);
	} else {
		dirs = ofi_split_and_alloc(provdir, ":", NULL);
	}

	if (dirs) {
		int num_dirs = 0;

		for (i = 0; dirs[i]; i++) {
			if (dirs[i][0] == '+') {
				ofi_load_preferred_dl_prov(dirs[i]+1);
			} else {
				ofi_ini_dir(dirs[i]);
				num_dirs++;
			}
		}

		ofi_free_string_array(dirs);

		if (num_dirs)
			return;

		/*
		 * When FI_PROVIDER_PATH contains only preferred providers, go
		 * back to search under the default path.
		 */
		dirs = ofi_split_and_alloc(PROVDLDIR, ":", NULL);
		if (dirs) {
			for (i = 0; dirs[i]; i++)
				ofi_ini_dir(dirs[i]);
			ofi_free_string_array(dirs);
		}
	}
}

#else /* HAVE_LIBDL */

static void ofi_load_dl_prov(void)
{
}

#endif

static char **hooks;
static size_t hook_cnt;

/*
 * Call the fabric() interface of the hooking provider.  We pass in the
 * fabric being hooked via the fabric attributes and the corresponding
 * fi_provider structure as the context.
 */
static void ofi_hook_install(struct fid_fabric *hfabric,
			     struct fid_fabric **fabric,
			     struct fi_provider *prov)
{
	struct fi_provider *hook_prov;
	struct fi_fabric_attr attr;
	int i, ret;

	*fabric = hfabric;
	if (!hook_cnt || !hooks)
		return;

	memset(&attr, 0, sizeof attr);

	for (i = 0; i < hook_cnt; i++) {
		hook_prov = ofi_get_hook(hooks[i]);
		if (!hook_prov)
			continue;

		attr.fabric = hfabric;
		ret = hook_prov->fabric(&attr, fabric, prov);
		if (ret)
			continue;

		hfabric = *fabric;
	}
}

static void ofi_hook_init(void)
{
	char *param_val = NULL;

	fi_param_define(NULL, "hook", FI_PARAM_STRING,
			"Intercept calls to underlying provider and apply "
			"the specified functionality to them.  Hook option: "
			"perf (gather performance data)");
	fi_param_get_str(NULL, "hook", &param_val);

	if (!param_val)
		return;

	hooks = ofi_split_and_alloc(param_val, ";", &hook_cnt);
}

static void ofi_hook_fini(void)
{
	if (hooks)
		ofi_free_string_array(hooks);
}

void fi_ini(void)
{
	char *param_val = NULL;

	pthread_mutex_lock(&common_locks.ini_lock);

	if (ofi_init)
		goto unlock;

	ofi_ordered_provs_init();
	fi_param_init();
	fi_log_init();
	ofi_dump_sysconfig();
	ofi_osd_init();
	ofi_mem_init();
	ofi_pmem_init();
	ofi_perf_init();
	ofi_hook_init();
	ofi_hmem_init();
	ofi_monitors_init();
	ofi_shm_p2p_init();

	fi_param_define(NULL, "provider", FI_PARAM_STRING,
			"Only use specified provider (default: all available)");
	fi_param_get_str(NULL, "provider", &param_val);
	ofi_create_filter(&prov_filter, param_val);

	fi_param_define(NULL, "fork_unsafe", FI_PARAM_BOOL,
			"Whether use of fork() may be unsafe for some providers "
			"(default: no). Setting this to yes could improve "
			"performance at the expense of making fork() potentially "
			"unsafe");
	fi_param_define(NULL, "universe_size", FI_PARAM_SIZE_T,
			"Defines the maximum number of processes that will be "
			"used by distribute OFI application. The provider uses "
			"this to optimize resource allocations "
			"(default: provider specific)");
	fi_param_get_size_t(NULL, "universe_size", &ofi_universe_size);

	fi_param_define(NULL, "av_remove_cleanup", FI_PARAM_BOOL,
			"When true, release any underlying resources, such as "
			"hidden connections when removing an entry from an "
			"AV.  This can help save resources on AV entries "
			"that reference a peer which is no longer active.  "
			"However, it may abruptly terminate data transfers "
			"from peers that are active at the time their "
			"address is removed from the local AV.  "
			"(default: false)");
	fi_param_get_bool(NULL, "av_remove_cleanup", &ofi_av_remove_cleanup);

	fi_param_define(NULL, "offload_coll_provider", FI_PARAM_STRING,
			"The name of a colective offload provider (default: \
			empty - no provider)");
	fi_param_get_str(NULL, "offload_coll_provider",
			    &ofi_offload_coll_prov_name);

	ofi_load_dl_prov();

	ofi_register_provider(PSM3_INIT, NULL);
	ofi_register_provider(PSM2_INIT, NULL);
	ofi_register_provider(CXI_INIT, NULL);
	ofi_register_provider(USNIC_INIT, NULL);
	ofi_register_provider(SHM_INIT, NULL);
	ofi_register_provider(SM2_INIT, NULL);

	ofi_register_provider(RXM_INIT, NULL);
	ofi_register_provider(VERBS_INIT, NULL);
	ofi_register_provider(MRAIL_INIT, NULL);
	ofi_register_provider(RXD_INIT, NULL);
	ofi_register_provider(EFA_INIT, NULL);
	ofi_register_provider(OPX_INIT, NULL);
	ofi_register_provider(UCX_INIT, NULL);
	ofi_register_provider(UDP_INIT, NULL);
	ofi_register_provider(SOCKETS_INIT, NULL);
	ofi_register_provider(TCP_INIT, NULL);

	ofi_register_provider(HOOK_PERF_INIT, NULL);
	ofi_register_provider(HOOK_TRACE_INIT, NULL);
	ofi_register_provider(HOOK_PROFILE_INIT, NULL);
	ofi_register_provider(HOOK_DEBUG_INIT, NULL);
	ofi_register_provider(HOOK_HMEM_INIT, NULL);
	ofi_register_provider(HOOK_DMABUF_PEER_MEM_INIT, NULL);
	ofi_register_provider(HOOK_NOOP_INIT, NULL);

	ofi_register_provider(COLL_INIT, NULL);

	pthread_atfork(NULL, NULL, ofi_memhooks_atfork_handler);

	ofi_init = 1;

unlock:
	pthread_mutex_unlock(&common_locks.ini_lock);
}

FI_DESTRUCTOR(fi_fini(void))
{
	struct ofi_prov *prov;

	pthread_mutex_lock(&common_locks.ini_lock);

	if (!ofi_init)
		goto unlock;

	while (prov_head) {
		prov = prov_head;
		prov_head = prov->next;
		ofi_free_prov(prov);
	}

	ofi_free_filter(&prov_filter);
	ofi_monitors_cleanup();
	ofi_hmem_cleanup();
	ofi_shm_p2p_cleanup();
	ofi_hook_fini();
	ofi_mem_fini();
	fi_log_fini();
	fi_param_fini();
	ofi_osd_fini();

	ofi_init = 0;

unlock:
	pthread_mutex_unlock(&common_locks.ini_lock);
}

__attribute__((visibility ("default"),EXTERNALLY_VISIBLE))
void DEFAULT_SYMVER_PRE(fi_freeinfo)(struct fi_info *info)
{
	struct fi_info *next;

	for (; info; info = next) {
		next = info->next;

		free(info->src_addr);
		free(info->dest_addr);
		free(info->tx_attr);
		free(info->rx_attr);
		if (info->ep_attr) {
			free(info->ep_attr->auth_key);
			free(info->ep_attr);
		}
		if (info->domain_attr) {
			free(info->domain_attr->auth_key);
			free(info->domain_attr->name);
			free(info->domain_attr);
		}
		if (info->fabric_attr) {
			free(info->fabric_attr->name);
			free(info->fabric_attr->prov_name);
			free(info->fabric_attr);
		}
		if (info->nic &&
		    FI_CHECK_OP(info->nic->fid.ops, struct fi_ops, close)) {
			fi_close(&info->nic->fid);
		}
		free(info);
	}
}
CURRENT_SYMVER(fi_freeinfo_, fi_freeinfo);

static bool
ofi_info_match_prov(struct fi_info *info, struct ofi_info_match *match)
{
	assert(match && match->prov_name);
	assert(info && info->fabric_attr && info->ep_attr);
	return !strcasecmp(info->fabric_attr->prov_name, match->prov_name) &&
		(info->ep_attr->type == match->ep_type);
}

static bool
ofi_info_split(struct fi_info **info, struct fi_info **new_list,
	       struct fi_info **new_tail, struct ofi_info_match *match)
{
	struct fi_info *cur, *prev, *next;

	*new_list = NULL;
	*new_tail = NULL;

	prev = NULL;
	for (cur = *info; cur; cur = next) {
		next = cur->next;

		if (!ofi_info_match_prov(cur, match)) {
			prev = cur;
			continue;
		}

		if (prev)
			prev->next = next;
		else
			*info = next;

		if (*new_list)
			(*new_tail)->next = cur;
		else
			*new_list = cur;

		*new_tail = cur;
		(*new_tail)->next = NULL;
	}

	return *new_list != NULL;
}

static void
ofi_info_insert(struct fi_info **info, struct fi_info *head,
		struct fi_info *tail, struct ofi_info_match *match)
{
	struct fi_info *cur, *prev;

	for (prev = NULL, cur = *info; cur; prev = cur, cur = cur->next) {
		if (ofi_info_match_prov(cur, match))
			break;
	}

	if (prev) {
		prev->next = head;
		tail->next = cur;
	} else {
		tail->next = *info;
		*info = head;
	}
}

static void
ofi_reorder_info(struct fi_info **info)
{
	struct fi_info *head, *tail;
	struct ofi_info_match match;

	/*
	 * Prioritize net over net;rxm for rdm ep's
	 */
	match.ep_type = FI_EP_RDM;
	match.prov_name = "tcp";
	if (!ofi_info_split(info, &head, &tail, &match))
		return;

	match.prov_name = "tcp;ofi_rxm";
	ofi_info_insert(info, head, tail, &match);
}

/*
 * Make a dummy info object for each provider, and copy in the
 * provider name and version.  We report utility providers directly
 * to export their version.
 */
static int ofi_getprovinfo(struct fi_info **info)
{
	struct ofi_prov *prov;
	struct fi_info *tail, *cur;
	int ret = -FI_ENODATA;

	*info = tail = NULL;
	for (prov = prov_head; prov; prov = prov->next) {
		if (!prov->provider)
			continue;

		cur = fi_allocinfo();
		if (!cur) {
			ret = -FI_ENOMEM;
			goto err;
		}

		cur->fabric_attr->prov_name = strdup(prov->provider->name);
		cur->fabric_attr->prov_version = prov->provider->version;

		if (!*info) {
			*info = tail = cur;
		} else {
			tail->next = cur;
		}
		tail = cur;

		ret = 0;
	}

	return ret;

err:
	while (tail) {
		cur = tail->next;
		fi_freeinfo(tail);
		tail = cur;
	}
	return ret;
}

static void ofi_set_prov_attr(struct fi_fabric_attr *attr,
			      struct fi_provider *prov)
{
	char *core_name;

	core_name = attr->prov_name;
	if (core_name) {
		assert(ofi_is_util_prov(prov));
		attr->prov_name = ofi_strdup_append(core_name, prov->name);
		free(core_name);
	} else {
		attr->prov_name = strdup(prov->name);
	}
	attr->prov_version = prov->version;
}

/*
 * The layering of utility providers over core providers follows these rules.
 * 0. Provider names are delimited by ";"
 * 1. Rules when # of providers <= 2:
 *    1a. If both are specified, then only return that layering
 *    1b. If a utility provider is specified, return it over any* core provider.
 *    1c. If a core provider is specified, return any utility provider that can
 *        layer over it, plus the core provider itself, if possible.
 *    1d. A utility provider will not layer over a provider that has disabled
 *        utility provider layering unless the user explicitly requests that
 *        combination.
 *    1e. OFI_CORE_PROV_ONLY flag prevents utility providers layering over other
 *        utility providers.
 * 2. If both the providers are utility providers or if more than two providers
 *    are specified, the rightmost provider would be compared.
 * 3. If any provider has a caret symbol "^" is prefixed before any provider
 *    name it would be excluded (internal use only). These excluded providers
 *    should be listed only at the end.
 */
static int ofi_layering_ok(const struct fi_provider *provider,
			   char **prov_vec, size_t count,
			   uint64_t flags)
{
	char *prov_name;
	struct ofi_prov *core_ofi_prov;
	ssize_t i;

	/* Excluded providers must be at the end */
	for (i = count - 1; i >= 0; i--) {
		if (prov_vec[i][0] != '^')
		    break;

		if (!strcasecmp(&prov_vec[i][1], provider->name)) {
			FI_INFO(&core_prov, FI_LOG_CORE,
				"Provider %s is excluded\n",
				provider->name);
			return 0;
		}
	}
	count = i + 1;

	if (flags & OFI_CORE_PROV_ONLY) {
		assert((count == 1) || (count == 0));
		if (!ofi_is_core_prov(provider)) {
			FI_INFO(&core_prov, FI_LOG_CORE,
				"Need core provider, skipping %s\n",
				provider->name);
			return 0;
		}

		if ((count == 0) && ofi_prov_ctx(provider)->disable_layering) {
			FI_INFO(&core_prov, FI_LOG_CORE,
				"Skipping util;%s layering\n", provider->name);
			return 0;
		}
	}

	if (!count)
		return 1;

	/* To maintain backward compatibility with the previous behavior of
	 * ofi_layering_ok we need to check if the # of providers is two or
	 * fewer. In such a case, we have to be agnostic to the ordering of
	 * core and utility providers */

	if ((count == 1) && ofi_is_util_prov(provider) &&
	    !ofi_has_util_prefix(prov_vec[0])) {
		core_ofi_prov = ofi_getprov(prov_vec[0], strlen(prov_vec[0]));
		if (core_ofi_prov && core_ofi_prov->provider &&
		    ofi_prov_ctx(core_ofi_prov->provider)->disable_layering) {
			FI_INFO(&core_prov, FI_LOG_CORE,
				"Skipping %s;%s layering\n", prov_vec[0],
				provider->name);
			return 0;
		}
		return 1;
	}

	if ((count == 2) && ofi_has_util_prefix(prov_vec[0]) &&
	    !ofi_has_util_prefix(prov_vec[1]))
		prov_name = prov_vec[0];
	else
		prov_name = prov_vec[count - 1];

	return !strcasecmp(provider->name, prov_name);
}

__attribute__((visibility ("default"),EXTERNALLY_VISIBLE))
int DEFAULT_SYMVER_PRE(fi_getinfo)(uint32_t version, const char *node,
		const char *service, uint64_t flags,
		const struct fi_info *hints, struct fi_info **info)
{
	struct ofi_prov *prov;
	struct fi_info *tail, *cur;
	char **prov_vec = NULL;
	size_t count = 0;
	enum fi_log_level level;
	int ret;

	fi_ini();

	if (FI_VERSION_LT(fi_version(), version)) {
		FI_WARN(&core_prov, FI_LOG_CORE,
			"Requested version is newer than library\n");
		return -FI_ENOSYS;
	}

	if (flags == FI_PROV_ATTR_ONLY) {
		return ofi_getprovinfo(info);
	}

	if (hints && hints->fabric_attr && hints->fabric_attr->prov_name) {
		prov_vec = ofi_split_and_alloc(hints->fabric_attr->prov_name,
					       ";", &count);
		if (!prov_vec)
			return -FI_ENOMEM;
		FI_DBG(&core_prov, FI_LOG_CORE, "hints prov_name: %s\n",
		       hints->fabric_attr->prov_name);
	}

	*info = tail = NULL;
	for (prov = prov_head; prov; prov = prov->next) {
		if (!prov->provider || !prov->provider->getinfo)
			continue;

		if (prov->hidden && !(flags & OFI_GETINFO_HIDDEN))
			continue;

		if ((ofi_prov_ctx(prov->provider)->type == OFI_PROV_OFFLOAD) &&
		    !(flags & OFI_OFFLOAD_PROV_ONLY))
			continue;

		if (!ofi_layering_ok(prov->provider, prov_vec, count, flags))
			continue;

		if (FI_VERSION_LT(prov->provider->fi_version, version)) {
			FI_WARN(&core_prov, FI_LOG_CORE,
				"Provider %s fi_version %d.%d < requested %d.%d\n",
				prov->provider->name,
				FI_MAJOR(prov->provider->fi_version),
				FI_MINOR(prov->provider->fi_version),
				FI_MAJOR(version), FI_MINOR(version));
			continue;
		}

		cur = NULL;
		ret = prov->provider->getinfo(version, node, service, flags,
					      hints, &cur);
		if (ret) {
			level = ((hints && hints->fabric_attr &&
				  hints->fabric_attr->prov_name &&
				  !strcmp(hints->fabric_attr->prov_name, prov->provider->name)) ?
				 FI_LOG_WARN : FI_LOG_INFO);

			FI_LOG(&core_prov, level, FI_LOG_CORE,
			       "fi_getinfo: provider %s returned -%d (%s)\n",
			       prov->provider->name, -ret, fi_strerror(-ret));
			continue;
		}

		if (!cur) {
			FI_WARN(&core_prov, FI_LOG_CORE,
				"fi_getinfo: provider %s output empty list\n",
				prov->provider->name);
			continue;
		}

		FI_DBG(&core_prov, FI_LOG_CORE, "fi_getinfo: provider %s "
		       "returned success\n", prov->provider->name);

		if (!*info)
			*info = cur;
		else
			tail->next = cur;

		for (tail = cur; tail->next; tail = tail->next) {
			ofi_set_prov_attr(tail->fabric_attr, prov->provider);
			tail->fabric_attr->api_version = version;
		}
		ofi_set_prov_attr(tail->fabric_attr, prov->provider);
		tail->fabric_attr->api_version = version;
	}
	ofi_free_string_array(prov_vec);

	if (*info && !(flags & (OFI_CORE_PROV_ONLY | OFI_GETINFO_INTERNAL |
				OFI_GETINFO_HIDDEN))) {
		ofi_filter_info(hints, info);
		ofi_reorder_info(info);
	}

	return *info ? 0 : -FI_ENODATA;
}
CURRENT_SYMVER(fi_getinfo_, fi_getinfo);

struct fi_info *ofi_allocinfo_internal(void)
{
	struct fi_info *info;

	info = calloc(1, sizeof(*info));
	if (!info)
		return NULL;

	info->tx_attr = calloc(1, sizeof(*info->tx_attr));
	info->rx_attr = calloc(1, sizeof(*info->rx_attr));
	info->ep_attr = calloc(1, sizeof(*info->ep_attr));
	info->domain_attr = calloc(1, sizeof(*info->domain_attr));
	info->fabric_attr = calloc(1, sizeof(*info->fabric_attr));
	if (!info->tx_attr|| !info->rx_attr || !info->ep_attr ||
	    !info->domain_attr || !info->fabric_attr)
		goto err;

	return info;
err:
	fi_freeinfo(info);
	return NULL;
}


__attribute__((visibility ("default"),EXTERNALLY_VISIBLE))
struct fi_info *DEFAULT_SYMVER_PRE(fi_dupinfo)(const struct fi_info *info)
{
	struct fi_info *dup;
	int ret;

	if (!info)
		return ofi_allocinfo_internal();

	dup = mem_dup(info, sizeof(*dup));
	if (dup == NULL) {
		return NULL;
	}
	dup->src_addr = NULL;
	dup->dest_addr = NULL;
	dup->tx_attr = NULL;
	dup->rx_attr = NULL;
	dup->ep_attr = NULL;
	dup->domain_attr = NULL;
	dup->fabric_attr = NULL;
	dup->next = NULL;

	if (info->src_addr != NULL) {
		dup->src_addr = mem_dup(info->src_addr, info->src_addrlen);
		if (dup->src_addr == NULL)
			goto fail;
	}
	if (info->dest_addr != NULL) {
		dup->dest_addr = mem_dup(info->dest_addr, info->dest_addrlen);
		if (dup->dest_addr == NULL)
			goto fail;
	}
	if (info->tx_attr != NULL) {
		dup->tx_attr = mem_dup(info->tx_attr, sizeof(*info->tx_attr));
		if (dup->tx_attr == NULL)
			goto fail;
	}
	if (info->rx_attr != NULL) {
		dup->rx_attr = mem_dup(info->rx_attr, sizeof(*info->rx_attr));
		if (dup->rx_attr == NULL)
			goto fail;
	}
	if (info->ep_attr != NULL) {
		dup->ep_attr = mem_dup(info->ep_attr, sizeof(*info->ep_attr));
		if (dup->ep_attr == NULL)
			goto fail;
		if (info->ep_attr->auth_key != NULL) {
			dup->ep_attr->auth_key =
				mem_dup(info->ep_attr->auth_key,
					info->ep_attr->auth_key_size);
			if (dup->ep_attr->auth_key == NULL)
				goto fail;
		}
	}
	if (info->domain_attr) {
		dup->domain_attr = mem_dup(info->domain_attr,
					   sizeof(*info->domain_attr));
		if (dup->domain_attr == NULL)
			goto fail;
		dup->domain_attr->name = NULL;
		dup->domain_attr->auth_key = NULL;
		if (info->domain_attr->name != NULL) {
			dup->domain_attr->name = strdup(info->domain_attr->name);
			if (dup->domain_attr->name == NULL)
				goto fail;
		}
		if (info->domain_attr->auth_key != NULL) {
			dup->domain_attr->auth_key =
				mem_dup(info->domain_attr->auth_key,
					info->domain_attr->auth_key_size);
			if (dup->domain_attr->auth_key == NULL)
				goto fail;
		}
	}
	if (info->fabric_attr) {
		dup->fabric_attr = mem_dup(info->fabric_attr,
					   sizeof(*info->fabric_attr));
		if (dup->fabric_attr == NULL)
			goto fail;
		dup->fabric_attr->name = NULL;
		dup->fabric_attr->prov_name = NULL;
		if (info->fabric_attr->name != NULL) {
			dup->fabric_attr->name = strdup(info->fabric_attr->name);
			if (dup->fabric_attr->name == NULL)
				goto fail;
		}
		if (info->fabric_attr->prov_name != NULL) {
			dup->fabric_attr->prov_name = strdup(info->fabric_attr->prov_name);
			if (dup->fabric_attr->prov_name == NULL)
				goto fail;
		}
	}

	if (info->nic) {
		ret = fi_control(&info->nic->fid, FI_DUP, &dup->nic);
		if (ret && ret != -FI_ENOSYS)
			goto fail;
	}

	return dup;

fail:
	fi_freeinfo(dup);
	return NULL;
}
CURRENT_SYMVER(fi_dupinfo_, fi_dupinfo);

__attribute__((visibility ("default"),EXTERNALLY_VISIBLE))
int DEFAULT_SYMVER_PRE(fi_fabric)(struct fi_fabric_attr *attr,
		struct fid_fabric **fabric, void *context)
{
	struct ofi_prov *prov;
	const char *top_name;
#ifdef HAVE_LIBDL
	Dl_info dl_info;
#endif
	int ret;

	if (!attr || !attr->prov_name || !attr->name)
		return -FI_EINVAL;

	fi_ini();

	top_name = strrchr(attr->prov_name, OFI_NAME_DELIM);
	if (top_name)
		top_name++;
	else
		top_name = attr->prov_name;

	if (!top_name)
		return -FI_EINVAL;

	prov = ofi_getprov(top_name, strlen(top_name));
	if (!prov || !prov->provider || !prov->provider->fabric)
		return -FI_ENODEV;

	ret = prov->provider->fabric(attr, fabric, context);
	if (!ret) {
		if (FI_VERSION_GE(prov->provider->fi_version, FI_VERSION(1, 5)))
			(*fabric)->api_version = attr->api_version;
		FI_INFO(&core_prov, FI_LOG_CORE, "Opened fabric: %s\n",
			attr->name);

		ofi_hook_install(*fabric, fabric, prov->provider);

#ifdef HAVE_LIBDL
		if (dladdr(prov->provider->fabric, &dl_info))
			FI_INFO(&core_prov, FI_LOG_CORE,
				"Using %s provider %u.%u, path:%s\n",
				prov->prov_name,
				FI_MAJOR(prov->provider->fi_version),
				FI_MINOR(prov->provider->fi_version),
				dl_info.dli_fname);
#endif
	}

	return ret;
}
DEFAULT_SYMVER(fi_fabric_, fi_fabric, FABRIC_1.1);

__attribute__((visibility ("default"),EXTERNALLY_VISIBLE))
uint32_t DEFAULT_SYMVER_PRE(fi_version)(void)
{
	return FI_VERSION(FI_MAJOR_VERSION, FI_MINOR_VERSION);
}
DEFAULT_SYMVER(fi_version_, fi_version, FABRIC_1.0);

__attribute__((visibility ("default"),EXTERNALLY_VISIBLE))
int DEFAULT_SYMVER_PRE(fi_open)(uint32_t version, const char *name,
		void *attr, size_t attr_len, uint64_t flags,
		struct fid **fid, void *context)
{
	if (!strcasecmp("mr_cache", name))
		return ofi_open_mr_cache(version, attr, attr_len,
					 flags, fid, context);
	if (!strcasecmp("logging", name))
		return ofi_open_log(version, attr, attr_len,
				    flags, fid, context);

	return -FI_ENOSYS;
}
DEFAULT_SYMVER(fi_open_, fi_open, FABRIC_1.5);

static const char *const errstr[] = {
	[FI_EOTHER - FI_ERRNO_OFFSET] = "Unspecified error",
	[FI_ETOOSMALL - FI_ERRNO_OFFSET] = "Provided buffer is too small",
	[FI_EOPBADSTATE - FI_ERRNO_OFFSET] = "Operation not permitted in current state",
	[FI_EAVAIL - FI_ERRNO_OFFSET]  = "Error available",
	[FI_EBADFLAGS - FI_ERRNO_OFFSET] = "Flags not supported",
	[FI_ENOEQ - FI_ERRNO_OFFSET] = "Missing or unavailable event queue",
	[FI_EDOMAIN - FI_ERRNO_OFFSET] = "Invalid resource domain",
	[FI_ENOCQ - FI_ERRNO_OFFSET] = "Missing or unavailable completion queue",
	[FI_ECRC - FI_ERRNO_OFFSET] = "CRC error",
	[FI_ETRUNC - FI_ERRNO_OFFSET] = "Truncation error",
	[FI_ENOKEY - FI_ERRNO_OFFSET] = "Required key not available",
	[FI_ENOAV - FI_ERRNO_OFFSET] = "Missing or unavailable address vector",
	[FI_EOVERRUN - FI_ERRNO_OFFSET] = "Queue has been overrun",
	[FI_ENORX - FI_ERRNO_OFFSET] = "Receiver not ready, no receive buffers available",
	[FI_ENOMR - FI_ERRNO_OFFSET] = "Memory registration limit exceeded",
};

__attribute__((visibility ("default"),EXTERNALLY_VISIBLE))
const char *DEFAULT_SYMVER_PRE(fi_strerror)(int errnum)
{
	if (errnum < 0)
		errnum = -errnum;

	if (errnum < FI_ERRNO_OFFSET)
		return strerror(errnum);
	else if (errnum < FI_ERRNO_MAX)
		return errstr[errnum - FI_ERRNO_OFFSET];
	else
		return errstr[FI_EOTHER - FI_ERRNO_OFFSET];
}
DEFAULT_SYMVER(fi_strerror_, fi_strerror, FABRIC_1.0);
