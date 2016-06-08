/*
 * Copyright © 2009 inria.  All rights reserved.
 * Copyright © 2009 Université Bordeaux
 * See COPYING in top-level directory.
 */

#ifndef HWLOC_PORT_SOLARIS_SYS_PROCESSOR_H
#define HWLOC_PORT_SOLARIS_SYS_PROCESSOR_H

#include <sys/procset.h>
typedef int processorid_t;
#define PBIND_NONE -1
#define PBIND_QUERY -2

extern int processor_bind(idtype_t idtype, id_t id, processorid_t processorid, processorid_t *obind);

#endif /* HWLOC_PORT_SOLARIS_SYS_PROCESSOR_H */
