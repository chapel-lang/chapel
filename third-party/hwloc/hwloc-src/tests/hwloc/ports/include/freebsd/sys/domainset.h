/*
 * Copyright © 2009 Université Bordeaux
 * Copyright © 2015-2020 Inria.  All rights reserved.
 * See COPYING in top-level directory.
 */

#ifndef HWLOC_PORT_FREEBSD_SYS_DOMAINSET_H
#define HWLOC_PORT_FREEBSD_SYS_DOMAINSET_H

typedef long domainset_t;

#define	DOMAINSET_POLICY_ROUNDROBIN	1
#define	DOMAINSET_POLICY_FIRSTTOUCH	2
#define	DOMAINSET_POLICY_PREFER		3
#define	DOMAINSET_POLICY_INTERLEAVE	4

#undef DOMAINSET_ZERO
#define	DOMAINSET_ZERO(domainset) (*(domainset) = 0)
#undef DOMAINSET_SETSIZE
#define DOMAINSET_SETSIZE (sizeof(domainset_t) * CHAR_BIT)

int	cpuset_getdomain(cpulevel_t, cpuwhich_t, id_t, size_t, domainset_t *, int *);
int	cpuset_setdomain(cpulevel_t, cpuwhich_t, id_t, size_t, const domainset_t *, int);

#endif /* HWLOC_PORT_FREEBSD_SYS_DOMAINSET_H */
