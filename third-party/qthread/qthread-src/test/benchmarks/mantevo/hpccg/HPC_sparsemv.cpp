// @HEADER
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
// @HEADER

// ///////////////////////////////////////////////////////////////////////

// Routine to compute matrix vector product y = Ax where:
// First call exchange_externals to get off-processor values of x

// A - known matrix
// x - known vector
// y - On exit contains Ax.

// ///////////////////////////////////////////////////////////////////////

#include <iostream>
using std::cerr;
using std::cout;
using std::endl;
#include "HPC_sparsemv.hpp"
#include "loop_defines.h"
#include <cassert>
#include <cctype>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <string>

#if defined(USING_QTHREADS) && !defined(CXX_LAMBDAS)

class HPC_sparsemv_outerloop {
public:
  HPC_sparsemv_outerloop(HPC_Sparse_Matrix *const a,
                         double const *const x,
                         double *const y):
    A(a), x(x), y(y), ptr_to_vals_in_row(a->ptr_to_vals_in_row),
    ptr_to_inds_in_row(a->ptr_to_inds_in_row), nnz_in_row(a->nnz_in_row) {}

  void operator()(size_t const startat, size_t const stopat) {
    for (size_t i = startat; i < stopat; ++i) {
      double sum = 0.0;
      unsigned int const cur_nnz = nnz_in_row[i];
      double const *const cur_vals = ptr_to_vals_in_row[i];
      int const *const cur_inds = ptr_to_inds_in_row[i];
      /* this inner-loop is too small to be worth parallelizing for now */
      for (unsigned int j = 0; j < cur_nnz; j++)
        sum += cur_vals[j] * x[cur_inds[j]];
      y[i] = sum;
    }
  }
private:
  HPC_Sparse_Matrix *const A;
  double const *const x;
  double *const y;
  double const *const *const ptr_to_vals_in_row;
  int const *const *const ptr_to_inds_in_row;
  int const *const nnz_in_row;
};

int HPC_sparsemv(HPC_Sparse_Matrix *A, double const *const x, double *const y) {
  int const nrow = (int)A->local_nrow;
  extern int tcount;

  if (tcount > 0) {
    HPC_sparsemv_outerloop loop(A, x, y);
    qt_loop_balance(0, nrow, loop);
  } else {
    for (int i = 0; i < nrow; i++) {
      double sum = 0.0;
      double const *const cur_vals = (double const *)A->ptr_to_vals_in_row[i];
      int const *const cur_inds = (int const *)A->ptr_to_inds_in_row[i];
      int const cur_nnz = (int)A->nnz_in_row[i];

      for (int j = 0; j < cur_nnz; j++) sum += cur_vals[j] * x[cur_inds[j]];
      y[i] = sum;
    }
  }
  return (0);
}

#else // if defined(USING_QTHREADS) && !defined(CXX_LAMBDAS)

int HPC_sparsemv(HPC_Sparse_Matrix *A, double const *const x, double *const y) {
  int const nrow = (int)A->local_nrow;

  LOOP_BEGIN(0, nrow, start, stop);
  for (unsigned int i = start; i < stop; ++i) {
    double sum = 0.0;
    double const *const cur_vals = (double const *)A->ptr_to_vals_in_row[i];
    int const *const cur_inds = (int const *)A->ptr_to_inds_in_row[i];
    int const cur_nnz = (int)A->nnz_in_row[i];

    for (int j = 0; j < cur_nnz; j++) sum += cur_vals[j] * x[cur_inds[j]];
    y[i] = sum;
  }
  LOOP_END();
  return (0);
}

#endif // if defined(USING_QTHREADS) && !defined(CXX_LAMBDAS)

/* vim:set expandtab: */
