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
// Routine to compute the 1-norm difference between two vectors where:
// n - number of vector elements (on this processor)
// v1, v2 - input vectors
// residual - pointer to scalar value, on exit will contain result.
/////////////////////////////////////////////////////////////////////////
#include <cmath>  // needed for fabs
#include "libxomp.h" 
using ::std::fabs;
#include "compute_residual.hpp"

int compute_residual(const int n,const double *const v1,const double *const v2,double *const residual)
{
   *residual = 0.0;
  for (int i = 0; i < n; i++) 
     *residual += fabs((v1[i] - v2[i]));
#ifdef USING_MPI
// Use MPI's reduce function to collect all partial sums
#endif
  return 0;
}
