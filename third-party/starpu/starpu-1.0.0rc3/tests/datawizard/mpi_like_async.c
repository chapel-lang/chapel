/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010-2011  Université de Bordeaux 1
 * Copyright (C) 2010, 2011, 2012  Centre National de la Recherche Scientifique
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

#include <config.h>
#include <starpu.h>
#include <pthread.h>
#include "../helper.h"

#define NTHREADS_DEFAULT	16
#define NITER_DEFAULT		128

static int nthreads = NTHREADS_DEFAULT;
static int niter = NITER_DEFAULT;

//#define DEBUG_MESSAGES	1

//static pthread_cond_t cond;
//static pthread_mutex_t mutex;

struct thread_data
{
	unsigned index;
	unsigned val;
	starpu_data_handle_t handle;
	pthread_t thread;

	pthread_mutex_t recv_mutex;
	unsigned recv_flag; // set when a message is received
	unsigned recv_buf;
	struct thread_data *neighbour;
};

struct data_req
{
	int (*test_func)(void *);
	void *test_arg;
	struct data_req *next;
};

static pthread_mutex_t data_req_mutex;
static pthread_cond_t data_req_cond;
struct data_req *data_req_list;
unsigned progress_thread_running;

static struct thread_data problem_data[NTHREADS_DEFAULT];

/* We implement some ring transfer, every thread will try to receive a piece of
 * data from its neighbour and increment it before transmitting it to its
 * successor. */

#ifdef STARPU_USE_CUDA
void cuda_codelet_unsigned_inc(void *descr[], __attribute__ ((unused)) void *cl_arg);
#endif

static void increment_handle_cpu_kernel(void *descr[], void *cl_arg __attribute__((unused)))
{
	STARPU_SKIP_IF_VALGRIND;

	unsigned *val = (unsigned *)STARPU_VARIABLE_GET_PTR(descr[0]);
	*val += 1;

//	FPRINTF(stderr, "VAL %d (&val = %p)\n", *val, val);
}

static struct starpu_codelet increment_handle_cl =
{
	.modes = { STARPU_RW },
	.where = STARPU_CPU|STARPU_CUDA,
	.cpu_funcs = {increment_handle_cpu_kernel, NULL},
#ifdef STARPU_USE_CUDA
	.cuda_funcs = {cuda_codelet_unsigned_inc, NULL},
#endif
	.nbuffers = 1
};

static void increment_handle_async(struct thread_data *thread_data)
{
	struct starpu_task *task = starpu_task_create();
	task->cl = &increment_handle_cl;

	task->handles[0] = thread_data->handle;

	task->detach = 1;
	task->destroy = 1;

	int ret = starpu_task_submit(task);
	if (ret == -ENODEV)
		exit(STARPU_TEST_SKIPPED);
	STARPU_ASSERT(!ret);
}

static int test_recv_handle_async(void *arg)
{
//	FPRINTF(stderr, "test_recv_handle_async\n");

	int ret;
	struct thread_data *thread_data = (struct thread_data *) arg;

	_STARPU_PTHREAD_MUTEX_LOCK(&thread_data->recv_mutex);

	ret = (thread_data->recv_flag == 1);

	if (ret)
	{
		thread_data->recv_flag = 0;
		thread_data->val = thread_data->recv_buf;
	}

	_STARPU_PTHREAD_MUTEX_UNLOCK(&thread_data->recv_mutex);

	if (ret)
	{
#ifdef DEBUG_MESSAGES
		FPRINTF(stderr, "Thread %d received value %d from thread %d\n",
			thread_data->index, thread_data->val, (thread_data->index - 1)%nthreads);
#endif
		starpu_data_release(thread_data->handle);
	}

	return ret;
}

static void recv_handle_async(void *_thread_data)
{
	struct thread_data *thread_data = (struct thread_data *) _thread_data;

	struct data_req *req = (struct data_req *) malloc(sizeof(struct data_req));
	req->test_func = test_recv_handle_async;
	req->test_arg = thread_data;
	req->next = NULL;

	_STARPU_PTHREAD_MUTEX_LOCK(&data_req_mutex);
	req->next = data_req_list;
	data_req_list = req;
	_STARPU_PTHREAD_COND_SIGNAL(&data_req_cond);
	_STARPU_PTHREAD_MUTEX_UNLOCK(&data_req_mutex);
}

static int test_send_handle_async(void *arg)
{
	int ret;
	struct thread_data *thread_data = (struct thread_data *) arg;
	struct thread_data *neighbour_data = thread_data->neighbour;

	_STARPU_PTHREAD_MUTEX_LOCK(&neighbour_data->recv_mutex);
	ret = (neighbour_data->recv_flag == 0);
	_STARPU_PTHREAD_MUTEX_UNLOCK(&neighbour_data->recv_mutex);

	if (ret)
	{
#ifdef DEBUG_MESSAGES
		FPRINTF(stderr, "Thread %d sends value %d to thread %d\n", thread_data->index, thread_data->val, neighbour_data->index);
#endif
		starpu_data_release(thread_data->handle);
	}

	return ret;
}

static void send_handle_async(void *_thread_data)
{
	struct thread_data *thread_data = (struct thread_data *) _thread_data;
	struct thread_data *neighbour_data = thread_data->neighbour;

//	FPRINTF(stderr, "send_handle_async\n");

	/* send the message */
	_STARPU_PTHREAD_MUTEX_LOCK(&neighbour_data->recv_mutex);
	neighbour_data->recv_buf = thread_data->val;
	neighbour_data->recv_flag = 1;
	_STARPU_PTHREAD_MUTEX_UNLOCK(&neighbour_data->recv_mutex);

	struct data_req *req = (struct data_req *) malloc(sizeof(struct data_req));
	req->test_func = test_send_handle_async;
	req->test_arg = thread_data;
	req->next = NULL;

	_STARPU_PTHREAD_MUTEX_LOCK(&data_req_mutex);
	req->next = data_req_list;
	data_req_list = req;
	_STARPU_PTHREAD_COND_SIGNAL(&data_req_cond);
	_STARPU_PTHREAD_MUTEX_UNLOCK(&data_req_mutex);
}

static void *progress_func(void *arg)
{
	_STARPU_PTHREAD_MUTEX_LOCK(&data_req_mutex);

	progress_thread_running = 1;
	_STARPU_PTHREAD_COND_SIGNAL(&data_req_cond);

	while (progress_thread_running)
	{
		struct data_req *req;

		if (data_req_list == NULL)
			_STARPU_PTHREAD_COND_WAIT(&data_req_cond, &data_req_mutex);

		req = data_req_list;

		if (req)
		{
			data_req_list = req->next;
			req->next = NULL;

			_STARPU_PTHREAD_MUTEX_UNLOCK(&data_req_mutex);

			int ret = req->test_func(req->test_arg);

			if (ret)
			{
				free(req);
				_STARPU_PTHREAD_MUTEX_LOCK(&data_req_mutex);
			}
			else
			{
				/* ret = 0 : the request is not finished, we put it back at the end of the list */
				_STARPU_PTHREAD_MUTEX_LOCK(&data_req_mutex);

				struct data_req *req_aux = data_req_list;
				if (!req_aux)
				{
					/* The list is empty */
					data_req_list = req;
				}
				else
				{
					while (req_aux)
					{
						if (req_aux->next == NULL)
						{
							req_aux->next = req;
							break;
						}

						req_aux = req_aux->next;
					}
				}
			}
		}
	}
	_STARPU_PTHREAD_MUTEX_UNLOCK(&data_req_mutex);

	return NULL;
}

static void *thread_func(void *arg)
{
	unsigned iter;
	struct thread_data *thread_data = (struct thread_data *) arg;
	unsigned index = thread_data->index;
	int ret;

	starpu_variable_data_register(&thread_data->handle, 0, (uintptr_t)&thread_data->val, sizeof(unsigned));

	for (iter = 0; iter < niter; iter++)
	{
		/* The first thread initiates the first transfer */
		if (!((index == 0) && (iter == 0)))
		{
			starpu_data_acquire_cb(
				thread_data->handle, STARPU_W,
				recv_handle_async, thread_data
			);
		}

		increment_handle_async(thread_data);

		if (!((index == (nthreads - 1)) && (iter == (niter - 1))))
		{
			starpu_data_acquire_cb(
				thread_data->handle, STARPU_R,
				send_handle_async, thread_data
			);
		}
	}

	ret = starpu_task_wait_for_all();
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_wait_for_all");

	return NULL;
}

int main(int argc, char **argv)
{
	int ret;
	void *retval;

#ifdef STARPU_SLOW_MACHINE
	niter /= 16;
	nthreads /= 4;
#endif

	ret = starpu_init(NULL);
	if (ret == -ENODEV) return STARPU_TEST_SKIPPED;
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_init");
	/* Create a thread to perform blocking calls */
	pthread_t progress_thread;
	_STARPU_PTHREAD_MUTEX_INIT(&data_req_mutex, NULL);
	_STARPU_PTHREAD_COND_INIT(&data_req_cond, NULL);
	data_req_list = NULL;
	progress_thread_running = 0;

	unsigned t;
	for (t = 0; t < nthreads; t++)
	{
		problem_data[t].index = t;
		problem_data[t].val = 0;
		_STARPU_PTHREAD_MUTEX_INIT(&problem_data[t].recv_mutex, NULL);
		problem_data[t].recv_flag = 0;
		problem_data[t].neighbour = &problem_data[(t+1)%nthreads];
	}

	pthread_create(&progress_thread, NULL, progress_func, NULL);

	_STARPU_PTHREAD_MUTEX_LOCK(&data_req_mutex);
	while (!progress_thread_running)
		_STARPU_PTHREAD_COND_WAIT(&data_req_cond, &data_req_mutex);
	_STARPU_PTHREAD_MUTEX_UNLOCK(&data_req_mutex);

	for (t = 0; t < nthreads; t++)
	{
		ret = pthread_create(&problem_data[t].thread, NULL, thread_func, &problem_data[t]);
		STARPU_ASSERT(!ret);
	}

	for (t = 0; t < nthreads; t++)
	{
		ret = pthread_join(problem_data[t].thread, &retval);
		STARPU_ASSERT(!ret);
		STARPU_ASSERT(retval == NULL);
	}

	_STARPU_PTHREAD_MUTEX_LOCK(&data_req_mutex);
	progress_thread_running = 0;
	_STARPU_PTHREAD_COND_SIGNAL(&data_req_cond);
	_STARPU_PTHREAD_MUTEX_UNLOCK(&data_req_mutex);

	ret = pthread_join(progress_thread, &retval);
	STARPU_ASSERT(!ret);
	STARPU_ASSERT(retval == NULL);

	/* We check that the value in the "last" thread is valid */
	starpu_data_handle_t last_handle = problem_data[nthreads - 1].handle;
	starpu_data_acquire(last_handle, STARPU_R);
	ret = EXIT_SUCCESS;
	if (problem_data[nthreads - 1].val != (nthreads * niter))
	{
		FPRINTF(stderr, "Final value : %u should be %d\n", problem_data[nthreads - 1].val, (nthreads * niter));
		ret = EXIT_FAILURE;
	}
	starpu_data_release(last_handle);

	for (t = 0; t < nthreads; t++)
	{
		starpu_data_unregister(problem_data[t].handle);
	}

	starpu_shutdown();

	STARPU_RETURN(ret);
}
