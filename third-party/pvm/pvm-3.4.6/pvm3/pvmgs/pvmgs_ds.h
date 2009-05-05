
/* $Id: pvmgs_ds.h,v 1.2 1997/07/09 13:51:30 pvmsrc Exp $ */

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

/* File: pvmgs_ds.h - Define various data structures used by the group server*/


/* Structure that defines a group  */
typedef struct group_struct {
        char *name;	        /* the name of the group */
        int  len;               /* length of the group name */
        int  ntids;	        /* number of tids */
        int  *tids;	        /* array of the tids */
        int  maxntids;          /* max number of tids before remalloc*/ 
        int  barrier_count;	/* count for the current barrier */
        int  barrier_reached;	/* number of procs reaching barrier */
        int  barrier_index;     /* number of tids we need to release */
	int  *btids;	        /* tids that have reached barrier */
        int  maxbtids;          /* max number of btids before remalloc  */
        int  sgroup_count;	/* count for static group formation */
        int  sgroup_reached;	/* number of procs reaching sgroup */
	int  *stids;	        /* tids that have reached sgroup */
        int  maxstids;          /* max number of stids before remalloc  */
        int  staticgroup;       /* tell if this is a static group  */
        int  ninfotids;         /* if static, how many have the info  */
        int  *infotids;         /* list of tids that have the static info */
        int  maxinfotids;       /* max num of info tids before remalloc   */ 
        int  nhosts;            /* hosts in this group                    */
        int  *np_onhost;        /* #procs on host                         */
        int  *pcoord;           /* proc that coords collectives on a host */ 
        int  maxhosts;          /* max number of hosts before remalloc    */
   
	} GROUP_STRUCT, *GROUP_STRUCT_PTR;

/* A doubly-linked list of group structures */
typedef struct group_list {
        struct group_list *prev, *next;
        GROUP_STRUCT_PTR group;
        } GROUP_LIST, *GROUP_LIST_PTR;


