/*   $Source: /var/local/cvs/gasnet/other/smp-collectives/smp_coll_dissem.h,v $
 *     $Date: 2009/10/22 20:24:55 $
 * $Revision: 1.3 $
 * Description: Shared Memory Collectives
 * Copyright 2009, Rajesh Nishtala <rajeshn@eecs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef ALREADY_SEEN_DISSEM_H
#define ALREADY_SEEN_DISSEM_H 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef MAX 
#define MAX(A,B) ((A < B) ? B : A)
#endif

struct dissem_vector_t_;
typedef struct dissem_vector_t_ dissem_vector_t;
struct  dissem_info_t_;
typedef struct dissem_info_t_ dissem_info_t;

#include <smp-collectives/smp_coll_internal.h>

struct dissem_vector_t_{
  int *elem_list;
  int n;
};

struct dissem_info_t_ {
  dissem_vector_t *barrier_order;
  dissem_vector_t *all_reduce_order;
  int dissemination_phases; /*log_radix(THREADS)*/
  int dissemination_radix;
  int max_dissem_blocks;
  /*an array that holds the number of blocks we send in each phase of the 
    dissemination all to all. Used to capture when nblocks is 1 to avoid copies*/
  int *n_blocks; 
  /*whether this dissem obj is designed to run the all_reduce*/
  /*only true when power of two proc count AND radix is 2*/
  int all_reduce_ok; 
};




/*given a radix ... build a dissem_info_t*/
dissem_info_t *smp_coll_build_dissemination(int radix, int MYTHREAD, int THREADS);
void smp_coll_dump_dissem_order(dissem_info_t *obj, int MYTHREAD);
void smp_coll_free_dissemination(dissem_info_t* obj);

#endif
