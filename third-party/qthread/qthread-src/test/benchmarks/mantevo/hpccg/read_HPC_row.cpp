
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

// Routine to read a sparse matrix, right hand side, initial guess, 
// and exact solution (as computed by a direct solver).

/////////////////////////////////////////////////////////////////////////

// nrow - number of rows of matrix (on this processor)

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include "read_HPC_row.hpp"
void read_HPC_row(char *data_file, HPC_Sparse_Matrix **A,
		  double **x, double **b, double **xexact)

{
  FILE *in_file ;

  int ret;
  int i,j/*, max_nnz_in_row*/;
  int total_nrow;
  long long total_nnz;
  int l;
  int * lp = &l;
  double v;
  double * vp = &v;
#ifdef DEBUG
  int debug = 1;
#else
  int debug = 0;
#endif

  printf("Reading matrix info from %s...\n",data_file);
  
  in_file = fopen( data_file, "r");
  if (in_file == NULL)
    {
      printf("Error: Cannot open file: %s\n",data_file);
      exit(1);
    }

  ret = fscanf(in_file,"%d",&total_nrow);
  ret += fscanf(in_file,"%lld",&total_nnz);
  assert(ret == 2);
#ifdef USING_MPI
  int size, rank; // Number of MPI processes, My process ID
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
#else
  int size = 1; // Serial case (not using MPI)
  int rank = 0;
#endif
  int n = total_nrow;
  int chunksize = n/size;
  int remainder = n%size;

  int mp = chunksize;
  if (rank<remainder) mp++;
  int local_nrow = mp;
  
  int off = rank*(chunksize+1);
  if (rank>remainder) off -= (rank - remainder);
  int start_row = off;
  int stop_row = off + mp -1;
  

  // Allocate arrays that are of length local_nrow
  int *nnz_in_row = new int[local_nrow];
  double **ptr_to_vals_in_row = new double*[local_nrow];
  int    **ptr_to_inds_in_row = new int   *[local_nrow];
  double **ptr_to_diags       = new double*[local_nrow];

  *x = new double[local_nrow];
  *b = new double[local_nrow];
  *xexact = new double[local_nrow];

  // Find nnz for this processor
  int local_nnz = 0;
  int cur_local_row = 0;

  for (i=0; i<total_nrow; i++)
    {
      ret = fscanf(in_file, "%d",lp); /* row #, nnz in row */
      assert(ret == 1);
      if (start_row <=i && i <= stop_row) // See if nnz for row should be added
	{
	  local_nnz += l;
	  nnz_in_row[cur_local_row] = l;
	  cur_local_row++;
	}
    }
  assert(cur_local_row==local_nrow); // cur_local_row should equal local_nrow


  // Allocate arrays that are of length local_nnz
  double *list_of_vals = new double[local_nnz];
  int *list_of_inds = new int   [local_nnz];

  // Define pointers into list_of_vals/inds 
  ptr_to_vals_in_row[0] = list_of_vals;
  ptr_to_inds_in_row[0] = list_of_inds;
  for (i=1; i<local_nrow; i++)
    {
      ptr_to_vals_in_row[i] = ptr_to_vals_in_row[i-1]+nnz_in_row[i-1];
      ptr_to_inds_in_row[i] = ptr_to_inds_in_row[i-1]+nnz_in_row[i-1];
    }

  cur_local_row = 0;
  for (i=0; i<total_nrow; i++)
    {
      int cur_nnz;
      ret = fscanf(in_file, "%d",&cur_nnz);
      assert(ret == 1);
      if (start_row <=i && i <= stop_row) // See if nnz for row should be added
	{
	  if (debug) cout << "Process "<<rank
			  <<" of "<<size<<" getting row "<<i<<endl;
	  for (j=0; j<cur_nnz; j++) 
	    {
	      ret = fscanf(in_file, "%lf %d",vp,lp);
              assert(ret == 2);
	      ptr_to_vals_in_row[cur_local_row][j] = v;
	      ptr_to_inds_in_row[cur_local_row][j] = l;
	    }
	      cur_local_row++;
	}
      else
	for (j=0; j<cur_nnz; j++) {
            ret = fscanf(in_file, "%lf %d",vp,lp);
            assert(ret == 2);
        }
    }

  
  cur_local_row = 0;
  double xt, bt, xxt;
  for (i=0; i<total_nrow; i++) 
    {
      if (start_row <=i && i <= stop_row) // See if entry should be added
	{
	  if (debug) cout << "Process "<<rank<<" of "
                       <<size<<" getting RHS "<<i<<endl;
	  ret = fscanf(in_file, "%lf %lf %lf",&xt, &bt, &xxt);
          assert(ret == 3);
	  (*x)[cur_local_row] = xt;
	  (*b)[cur_local_row] = bt;
	  (*xexact)[cur_local_row] = xxt;
	  cur_local_row++;
	}
      else {
	ret = fscanf(in_file, "%lf %lf %lf",vp, vp, vp); // or thrown away
        assert(ret == 3);
      }
    }

  fclose(in_file);
  if (debug) cout << "Process "<<rank<<" of "<<size<<" has "<<local_nrow;

  if (debug) cout << " rows. Global rows "<< start_row
		  <<" through "<< stop_row <<endl;

  if (debug) cout << "Process "<<rank<<" of "<<size
		  <<" has "<<local_nnz<<" nonzeros."<<endl;

  *A = new HPC_Sparse_Matrix; // Allocate matrix struct and fill it
  (*A)->title = 0;
  (*A)->start_row = start_row ; 
  (*A)->stop_row = stop_row;
  (*A)->total_nrow = total_nrow;
  (*A)->total_nnz = total_nnz;
  (*A)->local_nrow = local_nrow;
  (*A)->local_ncol = local_nrow;
  (*A)->local_nnz = local_nnz;
  (*A)->nnz_in_row = nnz_in_row;
  (*A)->ptr_to_vals_in_row = ptr_to_vals_in_row;
  (*A)->ptr_to_inds_in_row = ptr_to_inds_in_row;
  (*A)-> ptr_to_diags = ptr_to_diags;

  return;
}
