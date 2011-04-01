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
#undef __aligned__
#include <iostream>
#include <stdlib.h>
#include <string.h>


using namespace std;

using namespace nanos;
using namespace nanos::ext;

#define NREP 100

// Arguments' struct
typedef struct {
   int err;
   int n;
   int * Ah;
   int * Ad;
   int * Bh;
   int * Bd;
   int task;
} test_args;


// GPU kernels
__global__ void check_host_to_device ( int * a , int * err );
__global__ void check_device_to_device ( int * a, int * err );


// GPU tasks
///#pragma omp target device (cuda) copydeps
///#pragma omp task inout (args)
void test_init ( void * args );
///#pragma omp target device (cuda) copydeps
///#pragma omp task inout (args)
void test_host_to_device ( void * args );
///#pragma omp target device (cuda) copydeps
///#pragma omp task inout (args)
void test_device_to_device ( void * args );
///#pragma omp target device (cuda) copydeps
///#pragma omp task inout (args)
void test_device_to_host ( void * args );
///#pragma omp target device (cuda) copydeps
///#pragma omp task inout (args)
void test_cleanup ( void * args );


int main ( int argc, char **argv )
{
   std::cout << "Testing GPU memory manager ( GPUDevice ): allocate, free, copy in / out / local" << std::endl;

   int i, n = 512;
   int ngpus = nanos::ext::GPUDD::getGPUCount();
   test_args ** args = new test_args* [ngpus];
   
   for ( i = 0; i < ngpus; i++ ) {
      args[i] = new test_args();
      args[i]->err = 0;
      args[i]->n = n;
      args[i]->Ah = 0;
      args[i]->Ad = 0;
      args[i]->Bh = 0;
      args[i]->Bd = 0;
      args[i]->task = 0;
   }

   nanos::WG *wg = nanos::getMyThreadSafe()->getCurrentWD();

   // Execution of tasks: for every kind of task, create 100 times the number of GPUs, so that we
   // assume that each GPU will pick at least one task of each type.
   // 'task' member controls that each kind of task is executed once and only once on the same GPU.

   // Initialization
   for ( i = 0; i < ngpus*NREP; i++ ) {
///      test_init( args[i] );

      nanos::WD * wd = new nanos::WD( new nanos::ext::GPUDD( test_init ), sizeof( test_args ) * ngpus, args );
      wd->tied();
      wg->addWork( *wd );
      nanos::sys.submit( *wd );
      
      usleep(500);
   }
   wg->waitCompletion();
   
   for ( i = 0; i < ngpus; i++ ) {
      if ( args[i]->err != 0 ) {
         std::cout << "   [" << i << "] " << args[i]->err << " errors found: data allocation into device memory did not succeed.   FAIL" << std::endl;
         exit( 1 );
      }
   }
   
   std::cout << "   Initialization ... ok!" << std::endl;
   
   // First copy: host to device
   for ( i = 0; i < ngpus*NREP; i++ ) {
///      test_host_to_device( args[i] );

      nanos::WD * wd = new nanos::WD( new nanos::ext::GPUDD( test_host_to_device ), sizeof( test_args ) * ngpus, args );
      wd->tied();
      wg->addWork( *wd );
      nanos::sys.submit( *wd );
      
      usleep(500);
   }
   wg->waitCompletion();
   
   for ( i = 0; i < ngpus; i++ ) {
      if ( args[i]->err != 0 ) {
         std::cout << "   [" << i << "] " << args[i]->err << " errors found: copying memory from host to device did not succeed.   FAIL" << std::endl;
         exit( 1 );
      }
   }
   
   std::cout << "   Host --> Device ... ok!" << std::endl;
   
   // Second copy: device to device (local)
   for ( i = 0; i < ngpus*NREP; i++ ) {
///      test_device_to_device( args[i] );
      
      nanos::WD * wd = new nanos::WD( new nanos::ext::GPUDD( test_device_to_device ), sizeof( test_args ) * ngpus, args );
      wd->tied();
      wg->addWork( *wd );
      nanos::sys.submit( *wd );
      
      usleep(500);
   }
   wg->waitCompletion();
   
   for ( i = 0; i < ngpus; i++ ) {
      if ( args[i]->err != 0 ) {
         std::cout << "   [" << i << "] " << args[i]->err << " errors found: copying memory from device to device did not succeed.   FAIL" << std::endl;
         exit( 1 );
      }
   }
   
   std::cout << "   Device --> Device ... ok!" << std::endl;

   // Third copy: device to host
   for ( i = 0; i < ngpus*NREP; i++ ) {
///      test_device_to_host( args[i] );
      
      nanos::WD * wd = new nanos::WD( new nanos::ext::GPUDD( test_device_to_host ), sizeof( test_args ) * ngpus, args );
      wd->tied();
      wg->addWork( *wd );
      nanos::sys.submit( *wd );

      usleep(500);
   }
   wg->waitCompletion();
   
   for ( i = 0; i < ngpus; i++ ) {
      if ( args[i]->err != 0 ) {
         std::cout << "   [" << i << "] " << args[i]->err << " errors found: copying memory from device to host did not succeed.   FAIL" << std::endl;
         exit( 1 );
      }
   }
   
   std::cout << "   Device --> Host ... ok!" << std::endl;
   
   // CLEANUP
   for ( i = 0; i < ngpus*NREP; i++ ) {
///      test_cleanup( args[i] );

      nanos::WD * wd = new nanos::WD( new nanos::ext::GPUDD( test_cleanup ), sizeof( test_args ) * ngpus, args );
      wd->tied();
      wg->addWork( *wd );
      nanos::sys.submit( *wd );
      
      usleep(500);
   }
   wg->waitCompletion();
   
   for ( i = 0; i < ngpus; i++ ) {
      if ( args[i]->err != 0 ) {
         std::cout << "   [" << i << "] " << args[i]->err << " errors found: data cleanup from device memory did not succeed.   FAIL" << std::endl;
         exit( 1 );
      }
   }
   
   std::cout << "   Clean up ... ok!" << std::endl;
  
   for ( i = 0; i < ngpus; i++ ) {
      if ( args[i]->task != 5 ) {
         std::cout << "WARNING: thread " << i << " did not run all kinds of tasks: " << args[i]->task << std::endl;
      }
   }
 
   delete args;

   std::cout << "End testing GPU memory manager ( GPUDevice )" << std::endl;
    
   
   return 0;
}

///#pragma omp target device (cuda) copydeps
///#pragma omp task inout (args)
void test_init ( void * args )
{
   int id = ((nanos::ext::GPUThread *)nanos::getMyThreadSafe())->getGpuDevice();
   
   test_args ** full_args = ( test_args ** ) args;
   test_args * targs = full_args[id];

   if ( targs->task != 0 )
      return;

   targs->task++;

   size_t size = targs->n * sizeof ( int );
   targs->Ah = new int[targs->n];
   targs->Bh = new int[targs->n];
   targs->Ad = ( int * ) GPUDevice::allocate( size );
   targs->Bd = ( int * ) GPUDevice::allocate( size );

   targs->err = 0;

   if ( targs->Ah == 0 ) targs->err++;
   if ( targs->Bh == 0 ) targs->err++;
   if ( targs->Ad == 0 ) targs->err++;
   if ( targs->Bd == 0 ) targs->err++;
   
   usleep( 50 );
}

///#pragma omp target device (cuda) copydeps
///#pragma omp task inout (args)
void test_cleanup ( void * args )
{
   int id = ((nanos::ext::GPUThread *)nanos::getMyThreadSafe())->getGpuDevice();
   
   test_args ** full_args = ( test_args ** ) args;
   test_args * targs = full_args[id];
   
   if ( targs->task != 4 )
      return;
   
   targs->task++;

   delete targs->Ah;
   delete targs->Bh;
   GPUDevice::free( targs->Ad );
   GPUDevice::free( targs->Bd );
   
   targs->err = 0;
   
   usleep( 50 );
}

///#pragma omp target device (cuda) copydeps
///#pragma omp task inout (args)
void test_host_to_device ( void * args )
{
   int id = ((nanos::ext::GPUThread *)nanos::getMyThreadSafe())->getGpuDevice();
   
   test_args ** full_args = ( test_args ** ) args;
   test_args * targs = full_args[id];
   
   if ( targs->task != 1 )
      return;
   
   targs->task++;

   cudaStream_t inStream = ((nanos::ext::GPUProcessor *) getMyThreadSafe()->runningOn())->getGPUProcessorInfo()->getInTransferStream();
   
   int i;   
   size_t size = targs->n * sizeof ( int );

   // Initalize arrays
   for ( i = 0; i < targs->n; i++ ) {
      targs->Ah[i] = i;
      targs->Bh[i] = 1;
   }
   cudaMemset( targs->Ad, 0, size );
   cudaMemset( targs->Bd, 0, size );

   GPUDevice::copyIn( targs->Ad, ( uint64_t ) targs->Ah, size );
   
   cudaStreamSynchronize( inStream );
   
   // Launch a kernel to check the copy was successful and get the result back from the GPU
   // Arrays Bd and Bh will contain the error checking result
   check_host_to_device <<< 1, targs->n >>> ( targs->Ad, targs->Bd );
   cudaMemcpy( targs->Bh, targs->Bd, size, cudaMemcpyDeviceToHost );

   targs->err = 0;
   for ( i = 0; i < targs->n; i++ ) {
      if ( targs->Bh[i] ) {
         std::cout << "Error detected at position " << i << ": " << targs->Bh[i] << std::endl;
         targs->err++;
      }
   }
}

///#pragma omp target device (cuda) copydeps
///#pragma omp task inout (args)
void test_device_to_device ( void * args )
{
   int id = ((nanos::ext::GPUThread *)nanos::getMyThreadSafe())->getGpuDevice();
   
   test_args ** full_args = ( test_args ** ) args;
   test_args * targs = full_args[id];
   
   if ( targs->task != 2 )
      return;
   
   targs->task++;

   cudaStream_t inStream = ((nanos::ext::GPUProcessor *) getMyThreadSafe()->runningOn())->getGPUProcessorInfo()->getInTransferStream();
   
   int i;   
   size_t size = targs->n * sizeof ( int );
   
   // Initalize arrays
   for ( i = 0; i < targs->n; i++ ) {
      targs->Ah[i] = targs->n - i;
      targs->Bh[i] = 1;
   }
   cudaMemcpy( targs->Ad, targs->Ah, size, cudaMemcpyHostToDevice );
   cudaMemset( targs->Bd, 0, size );

   GPUDevice::copyLocal( targs->Bd, targs->Ad, size );
   
   cudaStreamSynchronize( inStream );
   
   // Launch a kernel to check the copy was successful and get the result back from the GPU
   // Arrays Ad and Bh will contain the error checking result
   check_device_to_device <<< 1, targs->n >>> ( targs->Bd, targs->Ad );
   cudaMemcpy( targs->Bh, targs->Ad, size, cudaMemcpyDeviceToHost );

   targs->err = 0;
   for ( i = 0; i < targs->n; i++ ) {
      if ( targs->Bh[i] ) {
         std::cout << "Error detected at position " << i << ": " << targs->Bh[i] << std::endl;
         targs->err++;
      }
   }
}

///#pragma omp target device (cuda) copydeps
///#pragma omp task inout (args)
void test_device_to_host ( void * args )
{
   int id = ((nanos::ext::GPUThread *)nanos::getMyThreadSafe())->getGpuDevice();
   
   test_args ** full_args = ( test_args ** ) args;
   test_args * targs = full_args[id];
   
   if ( targs->task != 3 )
      return;
   
   targs->task++;

   int i;   
   size_t size = targs->n * sizeof ( int );
   
   // Initialize arrays
   for ( i = 0; i < targs->n; i++ ) {
      targs->Ah[i] = i;
   }
   memset( targs->Bh, 0, size ); 
   cudaMemcpy( targs->Bd, targs->Ah, size, cudaMemcpyHostToDevice );

   // As asynchronous copies involve the cache management, make always synchronous copy out's
   GPUDevice::copyOutSyncToHost( targs->Bh, targs->Bd, size );

   ( ( GPUThread * ) getMyThreadSafe())->executePendingCopies();

   targs->err = 0;
   for ( i = 0; i < targs->n; i++ ) {
      if ( targs->Bh[i] != targs->Ah[i] ) {
         std::cout << "Error detected at position " << i << ": " << targs->Bh[i] << std::endl;
         targs->err++;
      }
   }
}



/***** GPU CODE *****/
__global__ void check_host_to_device ( int * a , int * err )
{
   int i = threadIdx.x;

   err[i] = a[i] - i;

   a[i] = blockDim.x - threadIdx.x;

}

__global__ void check_device_to_device ( int * b, int * err )
{
   int i = threadIdx.x;

   err[i] = b[i] - (blockDim.x - i);

   b[i] = threadIdx.x;

}
/***** END GPU CODE *****/

