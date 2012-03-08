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

#include <starpu.h>
#include <starpu_profiling.h>
#include <common/config.h>
#include <common/utils.h>
#include <unistd.h>
#include <sys/stat.h>
#include <core/perfmodel/perfmodel.h>
#include <core/jobs.h>
#include <core/workers.h>
#include <datawizard/datawizard.h>

#ifdef STARPU_HAVE_WINDOWS
#include <windows.h>
#endif

/* This flag indicates whether performance models should be calibrated or not.
 *	0: models need not be calibrated
 *	1: models must be calibrated
 *	2: models must be calibrated, existing models are overwritten.
 */
static unsigned calibrate_flag = 0;

void _starpu_set_calibrate_flag(unsigned val)
{
	calibrate_flag = val;
}

unsigned _starpu_get_calibrate_flag(void)
{
	return calibrate_flag;
}

enum starpu_perf_archtype starpu_worker_get_perf_archtype(int workerid)
{
	struct _starpu_machine_config *config = _starpu_get_machine_config();

	/* This workerid may either be a basic worker or a combined worker */
	unsigned nworkers = config->topology.nworkers;

	if (workerid < (int)config->topology.nworkers)
		return config->workers[workerid].perf_arch;

	/* We have a combined worker */
	unsigned ncombinedworkers = config->topology.ncombinedworkers;
	STARPU_ASSERT(workerid < (int)(ncombinedworkers + nworkers));
	return config->combined_workers[workerid - nworkers].perf_arch;
}

/*
 * PER ARCH model
 */

static double per_arch_task_expected_perf(struct starpu_perfmodel *model, enum starpu_perf_archtype arch, struct starpu_task *task, unsigned nimpl)
{
	double exp = NAN;
	double (*per_arch_cost_function)(struct starpu_task *task, enum starpu_perf_archtype arch, unsigned nimpl);
	double (*per_arch_cost_model)(struct starpu_buffer_descr *);

	per_arch_cost_function = model->per_arch[arch][nimpl].cost_function;
	per_arch_cost_model = model->per_arch[arch][nimpl].cost_model;

	if (per_arch_cost_function)
		exp = per_arch_cost_function(task, arch, nimpl);
	else if (per_arch_cost_model)
		exp = per_arch_cost_model(task->buffers);

	return exp;
}

/*
 * Common model
 */

double starpu_worker_get_relative_speedup(enum starpu_perf_archtype perf_archtype)
{
	if (perf_archtype < STARPU_CUDA_DEFAULT)
	{
		return _STARPU_CPU_ALPHA * (perf_archtype + 1);
	}
	else if (perf_archtype < STARPU_OPENCL_DEFAULT)
	{
		return _STARPU_CUDA_ALPHA;
	}
	else if (perf_archtype < STARPU_GORDON_DEFAULT)
	{
		return _STARPU_OPENCL_ALPHA;
	}
	else if (perf_archtype < STARPU_NARCH_VARIATIONS)
	{
		/* Gordon value */
		return _STARPU_GORDON_ALPHA;
	}

	STARPU_ABORT();

	/* Never reached ! */
	return NAN;
}

static double common_task_expected_perf(struct starpu_perfmodel *model, enum starpu_perf_archtype arch, struct starpu_task *task, unsigned nimpl)
{
	double exp;
	double alpha;

	if (model->cost_function)
	{
		exp = model->cost_function(task, nimpl);
		alpha = starpu_worker_get_relative_speedup(arch);

		STARPU_ASSERT(!_STARPU_IS_ZERO(alpha));

		return (exp/alpha);
	}
	else if (model->cost_model)
	{
		exp = model->cost_model(task->buffers);
		alpha = starpu_worker_get_relative_speedup(arch);

		STARPU_ASSERT(!_STARPU_IS_ZERO(alpha));

		return (exp/alpha);
	}

	return NAN;
}

void _starpu_load_perfmodel(struct starpu_perfmodel *model)
{
	if (!model || model->is_loaded)
		return;

	int load_model = _starpu_register_model(model);

	if (!load_model)
		return;

	switch (model->type)
	{
		case STARPU_PER_ARCH:
		case STARPU_COMMON:
			break;

		case STARPU_HISTORY_BASED:
		case STARPU_NL_REGRESSION_BASED:
			_starpu_load_history_based_model(model, 1);
			break;

		case STARPU_REGRESSION_BASED:
			_starpu_load_history_based_model(model, 0);
			break;

		default:
			STARPU_ABORT();
	}

	model->is_loaded = 1;
}

static double starpu_model_expected_perf(struct starpu_task *task, struct starpu_perfmodel *model, enum starpu_perf_archtype arch,  unsigned nimpl)
{
	if (model)
	{
		struct _starpu_job *j = _starpu_get_job_associated_to_task(task);
		switch (model->type)
		{
			case STARPU_PER_ARCH:

				return per_arch_task_expected_perf(model, arch, task, nimpl);
			case STARPU_COMMON:
				return common_task_expected_perf(model, arch, task, nimpl);

			case STARPU_HISTORY_BASED:

				return _starpu_history_based_job_expected_perf(model, arch, j, nimpl);
			case STARPU_REGRESSION_BASED:

				return _starpu_regression_based_job_expected_perf(model, arch, j, nimpl);

			case STARPU_NL_REGRESSION_BASED:

				return _starpu_non_linear_regression_based_job_expected_perf(model, arch, j,nimpl);

			default:
				STARPU_ABORT();
		}
	}

	/* no model was found */
	return 0.0;
}

double starpu_task_expected_length(struct starpu_task *task, enum starpu_perf_archtype arch, unsigned nimpl)
{

	return starpu_model_expected_perf(task, task->cl->model, arch, nimpl);
}

double starpu_task_expected_power(struct starpu_task *task, enum starpu_perf_archtype arch, unsigned nimpl)
{
	return starpu_model_expected_perf(task, task->cl->power_model, arch, nimpl);
}

double starpu_task_expected_conversion_time(struct starpu_task *task,
					    enum starpu_perf_archtype arch,
					    unsigned nimpl)
{
	unsigned i;
	int err;
	double sum = 0.0;
	unsigned int node, cpu_node;

	/* We need to get one node per archtype. This is kinda ugly,
	 * but it does the job.
	 * XXX : Should we return 0 if there are no devices ?
	 * (err != 1 && err != -ERANGE)
	 */
#ifdef STARPU_USE_CPU
	int cpu_worker;
	err = starpu_worker_get_ids_by_type(STARPU_CPU_WORKER,
					    &cpu_worker, 1);
	if (err != 1 && err != -ERANGE)
		return 0.0;
	cpu_node = starpu_worker_get_memory_node(cpu_worker);
#endif
#ifdef STARPU_USE_CUDA
	int cuda_worker, cuda_node;
	err = starpu_worker_get_ids_by_type(STARPU_CUDA_WORKER,
					    &cuda_worker, 1);
	if (err != 1 && err != -ERANGE)
		return 0.0;
	cuda_node = starpu_worker_get_memory_node(cuda_worker);
#endif
#ifdef STARPU_USE_OPENCL
	int opencl_worker, opencl_node;
	err = starpu_worker_get_ids_by_type(STARPU_OPENCL_WORKER,
					    &opencl_worker, 1);
	if (err != 1 && err != -ERANGE)
		return 0.0;

	opencl_node = starpu_worker_get_memory_node(opencl_worker);
#endif

	for (i = 0; i < task->cl->nbuffers; i++)
	{
		starpu_data_handle_t handle;
		struct starpu_task *conversion_task;

		handle = task->handles[i];
		if (!_starpu_data_is_multiformat_handle(handle))
			continue;

		if (arch < STARPU_CUDA_DEFAULT)
			node = cpu_node;
#ifdef STARPU_USE_CUDA
		else if (arch >= STARPU_CUDA_DEFAULT && arch < STARPU_OPENCL_DEFAULT)
			node = cuda_node;
#endif
#ifdef STARPU_USE_OPENCL
		else if (arch >= STARPU_OPENCL_DEFAULT && arch < STARPU_GORDON_DEFAULT)
			node = opencl_node;
#endif
		else {
			node = -EINVAL;
			STARPU_ASSERT(0);
		}

		if (!_starpu_handle_needs_conversion_task(handle, node))
			continue;

		conversion_task = _starpu_create_conversion_task(handle, node);
		sum += starpu_task_expected_length(conversion_task, arch, nimpl);
		handle->refcnt--;
		handle->busy_count--;
		starpu_task_deinit(conversion_task);
		free(conversion_task);
	}

	return sum;
}

/* Predict the transfer time (in µs) to move a handle to a memory node */
double starpu_data_expected_transfer_time(starpu_data_handle_t handle, unsigned memory_node, enum starpu_access_mode mode)
{
	/* If we don't need to read the content of the handle */
	if (!(mode & STARPU_R))
		return 0.0;

	if (_starpu_is_data_present_or_requested(handle, memory_node))
		return 0.0;

	size_t size = _starpu_data_get_size(handle);

	/* XXX in case we have an abstract piece of data (eg.  with the
	 * void interface, this does not introduce any overhead, and we
	 * don't even want to consider the latency that is not
	 * relevant). */
	if (size == 0)
		return 0.0;

	uint32_t src_node = _starpu_select_src_node(handle, memory_node);
	return _starpu_predict_transfer_time(src_node, memory_node, size);
}

/* Data transfer performance modeling */
double starpu_task_expected_data_transfer_time(uint32_t memory_node, struct starpu_task *task)
{
	unsigned nbuffers = task->cl->nbuffers;
	unsigned buffer;

	double penalty = 0.0;

	for (buffer = 0; buffer < nbuffers; buffer++)
	{
		starpu_data_handle_t handle = task->handles[buffer];
		enum starpu_access_mode mode = task->cl->modes[buffer];

		penalty += starpu_data_expected_transfer_time(handle, memory_node, mode);
	}

	return penalty;
}

/* Return the expected duration of the entire task bundle in µs */
double starpu_task_bundle_expected_length(starpu_task_bundle_t bundle, enum starpu_perf_archtype arch, unsigned nimpl)
{
	double expected_length = 0.0;

	/* We expect the length of the bundle the be the sum of the different tasks length. */
	_STARPU_PTHREAD_MUTEX_LOCK(&bundle->mutex);

	struct _starpu_task_bundle_entry *entry;
	entry = bundle->list;

	while (entry)
	{
		double task_length = starpu_task_expected_length(entry->task, arch, nimpl);

		/* In case the task is not calibrated, we consider the task
		 * ends immediately. */
		if (task_length > 0.0)
			expected_length += task_length;

		entry = entry->next;
	}

	_STARPU_PTHREAD_MUTEX_UNLOCK(&bundle->mutex);

	return expected_length;
}

/* Return the expected power consumption of the entire task bundle in J */
double starpu_task_bundle_expected_power(starpu_task_bundle_t bundle, enum starpu_perf_archtype arch, unsigned nimpl)
{
	double expected_power = 0.0;

	/* We expect total consumption of the bundle the be the sum of the different tasks consumption. */
	_STARPU_PTHREAD_MUTEX_LOCK(&bundle->mutex);

	struct _starpu_task_bundle_entry *entry;
	entry = bundle->list;

	while (entry)
	{
		double task_power = starpu_task_expected_power(entry->task, arch, nimpl);

		/* In case the task is not calibrated, we consider the task
		 * ends immediately. */
		if (task_power > 0.0)
			expected_power += task_power;

		entry = entry->next;
	}

	_STARPU_PTHREAD_MUTEX_UNLOCK(&bundle->mutex);

	return expected_power;
}

/* Return the time (in µs) expected to transfer all data used within the bundle */
double starpu_task_bundle_expected_data_transfer_time(starpu_task_bundle_t bundle, unsigned memory_node)
{
	_STARPU_PTHREAD_MUTEX_LOCK(&bundle->mutex);

	struct _starpu_handle_list *handles = NULL;

	/* We list all the handle that are accessed within the bundle. */

	/* For each task in the bundle */
	struct _starpu_task_bundle_entry *entry = bundle->list;
	while (entry)
	{
		struct starpu_task *task = entry->task;

		if (task->cl)
		{
			unsigned b;
			for (b = 0; b < task->cl->nbuffers; b++)
			{
				starpu_data_handle_t handle = task->handles[b];
				enum starpu_access_mode mode = task->cl->modes[b];

				if (!(mode & STARPU_R))
					continue;

				/* Insert the handle in the sorted list in case
				 * it's not already in that list. */
				_insertion_handle_sorted(&handles, handle, mode);
			}
		}

		entry = entry->next;
	}

	_STARPU_PTHREAD_MUTEX_UNLOCK(&bundle->mutex);

	/* Compute the sum of data transfer time, and destroy the list */

	double total_exp = 0.0;

	while (handles)
	{
		struct _starpu_handle_list *current = handles;
		handles = handles->next;

		double exp;
		exp = starpu_data_expected_transfer_time(current->handle, memory_node, current->mode);

		total_exp += exp;

		free(current);
	}

	return total_exp;
}

static int directory_existence_was_tested = 0;

void _starpu_get_perf_model_dir(char *path, size_t maxlen)
{
#ifdef STARPU_PERF_MODEL_DIR
	/* use the directory specified at configure time */
	snprintf(path, maxlen, "%s", STARPU_PERF_MODEL_DIR);
#else
	const char *home_path = getenv("XDG_CACHE_HOME");
	if (!home_path)
		home_path = getenv("STARPU_HOME");
	if (!home_path)
		home_path = getenv("HOME");
	if (!home_path)
		home_path = getenv("USERPROFILE");
	if (!home_path)
		_STARPU_ERROR("couldn't find a home place to put starpu data\n");
	snprintf(path, maxlen, "%s/.starpu/sampling/", home_path);
#endif
}

void _starpu_get_perf_model_dir_codelets(char *path, size_t maxlen)
{
	_starpu_get_perf_model_dir(path, maxlen);
	strncat(path, "/codelets/", maxlen);
}

void _starpu_get_perf_model_dir_bus(char *path, size_t maxlen)
{
	_starpu_get_perf_model_dir(path, maxlen);
	strncat(path, "/bus/", maxlen);
}

void _starpu_get_perf_model_dir_debug(char *path, size_t maxlen)
{
	_starpu_get_perf_model_dir(path, maxlen);
	strncat(path, "/debug/", maxlen);
}

void _starpu_create_sampling_directory_if_needed(void)
{
	if (!directory_existence_was_tested)
	{
		char perf_model_dir[256];
		_starpu_get_perf_model_dir(perf_model_dir, 256);

		/* The performance of the codelets are stored in
		 * $STARPU_PERF_MODEL_DIR/codelets/ while those of the bus are stored in
		 * $STARPU_PERF_MODEL_DIR/bus/ so that we don't have name collisions */

		/* Testing if a directory exists and creating it otherwise
		   may not be safe: it is possible that the permission are
		   changed in between. Instead, we create it and check if
		   it already existed before */
		int ret;
		ret = _starpu_mkpath(perf_model_dir, S_IRWXU);

		if (ret == -1)
		{
			if (errno != EEXIST) {
				fprintf(stderr,"Error making starpu directory %s:\n", perf_model_dir);
				perror("mkdir");
				STARPU_ASSERT(0);
			}

			/* make sure that it is actually a directory */
			struct stat sb;
			stat(perf_model_dir, &sb);
			if (!S_ISDIR(sb.st_mode)) {
				fprintf(stderr,"Error: %s is not a directory:\n", perf_model_dir);
				STARPU_ASSERT(0);
			}
		}

		/* Per-task performance models */
		char perf_model_dir_codelets[256];
		_starpu_get_perf_model_dir_codelets(perf_model_dir_codelets, 256);

		ret = _starpu_mkpath(perf_model_dir_codelets, S_IRWXU);
		if (ret == -1)
		{
			if (errno != EEXIST) {
				fprintf(stderr,"Error making starpu directory %s:\n", perf_model_dir);
				perror("mkdir");
				STARPU_ASSERT(0);
			}


			/* make sure that it is actually a directory */
			struct stat sb;
			stat(perf_model_dir_codelets, &sb);
			if (!S_ISDIR(sb.st_mode)) {
				fprintf(stderr,"Error: %s is not a directory:\n", perf_model_dir);
				STARPU_ASSERT(0);
			}
		}

		/* Performance of the memory subsystem */
		char perf_model_dir_bus[256];
		_starpu_get_perf_model_dir_bus(perf_model_dir_bus, 256);

		ret = _starpu_mkpath(perf_model_dir_bus, S_IRWXU);
		if (ret == -1)
		{
			if (errno != EEXIST) {
				fprintf(stderr,"Error making starpu directory %s:\n", perf_model_dir);
				perror("mkdir");
				STARPU_ASSERT(0);
			}

			/* make sure that it is actually a directory */
			struct stat sb;
			stat(perf_model_dir_bus, &sb);
			if (!S_ISDIR(sb.st_mode)) {
				fprintf(stderr,"Error: %s is not a directory:\n", perf_model_dir);
				STARPU_ASSERT(0);
			}
		}

		/* Performance debug measurements */
		char perf_model_dir_debug[256];
		_starpu_get_perf_model_dir_debug(perf_model_dir_debug, 256);

		ret = _starpu_mkpath(perf_model_dir_debug, S_IRWXU);
		if (ret == -1)
		{
			if (errno != EEXIST) {
				fprintf(stderr,"Error making starpu directory %s:\n", perf_model_dir);
				perror("mkdir");
				STARPU_ASSERT(0);
			}


			/* make sure that it is actually a directory */
			struct stat sb;
			stat(perf_model_dir_debug, &sb);
			if (!S_ISDIR(sb.st_mode)) {
				fprintf(stderr,"Error: %s is not a directory:\n", perf_model_dir);
				STARPU_ASSERT(0);
			}
		}

		directory_existence_was_tested = 1;
	}
}
