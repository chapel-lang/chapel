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
#include <portals4.h>
#include <portals4_runtime.h>

#include "runtime.h"

#include "qt_debug.h" /* for malloc debug wrappers */


int
qthread_internal_net_driver_runtime_init(void)
{
    return 0;
}


int
qthread_internal_net_driver_runtime_fini(void)
{
    return 0;
}


ptl_process_t*
qthread_internal_net_driver_runtime_get_mapping(ptl_handle_ni_t ni_h)
{
    int size, i;
    ptl_process_t *ret;
    struct runtime_proc_t *procs;

    size = runtime_get_size();
    ret = MALLOC(sizeof(ptl_process_t) * size);
    if (NULL == ret) return NULL;

    runtime_get_nidpid_map(&procs);

    for (i = 0 ; i < size ; ++i) {
        ret[i].phys.nid = procs[i].nid;
        ret[i].phys.pid = procs[i].pid;
    }

    return ret;
}


void
qthread_internal_net_driver_runtime_barrier(void)
{
    runtime_barrier();
}


int
qthread_internal_net_driver_get_rank(void)
{
    return runtime_get_rank();
}


int
qthread_internal_net_driver_get_size(void)
{
    return runtime_get_size();
}

