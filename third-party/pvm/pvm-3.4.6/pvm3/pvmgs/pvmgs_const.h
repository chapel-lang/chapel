
/* $Id: pvmgs_const.h,v 1.4 1998/11/20 19:49:04 pvmsrc Exp $ */

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

/* File: pvmgs_const.h - constants used for pvmgs programs                 */
#define GSNAME	         "pvmgs"

/* Message tags that the server responds to                                */
#define DIE	         0	/* halt the group server                   */
#define JOIN	         1	/* join a group                            */
#define LEAVE	         2	/* leave a group                           */
#define BARRIER	         3	/* group barrier                           */
#define BCAST	         4	/* broadcast to a group                    */
#define GETINST	         5	/* get the group inst given the task id    */
#define GETTID	         6	/* get the task id given the group inst    */
#define GSIZE	         7	/* get the group size                      */
#define RESET	         8	/* reset the group server                  */
#define DUMP	         9	/* dump the state                          */
#define GSLS	         10	/* send back a list of the groups          */
#define DEADTID	         11	/* notify tag used when a task dies        */
#define TIDLIST          12	/* get list of tids for group              */
#define STATICGROUP      13     /* form a static group                     */
#define HOSTCHAR         14     /* get nproc, coord for a host in a group  */
#define HOSTCHARV        15     /* get nproc, coord for all hosts in group */ 
#define BARRIERV         16     /* barrier call by host coordinator        */

/* Initial Storage dimensions and Delta Storage Dimensions                 */
#define DELTANTIDS       10              /* increment for remalloc         */ 
#define DELTABTIDS       DELTANTIDS      /* increment for remalloc         */

/* Hash Table dimension                                                    */
/* WIN32 wants to define its own values in WINNT.H -- we can't allow this... */
#ifdef WIN32
#undef MINCHAR
#undef MAXCHAR
#endif

#define MINCHAR          32 
#define MAXCHAR          127
#define HASHSIZE         MAXCHAR - MINCHAR + 1


/* Other manifest constants                                                */
#define CREATE           1               /* flag to create hash entry      */
#define NOCREATE         0                
#define NOTID            -1              /* indicates no tid               */
#define STATIC           1               /* indicate a group is static     */ 
#define DYNAMIC          2               /* indicate a group is dynamic    */
#define STATICDEAD       3               /* dead static group              */
#define PvmgsNotCached   1               /* tid has not cached the data    */
