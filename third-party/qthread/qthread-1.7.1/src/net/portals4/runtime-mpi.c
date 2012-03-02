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

static int rank = 0;
static int size = 0;
static ptl_process_t my_id;

int
qthread_internal_net_driver_runtime_init(void)
{
    int ret;
    ptl_handle_ni_t phys_ni_h;

    MPI_Initialized(&ret);
    if (!ret) {
        MPI_Init(NULL, NULL);
    }

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    ret = PtlNIInit(PTL_IFACE_DEFAULT,
                    PTL_NI_NO_MATCHING | PTL_NI_PHYSICAL,
                    PTL_PID_ANY,
                    NULL,
                    NULL,
                    0,
                    NULL,
                    NULL,
                    &phys_ni_h);
    if (PTL_OK != ret) return 1;

    ret = PtlGetId(phys_ni_h, &my_id);
    if (PTL_OK != ret) return 1;

    PtlNIFini(phys_ni_h);

    return 0;
}


int
qthread_internal_net_driver_runtime_fini(void)
{
    int ret;
    MPI_Finalized(&ret);
    if (!ret) {
        MPI_Finalize();
    }

    return 0;
}


ptl_process_t*
qthread_internal_net_driver_runtime_get_mapping(void)
{
    ptl_process_t *ret;

    ret = malloc(sizeof(ptl_process_t) * size);
    if (NULL == ret) return 0;

    MPI_Allgather(&my_id, sizeof(my_id), MPI_BYTE,
                  ret, sizeof(my_id), MPI_BYTE,
                  MPI_COMM_WORLD);

    return ret;
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

