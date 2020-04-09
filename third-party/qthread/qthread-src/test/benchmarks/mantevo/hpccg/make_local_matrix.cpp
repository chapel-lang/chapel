
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

#ifdef USING_MPI  // Compile this routine only if running in parallel
#include <iostream>
using std::cout;
using std::cerr;
using std::endl;
#include <map>
#include <algorithm>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include "HPC_Sparse_Matrix.hpp"
#include "read_HPC_row.hpp"
#include "make_local_matrix.hpp"
#include "mytimer.hpp"
//#define DEBUG
void make_local_matrix(HPC_Sparse_Matrix * A)
{
  std::map< int, int > externals;
  int i, j, k;
  int num_external = 0;
  double t0;

  int debug_details = 0; // Set to 1 for voluminous output
#ifdef DEBUG
  int debug = 1;
#else
  int debug = 0;
#endif

  // Get MPI process info

  int size, rank; // Number of MPI processes, My process ID
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  
  // Extract Matrix pieces

  int start_row = A->start_row;
  int stop_row = A->stop_row;
  int total_nrow = A->total_nrow;
  long long total_nnz = A->total_nnz;
  int local_nrow = A->local_nrow;
  int local_nnz = A->local_nnz;
  int  * nnz_in_row = A->nnz_in_row;
  double ** ptr_to_vals_in_row = A->ptr_to_vals_in_row;
  int ** ptr_to_inds_in_row = A->ptr_to_inds_in_row;
  
  

  // We need to convert the index values for the rows on this processor
  // to a local index space. We need to:
  // - Determine if each index reaches to a local value or external value
  // - If local, subtract start_row from index value to get local index
  // - If external, find out if it is already accounted for.  
  //   - If so, then do nothing, 
  //   - otherwise 
  //     - add it to the list of external indices,  
  //     - find out which processor owns the value. 
  //     - Set up communication for sparse MV operation.
  
  
  ///////////////////////////////////////////
  // Scan the indices and transform to local
    ///////////////////////////////////////////

  if (debug) t0 = mytimer();

  int *external_index = new int[max_external];
  int *external_local_index = new int[max_external];
  A->external_index = external_index;
  A->external_local_index = external_local_index;

  for (i=0; i< local_nrow; i++)
    {
      for (j=0; j<nnz_in_row[i]; j++)
	{
	  int cur_ind = ptr_to_inds_in_row[i][j];
	  if (debug_details)
	    cout << "Process "<<rank<<" of "<<size<<" getting index "
		 <<cur_ind<<" in local row "<<i<<endl;
	  if (start_row <= cur_ind && cur_ind <= stop_row)
	    {
	      ptr_to_inds_in_row[i][j] -= start_row;
	    }
	  else // Must find out if we have already set up this point
	    {
	      if (externals.find(cur_ind)==externals.end())
		{
		  externals[cur_ind] = num_external++;
		  if (num_external<=max_external)
		    {
		      external_index[num_external-1] = cur_ind;
		      // Mark index as external by negating it
		      ptr_to_inds_in_row[i][j] = - (ptr_to_inds_in_row[i][j] + 1);
		    }
		  else 
		    {
		      cerr << "Must increase max_external in HPC_Sparse_Matrix.hpp"
			   <<endl;
		      abort();
		    }
		}
	      else
		{
		  // Mark index as external by adding 1 and negating it
		  ptr_to_inds_in_row[i][j] = - (ptr_to_inds_in_row[i][j] + 1);
		}
	    }
	}
    }

  if (debug) {
    t0 = mytimer() - t0;
    cout << "            Time in transform to local phase = " << t0 << endl;
    cout << "Processor " << rank << " of " << size <<
	       ": Number of external equations = " << num_external << endl;
  }

  ////////////////////////////////////////////////////////////////////////////
  // Go through list of externals to find out which processors must be accessed.
  ////////////////////////////////////////////////////////////////////////////

  if (debug) t0 = mytimer();

  A->num_external = num_external;
  int * tmp_buffer  = new int[size];  // Temp buffer space needed below

  // Build list of global index offset

  int * global_index_offsets = new int[size];
  for (i=0;i<size; i++) tmp_buffer[i] = 0;  // First zero out

  tmp_buffer[rank] = start_row; // This is my start row

  // This call sends the start_row of each ith processor to the ith 
  // entry of global_index_offset on all processors.
  // Thus, each processor know the range of indices owned by all
  // other processors.
  // Note:  There might be a better algorithm for doing this, but this
  //        will work...

  MPI_Allreduce(tmp_buffer, global_index_offsets, size, MPI_INT,
		MPI_SUM, MPI_COMM_WORLD);

  // Go through list of externals and find the processor that owns each
  int * external_processor = new int[num_external];
  int * new_external_processor = new int[num_external];
  for (i=0; i< num_external; i++)
    {
      int cur_ind = external_index[i];
      for (int j=size-1; j>=0; j--)
	if (global_index_offsets[j] <= cur_ind) 
	  {
	    external_processor[i] = j;
	    break;
	  }
    }
  if (debug) {
    t0 = mytimer() - t0;
    cout << "          Time in finding processors phase = " << t0 << endl;
  }


  ////////////////////////////////////////////////////////////////////////////
  // Sift through the external elements. For each newly encountered external
  // point assign it the next index in the sequence. Then look for other
  // external elements who are update by the same node and assign them the next
  // set of index numbers in the sequence (ie. elements updated by the same node
  // have consecutive indices).
  ////////////////////////////////////////////////////////////////////////////

  if (debug) t0 = mytimer();

  int count = local_nrow;
  for (i = 0; i < num_external; i++) external_local_index[i] = -1;

  for (i = 0; i < num_external; i++) {
    if (external_local_index[i] == -1) {
      external_local_index[i] = count++;

      for (j = i + 1; j < num_external; j++) {
        if (external_processor[j] == external_processor[i]) 
	  external_local_index[j] = count++;
      }
    }
  }

  if (debug) {
    t0 = mytimer() - t0;
    cout << "           Time in scanning external indices phase = " << t0 << endl;
  }
  if (debug) t0 = mytimer();


  for (i=0; i< local_nrow; i++)
    {
      for (j=0; j<nnz_in_row[i]; j++)
	{
	  if (ptr_to_inds_in_row[i][j]<0) // Change index values of externals
	    {
	      int cur_ind = - ptr_to_inds_in_row[i][j] - 1;
	      ptr_to_inds_in_row[i][j] = external_local_index[externals[cur_ind]];
	    }
	}
    }

  for (i = 0 ; i < num_external; i++)  new_external_processor[i] = 0;

  for (i = 0; i < num_external; i++)
      new_external_processor[external_local_index[i] - local_nrow] = 
	external_processor[i];

  if (debug) {
    t0 = mytimer() - t0;
    cout << "           Time in assigning external indices phase = " << t0 << endl;
  }

  if (debug_details)
    {
      for (i = 0; i < num_external; i++)
	{
	  cout << "Processor " << rank << " of " << size <<
	    ": external processor["<< i<< "] = " << external_processor[i]<< endl;
	  cout << "Processor " << rank << " of " << size <<
	    ": new external processor["<< i<< "] = " 
	       << new_external_processor[i]<< endl;
	}
    }

  ////////////////////////////////////////////////////////////////////////////
  ///
  // Count the number of neighbors from which we receive information to update
  // our external elements. Additionally, fill the array tmp_neighbors in the
  // following way:
  //      tmp_neighbors[i] = 0   ==>  No external elements are updated by
  //                              processor i.
  //      tmp_neighbors[i] = x   ==>  (x-1)/size elements are updated from
  //                              processor i.
  ///
  ////////////////////////////////////////////////////////////////////////////

  t0 = mytimer();
  int * tmp_neighbors = new int[size];
  for (i = 0 ; i < size ; i++)  tmp_neighbors[i] = 0;

  int num_recv_neighbors = 0;
  int length             = 1;

  for (i = 0; i < num_external; i++) 
    {
      if (tmp_neighbors[new_external_processor[i]] == 0) 
	{
	  num_recv_neighbors++;
	  tmp_neighbors[new_external_processor[i]] = 1;
	}
      tmp_neighbors[new_external_processor[i]] += size;
    }

  /// sum over all processors all the tmp_neighbors arrays ///

  MPI_Allreduce(tmp_neighbors, tmp_buffer, size, MPI_INT, MPI_SUM, 
		MPI_COMM_WORLD);

  /// decode the combined 'tmp_neighbors' (stored in tmp_buffer) 
  //  array from all the processors

  int num_send_neighbors = tmp_buffer[rank] % size;

  /// decode 'tmp_buffer[rank] to deduce total number of elements 
  //  we must send

  int total_to_be_sent = (tmp_buffer[rank] - num_send_neighbors) / size;

  //
  // Check to see if we have enough workspace allocated.  This could be 
  // dynamically modified, but let's keep it simple for now...
  //

  if (num_send_neighbors > max_num_messages)
    {
      cerr << "Must increase max_num_messages in HPC_Sparse_Matrix.hpp"
	   <<endl;
      cerr << "Must be at least " <<  num_send_neighbors <<endl;
      abort();
    }

  if (total_to_be_sent > max_external )
    {
      cerr << "Must increase max_external in HPC_Sparse_Matrix.hpp"
	   <<endl;
      cerr << "Must be at least " << total_to_be_sent <<endl;
      abort();
    }
  delete [] tmp_neighbors;

  if (debug) {
    t0 = mytimer() - t0;
    cout << "           Time in finding neighbors phase = " << t0 << endl;
  }
  if (debug) cout << "Processor " << rank << " of " << size <<
	       ": Number of send neighbors = " << num_send_neighbors << endl;

  if (debug) cout << "Processor " << rank << " of " << size <<
	       ": Number of receive neighbors = " << num_recv_neighbors << endl;

  if (debug) cout << "Processor " << rank << " of " << size <<
	       ": Total number of elements to send = " << total_to_be_sent << endl;

  if (debug) MPI_Barrier(MPI_COMM_WORLD);

  /////////////////////////////////////////////////////////////////////////
  ///
  // Make a list of the neighbors that will send information to update our
  // external elements (in the order that we will receive this information).
  ///
  /////////////////////////////////////////////////////////////////////////

  int * recv_list = new int[max_external];
  j = 0;
  recv_list[j++] = new_external_processor[0];
  for (i = 1; i < num_external; i++) {
    if (new_external_processor[i - 1] != new_external_processor[i]) {
      recv_list[j++] = new_external_processor[i];
    }
  }

  //
  // Send a 0 length message to each of our recv neighbors
  //

  int * send_list = new int[num_send_neighbors];
  for (i = 0 ; i < num_send_neighbors; i++ ) send_list[i] = 0;

  //
  //  first post receives, these are immediate receives
  //  Do not wait for result to come, will do that at the
  //  wait call below.
  //
  int MPI_MY_TAG = 99;
  
  MPI_Request * request = new MPI_Request[max_num_messages];
  for (i = 0; i < num_send_neighbors; i++) 
    {
      MPI_Irecv(tmp_buffer+i, 1, MPI_INT, MPI_ANY_SOURCE, MPI_MY_TAG, 
		MPI_COMM_WORLD, request+i);
    }

  // send messages 

  for (i = 0; i < num_recv_neighbors; i++) 
      MPI_Send(tmp_buffer+i, 1, MPI_INT, recv_list[i], MPI_MY_TAG, 
	       MPI_COMM_WORLD);
  ///
   // Receive message from each send neighbor to construct 'send_list'.
   ///

  MPI_Status status;
  for (i = 0; i < num_send_neighbors; i++)
    {
      if ( MPI_Wait(request+i, &status) )
	{
	  cerr << "MPI_Wait error\n"<<endl;
	  exit(-1);
	}
      send_list[i] = status.MPI_SOURCE;
  }



  /////////////////////////////////////////////////////////////////////////
  ///
  //  Compare the two lists. In most cases they should be the same.  
  //  However, if they are not then add new entries to the recv list
  //  that are in the send list (but not already in the recv list).
  ///
  /////////////////////////////////////////////////////////////////////////

  for (j = 0; j < num_send_neighbors; j++) 
    {
      int found = 0;
      for (i = 0; i < num_recv_neighbors; i++) 
	{
	  if (recv_list[i] == send_list[j])
	    found = 1;
	}

      if (found == 0) {
	if (debug) cout << "Processor " << rank << " of " << size <<
		     ": recv_list[" << num_recv_neighbors <<"] = " 
			<< send_list[j] << endl;
	recv_list[num_recv_neighbors] = send_list[j];
	(num_recv_neighbors)++;

      }
    }

  delete [] send_list;
  num_send_neighbors = num_recv_neighbors;

  if (num_send_neighbors > max_num_messages)
    {
      cerr << "Must increase max_external in HPC_Sparse_Matrix.hpp"
	   <<endl;
      abort();
    }

  /////////////////////////////////////////////////////////////////////////
  /// Start filling HPC_Sparse_Matrix struct
  /////////////////////////////////////////////////////////////////////////

  A->total_to_be_sent = total_to_be_sent;
  int * elements_to_send = new int[total_to_be_sent];
  A->elements_to_send = elements_to_send;

  for (i = 0 ; i < total_to_be_sent; i++ ) elements_to_send[i] = 0;
  
  //
  // Create 'new_external' which explicitly put the external elements in the
  // order given by 'external_local_index'
  //

  int * new_external = new int[num_external];
  for (i = 0; i < num_external; i++) {
    new_external[external_local_index[i] - local_nrow] = external_index[i];
  }

  /////////////////////////////////////////////////////////////////////////
  //
  // Send each processor the global index list of the external elements in the
  // order that I will want to receive them when updating my external elements
  //
  /////////////////////////////////////////////////////////////////////////

  int * lengths = new int[num_recv_neighbors];

  MPI_MY_TAG++;

  // First post receives

  for (i = 0; i < num_recv_neighbors; i++) 
    {
      int partner    = recv_list[i];
      MPI_Irecv(lengths+i, 1, MPI_INT, partner, MPI_MY_TAG, MPI_COMM_WORLD, 
		request+i);
    }

  int * neighbors = new int[max_num_neighbors];
  int * recv_length = new int[max_num_neighbors];		
  int * send_length = new int[max_num_neighbors];

  A->neighbors = neighbors;
  A->recv_length = recv_length;
  A->send_length = send_length;

  j = 0;
  for (i = 0; i < num_recv_neighbors; i++) 
    {
      int start  = j;
      int newlength = 0;

      // go through list of external elements until updating 
      // processor changes

      while ((j < num_external) && 
	     (new_external_processor[j] == recv_list[i])) 
	{
	  newlength++;
	  j++;
	  if (j == num_external) break;
	}

      recv_length[i] = newlength;
      neighbors[i]  = recv_list[i];
      
      length = j - start;
      MPI_Send(&length, 1, MPI_INT, recv_list[i], MPI_MY_TAG, MPI_COMM_WORLD);
    }

  // Complete the receives of the number of externals

  for (i = 0; i < num_recv_neighbors; i++)
    {
      if ( MPI_Wait(request+i, &status) ) 
	{
	  cerr << "MPI_Wait error\n"<<endl;
	  exit(-1);
	}
      send_length[i] = lengths[i];
    }
  delete [] lengths;


  ///////////////////////////////////////////////////////////////////
  // Build "elements_to_send" list.  These are the x elements I own
  // that need to be sent to other processors.
  ///////////////////////////////////////////////////////////////////

  MPI_MY_TAG++;

  j = 0;
  for (i = 0; i < num_recv_neighbors; i++)
    {
      MPI_Irecv(elements_to_send+j, send_length[i], MPI_INT, neighbors[i], 
		MPI_MY_TAG, MPI_COMM_WORLD, request+i);
      j += send_length[i];
    }

  j = 0;
  for (i = 0; i < num_recv_neighbors; i++) 
    {
      int start  = j;
      int newlength = 0;
      
      // Go through list of external elements 
      // until updating processor changes.  This is redundant, but 
      // saves us from recording this information.

      while ((j < num_external) && 
	     (new_external_processor[j] == recv_list[i])) {
	
	newlength++;
	j++;
	if (j == num_external) break;
      }
      MPI_Send(new_external+start, j-start, MPI_INT, recv_list[i], 
	       MPI_MY_TAG, MPI_COMM_WORLD);
    }

  // receive from each neighbor the global index list of external elements

  for (i = 0; i < num_recv_neighbors; i++) 
    {
      if ( MPI_Wait(request+i, &status) ) 
	{
	  cerr << "MPI_Wait error\n"<<endl;
	  exit(-1);
	}
  }

  /// replace global indices by local indices ///

  for (i = 0; i < total_to_be_sent; i++) elements_to_send[i] -= start_row;


  ////////////////
  // Finish up !!
  ////////////////

  A->num_send_neighbors = num_send_neighbors;
  A->local_ncol = A->local_nrow + num_external;

  //Used in exchange_externals
  double *send_buffer = new double[total_to_be_sent];
  A->send_buffer = send_buffer;

  delete [] tmp_buffer;
  delete [] global_index_offsets;
  delete [] recv_list;
  delete [] external_processor;
  delete [] new_external_processor;
  delete [] request;

  return;
}
#endif // USING_MPI
