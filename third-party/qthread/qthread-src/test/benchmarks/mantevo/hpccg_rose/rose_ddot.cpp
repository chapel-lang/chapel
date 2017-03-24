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
// Routine to compute the dot product of two vectors where:
// n - number of vector elements (on this processor)
// x, y - input vectors
// residual - pointer to scalar value, on exit will contain result.
/////////////////////////////////////////////////////////////////////////
#include "ddot.hpp"
#include "libxomp.h" 

struct OUT__1__1527___data 
{
  void *n_p;
  void *x_p;
  void *y_p;
  void *dot_p;
}
;
static void OUT__1__1527__(void *__out_argv);

struct OUT__2__1527___data 
{
  void *n_p;
  void *x_p;
  void *dot_p;
}
;
static void OUT__2__1527__(void *__out_argv);

int ddot(const int n,const double *const x,const double *const y,double *const result)
{
  double dot = 0.0;
  if (y == x) {
    int i;
    struct OUT__2__1527___data __out_argv2__1527__;
    __out_argv2__1527__.OUT__2__1527___data::dot_p = ((void *)(&dot));
    __out_argv2__1527__.OUT__2__1527___data::x_p = ((void *)(&x));
    __out_argv2__1527__.OUT__2__1527___data::n_p = ((void *)(&n));
    XOMP_parallel_start(OUT__2__1527__,&__out_argv2__1527__,1,0,"OUT__2__1527__");
    XOMP_parallel_end();
  }
  else {
    int i;
    struct OUT__1__1527___data __out_argv1__1527__;
    __out_argv1__1527__.OUT__1__1527___data::dot_p = ((void *)(&dot));
    __out_argv1__1527__.OUT__1__1527___data::y_p = ((void *)(&y));
    __out_argv1__1527__.OUT__1__1527___data::x_p = ((void *)(&x));
    __out_argv1__1527__.OUT__1__1527___data::n_p = ((void *)(&n));
    XOMP_parallel_start(OUT__1__1527__,&__out_argv1__1527__,1,0,"OUT__1__1527__");
    XOMP_parallel_end();
  }
   *result = dot;
#ifdef USING_MPI
// Use MPI's reduce function to collect all partial sums
#endif
  return 0;
}

static void OUT__1__1527__(void *__out_argv)
{
  const int *n = (const int *)(((struct OUT__1__1527___data *)__out_argv) -> OUT__1__1527___data::n_p);
  const double *const *x = (const double *const *)(((struct OUT__1__1527___data *)__out_argv) -> OUT__1__1527___data::x_p);
  const double *const *y = (const double *const *)(((struct OUT__1__1527___data *)__out_argv) -> OUT__1__1527___data::y_p);
  double *dot = (double *)(((struct OUT__1__1527___data *)__out_argv) -> OUT__1__1527___data::dot_p);
  double _p_dot;
  _p_dot = 0;
  int _p_i;
  long p_index_;
  long p_lower_;
  long p_upper_;
  XOMP_loop_default(0, *n - 1,1,&p_lower_,&p_upper_);
  for (p_index_ = p_lower_; p_index_ <= p_upper_; p_index_ += 1) {
    _p_dot += (( *x)[p_index_] * ( *y)[p_index_]);
  }
  XOMP_atomic_start();
   *dot =  *dot + _p_dot;
  XOMP_atomic_end();
  XOMP_barrier();
}

static void OUT__2__1527__(void *__out_argv)
{
  const int *n = (const int *)(((struct OUT__2__1527___data *)__out_argv) -> OUT__2__1527___data::n_p);
  const double *const *x = (const double *const *)(((struct OUT__2__1527___data *)__out_argv) -> OUT__2__1527___data::x_p);
  double *dot = (double *)(((struct OUT__2__1527___data *)__out_argv) -> OUT__2__1527___data::dot_p);
  double _p_dot;
  _p_dot = 0;
  int _p_i;
  long p_index_;
  long p_lower_;
  long p_upper_;
  XOMP_loop_default(0, *n - 1,1,&p_lower_,&p_upper_);
  for (p_index_ = p_lower_; p_index_ <= p_upper_; p_index_ += 1) {
    _p_dot += (( *x)[p_index_] * ( *x)[p_index_]);
  }
  XOMP_atomic_start();
   *dot =  *dot + _p_dot;
  XOMP_atomic_end();
  XOMP_barrier();
}
