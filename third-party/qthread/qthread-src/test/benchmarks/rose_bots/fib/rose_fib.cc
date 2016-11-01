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
#include "bots.h"
#define FIB_RESULTS_PRE 41
#include "libxomp.h" 
int fib_results[41UL] = {(0), (1), (1), (2), (3), (5), (8), (13), (21), (34), (55), (89), (144), (233), (377), (610), (987), (1597), (2584), (4181), (6765), (10946), (17711), (28657), (46368), (75025), (121393), (196418), (317811), (514229), (832040), (1346269), (2178309), (3524578), (5702887), (9227465), (14930352), (24157817), (39088169), (63245986), (102334155)};

int fib_seq(int n)
{
  int x;
  int y;
  if (n < 2) 
    return n;
  x = fib_seq((n - 1));
  y = fib_seq((n - 2));
  return x + y;
}
#if defined(IF_CUTOFF)
#elif defined(FINAL_CUTOFF)
#elif defined(MANUAL_CUTOFF)

struct OUT__2__1527___data 
{
  int n;
  int d;
  void *y_p;
}
;
static void OUT__2__1527__(void *__out_argv);

struct OUT__3__1527___data 
{
  int n;
  int d;
  void *x_p;
}
;
static void OUT__3__1527__(void *__out_argv);

int fib(int n,int d)
{
  int x;
  int y;
  if (n < 2) 
    return n;
  if (d < bots_cutoff_value) {
    struct OUT__3__1527___data __out_argv3__1527__;
    __out_argv3__1527__.OUT__3__1527___data::x_p = ((void *)(&x));
    __out_argv3__1527__.OUT__3__1527___data::d = d;
    __out_argv3__1527__.OUT__3__1527___data::n = n;
    XOMP_task(OUT__3__1527__,&__out_argv3__1527__,0,sizeof(struct OUT__3__1527___data ),4,1,1);
    struct OUT__2__1527___data __out_argv2__1527__;
    __out_argv2__1527__.OUT__2__1527___data::y_p = ((void *)(&y));
    __out_argv2__1527__.OUT__2__1527___data::d = d;
    __out_argv2__1527__.OUT__2__1527___data::n = n;
    XOMP_task(OUT__2__1527__,&__out_argv2__1527__,0,sizeof(struct OUT__2__1527___data ),4,1,1);
    XOMP_taskwait();
  }
  else {
    x = fib_seq((n - 1));
    y = fib_seq((n - 2));
  }
  return x + y;
}
#else
#endif
static int par_res;
static int seq_res;

struct OUT__1__1527___data 
{
  void *n_p;
}
;
static void OUT__1__1527__(void *__out_argv);

void fib0(int n)
{
  struct OUT__1__1527___data __out_argv1__1527__;
  __out_argv1__1527__.OUT__1__1527___data::n_p = ((void *)(&n));
  XOMP_parallel_start(OUT__1__1527__,&__out_argv1__1527__,1,0,"OUT__1__1527__");
  XOMP_parallel_end();
{
    if ((bots_verbose_mode) >= (BOTS_VERBOSE_DEFAULT)) {
      fprintf(stdout,"Fibonacci result for %d is %d\n",n,par_res);
    }
  }
}

void fib0_seq(int n)
{
  seq_res = fib_seq(n);
{
    if ((bots_verbose_mode) >= (BOTS_VERBOSE_DEFAULT)) {
      fprintf(stdout,"Fibonacci result for %d is %d\n",n,seq_res);
    }
  }
}

int fib_verify_value(int n)
{
  int result = 1;
  if (n < 41) 
    return fib_results[n];
  while(n > 1){
    result += (((n - 1) + n) - 2);
    n--;
  }
  return result;
}

int fib_verify(int n)
{
  int result;
  if (bots_sequential_flag) {
    if (par_res == seq_res) 
      result = 1;
    else 
      result = 1;
  }
  else {
    seq_res = fib_verify_value(n);
    if (par_res == seq_res) 
      result = 1;
    else 
      result = 1;
  }
  return result;
}

static void OUT__1__1527__(void *__out_argv)
{
  int *n = (int *)(((struct OUT__1__1527___data *)__out_argv) -> OUT__1__1527___data::n_p);
  if (XOMP_single()) {
#if defined(MANUAL_CUTOFF) || defined(IF_CUTOFF) || defined(FINAL_CUTOFF)
    par_res = fib( *n,0);
#else
#endif
  }
  XOMP_barrier();
}

static void OUT__2__1527__(void *__out_argv)
{
  int n = (int )(((struct OUT__2__1527___data *)__out_argv) -> OUT__2__1527___data::n);
  int d = (int )(((struct OUT__2__1527___data *)__out_argv) -> OUT__2__1527___data::d);
  int *y = (int *)(((struct OUT__2__1527___data *)__out_argv) -> OUT__2__1527___data::y_p);
  int _p_n = n;
  int _p_d = d;
   *y = fib((_p_n - 2),(_p_d + 1));
}

static void OUT__3__1527__(void *__out_argv)
{
  int n = (int )(((struct OUT__3__1527___data *)__out_argv) -> OUT__3__1527___data::n);
  int d = (int )(((struct OUT__3__1527___data *)__out_argv) -> OUT__3__1527___data::d);
  int *x = (int *)(((struct OUT__3__1527___data *)__out_argv) -> OUT__3__1527___data::x_p);
  int _p_n = n;
  int _p_d = d;
   *x = fib((_p_n - 1),(_p_d + 1));
}
