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
// Routine to compute matrix vector product y = Ax where:
// First call exchange_externals to get off-processor values of x
// A - known matrix 
// x - known vector
// y - On exit contains Ax.
/////////////////////////////////////////////////////////////////////////
#include <iostream>
#include "libxomp.h" 
using ::std::cout;
using ::std::cerr;
using ::std::endl;
#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <cassert>
#include <string>
#include <cmath>
#include "HPC_sparsemv.hpp"

struct OUT__1__1527___data 
{
  void *A_p;
  void *x_p;
  void *y_p;
  void *nrow_p;
}
;
static void OUT__1__1527__(void *__out_argv);

int HPC_sparsemv(HPC_Sparse_Matrix *A,const double *const x,double *const y)
{
  const int nrow = (A -> HPC_Sparse_Matrix_STRUCT::local_nrow);
  int i;
  struct OUT__1__1527___data __out_argv1__1527__;
  __out_argv1__1527__.OUT__1__1527___data::nrow_p = ((void *)(&nrow));
  __out_argv1__1527__.OUT__1__1527___data::y_p = ((void *)(&y));
  __out_argv1__1527__.OUT__1__1527___data::x_p = ((void *)(&x));
  __out_argv1__1527__.OUT__1__1527___data::A_p = ((void *)(&A));
  XOMP_parallel_start(OUT__1__1527__,&__out_argv1__1527__,1,0,"OUT__1__1527__");
  XOMP_parallel_end();
  return 0;
}

static void OUT__1__1527__(void *__out_argv)
{
  HPC_Sparse_Matrix **A = (HPC_Sparse_Matrix **)(((struct OUT__1__1527___data *)__out_argv) -> OUT__1__1527___data::A_p);
  const double *const *x = (const double *const *)(((struct OUT__1__1527___data *)__out_argv) -> OUT__1__1527___data::x_p);
  double *const *y = (double *const *)(((struct OUT__1__1527___data *)__out_argv) -> OUT__1__1527___data::y_p);
  const int *nrow = (const int *)(((struct OUT__1__1527___data *)__out_argv) -> OUT__1__1527___data::nrow_p);
  int _p_i;
  long p_index_;
  long p_lower_;
  long p_upper_;
  XOMP_loop_default(0, *nrow - 1,1,&p_lower_,&p_upper_);
  for (p_index_ = p_lower_; p_index_ <= p_upper_; p_index_ += 1) {
    double sum = 0.0;
    const double *const cur_vals = (const double *)(( *A) -> HPC_Sparse_Matrix_STRUCT::ptr_to_vals_in_row)[p_index_];
    const int *const cur_inds = (const int *)(( *A) -> HPC_Sparse_Matrix_STRUCT::ptr_to_inds_in_row)[p_index_];
    const int cur_nnz = (( *A) -> HPC_Sparse_Matrix_STRUCT::nnz_in_row)[p_index_];
    for (int j = 0; j < cur_nnz; j++) 
      sum += (cur_vals[j] * ( *x)[cur_inds[j]]);
    ( *y)[p_index_] = sum;
  }
  XOMP_barrier();
}
