/* -*- C -*-
 *
 * Copyright 2011 Sandia Corporation. Under the terms of Contract
 * DE-AC04-94AL85000 with Sandia Corporation, the U.S.  Government
 * retains certain rights in this software.
 *
 * Run-time support interface for portals
 */


#ifndef NET_PORTALS4_RUNTIME_H
#define NET_PORTALS4_RUNTIME_H

int qthread_internal_net_driver_runtime_init(void);
int qthread_internal_net_driver_runtime_fini(void);
/* note: get_mapping has barrier semantics */
ptl_process_t* qthread_internal_net_driver_runtime_get_mapping(ptl_handle_ni_t ni_h);
void qthread_internal_net_driver_runtime_barrier(void);

int qthread_internal_net_driver_get_rank(void);
int qthread_internal_net_driver_get_size(void);

#endif
