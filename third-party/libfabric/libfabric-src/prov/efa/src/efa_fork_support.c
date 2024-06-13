/* SPDX-License-Identifier: BSD-2-Clause OR GPL-2.0-only */
/* SPDX-FileCopyrightText: Copyright Amazon.com, Inc. or its affiliates. All rights reserved. */

#include "efa.h"

enum efa_fork_support_status g_efa_fork_status = EFA_FORK_SUPPORT_OFF;

/**
 * @brief initialize g_efa_fork_status based on environment and system statsu
 *
 */
void efa_fork_support_request_initialize()
{
	g_efa_fork_status = EFA_FORK_SUPPORT_OFF;

#if HAVE_IBV_IS_FORK_INITIALIZED == 1
	if (ibv_is_fork_initialized() == IBV_FORK_UNNEEDED) {
		g_efa_fork_status = EFA_FORK_SUPPORT_UNNEEDED;
		return;
	}
#endif
	int fork_support_requested = 0;

	fi_param_get_bool(&efa_prov, "fork_safe", &fork_support_requested);

	/*
	 * Check if any environment variables which would trigger
	 * libibverbs' fork support are set. These variables are
	 * defined by ibv_fork_init(3).
	 */
	if (fork_support_requested || getenv("RDMAV_FORK_SAFE") || getenv("IBV_FORK_SAFE")) {
		g_efa_fork_status = EFA_FORK_SUPPORT_ON;
		/* libibverbs's fork support does not work for huge page.
		 * libibverbs does have huge page fork support, which can be activated by
		 * setting environment RDMAV_HUGEPAGE_SAFE to 1. However, huge page fork
		 * support is extremely expensive, enabling it would defeat the purpose
		 * of using huge page. Therefore, we simply disable huge page usage
		 * when libfabric's fork support is enabled.
		 */
		if (efa_env.huge_page_setting == EFA_ENV_HUGE_PAGE_ENABLED) {
			fprintf(stderr,
				"The envrionment variable FI_EFA_USE_HUGE_PAGE has been\n"
				"set to 1/on/true, which indicate user want EFA provider\n"
				"to use huge page. However, rdma-core's fork support is\n"
				"enabled by environment variable\n"
				"     FI_EFA_FORK_SAFE/RDMAV_FORK_SAFE/IBV_FORK_SAFE.\n"
				"The usage of huge page is incompatible with rdma-core's "
				"fork support.\n"
				"Use them at the same time will cause memory corruption.\n"
				"If your application does use fork(), please unset the\n"
				"environment variable\n"
				"        FI_EFA_USE_HUGE_PAGE.\n"
				"Otherwise, please unset the environment variable(s):\n"
				"        FI_EFA_FORK_SAFE/RDMAV_FORK_SAFE/IBV_FORK_SAFE\n"
				"Your application will now abort!\n"
				);
			abort();
		}

		efa_env.huge_page_setting = EFA_ENV_HUGE_PAGE_DISABLED;
	}
}

/* @brief Check if rdma-core fork support is enabled and prevent fork
 * support from being enabled later.
 *
 * Register a temporary buffer and call ibv_fork_init() to determine
 * if fork support is enabled. Registering a buffer prevents future
 * calls to ibv_fork_init() from completing successfully.
 *
 * This relies on internal behavior in rdma-core and is a temporary workaround.
 *
 * @param domain_fid domain fid so we can register memory
 * @return 1 if fork support is enabled
 *         0 if not enabled
 *         -FI_EINVAL/-FI_NOMEM on errors.
 */
static int efa_fork_support_is_enabled(struct fid_domain *domain_fid)
{
	/* If ibv_is_fork_initialized is availble, check if the function
	 * can exit early.
	 */
#if HAVE_IBV_IS_FORK_INITIALIZED == 1
	enum ibv_fork_status fork_status = ibv_is_fork_initialized();

	/* If fork support is ENABLED or UNNEEDED, return 1. */
	return fork_status != IBV_FORK_DISABLED;
#else
	struct efa_domain *efa_domain;
	struct ibv_mr *mr = NULL;
	char *buf = NULL;
	int ret=0, ret_init=0;
	long page_size;

	efa_domain = container_of(domain_fid, struct efa_domain, util_domain.domain_fid);

	page_size = ofi_get_page_size();
	if (page_size <= 0) {
		EFA_WARN(FI_LOG_DOMAIN, "Unable to determine page size %ld\n",
			 page_size);
		return -FI_EINVAL;
	}

	buf = malloc(page_size);
	if (!buf)
		return -FI_ENOMEM;


	mr = ibv_reg_mr(efa_domain->ibv_pd, buf, page_size, 0);
	if (mr == NULL) {
		ret = errno;
		goto out;
	}

	/*
	 * libibverbs maintains a global variable to determine if any
	 * registrations have occurred before ibv_fork_init() is called.
	 * EINVAL is returned if a memory region was registered before
	 * ibv_fork_init() was called and returns 0 if fork support is
	 * initialized already.
	 */
	ret_init = ibv_fork_init();

out:
	if(buf) free(buf);
	if(mr) ibv_dereg_mr(mr);
	if (ret) {
		EFA_WARN(FI_LOG_DOMAIN,
			"Unexpected error during ibv_reg_mr in "
			"efa_fork_support_is_enabled(): %s\n",strerror(ret));
		return -FI_EINVAL;
	}
	if (ret_init == 0) return 0;
	if (ret_init == EINVAL) return 1;
	EFA_WARN(FI_LOG_DOMAIN,
		"Unexpected error during ibv_fork_init in "
		"efa_fork_support_is_enabled(): %s\n",strerror(ret_init));
	return -FI_EINVAL;
#endif /* HAVE_IBV_IS_FORK_INITIALIZED */
}

/* @brief Fork handler that is installed when EFA is loaded
 *
 * We register this fork handler so that users do not inadvertently trip over
 * memory corruption when fork is called. Calling fork() without enabling fork
 * support in rdma-core can cause corruption, even if the registered pages are
 * not used in the child process.
 *
 * It is critical that this fork handler is only installed once an EFA device
 * is present and selected. We don't want this to trigger when Libfabric is not
 * running on an EC2 instance.
 */
static
void efa_atfork_callback_warn_and_abort()
{
	static int visited = 0;

	if (visited)
		return;
	visited = 1;

	fprintf(stderr,
		"A process has executed an operation involving a call\n"
		"to the fork() system call to create a child process.\n"
		"\n"
		"As a result, the Libfabric EFA provider is operating in\n"
		"a condition that could result in memory corruption or\n"
		"other system errors.\n"
		"\n"
		"For the Libfabric EFA provider to work safely when fork()\n"
		"is called please do one of the following:\n"
		"1) Set the environment variable:\n"
		"          FI_EFA_FORK_SAFE=1\n"
		"and verify you are using rdma-core v31.1 or later.\n"
		"\n"
		"OR\n"
		"2) Use Linux Kernel 5.13+ with rdma-core v35.0+\n"
		"\n"
		"Please note that enabling fork support may cause a\n"
		"small performance impact.\n"
		"\n"
		"You may want to check with your application vendor to see\n"
		"if an application-level alternative (of not using fork)\n"
		"exists.\n"
		"\n"
		"Your job will now abort.\n");
	abort();
}

/**
 * @brief flush all MR caches
 *
 * Going through all domains, and flush the MR caches in them
 * until all inactive MRs are de-registered.
 * This makes all memory regions that are not actively used
 * in data transfer visible to the child process.
 */
void efa_atfork_callback_flush_mr_cache()
{
	struct dlist_entry *tmp;
	struct efa_domain *efa_domain;
	bool flush_lru = true;

	dlist_foreach_container_safe(&g_efa_domain_list,
				     struct efa_domain,
				     efa_domain, list_entry, tmp) {
		if (efa_domain->cache) {
			while(ofi_mr_cache_flush(efa_domain->cache, flush_lru));
		}
	}
}

#ifndef _WIN32

/* @brief Check when fork is requested and abort in unsupported cases
 *
 * We check if user or another library asked or enabled fork support and
 * return failure if HUGEPAGES are also enabled as EFA provider does not support this case.
 *
 * In addition, we install a fork handler to ensure that we abort if another
 * library or process initiates a fork and we determined from previous logic
 * that we cannot support that.
 *
 * @param domain_fid domain fid so we can check register memory during initialization.
 * @return error number if we failed to initialize, 0 otherwise
 */
int efa_fork_support_enable_if_requested(struct fid_domain* domain_fid)
{
	static int fork_handler_installed = 0;
	int ret;
	int is_enabled;

	/*
	 * Call ibv_fork_init if the user asked for fork support.
	 */
	if (g_efa_fork_status == EFA_FORK_SUPPORT_ON) {
		ret = -ibv_fork_init();
		if (ret) {
			EFA_WARN(FI_LOG_DOMAIN,
				 "Fork support requested but ibv_fork_init failed: %s\n",
				 strerror(-ret));
			return ret;
		}
	}

	/*
	 * Run check to see if fork support was enabled by another library. If
	 * one of the environment variables was set to enable fork support,
	 * this variable was set to ON during provider init.  Huge pages for
	 * bounce buffers will not be used if fork support is on.
	 */
	ret = efa_fork_support_is_enabled(domain_fid);
	if (ret < 0) return ret;
	is_enabled = ret;

	if (g_efa_fork_status == EFA_FORK_SUPPORT_OFF && is_enabled)
		g_efa_fork_status = EFA_FORK_SUPPORT_ON;

	if (g_efa_fork_status == EFA_FORK_SUPPORT_ON && getenv("RDMAV_HUGEPAGES_SAFE")) {
			EFA_WARN(FI_LOG_DOMAIN,
				 "Using libibverbs fork support and huge pages is not"
				 " supported by the EFA provider.\n");
		return -FI_EINVAL;
	}

	/*
	 * It'd be better to install this during provider init (since that's
	 * only invoked once) but we need to do a memory registration for the
	 * fork check above. This can move to the provider init once that check
	 * is gone.
	 */
	if (!fork_handler_installed && g_efa_fork_status != EFA_FORK_SUPPORT_UNNEEDED) {
		if (g_efa_fork_status == EFA_FORK_SUPPORT_OFF) {
			ret = pthread_atfork(efa_atfork_callback_warn_and_abort, NULL, NULL);
		} else {
			assert(g_efa_fork_status == EFA_FORK_SUPPORT_ON);
			ret = pthread_atfork(efa_atfork_callback_flush_mr_cache, NULL, NULL);
		}

		if (ret) {
			EFA_WARN(FI_LOG_DOMAIN,
				 "Unable to register atfork callback: %s\n",
				 strerror(-ret));
			return ret;
		}

		fork_handler_installed = 1;
	}

	return 0;
}

#else

/* @brief Check when fork is requested and return failure on Windows
 *
 * We check if fork is requested and return failure as fork is not supported on Windows
 *
 * @param domain_fid domain unused
 * @return error number if fork is requested, 0 otherwise
 */
int efa_fork_support_enable_if_requested(struct domain_fid* domain_fid)
{
	if (g_efa_fork_status == EFA_FORK_SUPPORT_ON) {
		EFA_WARN(FI_LOG_DOMAIN,
			 "Using fork support is not supported by the EFA provider on Windows\n");
		return -FI_EINVAL;
	}
	return 0;
}

#endif

