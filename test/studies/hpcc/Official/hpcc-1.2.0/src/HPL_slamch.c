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
#include <hpcc.h>
#ifdef HPL_rone
#undef HPL_rone
#endif
#define HPL_rone 1.0f
#ifdef HPL_rtwo
#undef HPL_rtwo
#endif
#define HPL_rtwo 2.0f
#ifdef HPL_rzero
#undef HPL_rzero
#endif
#define HPL_rzero 0.0f
/*
 * ---------------------------------------------------------------------
 * Static function prototypes
 * ---------------------------------------------------------------------
 */
static void     HPL_slamc1
STDC_ARGS(
(  int *,           int *,           int *,           int * ) );
static void     HPL_slamc2
STDC_ARGS(
(  int *,           int *,           int *,           float *,
   int *,           float *,        int *,           float * ) );
static float   HPL_slamc3
STDC_ARGS(
(  const float,    const float ) );
static void     HPL_slamc4
STDC_ARGS(
(  int *,           const float,    const int ) );
static void     HPL_slamc5
STDC_ARGS(
(  const int,       const int,       const int,       const int,
   int *,           float * ) );
static float   HPL_sipow
STDC_ARGS(
(  const float,    const int ) );

#ifdef HPL_STDC_HEADERS
float HPL_slamch
(
   const HPL_T_MACH                 CMACH
)
#else
float HPL_slamch
( CMACH )
   const HPL_T_MACH                 CMACH;
#endif
{
/*
 * Purpose
 * =======
 *
 * HPL_slamch determines  machine-specific  arithmetic constants such as
 * the relative machine precision  (eps),  the safe minimum (sfmin) such
 * that 1 / sfmin does not overflow, the base of the machine (base), the
 * precision (prec), the  number of (base) digits  in the  mantissa (t),
 * whether rounding occurs in addition (rnd=1.0 and 0.0 otherwise),  the
 * minimum exponent before  (gradual)  underflow (emin),  the  underflow
 * threshold (rmin) base**(emin-1), the largest exponent before overflow
 * (emax), the overflow threshold (rmax) (base**emax)*(1-eps).
 *
 * Notes
 * =====
 *
 * This function has been manually translated from the Fortran 77 LAPACK
 * auxiliary function slamch.f  (version 2.0 -- 1992), that  was  itself
 * based on the function ENVRON  by Malcolm and incorporated suggestions
 * by Gentleman and Marovich. See
 *
 * Malcolm M. A.,  Algorithms  to  reveal  properties  of floating-point
 * arithmetic.,  Comms. of the ACM, 15, 949-951 (1972).
 *
 * Gentleman W. M. and Marovich S. B.,  More  on algorithms  that reveal
 * properties of  floating point arithmetic units.,  Comms. of  the ACM,
 * 17, 276-277 (1974).
 *
 * Arguments
 * =========
 *
 * CMACH   (local input)                 const HPL_T_MACH
 *         Specifies the value to be returned by HPL_slamch
 *            = HPL_MACH_EPS,   HPL_slamch := eps (default)
 *            = HPL_MACH_SFMIN, HPL_slamch := sfmin
 *            = HPL_MACH_BASE,  HPL_slamch := base
 *            = HPL_MACH_PREC,  HPL_slamch := eps*base
 *            = HPL_MACH_MLEN,  HPL_slamch := t
 *            = HPL_MACH_RND,   HPL_slamch := rnd
 *            = HPL_MACH_EMIN,  HPL_slamch := emin
 *            = HPL_MACH_RMIN,  HPL_slamch := rmin
 *            = HPL_MACH_EMAX,  HPL_slamch := emax
 *            = HPL_MACH_RMAX,  HPL_slamch := rmax
 *
 *         where
 *
 *            eps   = relative machine precision,
 *            sfmin = safe minimum,
 *            base  = base of the machine,
 *            prec  = eps*base,
 *            t     = number of digits in the mantissa,
 *            rnd   = 1.0 if rounding occurs in addition,
 *            emin  = minimum exponent before underflow,
 *            rmin  = underflow threshold,
 *            emax  = largest exponent before overflow,
 *            rmax  = overflow threshold.
 *
 * ---------------------------------------------------------------------
 */
/*
 * .. Local Variables ..
 */
   static float              eps, sfmin, base, t, rnd, emin, rmin, emax,
                              rmax, prec;
   float                     small;
   static int                 first=1;
   int                        beta=0, imax=0, imin=0, it=0, lrnd=0;
/* ..
 * .. Executable Statements ..
 */
   if( first != 0 )
   {
      first = 0;
      HPL_slamc2( &beta, &it, &lrnd, &eps, &imin, &rmin, &imax, &rmax );
      base  = (float)(beta);  t     = (float)(it);
      if( lrnd != 0 )
      { rnd = HPL_rone;  eps = HPL_sipow( base, 1 - it ) / HPL_rtwo; }
      else
      { rnd = HPL_rzero; eps = HPL_sipow( base, 1 - it );            }
      prec  = eps * base;  emin  = (float)(imin); emax  = (float)(imax);
      sfmin = rmin;        small = HPL_rone / rmax;
/*
 * Use  SMALL  plus a bit,  to avoid the possibility of rounding causing
 * overflow when computing  1/sfmin.
 */
      if( small >= sfmin ) sfmin = small * ( HPL_rone + eps );
   }

   if( CMACH == HPL_MACH_EPS   ) return( eps   );
   if( CMACH == HPL_MACH_SFMIN ) return( sfmin );
   if( CMACH == HPL_MACH_BASE  ) return( base  );
   if( CMACH == HPL_MACH_PREC  ) return( prec  );
   if( CMACH == HPL_MACH_MLEN  ) return( t     );
   if( CMACH == HPL_MACH_RND   ) return( rnd   );
   if( CMACH == HPL_MACH_EMIN  ) return( emin  );
   if( CMACH == HPL_MACH_RMIN  ) return( rmin  );
   if( CMACH == HPL_MACH_EMAX  ) return( emax  );
   if( CMACH == HPL_MACH_RMAX  ) return( rmax  );

   return( eps );
/*
 * End of HPL_slamch
 */
}

#ifdef HPL_STDC_HEADERS
static void HPL_slamc1
(
   int                        * BETA,
   int                        * T,
   int                        * RND,
   int                        * IEEE1
)
#else
static void HPL_slamc1
( BETA, T, RND, IEEE1 )
/*
 * .. Scalar Arguments ..
 */
   int                        * BETA, * IEEE1, * RND, * T;
#endif
{
/*
 * Purpose
 * =======
 *
 * HPL_slamc1  determines  the machine parameters given by BETA, T, RND,
 * and IEEE1.
 *
 * Notes
 * =====
 *
 * This function has been manually translated from the Fortran 77 LAPACK
 * auxiliary function slamc1.f  (version 2.0 -- 1992), that  was  itself
 * based on the function ENVRON  by Malcolm and incorporated suggestions
 * by Gentleman and Marovich. See
 *
 * Malcolm M. A.,  Algorithms  to  reveal  properties  of floating-point
 * arithmetic.,  Comms. of the ACM, 15, 949-951 (1972).
 *
 * Gentleman W. M. and Marovich S. B.,  More  on algorithms  that reveal
 * properties of  floating point arithmetic units.,  Comms. of  the ACM,
 * 17, 276-277 (1974).
 *
 * Arguments
 * =========
 *
 * BETA    (local output)              int *
 *         The base of the machine.
 *
 * T       (local output)              int *
 *         The number of ( BETA ) digits in the mantissa.
 *
 * RND     (local output)              int *
 *         Specifies whether proper rounding (RND=1) or chopping (RND=0)
 *         occurs in addition.  This may not be a  reliable guide to the
 *         way in which the machine performs its arithmetic.
 *
 * IEEE1   (local output)              int *
 *         Specifies  whether  rounding  appears  to be done in the IEEE
 *         `round to nearest' style (IEEE1=1), (IEEE1=0) otherwise.
 *
 * ---------------------------------------------------------------------
 */
/*
 * .. Local Variables ..
 */
   float                     a, b, c, f, one, qtr, savec, t1, t2;
   static int                 first=1, lbeta, lieee1, lrnd, lt;
/* ..
 * .. Executable Statements ..
 */
   if( first != 0 )
   {
      first = 0; one = HPL_rone;
/*
 * lbeta, lieee1, lt and lrnd are the local values of BETA, IEEE1, T and
 * RND. Throughout this routine we use the function HPL_slamc3 to ensure
 * that relevant values are stored and not held in registers, or are not
 * affected by optimizers.
 *
 * Compute  a = 2.0**m  with the  smallest  positive integer m such that
 * fl( a + 1.0 ) == a.
 */
      a = HPL_rone; c = HPL_rone;
      do
      { a *= HPL_rtwo; c = HPL_slamc3( a, one ); c = HPL_slamc3( c, -a ); }
      while( c == HPL_rone );
/*
 * Now compute b = 2.0**m with the smallest positive integer m such that
 * fl( a + b ) > a.
 */
      b = HPL_rone; c = HPL_slamc3( a, b );
      while( c == a ) { b *= HPL_rtwo; c = HPL_slamc3( a, b ); }
/*
 * Now compute the base.  a and c  are  neighbouring floating point num-
 * bers in the interval ( BETA**T, BETA**( T + 1 ) ) and so their diffe-
 * rence is BETA.  Adding 0.25 to c is to ensure that it is truncated to
 * BETA and not (BETA-1).
 */
      qtr = one / 4.0; savec = c;
      c   = HPL_slamc3( c, -a ); lbeta = (int)(c+qtr);
/*
 * Now  determine  whether  rounding or chopping occurs, by adding a bit
 * less than BETA/2 and a bit more than BETA/2 to a.
 */
      b = (float)(lbeta);
      f = HPL_slamc3( b / HPL_rtwo, -b / 100.0 ); c = HPL_slamc3( f, a );
      if( c == a ) { lrnd = 1; } else { lrnd = 0; }
      f = HPL_slamc3( b / HPL_rtwo,  b / 100.0 ); c = HPL_slamc3( f, a );
      if( ( lrnd != 0 ) && ( c == a ) ) lrnd = 0;
/*
 * Try  and decide whether rounding is done in the  IEEE  round to nea-
 * rest style.  b/2 is half a unit in the last place of the two numbers
 * a  and savec. Furthermore, a is even, i.e. has last bit zero, and sa-
 * vec is odd.  Thus adding b/2 to a should not change a, but adding b/2
 * to savec should change savec.
 */
      t1 = HPL_slamc3( b / HPL_rtwo, a );
      t2 = HPL_slamc3( b / HPL_rtwo, savec );
      if ( ( t1 == a ) && ( t2 > savec ) && ( lrnd != 0 ) ) lieee1 = 1;
      else                                                  lieee1 = 0;
/*
 * Now find the mantissa, T. It should be the integer part of log to the
 * base BETA of a, however it is safer to determine T by powering. So we
 * find T as the smallest positive integer for which fl( beta**t + 1.0 )
 * is equal to 1.0.
 */
      lt = 0; a = HPL_rone; c = HPL_rone;

      do
      {
         lt++; a *= (float)(lbeta);
         c = HPL_slamc3( a, one ); c = HPL_slamc3( c,  -a );
      } while( c == HPL_rone );
   }

   *BETA  = lbeta; *T = lt; *RND = lrnd; *IEEE1 = lieee1;
}

#ifdef HPL_STDC_HEADERS
static void HPL_slamc2
(
   int                        * BETA,
   int                        * T,
   int                        * RND,
   float                     * EPS,
   int                        * EMIN,
   float                     * RMIN,
   int                        * EMAX,
   float                     * RMAX
)
#else
static void HPL_slamc2( BETA, T, RND, EPS, EMIN, RMIN, EMAX, RMAX )
/*
 * .. Scalar Arguments ..
 */
   int                        * BETA, * EMAX, * EMIN, * RND, * T;
   float                     * EPS, * RMAX, * RMIN;
#endif
{
/*
 * Purpose
 * =======
 *
 * HPL_slamc2  determines the machine  parameters specified in its argu-
 * ment list.
 *
 * Notes
 * =====
 *
 * This function has been manually translated from the Fortran 77 LAPACK
 * auxiliary function  slamc2.f (version 2.0 -- 1992), that  was  itself
 * based on a function PARANOIA  by  W. Kahan of the University of Cali-
 * fornia at Berkeley for the computation of the  relative machine epsi-
 * lon eps.
 *
 * Arguments
 * =========
 *
 * BETA    (local output)              int *
 *         The base of the machine.
 *
 * T       (local output)              int *
 *         The number of ( BETA ) digits in the mantissa.
 *
 * RND     (local output)              int *
 *         Specifies whether proper rounding (RND=1) or chopping (RND=0)
 *         occurs in addition. This may not be a reliable  guide to  the
 *         way in which the machine performs its arithmetic.
 *
 * EPS     (local output)              float *
 *         The smallest positive number such that fl( 1.0 - EPS ) < 1.0,
 *         where fl denotes the computed value.
 *
 * EMIN    (local output)              int *
 *         The minimum exponent before (gradual) underflow occurs.
 *
 * RMIN    (local output)              float *
 *         The smallest  normalized  number  for  the  machine, given by
 *         BASE**( EMIN - 1 ), where  BASE  is the floating  point value
 *         of BETA.
 *
 * EMAX    (local output)              int *
 *         The maximum exponent before overflow occurs.
 *
 * RMAX    (local output)              float *
 *         The  largest  positive  number  for  the  machine,  given  by
 *         BASE**EMAX * ( 1 - EPS ), where  BASE  is the floating  point
 *         value of BETA.
 *
 * ---------------------------------------------------------------------
 */
/*
 * .. Local Variables ..
 */
   static float              leps, lrmax, lrmin;
   float                     a, b, c, half, one, rbase, sixth, small,
                              third, two, zero;
   static int                 first=1, iwarn=0, lbeta=0, lemax, lemin,
                              lt=0;
   int                        gnmin=0, gpmin=0, i, ieee, lieee1=0,
                              lrnd=0, ngnmin=0, ngpmin=0;
/* ..
 * .. Executable Statements ..
 */
   if( first != 0 )
   {
      first = 0; zero = HPL_rzero; one = HPL_rone; two = HPL_rtwo;
/*
 * lbeta, lt, lrnd, leps, lemin and lrmin are the local values of  BETA,
 * T, RND, EPS, EMIN and RMIN.
 *
 * Throughout this routine we use the function HPL_slamc3 to ensure that
 * relevant values are stored and not held in registers,  or are not af-
 * fected by optimizers.
 *
 * HPL_slamc1 returns the parameters  lbeta, lt, lrnd and lieee1.
 */
      HPL_slamc1( &lbeta, &lt, &lrnd, &lieee1 );
/*
 * Start to find eps.
 */
      b = (float)(lbeta); a = HPL_sipow( b, -lt ); leps = a;
/*
 * Try some tricks to see whether or not this is the correct  EPS.
 */
      b     = two / 3.0;
      half  = one / HPL_rtwo;
      sixth = HPL_slamc3( b, -half );
      third = HPL_slamc3( sixth, sixth );
      b     = HPL_slamc3( third, -half );
      b     = HPL_slamc3( b, sixth );
      b     = Mabs( b ); if( b < leps ) b = leps;

      leps = HPL_rone;

      while( ( leps > b ) && ( b > zero ) )
      {
         leps = b;
         c = HPL_slamc3( half * leps,
                         HPL_sipow( two, 5 ) * HPL_sipow( leps, 2 ) );
         c = HPL_slamc3( half, -c ); b = HPL_slamc3( half, c );
         c = HPL_slamc3( half, -b ); b = HPL_slamc3( half, c );
      }
      if( a < leps ) leps = a;
/*
 * Computation of EPS complete.
 *
 * Now find  EMIN.  Let a = + or - 1, and + or - (1 + BASE**(-3)).  Keep
 * dividing a by BETA until (gradual) underflow occurs. This is detected
 * when we cannot recover the previous a.
 */
      rbase = one / (float)(lbeta); small = one;
      for( i = 0; i < 3; i++ ) small = HPL_slamc3( small * rbase, zero );
      a = HPL_slamc3( one, small );
      HPL_slamc4( &ngpmin, one, lbeta ); HPL_slamc4( &ngnmin, -one, lbeta );
      HPL_slamc4( &gpmin,    a, lbeta ); HPL_slamc4( &gnmin,    -a, lbeta );

      ieee = 0;

      if( ( ngpmin == ngnmin ) && ( gpmin == gnmin ) )
      {
         if( ngpmin == gpmin )
         {
/*
 * Non twos-complement machines, no gradual underflow; e.g.,  VAX )
 */
            lemin = ngpmin;
         }
         else if( ( gpmin-ngpmin ) == 3 )
         {
/*
 * Non twos-complement machines with gradual underflow; e.g., IEEE stan-
 * dard followers
 */
            lemin = ngpmin - 1 + lt; ieee = 1;
         }
         else
         {
/*
 * A guess; no known machine
 */
            lemin = Mmin( ngpmin, gpmin );
            iwarn = 1;
         }
      }
      else if( ( ngpmin == gpmin ) && ( ngnmin == gnmin ) )
      {
         if( Mabs( ngpmin-ngnmin ) == 1 )
         {
/*
 * Twos-complement machines, no gradual underflow; e.g., CYBER 205
 */
            lemin = Mmax( ngpmin, ngnmin );
         }
         else
         {
/*
 * A guess; no known machine
 */
            lemin = Mmin( ngpmin, ngnmin );
            iwarn = 1;
         }
      }
      else if( ( Mabs( ngpmin-ngnmin ) == 1 ) && ( gpmin == gnmin ) )
      {
         if( ( gpmin - Mmin( ngpmin, ngnmin ) ) == 3 )
         {
/*
 * Twos-complement machines with gradual underflow; no known machine
 */
            lemin = Mmax( ngpmin, ngnmin ) - 1 + lt;
         }
         else
         {
/*
 * A guess; no known machine
 */
            lemin = Mmin( ngpmin, ngnmin );
            iwarn = 1;
         }
      }
      else
      {
/*
 * A guess; no known machine
 */
         lemin = Mmin( ngpmin, ngnmin ); lemin = Mmin( lemin, gpmin );
         lemin = Mmin( lemin, gnmin ); iwarn = 1;
      }
/*
 * Comment out this if block if EMIN is ok
 */
      if( iwarn != 0 )
      {
         first = 1;
         HPL_fprintf( stderr, "\n %s %8d\n%s\n%s\n%s\n",
"WARNING. The value EMIN may be incorrect:- EMIN =", lemin,
"If, after inspection, the value EMIN looks acceptable, please comment ",
"out the  if  block  as marked within the code of routine  HPL_slamc2, ",
"otherwise supply EMIN explicitly." );
      }
/*
 * Assume IEEE arithmetic if we found denormalised  numbers above, or if
 * arithmetic seems to round in the  IEEE style,  determined  in routine
 * HPL_slamc1.  A true  IEEE  machine should have both things true; how-
 * ever, faulty machines may have one or the other.
 */
      if( ( ieee != 0 ) || ( lieee1 != 0 ) ) ieee = 1;
      else                                   ieee = 0;
/*
 * Compute  RMIN by successive division by  BETA. We could compute  RMIN
 * as BASE**( EMIN - 1 ), but some machines underflow during this compu-
 * tation.
 */
      lrmin = HPL_rone;
      for( i = 0; i < 1 - lemin; i++ )
         lrmin = HPL_slamc3( lrmin*rbase, zero );
/*
 * Finally, call HPL_slamc5 to compute emax and rmax.
 */
      HPL_slamc5( lbeta, lt, lemin, ieee, &lemax, &lrmax );
   }
   *BETA = lbeta; *T    = lt;    *RND  = lrnd;  *EPS  = leps;
   *EMIN = lemin; *RMIN = lrmin; *EMAX = lemax; *RMAX = lrmax;
}

#ifdef HPL_STDC_HEADERS
static float HPL_slamc3( const float A, const float B )
#else
static float HPL_slamc3( A, B )
/*
 * .. Scalar Arguments ..
 */
   const float               A, B;
#endif
{
/*
 * Purpose
 * =======
 *
 * HPL_slamc3  is intended to force a and b  to be stored prior to doing
 * the addition of  a  and  b,  for  use  in situations where optimizers
 * might hold one of these in a register.
 *
 * Notes
 * =====
 *
 * This function has been manually translated from the Fortran 77 LAPACK
 * auxiliary function slamc3.f (version 2.0 -- 1992).
 *
 * Arguments
 * =========
 *
 * A, B    (local input)               float
 *         The values a and b.
 *
 * ---------------------------------------------------------------------
 */
/* ..
 * .. Executable Statements ..
 */
   return( A + B );
}

#ifdef HPL_STDC_HEADERS
static void HPL_slamc4
(
   int                        * EMIN,
   const float               START,
   const int                  BASE
)
#else
static void HPL_slamc4( EMIN, START, BASE )
/*
 * .. Scalar Arguments ..
 */
   int                        * EMIN;
   const int                  BASE;
   const float               START;
#endif
{
/*
 * Purpose
 * =======
 *
 * HPL_slamc4 is a service function for HPL_slamc2.
 *
 * Notes
 * =====
 *
 * This function has been manually translated from the Fortran 77 LAPACK
 * auxiliary function slamc4.f (version 2.0 -- 1992).
 *
 * Arguments
 * =========
 *
 * EMIN    (local output)              int *
 *         The minimum exponent before  (gradual) underflow, computed by
 *         setting A = START and dividing  by  BASE until the previous A
 *         can not be recovered.
 *
 * START   (local input)               float
 *         The starting point for determining EMIN.
 *
 * BASE    (local input)               int
 *         The base of the machine.
 *
 * ---------------------------------------------------------------------
 */
/*
 * .. Local Variables ..
 */
   float                     a, b1, b2, c1, c2, d1, d2, one, rbase, zero;
   int                        i;
/* ..
 * .. Executable Statements ..
 */
   a     = START; one = HPL_rone; rbase = one / (float)(BASE);
   zero  = HPL_rzero;
   *EMIN = 1; b1 = HPL_slamc3( a * rbase, zero ); c1 = c2 = d1 = d2 = a;

   do
   {
      (*EMIN)--; a = b1;
      b1 = HPL_slamc3( a /  BASE,  zero );
      c1 = HPL_slamc3( b1 *  BASE, zero );
      d1 = zero; for( i = 0; i < BASE; i++ ) d1 = d1 + b1;
      b2 = HPL_slamc3( a * rbase,  zero );
      c2 = HPL_slamc3( b2 / rbase, zero );
      d2 = zero; for( i = 0; i < BASE; i++ ) d2 = d2 + b2;
   } while( ( c1 == a ) && ( c2 == a ) &&  ( d1 == a ) && ( d2 == a ) );
}

#ifdef HPL_STDC_HEADERS
static void HPL_slamc5
(
   const int                  BETA,
   const int                  P,
   const int                  EMIN,
   const int                  IEEE,
   int                        * EMAX,
   float                     * RMAX
)
#else
static void HPL_slamc5( BETA, P, EMIN, IEEE, EMAX, RMAX )
/*
 * .. Scalar Arguments ..
 */
   const int                  BETA, EMIN, IEEE, P;
   int                        * EMAX;
   float                     * RMAX;
#endif
{
/*
 * Purpose
 * =======
 *
 * HPL_slamc5  attempts  to compute RMAX, the largest machine  floating-
 * point number, without overflow.  It assumes that EMAX + abs(EMIN) sum
 * approximately to a power of 2.  It will fail  on machines where  this
 * assumption does not hold, for example, the  Cyber 205 (EMIN = -28625,
 * EMAX = 28718).  It will also fail if  the value supplied for  EMIN is
 * too large (i.e. too close to zero), probably with overflow.
 *
 * Notes
 * =====
 *
 * This function has been manually translated from the Fortran 77 LAPACK
 * auxiliary function slamc5.f (version 2.0 -- 1992).
 *
 * Arguments
 * =========
 *
 * BETA    (local input)               int
 *         The base of floating-point arithmetic.
 *
 * P       (local input)               int
 *         The number of base BETA digits in the mantissa of a floating-
 *         point value.
 *
 * EMIN    (local input)               int
 *         The minimum exponent before (gradual) underflow.
 *
 * IEEE    (local input)               int
 *         A logical flag specifying whether or not  the arithmetic sys-
 *         tem is thought to comply with the IEEE standard.
 *
 * EMAX    (local output)              int *
 *         The largest exponent before overflow.
 *
 * RMAX    (local output)              float *
 *         The largest machine floating-point number.
 *
 * ---------------------------------------------------------------------
 */
/*
 * .. Local Variables ..
 */
   float                     oldy=HPL_rzero, recbas, y, z;
   int                        exbits=1, expsum, i, lexp=1, nbits, try_,
                              uexp;
/* ..
 * .. Executable Statements ..
 */
/*
 * First compute  lexp  and  uexp, two powers of 2 that bound abs(EMIN).
 * We then assume that  EMAX + abs( EMIN ) will sum approximately to the
 * bound that  is closest to abs( EMIN ). (EMAX  is the  exponent of the
 * required number RMAX).
 */
l_10:
   try_ = (int)( (unsigned int)(lexp) << 1 );
   if( try_ <= ( -EMIN ) ) { lexp = try_; exbits++; goto l_10; }

   if( lexp == -EMIN ) { uexp = lexp; } else { uexp = try_; exbits++; }
/*
 * Now -lexp is less than or equal to EMIN, and -uexp is greater than or
 * equal to EMIN. exbits is the number of bits needed to store the expo-
 * nent.
 */
   if( ( uexp+EMIN ) > ( -lexp-EMIN ) )
   { expsum = (int)( (unsigned int)(lexp) << 1 ); }
   else
   { expsum = (int)( (unsigned int)(uexp) << 1 ); }
/*
 * expsum is the exponent range, approximately equal to EMAX - EMIN + 1.
 */
   *EMAX = expsum + EMIN - 1;
/*
 * nbits  is  the total number of bits needed to store a  floating-point
 * number.
 */
   nbits = 1 + exbits + P;

   if( ( nbits % 2 == 1 ) && ( BETA == 2 ) )
   {
/*
 * Either there are an odd number of bits used to store a floating-point
 * number, which is unlikely, or some bits are not used in the represen-
 * tation of numbers,  which is possible,  (e.g. Cray machines)  or  the
 * mantissa has an implicit bit, (e.g. IEEE machines, Dec Vax machines),
 * which is perhaps the most likely. We have to assume the last alterna-
 * tive.  If this is true,  then we need to reduce  EMAX  by one because
 * there must be some way of representing zero  in an  implicit-bit sys-
 * tem. On machines like Cray we are reducing EMAX by one unnecessarily.
 */
      (*EMAX)--;
   }

   if( IEEE != 0 )
   {
/*
 * Assume we are on an IEEE  machine which reserves one exponent for in-
 * finity and NaN.
 */
      (*EMAX)--;
   }
/*
 * Now create RMAX, the largest machine number, which should be equal to
 * (1.0 - BETA**(-P)) * BETA**EMAX . First compute 1.0-BETA**(-P), being
 * careful that the result is less than 1.0.
 */
   recbas = HPL_rone / (float)(BETA);
   z      = (float)(BETA) - HPL_rone;
   y      = HPL_rzero;

   for( i = 0; i < P; i++ )
   { z *= recbas; if( y < HPL_rone ) oldy = y; y = HPL_slamc3( y, z ); }

   if( y >= HPL_rone ) y = oldy;
/*
 * Now multiply by BETA**EMAX to get RMAX.
 */
   for( i = 0; i < *EMAX; i++ ) y = HPL_slamc3( y * BETA, HPL_rzero );

   *RMAX = y;
/*
 * End of HPL_slamch
 */
}

#ifdef HPL_STDC_HEADERS
static float HPL_sipow
(
   const float               X,
   const int                  N
)
#else
static float HPL_sipow( X, N )
/*
 * .. Scalar Arguments ..
 */
   const int                  N;
   const float               X;
#endif
{
/*
 * Purpose
 * =======
 *
 * HPL_sipow computes the integer n-th power of a real scalar x.
 *
 * Arguments
 * =========
 *
 * X       (local input)               const float
 *         The real scalar x.
 *
 * N       (local input)               const int
 *         The integer power to raise x to.
 *
 * ---------------------------------------------------------------------
 */
/*
 * .. Local Variables ..
 */
   float                     r, y=HPL_rone;
   int                        k, n;
/* ..
 * .. Executable Statements ..
 */
   if( X == HPL_rzero ) return( HPL_rzero );
   if( N < 0 ) { n = -N; r = HPL_rone / X; } else { n = N; r = X; }
   for( k = 0; k < n; k++ ) y *= r;

   return( y );
}
