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
test_generator=gens/api-generator
</testinfo>
*/

#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <nanos.h>

int fib_seq ( int n )
{
   int x, y;

   if ( n < 2 ) return n;

   x = fib_seq( n-1 );

   y = fib_seq( n-2 );

   return x + y;
}

int fib ( int n, int d );

typedef struct {
   int n;
   int d;
   int *x;
} fib_args;

void fib_1( void *ptr )
{
   fib_args * args = ( fib_args * )ptr;
   *args->x = fib( args->n-1,args->d+1 );
}

void fib_2( void *ptr )
{
   fib_args * args = ( fib_args * )ptr;
   *args->x = fib( args->n-2,args->d+1 );
}

nanos_smp_args_t fib_device_arg_1 = { fib_1 };
nanos_smp_args_t fib_device_arg_2 = { fib_2 };


int fib ( int n, int d )
{
   int x, y;

   if ( n < 2 ) return n;

//       #pragma omp task untied shared(x) firstprivate(n,d)
//      x = fib(n - 1,d+1);
   {
      nanos_wd_t wd=0;
      fib_args *args=0;
      nanos_device_t fib_devices_1[1] = { NANOS_SMP_DESC( fib_device_arg_1 ) };

      nanos_create_wd ( &wd, 1, fib_devices_1 , sizeof( fib_args ), __alignof__( fib_args ),
                        ( void ** )&args, nanos_current_wd(), 0, 0, NULL );

      if ( wd != 0 ) {
         args->n = n;
         args->d = d;
         args->x = &x;
         nanos_submit( wd,0,0,0 );
      } else
         x = fib_seq( n-1 );
   }

//            #pragma omp task untied shared(y) firstprivate(n,d)
//            y = fib(n - 2,d+1);
   {
      nanos_wd_t wd=0;
      fib_args *args=0;
      nanos_device_t fib_devices_2[1] = { NANOS_SMP_DESC( fib_device_arg_2 ) };
      
      nanos_create_wd ( &wd, 1, fib_devices_2 , sizeof( fib_args ), __alignof__(fib_args), ( void ** )&args, nanos_current_wd(), 0, 0, NULL );

      if ( wd != 0 ) {
         args->n = n;
         args->d = d;
         args->x = &y;
         nanos_submit( wd,0,0,0 );
      } else
         y = fib_seq( n-2 );

   }

//            #pragma omp taskwait
   nanos_wg_wait_completion( nanos_current_wd() );

   return x + y;
}

double get_wtime( void )
{

   struct timeval ts;
   double t;
   int err;

   err = gettimeofday( &ts, NULL );
   t = ( double ) ( ts.tv_sec )  + ( double ) ts.tv_usec * 1.0e-6;

   return t;
}

int fib0 ( int n )
{
   double start,end;
   int par_res;

   start = get_wtime();
   par_res = fib( n,0 );
   end = get_wtime();

   printf( "Fibonacci result for %d is %d\n", n, par_res );
   printf( "Computation time: %f seconds.\n",  end - start );
   return par_res;
}


int main ( int argc, char **argv )
{
   int n=25;

   if ( argc > 1 ) n = atoi( argv[1] );

   if ( fib0( n ) != fib_seq(n) ) return 1;

   return 0;
}
