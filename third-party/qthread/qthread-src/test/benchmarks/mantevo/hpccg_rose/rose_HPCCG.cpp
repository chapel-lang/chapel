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
// Routine to compute an approximate solution to Ax = b where:
// A - known matrix stored as an HPC_Sparse_Matrix struct
// b - known right hand side vector
// x - On entry is initial guess, on exit new approximate solution
// max_iter - Maximum number of iterations to perform, even if
//            tolerance is not met.
// tolerance - Stop and assert convergence if norm of residual is <=
//             to tolerance.
// niters - On output, the number of iterations actually performed.
/////////////////////////////////////////////////////////////////////////
#include <iostream>
#include "libxomp.h" 
using ::std::cout;
using ::std::cerr;
using ::std::endl;
#include <cmath>
#include "mytimer.hpp"
#include "HPCCG.hpp"
#define TICK()  t0 = mytimer() // Use TICK and TOCK to time a code section
#define TOCK(t) t += mytimer() - t0

int HPCCG(HPC_Sparse_Matrix *A,const double *const b,double *const x,const int max_iter,const double tolerance,int &niters,double &normr,double *times)
{
// Start timing right away
  double t_begin = mytimer();
  double t0 = 0.0;
  double t1 = 0.0;
  double t2 = 0.0;
  double t3 = 0.0;
#ifdef USING_MPI
#endif
  int nrow = (A -> HPC_Sparse_Matrix_STRUCT::local_nrow);
  int ncol = (A -> HPC_Sparse_Matrix_STRUCT::local_ncol);
  double *r = new double [nrow];
// In parallel case, A is rectangular
  double *p = new double [ncol];
  double *Ap = new double [nrow];
  normr = 0.0;
  double rtrans = 0.0;
  double oldrtrans = 0.0;
#ifdef USING_MPI
// Number of MPI processes, My process ID
#else
// Serial case (not using MPI)
  int size = 1;
  int rank = 0;
#endif
  int print_freq = (max_iter / 10);
  if (print_freq > 50) 
    print_freq = 50;
  if (print_freq < 1) 
    print_freq = 1;
// p is of length ncols, copy x to p for sparse MV operation
  t0 = mytimer();
  waxpby(nrow,1.0,(x),0.0,(x),p);
  t2 += (mytimer() - t0);
#ifdef USING_MPI
#endif
  t0 = mytimer();
  HPC_sparsemv(A,(p),Ap);
  t3 += (mytimer() - t0);
  t0 = mytimer();
  waxpby(nrow,1.0,b,(-1.0),(Ap),r);
  t2 += (mytimer() - t0);
  t0 = mytimer();
  ddot(nrow,(r),(r),&rtrans);
  t1 += (mytimer() - t0);
  normr = sqrt(rtrans);
  if (rank == 0) 
    (( *((&std::cout))<<"Initial Residual = ") << normr) << std::endl;
  for (int k = 1; (k < max_iter) && (normr > tolerance); k++) {
    if (k == 1) {
      t0 = mytimer();
      waxpby(nrow,1.0,(r),0.0,(r),p);
      t2 += (mytimer() - t0);
    }
    else {
      oldrtrans = rtrans;
// 2*nrow ops
      t0 = mytimer();
      ddot(nrow,(r),(r),&rtrans);
      t1 += (mytimer() - t0);
      double beta = (rtrans / oldrtrans);
// 2*nrow ops
      t0 = mytimer();
      waxpby(nrow,1.0,(r),beta,(p),p);
      t2 += (mytimer() - t0);
    }
    normr = sqrt(rtrans);
    if ((rank == 0) && (((k % print_freq) == 0) || ((k + 1) == max_iter))) 
      ((( *((&std::cout))<<"Iteration = ") << k<<"   Residual = ") << normr) << std::endl;
#ifdef USING_MPI
#endif
// 2*nnz ops
    t0 = mytimer();
    HPC_sparsemv(A,(p),Ap);
    t3 += (mytimer() - t0);
    double alpha = 0.0;
// 2*nrow ops
    t0 = mytimer();
    ddot(nrow,(p),(Ap),&alpha);
    t1 += (mytimer() - t0);
    alpha = (rtrans / alpha);
// 2*nrow ops
    t0 = mytimer();
    waxpby(nrow,1.0,(x),alpha,(p),x);
// 2*nrow ops
    waxpby(nrow,1.0,(r),-alpha,(Ap),r);
    t2 += (mytimer() - t0);
    niters = k;
  }
// Store times
// waxpby time
  times[1] = t1;
// sparsemv time
  times[2] = t2;
// ddot time
  times[3] = t3;
#ifdef USING_MPI
// exchange boundary time
#endif
  delete []p;
  delete []Ap;
  delete []r;
// Total time. All done...
  times[0] = (mytimer() - t_begin);
  return 0;
}
