
/* $Id: pvm_consts.h,v 1.4 1999/12/10 15:21:45 pvmsrc Exp $ */

/*
 *         PVM version 3.4:  Parallel Virtual Machine System
 *               University of Tennessee, Knoxville TN.
 *           Oak Ridge National Laboratory, Oak Ridge TN.
 *                   Emory University, Atlanta GA.
 *      Authors:  J. J. Dongarra, G. E. Fagg, M. Fischer
 *          G. A. Geist, J. A. Kohl, R. J. Manchek, P. Mucci,
 *         P. M. Papadopoulos, S. L. Scott, and V. S. Sunderam
 *                   (C) 1997 All Rights Reserved
 *
 *                              NOTICE
 *
 * Permission to use, copy, modify, and distribute this software and
 * its documentation for any purpose and without fee is hereby granted
 * provided that the above copyright notice appear in all copies and
 * that both the copyright notice and this permission notice appear in
 * supporting documentation.
 *
 * Neither the Institutions (Emory University, Oak Ridge National
 * Laboratory, and University of Tennessee) nor the Authors make any
 * representations about the suitability of this software for any
 * purpose.  This software is provided ``as is'' without express or
 * implied warranty.
 *
 * PVM version 3 was funded in part by the U.S. Department of Energy,
 * the National Science Foundation and the State of Tennessee.
 */

#include <sys/types.h>
#include <fcntl.h>

#define MAX_ARCH_NAME	31
#define MAX_EVENT_NAME	31
#define MAX_HOST_NAME	256
#define MAX_PROC_NAME	256
#define MAX_GRP_NAME	31
#define MAX_HOSTS       256
#define MAX_MBOX_NAME	256

#define STRING    0
#define BYTE1     1
#define INTEGER2  2
#define INTEGER4  3
#define REAL4     4
#define COMPLEX8  5
#define REAL8     6
#define COMPLEX16 7
#define INTEGER8  8

