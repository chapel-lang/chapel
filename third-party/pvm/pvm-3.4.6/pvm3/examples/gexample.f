C
C $Id: gexample.f,v 1.3 1997/08/04 15:38:55 pvmsrc Exp $
C
C Example of some group function and reduction functions in PVM 
C SPMD style program
C
C 11 March 1994 - Creation by P. Papadopoulos (phil@msr.epm.ornl.gov)
C
C
C
      program gexample

      implicit none
      include '../include/fpvm3.h'

C --- Set Initial and Default Parameters
      integer    DEF_DIMENSION, INITTAG,SUMTAG,PRODTAG
      parameter (DEF_DIMENSION = 100       )
      parameter (INITTAG       = 1000     )
      parameter (SUMTAG        = INITTAG+1)
      parameter (PRODTAG       = INITTAG+2)

      integer mytid, myinst, nproc, maxmax, root
      integer dimension, ninst, bufid
      integer tids(32)
      integer nsibs
      integer nhost, narch, dtid, speed
      integer blksize, nextra, mysrow, i, j, itemp, info
      integer subblock(DEF_DIMENSION,DEF_DIMENSION)
      integer colsum(DEF_DIMENSION)
      real*8  colprod(DEF_DIMENSION)
      character*32 host, arch
      logical spmd
	

C --- External declarations of PVM and User defined reduce function
      external PvmSum 
      external calcprod 

C ---------------- Begin Program -----------------------------------------
      spmd = .false.

c     Enroll in PVM and join a group
      call pvmfmytid( mytid )

C	Try to determine if we were spawned spmd-style 

      call pvmfsiblings(nsibs, 0, tids(1))
      if (nsibs .gt. 1) spmd = .true.
	
      call pvmfjoingroup( 'matrix', myinst )
      if( myinst .lt. 0 ) then
        call pvmfperror( 'joingroup: ', info)
        call pvmfexit( info )
        stop
      endif

c     Set matrix size and number of tasks.
      call pvmfconfig( nhost, narch, dtid, host, arch, speed, info )
      nproc = 2*nhost
      if( nproc .gt. 32 ) nproc = 32
      dimension = DEF_DIMENSION

      if( myinst .eq. 0 ) then              
        print* 
        print*, 'This program demonstrates some group and reduction'
        print*, 'operations in PVM.  The output displays the' 
        print*, 'the product of the first column of a 100x100 Toeplitz'
        print*, 'matrix and the matrix 1-norm. The matrix data is'
        print*, 'distributed among several tasks.  The Toeplitz'
        print*, 'matrix is symmetric with the first row being the'
        print*, 'row vector [1 2 ... n].'
        print* 

c       Start up more copies of myself
        if(nproc  .gt.  1 .and. .not.spmd)  then

          print*, 'There are ',nhost, ' machines in the configuration'
          print*, 'Starting ',nproc - 1, ' tasks'

          call pvmfspawn( 'fgexample', PVMDEFAULT, '*', 
     >                    nproc -1, tids, ninst ) 
          if( ninst .lt. nproc-1 ) then
            print*, 'Trouble in spawn. Check tids'
            print*, tids
            call pvmflvgroup( 'matrix', info )
            call pvmfexit( info )
          endif  
        endif  

        if ( spmd ) nproc = nsibs
        print*, ' --> using ', nproc, ' processors <--'
        print*

      endif  


c     Wait till everyone has joined the group and freeze it  
      call pvmffreezegroup( 'matrix', nproc, info )

c     Broadcast input data to all members
      if( myinst .eq. 0 ) then              
        call pvmfinitsend( PVMDEFAULT, bufid )
        call pvmfpack(INTEGER4, nproc, 1, 1, info) 
        call pvmfpack(INTEGER4, dimension, 1, 1, info )
        call pvmfbcast( 'matrix', INITTAG ,info ) 
      else
        call pvmfrecv( -1, INITTAG, info )
        call pvmfunpack( INTEGER4, nproc, 1, 1 ,info)
        call pvmfunpack( INTEGER4, dimension, 1, 1, info)
      endif  

C     Map matrix rows to processors --       
      blksize =  dimension/nproc 
      nextra =   mod(dimension, nproc) 
      if( myinst .lt.  nextra ) then 
         mysrow = 1 + (blksize + 1) * myinst  
         blksize = blksize + 1 
      else
         mysrow = 1+ (blksize + 1)*(nextra) + blksize*(myinst - nextra)
      endif 
      if( mysrow .gt. dimension)  then  
        blksize = 0
      endif 
    
C     Assign data to this subblock.  The entries below make the entire matrix
C     a symmetric Toeplitz matrix (i.e. diagonals are of constant value)  
      do j=1, dimension
        do i=1, blksize
          subblock(i,j) = abs(mysrow + i - j) 
        end do
      end do

C     Locally compute the sum of each column and put into colsum  
      do j=1, dimension
        colsum(j) = 0
        colprod(j) = 1.0
      end do 
      do j=1, dimension
        do i=1,blksize
          itemp =  abs ( subblock(i,j) )
          colsum(j) = colsum(j) + itemp
          colprod(j) = colprod(j) * itemp
        end do
      end do

C     Get global sum by calling reduce using PvmSum 
      root = 0
      call pvmfreduce( PvmSum, colsum, dimension, INTEGER4, SUMTAG,
     >                 'matrix', root, info) 

c     Here is example of supplying a user-defined OP to reduce
      call pvmfreduce( calcprod, colprod, dimension, REAL8, PRODTAG,
     >                 'matrix', root, info) 

c     Root prints out result
      if( myinst .eq. root ) then
        maxmax = 0
        do j=1,dimension
          maxmax = max(colsum(j),maxmax)
        end do
        write(6,*) ' The 1-Norm is ', maxmax 
        write(6,1000)  dimension
        write(6,*) ' The product of column 1 is', colprod(1)
        write(6,1001) dimension
      endif  
1000  format(' (Should be the sum of the first ', I3, ' integers)')
1001  format(' (Should be ', I3, ' factorial)')

c     Problem done. Be sure all members have finished computation before exit.
      call pvmfbarrier( 'matrix', nproc, info)
      call pvmflvgroup( 'matrix', info)
      call pvmfexit( info )
      stop
      end

c----------------------------------------------------------------------------
C *** Example of a User-defined Reduction Function ***/

      subroutine calcprod( datatype, x, y, num, info )
      integer datatype
      real*8 x(num), y(num)
      integer num, info
  
      integer i
      do i=1,num
         x(i) = x(i) * y(i)
      end do
      return
      end
