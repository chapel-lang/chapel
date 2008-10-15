/*
 * Include files
 */
#include "hpl.h"

#ifndef HPL_daxpy

#ifdef HPL_STDC_HEADERS
void HPL_daxpy
(
   const int                        N,
   const double                     ALPHA,
   const double *                   X,
   const int                        INCX,
   double *                         Y,
   const int                        INCY
)
#else
void HPL_daxpy
( N, ALPHA, X, INCX, Y, INCY )
   const int                        N;
   const double                     ALPHA;
   const double *                   X;
   const int                        INCX;
   double *                         Y;
   const int                        INCY;
#endif
{
/* 
 * Purpose
 * =======
 *
 * HPL_daxpy scales the vector x by alpha and adds it to y.
 * 
 *
 * Arguments
 * =========
 *
 * N       (local input)                 const int
 *         On entry, N specifies the length of the vectors  x  and  y. N
 *         must be at least zero.
 *
 * ALPHA   (local input)                 const double
 *         On entry, ALPHA specifies the scalar alpha.   When  ALPHA  is
 *         supplied as zero, then the entries of the incremented array X
 *         need not be set on input.
 *
 * X       (local input)                 const double *
 *         On entry,  X  is an incremented array of dimension  at  least
 *         ( 1 + ( n - 1 ) * abs( INCX ) )  that  contains the vector x.
 *
 * INCX    (local input)                 const int
 *         On entry, INCX specifies the increment for the elements of X.
 *         INCX must not be zero.
 *
 * Y       (local input/output)          double *
 *         On entry,  Y  is an incremented array of dimension  at  least
 *         ( 1 + ( n - 1 ) * abs( INCY ) )  that  contains the vector y.
 *         On exit, the entries of the incremented array  Y  are updated
 *         with the scaled entries of the incremented array X.
 *
 * INCY    (local input)                 const int
 *         On entry, INCY specifies the increment for the elements of Y.
 *         INCY must not be zero.
 *
 * ---------------------------------------------------------------------
 */ 
#ifdef HPL_CALL_CBLAS
   cblas_daxpy( N, ALPHA, X, INCX, Y, INCY );
#endif
#ifdef HPL_CALL_VSIPL
   register const double     alpha = ALPHA;
   register double           x0, x1, x2, x3, y0, y1, y2, y3;
   const double              * StX;
   register int              i;
   int                       nu;
   const int                 incX2 = 2 * INCX, incY2 = 2 * INCY,
                             incX3 = 3 * INCX, incY3 = 3 * INCY,
                             incX4 = 4 * INCX, incY4 = 4 * INCY;

   if( ( N > 0 ) && ( alpha != HPL_rzero ) )
   {
      if( ( nu = ( N >> 2 ) << 2 ) != 0 )
      {
         StX = X + nu * INCX;
 
         do
         {
            x0 = (*X);     y0 = (*Y);     x1 = X[INCX ]; y1 = Y[INCY ];
            x2 = X[incX2]; y2 = Y[incY2]; x3 = X[incX3]; y3 = Y[incY3];
 
            *Y       = y0 + alpha * x0; Y[INCY ] = y1 + alpha * x1;
            Y[incY2] = y2 + alpha * x2; Y[incY3] = y3 + alpha * x3;
 
            X  += incX4;
            Y  += incY4;
 
         } while( X != StX );
      }
 
      for( i = N - nu; i != 0; i-- )
      {
         x0  = (*X);
         y0  = (*Y);
 
         *Y  = y0 + alpha * x0;
 
         X  += INCX;
         Y  += INCY;
      }
   }
#endif
#ifdef HPL_CALL_FBLAS
   double                    alpha = ALPHA;
#ifdef HPL_USE_F77_INTEGER_DEF
   const F77_INTEGER         F77N = N, F77incx = INCX, F77incy = INCY;
#else
#define F77N                 N
#define F77incx              INCX
#define F77incy              INCY
#endif
   F77daxpy( &F77N, &alpha, X, &F77incx, Y, &F77incy );
#endif
/*
 * End of HPL_daxpy
 */
}
 
#endif
