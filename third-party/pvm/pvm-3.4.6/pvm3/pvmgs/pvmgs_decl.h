
/* $Id: pvmgs_decl.h,v 1.2 1997/07/09 13:51:29 pvmsrc Exp $ */

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

int gs_init();
int gs_hash_init();
int gs_sgroup_init();
GROUP_STRUCT_PTR gs_struct_init();
int gs_struct_destroy();
GROUP_LIST_PTR gs_hash_name();
GROUP_LIST_PTR gs_list_insert();
GROUP_STRUCT_PTR gs_group();
int gs_delete_group();
int gs_handle();
int gs_pstate();
int gs_join();
int gs_leave();
int gs_barrier();
int gs_deadtid();
int gs_tidlist();
int gs_gsize();
int gs_gettid();
int gs_pstate();
int gs_ls();
int gs_uniquename();
int gs_static();
int gs_pkstaticinfo();
int gs_newstaticcache();
int * gs_realloc_int_array();
int gs_binsearch();
int gs_newhost();
int gs_delhost();
int gs_host_char();
int gs_host_all();
int gs_retint();
int gs_tidtohost();
int gs_hash_clear();

