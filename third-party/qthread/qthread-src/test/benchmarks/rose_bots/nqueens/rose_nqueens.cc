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
 * Original code from the Cilk project (by Keith Randall)
 * 
 * Copyright (c) 2000 Massachusetts Institute of Technology
 * Copyright (c) 2000 Matteo Frigo
 */
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <alloca.h>
#include "bots.h"
//#include <omp.h>
/* Checking information */
#include "libxomp.h" 
static int solutions[] = {(1), (0), (0), (2), (10), (4), (40), (92), (352), (724), (2680), (14200), (73712), (365596)
/* 5 */
/* 10 */
};
#define MAX_SOLUTIONS sizeof(solutions)/sizeof(int)
#ifdef FORCE_TIED_TASKS
__thread int mycount = 0;
#endif
int total_count;
/*
 * <a> contains array of <n> queen positions.  Returns 1
 * if none of the queens conflict, and returns 0 otherwise.
 */

int ok(int n,char *a)
{
  int i;
  int j;
  char p;
  char q;
  for (i = 0; i < n; i++) {
    p = a[i];
    for (j = (i + 1); j < n; j++) {
      q = a[j];
      if ((((q) == (p)) || ((q) == ((p) - (j - i)))) || ((q) == ((p) + (j - i)))) 
        return 0;
    }
  }
  return 1;
}

void nqueens_ser(int n,int j,char *a,int *solutions)
{
  int i;
  int res;
  if (n == j) {
/* good solution, count it */
#ifndef FORCE_TIED_TASKS
#else
    mycount++;
#endif
    return ;
  }
#ifndef FORCE_TIED_TASKS
#endif
/* try each possible position for queen <j> */
  for (i = 0; i < n; i++) {{
/* allocate a temporary array and copy <a> into it */
      a[j] = (i);
      if ((ok((j + 1),a))) {
        nqueens_ser(n,(j + 1),a,&res);
#ifndef FORCE_TIED_TASKS
#endif
      }
    }
  }
}
#if defined(IF_CUTOFF)
/* good solution, count it */
#ifndef FORCE_TIED_TASKS
#else
#endif
#ifndef FORCE_TIED_TASKS
#endif
/* try each possible position for queen <j> */
/* allocate a temporary array and copy <a> into it */
#ifndef FORCE_TIED_TASKS
#endif
#elif defined(FINAL_CUTOFF)
/* good solution, count it */
#ifndef FORCE_TIED_TASKS
#else
#endif
#ifndef FORCE_TIED_TASKS
#endif
/* try each possible position for queen <j> */
/* allocate a temporary array and copy <a> into it */
#ifndef FORCE_TIED_TASKS
#endif
#elif defined(MANUAL_CUTOFF)

struct OUT__2__1527___data 
{
  int n;
  int j;
  char *a;
  int depth;
  int i;
  int *csols;
}
;
static void OUT__2__1527__(void *__out_argv);

void nqueens(int n,int j,char *a,int *solutions,int depth)
{
  int i;
  int *csols;
  if (n == j) {
/* good solution, count it */
#ifndef FORCE_TIED_TASKS
#else
    mycount++;
#endif
    return ;
  }
#ifndef FORCE_TIED_TASKS
#endif
/* try each possible position for queen <j> */
  for (i = 0; i < n; i++) {
    if (depth < bots_cutoff_value) {
      struct OUT__2__1527___data __out_argv2__1527__;
      __out_argv2__1527__.OUT__2__1527___data::csols = csols;
      __out_argv2__1527__.OUT__2__1527___data::i = i;
      __out_argv2__1527__.OUT__2__1527___data::depth = depth;
      __out_argv2__1527__.OUT__2__1527___data::a = a;
      __out_argv2__1527__.OUT__2__1527___data::j = j;
      __out_argv2__1527__.OUT__2__1527___data::n = n;
      XOMP_task(OUT__2__1527__,&__out_argv2__1527__,0,sizeof(struct OUT__2__1527___data ),4,1,1);
    }
    else {
      a[j] = (i);
      if ((ok((j + 1),a))) 
        nqueens_ser(n,(j + 1),a,(csols + i));
    }
  }
  XOMP_taskwait();
#ifndef FORCE_TIED_TASKS
#endif
}
#else 
/* good solution, count it */
#ifndef FORCE_TIED_TASKS
#else
#endif
#ifndef FORCE_TIED_TASKS
#endif
/* try each possible position for queen <j> */
/* allocate a temporary array and copy <a> into it */
#ifndef FORCE_TIED_TASKS
#endif
#endif

struct OUT__1__1527___data 
{
  void *size_p;
}
;
static void OUT__1__1527__(void *__out_argv);

void find_queens(int size)
{
  total_count = 0;
{
    if ((bots_verbose_mode) >= (BOTS_VERBOSE_DEFAULT)) {
      fprintf(stdout,"Computing N-Queens algorithm (n=%d) ",size);
    }
  }
  struct OUT__1__1527___data __out_argv1__1527__;
  __out_argv1__1527__.OUT__1__1527___data::size_p = ((void *)(&size));
  XOMP_parallel_start(OUT__1__1527__,&__out_argv1__1527__,1,0,"OUT__1__1527__");
  XOMP_parallel_end();
{
    if ((bots_verbose_mode) >= (BOTS_VERBOSE_DEFAULT)) {
      fprintf(stdout," completed!\n");
    }
  }
}

int verify_queens(int size)
{
  if ((size_t)(size) > ((sizeof(solutions)) / (sizeof(int )))) 
    return 0;
  if (total_count == solutions[size - 1]) 
    return 1;
  return 2;
}

static void OUT__1__1527__(void *__out_argv)
{
  int *size = (int *)(((struct OUT__1__1527___data *)__out_argv) -> OUT__1__1527___data::size_p);
  if (XOMP_single()) {
    char *a;
    a = ((char *)(__builtin_alloca(((( *size)) * (sizeof(char ))))));
    nqueens( *size,0,a,&total_count,0);
  }
  XOMP_barrier();
#ifdef FORCE_TIED_TASKS
  XOMP_atomic_start();
  total_count += mycount;
  XOMP_atomic_end();
#endif
}

static void OUT__2__1527__(void *__out_argv)
{
  int n = (int )(((struct OUT__2__1527___data *)__out_argv) -> OUT__2__1527___data::n);
  int j = (int )(((struct OUT__2__1527___data *)__out_argv) -> OUT__2__1527___data::j);
  char *a = (char *)(((struct OUT__2__1527___data *)__out_argv) -> OUT__2__1527___data::a);
  int depth = (int )(((struct OUT__2__1527___data *)__out_argv) -> OUT__2__1527___data::depth);
  int i = (int )(((struct OUT__2__1527___data *)__out_argv) -> OUT__2__1527___data::i);
  int *csols = (int *)(((struct OUT__2__1527___data *)__out_argv) -> OUT__2__1527___data::csols);
  int _p_n = n;
  int _p_j = j;
  char *_p_a = a;
  int _p_depth = depth;
  int _p_i = i;
  int *_p_csols = csols;
/* allocate a temporary array and copy <a> into it */
  char *b = (char *)(__builtin_alloca((((_p_j + 1)) * (sizeof(char )))));
  memcpy((b),(_p_a),(((_p_j) * (sizeof(char )))));
  b[_p_j] = (_p_i);
  if ((ok((_p_j + 1),b))) 
    nqueens(_p_n,(_p_j + 1),b,(_p_csols + _p_i),(_p_depth + 1));
}
