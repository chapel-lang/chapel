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
 * ---------------------------------------------------------------------
 * Static variables
 * ---------------------------------------------------------------------
 */
static int                    HPL_timer_disabled;
static double                 HPL_timer_cpusec   [HPL_NTIMER],
                              HPL_timer_cpustart [HPL_NTIMER],
                              HPL_timer_wallsec  [HPL_NTIMER],
                              HPL_timer_wallstart[HPL_NTIMER];
/*
 * ---------------------------------------------------------------------
 * User callable functions
 * ---------------------------------------------------------------------
 */
#ifdef HPL_STDC_HEADERS
void HPL_timer_boot( void )
#else
void HPL_timer_boot()
#endif
{
/*
 * HPL_timer_boot (re)sets all timers to 0, and enables HPL_timer.
 */
/*
 * .. Local Variables ..
 */
   int                        i;
/* ..
 * .. Executable Statements ..
 */
   HPL_timer_disabled = 0;

   for( i = 0; i < HPL_NTIMER; i++ )
   {
      HPL_timer_cpusec  [i] = HPL_timer_wallsec  [i] = HPL_rzero;
      HPL_timer_cpustart[i] = HPL_timer_wallstart[i] = HPL_TIMER_STARTFLAG;
   }
/*
 * End of HPL_timer_boot
 */
}

#ifdef HPL_STDC_HEADERS
void HPL_timer( const int I )
#else
void HPL_timer( I )
   const int                  I;
#endif
{
/* 
 * Purpose
 * =======
 *
 * HPL_timer provides a  "stopwatch"  functionality  cpu/wall  timer  in
 * seconds.  Up to  64  separate timers can be functioning at once.  The
 * first call starts the timer,  and the second stops it.  This  routine
 * can be disenabled  by calling  HPL_timer_disable(),  so that calls to
 * the timer are ignored.  This feature can be used to make sure certain
 * sections of code do not affect timings,  even  if  they call routines
 * which have HPL_timer calls in them. HPL_timer_enable() will re-enable
 * the  timer  functionality.  One  can retrieve  the current value of a
 * timer by calling
 *  
 * t0 = HPL_timer_inquire( HPL_WALL_TIME | HPL_CPU_TIME, I )
 *  
 * where  I  is the timer index in  [0..64).  To  initialize  the  timer
 * functionality, one must have called HPL_timer_boot()  prior to any of
 * the functions mentioned above.
 *
 * Arguments
 * =========
 *
 * I       (global input)                const int
 *         On entry, I specifies the timer to stop/start.
 *
 * ---------------------------------------------------------------------
 */ 
/* ..
 * .. Executable Statements ..
 */
   if( HPL_timer_disabled ) return;
/*
 * If timer has not been started, start it.  Otherwise,  stop it and add
 * interval to count
 */
   if( HPL_timer_wallstart[I] == HPL_TIMER_STARTFLAG )
   {
      HPL_timer_wallstart[I] = HPL_timer_walltime();
      HPL_timer_cpustart [I] = HPL_timer_cputime ();
   }
   else
   {
      HPL_timer_cpusec   [I] += HPL_timer_cputime () - HPL_timer_cpustart [I];
      HPL_timer_wallsec  [I] += HPL_timer_walltime() - HPL_timer_wallstart[I];
      HPL_timer_wallstart[I]  = HPL_TIMER_STARTFLAG;
   }
/*
 * End of HPL_timer
 */
}

#ifdef HPL_STDC_HEADERS
void HPL_timer_enable( void )
#else
void HPL_timer_enable()
#endif
{
/*
 * HPL_timer_enable sets it so calls to HPL_timer are not ignored.
 */
/* ..
 * .. Executable Statements ..
 */
   HPL_timer_disabled = 0;
   return;
/*
 * End of HPL_timer_enable
 */
}

#ifdef HPL_STDC_HEADERS
void HPL_timer_disable( void )
#else
void HPL_timer_disable()
#endif
{
/*
 * HPL_timer_disable sets it so calls to HPL_timer are ignored.
 */
/* ..
 * .. Executable Statements ..
 */
   HPL_timer_disabled = 1;
   return;
/*
 * End of HPL_timer_disable
 */
}

#ifdef HPL_STDC_HEADERS
double HPL_timer_inquire
(
   const HPL_T_TIME           TMTYPE,
   const int                  I
)
#else
double HPL_timer_inquire( TMTYPE, I )
   const int                  I;
   const HPL_T_TIME           TMTYPE;
#endif
{
/*
 * Purpose
 * =======
 *
 * HPL_timer_inquire returns  wall- or cpu- time that has accumulated in
 * timer I.
 *
 * Arguments
 * =========
 *
 * TMTYPE  (global input)              const HPL_T_TIME
 *         On entry, TMTYPE specifies what time will be returned as fol-
 *         lows
 *            = HPL_WALL_TIME : wall clock time is returned,
 *            = HPL_CPU_TIME  : CPU time is returned (default).
 *
 * I       (global input)              const int
 *         On entry, I specifies the timer to return.
 *
 * ---------------------------------------------------------------------
 */
/*
 * .. Local Variables ..
 */
   double          time;
/* ..
 * .. Executable Statements ..
 */
/*
 * If wall- or cpu-time are not available on this machine, return
 * HPL_TIMER_ERROR
 */
   if( TMTYPE == HPL_WALL_TIME )
   {
      if( HPL_timer_walltime() == HPL_TIMER_ERROR )
         time = HPL_TIMER_ERROR;
      else
         time = HPL_timer_wallsec[I];
   }
   else
   {
      if( HPL_timer_cputime()  == HPL_TIMER_ERROR )
         time = HPL_TIMER_ERROR;
      else
         time = HPL_timer_cpusec [I];
   }
   return( time );
/*
 * End of HPL_timer_inquire
 */
}
