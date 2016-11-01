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
// Changelog
//
// Version 0.3
// - Added timing of setup time for sparse MV
// - Corrected percentages reported for sparse MV with overhead
//
/////////////////////////////////////////////////////////////////////////
// Main routine of a program that reads a sparse matrix, right side
// vector, solution vector and initial guess from a file  in HPC
// format.  This program then calls the HPCCG conjugate gradient
// solver to solve the problem, and then prints results.
// Calling sequence:
// test_HPCCG linear_system_file
// Routines called:
// read_HPC_row - Reads in linear system
// mytimer - Timing routine (compile with -DWALL to get wall clock
//           times
// HPCCG - CG Solver
// compute_residual - Compares HPCCG solution to known solution.
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
#ifdef USING_MPI
#include <mpi.h> // If this routine is compiled with -DUSING_MPI
// then include mpi.h
#include "make_local_matrix.hpp" // Also include this function
#endif
#include "generate_matrix.hpp"
#include "read_HPC_row.hpp"
#include "mytimer.hpp"
#include "HPC_sparsemv.hpp"
#include "compute_residual.hpp"
#include "HPCCG.hpp"
#include "HPC_Sparse_Matrix.hpp"
#undef DEBUG

int main(int argc,char *argv[])
{
  int status = 0;
  XOMP_init(argc,argv);
  HPC_Sparse_Matrix *A;
  double *x;
  double *b;
  double *xexact;
  double norm;
  double d;
  int ierr = 0;
  int i;
  int j;
  int ione = 1;
  double times[6UL];
#ifdef USING_MPI
// Initialize MPI
// Number of MPI processes, My process ID
// I'm alive !!!
#else
// Serial case (not using MPI)
  int size = 1;
  int rank = 0;
#endif
#ifdef DEBUG
#endif
  if ((argc != 2) && (argc != 4)) {
    if (rank == 0) 
      ((((( *((&std::cerr))<<"Usage:") << std::endl<<"Mode 1: "<<(argv[0])<<" nx ny nz") << std::endl<<"     where nx, ny and nz are the local sub-block dimensions, or") << std::endl<<"Mode 2: "<<(argv[0])<<" HPC_data_file ") << std::endl<<"     where HPC_data_file is a globally accessible file containing matrix data.") << std::endl;
    exit(1);
  }
  if (argc == 4) {
    int nx = atoi((argv[1]));
    int ny = atoi((argv[2]));
    int nz = atoi((argv[3]));
    generate_matrix(nx,ny,nz,&A,&x,&b,&xexact);
  }
  else 
    read_HPC_row(argv[1],&A,&x,&b,&xexact);
#ifdef USING_MPI
// Transform matrix indices from global to local values.
// Define number of columns for the local matrix.
#endif
// Initialize it (if needed)
  double t1 = mytimer();
  int niters = 0;
  double normr = 0.0;
// SLO: original default: int max_iter = 500;
  int max_iter = 150;
// SLO: original default: double tolerance = 3.0E-15;
  double tolerance = (0);
  ierr = HPCCG(A,(b),x,max_iter,tolerance,niters,normr,times);
  if (ierr) 
    (( *((&std::cerr))<<"Error in call to CG: ") << ierr<<".\n") << std::endl;
// Only PE 0 needs to compute and report timing results
  if (rank == 0) {
    (( *((&std::cout))<<"Time spent in CG = ") << times[0]<<".\n") << std::endl;
    double fniters = niters;
    double fnrow = (A -> HPC_Sparse_Matrix_STRUCT::total_nrow);
    double fnnz = (A -> HPC_Sparse_Matrix_STRUCT::total_nnz);
    double fnops_ddot = ((fniters * (4)) * fnrow);
    double fnops_waxpby = ((fniters * (6)) * fnrow);
    double fnops_sparsemv = ((fniters * (2)) * fnnz);
    double fnops = ((fnops_ddot + fnops_waxpby) + fnops_sparsemv);
    (( *((&std::cout))<<"Number of iterations = ") << niters<<".\n") << std::endl;
    (( *((&std::cout))<<"Final residual = ") << normr<<".\n") << std::endl;
    (( *((&std::cout))<<"********** Performance Summary (times in sec) ***********") << std::endl) << std::endl;
    (((( *((&std::cout))<<"Total Time/FLOPS/MFLOPS               = ") << times[0]<<"/") << fnops<<"/") << ((fnops / times[0]) / 1.0E6)<<".\n") << std::endl;
    (((( *((&std::cout))<<"DDOT  Time/FLOPS/MFLOPS               = ") << times[1]<<"/") << fnops_ddot<<"/") << ((fnops_ddot / times[1]) / 1.0E6)<<".\n") << std::endl;
    (((( *((&std::cout))<<"WAXPBY Time/FLOPS/MFLOPS              = ") << times[2]<<"/") << fnops_waxpby<<"/") << ((fnops_waxpby / times[2]) / 1.0E6)<<".\n") << std::endl;
    (((( *((&std::cout))<<"SPARSEMV Time/FLOPS/MFLOPS            = ") << times[3]<<"/") << fnops_sparsemv<<"/") << ((fnops_sparsemv / times[3]) / 1.0E6)<<".\n") << std::endl;
#ifdef USING_MPI
#endif
  }
// Compute difference between known exact solution and computed solution
// All processors are needed here.
  double residual = (0);
  if ((ierr = compute_residual((A -> HPC_Sparse_Matrix_STRUCT::local_nrow),(x),(xexact),&residual))) 
    (( *((&std::cerr))<<"Error in call to compute_residual: ") << ierr<<".\n") << std::endl;
  if (rank == 0) 
    (( *((&std::cout))<<"Difference between computed and exact  = ") << residual<<".\n") << std::endl;
// Finish up
#ifdef USING_MPI
#endif
  XOMP_terminate(status);
  return 0;
}
