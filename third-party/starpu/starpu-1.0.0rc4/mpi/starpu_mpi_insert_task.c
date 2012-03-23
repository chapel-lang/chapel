/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2011, 2012  Centre National de la Recherche Scientifique
 * Copyright (C) 2011-2012  Université de Bordeaux 1
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

#include <stdarg.h>
#include <mpi.h>

#include <starpu.h>
#include <starpu_data.h>
#include <common/utils.h>
#include <starpu_hash.h>
#include <common/htable32.h>
#include <util/starpu_insert_task_utils.h>
#include <datawizard/coherency.h>

//#define STARPU_MPI_VERBOSE	1
#include <starpu_mpi_private.h>

/* Whether we are allowed to keep copies of remote data. Does not work
 * yet: the sender has to know whether the receiver has it, keeping it
 * in an array indexed by node numbers. */
//#define MPI_CACHE
#include <starpu_mpi_insert_task_cache.h>

static void _starpu_mpi_tables_init()
{
        if (sent_data == NULL) {
                int nb_nodes;
		int i;

                MPI_Comm_size(MPI_COMM_WORLD, &nb_nodes);
		_STARPU_MPI_DEBUG("Initialising hash table for cache\n");
		sent_data = malloc(nb_nodes * sizeof(struct starpu_htbl32_node *));
		for(i=0 ; i<nb_nodes ; i++) sent_data[i] = NULL;
		received_data = malloc(nb_nodes * sizeof(struct starpu_htbl32_node *));
		for(i=0 ; i<nb_nodes ; i++) received_data[i] = NULL;
	}
}

void _starpu_data_deallocate(starpu_data_handle_t data_handle)
{
#ifdef STARPU_DEVEL
#warning _starpu_data_deallocate not implemented yet
#endif
}

int starpu_mpi_insert_task(MPI_Comm comm, struct starpu_codelet *codelet, ...)
{
        int arg_type;
        va_list varg_list;
        int me, do_execute, xrank, nb_nodes;
	size_t *size_on_nodes;
	size_t arg_buffer_size = 0;
	char *arg_buffer;
        int dest=0, inconsistent_execute;

        _STARPU_MPI_LOG_IN();

	MPI_Comm_rank(comm, &me);
	MPI_Comm_size(comm, &nb_nodes);

	size_on_nodes = (size_t *)calloc(1, nb_nodes * sizeof(size_t));

	_starpu_mpi_tables_init();

        /* Get the number of buffers and the size of the arguments */
	va_start(varg_list, codelet);
        arg_buffer_size = _starpu_insert_task_get_arg_size(varg_list);

	va_start(varg_list, codelet);
	_starpu_codelet_pack_args(arg_buffer_size, &arg_buffer, varg_list);

	/* Find out whether we are to execute the data because we own the data to be written to. */
        inconsistent_execute = 0;
        do_execute = -1;
	xrank = -1;
	va_start(varg_list, codelet);
	while ((arg_type = va_arg(varg_list, int)) != 0) {
		if (arg_type==STARPU_EXECUTE_ON_NODE) {
                        xrank = va_arg(varg_list, int);
			_STARPU_MPI_DEBUG("Executing on node %d\n", xrank);
			do_execute = 1;
                }
		else if (arg_type==STARPU_EXECUTE_ON_DATA) {
			starpu_data_handle_t data = va_arg(varg_list, starpu_data_handle_t);
                        xrank = starpu_data_get_rank(data);
			_STARPU_MPI_DEBUG("Executing on data node %d\n", xrank);
			STARPU_ASSERT(xrank <= nb_nodes);
			do_execute = 1;
                }
		else if (arg_type==STARPU_R || arg_type==STARPU_W || arg_type==STARPU_RW || arg_type==STARPU_SCRATCH || arg_type==STARPU_REDUX) {
                        starpu_data_handle_t data = va_arg(varg_list, starpu_data_handle_t);

                        if (data && arg_type & STARPU_R) {
				int rank = starpu_data_get_rank(data);
				struct starpu_data_interface_ops *ops;
				ops = data->ops;
				size_on_nodes[rank] += ops->get_size(data);
			}

                        if (arg_type & STARPU_W) {
                                if (!data) {
                                        /* We don't have anything allocated for this.
                                         * The application knows we won't do anything
                                         * about this task */
                                        /* Yes, the app could actually not call
                                         * insert_task at all itself, this is just a
                                         * safeguard. */
                                        _STARPU_MPI_DEBUG("oh oh\n");
                                        _STARPU_MPI_LOG_OUT();
					free(size_on_nodes);
                                        return -EINVAL;
                                }
                                int mpi_rank = starpu_data_get_rank(data);
                                if (mpi_rank == me) {
                                        if (do_execute == 0) {
                                                inconsistent_execute = 1;
                                        }
                                        else {
                                                do_execute = 1;
                                        }
                                }
                                else if (mpi_rank != -1) {
                                        if (do_execute == 1) {
                                                inconsistent_execute = 1;
                                        }
                                        else {
                                                do_execute = 0;
                                                dest = mpi_rank;
                                                /* That's the rank which needs the data to be sent to */
                                        }
                                }
                                else {
                                        _STARPU_ERROR("rank invalid\n");
                                }
                        }
                }
		else if (arg_type==STARPU_VALUE) {
			va_arg(varg_list, void *);
			va_arg(varg_list, size_t);
		}
		else if (arg_type==STARPU_CALLBACK) {
			va_arg(varg_list, void (*)(void *));
		}
		else if (arg_type==STARPU_CALLBACK_WITH_ARG) {
			va_arg(varg_list, void (*)(void *));
			va_arg(varg_list, void *);
		}
		else if (arg_type==STARPU_CALLBACK_ARG) {
			va_arg(varg_list, void *);
		}
		else if (arg_type==STARPU_PRIORITY) {
			va_arg(varg_list, int);
		}
		else if (arg_type==STARPU_EXECUTE_ON_NODE) {
			va_arg(varg_list, int);
		}
		else if (arg_type==STARPU_EXECUTE_ON_DATA) {
			va_arg(varg_list, int);
		}
	}
	va_end(varg_list);

	if (do_execute == -1) {
		int i;
		size_t max_size = 0;
		for(i=0 ; i<nb_nodes ; i++) {
			if (size_on_nodes[i] > max_size)
			{
				max_size = size_on_nodes[i];
				xrank = i;
			}
		}
		free(size_on_nodes);
		if (xrank != -1) {
			_STARPU_MPI_DEBUG("Node %d is having the most R data\n", xrank);
			do_execute = 1;
		}
	}

	STARPU_ASSERT(do_execute != -1 && "StarPU needs to see a W or a REDUX data which will tell it where to execute the task");

        if (inconsistent_execute == 1) {
                if (xrank == -1) {
                        _STARPU_MPI_DEBUG("Different tasks are owning W data. Needs to specify which one is to execute the codelet, using STARPU_EXECUTE_ON_NODE or STARPU_EXECUTE_ON_DATA\n");
			free(size_on_nodes);
			return -EINVAL;
                }
                else {
                        do_execute = (me == xrank);
                        dest = xrank;
                }
        }
	else if (xrank != -1) {
		do_execute = (me == xrank);
		dest = xrank;
	}

        /* Send and receive data as requested */
	va_start(varg_list, codelet);
	while ((arg_type = va_arg(varg_list, int)) != 0) {
		if (arg_type==STARPU_R || arg_type==STARPU_W || arg_type==STARPU_RW || arg_type == STARPU_SCRATCH) {
                        starpu_data_handle_t data = va_arg(varg_list, starpu_data_handle_t);
                        if (data && arg_type & STARPU_R) {
                                int mpi_rank = starpu_data_get_rank(data);
				int mpi_tag = starpu_data_get_tag(data);
				STARPU_ASSERT(mpi_tag >= 0 && "StarPU needs to be told the MPI rank of this data, using starpu_data_set_rank");
                                /* The task needs to read this data */
                                if (do_execute && mpi_rank != me && mpi_rank != -1) {
                                        /* I will have to execute but I don't have the data, receive */
#ifdef MPI_CACHE
                                        uint32_t key = starpu_crc32_be((uintptr_t)data, 0);
                                        void *already_received = _starpu_htbl_search_32(received_data[mpi_rank], key);
                                        if (!already_received) {
                                                _starpu_htbl_insert_32(&received_data[mpi_rank], key, data);
                                        }
                                        else {
                                                _STARPU_MPI_DEBUG("Do not receive data %p from node %d as it is already available\n", data, mpi_rank);
                                        }
                                        if (!already_received)
#endif
					{
						_STARPU_MPI_DEBUG("Receive data %p from %d\n", data, mpi_rank);
						starpu_mpi_irecv_detached(data, mpi_rank, mpi_tag, comm, NULL, NULL);
					}
                                }
                                if (!do_execute && mpi_rank == me) {
                                        /* Somebody else will execute it, and I have the data, send it. */
#ifdef MPI_CACHE
                                        uint32_t key = starpu_crc32_be((uintptr_t)data, 0);
                                        void *already_sent = _starpu_htbl_search_32(sent_data[dest], key);
                                        if (!already_sent) {
                                                _starpu_htbl_insert_32(&sent_data[dest], key, data);
                                        }
                                        else {
                                                _STARPU_MPI_DEBUG("Do not sent data %p to node %d as it has already been sent\n", data, dest);
                                        }
                                        if (!already_sent)
#endif
					{
						_STARPU_MPI_DEBUG("Send data %p to %d\n", data, dest);
						starpu_mpi_isend_detached(data, dest, mpi_tag, comm, NULL, NULL);
					}
                                }
                        }
                }
		else if (arg_type==STARPU_VALUE) {
			va_arg(varg_list, void *);
			va_arg(varg_list, size_t);
		}
		else if (arg_type==STARPU_CALLBACK) {
			va_arg(varg_list, void (*)(void *));
		}
		else if (arg_type==STARPU_CALLBACK_WITH_ARG) {
			va_arg(varg_list, void (*)(void *));
			va_arg(varg_list, void *);
		}
		else if (arg_type==STARPU_CALLBACK_ARG) {
			va_arg(varg_list, void *);
		}
		else if (arg_type==STARPU_PRIORITY) {
			va_arg(varg_list, int);
		}
		else if (arg_type==STARPU_EXECUTE_ON_NODE) {
			va_arg(varg_list, int);
		}
		else if (arg_type==STARPU_EXECUTE_ON_DATA) {
			va_arg(varg_list, starpu_data_handle_t);
		}
        }
	va_end(varg_list);

	if (do_execute) {
                _STARPU_MPI_DEBUG("Execution of the codelet %p (%s)\n", codelet, codelet->name);
                va_start(varg_list, codelet);
                struct starpu_task *task = starpu_task_create();
                int ret = _starpu_insert_task_create_and_submit(arg_buffer, codelet, &task, varg_list);
                _STARPU_MPI_DEBUG("ret: %d\n", ret);
                STARPU_ASSERT(ret==0);
        }

        if (inconsistent_execute) {
                va_start(varg_list, codelet);
                while ((arg_type = va_arg(varg_list, int)) != 0) {
                        if (arg_type==STARPU_R || arg_type==STARPU_W || arg_type==STARPU_RW || arg_type == STARPU_SCRATCH) {
                                starpu_data_handle_t data = va_arg(varg_list, starpu_data_handle_t);
                                if (arg_type & STARPU_W) {
                                        int mpi_rank = starpu_data_get_rank(data);
					int mpi_tag = starpu_data_get_tag(data);
					STARPU_ASSERT(mpi_tag >= 0 && "StarPU needs to be told the MPI rank of this data, using starpu_data_set_rank");
                                        if (mpi_rank == me) {
                                                if (xrank != -1 && me != xrank) {
                                                        _STARPU_MPI_DEBUG("Receive data %p back from the task %d which executed the codelet ...\n", data, dest);
                                                        starpu_mpi_irecv_detached(data, dest, mpi_tag, comm, NULL, NULL);
                                                }
                                        }
                                        else if (do_execute) {
                                                _STARPU_MPI_DEBUG("Send data %p back to its owner %d...\n", data, mpi_rank);
                                                starpu_mpi_isend_detached(data, mpi_rank, mpi_tag, comm, NULL, NULL);
                                        }
                                }
                        }
                        else if (arg_type==STARPU_VALUE) {
                                va_arg(varg_list, void *);
				va_arg(varg_list, size_t);
                        }
                        else if (arg_type==STARPU_CALLBACK) {
                                va_arg(varg_list, void (*)(void *));
                        }
                        else if (arg_type==STARPU_CALLBACK_WITH_ARG) {
                                va_arg(varg_list, void (*)(void *));
                                va_arg(varg_list, void *);
                        }
                        else if (arg_type==STARPU_CALLBACK_ARG) {
                                va_arg(varg_list, void *);
                        }
                        else if (arg_type==STARPU_PRIORITY) {
                                va_arg(varg_list, int);
                        }
                        else if (arg_type==STARPU_EXECUTE_ON_NODE) {
                                va_arg(varg_list, int);
                        }
                        else if (arg_type==STARPU_EXECUTE_ON_DATA) {
                                va_arg(varg_list, starpu_data_handle_t);
                        }
                }
                va_end(varg_list);
        }

	va_start(varg_list, codelet);
	while ((arg_type = va_arg(varg_list, int)) != 0) {
		if (arg_type==STARPU_R || arg_type==STARPU_W || arg_type==STARPU_RW || arg_type == STARPU_SCRATCH) {
                        starpu_data_handle_t data = va_arg(varg_list, starpu_data_handle_t);
#ifdef MPI_CACHE
                        if (arg_type & STARPU_W) {
                                uint32_t key = starpu_crc32_be((uintptr_t)data, 0);
                                if (do_execute) {
                                        /* Note that all copies I've sent to neighbours are now invalid */
                                        int n, size;
                                        MPI_Comm_size(comm, &size);
                                        for(n=0 ; n<size ; n++) {
                                                void *already_sent = _starpu_htbl_search_32(sent_data[n], key);
                                                if (already_sent) {
                                                        _STARPU_MPI_DEBUG("Posting request to clear send cache for data %p\n", data);
                                                        _starpu_mpi_clear_cache_request(data, n, _STARPU_MPI_CLEAR_SENT_DATA);
                                                }
                                        }
                                }
                                else {
                                        int mpi_rank = starpu_data_get_rank(data);
                                        void *already_received = _starpu_htbl_search_32(received_data[mpi_rank], key);
                                        if (already_received) {
                                                /* Somebody else will write to the data, so discard our cached copy if any */
                                                /* TODO: starpu_mpi could just remember itself. */
                                                _STARPU_MPI_DEBUG("Posting request to clear receive cache for data %p\n", data);
                                                _starpu_mpi_clear_cache_request(data, mpi_rank, _STARPU_MPI_CLEAR_RECEIVED_DATA);
                                                _starpu_data_deallocate(data);
                                        }
                                }
                        }
#else
                        /* We allocated a temporary buffer for the received data, now drop it */
                        if ((arg_type & STARPU_R) && do_execute) {
                                int mpi_rank = starpu_data_get_rank(data);
                                if (mpi_rank != me && mpi_rank != -1) {
                                        _starpu_data_deallocate(data);
                                }
                        }
#endif
                }
		else if (arg_type==STARPU_VALUE) {
			va_arg(varg_list, void *);
			va_arg(varg_list, size_t);
		}
		else if (arg_type==STARPU_CALLBACK) {
			va_arg(varg_list, void (*)(void *));
		}
		else if (arg_type==STARPU_CALLBACK_WITH_ARG) {
			va_arg(varg_list, void (*)(void *));
			va_arg(varg_list, void *);
		}
		else if (arg_type==STARPU_CALLBACK_ARG) {
			va_arg(varg_list, void *);
		}
		else if (arg_type==STARPU_PRIORITY) {
			va_arg(varg_list, int);
		}
		else if (arg_type==STARPU_EXECUTE_ON_NODE) {
			va_arg(varg_list, int);
		}
		else if (arg_type==STARPU_EXECUTE_ON_DATA) {
			va_arg(varg_list, starpu_data_handle_t);
		}
        }
	va_end(varg_list);
        _STARPU_MPI_LOG_OUT();
        return 0;
}

void starpu_mpi_get_data_on_node_detached(MPI_Comm comm, starpu_data_handle_t data_handle, int node, void (*callback)(void*), void *arg)
{
        int me, rank, tag;

        rank = starpu_data_get_rank(data_handle);
        tag = starpu_data_get_tag(data_handle);
	MPI_Comm_rank(comm, &me);

        if (node == rank) return;

        if (me == node)
        {
		starpu_mpi_irecv_detached(data_handle, rank, tag, comm, callback, arg);
        }
        else if (me == rank)
        {
		starpu_mpi_isend_detached(data_handle, node, tag, comm, NULL, NULL);
        }
}

void starpu_mpi_get_data_on_node(MPI_Comm comm, starpu_data_handle_t data_handle, int node)
{
        int me, rank, tag;

        rank = starpu_data_get_rank(data_handle);
        tag = starpu_data_get_tag(data_handle);
	MPI_Comm_rank(comm, &me);

        if (node == rank) return;

        if (me == node)
        {
                MPI_Status status;
                starpu_mpi_recv(data_handle, rank, tag, comm, &status);
        }
        else if (me == rank)
        {
                starpu_mpi_send(data_handle, node, tag, comm);
        }
}

void starpu_mpi_redux_data(MPI_Comm comm, starpu_data_handle_t data_handle)
{
        int me, rank, tag, nb_nodes;

        rank = starpu_data_get_rank(data_handle);
        tag = starpu_data_get_tag(data_handle);

	MPI_Comm_rank(comm, &me);
	MPI_Comm_size(comm, &nb_nodes);

	_STARPU_MPI_DEBUG("Doing reduction for data %p on node %d with %d nodes ...\n", data_handle, rank, nb_nodes);

	// need to count how many nodes have the data in redux mode
	if (me == rank) {
		int i;

		for(i=0 ; i<nb_nodes ; i++) {
			if (i != rank) {
				starpu_data_handle_t new_handle;

				starpu_data_register_same(&new_handle, data_handle);

				_STARPU_MPI_DEBUG("Receiving redux handle from %d in %p ...\n", i, new_handle);

				starpu_mpi_irecv_detached(new_handle, i, tag, comm, NULL, NULL);
				starpu_insert_task(data_handle->redux_cl,
						   STARPU_RW, data_handle,
						   STARPU_R, new_handle,
						   0);
			}
		}
	}
	else {
		_STARPU_MPI_DEBUG("Sending redux handle to %d ...\n", rank);
		starpu_mpi_isend_detached(data_handle, rank, tag, comm, NULL, NULL);
	}
}
