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

#ifndef HPL_dscal

#ifdef HPL_STDC_HEADERS
void HPL_dscal
(
   const int                        N,
   const double                     ALPHA,
   double *                         X,
   const int                        INCX
)
#else
void HPL_dscal
( N, ALPHA, X, INCX )
   const int                        N;
   const double                     ALPHA;
   double *                         X;
   const int                        INCX;
#endif
{
/* 
 * Purpose
 * =======
 *
 * HPL_dscal scales the vector x by alpha.
 * 
 *
 * Arguments
 * =========
 *
 * N       (local input)                 const int
 *         On entry, N specifies the length of the vector x. N  must  be
 *         at least zero.
 *
 * ALPHA   (local input)                 const double
 *         On entry, ALPHA specifies the scalar alpha.   When  ALPHA  is
 *         supplied as zero, then the entries of the incremented array X
 *         need not be set on input.
 *
 * X       (local input/output)          double *
 *         On entry,  X  is an incremented array of dimension  at  least
 *         ( 1 + ( n - 1 ) * abs( INCX ) )  that  contains the vector x.
 *         On exit, the entries of the incremented array  X  are  scaled
 *         by the scalar alpha.
 *
 * INCX    (local input)                 const int
 *         On entry, INCX specifies the increment for the elements of X.
 *         INCX must not be zero.
 *
 * ---------------------------------------------------------------------
 */ 
#ifdef HPL_CALL_CBLAS
   cblas_dscal( N, ALPHA, X, INCX );
#endif
#ifdef HPL_CALL_VSIPL
   register double           x0, x1, x2, x3, x4, x5, x6, x7;
   register const double     alpha = ALPHA;
   const double              * StX;
   register int              i;
   int                       nu;
   const int                 incX2 = 2 * INCX, incX3 = 3 * INCX,
                             incX4 = 4 * INCX, incX5 = 5 * INCX,
                             incX6 = 6 * INCX, incX7 = 7 * INCX,
                             incX8 = 8 * INCX;

   if( ( N > 0 ) && ( alpha != HPL_rone ) )
   {
      if( alpha == HPL_rzero )
      {
         if( ( nu = ( N >> 3 ) << 3 ) != 0 )
         {
            StX = (double *)X + nu * INCX;
 
            do
            {
               (*X)     = HPL_rzero; X[incX4] = HPL_rzero;
               X[INCX ] = HPL_rzero; X[incX5] = HPL_rzero;
               X[incX2] = HPL_rzero; X[incX6] = HPL_rzero;
               X[incX3] = HPL_rzero; X[incX7] = HPL_rzero; X += incX8;

            } while( X != StX );
         }
 
         for( i = N - nu; i != 0; i-- ) { *X = HPL_rzero; X += INCX; }
      }
      else
      {
         if( ( nu = ( N >> 3 ) << 3 ) != 0 )
         {
            StX = X + nu * INCX;
 
            do
            {
               x0 = (*X);     x4 = X[incX4]; x1 = X[INCX ]; x5 = X[incX5];
               x2 = X[incX2]; x6 = X[incX6]; x3 = X[incX3]; x7 = X[incX7];
 
               x0 *= alpha;   x4 *= alpha;   x1 *= alpha;   x5 *= alpha;
               x2 *= alpha;   x6 *= alpha;   x3 *= alpha;   x7 *= alpha;
 
               (*X)     = x0; X[incX4] = x4; X[INCX ] = x1; X[incX5] = x5;
               X[incX2] = x2; X[incX6] = x6; X[incX3] = x3; X[incX7] = x7;
 
               X  += incX8;
 
            } while( X != StX );
         }
 
         for( i = N - nu; i != 0; i-- )
         { x0 = (*X); x0 *= alpha; *X = x0; X += INCX; }
      }
   }
#endif
#ifdef HPL_CALL_FBLAS
   double                    alpha = ALPHA;
#ifdef HPL_USE_F77_INTEGER_DEF
   const F77_INTEGER         F77N = N, F77incx = INCX;
#else
#define F77N                 N
#define F77incx              INCX
#endif

   F77dscal( &F77N, &alpha, X, &F77incx );
#endif
/*
 * End of HPL_dscal
 */
}
 
#endif
