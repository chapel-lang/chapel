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

// Block size
#define BSIZE 8
const int bsize = BSIZE;

// Number of blocks
#define NB 4
const int nb = NB;

// Arguments' struct
typedef struct {
   int bs;
   float * a;
   float * b;
   float * c;
} test_args;


// Auxiliar functions
void matmul_host ( int bs, float * a, float * b, float * c );


// GPU kernel
__global__ void matmul_gpu ( float * a , float * b, float * c );


// GPU task
///#pragma omp target device (cuda) copydeps
///#pragma omp task input(a, b) inout(c)
//void matmul_task ( int nb, int bs, float * a, float * b, float * c );
void matmul_task ( void * args );


nanos_smp_args_t test_device_arg = { matmul_task };

int main ( int argc, char **argv )
{
   std::cout << "Testing matrix multiply on GPU" << std::endl;

   int i, j, k, n;
   n = nb * bsize;

   // Data structures initialization
   float * a = new float[n * n];
   float * b = new float[n * n];
   float * c = new float[n * n];

   for ( i = 0; i < n; i++ ) {
      for ( j = 0; j < n; j++ ) {
         a[i * n + j] = 1.0;
         b[i * n + j] = 2.0;
         c[i * n + j] = 0.0;
      }
   }

   // Task creation
   nanos::WG *wg = nanos::getMyThreadSafe()->getCurrentWD();

   for ( i = 0; i < nb; i++ ) {
      for ( j = 0; j < nb; j++ ) {
         for ( k = 0; k < nb; k++ ) {
            nanos_wd_t wd = 0;
            
            test_args *args = 0;
            
            
            //nanos_device_t test_devices[1] = { {NANOS_GPU_DESC( test_device_arg )} };
            nanos_device_t test_devices[] = {
               {
                  nanos_gpu_factory,
                  nanos_gpu_dd_size,
                  &test_device_arg

               }
            };
            
            nanos_wd_props_t props = {
               .mandatory_creation = true,
            };
            props.tied = 1;
            
            nanos_copy_data_t *cd = 0;

//            nanos::WD * wd = new nanos::WD( new nanos::ext::GPUDD( matmul_task ), sizeof( args ), &args, 4, cd );
//            wd->tied();
//            wg->addWork( *wd );
//            nanos::sys.submitWithDependencies( *wd, 1, dep );
            
            NANOS_SAFE( nanos_create_wd ( &wd, 1, test_devices , sizeof( test_args ), (void **) &args, nanos_current_wd(), &props, 4, &cd ) );
            
            args->bs = bsize;
            args->a = &a[i * nb * bsize * bsize + k * bsize * bsize];
            args->b = &b[k * nb * bsize * bsize + j * bsize * bsize];
            args->c = &c[i * nb * bsize * bsize + j * bsize * bsize];
            
            cd[0].address = (uint64_t) &args->bs;
            cd[0].sharing = NANOS_PRIVATE;
            cd[0].flags.input = true;
            cd[0].flags.output = false;
            cd[0].size = sizeof(int);
            //cd[0] = (nanos_copy_data_t) { (uint64_t) &args->bs, NANOS_PRIVATE, { true, false }, sizeof(int) };
            
            cd[1].address = (uint64_t) args->a;
            cd[1].sharing = NANOS_SHARED;
            cd[1].flags.input = true;
            cd[1].flags.output = false;
            cd[1].size = bsize * bsize * sizeof(float);
            //cd[1] = (nanos_copy_data_t) { (uint64_t) args->a, NANOS_SHARED, { true, false }, bsize * bsize * sizeof(float) };
            
            cd[2].address = (uint64_t) args->b;
            cd[2].sharing = NANOS_SHARED;
            cd[2].flags.input = true;
            cd[2].flags.output = false;
            cd[2].size = bsize * bsize * sizeof(float);
            //cd[2] = (nanos_copy_data_t) { (uint64_t) args->b, NANOS_SHARED, { true, false }, bsize * bsize * sizeof(float) };
            
            cd[3].address = (uint64_t) args->c;
            cd[3].sharing = NANOS_SHARED;
            cd[3].flags.input = true;
            cd[3].flags.output = true;
            cd[3].size = bsize * bsize * sizeof(float);
            //cd[3] = (nanos_copy_data_t) { (uint64_t) args->c, NANOS_SHARED, { true, true }, bsize * bsize * sizeof(float) };

            nanos_dependence_t deps[3] = {
               {
                  (void **) args->a,
                  ((char *) (&a[i * nb * bsize * bsize + k * bsize * bsize]) - (char *) args->a),
                  { 1, 0, 0 },
                  sizeof(float) * bsize * bsize
               },
               {
                  (void **) args->b,
                  ((char *) (&b[k * nb * bsize * bsize + j * bsize * bsize]) - (char *) args->b),
                  { 1, 0, 0 },
                  sizeof(float) * bsize * bsize
               },
               {
                  (void **) args->c,
                  ((char *) (&c[i * nb * bsize * bsize + j * bsize * bsize]) - (char *) args->c),
                  { 1, 1, 0 },
                  sizeof(float) * bsize * bsize
               }
            };
            
            NANOS_SAFE( nanos_submit( wd, 1, deps, 0 ) );
            
            usleep(500);
         }
      }
   }


   NANOS_SAFE( nanos_wg_wait_completion( nanos_current_wd() ) );

   // Error checking
   int err = 0;
   for ( i = 0; i < n; i++ ) {
      for ( j = 0; j < n; j++ ) {
         if ( c[i*n+j] != bsize * nb * 2 ) {
        	 std::cout << "Error at " << i << ", " << j << ": (GPU) " << c[i*n+j] << " vs (CPU) " << bsize * nb * 2 << std::endl;
            err++;
         }
      }
   }

   if ( err ) {
         std::cout << err << " errors found: matrix multiply did not succeed.   FAIL" << std::endl;
         exit( 1 );
   }
   
   std::cout << "   Execution ... ok!" << std::endl;
   
   std::cout << "End testing matrix multiply on GPU" << std::endl;
    
   
   return 0;
}



///#pragma omp target device (cuda) copydeps
///#pragma omp task input(n, idx, a) output(b, err) inout(c)
void matmul_task ( void * args )
{

   test_args * targs = new test_args();

   targs->bs = ( (test_args *) args)->bs;
   nanos_get_addr(1, ( void ** ) &targs->a);
   nanos_get_addr(2, ( void ** ) &targs->b);
   nanos_get_addr(3, ( void ** ) &targs->c);
   
   dim3 dimBlock(targs->bs, targs->bs);
   matmul_gpu <<< 1, dimBlock >>> ( targs->a, targs->b, targs->c );
   
   
   cudaThreadSynchronize();
   
   delete targs;

}


/***** GPU CODE *****/
__global__ void matmul_gpu ( float * a , float * b, float * c )
{
   int idx = threadIdx.x * blockDim.x + threadIdx.y;
   int i = threadIdx.x;
   int j = threadIdx.y;

   int k;
   for ( k = 0; k < blockDim.x; k++ ) {
      c[idx] += a[i*blockDim.x + k] * b[k*blockDim.x + j];
   }
}
/***** END GPU CODE *****/

