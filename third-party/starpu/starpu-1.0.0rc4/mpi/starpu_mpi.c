/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2009, 2010-2012  Université de Bordeaux 1
 * Copyright (C) 2010, 2011  Centre National de la Recherche Scientifique
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

#include <stdlib.h>
#include <starpu_mpi.h>
#include <starpu_mpi_datatype.h>
//#define STARPU_MPI_VERBOSE	1
#include <starpu_mpi_private.h>
#include <starpu_profiling.h>

/* TODO find a better way to select the polling method (perhaps during the
 * configuration) */
//#define USE_STARPU_ACTIVITY	1

static void submit_mpi_req(void *arg);
static void handle_request_termination(struct _starpu_mpi_req *req);

/* The list of requests that have been newly submitted by the application */
static struct _starpu_mpi_req_list *new_requests;

/* The list of detached requests that have already been submitted to MPI */
static struct _starpu_mpi_req_list *detached_requests;
static pthread_mutex_t detached_requests_mutex;

/* Condition to wake up progression thread */
static pthread_cond_t cond_progression;
/* Condition to wake up waiting for all current MPI requests to finish */
static pthread_cond_t cond_finished;
static pthread_mutex_t mutex;
static pthread_t progress_thread;
static int running = 0;

/* Count requests posted by the application and not yet submitted to MPI, i.e pushed into the new_requests list */
static pthread_mutex_t mutex_posted_requests;
static int posted_requests = 0, newer_requests, barrier_running = 0;

#define INC_POSTED_REQUESTS(value) { _STARPU_PTHREAD_MUTEX_LOCK(&mutex_posted_requests); posted_requests += value; _STARPU_PTHREAD_MUTEX_UNLOCK(&mutex_posted_requests); }

/*
 *	Isend
 */

static void starpu_mpi_isend_func(struct _starpu_mpi_req *req)
{
        _STARPU_MPI_LOG_IN();
	void *ptr = starpu_mpi_handle_to_ptr(req->data_handle);

        _STARPU_MPI_DEBUG("post MPI isend tag %d dst %d ptr %p req %p\n", req->mpi_tag, req->srcdst, ptr, &req->request);

	starpu_mpi_handle_to_datatype(req->data_handle, &req->datatype);

        req->ret = MPI_Isend(ptr, 1, req->datatype, req->srcdst, req->mpi_tag, req->comm, &req->request);
        STARPU_ASSERT(req->ret == MPI_SUCCESS);

	TRACE_MPI_ISEND(req->srcdst, req->mpi_tag, 0);

	/* somebody is perhaps waiting for the MPI request to be posted */
	_STARPU_PTHREAD_MUTEX_LOCK(&req->req_mutex);
	req->submitted = 1;
	_STARPU_PTHREAD_COND_BROADCAST(&req->req_cond);
	_STARPU_PTHREAD_MUTEX_UNLOCK(&req->req_mutex);
        _STARPU_MPI_LOG_OUT();
}

static struct _starpu_mpi_req *_starpu_mpi_isend_common(starpu_data_handle_t data_handle,
							int dest, int mpi_tag, MPI_Comm comm,
							unsigned detached, void (*callback)(void *), void *arg)
{
	struct _starpu_mpi_req *req = calloc(1, sizeof(struct _starpu_mpi_req));
	STARPU_ASSERT(req);

        _STARPU_MPI_LOG_IN();

        INC_POSTED_REQUESTS(1);

	/* Initialize the request structure */
	req->submitted = 0;
	req->completed = 0;
	_STARPU_PTHREAD_MUTEX_INIT(&req->req_mutex, NULL);
	_STARPU_PTHREAD_COND_INIT(&req->req_cond, NULL);

	req->request_type = SEND_REQ;

	req->data_handle = data_handle;
	req->srcdst = dest;
	req->mpi_tag = mpi_tag;
	req->comm = comm;
	req->func = starpu_mpi_isend_func;

	req->detached = detached;
	req->callback = callback;
	req->callback_arg = arg;

	/* Asynchronously request StarPU to fetch the data in main memory: when
	 * it is available in main memory, submit_mpi_req(req) is called and
	 * the request is actually submitted  */
	starpu_data_acquire_cb(data_handle, STARPU_R, submit_mpi_req, (void *)req);

        _STARPU_MPI_LOG_OUT();
	return req;
}

int starpu_mpi_isend(starpu_data_handle_t data_handle, starpu_mpi_req *public_req, int dest, int mpi_tag, MPI_Comm comm)
{
        _STARPU_MPI_LOG_IN();
	STARPU_ASSERT(public_req);

	struct _starpu_mpi_req *req;
	req = _starpu_mpi_isend_common(data_handle, dest, mpi_tag, comm, 0, NULL, NULL);

	STARPU_ASSERT(req);
	*public_req = req;

        _STARPU_MPI_LOG_OUT();
	return 0;
}

/*
 *	Isend (detached)
 */

int starpu_mpi_isend_detached(starpu_data_handle_t data_handle,
				int dest, int mpi_tag, MPI_Comm comm, void (*callback)(void *), void *arg)
{
        _STARPU_MPI_LOG_IN();
	_starpu_mpi_isend_common(data_handle, dest, mpi_tag, comm, 1, callback, arg);

        _STARPU_MPI_LOG_OUT();
	return 0;
}

/*
 *	Irecv
 */

static void starpu_mpi_irecv_func(struct _starpu_mpi_req *req)
{
        _STARPU_MPI_LOG_IN();
	void *ptr = starpu_mpi_handle_to_ptr(req->data_handle);
	STARPU_ASSERT(ptr);

	starpu_mpi_handle_to_datatype(req->data_handle, &req->datatype);

	_STARPU_MPI_DEBUG("post MPI irecv tag %d src %d data %p ptr %p req %p datatype %d\n", req->mpi_tag, req->srcdst, req->data_handle, ptr, &req->request, (int)req->datatype);

        req->ret = MPI_Irecv(ptr, 1, req->datatype, req->srcdst, req->mpi_tag, req->comm, &req->request);
        STARPU_ASSERT(req->ret == MPI_SUCCESS);

	/* somebody is perhaps waiting for the MPI request to be posted */
	_STARPU_PTHREAD_MUTEX_LOCK(&req->req_mutex);
	req->submitted = 1;
	_STARPU_PTHREAD_COND_BROADCAST(&req->req_cond);
	_STARPU_PTHREAD_MUTEX_UNLOCK(&req->req_mutex);
        _STARPU_MPI_LOG_OUT();
}

static struct _starpu_mpi_req *_starpu_mpi_irecv_common(starpu_data_handle_t data_handle, int source, int mpi_tag, MPI_Comm comm, unsigned detached, void (*callback)(void *), void *arg)
{
        _STARPU_MPI_LOG_IN();
	struct _starpu_mpi_req *req = calloc(1, sizeof(struct _starpu_mpi_req));
	STARPU_ASSERT(req);

        INC_POSTED_REQUESTS(1);

	/* Initialize the request structure */
	req->submitted = 0;
	_STARPU_PTHREAD_MUTEX_INIT(&req->req_mutex, NULL);
	_STARPU_PTHREAD_COND_INIT(&req->req_cond, NULL);

	req->request_type = RECV_REQ;

	req->data_handle = data_handle;
	req->srcdst = source;
	req->mpi_tag = mpi_tag;
	req->comm = comm;

	req->detached = detached;
	req->callback = callback;
	req->callback_arg = arg;

	req->func = starpu_mpi_irecv_func;

	/* Asynchronously request StarPU to fetch the data in main memory: when
	 * it is available in main memory, submit_mpi_req(req) is called and
	 * the request is actually submitted  */
	starpu_data_acquire_cb(data_handle, STARPU_W, submit_mpi_req, (void *)req);

        _STARPU_MPI_LOG_OUT();
	return req;
}

int starpu_mpi_irecv(starpu_data_handle_t data_handle, starpu_mpi_req *public_req, int source, int mpi_tag, MPI_Comm comm)
{
        _STARPU_MPI_LOG_IN();
	STARPU_ASSERT(public_req);

	struct _starpu_mpi_req *req;
	req = _starpu_mpi_irecv_common(data_handle, source, mpi_tag, comm, 0, NULL, NULL);

	STARPU_ASSERT(req);
	*public_req = req;

        _STARPU_MPI_LOG_OUT();
	return 0;
}

/*
 *	Irecv (detached)
 */

int starpu_mpi_irecv_detached(starpu_data_handle_t data_handle, int source, int mpi_tag, MPI_Comm comm, void (*callback)(void *), void *arg)
{
        _STARPU_MPI_LOG_IN();
	_starpu_mpi_irecv_common(data_handle, source, mpi_tag, comm, 1, callback, arg);

        _STARPU_MPI_LOG_OUT();
	return 0;
}


/*
 *	Recv
 */

int starpu_mpi_recv(starpu_data_handle_t data_handle, int source, int mpi_tag, MPI_Comm comm, MPI_Status *status)
{
	starpu_mpi_req req;

        _STARPU_MPI_LOG_IN();
	starpu_mpi_irecv(data_handle, &req, source, mpi_tag, comm);
	starpu_mpi_wait(&req, status);

        _STARPU_MPI_LOG_OUT();
	return 0;
}

/*
 *	Send
 */

int starpu_mpi_send(starpu_data_handle_t data_handle, int dest, int mpi_tag, MPI_Comm comm)
{
	starpu_mpi_req req;
	MPI_Status status;

        _STARPU_MPI_LOG_IN();
	memset(&status, 0, sizeof(MPI_Status));

	starpu_mpi_isend(data_handle, &req, dest, mpi_tag, comm);
	starpu_mpi_wait(&req, &status);

        _STARPU_MPI_LOG_OUT();
	return 0;
}

/*
 *	Wait
 */

static void starpu_mpi_wait_func(struct _starpu_mpi_req *waiting_req)
{
        _STARPU_MPI_LOG_IN();
	/* Which is the mpi request we are waiting for ? */
	struct _starpu_mpi_req *req = waiting_req->other_request;

	req->ret = MPI_Wait(&req->request, waiting_req->status);
        STARPU_ASSERT(req->ret == MPI_SUCCESS);

	handle_request_termination(req);
        _STARPU_MPI_LOG_OUT();
}

int starpu_mpi_wait(starpu_mpi_req *public_req, MPI_Status *status)
{
        _STARPU_MPI_LOG_IN();
	int ret;
	struct _starpu_mpi_req *waiting_req = calloc(1, sizeof(struct _starpu_mpi_req));
	STARPU_ASSERT(waiting_req);
	struct _starpu_mpi_req *req = *public_req;

        INC_POSTED_REQUESTS(1);

	/* We cannot try to complete a MPI request that was not actually posted
	 * to MPI yet. */
	_STARPU_PTHREAD_MUTEX_LOCK(&(req->req_mutex));
	while (!(req->submitted))
		_STARPU_PTHREAD_COND_WAIT(&(req->req_cond), &(req->req_mutex));
	_STARPU_PTHREAD_MUTEX_UNLOCK(&(req->req_mutex));

	/* Initialize the request structure */
	_STARPU_PTHREAD_MUTEX_INIT(&(waiting_req->req_mutex), NULL);
	_STARPU_PTHREAD_COND_INIT(&(waiting_req->req_cond), NULL);
	waiting_req->status = status;
	waiting_req->other_request = req;
	waiting_req->func = starpu_mpi_wait_func;
	waiting_req->request_type = WAIT_REQ;

	submit_mpi_req(waiting_req);

	/* We wait for the MPI request to finish */
	_STARPU_PTHREAD_MUTEX_LOCK(&req->req_mutex);
	while (!req->completed)
		_STARPU_PTHREAD_COND_WAIT(&req->req_cond, &req->req_mutex);
	_STARPU_PTHREAD_MUTEX_UNLOCK(&req->req_mutex);

	ret = req->ret;

	/* The internal request structure was automatically allocated */
	*public_req = NULL;
	free(req);

        //free(waiting_req);
        _STARPU_MPI_LOG_OUT();
	return ret;
}

/*
 * 	Test
 */

static void starpu_mpi_test_func(struct _starpu_mpi_req *testing_req)
{
        _STARPU_MPI_LOG_IN();
	/* Which is the mpi request we are testing for ? */
	struct _starpu_mpi_req *req = testing_req->other_request;

        _STARPU_MPI_DEBUG("Test request %p - mpitag %d - TYPE %s %d\n", &req->request, req->mpi_tag, (req->request_type == RECV_REQ)?"recv : source":"send : dest", req->srcdst);
	req->ret = MPI_Test(&req->request, testing_req->flag, testing_req->status);
        STARPU_ASSERT(req->ret == MPI_SUCCESS);

	if (*testing_req->flag)
	{
		testing_req->ret = req->ret;
		handle_request_termination(req);
	}

	_STARPU_PTHREAD_MUTEX_LOCK(&testing_req->req_mutex);
	testing_req->completed = 1;
	_STARPU_PTHREAD_COND_SIGNAL(&testing_req->req_cond);
	_STARPU_PTHREAD_MUTEX_UNLOCK(&testing_req->req_mutex);
        _STARPU_MPI_LOG_OUT();
}

int starpu_mpi_test(starpu_mpi_req *public_req, int *flag, MPI_Status *status)
{
        _STARPU_MPI_LOG_IN();
	int ret = 0;

	STARPU_ASSERT(public_req);

	struct _starpu_mpi_req *req = *public_req;

	STARPU_ASSERT(!req->detached);

	_STARPU_PTHREAD_MUTEX_LOCK(&req->req_mutex);
	unsigned submitted = req->submitted;
	_STARPU_PTHREAD_MUTEX_UNLOCK(&req->req_mutex);

	if (submitted)
	{
		struct _starpu_mpi_req *testing_req = calloc(1, sizeof(struct _starpu_mpi_req));
                STARPU_ASSERT(testing_req);
                //		memset(testing_req, 0, sizeof(struct _starpu_mpi_req));

		/* Initialize the request structure */
		_STARPU_PTHREAD_MUTEX_INIT(&(testing_req->req_mutex), NULL);
		_STARPU_PTHREAD_COND_INIT(&(testing_req->req_cond), NULL);
		testing_req->flag = flag;
		testing_req->status = status;
		testing_req->other_request = req;
		testing_req->func = starpu_mpi_test_func;
		testing_req->completed = 0;
                testing_req->request_type = TEST_REQ;

                INC_POSTED_REQUESTS(1);
                submit_mpi_req(testing_req);

		/* We wait for the test request to finish */
		_STARPU_PTHREAD_MUTEX_LOCK(&(testing_req->req_mutex));
		while (!(testing_req->completed))
                        _STARPU_PTHREAD_COND_WAIT(&(testing_req->req_cond), &(testing_req->req_mutex));
		_STARPU_PTHREAD_MUTEX_UNLOCK(&(testing_req->req_mutex));

		ret = testing_req->ret;

		if (*(testing_req->flag))
		{
			/* The request was completed so we free the internal
			 * request structure which was automatically allocated
			 * */
			*public_req = NULL;
			free(req);
		}
	}
	else {
		*flag = 0;
	}

        _STARPU_MPI_LOG_OUT();
	return ret;
}

/*
 *	Barrier
 */

static void starpu_mpi_barrier_func(struct _starpu_mpi_req *barrier_req)
{
        _STARPU_MPI_LOG_IN();

	barrier_req->ret = MPI_Barrier(barrier_req->comm);
        STARPU_ASSERT(barrier_req->ret == MPI_SUCCESS);

	handle_request_termination(barrier_req);
        _STARPU_MPI_LOG_OUT();
}

int starpu_mpi_barrier(MPI_Comm comm)
{
        _STARPU_MPI_LOG_IN();
	int ret;
	struct _starpu_mpi_req *barrier_req = calloc(1, sizeof(struct _starpu_mpi_req));
	STARPU_ASSERT(barrier_req);

	/* First wait for *both* all tasks and MPI requests to finish, in case
	 * some tasks generate MPI requests, MPI requests generate tasks, etc.
	 */
	_STARPU_PTHREAD_MUTEX_LOCK(&mutex);
	STARPU_ASSERT_MSG(!barrier_running, "Concurrent starpu_mpi_barrier is not implemented, even on different communicators");
	barrier_running = 1;
	do {
		while (posted_requests)
			/* Wait for all current MPI requests to finish */
			_STARPU_PTHREAD_COND_WAIT(&cond_finished, &mutex);
		/* No current request, clear flag */
		newer_requests = 0;
		_STARPU_PTHREAD_MUTEX_UNLOCK(&mutex);
		/* Now wait for all tasks */
		starpu_task_wait_for_all();
		_STARPU_PTHREAD_MUTEX_LOCK(&mutex);
		/* Check newer_requests again, in case some MPI requests
		 * triggered by tasks completed and triggered tasks between
		 * wait_for_all finished and we take the lock */
	} while (posted_requests || newer_requests);
	barrier_running = 0;
	_STARPU_PTHREAD_MUTEX_UNLOCK(&mutex);

	/* Initialize the request structure */
	_STARPU_PTHREAD_MUTEX_INIT(&(barrier_req->req_mutex), NULL);
	_STARPU_PTHREAD_COND_INIT(&(barrier_req->req_cond), NULL);
	barrier_req->func = starpu_mpi_barrier_func;
	barrier_req->request_type = BARRIER_REQ;
	barrier_req->comm = comm;

        INC_POSTED_REQUESTS(1);
	submit_mpi_req(barrier_req);

	/* We wait for the MPI request to finish */
	_STARPU_PTHREAD_MUTEX_LOCK(&barrier_req->req_mutex);
	while (!barrier_req->completed)
		_STARPU_PTHREAD_COND_WAIT(&barrier_req->req_cond, &barrier_req->req_mutex);
	_STARPU_PTHREAD_MUTEX_UNLOCK(&barrier_req->req_mutex);

	ret = barrier_req->ret;

        //free(waiting_req);
        _STARPU_MPI_LOG_OUT();
	return ret;
}

/*
 *	Requests
 */

#ifdef STARPU_MPI_VERBOSE
static char *starpu_mpi_request_type(unsigned request_type)
{
        switch (request_type)
                {
                case SEND_REQ: return "send";
                case RECV_REQ: return "recv";
                case WAIT_REQ: return "wait";
                case TEST_REQ: return "test";
                case BARRIER_REQ: return "barrier";
                default: return "unknown request type";
                }
}
#endif

static void handle_request_termination(struct _starpu_mpi_req *req)
{
        _STARPU_MPI_LOG_IN();

	_STARPU_MPI_DEBUG("complete MPI (%s %d) data %p req %p - tag %d\n", starpu_mpi_request_type(req->request_type), req->srcdst, req->data_handle, &req->request, req->mpi_tag);
        if (req->request_type != BARRIER_REQ) {
                MPI_Type_free(&req->datatype);
                starpu_data_release(req->data_handle);
        }


	if (req->request_type == RECV_REQ)
	{
		TRACE_MPI_IRECV_END(req->srcdst, req->mpi_tag);
	}

	/* Execute the specified callback, if any */
	if (req->callback)
		req->callback(req->callback_arg);

	/* tell anyone potentiallly waiting on the request that it is
	 * terminated now */
	_STARPU_PTHREAD_MUTEX_LOCK(&req->req_mutex);
	req->completed = 1;
	_STARPU_PTHREAD_COND_BROADCAST(&req->req_cond);
	_STARPU_PTHREAD_MUTEX_UNLOCK(&req->req_mutex);
        _STARPU_MPI_LOG_OUT();
}

static void submit_mpi_req(void *arg)
{
        _STARPU_MPI_LOG_IN();
	struct _starpu_mpi_req *req = arg;

        INC_POSTED_REQUESTS(-1);

	_STARPU_PTHREAD_MUTEX_LOCK(&mutex);
	_starpu_mpi_req_list_push_front(new_requests, req);
	newer_requests = 1;
        _STARPU_MPI_DEBUG("Pushing new request type %d\n", req->request_type);
	_STARPU_PTHREAD_COND_BROADCAST(&cond_progression);
	_STARPU_PTHREAD_MUTEX_UNLOCK(&mutex);
        _STARPU_MPI_LOG_OUT();
}

/*
 *	Scheduler hook
 */

#ifdef USE_STARPU_ACTIVITY
static unsigned progression_hook_func(void *arg __attribute__((unused)))
{
	unsigned may_block = 1;

	_STARPU_PTHREAD_MUTEX_LOCK(&mutex);
	if (!_starpu_mpi_req_list_empty(detached_requests))
	{
		_STARPU_PTHREAD_COND_SIGNAL(&cond_progression);
		may_block = 0;
	}
	_STARPU_PTHREAD_MUTEX_UNLOCK(&mutex);

	return may_block;
}
#endif

/*
 *	Progression loop
 */

static void test_detached_requests(void)
{
        _STARPU_MPI_LOG_IN();
	int flag;
	MPI_Status status;
	struct _starpu_mpi_req *req, *next_req;

	_STARPU_PTHREAD_MUTEX_LOCK(&detached_requests_mutex);

	for (req = _starpu_mpi_req_list_begin(detached_requests);
		req != _starpu_mpi_req_list_end(detached_requests);
		req = next_req)
	{
		next_req = _starpu_mpi_req_list_next(req);

		_STARPU_PTHREAD_MUTEX_UNLOCK(&detached_requests_mutex);

                //_STARPU_MPI_DEBUG("Test detached request %p - mpitag %d - TYPE %s %d\n", &req->request, req->mpi_tag, (req->request_type == RECV_REQ)?"recv : source":"send : dest", req->srcdst);
		req->ret = MPI_Test(&req->request, &flag, &status);
		STARPU_ASSERT(req->ret == MPI_SUCCESS);

		if (flag)
		{
			handle_request_termination(req);
		}

		_STARPU_PTHREAD_MUTEX_LOCK(&detached_requests_mutex);

		if (flag)
			_starpu_mpi_req_list_erase(detached_requests, req);

#ifdef STARPU_DEVEL
#warning TODO fix memleak
#endif
		/* Detached requests are automatically allocated by the lib */
		//if (req->detached)
		//	free(req);
	}

	_STARPU_PTHREAD_MUTEX_UNLOCK(&detached_requests_mutex);
        _STARPU_MPI_LOG_OUT();
}

static void handle_new_request(struct _starpu_mpi_req *req)
{
        _STARPU_MPI_LOG_IN();
	STARPU_ASSERT(req);

	/* submit the request to MPI */
        _STARPU_MPI_DEBUG("Handling new request type %d\n", req->request_type);
	req->func(req);

	if (req->detached)
	{
		_STARPU_PTHREAD_MUTEX_LOCK(&mutex);
		_starpu_mpi_req_list_push_front(detached_requests, req);
		_STARPU_PTHREAD_MUTEX_UNLOCK(&mutex);

		starpu_wake_all_blocked_workers();

		/* put the submitted request into the list of pending requests
		 * so that it can be handled by the progression mechanisms */
		_STARPU_PTHREAD_MUTEX_LOCK(&mutex);
		_STARPU_PTHREAD_COND_SIGNAL(&cond_progression);
		_STARPU_PTHREAD_MUTEX_UNLOCK(&mutex);
	}
        _STARPU_MPI_LOG_OUT();
}

static void *progress_thread_func(void *arg)
{
        int initialize_mpi = *((int *) arg);

        _STARPU_DEBUG("Initialize mpi: %d\n", initialize_mpi);

        if (initialize_mpi) {
#ifdef STARPU_DEVEL
#warning get real argc and argv from the application
#endif
                int argc = 0;
                char **argv = NULL;
                int thread_support;
                _STARPU_DEBUG("Calling MPI_Init_thread\n");
                if (MPI_Init_thread(&argc, &argv, MPI_THREAD_SERIALIZED, &thread_support) != MPI_SUCCESS) {
                        fprintf(stderr,"MPI_Init_thread failed\n");
                        exit(1);
                }
                if (thread_support == MPI_THREAD_FUNNELED)
                        fprintf(stderr,"Warning: MPI only has funneled thread support, not serialized, hoping this will work\n");
                if (thread_support < MPI_THREAD_FUNNELED)
                        fprintf(stderr,"Warning: MPI does not have thread support!\n");
        }

	/* notify the main thread that the progression thread is ready */
	_STARPU_PTHREAD_MUTEX_LOCK(&mutex);
	running = 1;
	_STARPU_PTHREAD_COND_SIGNAL(&cond_progression);
	_STARPU_PTHREAD_MUTEX_UNLOCK(&mutex);

	_STARPU_PTHREAD_MUTEX_LOCK(&mutex);
	while (running || posted_requests || !(_starpu_mpi_req_list_empty(new_requests)) || !(_starpu_mpi_req_list_empty(detached_requests))) {
		/* shall we block ? */
		unsigned block = _starpu_mpi_req_list_empty(new_requests);

#ifndef USE_STARPU_ACTIVITY
		block = block && _starpu_mpi_req_list_empty(detached_requests);
#endif

		if (block)
		{
                        _STARPU_MPI_DEBUG("NO MORE REQUESTS TO HANDLE\n");
			if (barrier_running)
				/* Tell mpi_barrier */
				_STARPU_PTHREAD_COND_SIGNAL(&cond_finished);
			_STARPU_PTHREAD_COND_WAIT(&cond_progression, &mutex);
		}

		/* test whether there are some terminated "detached request" */
		_STARPU_PTHREAD_MUTEX_UNLOCK(&mutex);
		test_detached_requests();
		_STARPU_PTHREAD_MUTEX_LOCK(&mutex);

		/* get one request */
		struct _starpu_mpi_req *req;
		while (!_starpu_mpi_req_list_empty(new_requests))
		{
			req = _starpu_mpi_req_list_pop_back(new_requests);

			/* handling a request is likely to block for a while
			 * (on a sync_data_with_mem call), we want to let the
			 * application submit requests in the meantime, so we
			 * release the lock.  */
			_STARPU_PTHREAD_MUTEX_UNLOCK(&mutex);
			handle_new_request(req);
			_STARPU_PTHREAD_MUTEX_LOCK(&mutex);
		}
	}

	STARPU_ASSERT(_starpu_mpi_req_list_empty(detached_requests));
	STARPU_ASSERT(_starpu_mpi_req_list_empty(new_requests));
        STARPU_ASSERT(posted_requests == 0);

        if (initialize_mpi) {
                _STARPU_MPI_DEBUG("Calling MPI_Finalize()\n");
                MPI_Finalize();
        }

	_STARPU_PTHREAD_MUTEX_UNLOCK(&mutex);

	return NULL;
}

/*
 *	(De)Initialization methods
 */

#ifdef USE_STARPU_ACTIVITY
static int hookid = - 1;
#endif

static void _starpu_mpi_add_sync_point_in_fxt(void)
{
#ifdef STARPU_USE_FXT
	int rank;
	int worldsize;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &worldsize);

	int barrier_ret = MPI_Barrier(MPI_COMM_WORLD);
	STARPU_ASSERT(barrier_ret == MPI_SUCCESS);

	/* We generate a "unique" key so that we can make sure that different
	 * FxT traces come from the same MPI run. */
	int random_number;

	/* XXX perhaps we don't want to generate a new seed if the application
	 * specified some reproductible behaviour ? */
	if (rank == 0)
	{
		srand(time(NULL));
		random_number = rand();
	}

	MPI_Bcast(&random_number, 1, MPI_INT, 0, MPI_COMM_WORLD);

	TRACE_MPI_BARRIER(rank, worldsize, random_number);

        _STARPU_MPI_DEBUG("unique key %x\n", random_number);
#endif
}

static
int _starpu_mpi_initialize(int initialize_mpi, int *rank, int *world_size)
{
	_STARPU_PTHREAD_MUTEX_INIT(&mutex, NULL);
	_STARPU_PTHREAD_COND_INIT(&cond_progression, NULL);
	_STARPU_PTHREAD_COND_INIT(&cond_finished, NULL);
	new_requests = _starpu_mpi_req_list_new();

	_STARPU_PTHREAD_MUTEX_INIT(&detached_requests_mutex, NULL);
	detached_requests = _starpu_mpi_req_list_new();

        _STARPU_PTHREAD_MUTEX_INIT(&mutex_posted_requests, NULL);

	pthread_create(&progress_thread, NULL, progress_thread_func, (void *)&initialize_mpi);

	_STARPU_PTHREAD_MUTEX_LOCK(&mutex);
	while (!running)
		_STARPU_PTHREAD_COND_WAIT(&cond_progression, &mutex);
	_STARPU_PTHREAD_MUTEX_UNLOCK(&mutex);

        if (rank && world_size) {
                _STARPU_DEBUG("Calling MPI_Comm_rank\n");
                MPI_Comm_rank(MPI_COMM_WORLD, rank);
                MPI_Comm_size(MPI_COMM_WORLD, world_size);
        }

#ifdef STARPU_USE_FXT
	int prank;
	MPI_Comm_rank(MPI_COMM_WORLD, &prank);
	starpu_set_profiling_id(prank);
#endif //STARPU_USE_FXT

#ifdef USE_STARPU_ACTIVITY
	hookid = starpu_progression_hook_register(progression_hook_func, NULL);
	STARPU_ASSERT(hookid >= 0);
#endif

	_starpu_mpi_add_sync_point_in_fxt();

	return 0;
}

int starpu_mpi_initialize(void)
{
        return _starpu_mpi_initialize(0, NULL, NULL);
}

int starpu_mpi_initialize_extended(int *rank, int *world_size)
{
        return _starpu_mpi_initialize(1, rank, world_size);
}

int starpu_mpi_shutdown(void)
{
	void *value;

	/* kill the progression thread */
	_STARPU_PTHREAD_MUTEX_LOCK(&mutex);
	running = 0;
	_STARPU_PTHREAD_COND_BROADCAST(&cond_progression);
	_STARPU_PTHREAD_MUTEX_UNLOCK(&mutex);

	pthread_join(progress_thread, &value);

#ifdef USE_STARPU_ACTIVITY
	starpu_progression_hook_deregister(hookid);
#endif

	/* free the request queues */
	_starpu_mpi_req_list_delete(detached_requests);
	_starpu_mpi_req_list_delete(new_requests);

	return 0;
}

