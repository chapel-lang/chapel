/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2009-2012  Université de Bordeaux 1
 * Copyright (C) 2010, 2011, 2012  Centre National de la Recherche Scientifique
 * Copyright (C) 2011  Télécom-SudParis
 *
 * StarPU is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or (at
 * your option) any later version.
 *
 * StarPU is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See the GNU Lesser General Public License in COPYING.LGPL for more details.
 */

#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <common/config.h>
#include <common/utils.h>
#include <core/perfmodel/perfmodel.h>
#include <core/jobs.h>
#include <core/workers.h>
#include <pthread.h>
#include <datawizard/datawizard.h>
#include <core/perfmodel/regression.h>
#include <common/config.h>
#include <starpu_parameters.h>

#ifdef STARPU_HAVE_WINDOWS
#include <windows.h>
#endif

/* We want more than 10% variance on X to trust regression */
#define VALID_REGRESSION(reg_model) \
	((reg_model)->minx < (9*(reg_model)->maxx)/10 && (reg_model)->nsample >= _STARPU_CALIBRATION_MINIMUM)

static pthread_rwlock_t registered_models_rwlock;
static struct starpu_model_list *registered_models = NULL;

/*
 * History based model
 */
static void insert_history_entry(struct starpu_history_entry *entry, struct starpu_history_list **list, struct starpu_htbl32_node **history_ptr)
{
	struct starpu_history_list *link;
	struct starpu_history_entry *old;

	link = (struct starpu_history_list *) malloc(sizeof(struct starpu_history_list));
	link->next = *list;
	link->entry = entry;
	*list = link;

	old = (struct starpu_history_entry *) _starpu_htbl_insert_32(history_ptr, entry->footprint, entry);
	/* that may fail in case there is some concurrency issue */
	STARPU_ASSERT(old == NULL);
}

static void dump_reg_model(FILE *f, struct starpu_perfmodel *model, unsigned arch, unsigned nimpl)
{
	struct starpu_per_arch_perfmodel *per_arch_model;

	per_arch_model = &model->per_arch[arch][nimpl];
	struct starpu_regression_model *reg_model;
	reg_model = &per_arch_model->regression;

	/*
	 * Linear Regression model
	 */

	/* Unless we have enough measurements, we put NaN in the file to indicate the model is invalid */
	double alpha = nan(""), beta = nan("");
	if (model->type == STARPU_REGRESSION_BASED || model->type == STARPU_NL_REGRESSION_BASED)
	{
		if (reg_model->nsample > 1)
		{
			alpha = reg_model->alpha;
			beta = reg_model->beta;
		}
	}

	fprintf(f, "# sumlnx\tsumlnx2\t\tsumlny\t\tsumlnxlny\talpha\t\tbeta\t\tn\tminx\t\tmaxx\n");
	fprintf(f, "%-15le\t%-15le\t%-15le\t%-15le\t%-15le\t%-15le\t%u\t%-15lu\t%-15lu\n", reg_model->sumlnx, reg_model->sumlnx2, reg_model->sumlny, reg_model->sumlnxlny, alpha, beta, reg_model->nsample, reg_model->minx, reg_model->maxx);

	/*
	 * Non-Linear Regression model
	 */

	double a = nan(""), b = nan(""), c = nan("");

	if (model->type == STARPU_NL_REGRESSION_BASED)
		_starpu_regression_non_linear_power(per_arch_model->list, &a, &b, &c);

	fprintf(f, "# a\t\tb\t\tc\n");
	fprintf(f, "%-15le\t%-15le\t%-15le\n", a, b, c);
}

static void scan_reg_model(FILE *f, struct starpu_regression_model *reg_model)
{
	int res;

	/*
	 * Linear Regression model
	 */

	_starpu_drop_comments(f);

	res = fscanf(f, "%le\t%le\t%le\t%le\t%le\t%le\t%u\t%lu\t%lu\n",
		&reg_model->sumlnx, &reg_model->sumlnx2, &reg_model->sumlny,
		&reg_model->sumlnxlny, &reg_model->alpha, &reg_model->beta,
		&reg_model->nsample,
		&reg_model->minx, &reg_model->maxx);
	STARPU_ASSERT(res == 9);

	/* If any of the parameters describing the linear regression model is NaN, the model is invalid */
	unsigned invalid = (isnan(reg_model->alpha)||isnan(reg_model->beta));
	reg_model->valid = !invalid && VALID_REGRESSION(reg_model);

	/*
	 * Non-Linear Regression model
	 */

	_starpu_drop_comments(f);

	res = fscanf(f, "%le\t%le\t%le\n", &reg_model->a, &reg_model->b, &reg_model->c);
	STARPU_ASSERT(res == 3);

	/* If any of the parameters describing the non-linear regression model is NaN, the model is invalid */
	unsigned nl_invalid = (isnan(reg_model->a)||isnan(reg_model->b)||isnan(reg_model->c));
	reg_model->nl_valid = !nl_invalid && VALID_REGRESSION(reg_model);
}

static void dump_history_entry(FILE *f, struct starpu_history_entry *entry)
{
	fprintf(f, "%08x\t%-15lu\t%-15le\t%-15le\t%-15le\t%-15le\t%u\n", entry->footprint, (unsigned long) entry->size, entry->mean, entry->deviation, entry->sum, entry->sum2, entry->nsample);
}

static void scan_history_entry(FILE *f, struct starpu_history_entry *entry)
{
	int res;

	_starpu_drop_comments(f);

	/* In case entry is NULL, we just drop these values */
	unsigned nsample;
	uint32_t footprint;
#ifdef STARPU_HAVE_WINDOWS
	unsigned size; /* in bytes */
#else
	size_t size; /* in bytes */
#endif
	double mean;
	double deviation;
	double sum;
	double sum2;

	/* Read the values from the file */
	res = fscanf(f, "%x\t%"
#ifndef STARPU_HAVE_WINDOWS
	"z"
#endif
	"u\t%le\t%le\t%le\t%le\t%u\n", &footprint, &size, &mean, &deviation, &sum, &sum2, &nsample);
	STARPU_ASSERT(res == 7);

	if (entry)
	{
		entry->footprint = footprint;
		entry->size = size;
		entry->mean = mean;
		entry->deviation = deviation;
		entry->sum = sum;
		entry->sum2 = sum2;
		entry->nsample = nsample;
	}
}

static void parse_per_arch_model_file(FILE *f, struct starpu_per_arch_perfmodel *per_arch_model, unsigned scan_history)
{
	unsigned nentries;

	_starpu_drop_comments(f);

	int res = fscanf(f, "%u\n", &nentries);
	STARPU_ASSERT(res == 1);

	scan_reg_model(f, &per_arch_model->regression);

	/* parse cpu entries */
	unsigned i;
	for (i = 0; i < nentries; i++)
	{
		struct starpu_history_entry *entry = NULL;
		if (scan_history)
		{
			entry = (struct starpu_history_entry *) malloc(sizeof(struct starpu_history_entry));
			STARPU_ASSERT(entry);
		}

		scan_history_entry(f, entry);

		/* insert the entry in the hashtable and the list structures  */
		if (scan_history)
			insert_history_entry(entry, &per_arch_model->list, &per_arch_model->history);
	}
}

static void parse_arch(FILE *f, struct starpu_perfmodel *model, unsigned scan_history, unsigned archmin, unsigned archmax, unsigned skiparch)
{
	struct starpu_per_arch_perfmodel dummy;
	int nimpls, implmax, skipimpl, impl;
	unsigned ret, arch;

	for (arch = archmin; arch < archmax; arch++)
	{
		_STARPU_DEBUG("Parsing arch %u\n", arch);
		_starpu_drop_comments(f);
		ret = fscanf(f, "%d\n", &nimpls);
		_STARPU_DEBUG("%u implementations\n", nimpls);
		STARPU_ASSERT(ret == 1);
		implmax = STARPU_MIN(nimpls, STARPU_MAXIMPLEMENTATIONS);
		skipimpl = nimpls - STARPU_MAXIMPLEMENTATIONS;
		for (impl = 0; impl < implmax; impl++)
		{
			parse_per_arch_model_file(f, &model->per_arch[arch][impl], scan_history);
		}
		if (skipimpl > 0)
		{
			for (impl = 0; impl < skipimpl; impl++)
			{
				parse_per_arch_model_file(f, &dummy, 0);
			}
		}
	}

	if (skiparch > 0)
	{
		_starpu_drop_comments(f);
		for (arch = 0; arch < skiparch; arch ++)
		{
			_STARPU_DEBUG("skipping arch %u\n", arch);
			ret = fscanf(f, "%d\n", &nimpls);
			_STARPU_DEBUG("%u implementations\n", nimpls);
			STARPU_ASSERT(ret == 1);
			implmax = STARPU_MIN(nimpls, STARPU_MAXIMPLEMENTATIONS);
			skipimpl = nimpls - STARPU_MAXIMPLEMENTATIONS;
			for (impl = 0; impl < implmax; impl++)
			{
				parse_per_arch_model_file(f, &dummy, 0);
			}
			if (skipimpl > 0)
			{
				for (impl = 0; impl < skipimpl; impl++)
				{
					parse_per_arch_model_file(f, &dummy, 0);
				}
			}
		}
	}
}

static void parse_model_file(FILE *f, struct starpu_perfmodel *model, unsigned scan_history)
{
	unsigned ret;
	unsigned archmin = 0;
	unsigned max_gordondevs = 1; /* XXX : we need a STARPU_MAXGORDONDEVS cst */
	unsigned narchs;

	/* We could probably write a clean loop here, but the code would not
	 * really be easier to read. */

	/* Parsing CPUs */
	_starpu_drop_comments(f);
	ret = fscanf(f, "%u\n", &narchs);
	STARPU_ASSERT(ret == 1);

	_STARPU_DEBUG("Parsing %u CPUs\n", narchs);
	if (narchs > 0)
	{
		parse_arch(f, model, scan_history,
			   archmin,
			   STARPU_MIN(narchs, STARPU_MAXCPUS),
			   narchs > STARPU_MAXCPUS ? narchs - STARPU_MAXCPUS : 0);
	}

	/* Parsing CUDA devs */
	_starpu_drop_comments(f);
	ret = fscanf(f, "%u\n", &narchs);
	STARPU_ASSERT(ret == 1);
	archmin += STARPU_MAXCPUS;
	_STARPU_DEBUG("Parsing %u CUDA devices\n", narchs);
	if (narchs > 0)
	{
		parse_arch(f, model, scan_history,
			   archmin,
			   archmin + STARPU_MIN(narchs, STARPU_MAXCUDADEVS),
			   narchs > STARPU_MAXCUDADEVS ? narchs - STARPU_MAXCUDADEVS : 0);
	}

	/* Parsing OpenCL devs */
	_starpu_drop_comments(f);
	ret = fscanf(f, "%u\n", &narchs);
	STARPU_ASSERT(ret == 1);

	archmin += STARPU_MAXCUDADEVS;
	_STARPU_DEBUG("Parsing %u OpenCL devices\n", narchs);
	if (narchs > 0)
	{
		parse_arch(f, model, scan_history,
			   archmin,
			   archmin + STARPU_MIN(narchs, STARPU_MAXOPENCLDEVS),
			   narchs > STARPU_MAXOPENCLDEVS ? narchs - STARPU_MAXOPENCLDEVS : 0);
	}

	/* Parsing Gordon implementations */
	_starpu_drop_comments(f);
	ret = fscanf(f, "%u\n", &narchs);
	STARPU_ASSERT(ret == 1);

	archmin += STARPU_MAXOPENCLDEVS;
	_STARPU_DEBUG("Parsing %u Gordon devices\n", narchs);
	if (narchs > 0)
	{
		parse_arch(f, model, scan_history,
			   archmin,
			   archmin + max_gordondevs,
			   narchs > max_gordondevs ? narchs - max_gordondevs : 0);
	}
}


static void dump_per_arch_model_file(FILE *f, struct starpu_perfmodel *model, unsigned arch, unsigned nimpl)
{
	struct starpu_per_arch_perfmodel *per_arch_model;

	per_arch_model = &model->per_arch[arch][nimpl];
	/* count the number of elements in the lists */
	struct starpu_history_list *ptr = NULL;
	unsigned nentries = 0;

	if (model->type == STARPU_HISTORY_BASED || model->type == STARPU_NL_REGRESSION_BASED)
	{
		/* Dump the list of all entries in the history */
		ptr = per_arch_model->list;
		while(ptr)
		{
			nentries++;
			ptr = ptr->next;
		}
	}

	/* header */
	char archname[32];
	starpu_perfmodel_get_arch_name((enum starpu_perf_archtype) arch, archname, 32, nimpl);
	fprintf(f, "# Model for %s\n", archname);
	fprintf(f, "# number of entries\n%u\n", nentries);

	dump_reg_model(f, model, arch, nimpl);

	/* Dump the history into the model file in case it is necessary */
	if (model->type == STARPU_HISTORY_BASED || model->type == STARPU_NL_REGRESSION_BASED)
	{
		fprintf(f, "# hash\t\tsize\t\tmean\t\tdev\t\tsum\t\tsum2\t\tn\n");
		ptr = per_arch_model->list;
		while (ptr)
		{
			dump_history_entry(f, ptr->entry);
			ptr = ptr->next;
		}
	}

	fprintf(f, "\n##################\n");
}

static unsigned get_n_entries(struct starpu_perfmodel *model, unsigned arch, unsigned impl)
{
	struct starpu_per_arch_perfmodel *per_arch_model;
	per_arch_model = &model->per_arch[arch][impl];
	/* count the number of elements in the lists */
	struct starpu_history_list *ptr = NULL;
	unsigned nentries = 0;

	if (model->type == STARPU_HISTORY_BASED || model->type == STARPU_NL_REGRESSION_BASED)
	{
		/* Dump the list of all entries in the history */
		ptr = per_arch_model->list;
		while(ptr)
		{
			nentries++;
			ptr = ptr->next;
		}
	}
	return nentries;
}

static void dump_model_file(FILE *f, struct starpu_perfmodel *model)
{
	unsigned narch[4] = { 0, 0, 0, 0};
	unsigned arch, arch_base = 0, my_narch = 0;
	unsigned nimpl;
	unsigned idx = 0;

	/* Finding the number of archs to write for each kind of device */
	for (arch = 0; arch < STARPU_NARCH_VARIATIONS; arch++)
	{
		switch (arch)
		{
			case STARPU_CUDA_DEFAULT:
			case STARPU_OPENCL_DEFAULT:
			case STARPU_GORDON_DEFAULT:
				arch_base = arch;
				idx++;
				break;
			default:
				break;
		}

		if (model->type == STARPU_HISTORY_BASED || model->type == STARPU_NL_REGRESSION_BASED)
		{
			for (nimpl = 0; nimpl < STARPU_MAXIMPLEMENTATIONS; nimpl++)
				if (get_n_entries(model, arch, nimpl))
				{
					narch[idx]=arch-arch_base+1;
					break;
				}
		}
		else if (model->type == STARPU_REGRESSION_BASED)
		{
			for (nimpl = 0; nimpl < STARPU_MAXIMPLEMENTATIONS; nimpl++)
				if (model->per_arch[arch][nimpl].regression.nsample)
				{
					narch[idx]=arch-arch_base+1;
					break;
				}
		}
		else
			STARPU_ASSERT_MSG(0, "Unknown history-based performance model");
	}

	/* Writing stuff */
	char *name = "unknown";
	unsigned substract_to_arch = 0;
	for (arch = 0; arch < STARPU_NARCH_VARIATIONS; arch++)
	{
		switch (arch)
		{
			case STARPU_CPU_DEFAULT:
				arch_base = arch;
				name = "CPU";
				fprintf(f, "##################\n");
				fprintf(f, "# %ss\n", name);
				fprintf(f, "# maximum number of %ss\n", name);
				fprintf(f, "%u\n", my_narch = narch[0]);
				break;
			case STARPU_CUDA_DEFAULT:
				arch_base = arch;
				name = "CUDA";
				substract_to_arch = STARPU_MAXCPUS;
				fprintf(f, "##################\n");
				fprintf(f, "# %ss\n", name);
				fprintf(f, "# number of %s architectures\n", name);
				fprintf(f, "%u\n", my_narch = narch[1]);
				break;
			case STARPU_OPENCL_DEFAULT:
				arch_base = arch;
				name = "OPENCL";
				substract_to_arch += STARPU_MAXCUDADEVS;
				fprintf(f, "##################\n");
				fprintf(f, "# %ss\n", name);
				fprintf(f, "# number of %s architectures\n", name);
				fprintf(f, "%u\n", my_narch = narch[2]);
				break;
			case STARPU_GORDON_DEFAULT:
				arch_base = arch;
				name = "GORDON";
				substract_to_arch += STARPU_MAXOPENCLDEVS;
				fprintf(f, "##################\n");
				fprintf(f, "# %ss\n", name);
				fprintf(f, "# number of %s architectures\n", name);
				fprintf(f, "%u\n", my_narch = narch[3]);
				break;
			default:
				break;
		}

		unsigned max_impl = 0;
		if (model->type == STARPU_HISTORY_BASED || model->type == STARPU_NL_REGRESSION_BASED)
		{
			for (nimpl = 0; nimpl < STARPU_MAXIMPLEMENTATIONS; nimpl++)
				if (get_n_entries(model, arch, nimpl))
					max_impl = nimpl + 1;
		}
		else if (model->type == STARPU_REGRESSION_BASED)
		{
			for (nimpl = 0; nimpl < STARPU_MAXIMPLEMENTATIONS; nimpl++)
				if (model->per_arch[arch][nimpl].regression.nsample)
					max_impl = nimpl + 1;
		}
		else
			STARPU_ASSERT_MSG(0, "Unknown history-based performance model");

		if (arch >= my_narch + arch_base)
			continue;

		fprintf(f, "###########\n");
		if (substract_to_arch)
			fprintf(f, "# %s_%u\n", name, arch - substract_to_arch);
		else
			/* CPU */
			fprintf(f, "# %u CPU(s) in parallel\n", arch + 1);
		fprintf(f, "# number of implementations\n");
		fprintf(f, "%u\n", max_impl);
		for (nimpl = 0; nimpl < max_impl; nimpl++)
		{
			dump_per_arch_model_file(f, model, arch, nimpl);
		}
	}
}

static void initialize_per_arch_model(struct starpu_per_arch_perfmodel *per_arch_model)
{
	per_arch_model->history = NULL;
	per_arch_model->list = NULL;
}

static void initialize_model(struct starpu_perfmodel *model)
{
	unsigned arch;
	unsigned nimpl;
	for (arch = 0; arch < STARPU_NARCH_VARIATIONS; arch++)
	{
		for (nimpl = 0; nimpl < STARPU_MAXIMPLEMENTATIONS; nimpl++)
		{
			initialize_per_arch_model(&model->per_arch[arch][nimpl]);
		}
	}
}

static void get_model_debug_path(struct starpu_perfmodel *model, const char *arch, char *path, size_t maxlen)
{
	STARPU_ASSERT(path);

	_starpu_get_perf_model_dir_debug(path, maxlen);
	strncat(path, model->symbol, maxlen);

	char hostname[32];
	char *forced_hostname = getenv("STARPU_HOSTNAME");
	if (forced_hostname && forced_hostname[0])
		snprintf(hostname, sizeof(hostname), "%s", forced_hostname);
	else
		gethostname(hostname, sizeof(hostname));
	strncat(path, ".", maxlen);
	strncat(path, hostname, maxlen);
	strncat(path, ".", maxlen);
	strncat(path, arch, maxlen);
	strncat(path, ".debug", maxlen);
}

/*
 * Returns 0 is the model was already loaded, 1 otherwise.
 */
int _starpu_register_model(struct starpu_perfmodel *model)
{
	/* If the model has already been loaded, there is nothing to do */
	_STARPU_PTHREAD_RWLOCK_RDLOCK(&registered_models_rwlock);
	if (model->is_loaded)
	{
		_STARPU_PTHREAD_RWLOCK_UNLOCK(&registered_models_rwlock);
		return 0;
	}
	_STARPU_PTHREAD_RWLOCK_UNLOCK(&registered_models_rwlock);

	/* We have to make sure the model has not been loaded since the
         * last time we took the lock */
	_STARPU_PTHREAD_RWLOCK_WRLOCK(&registered_models_rwlock);
	if (model->is_loaded)
	{
		_STARPU_PTHREAD_RWLOCK_UNLOCK(&registered_models_rwlock);
		return 0;
	}

	/* add the model to a linked list */
	struct starpu_model_list *node = (struct starpu_model_list *) malloc(sizeof(struct starpu_model_list));

	node->model = model;
	//model->debug_modelid = debug_modelid++;

	/* put this model at the beginning of the list */
	node->next = registered_models;
	registered_models = node;

#ifdef STARPU_MODEL_DEBUG
	_starpu_create_sampling_directory_if_needed();

	unsigned arch;
	unsigned nimpl;

	for (arch = 0; arch < STARPU_NARCH_VARIATIONS; arch++)
	{
		for (nimpl = 0; nimpl < STARPU_MAXIMPLEMENTATIONS; nimpl++)
		{
			starpu_perfmodel_debugfilepath(model, arch, model->per_arch[arch][nimpl].debug_path, 256, nimpl);
		}
	}
#endif

	_STARPU_PTHREAD_RWLOCK_UNLOCK(&registered_models_rwlock);
	return 1;
}

static void get_model_path(struct starpu_perfmodel *model, char *path, size_t maxlen)
{
	_starpu_get_perf_model_dir_codelets(path, maxlen);
	strncat(path, model->symbol, maxlen);

	char hostname[32];
	char *forced_hostname = getenv("STARPU_HOSTNAME");
	if (forced_hostname && forced_hostname[0])
		snprintf(hostname, sizeof(hostname), "%s", forced_hostname);
	else
		gethostname(hostname, sizeof(hostname));
	strncat(path, ".", maxlen);
	strncat(path, hostname, maxlen);
}

static void save_history_based_model(struct starpu_perfmodel *model)
{
	STARPU_ASSERT(model);
	STARPU_ASSERT(model->symbol);

	/* TODO checks */

	/* filename = $STARPU_PERF_MODEL_DIR/codelets/symbol.hostname */
	char path[256];
	get_model_path(model, path, 256);

	_STARPU_DEBUG("Opening performance model file %s for model %s\n", path, model->symbol);

	/* overwrite existing file, or create it */
	FILE *f;
	f = fopen(path, "w+");
	STARPU_ASSERT(f);

	dump_model_file(f, model);

	fclose(f);
}

static void _starpu_dump_registered_models(void)
{
	_STARPU_PTHREAD_RWLOCK_WRLOCK(&registered_models_rwlock);

	struct starpu_model_list *node;
	node = registered_models;

	_STARPU_DEBUG("DUMP MODELS !\n");

	while (node)
	{
		save_history_based_model(node->model);
		node = node->next;
	}

	_STARPU_PTHREAD_RWLOCK_UNLOCK(&registered_models_rwlock);
}

void _starpu_initialize_registered_performance_models(void)
{
	registered_models = NULL;

	_STARPU_PTHREAD_RWLOCK_INIT(&registered_models_rwlock, NULL);
}

void _starpu_deinitialize_registered_performance_models(void)
{
	if (_starpu_get_calibrate_flag())
		_starpu_dump_registered_models();

	_STARPU_PTHREAD_RWLOCK_WRLOCK(&registered_models_rwlock);

	struct starpu_model_list *node, *pnode;
	node = registered_models;

	_STARPU_DEBUG("FREE MODELS !\n");

	while (node)
	{
		struct starpu_perfmodel *model = node->model;
		unsigned arch;
		unsigned nimpl;

		_STARPU_PTHREAD_RWLOCK_WRLOCK(&model->model_rwlock);
		for (arch = 0; arch < STARPU_NARCH_VARIATIONS; arch++)
		{
			for (nimpl = 0; nimpl < STARPU_MAXIMPLEMENTATIONS; nimpl++)
			{
				struct starpu_per_arch_perfmodel *archmodel = &model->per_arch[arch][nimpl];
				struct starpu_history_list *list, *plist;
				_starpu_htbl_destroy_32(archmodel->history, NULL);
				archmodel->history = NULL;
				list = archmodel->list;
				while (list) {
					free(list->entry);
					plist = list;
					list = list->next;
					free(plist);
				}
				archmodel->list = NULL;
			}
		}

		model->is_loaded = 0;
		_STARPU_PTHREAD_RWLOCK_UNLOCK(&model->model_rwlock);

		pnode = node;
		node = node->next;
		free(pnode);
	}
	registered_models = NULL;

	_STARPU_PTHREAD_RWLOCK_UNLOCK(&registered_models_rwlock);
	_STARPU_PTHREAD_RWLOCK_DESTROY(&registered_models_rwlock);
}

/* We first try to grab the global lock in read mode to check whether the model
 * was loaded or not (this is very likely to have been already loaded). If the
 * model was not loaded yet, we take the lock in write mode, and if the model
 * is still not loaded once we have the lock, we do load it.  */
void _starpu_load_history_based_model(struct starpu_perfmodel *model, unsigned scan_history)
{
	STARPU_ASSERT(model);
	STARPU_ASSERT(model->symbol);

	int already_loaded;

	_STARPU_PTHREAD_RWLOCK_RDLOCK(&registered_models_rwlock);
	already_loaded = model->is_loaded;
	_STARPU_PTHREAD_RWLOCK_UNLOCK(&registered_models_rwlock);

	if (already_loaded)
		return;

	/* The model is still not loaded so we grab the lock in write mode, and
	 * if it's not loaded once we have the lock, we do load it. */

	_STARPU_PTHREAD_RWLOCK_WRLOCK(&registered_models_rwlock);

	/* Was the model initialized since the previous test ? */
	if (model->is_loaded)
	{
		_STARPU_PTHREAD_RWLOCK_UNLOCK(&registered_models_rwlock);
		return;
	}

	_STARPU_PTHREAD_RWLOCK_INIT(&model->model_rwlock, NULL);

	_STARPU_PTHREAD_RWLOCK_WRLOCK(&model->model_rwlock);

	/* make sure the performance model directory exists (or create it) */
	_starpu_create_sampling_directory_if_needed();

	char path[256];
	get_model_path(model, path, 256);

	_STARPU_DEBUG("Opening performance model file %s for model %s ...\n", path, model->symbol);

	unsigned calibrate_flag = _starpu_get_calibrate_flag();
	model->benchmarking = calibrate_flag;

	/* try to open an existing file and load it */
	int res;
	res = access(path, F_OK);
	if (res == 0)
	{
		if (calibrate_flag == 2)
		{
			/* The user specified that the performance model should
			 * be overwritten, so we don't load the existing file !
			 * */
                        _STARPU_DEBUG("Overwrite existing file\n");
			initialize_model(model);
		}
		else
		{
			/* We load the available file */
			_STARPU_DEBUG("File exists\n");
			FILE *f;
			f = fopen(path, "r");
			STARPU_ASSERT(f);

			parse_model_file(f, model, scan_history);

			fclose(f);
		}
	}
	else
	{
		_STARPU_DEBUG("File does not exists\n");
		if (!calibrate_flag)
		{
			_STARPU_DISP("Warning: model %s is not calibrated, forcing calibration for this run. Use the STARPU_CALIBRATE environment variable to control this.\n", model->symbol);
			_starpu_set_calibrate_flag(1);
			model->benchmarking = 1;
		}
		initialize_model(model);
	}

	_STARPU_DEBUG("Performance model file %s for model %s is loaded\n", path, model->symbol);

	model->is_loaded = 1;

	_STARPU_PTHREAD_RWLOCK_UNLOCK(&model->model_rwlock);

	_STARPU_PTHREAD_RWLOCK_UNLOCK(&registered_models_rwlock);
}

/* This function is intended to be used by external tools that should read
 * the performance model files */
int starpu_list_models(FILE *output)
{
        char path[256];
        DIR *dp;
        struct dirent *ep;

	char perf_model_dir_codelets[256];
	_starpu_get_perf_model_dir_codelets(perf_model_dir_codelets, 256);

        strncpy(path, perf_model_dir_codelets, 256);
        dp = opendir(path);
        if (dp != NULL)
	{
                while ((ep = readdir(dp)))
		{
                        if (strcmp(ep->d_name, ".") && strcmp(ep->d_name, ".."))
                                fprintf(output, "file: <%s>\n", ep->d_name);
                }
                closedir (dp);
                return 0;
        }
        else
	{
                perror("Couldn't open the directory");
                return 1;
        }
}

/* This function is intended to be used by external tools that should read the
 * performance model files */
int starpu_load_history_debug(const char *symbol, struct starpu_perfmodel *model)
{
	model->symbol = strdup(symbol);

	/* where is the file if it exists ? */
	char path[256];
	get_model_path(model, path, 256);

	//	_STARPU_DEBUG("get_model_path -> %s\n", path);

	/* does it exist ? */
	int res;
	res = access(path, F_OK);
	if (res)
	{
		const char *dot = strrchr(symbol, '.');
		if (dot)
		{
			char *symbol2 = strdup(symbol);
			symbol2[dot-symbol] = '\0';
			int ret;
			fprintf(stderr,"note: loading history from %s instead of %s\n", symbol2, symbol);
			ret = starpu_load_history_debug(symbol2,model);
			free(symbol2);
			return ret;
		}
		_STARPU_DISP("There is no performance model for symbol %s\n", symbol);
		return 1;
	}

	FILE *f = fopen(path, "r");
	STARPU_ASSERT(f);

	parse_model_file(f, model, 1);

	STARPU_ASSERT(fclose(f) == 0);

	return 0;
}

void starpu_perfmodel_get_arch_name(enum starpu_perf_archtype arch, char *archname, size_t maxlen,unsigned nimpl)
{
	if (arch < STARPU_CUDA_DEFAULT)
	{
		if (arch == STARPU_CPU_DEFAULT)
		{
			/* NB: We could just use cpu_1 as well ... */
			snprintf(archname, maxlen, "cpu_impl_%u",nimpl);
		}
		else
		{
			/* For combined CPU workers */
			int cpu_count = arch - STARPU_CPU_DEFAULT + 1;
			snprintf(archname, maxlen, "cpu_%d_impl_%u", cpu_count,nimpl);
		}
	}
	else if ((STARPU_CUDA_DEFAULT <= arch)
		&& (arch < STARPU_CUDA_DEFAULT + STARPU_MAXCUDADEVS))
	{
		int devid = arch - STARPU_CUDA_DEFAULT;
		snprintf(archname, maxlen, "cuda_%d_impl_%u", devid,nimpl);
	}
	else if ((STARPU_OPENCL_DEFAULT <= arch)
		&& (arch < STARPU_OPENCL_DEFAULT + STARPU_MAXOPENCLDEVS))
	{
		int devid = arch - STARPU_OPENCL_DEFAULT;
		snprintf(archname, maxlen, "opencl_%d_impl_%u", devid,nimpl);
	}
	else if (arch == STARPU_GORDON_DEFAULT)
	{
		snprintf(archname, maxlen, "gordon_impl_%u",nimpl);
	}
	else
	{
		STARPU_ABORT();
	}
}

void starpu_perfmodel_debugfilepath(struct starpu_perfmodel *model,
				    enum starpu_perf_archtype arch, char *path, size_t maxlen, unsigned nimpl)
{
	char archname[32];
	starpu_perfmodel_get_arch_name(arch, archname, 32, nimpl);

	STARPU_ASSERT(path);

	get_model_debug_path(model, archname, path, maxlen);
}

double _starpu_regression_based_job_expected_perf(struct starpu_perfmodel *model, enum starpu_perf_archtype arch, struct _starpu_job *j, unsigned nimpl)
{
	double exp = NAN;
	size_t size = _starpu_job_get_data_size(model, arch, nimpl, j);
	struct starpu_regression_model *regmodel;

	regmodel = &model->per_arch[arch][nimpl].regression;

	if (regmodel->valid)
                exp = regmodel->alpha*pow((double)size, regmodel->beta);

	return exp;
}

double _starpu_non_linear_regression_based_job_expected_perf(struct starpu_perfmodel *model, enum starpu_perf_archtype arch, struct _starpu_job *j,unsigned nimpl)
{
	double exp = NAN;
	size_t size = _starpu_job_get_data_size(model, arch, nimpl, j);
	struct starpu_regression_model *regmodel;

	regmodel = &model->per_arch[arch][nimpl].regression;

	if (regmodel->nl_valid && size >= regmodel->minx * 0.9 && size <= regmodel->maxx * 1.1)
		exp = regmodel->a*pow((double)size, regmodel->b) + regmodel->c;
	else
	{
		uint32_t key = _starpu_compute_buffers_footprint(model, arch, nimpl, j);
		struct starpu_per_arch_perfmodel *per_arch_model = &model->per_arch[arch][nimpl];
		struct starpu_htbl32_node *history;
		struct starpu_history_entry *entry;

		_STARPU_PTHREAD_RWLOCK_RDLOCK(&model->model_rwlock);
		history = per_arch_model->history;
		entry = (struct starpu_history_entry *) _starpu_htbl_search_32(history, key);
		_STARPU_PTHREAD_RWLOCK_UNLOCK(&model->model_rwlock);

		if (entry && entry->nsample >= _STARPU_CALIBRATION_MINIMUM)
			exp = entry->mean;
		else if (!model->benchmarking)
		{
			_STARPU_DISP("Warning: model %s is not calibrated enough, forcing calibration for this run. Use the STARPU_CALIBRATE environment variable to control this.\n", model->symbol);
			_starpu_set_calibrate_flag(1);
			model->benchmarking = 1;
		}
	}

	return exp;
}

double _starpu_history_based_job_expected_perf(struct starpu_perfmodel *model, enum starpu_perf_archtype arch, struct _starpu_job *j,unsigned nimpl)
{
	double exp;
	struct starpu_per_arch_perfmodel *per_arch_model;
	struct starpu_history_entry *entry;
	struct starpu_htbl32_node *history;

	uint32_t key = _starpu_compute_buffers_footprint(model, arch, nimpl, j);

	per_arch_model = &model->per_arch[arch][nimpl];

	_STARPU_PTHREAD_RWLOCK_RDLOCK(&model->model_rwlock);
	history = per_arch_model->history;
	if (!history) {
		_STARPU_PTHREAD_RWLOCK_UNLOCK(&model->model_rwlock);
		return NAN;
	}

	entry = (struct starpu_history_entry *) _starpu_htbl_search_32(history, key);
	_STARPU_PTHREAD_RWLOCK_UNLOCK(&model->model_rwlock);

	exp = entry?entry->mean:NAN;

	if (entry && entry->nsample < _STARPU_CALIBRATION_MINIMUM)
		/* TODO: report differently if we've scheduled really enough
		 * of that task and the scheduler should perhaps put it aside */
		/* Not calibrated enough */
		exp = NAN;

	if (isnan(exp) && !model->benchmarking)
	{
		_STARPU_DISP("Warning: model %s is not calibrated enough, forcing calibration for this run. Use the STARPU_CALIBRATE environment variable to control this.\n", model->symbol);
		_starpu_set_calibrate_flag(1);
		model->benchmarking = 1;
	}

	return exp;
}

void _starpu_update_perfmodel_history(struct _starpu_job *j, struct starpu_perfmodel *model, enum starpu_perf_archtype arch, unsigned cpuid STARPU_ATTRIBUTE_UNUSED, double measured, unsigned nimpl)
{
	if (model)
	{
		_STARPU_PTHREAD_RWLOCK_WRLOCK(&model->model_rwlock);

		struct starpu_per_arch_perfmodel *per_arch_model = &model->per_arch[arch][nimpl];

		if (model->type == STARPU_HISTORY_BASED || model->type == STARPU_NL_REGRESSION_BASED)
		{
			struct starpu_history_entry *entry;
			struct starpu_htbl32_node *history;
			struct starpu_htbl32_node **history_ptr;
			struct starpu_history_list **list;
			uint32_t key = _starpu_compute_buffers_footprint(model, arch, nimpl, j);

			history = per_arch_model->history;
			history_ptr = &per_arch_model->history;
			list = &per_arch_model->list;

			entry = (struct starpu_history_entry *) _starpu_htbl_search_32(history, key);

			if (!entry)
			{
				/* this is the first entry with such a footprint */
				entry = (struct starpu_history_entry *) malloc(sizeof(struct starpu_history_entry));
				STARPU_ASSERT(entry);
					entry->mean = measured;
					entry->sum = measured;

					entry->deviation = 0.0;
					entry->sum2 = measured*measured;

					entry->size = _starpu_job_get_data_size(model, arch, nimpl, j);

					entry->footprint = key;
					entry->nsample = 1;

				insert_history_entry(entry, list, history_ptr);

			}
			else
			{
				/* there is already some entry with the same footprint */
				entry->sum += measured;
				entry->sum2 += measured*measured;
				entry->nsample++;

				unsigned n = entry->nsample;
				entry->mean = entry->sum / n;
				entry->deviation = sqrt((entry->sum2 - (entry->sum*entry->sum)/n)/n);
			}

			STARPU_ASSERT(entry);
		}

		if (model->type == STARPU_REGRESSION_BASED || model->type == STARPU_NL_REGRESSION_BASED)
		{
			struct starpu_regression_model *reg_model;
			reg_model = &per_arch_model->regression;

			/* update the regression model */
			size_t job_size = _starpu_job_get_data_size(model, arch, nimpl, j);
			double logy, logx;
			logx = log((double)job_size);
			logy = log(measured);

			reg_model->sumlnx += logx;
			reg_model->sumlnx2 += logx*logx;
			reg_model->sumlny += logy;
			reg_model->sumlnxlny += logx*logy;
			if (reg_model->minx == 0 || job_size < reg_model->minx)
				reg_model->minx = job_size;
			if (reg_model->maxx == 0 || job_size > reg_model->maxx)
				reg_model->maxx = job_size;
			reg_model->nsample++;

			unsigned n = reg_model->nsample;

			double num = (n*reg_model->sumlnxlny - reg_model->sumlnx*reg_model->sumlny);
			double denom = (n*reg_model->sumlnx2 - reg_model->sumlnx*reg_model->sumlnx);

			reg_model->beta = num/denom;
			reg_model->alpha = exp((reg_model->sumlny - reg_model->beta*reg_model->sumlnx)/n);

			if (VALID_REGRESSION(reg_model))
				reg_model->valid = 1;
		}

#ifdef STARPU_MODEL_DEBUG
		struct starpu_task *task = j->task;
		FILE *f = fopen(per_arch_model->debug_path, "a+");
		if (f == NULL)
		{
			_STARPU_DISP("Error <%s> when opening file <%s>\n", strerror(errno), per_arch_model->debug_path);
			STARPU_ASSERT(0);
		}

		if (!j->footprint_is_computed)
			(void) _starpu_compute_buffers_footprint(model, arch, nimpl, j);

		STARPU_ASSERT(j->footprint_is_computed);

		fprintf(f, "0x%x\t%lu\t%f\t%f\t%f\t%d\t\t", j->footprint, (unsigned long) _starpu_job_get_data_size(model, arch, nimpl, j), measured, task->predicted, task->predicted_transfer, cpuid);
		unsigned i;

		for (i = 0; i < task->cl->nbuffers; i++)
		{
			starpu_data_handle_t handle = task->handles[i];

			STARPU_ASSERT(handle->ops);
			STARPU_ASSERT(handle->ops->display);
			handle->ops->display(handle, f);
		}
		fprintf(f, "\n");
		fclose(f);
#endif
		_STARPU_PTHREAD_RWLOCK_UNLOCK(&model->model_rwlock);
	}
}
