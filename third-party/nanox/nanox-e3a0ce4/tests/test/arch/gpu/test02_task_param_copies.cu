/*************************************************************************************/
/*      Copyright 2009 Barcelona Supercomputing Center                               */
/*                                                                                   */
/*      This file is part of the NANOS++ library.                                    */
/*                                                                                   */
/*      NANOS++ is free software: you can redistribute it and/or modify              */
/*      it under the terms of the GNU Lesser General Public License as published by  */
/*      the Free Software Foundation, either version 3 of the License, or            */
/*      (at your option) any later version.                                          */
/*                                                                                   */
/*      NANOS++ is distributed in the hope that it will be useful,                   */
/*      but WITHOUT ANY WARRANTY; without even the implied warranty of               */
/*      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                */
/*      GNU Lesser General Public License for more details.                          */
/*                                                                                   */
/*      You should have received a copy of the GNU Lesser General Public License     */
/*      along with NANOS++.  If not, see <http://www.gnu.org/licenses/>.             */
/*************************************************************************************/

/*
<testinfo>
test_generator=gens/gpu-generator
test_CXX=nvcc
</testinfo>
*/

#define __aligned__ ignored
#include "gpuprocessor.hpp"
#include "nanos.h"
#undef __aligned__
#include <iostream>
#include <stdlib.h>
#include <string.h>


using namespace std;

using namespace nanos;
using namespace nanos::ext;

#define NTASKS 32
const int ntasks = NTASKS;

#define N 512
const int n = 512;

// Arguments' struct
typedef struct {
   int * err;
   int n;
   int idx;
   int * a;
   int * b;
   int * c;
} test_args;


// GPU kernels
__global__ void check_task_params ( int idx, int * a , int * b, int * c, int * err );


// GPU tasks
///#pragma omp target device (cuda) copydeps
///#pragma omp task input(n, idx, a) output(b, err) inout(c)
//void test_gpu_task ( int * err, int n, int idx, int * a, int * b, int * c );
void test_gpu_task ( void * args );


///#pragma omp target device (cuda) copydeps
///#pragma omp task 
void dummy_task ( void * args );


int main ( int argc, char **argv )
{
   std::cout << "Testing GPU task parameter copy mechanism" << std::endl;

   int i, j;
   test_args ** args = new test_args*[ntasks];

   nanos::WG *wg = nanos::getMyThreadSafe()->getCurrentWD();

   for ( i = 0; i < ntasks; i++ ) {
      // Data structures initialization
      args[i] = new test_args();
      args[i]->err = new int(1);
      args[i]->n = n;
      args[i]->idx = i;

      args[i]->a = new int[n];
      args[i]->b = new int[n];
      args[i]->c = new int[n];

      args[i]->a[0] = i;
      args[i]->b[0] = 0;
      args[i]->c[0] = i;

      for ( j = 1; j < n; j++ ) {
         args[i]->a[j] = args[i]->a[j-1] + 1;
         args[i]->b[j] = 0;
         args[i]->c[j] = args[i]->c[j-1] + 1;
      }

      // Parameter copies
      CopyData* cd = new CopyData[6];
      // CopyData( address, attribute, input, output, size )
      cd[0] = CopyData( (uint64_t)args[i]->err, NANOS_SHARED, true, true, sizeof(int) );
      cd[1] = CopyData( (uint64_t)&args[i]->n, NANOS_PRIVATE, true, false, sizeof(args[i]->n) );
      cd[2] = CopyData( (uint64_t)&args[i]->idx, NANOS_PRIVATE, true, false, sizeof(args[i]->idx) );
      cd[3] = CopyData( (uint64_t)args[i]->a, NANOS_SHARED, true, false, n * sizeof(int) ); 
      cd[4] = CopyData( (uint64_t)args[i]->b, NANOS_SHARED, false, true, n * sizeof(int) ); 
      cd[5] = CopyData( (uint64_t)args[i]->c, NANOS_SHARED, true, true, n * sizeof(int) );

      // Task execution
      nanos::WD * wd = new nanos::WD( new nanos::ext::GPUDD( test_gpu_task ), sizeof( args[i] ), args[i], 6, cd );
      wd->tied();
      wg->addWork( *wd );
      nanos::sys.submit( *wd );
   }

   usleep(500);

   wg->waitCompletion();

   // waitCompletion does not wait for the last copy-out to finish when overlapping data and computation,
   // so create several dummy tasks in order to force last data to be copied. (See #307)
   for ( i = 0; i < ntasks; i++ ) {
      nanos::WD * wd = new nanos::WD( new nanos::ext::GPUDD( dummy_task ), 0, 0, 0, 0 );
      wd->tied();
      wg->addWork( *wd );
      nanos::sys.submit( *wd );
   }

   usleep(500);

   wg->waitCompletion();

   for ( i = 0; i < ntasks; i++ ) {
      if ( *args[i]->err != 0 ) {
         std::cout << "   [" << i << "] " << *args[i]->err << " errors found: task parameter copy mechanism did not succeed.   FAIL" << std::endl;
         exit( 1 );
      }
   }
   
   delete[] args;

   std::cout << "   Execution ... ok!" << std::endl;
   
   std::cout << "End testing GPU task parameter copy mechanism" << std::endl;
    
   
   return 0;
}

///#pragma omp target device (cuda) copydeps
///#pragma omp task 
void dummy_task ( void * args )
{
   int a = 4985;
   float b = 0.6234;
   double c = 0.0;

   int i;
   for ( i = 0; i < NTASKS*NTASKS*NTASKS*NTASKS; i++ ) {
      c += (double) (a * b) - (b / a) + (a + b);
   }
}

///#pragma omp target device (cuda) copydeps
///#pragma omp task input(n, idx, a) output(b, err) inout(c)
void test_gpu_task ( void * args )
{

   test_args targs;

   nanos_get_addr(0, ( void ** ) &targs.err);
   targs.n = ( (test_args *) args)->n;
   targs.idx = ( (test_args *) args)->idx;
   nanos_get_addr(3, ( void ** ) &targs.a);
   nanos_get_addr(4, ( void ** ) &targs.b);
   nanos_get_addr(5, ( void ** ) &targs.c);

   int i, err, value;

   size_t size = targs.n * sizeof ( int );
   int err_h[targs.n];
   int * err_d;
   cudaMalloc( &err_d, size );

   memset( err_h, 0x1, size );
   cudaMemcpy( err_d, err_h, size, cudaMemcpyHostToDevice );

   check_task_params <<< 1, targs.n >>> ( targs.idx, targs.a, targs.b, targs.c, err_d );

   cudaThreadSynchronize();

   cudaMemcpy( err_h, err_d, size, cudaMemcpyDeviceToHost );

   err = 0;
   for ( i = 0; i < targs.n; i++ ) {
      // Error checking for A and C, from kernel execution (as inputs)
      if ( err_h[i] ) {
         std::cout << "[" << targs.idx << "] Input error detected at position " << i << ": " << err_h[i] << std::endl;
         err++;
      }
   }

   // Error checking for B, from kernel execution (as output)
   cudaMemcpy( err_h, targs.b, size, cudaMemcpyDeviceToHost );
   value = targs.idx;
   for ( i = 0; i < targs.n; i++ ) {
      if ( err_h[i] != value ) {
         std::cout << "[" << targs.idx << "] Output error detected at position " << i << ": " << err_h[i] << std::endl;
         err++;
      }
      value++;
   }

   // Write the result through cudaMemcpy, as 'targs->err' address belongs to GPU memory space
   cudaMemcpy( targs.err, &err, sizeof( int ), cudaMemcpyHostToDevice );

   cudaFree(err_d);

}


/***** GPU CODE *****/
__global__ void check_task_params ( int idx, int * a , int * b, int * c, int * err )
{
   int i = threadIdx.x;
   int val = idx + threadIdx.x;

   err[i] = 0;

   err[i] += a[i] - val;
   err[i] += c[i] - val;

   b[i] = val;

}
/***** END GPU CODE *****/

