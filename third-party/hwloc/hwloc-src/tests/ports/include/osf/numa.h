/*
 * Copyright © 2009 inria.  All rights reserved.
 * Copyright © 2009-2010 Université Bordeaux
 * See COPYING in top-level directory.
 */

#ifndef HWLOC_PORT_OSF_NUMA_H
#define HWLOC_PORT_OSF_NUMA_H

#include "sys/types.h"
#include "radset.h"
#include "cpuset.h"

int rad_get_num(void);
int rad_get_cpus(radid_t rad, cpuset_t cpuset);
ssize_t rad_get_physmem(radid_t rad);

typedef struct {
  unsigned long mattr_policy;
  radid_t mattr_rad;
  radset_t mattr_radset;
} memalloc_attr_t;

void *nmmap(void *addr, size_t len, int prot, unsigned long flags, int filedes, off_t off, memalloc_attr_t *attr );
int nmadvise(const void *addr, long len, int behavior, memalloc_attr_t *attr);

#endif /* HWLOC_PORT_OSF_NUMA_H */
