//@HEADER
// ************************************************************************
// 
//               HPCCG: Simple Conjugate Gradient Benchmark Code
//                 Copyright (2006) Sandia Corporation
// 
// Under terms of Contract DE-AC04-94AL85000, there is a non-exclusive
// license for use of this work by or on behalf of the U.S. Government.
// 
// This library is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation; either version 2.1 of the
// License, or (at your option) any later version.
//  
// This library is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//  
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA
// Questions? Contact Michael A. Heroux (maherou@sandia.gov) 
// 
// ************************************************************************
//@HEADER
/////////////////////////////////////////////////////////////////////////
// Routine to compute the update of a vector with the sum of two 
// scaled vectors where:
// w = alpha*x + beta*y
// n - number of vector elements (on this processor)
// x, y - input vectors
// alpha, beta - scalars applied to x and y respectively.
// w - output vector.
/////////////////////////////////////////////////////////////////////////
#include "waxpby.hpp"
#include "libxomp.h" 

struct OUT__1__1527___data 
{
  void *n_p;
  void *alpha_p;
  void *x_p;
  void *beta_p;
  void *y_p;
  void *w_p;
}
;
static void OUT__1__1527__(void *__out_argv);

struct OUT__2__1527___data 
{
  void *n_p;
  void *alpha_p;
  void *x_p;
  void *y_p;
  void *w_p;
}
;
static void OUT__2__1527__(void *__out_argv);

struct OUT__3__1527___data 
{
  void *n_p;
  void *x_p;
  void *beta_p;
  void *y_p;
  void *w_p;
}
;
static void OUT__3__1527__(void *__out_argv);

int waxpby(const int n,const double alpha,const double *const x,const double beta,const double *const y,double *const w)
{
  int i;
  if (alpha == 1.0) {
    struct OUT__3__1527___data __out_argv3__1527__;
    __out_argv3__1527__.OUT__3__1527___data::w_p = ((void *)(&w));
    __out_argv3__1527__.OUT__3__1527___data::y_p = ((void *)(&y));
    __out_argv3__1527__.OUT__3__1527___data::beta_p = ((void *)(&beta));
    __out_argv3__1527__.OUT__3__1527___data::x_p = ((void *)(&x));
    __out_argv3__1527__.OUT__3__1527___data::n_p = ((void *)(&n));
    XOMP_parallel_start(OUT__3__1527__,&__out_argv3__1527__,1,0,"OUT__3__1527__");
    XOMP_parallel_end();
  }
  else if (beta == 1.0) {
    struct OUT__2__1527___data __out_argv2__1527__;
    __out_argv2__1527__.OUT__2__1527___data::w_p = ((void *)(&w));
    __out_argv2__1527__.OUT__2__1527___data::y_p = ((void *)(&y));
    __out_argv2__1527__.OUT__2__1527___data::x_p = ((void *)(&x));
    __out_argv2__1527__.OUT__2__1527___data::alpha_p = ((void *)(&alpha));
    __out_argv2__1527__.OUT__2__1527___data::n_p = ((void *)(&n));
    XOMP_parallel_start(OUT__2__1527__,&__out_argv2__1527__,1,0,"OUT__2__1527__");
    XOMP_parallel_end();
  }
  else {
    struct OUT__1__1527___data __out_argv1__1527__;
    __out_argv1__1527__.OUT__1__1527___data::w_p = ((void *)(&w));
    __out_argv1__1527__.OUT__1__1527___data::y_p = ((void *)(&y));
    __out_argv1__1527__.OUT__1__1527___data::beta_p = ((void *)(&beta));
    __out_argv1__1527__.OUT__1__1527___data::x_p = ((void *)(&x));
    __out_argv1__1527__.OUT__1__1527___data::alpha_p = ((void *)(&alpha));
    __out_argv1__1527__.OUT__1__1527___data::n_p = ((void *)(&n));
    XOMP_parallel_start(OUT__1__1527__,&__out_argv1__1527__,1,0,"OUT__1__1527__");
    XOMP_parallel_end();
  }
  return 0;
}

static void OUT__1__1527__(void *__out_argv)
{
  const int *n = (const int *)(((struct OUT__1__1527___data *)__out_argv) -> OUT__1__1527___data::n_p);
  const double *alpha = (const double *)(((struct OUT__1__1527___data *)__out_argv) -> OUT__1__1527___data::alpha_p);
  const double *const *x = (const double *const *)(((struct OUT__1__1527___data *)__out_argv) -> OUT__1__1527___data::x_p);
  const double *beta = (const double *)(((struct OUT__1__1527___data *)__out_argv) -> OUT__1__1527___data::beta_p);
  const double *const *y = (const double *const *)(((struct OUT__1__1527___data *)__out_argv) -> OUT__1__1527___data::y_p);
  double *const *w = (double *const *)(((struct OUT__1__1527___data *)__out_argv) -> OUT__1__1527___data::w_p);
  int _p_i;
  long p_index_;
  long p_lower_;
  long p_upper_;
  XOMP_loop_default(0, *n - 1,1,&p_lower_,&p_upper_);
  for (p_index_ = p_lower_; p_index_ <= p_upper_; p_index_ += 1) {
    ( *w)[p_index_] = (( *alpha * ( *x)[p_index_]) + ( *beta * ( *y)[p_index_]));
  }
  XOMP_barrier();
}

static void OUT__2__1527__(void *__out_argv)
{
  const int *n = (const int *)(((struct OUT__2__1527___data *)__out_argv) -> OUT__2__1527___data::n_p);
  const double *alpha = (const double *)(((struct OUT__2__1527___data *)__out_argv) -> OUT__2__1527___data::alpha_p);
  const double *const *x = (const double *const *)(((struct OUT__2__1527___data *)__out_argv) -> OUT__2__1527___data::x_p);
  const double *const *y = (const double *const *)(((struct OUT__2__1527___data *)__out_argv) -> OUT__2__1527___data::y_p);
  double *const *w = (double *const *)(((struct OUT__2__1527___data *)__out_argv) -> OUT__2__1527___data::w_p);
  int _p_i;
  long p_index_;
  long p_lower_;
  long p_upper_;
  XOMP_loop_default(0, *n - 1,1,&p_lower_,&p_upper_);
  for (p_index_ = p_lower_; p_index_ <= p_upper_; p_index_ += 1) {
    ( *w)[p_index_] = (( *alpha * ( *x)[p_index_]) + ( *y)[p_index_]);
  }
  XOMP_barrier();
}

static void OUT__3__1527__(void *__out_argv)
{
  const int *n = (const int *)(((struct OUT__3__1527___data *)__out_argv) -> OUT__3__1527___data::n_p);
  const double *const *x = (const double *const *)(((struct OUT__3__1527___data *)__out_argv) -> OUT__3__1527___data::x_p);
  const double *beta = (const double *)(((struct OUT__3__1527___data *)__out_argv) -> OUT__3__1527___data::beta_p);
  const double *const *y = (const double *const *)(((struct OUT__3__1527___data *)__out_argv) -> OUT__3__1527___data::y_p);
  double *const *w = (double *const *)(((struct OUT__3__1527___data *)__out_argv) -> OUT__3__1527___data::w_p);
  int _p_i;
  long p_index_;
  long p_lower_;
  long p_upper_;
  XOMP_loop_default(0, *n - 1,1,&p_lower_,&p_upper_);
  for (p_index_ = p_lower_; p_index_ <= p_upper_; p_index_ += 1) {
    ( *w)[p_index_] = (( *x)[p_index_] + ( *beta * ( *y)[p_index_]));
  }
  XOMP_barrier();
}
