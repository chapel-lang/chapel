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
test_generator=gens/core-generator
</testinfo>
*/

#include <iostream>
#include "system.hpp"
#include "smpprocessor.hpp"
#include "basethread.hpp"
#include <sys/time.h>

int cutoff_value = 10;

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

void fib_0( void *ptr )
{
   fib_args * args = ( fib_args * )ptr;
   *args->x = fib( args->n-1,args->d+1 );
}

void fib_1( void *ptr )
{
   fib_args * args = ( fib_args * )ptr;
   *args->x = fib( args->n-2,args->d+1 );
}

int fib ( int n, int d )
{
   int x, y;

   if ( n < 2 ) return n;

   if ( d < cutoff_value ) {
      nanos::WG *wg = nanos::getMyThreadSafe()->getCurrentWD();

//		#pragma omp task untied shared(x) firstprivate(n,d)
//		x = fib(n - 2,d+1);
      {
         fib_args * args = new fib_args();
         args->n = n;
         args->d = d;
         args->x = &x;
         nanos::WD * wd = new nanos::WD( new nanos::ext::SMPDD( fib_0 ), sizeof(fib_args), __alignof__(fib_args), args );
         wg->addWork( *wd );
         nanos::sys.submit( *wd );
      }

//		#pragma omp task untied shared(y) firstprivate(n,d)
//		y = fib(n - 2,d+1);
      {
         fib_args * args = new fib_args();
         args->n = n;
         args->d = d;
         args->x = &y;
         nanos::WD * wd = new nanos::WD( new nanos::ext::SMPDD( fib_1 ), sizeof(fib_args), __alignof__(fib_args), args );
         wg->addWork( *wd );
         nanos::sys.submit( *wd );
      }

//		#pragma omp taskwait
      wg->waitCompletion();
   } else {
      x = fib_seq( n-1 );
      y = fib_seq( n-2 );
   }

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

   std::cout << "Fibonacci result for " << n << " is " << par_res << std::endl;
   std::cout << "Computation time:  " << end - start << " seconds." << std::endl;
   return par_res;
}


int main ( int argc, char **argv )
{
   int n=25;

   if ( argc > 1 ) n = atoi( argv[1] );

   if ( fib0( n ) != 75025 ) return 1;
   return 0;
}
