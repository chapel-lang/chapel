/**********************************************************************************************/
/*  This program is part of the Barcelona OpenMP Tasks Suite                                  */
/*  Copyright (C) 2009 Barcelona Supercomputing Center - Centro Nacional de Supercomputacion  */
/*  Copyright (C) 2009 Universitat Politecnica de Catalunya                                   */
/*                                                                                            */
/*  This program is free software; you can redistribute it and/or modify                      */
/*  it under the terms of the GNU General Public License as published by                      */
/*  the Free Software Foundation; either version 2 of the License, or                         */
/*  (at your option) any later version.                                                       */
/*                                                                                            */
/*  This program is distributed in the hope that it will be useful,                           */
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of                            */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                             */
/*  GNU General Public License for more details.                                              */
/*                                                                                            */
/*  You should have received a copy of the GNU General Public License                         */
/*  along with this program; if not, write to the Free Software                               */
/*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA            */
/**********************************************************************************************/
/*
 *  Original code from the Cilk project
 *
 * Copyright (c) 2000 Massachusetts Institute of Technology
 * Copyright (c) 2000 Matteo Frigo
 */
/*
 * this program uses an algorithm that we call `cilksort'.
 * The algorithm is essentially mergesort:
 *
 *   cilksort(in[1..n]) =
 *       spawn cilksort(in[1..n/2], tmp[1..n/2])
 *       spawn cilksort(in[n/2..n], tmp[n/2..n])
 *       sync
 *       spawn cilkmerge(tmp[1..n/2], tmp[n/2..n], in[1..n])
 *
 *
 * The procedure cilkmerge does the following:
 *       
 *       cilkmerge(A[1..n], B[1..m], C[1..(n+m)]) =
 *          find the median of A \union B using binary
 *          search.  The binary search gives a pair
 *          (ma, mb) such that ma + mb = (n + m)/2
 *          and all elements in A[1..ma] are smaller than
 *          B[mb..m], and all the B[1..mb] are smaller
 *          than all elements in A[ma..n].
 *
 *          spawn cilkmerge(A[1..ma], B[1..mb], C[1..(n+m)/2])
 *          spawn cilkmerge(A[ma..m], B[mb..n], C[(n+m)/2 .. (n+m)])
 *          sync
 *
 * The algorithm appears for the first time (AFAIK) in S. G. Akl and
 * N. Santoro, "Optimal Parallel Merging and Sorting Without Memory
 * Conflicts", IEEE Trans. Comp., Vol. C-36 No. 11, Nov. 1987 .  The
 * paper does not express the algorithm using recursion, but the
 * idea of finding the median is there.
 *
 * For cilksort of n elements, T_1 = O(n log n) and
 * T_\infty = O(log^3 n).  There is a way to shave a
 * log factor in the critical path (left as homework).
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bots.h"
#include "app-desc.h"
#include "libxomp.h" 
typedef long ELM;
ELM *array;
ELM *tmp;
static unsigned long rand_nxt = (0);

inline static unsigned long my_rand()
{
  rand_nxt = ((rand_nxt * (1103515245)) + (12345));
  return rand_nxt;
}

inline static void my_srand(unsigned long seed)
{
  rand_nxt = seed;
}

inline static ELM med3(ELM a,ELM b,ELM c)
{
  if (a < b) {
    if (b < c) {
      return b;
    }
    else {
      if (a < c) 
        return c;
      else 
        return a;
    }
  }
  else {
    if (b > c) {
      return b;
    }
    else {
      if (a > c) 
        return c;
      else 
        return a;
    }
  }
}
/*
 * simple approach for now; a better median-finding
 * may be preferable
 */

inline static ELM choose_pivot(ELM *low,ELM *high)
{
  return med3( *low, *high,low[(high - low) / (2)]);
}

static ELM *seqpart(ELM *low,ELM *high)
{
  ELM pivot;
  ELM h;
  ELM l;
  ELM *curr_low = low;
  ELM *curr_high = high;
  pivot = choose_pivot(low,high);
  while((1)){
    while((h =  *curr_high) > pivot)
      curr_high--;
    while((l =  *curr_low) < pivot)
      curr_low++;
    if (curr_low >= curr_high) 
      break; 
     *(curr_high--) = l;
     *(curr_low++) = h;
  }
/*
      * I don't know if this is really necessary.
      * The problem is that the pivot is not always the
      * first element, and the partition may be trivial.
      * However, if the partition is trivial, then
      * *high is the largest element, whence the following
      * code.
      */
  if (curr_high < high) 
    return curr_high;
  else 
    return curr_high - 1;
}
#define swap(a, b) \
{ \
  ELM tmp;\
  tmp = a;\
  a = b;\
  b = tmp;\
}

static void insertion_sort(ELM *low,ELM *high)
{
  ELM *p;
  ELM *q;
  ELM a;
  ELM b;
  for (q = (low + 1); q <= high; ++q) {
    a = q[0];
    for (p = (q - 1); (p >= low) && ((b = p[0]) > a); p--) 
      p[1] = b;
    p[1] = a;
  }
}
/*
 * tail-recursive quicksort, almost unrecognizable :-)
 */

void seqquick(ELM *low,ELM *high)
{
  ELM *p;
  while((high - low) >= (bots_app_cutoff_value_2)){
    p = seqpart(low,high);
    seqquick(low,p);
    low = (p + 1);
  }
  insertion_sort(low,high);
}

void seqmerge(ELM *low1,ELM *high1,ELM *low2,ELM *high2,ELM *lowdest)
{
  ELM a1;
  ELM a2;
/*
      * The following 'if' statement is not necessary
      * for the correctness of the algorithm, and is
      * in fact subsumed by the rest of the function.
      * However, it is a few percent faster.  Here is why.
      *
      * The merging loop below has something like
      *   if (a1 < a2) {
      *        *dest++ = a1;
      *        ++low1;
      *        if (end of array) break;
      *        a1 = *low1;
      *   }
      *
      * Now, a1 is needed immediately in the next iteration
      * and there is no way to mask the latency of the load.
      * A better approach is to load a1 *before* the end-of-array
      * check; the problem is that we may be speculatively
      * loading an element out of range.  While this is
      * probably not a problem in practice, yet I don't feel
      * comfortable with an incorrect algorithm.  Therefore,
      * I use the 'fast' loop on the array (except for the last 
      * element) and the 'slow' loop for the rest, saving both
      * performance and correctness.
      */
  if ((low1 < high1) && (low2 < high2)) {
    a1 =  *low1;
    a2 =  *low2;
    for (; ; ) {
      if (a1 < a2) {
         *(lowdest++) = a1;
        a1 =  *(++low1);
        if (low1 >= high1) 
          break; 
      }
      else {
         *(lowdest++) = a2;
        a2 =  *(++low2);
        if (low2 >= high2) 
          break; 
      }
    }
  }
  if ((low1 <= high1) && (low2 <= high2)) {
    a1 =  *low1;
    a2 =  *low2;
    for (; ; ) {
      if (a1 < a2) {
         *(lowdest++) = a1;
        ++low1;
        if (low1 > high1) 
          break; 
        a1 =  *low1;
      }
      else {
         *(lowdest++) = a2;
        ++low2;
        if (low2 > high2) 
          break; 
        a2 =  *low2;
      }
    }
  }
  if (low1 > high1) {
    memcpy((lowdest),(low2),(((sizeof(ELM )) * (((high2 - low2) + (1))))));
  }
  else {
    memcpy((lowdest),(low1),(((sizeof(ELM )) * (((high1 - low1) + (1))))));
  }
}
#define swap_indices(a, b) \
{ \
  ELM *tmp;\
  tmp = a;\
  a = b;\
  b = tmp;\
}

ELM *binsplit(ELM val,ELM *low,ELM *high)
{
/*
      * returns index which contains greatest element <= val.  If val is
      * less than all elements, returns low-1
      */
  ELM *mid;
  while(low != high){
    mid = (low + (((high - low) + (1)) >> 1));
    if (val <=  *mid) 
      high = (mid - 1);
    else 
      low = mid;
  }
  if ( *low > val) 
    return low - 1;
  else 
    return low;
}

struct OUT__9__1527___data 
{
  ELM *high1;
  ELM *high2;
  ELM *lowdest;
  ELM *split1;
  ELM *split2;
  long lowsize;
}
;
static void OUT__9__1527__(void *__out_argv);

struct OUT__10__1527___data 
{
  ELM *low1;
  ELM *low2;
  ELM *lowdest;
  ELM *split1;
  ELM *split2;
}
;
static void OUT__10__1527__(void *__out_argv);

void cilkmerge_par(ELM *low1,ELM *high1,ELM *low2,ELM *high2,ELM *lowdest)
{
/*
      * Cilkmerge: Merges range [low1, high1] with range [low2, high2] 
      * into the range [lowdest, ...]  
      */
/*
				 * where each of the ranges are broken for 
				 * recursive merge 
				 */
  ELM *split1;
  ELM *split2;
/*
				 * total size of lower halves of two
				 * ranges - 2 
				 */
  long lowsize;
/*
      * We want to take the middle element (indexed by split1) from the
      * larger of the two arrays.  The following code assumes that split1
      * is taken from range [low1, high1].  So if [low1, high1] is
      * actually the smaller range, we should swap it with [low2, high2] 
      */
  if ((high2 - low2) > (high1 - low1)) {{
      ELM *tmp;
      tmp = low1;
      low1 = low2;
      low2 = tmp;
    }
{
      ELM *tmp;
      tmp = high1;
      high1 = high2;
      high2 = tmp;
    }
  }
  if (high2 < low2) {
/* smaller range is empty */
    memcpy((lowdest),(low1),(((sizeof(ELM )) * ((high1 - low1)))));
    return ;
  }
  if ((high2 - low2) < (bots_app_cutoff_value)) {
    seqmerge(low1,high1,low2,high2,lowdest);
    return ;
  }
/*
      * Basic approach: Find the middle element of one range (indexed by
      * split1). Find where this element would fit in the other range
      * (indexed by split 2). Then merge the two lower halves and the two
      * upper halves. 
      */
  split1 = (low1 + (((high1 - low1) + (1)) / (2)));
  split2 = binsplit( *split1,low2,high2);
  lowsize = (((split2 + (split1 - low1)) - low2));
/* 
      * directly put the splitting element into
      * the appropriate location
      */
   *((lowdest + lowsize) + 1) =  *split1;
  struct OUT__10__1527___data __out_argv10__1527__;
  __out_argv10__1527__.OUT__10__1527___data::split2 = split2;
  __out_argv10__1527__.OUT__10__1527___data::split1 = split1;
  __out_argv10__1527__.OUT__10__1527___data::lowdest = lowdest;
  __out_argv10__1527__.OUT__10__1527___data::low2 = low2;
  __out_argv10__1527__.OUT__10__1527___data::low1 = low1;
  XOMP_task(OUT__10__1527__,&__out_argv10__1527__,0,sizeof(struct OUT__10__1527___data ),4,1,1);
  struct OUT__9__1527___data __out_argv9__1527__;
  __out_argv9__1527__.OUT__9__1527___data::lowsize = lowsize;
  __out_argv9__1527__.OUT__9__1527___data::split2 = split2;
  __out_argv9__1527__.OUT__9__1527___data::split1 = split1;
  __out_argv9__1527__.OUT__9__1527___data::lowdest = lowdest;
  __out_argv9__1527__.OUT__9__1527___data::high2 = high2;
  __out_argv9__1527__.OUT__9__1527___data::high1 = high1;
  XOMP_task(OUT__9__1527__,&__out_argv9__1527__,0,sizeof(struct OUT__9__1527___data ),4,1,1);
  XOMP_taskwait();
}

struct OUT__3__1527___data 
{
  ELM *low;
  long size;
  long quarter;
  ELM *C;
  ELM *D;
  ELM *tmpC;
}
;
static void OUT__3__1527__(void *__out_argv);

struct OUT__4__1527___data 
{
  long quarter;
  ELM *A;
  ELM *B;
  ELM *tmpA;
}
;
static void OUT__4__1527__(void *__out_argv);

struct OUT__5__1527___data 
{
  long size;
  long quarter;
  ELM *D;
  ELM *tmpD;
}
;
static void OUT__5__1527__(void *__out_argv);

struct OUT__6__1527___data 
{
  long quarter;
  ELM *C;
  ELM *tmpC;
}
;
static void OUT__6__1527__(void *__out_argv);

struct OUT__7__1527___data 
{
  long quarter;
  ELM *B;
  ELM *tmpB;
}
;
static void OUT__7__1527__(void *__out_argv);

struct OUT__8__1527___data 
{
  long quarter;
  ELM *A;
  ELM *tmpA;
}
;
static void OUT__8__1527__(void *__out_argv);

void cilksort_par(ELM *low,ELM *tmp,long size)
{
/*
      * divide the input in four parts of the same size (A, B, C, D)
      * Then:
      *   1) recursively sort A, B, C, and D (in parallel)
      *   2) merge A and B into tmp1, and C and D into tmp2 (in parallel)
      *   3) merge tmp1 and tmp2 into the original array
      */
  long quarter = (size / (4));
  ELM *A;
  ELM *B;
  ELM *C;
  ELM *D;
  ELM *tmpA;
  ELM *tmpB;
  ELM *tmpC;
  ELM *tmpD;
  if (size < ((unsigned int)bots_app_cutoff_value_1)) {
/* quicksort when less than 1024 elements */
    seqquick(low,((low + size) - 1));
    return ;
  }
  A = low;
  tmpA = tmp;
  B = (A + quarter);
  tmpB = (tmpA + quarter);
  C = (B + quarter);
  tmpC = (tmpB + quarter);
  D = (C + quarter);
  tmpD = (tmpC + quarter);
  struct OUT__8__1527___data __out_argv8__1527__;
  __out_argv8__1527__.OUT__8__1527___data::tmpA = tmpA;
  __out_argv8__1527__.OUT__8__1527___data::A = A;
  __out_argv8__1527__.OUT__8__1527___data::quarter = quarter;
  XOMP_task(OUT__8__1527__,&__out_argv8__1527__,0,sizeof(struct OUT__8__1527___data ),4,1,1);
  struct OUT__7__1527___data __out_argv7__1527__;
  __out_argv7__1527__.OUT__7__1527___data::tmpB = tmpB;
  __out_argv7__1527__.OUT__7__1527___data::B = B;
  __out_argv7__1527__.OUT__7__1527___data::quarter = quarter;
  XOMP_task(OUT__7__1527__,&__out_argv7__1527__,0,sizeof(struct OUT__7__1527___data ),4,1,1);
  struct OUT__6__1527___data __out_argv6__1527__;
  __out_argv6__1527__.OUT__6__1527___data::tmpC = tmpC;
  __out_argv6__1527__.OUT__6__1527___data::C = C;
  __out_argv6__1527__.OUT__6__1527___data::quarter = quarter;
  XOMP_task(OUT__6__1527__,&__out_argv6__1527__,0,sizeof(struct OUT__6__1527___data ),4,1,1);
  struct OUT__5__1527___data __out_argv5__1527__;
  __out_argv5__1527__.OUT__5__1527___data::tmpD = tmpD;
  __out_argv5__1527__.OUT__5__1527___data::D = D;
  __out_argv5__1527__.OUT__5__1527___data::quarter = quarter;
  __out_argv5__1527__.OUT__5__1527___data::size = size;
  XOMP_task(OUT__5__1527__,&__out_argv5__1527__,0,sizeof(struct OUT__5__1527___data ),4,1,1);
  XOMP_taskwait();
  struct OUT__4__1527___data __out_argv4__1527__;
  __out_argv4__1527__.OUT__4__1527___data::tmpA = tmpA;
  __out_argv4__1527__.OUT__4__1527___data::B = B;
  __out_argv4__1527__.OUT__4__1527___data::A = A;
  __out_argv4__1527__.OUT__4__1527___data::quarter = quarter;
  XOMP_task(OUT__4__1527__,&__out_argv4__1527__,0,sizeof(struct OUT__4__1527___data ),4,1,1);
  struct OUT__3__1527___data __out_argv3__1527__;
  __out_argv3__1527__.OUT__3__1527___data::tmpC = tmpC;
  __out_argv3__1527__.OUT__3__1527___data::D = D;
  __out_argv3__1527__.OUT__3__1527___data::C = C;
  __out_argv3__1527__.OUT__3__1527___data::quarter = quarter;
  __out_argv3__1527__.OUT__3__1527___data::size = size;
  __out_argv3__1527__.OUT__3__1527___data::low = low;
  XOMP_task(OUT__3__1527__,&__out_argv3__1527__,0,sizeof(struct OUT__3__1527___data ),4,1,1);
  XOMP_taskwait();
  cilkmerge_par(tmpA,(tmpC - 1),tmpC,((tmpA + size) - 1),A);
}

void scramble_array(ELM *array)
{
  unsigned long i;
  unsigned long j;
  for (i = (0); i < (bots_arg_size); ++i) {
    j = my_rand();
    j = (j % (bots_arg_size));
{
      ELM tmp;
      tmp = array[i];
      array[i] = array[j];
      array[j] = tmp;
    }
  }
}

void fill_array(ELM *array)
{
  unsigned long i;
  my_srand((1));
/* first, fill with integers 1..size */
  for (i = (0); i < (bots_arg_size); ++i) {
    array[i] = (i);
  }
}

void sort_init()
{
/* Checking arguments */
  if (bots_arg_size < 4) {{
      if ((bots_verbose_mode) >= (BOTS_VERBOSE_DEFAULT)) {
        fprintf(stdout,"%s can not be less than 4, using 4 as a parameter.\n","Array size");
      }
    }
    bots_arg_size = 4;
  }
  if (bots_app_cutoff_value < 2) {{
      if ((bots_verbose_mode) >= (BOTS_VERBOSE_DEFAULT)) {
        fprintf(stdout,"%s can not be less than 2, using 2 as a parameter.\n","Sequential Merge cutoff value");
      }
    }
    bots_app_cutoff_value = 2;
  }
  else if ((unsigned int)bots_app_cutoff_value > bots_arg_size) {{
      if ((bots_verbose_mode) >= (BOTS_VERBOSE_DEFAULT)) {
        fprintf(stdout,"%s can not be greather than vector size, using %d as a parameter.\n","Sequential Merge cutoff value",bots_arg_size);
      }
    }
    bots_app_cutoff_value = bots_arg_size;
  }
  if ((unsigned int)bots_app_cutoff_value_1 > bots_arg_size) {{
      if ((bots_verbose_mode) >= (BOTS_VERBOSE_DEFAULT)) {
        fprintf(stdout,"%s can not be greather than vector size, using %d as a parameter.\n","Sequential Quicksort cutoff value",bots_arg_size);
      }
    }
    bots_app_cutoff_value_1 = bots_arg_size;
  }
  if ((unsigned int)bots_app_cutoff_value_2 > bots_arg_size) {{
      if ((bots_verbose_mode) >= (BOTS_VERBOSE_DEFAULT)) {
        fprintf(stdout,"%s can not be greather than vector size, using %d as a parameter.\n","Sequential Insertion cutoff value",bots_arg_size);
      }
    }
    bots_app_cutoff_value_2 = bots_arg_size;
  }
  if (bots_app_cutoff_value_2 > bots_app_cutoff_value_1) {{
      if ((bots_verbose_mode) >= (BOTS_VERBOSE_DEFAULT)) {
        fprintf(stdout,"%s can not be greather than %s, using %d as a parameter.\n","Sequential Insertion cutoff value","Sequential Quicksort cutoff value",bots_app_cutoff_value_1);
      }
    }
    bots_app_cutoff_value_2 = bots_app_cutoff_value_1;
  }
  array = ((ELM *)(malloc((((bots_arg_size) * (sizeof(ELM )))))));
  tmp = ((ELM *)(malloc((((bots_arg_size) * (sizeof(ELM )))))));
  fill_array(array);
  scramble_array(array);
}
static void OUT__1__1527__(void *__out_argv);
static void OUT__2__1527__(void *__out_argv);

void sort_par()
{
{
    if ((bots_verbose_mode) >= (BOTS_VERBOSE_DEFAULT)) {
      fprintf(stdout,"Computing multisort algorithm (n=%d) ",bots_arg_size);
    }
  }
  XOMP_parallel_start(OUT__2__1527__,0,1,0,"OUT__2__1527__");
  XOMP_parallel_end();
{
    if ((bots_verbose_mode) >= (BOTS_VERBOSE_DEFAULT)) {
      fprintf(stdout," completed!\n");
    }
  }
}

int sort_verify()
{
  unsigned int i;
  int success = 1;
  for (i = 0; i < bots_arg_size; ++i) 
    if (array[i] != (i)) 
      success = 0;
  return (success)?1 : 2;
}

static void OUT__1__1527__(void *__out_argv)
{
  cilksort_par(array,tmp,(bots_arg_size));
}

static void OUT__2__1527__(void *__out_argv)
{
  if (XOMP_single()) {
    XOMP_task(OUT__1__1527__,0,0,0,0,1,1);
  }
  XOMP_barrier();
}

static void OUT__3__1527__(void *__out_argv)
{
  ELM *low = (ELM *)(((struct OUT__3__1527___data *)__out_argv) -> OUT__3__1527___data::low);
  long size = (long )(((struct OUT__3__1527___data *)__out_argv) -> OUT__3__1527___data::size);
  long quarter = (long )(((struct OUT__3__1527___data *)__out_argv) -> OUT__3__1527___data::quarter);
  ELM *C = (ELM *)(((struct OUT__3__1527___data *)__out_argv) -> OUT__3__1527___data::C);
  ELM *D = (ELM *)(((struct OUT__3__1527___data *)__out_argv) -> OUT__3__1527___data::D);
  ELM *tmpC = (ELM *)(((struct OUT__3__1527___data *)__out_argv) -> OUT__3__1527___data::tmpC);
  ELM *_p_low = low;
  long _p_size = size;
  long _p_quarter = quarter;
  ELM *_p_C = C;
  ELM *_p_D = D;
  ELM *_p_tmpC = tmpC;
  cilkmerge_par(_p_C,((_p_C + _p_quarter) - 1),_p_D,((_p_low + _p_size) - 1),_p_tmpC);
}

static void OUT__4__1527__(void *__out_argv)
{
  long quarter = (long )(((struct OUT__4__1527___data *)__out_argv) -> OUT__4__1527___data::quarter);
  ELM *A = (ELM *)(((struct OUT__4__1527___data *)__out_argv) -> OUT__4__1527___data::A);
  ELM *B = (ELM *)(((struct OUT__4__1527___data *)__out_argv) -> OUT__4__1527___data::B);
  ELM *tmpA = (ELM *)(((struct OUT__4__1527___data *)__out_argv) -> OUT__4__1527___data::tmpA);
  long _p_quarter = quarter;
  ELM *_p_A = A;
  ELM *_p_B = B;
  ELM *_p_tmpA = tmpA;
  cilkmerge_par(_p_A,((_p_A + _p_quarter) - 1),_p_B,((_p_B + _p_quarter) - 1),_p_tmpA);
}

static void OUT__5__1527__(void *__out_argv)
{
  long size = (long )(((struct OUT__5__1527___data *)__out_argv) -> OUT__5__1527___data::size);
  long quarter = (long )(((struct OUT__5__1527___data *)__out_argv) -> OUT__5__1527___data::quarter);
  ELM *D = (ELM *)(((struct OUT__5__1527___data *)__out_argv) -> OUT__5__1527___data::D);
  ELM *tmpD = (ELM *)(((struct OUT__5__1527___data *)__out_argv) -> OUT__5__1527___data::tmpD);
  long _p_size = size;
  long _p_quarter = quarter;
  ELM *_p_D = D;
  ELM *_p_tmpD = tmpD;
  cilksort_par(_p_D,_p_tmpD,(_p_size - ((3) * _p_quarter)));
}

static void OUT__6__1527__(void *__out_argv)
{
  long quarter = (long )(((struct OUT__6__1527___data *)__out_argv) -> OUT__6__1527___data::quarter);
  ELM *C = (ELM *)(((struct OUT__6__1527___data *)__out_argv) -> OUT__6__1527___data::C);
  ELM *tmpC = (ELM *)(((struct OUT__6__1527___data *)__out_argv) -> OUT__6__1527___data::tmpC);
  long _p_quarter = quarter;
  ELM *_p_C = C;
  ELM *_p_tmpC = tmpC;
  cilksort_par(_p_C,_p_tmpC,_p_quarter);
}

static void OUT__7__1527__(void *__out_argv)
{
  long quarter = (long )(((struct OUT__7__1527___data *)__out_argv) -> OUT__7__1527___data::quarter);
  ELM *B = (ELM *)(((struct OUT__7__1527___data *)__out_argv) -> OUT__7__1527___data::B);
  ELM *tmpB = (ELM *)(((struct OUT__7__1527___data *)__out_argv) -> OUT__7__1527___data::tmpB);
  long _p_quarter = quarter;
  ELM *_p_B = B;
  ELM *_p_tmpB = tmpB;
  cilksort_par(_p_B,_p_tmpB,_p_quarter);
}

static void OUT__8__1527__(void *__out_argv)
{
  long quarter = (long )(((struct OUT__8__1527___data *)__out_argv) -> OUT__8__1527___data::quarter);
  ELM *A = (ELM *)(((struct OUT__8__1527___data *)__out_argv) -> OUT__8__1527___data::A);
  ELM *tmpA = (ELM *)(((struct OUT__8__1527___data *)__out_argv) -> OUT__8__1527___data::tmpA);
  long _p_quarter = quarter;
  ELM *_p_A = A;
  ELM *_p_tmpA = tmpA;
  cilksort_par(_p_A,_p_tmpA,_p_quarter);
}

static void OUT__9__1527__(void *__out_argv)
{
  ELM *high1 = (ELM *)(((struct OUT__9__1527___data *)__out_argv) -> OUT__9__1527___data::high1);
  ELM *high2 = (ELM *)(((struct OUT__9__1527___data *)__out_argv) -> OUT__9__1527___data::high2);
  ELM *lowdest = (ELM *)(((struct OUT__9__1527___data *)__out_argv) -> OUT__9__1527___data::lowdest);
  ELM *split1 = (ELM *)(((struct OUT__9__1527___data *)__out_argv) -> OUT__9__1527___data::split1);
  ELM *split2 = (ELM *)(((struct OUT__9__1527___data *)__out_argv) -> OUT__9__1527___data::split2);
  long lowsize = (long )(((struct OUT__9__1527___data *)__out_argv) -> OUT__9__1527___data::lowsize);
  ELM *_p_high1 = high1;
  ELM *_p_high2 = high2;
  ELM *_p_lowdest = lowdest;
  ELM *_p_split1 = split1;
  ELM *_p_split2 = split2;
  long _p_lowsize = lowsize;
  cilkmerge_par((_p_split1 + 1),_p_high1,(_p_split2 + 1),_p_high2,((_p_lowdest + _p_lowsize) + 2));
}

static void OUT__10__1527__(void *__out_argv)
{
  ELM *low1 = (ELM *)(((struct OUT__10__1527___data *)__out_argv) -> OUT__10__1527___data::low1);
  ELM *low2 = (ELM *)(((struct OUT__10__1527___data *)__out_argv) -> OUT__10__1527___data::low2);
  ELM *lowdest = (ELM *)(((struct OUT__10__1527___data *)__out_argv) -> OUT__10__1527___data::lowdest);
  ELM *split1 = (ELM *)(((struct OUT__10__1527___data *)__out_argv) -> OUT__10__1527___data::split1);
  ELM *split2 = (ELM *)(((struct OUT__10__1527___data *)__out_argv) -> OUT__10__1527___data::split2);
  ELM *_p_low1 = low1;
  ELM *_p_low2 = low2;
  ELM *_p_lowdest = lowdest;
  ELM *_p_split1 = split1;
  ELM *_p_split2 = split2;
  cilkmerge_par(_p_low1,(_p_split1 - 1),_p_low2,_p_split2,_p_lowdest);
}
