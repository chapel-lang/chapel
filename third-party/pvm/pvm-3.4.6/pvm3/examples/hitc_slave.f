c
c $Id: hitc_slave.f,v 1.1 1996/09/23 20:55:32 pvmsrc Exp $
c
      program hitc_slave
      include '../include/fpvm3.h'
c ------------------------------------------------------
c Slave performs work requested by the master
c ------------------------------------------------------
      parameter(iplmax=3,ipkkr=(iplmax+1)**2,ipbase=5)
      parameter(iprkkr=ipkkr*2,iprmtr=iprkkr*ipbase)
c
      integer  info, mytid, mtid, msgtype
      real*8   random
      complex*16  amt(iprmtr,iprmtr),bmt(iprmtr,iprmtr)

c  Enroll this program in PVM 
      call pvmfmytid( mytid )
c  Get the master's task id
      call pvmfparent( mtid )

c ------- Begin work loop -------- 

   1  continue
c     Receive data from host 
      msgtype  = 1 
      call pvmfrecv( mtid, msgtype, info ) 
      call pvmfunpack( INTEGER4, n, 1, 1, info )

c     Check if problem is over
      if( n .lt. 1 ) goto 9999
 
c     Generate matrix of given size
      do 20 i=1,n
        do 10 j=1,n
          amt(i,j) = random()
          bmt(i,j) = cmplx(0.d0,0.d0)
  10    continue
  20  continue

c     Calculate Matrix Inverse
      call matinv( n, amt, bmt ) 
                    
c     Send result to host 
      call pvmfinitsend( PVMDEFAULT, info )
      call pvmfpack( INTEGER4, mytid, 1, 1, info )
      msgtype  = 2 
      call pvmfsend( mtid, msgtype, info ) 

c     Go To top and await more work.
      goto 1
9999  continue
      print *,mytid,' received no-more-work flag: exiting...'
      call pvmfexit(info) 
      stop
      end

c    ==================================================================
c
      subroutine matinv( nrmatr, amt, bmt )
c
c    ==================================================================
c
      implicit real*8 (a-h,o-z)
c
       parameter(iplmax=3,ipkkr=(iplmax+1)**2,ipbase=5)
       parameter(iprkkr=ipkkr*2,iprmtr=iprkkr*ipbase)
c
      complex*16  amt(iprmtr,iprmtr),bmt(iprmtr,iprmtr)
      complex*16  td(iprmtr),ad(iprmtr),bd(iprmtr)
      complex*16  amtinv,m1
c
c     **************************************************************
c     calculates the matrix inverse needed to determine
c     elements of the tau-matrix.
c
c     Level 2 BLAS version...............................
c     **************************************************************
         m1=(-1.00d+00,0.0d+00)
c
c        general case............
         do 16 i=1,nrmatr
            amtinv=1.0d+00/amt(i,i)
            do 12 j=1,i
               bd(j)=bmt(i,j)
               td(j)=amtinv*amt(j,i)
 12         continue
            ad(i)=amt(i,i)
            td(i)=(0.0d+00,0.0d+00)
            do 13 j=i+1,nrmatr
               ad(j)=amt(i,j)
 13            td(j)=amtinv*amt(j,i)
c
c  level 2  blas
c
        call zgeru(nrmatr,i,m1,td,1,bd,1,bmt,iprmtr)
        call zgeru(nrmatr,nrmatr-i+1,m1,td,1,ad(i),1,amt(1,i),iprmtr)
c
 16      continue
c
      return
      end

************************************************************************
*
      SUBROUTINE ZGERU ( M, N, ALPHA, X, INCX, Y, INCY, A, LDA )
*     .. Scalar Arguments ..
      COMPLEX*16         ALPHA
      INTEGER            INCX, INCY, LDA, M, N
*     .. Array Arguments ..
      COMPLEX*16         A( LDA, * ), X( * ), Y( * )
*     ..
*
*  Purpose
*  =======
*
*  ZGERU  performs the rank 1 operation
*
*     A := alpha*x*y' + A,
*
*  where alpha is a scalar, x is an m element vector, y is an n element
*  vector and A is an m by n matrix.
*
*  Parameters
*  ==========
*
*  M      - INTEGER.
*           On entry, M specifies the number of rows of the matrix A.
*           M must be at least zero.
*           Unchanged on exit.
*
*  N      - INTEGER.
*           On entry, N specifies the number of columns of the matrix A.
*           N must be at least zero.
*           Unchanged on exit.
*
*  ALPHA  - COMPLEX*16      .
*           On entry, ALPHA specifies the scalar alpha.
*           Unchanged on exit.
*
*  X      - COMPLEX*16       array of dimension at least
*           ( 1 + ( m - 1 )*abs( INCX ) ).
*           Before entry, the incremented array X must contain the m
*           element vector x.
*           Unchanged on exit.
*
*  INCX   - INTEGER.
*           On entry, INCX specifies the increment for the elements of
*           X. INCX must not be zero.
*           Unchanged on exit.
*
*  Y      - COMPLEX*16       array of dimension at least
*           ( 1 + ( n - 1 )*abs( INCY ) ).
*           Before entry, the incremented array Y must contain the n
*           element vector y.
*           Unchanged on exit.
*
*  INCY   - INTEGER.
*           On entry, INCY specifies the increment for the elements of
*           Y. INCY must not be zero.
*           Unchanged on exit.
*
*  A      - COMPLEX*16       array of DIMENSION ( LDA, n ).
*           Before entry, the leading m by n part of the array A must
*           contain the matrix of coefficients. On exit, A is
*           overwritten by the updated matrix.
*
*  LDA    - INTEGER.
*           On entry, LDA specifies the first dimension of A as declared
*           in the calling (sub) program. LDA must be at least
*           max( 1, m ).
*           Unchanged on exit.
*
*
*  Level 2 Blas routine.
*
*  -- Written on 22-October-1986.
*     Jack Dongarra, Argonne National Lab.
*     Jeremy Du Croz, Nag Central Office.
*     Sven Hammarling, Nag Central Office.
*     Richard Hanson, Sandia National Labs.
*
*
*     .. Parameters ..
      COMPLEX*16         ZERO
      PARAMETER        ( ZERO = ( 0.0D+0, 0.0D+0 ) )
*     .. Local Scalars ..
      COMPLEX*16         TEMP
      INTEGER            I, INFO, IX, J, JY, KX
*     .. External Subroutines ..
      EXTERNAL           XERBLA
*     .. Intrinsic Functions ..
      INTRINSIC          MAX
*     ..
*     .. Executable Statements ..
*
*     Test the input parameters.
*
      INFO = 0
      IF     ( M.LT.0 )THEN
         INFO = 1
      ELSE IF( N.LT.0 )THEN
         INFO = 2
      ELSE IF( INCX.EQ.0 )THEN
         INFO = 5
      ELSE IF( INCY.EQ.0 )THEN
         INFO = 7
      ELSE IF( LDA.LT.MAX( 1, M ) )THEN
         INFO = 9
      END IF
      IF( INFO.NE.0 )THEN
         CALL XERBLA( 'ZGERU ', INFO )
         RETURN
      END IF
*
*     Quick return if possible.
*
      IF( ( M.EQ.0 ).OR.( N.EQ.0 ).OR.( ALPHA.EQ.ZERO ) )
     $   RETURN
*
*     Start the operations. In this version the elements of A are
*     accessed sequentially with one pass through A.
*
      IF( INCY.GT.0 )THEN
         JY = 1
      ELSE
         JY = 1 - ( N - 1 )*INCY
      END IF
      IF( INCX.EQ.1 )THEN
         DO 20, J = 1, N
            IF( Y( JY ).NE.ZERO )THEN
               TEMP = ALPHA*Y( JY )
               DO 10, I = 1, M
                  A( I, J ) = A( I, J ) + X( I )*TEMP
   10          CONTINUE
            END IF
            JY = JY + INCY
   20    CONTINUE
      ELSE
         IF( INCX.GT.0 )THEN
            KX = 1
         ELSE
            KX = 1 - ( M - 1 )*INCX
         END IF
         DO 40, J = 1, N
            IF( Y( JY ).NE.ZERO )THEN
               TEMP = ALPHA*Y( JY )
               IX   = KX
               DO 30, I = 1, M
                  A( I, J ) = A( I, J ) + X( IX )*TEMP
                  IX        = IX        + INCX
   30          CONTINUE
            END IF
            JY = JY + INCY
   40    CONTINUE
      END IF
*
      RETURN
*
*     End of ZGERU .
*
      END
      SUBROUTINE XERBLA ( SRNAME, INFO )
*     ..    Scalar Arguments ..
      INTEGER            INFO
      CHARACTER*6        SRNAME
*     ..
*
*  Purpose
*  =======
*
*  XERBLA  is an error handler for the Level 2 BLAS routines.
*
*  It is called by the Level 2 BLAS routines if an input parameter is
*  invalid.
*
*  Installers should consider modifying the STOP statement in order to
*  call system-specific exception-handling facilities.
*
*  Parameters
*  ==========
*
*  SRNAME - CHARACTER*6.
*           On entry, SRNAME specifies the name of the routine which
*           called XERBLA.
*
*  INFO   - INTEGER.
*           On entry, INFO specifies the position of the invalid
*           parameter in the parameter-list of the calling routine.
*
*
*  Auxiliary routine for Level 2 Blas.
*
*  Written on 20-July-1986.
*
*     .. Executable Statements ..
*
      WRITE (*,99999) SRNAME, INFO
*
      STOP
*
99999 FORMAT ( ' ** On entry to ', A6, ' parameter number ', I2,
     $         ' had an illegal value' )
*
*     End of XERBLA.
*
      END

c-----------------------------------------------------------------------------
      double precision function random()

c-----------------------------------------------------
c  Routine returns a pseudo-random number between 0-1. 
c-----------------------------------------------------
      integer m, i, md, seed
      double precision fmd

      data m/25173/,i/13849/,md/65536/,fmd/65536.d0/,seed/17/

      save seed

      seed   = mod(m*seed+i,md)
      random = seed/fmd
      return
      end

