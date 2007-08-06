***********************************************************************
*     accumulate operation for the following datatypes:
*            real, double precision, complex, double complex, integer
*
*     WARNING: This file must be compiled WITH optimization under AIX.
*              IBM fortran compilers generate bad code with -g option. 
*
*     Two versions of each routine are provided: 
*         original and unrolled loops.
*
***********************************************************************


*----------------------- original loop versions ----------------------

      subroutine d_accumulate_1d(alpha,  A,  B, rows)
      integer rows, r
      double precision A(*), B(*), alpha
ccdir$ no_cache_alloc a,b
         do r = 1, rows
            A(r) = A(r)+ alpha*B(r)
         enddo
      end

      subroutine d_accumulate_2d(alpha, rows, cols, A, ald, B, bld)
      integer rows, cols
      integer c, r, ald, bld
      double precision A(ald,*), B(bld,*), alpha
ccdir$ no_cache_alloc a,b
      do c = 1, cols
         do r = 1, rows
            A(r,c) = A(r,c)+ alpha*B(r,c)
         enddo
      enddo
      end

      subroutine f_accumulate_1d(alpha,  A,  B, rows)
      integer rows, r
      real A(*), B(*), alpha
         do r = 1, rows
            A(r) = A(r)+ alpha*B(r)
         enddo
      end

      subroutine f_accumulate_2d(alpha, rows, cols, A, ald, B, bld)
      integer rows, cols
      integer c, r, ald, bld
      real A(ald,*), B(bld,*), alpha
      do c = 1, cols
         do r = 1, rows
            A(r,c) = A(r,c)+ alpha*B(r,c)
         enddo
      enddo
      end

      subroutine z_accumulate_1d(alpha,  A,  B, rows)
      integer rows, r
      double complex  A(*), B(*), alpha
         do r = 1, rows
            A(r) = A(r)+ alpha*B(r)
         enddo
      end

      subroutine z_accumulate_2d(alpha, rows, cols, A, ald, B, bld)
      integer rows, cols
      integer c, r, ald, bld
      double complex A(ald,*), B(bld,*), alpha
      do c = 1, cols
         do r = 1, rows
            A(r,c) = A(r,c)+ alpha*B(r,c)
         enddo
      enddo
      end

      subroutine c_accumulate_1d(alpha,  A,  B, rows)
      integer rows, r
      complex  A(*), B(*), alpha
         do r = 1, rows
            A(r) = A(r)+ alpha*B(r)
         enddo
      end

      subroutine c_accumulate_2d(alpha, rows, cols, A, ald, B, bld)
      integer rows, cols
      integer c, r, ald, bld
      complex A(ald,*), B(bld,*), alpha

      do c = 1, cols
         do r = 1, rows
            A(r,c) = A(r,c)+ alpha*B(r,c)
         enddo
      enddo
      end


      subroutine i_accumulate_2d(alpha, rows, cols, A, ald, B, bld)
      integer rows, cols
      integer c, r, ald, bld
      integer A(ald,*), B(bld,*), alpha

      do c = 1, cols
         do r = 1, rows
            A(r,c) = A(r,c)+ alpha*B(r,c)
         enddo
      enddo
      end

      subroutine i_accumulate_1d(alpha,  A,  B, rows)
      integer rows, r
      integer A(*), B(*), alpha
         do r = 1, rows
            A(r) = A(r)+ alpha*B(r)
         enddo
      end

*
*-----------------------------  loops unrolled ----------------------
*
      subroutine d_accumulate_2d_u(alpha, rows, cols, A, ald, B, bld)
      integer rows, cols
      integer c, r, ald, bld
      double precision A(ald,*), B(bld,*), alpha

      integer r1
      doubleprecision d1, d2, d3, d4
      do c = 1, cols
      r1 = iand(max0(rows,0),3)
      do r = 1, r1
         a(r,c) = a(r,c) + alpha*b(r,c)
      end do
      do r = r1 + 1, rows, 4
         d1 = a(r,c) + alpha*b(r,c)
         d2 = a(r+1,c) + alpha*b(r+1,c)
         d3 = a(r+2,c) + alpha*b(r+2,c)
         d4 = a(r+3,c) + alpha*b(r+3,c)
         a(r,c) = d1
         a(r+1,c) = d2
         a(r+2,c) = d3
         a(r+3,c) = d4
      enddo
      enddo
      end

      subroutine f_accumulate_2d_u(alpha, rows, cols, A, ald, B, bld)
      integer rows, cols
      integer c, r, ald, bld
      real A(ald,*), B(bld,*), alpha

      integer r1
      real d1, d2, d3, d4
      do c = 1, cols
      r1 = iand(max0(rows,0),3)
      do r = 1, r1
         a(r,c) = a(r,c) + alpha*b(r,c)
      end do
      do r = r1 + 1, rows, 4
         d1 = a(r,c) + alpha*b(r,c)
         d2 = a(r+1,c) + alpha*b(r+1,c)
         d3 = a(r+2,c) + alpha*b(r+2,c)
         d4 = a(r+3,c) + alpha*b(r+3,c)
         a(r,c) = d1
         a(r+1,c) = d2
         a(r+2,c) = d3
         a(r+3,c) = d4
      enddo
      enddo
      end


      subroutine z_accumulate_2d_u(alpha, rows, cols, A, ald, B, bld)
      integer rows, cols
      integer c, r, ald, bld
      double complex A(ald,*), B(bld,*), alpha

      integer r1
      double complex x1, x2, x3, x4
      do c = 1, cols
      r1 = iand(max0(rows,0),3)
      do r = 1, r1
         a(r,c) = a(r,c) + alpha*b(r,c)
      end do
      do r = r1 + 1, rows, 4
         x1 = a(r,c) + alpha*b(r,c)
         x2 = a(r+1,c) + alpha*b(r+1,c)
         x3 = a(r+2,c) + alpha*b(r+2,c)
         x4 = a(r+3,c) + alpha*b(r+3,c)
         a(r,c) = x1
         a(r+1,c) = x2
         a(r+2,c) = x3
         a(r+3,c) = x4
      enddo
      enddo
      end


      subroutine c_accumulate_2d_u(alpha, rows, cols, A, ald, B, bld)
      integer rows, cols
      integer c, r, ald, bld
      complex A(ald,*), B(bld,*), alpha

      integer r1
      complex x1, x2, x3, x4
      do c = 1, cols
      r1 = iand(max0(rows,0),3)
      do r = 1, r1
         a(r,c) = a(r,c) + alpha*b(r,c)
      end do
      do r = r1 + 1, rows, 4
         x1 = a(r,c) + alpha*b(r,c)
         x2 = a(r+1,c) + alpha*b(r+1,c)
         x3 = a(r+2,c) + alpha*b(r+2,c)
         x4 = a(r+3,c) + alpha*b(r+3,c)
         a(r,c) = x1
         a(r+1,c) = x2
         a(r+2,c) = x3
         a(r+3,c) = x4
      enddo
      enddo
      end


      subroutine i_accumulate_2d_u(alpha, rows, cols, A, ald, B, bld)
      integer rows, cols
      integer c, r, ald, bld
      integer A(ald,*), B(bld,*), alpha

      integer r1, j2, j3, j4, j5
      do c = 1, cols
      r1 = iand(max0(rows,0),3)
      do r = 1, r1
         a(r,c) = a(r,c) + alpha*b(r,c)
      end do
      do r = r1 + 1, rows, 4
         j2 = a(r,c) + alpha*b(r,c)
         j3 = a(r+1,c) + alpha*b(r+1,c)
         j4 = a(r+2,c) + alpha*b(r+2,c)
         j5 = a(r+3,c) + alpha*b(r+3,c)
         a(r,c) = j2
         a(r+1,c) = j3
         a(r+2,c) = j4
         a(r+3,c) = j5
      enddo
      enddo
      end

c---------- operations used in armci gops --------------
c
      subroutine fort_dadd(n, x, work)
      integer n,i
      double precision x(n), work(n)
      do i= 1,n
         x(i) = x(i) + work(i)
      enddo
      end

      subroutine fort_dadd2(n, x, work, work2)
      integer n,i
      double precision x(n), work(n), work2(n)
      do i= 1,n
         x(i) = work(i) + work2(i)
      enddo
      end


      subroutine fort_dmult(n, x, work)
      integer n,i
      double precision x(n), work(n)
      do i= 1,n
         x(i) = x(i) * work(i)
      enddo
      end

      subroutine fort_dmult2(n, x, work,work2)
      integer n,i
      double precision x(n), work(n)
      do i= 1,n
         x(i) = work(i)*work2(i)
      enddo
      end
