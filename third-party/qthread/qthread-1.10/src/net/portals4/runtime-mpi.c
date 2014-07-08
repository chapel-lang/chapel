/* -*- C -*-
 *
 * Copyright 2011 Sandia Corporation. Under the terms of Contract
 * DE-AC04-94AL85000 with Sandia Corporation, the U.S.  Government
 * retains certain rights in this software.
 *
 * Run-time support for the built-in runtime that is part of the
 * shared memory implementation Portals
 */

#include "config.h"

#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include <portals4.h>

#include "runtime.h"
#include "qt_debug.h" /* for malloc debug wrappers */

static int rank = 0;
static int size = 0;

int
qthread_internal_net_driver_runtime_init(void)
{
    int ret;

    MPI_Initialized(&ret);
    if (!ret) {
        if (MPI_SUCCESS != MPI_Init(NULL, NULL)) {
            return 1;
        }
    }

    if (MPI_SUCCESS != MPI_Comm_size(MPI_COMM_WORLD, &size)) {
        return 1;
    }

    if (MPI_SUCCESS != MPI_Comm_rank(MPI_COMM_WORLD, &rank)) {
        return 1;
    }

    return 0;
}


int
qthread_internal_net_driver_runtime_fini(void)
{
    int ret;

    qthread_debug(MULTINODE_DETAILS, "begin call to MPI_Finalize(d)\n", ret);

    MPI_Finalized(&ret);
    if (!ret) {
        MPI_Finalize();
    }

    qthread_debug(MULTINODE_DETAILS, "end call to MPI_Finalize(d)\n", ret);

    return 0;
}


ptl_process_t*
qthread_internal_net_driver_runtime_get_mapping(ptl_handle_ni_t ni_h)
{
    static ptl_process_t *mapping = NULL;
    ptl_process_t my_id;
    int ret;

    ret = PtlGetPhysId(ni_h, &my_id);
    if (ret != PTL_OK) return NULL;

    mapping = MALLOC(sizeof(ptl_process_t) * size);
    if (!mapping) return NULL;

    if (MPI_SUCCESS != MPI_Allgather(&my_id, sizeof(my_id), MPI_BYTE,
                                     mapping, sizeof(my_id), MPI_BYTE,
                                     MPI_COMM_WORLD)) {
        FREE(mapping, sizeof(ptl_process_t) * size);
        mapping = NULL;
    }

    return mapping;
}


void
qthread_internal_net_driver_runtime_barrier(void)
{
    MPI_Barrier(MPI_COMM_WORLD);
}


int
qthread_internal_net_driver_get_rank(void)
{
    return rank;
}


int
qthread_internal_net_driver_get_size(void)
{
    return size;
}

