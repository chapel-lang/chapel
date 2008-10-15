/* 
 * -- High Performance Computing Linpack Benchmark (HPL)                
 *    HPL - 1.0a - January 20, 2004                          
 *    Antoine P. Petitet                                                
 *    University of Tennessee, Knoxville                                
 *    Innovative Computing Laboratories                                 
 *    (C) Copyright 2000-2004 All Rights Reserved                       
 *                                                                      
 * -- Copyright notice and Licensing terms:                             
 *                                                                      
 * Redistribution  and  use in  source and binary forms, with or without
 * modification, are  permitted provided  that the following  conditions
 * are met:                                                             
 *                                                                      
 * 1. Redistributions  of  source  code  must retain the above copyright
 * notice, this list of conditions and the following disclaimer.        
 *                                                                      
 * 2. Redistributions in binary form must reproduce  the above copyright
 * notice, this list of conditions,  and the following disclaimer in the
 * documentation and/or other materials provided with the distribution. 
 *                                                                      
 * 3. All  advertising  materials  mentioning  features  or  use of this
 * software must display the following acknowledgement:                 
 * This  product  includes  software  developed  at  the  University  of
 * Tennessee, Knoxville, Innovative Computing Laboratories.             
 *                                                                      
 * 4. The name of the  University,  the name of the  Laboratory,  or the
 * names  of  its  contributors  may  not  be used to endorse or promote
 * products  derived   from   this  software  without  specific  written
 * permission.                                                          
 *                                                                      
 * -- Disclaimer:                                                       
 *                                                                      
 * THIS  SOFTWARE  IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,  INCLUDING,  BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE UNIVERSITY
 * OR  CONTRIBUTORS  BE  LIABLE FOR ANY  DIRECT,  INDIRECT,  INCIDENTAL,
 * SPECIAL,  EXEMPLARY,  OR  CONSEQUENTIAL DAMAGES  (INCLUDING,  BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA OR PROFITS; OR BUSINESS INTERRUPTION)  HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT,  STRICT LIABILITY,  OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 * ---------------------------------------------------------------------
 */ 
/*
 * Include files
 */
#include "hpl.h"

/* 
 * Purpose
 * =======
 *
 * HPL_timer_cputime returns the cpu time.  If HPL_USE_CLOCK is defined,
 * the  clock() function is used to return an approximation of processor
 * time used by the program.  The value returned is the CPU time used so
 * far as a clock_t;  to get the number of seconds used,  the result  is
 * divided by  CLOCKS_PER_SEC.  This function is part of the  ANSI/ISO C
 * standard library.  If  HPL_USE_TIMES is defined, the times() function
 * is used instead.  This  function  returns  the current process times.
 * times() returns the number of clock ticks that have elapsed since the
 * system has been up.  Otherwise and by default,  the  standard library
 * function getrusage() is used.
 *
 * ---------------------------------------------------------------------
 */ 

#if   defined( HPL_USE_CLOCK )
 
#include <time.h>

#ifdef HPL_STDC_HEADERS
double HPL_timer_cputime( void )
#else
double HPL_timer_cputime()
#endif
{
   static double              cps = CLOCKS_PER_SEC;
   double                     d;
   clock_t                    t1;
   static clock_t             t0 = 0;
 
   if( t0 == 0 ) t0 = clock();
   t1 = clock() - t0;
   d = (double)(t1) / cps;
   return( d );
}

#elif defined( HPL_USE_TIMES )
 
#include <sys/times.h>
#include <unistd.h>

#ifdef HPL_STDC_HEADERS
double HPL_timer_cputime( void )
#else
double HPL_timer_cputime()
#endif
{
   clock_t                    t1;
   struct tms                 ts;
   static double              ClockTick = HPL_rzero;
 
   if( ClockTick == HPL_rzero ) ClockTick = (double)(sysconf(_SC_CLK_TCK));
   (void) times( &ts );
   return( (double)(ts.tms_utime) / ClockTick );
}

#elif defined( HPL_USE_GETPROCESSTIMES )

#include <time.h>
#include <windows.h>

#ifdef HPL_STDC_HEADERS
double HPL_timer_cputime( void )
#else
double HPL_timer_cputime()
#endif
{
  FILETIME creation, exit, kernel, user;
  GetProcessTimes( GetCurrentProcess(), &creation, &exit, &kernel, &user );
  return (*(LONGLONG*)&kernel+*(LONGLONG*)&user)*1e-7;
}

/* #elif defined( HPL_USE_GETRUSAGE ) */
#else
 
#include <sys/time.h>
#include <sys/resource.h>

#ifdef HPL_STDC_HEADERS
double HPL_timer_cputime( void )
#else
double HPL_timer_cputime()
#endif
{
   struct rusage              ruse;

   (void) getrusage( RUSAGE_SELF, &ruse );
   return( (double)( ruse.ru_utime.tv_sec  ) +
           ( (double)( ruse.ru_utime.tv_usec ) / 1000000.0 ) );
}

/* 
#else

#ifdef HPL_STDC_HEADERS
double HPL_timer_cputime( void )
#else
double HPL_timer_cputime()
#endif
{
   return( HPL_TIMER_ERROR );
}
*/

#endif
/*
 * End of HPL_timer_cputime
 */
