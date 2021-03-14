
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

// Function to return time in seconds.
// If compiled with no flags, return CPU time (user and system).
// If compiled with -DWALL, returns elapsed time.

/////////////////////////////////////////////////////////////////////////
#ifdef USING_MPI
#include <mpi.h> // If this routine is compiled with -DUSING_MPI
                 // then include mpi.h
double mytimer(void)
{
   return(MPI_Wtime());
}


#elif defined(UseClock)

#include <time.hpp>
double mytimer(void)
{
   clock_t t1;
   static clock_t t0=0;
   static double CPS = CLOCKS_PER_SEC;
   double d;

   if (t0 == 0) t0 = clock();
   t1 = clock() - t0;
   d = t1 / CPS;
   return(d);
}

#elif defined(WALL)

#include <cstdlib>
#include <sys/time.h>
#include <sys/resource.h>
double mytimer(void)
{
   struct timeval tp;
   static long start=0, startu;
   if (!start)
   {
      gettimeofday(&tp, NULL);
      start = tp.tv_sec;
      startu = tp.tv_usec;
      return(0.0);
   }
   gettimeofday(&tp, NULL);
   return( ((double) (tp.tv_sec - start)) + (tp.tv_usec-startu)/1000000.0 );
}

#elif defined(UseTimes)

#include <cstdlib>
#include <sys/times.h>
#include <unistd.h>
double mytimer(void)
{
   struct tms ts;
   static double ClockTick=0.0;

   if (ClockTick == 0.0) ClockTick = (double) sysconf(_SC_CLK_TCK);
   times(&ts);
   return( (double) ts.tms_utime / ClockTick );
}

#else

#include <cstdlib>
#include <sys/time.h>
#include <sys/resource.h>
double mytimer(void)
{
   struct rusage ruse;
   getrusage(RUSAGE_SELF, &ruse);
   return( (double)(ruse.ru_utime.tv_sec+ruse.ru_utime.tv_usec / 1000000.0) );
}

#endif
