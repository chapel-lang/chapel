/*
 * Copyright © 2013 Inria.  All rights reserved.
 * See COPYING in top-level directory.
 */

#ifndef HWLOC_PORT_BGQ_KERNEL_LOCATION_H
#define HWLOC_PORT_BGQ_KERNEL_LOCATION_H

#include <stdint.h>

uint32_t Kernel_ProcessorID( void );
uint32_t Kernel_MyTcoord( void );

#endif /* HWLOC_PORT_BGQ_KERNEL_LOCATION_H */
