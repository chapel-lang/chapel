/*
 * Copyright © 2009 inria.  All rights reserved.
 * Copyright © 2009 Université Bordeaux
 * See COPYING in top-level directory.
 */

#ifndef HWLOC_PORT_FREEBSD_SYS_SYSCTL_H
#define HWLOC_PORT_FREEBSD_SYS_SYSCTL_H

extern int sysctl(int *name, int name_len, void *oldp, size_t *oldlenp, void *newp, size_t newlen);
extern int sysctlbyname(const char *name, void *oldp, size_t *oldlenp, const void *newp, size_t newlen);

/*
 * CTL_KERN identifiers
 */
#define	CTL_KERN	1
#define CTL_HW 6

/*
 * CTL_KERN identifiers
 */
#define	KERN_PROC 14	

/*
 * KERN_PROC subtypes
 */
#define	KERN_PROC_PID 1
#define	KERN_PROC_INC_THREAD 0x10

/*
 * CTL_HW identifiers
 */
#define HW_PHYSMEM 5

#endif /* HWLOC_PORT_FREEBSD_SYS_SYSCTL_H */
