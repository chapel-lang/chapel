/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */

#include <ofi_prov.h>
#include <ofi_lock.h>
#include "efa.h"
#include "efa_prov.h"
#include "efa_prov_info.h"
#include "efa_env.h"

#ifndef _WIN32

void efa_win_lib_finalize(void)
{
	// Nothing to do when we are not compiling for Windows
}

int efa_win_lib_initialize(void)
{
	return 0;
}

#else // _WIN32

#include "efawin.h"

static bool efa_win_initialized = false;
/**
 * @brief open efawin.dll and load the symbols on windows platform
 *
 * This function is a no-op when not on windows
 */
int efa_win_lib_initialize(void)
{
	/* On Windows we need to load efawin dll to interact with
 	* efa device as there is no built-in verbs integration in the OS.
	* efawin dll provides all the ibv_* functions on Windows.
	* efa_load_efawin_lib function will replace stub ibv_* functions with
	* functions from efawin dll
	*/
	int err = efa_load_efawin_lib();
	if (err)
		EFA_WARN(FI_LOG_CORE, "Failed to load efawin dll. error: %d\n",
			 err);
	else
		efa_win_initialized = true;
	return err;
}

/**
 * @brief close efawin.dll on windows
 *
 * This function is a no-op on windows
 */
void efa_win_lib_finalize(void) {
	if (efa_win_initialized)
		efa_free_efawin_lib();
	efa_win_initialized = false;
}

#endif // _WIN32

static void efa_prov_finalize();

struct fi_provider efa_prov = {
	.name = "efa",
	.version = OFI_VERSION_DEF_PROV,
	.fi_version = OFI_VERSION_LATEST,
	.getinfo = efa_getinfo,
	.fabric = efa_fabric,
	.cleanup = efa_prov_finalize
};

struct util_prov efa_util_prov = {
	.prov = &efa_prov,
};

ofi_mutex_t g_efa_domain_list_lock;

/**
 * @brief initialize global variable: efa_util_prov
 *
 * efa_util_prov is the util_provider with its
 * info pointing a linked list of info object
 */
static int efa_util_prov_initialize()
{
	int i, err;
	struct fi_info *head, *tail, *prov_info_rdm, *prov_info_dgram, *prov_info_direct;

	head = NULL;
	tail = NULL;

	/*
	* EFA direct provider is more performant if the application can use it
	* Therefore, the efa-direct info objects should be returned _before_ efa rdm or dgram
	* So we populate the efa-direct info objects first
	*/
	for (i = 0; i < g_efa_selected_device_cnt; ++i) {
		prov_info_direct = fi_dupinfo(g_efa_selected_device_list[i].rdm_info);
		if (!prov_info_direct) {
			EFA_WARN(FI_LOG_DOMAIN, "Failed to allocate prov_info for EFA direct\n");
			continue;
		}

		err = efa_prov_info_set_fabric_name(prov_info_direct, EFA_DIRECT_FABRIC_NAME);
		if (err) {
			EFA_WARN(FI_LOG_DOMAIN, "Failed to allocate fabric name. error: %d\n", err);
			continue;
		}

		/* This function must be called after efa_hmem_info_initialize() */
		efa_prov_info_direct_set_hmem_flags(prov_info_direct);

		if (!head) {
			head = prov_info_direct;
		} else {
			assert(tail);
			tail->next = prov_info_direct;
		}

		tail = prov_info_direct;
	}

	for (i = 0; i < g_efa_selected_device_cnt; ++i) {
		err = efa_prov_info_alloc_for_rdm(&prov_info_rdm, &g_efa_selected_device_list[i]);
		if (err) {
			EFA_WARN(FI_LOG_DOMAIN, "Failed to allocate prov_info for rdm. error: %d\n",
				 err);
			continue;
		}

		err = efa_prov_info_set_fabric_name(prov_info_rdm, EFA_FABRIC_NAME);
		if (err) {
			EFA_WARN(FI_LOG_DOMAIN, "Failed to allocate fabric name. error: %d\n", err);
			continue;
		}

		if (!head) {
			head = prov_info_rdm;
		} else {
			assert(tail);
			tail->next = prov_info_rdm;
		}

		tail = prov_info_rdm;
	}

	for (i = 0; i < g_efa_selected_device_cnt; ++i) {
		prov_info_dgram = fi_dupinfo(g_efa_selected_device_list[i].dgram_info);
		if (!prov_info_dgram) {
			EFA_WARN(FI_LOG_DOMAIN, "Failed to allocate prov_info for dgram\n");
			continue;
		}

		err = efa_prov_info_set_fabric_name(prov_info_dgram, EFA_FABRIC_NAME);
		if (err) {
			EFA_WARN(FI_LOG_DOMAIN, "Failed to allocate fabric name. error: %d\n", err);
			continue;
		}

		if (!head) {
			head = prov_info_dgram;
		} else {
			assert(tail);
			tail->next = prov_info_dgram;
		}

		tail = prov_info_dgram;
	}

	if (!head)
		return -FI_ENODATA;

	efa_util_prov.info = head;
	return 0;
}

static void efa_util_prov_finalize()
{
	struct fi_info *prov_info;

	prov_info = (struct fi_info *)efa_util_prov.info;
	if (prov_info)
		fi_freeinfo(prov_info);
	efa_util_prov.info = NULL;
}

EFA_INI
{
#if HAVE_EFA_DL
	ofi_mem_init();
	ofi_hmem_init();
	ofi_monitors_init();
	ofi_params_init();
#endif
	int err;

	err = efa_win_lib_initialize();
	if (err)
		goto err_free;

	efa_env_initialize();

	/*
	 * efa_device_list_initialize uses FI_EFA_IFACE, so
	 * efa_device_list_initialize must be called after efa_env_initialize
	 */
	err = efa_device_list_initialize();
	if (err)
		return &efa_prov;


	/*
	 * efa_fork_support_enable_if_requested must be called before
	 * efa_hmem_info_initialize.
	 *
	 * efa_hmem_info_initialize calls ibv_reg_mr to test for p2p support.
	 *
	 * On older kernels (without support for IBV_FORK_UNNEEDED),
	 * efa_fork_support_enable_if_requested calls ibv_fork_init,
	 * when fork support is requested.
	 *
	 * If ibv_fork_init is called, it must be called before any
	 * ibv_reg_mr calls. Otherwise, ibv_fork_init will return EINVAL.
	 */
	err = efa_fork_support_enable_if_requested();
	if (err)
		goto err_free;

	err = efa_hmem_info_initialize();
	if (err)
		goto err_free;

	/*
	* efa_util_prov_initialize uses g_efa_hmem_info, so it
	* must be called after efa_hmem_info_initialize
	*/
	err = efa_util_prov_initialize();
	if (err)
		goto err_free;

	ofi_mutex_init(&g_efa_domain_list_lock);
	dlist_init(&g_efa_domain_list);

	return &efa_prov;

err_free:
	efa_prov_finalize();
	return &efa_prov;
}

/**
 * @brief release the resources of global variables of provider
 *
 * This function calls various functions to release
 * util_prov, device_list, pd_list, win_lib
 */
static void efa_prov_finalize(void)
{
	efa_util_prov_finalize();

	efa_device_list_finalize();

	efa_win_lib_finalize();

#if HAVE_EFA_DL
	ofi_monitors_cleanup();
	ofi_hmem_cleanup();
	ofi_mem_fini();
#endif
}
