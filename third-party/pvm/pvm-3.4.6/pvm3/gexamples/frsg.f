c
c $Id: frsg.f,v 1.3 1999/03/15 19:04:45 pvmsrc Exp $
c
c*----------------------------------------------------------------------
c* Example of group Reduce, Scatter, and Gather functions - J.M. Donato
c* 
c* This example calculates the sum of squares of the first N integers
c* in three different ways where 
c*
c*   N = (number of processors)*(number of elements per row)
c*
c* Note:  This is obviously not an efficient way to compute the 
c*        sum of squares, but it is a cutesy example and test case.
c*----------------------------------------------------------------------

      program main

      implicit none
      include '../include/fpvm3.h'

      external PvmMin
      external PvmMax
      external PvmSum
      external PvmProduct
      external MaxWithLoc

      integer MAXNDATA, MAXNPROCS, DFLTNDATA, DFLTNPROCS
      character*10 GROUP, TASK_NAME
      parameter (MAXNDATA = 20, MAXNPROCS = 16, 
     &           DFLTNDATA = 5, DFLTNPROCS = 4,
     &           GROUP = 'grp_frsg', TASK_NAME = 'frsg')

      integer myginst, i, j, gsize, count, nprocs, msgtag, datatype,
     &  buffer, info, info_product, info_user,
     &  tids(MAXNPROCS), myrow(MAXNDATA), matrix(MAXNDATA*MAXNPROCS),
     &  midpoint, bigN, Sum1, Sum2, SumSquares, rootginst,
     &  PSum, PartSums(MAXNPROCS), dupls(MAXNDATA)

      double precision values(2)

      data Sum1/0/, Sum2/0/

c*  join the group 
      call PvmfJoinGroup(GROUP, myginst)       
c* I am the first group member, get input, start up copies of myself 
      if ( myginst.eq.0 ) then 

        call PvmfMyTid(tids(1))
        call PvmfParent(i)
        if (i .eq. PvmNoParent) then
            print *, '/ * Example: PVM Reduce, Scatter, and Gather * ',
     &           '/ Number of processors to use (1-', MAXNPROCS,')? : '
            read *, nprocs
            if (nprocs.gt.MAXNPROCS) nprocs = MAXNPROCS
                 print *, ' Number of elements per row to use (1-', 
     &             MAXNDATA, ')? : '
             read *, count
            if (count.gt.MAXNDATA) count = MAXNDATA
        else
            nprocs = DFLTNPROCS
            count = DFLTNDATA
        endif
        print *, ' INPUT values: nprocs = ', nprocs, ', count = ', count

        if ( nprocs.gt.1 ) then 
          call PvmfSpawn(TASK_NAME, PvmDefault, '*', 
     &                   nprocs-1, tids(2), info)

c*  wait until they have all started, then send input values 
          call PvmfGsize(GROUP, gsize)
9000      continue
          if ( gsize.lt.nprocs) then
            call PvmfSleep(1) 
            call PvmfGsize(GROUP, gsize)
            go to 9000
          end if

          call PvmfInitsend(PvmDataDefault, buffer)
          call PvmfPack(INTEGER4, nprocs, 1, 1, info)
          call PvmfPack(INTEGER4, count,  1, 1, info)
          msgtag = 17
          call PvmfBcast(GROUP, msgtag, info)
        end if

      else
c*  receive the input values if child node
        msgtag = 17
        call PvmfRecv(-1, msgtag, info)
        call PvmfUnpack(INTEGER4, nprocs, 1, 1, info)
        call PvmfUnpack(INTEGER4, count,  1, 1, info)
      end if
  
c*  determine the group root 
      rootginst = 0

c*  init the matrix values on the root processor 
      if ( myginst.eq.rootginst ) then
        do j=1, nprocs
          do i=1, count
            matrix((j-1)*count + i) = (j-1)*count + i
          end do
        end do
      end if

c*  scatter rows of matrix to each processor 
      msgtag = 19
      call PvmfScatter(myrow, matrix, count, INTEGER4, msgtag, 
     &                 GROUP, rootginst, info)

c*  this should end up squaring each value on each processor 
      do i=1, count
        dupls(i) = myginst*count + i 
      end do
      datatype = INTEGER4
      call PvmProduct(datatype, myrow, dupls, count, info_product)
      if ((myginst.eq.rootginst).and.(info_product.lt.0))
     &    print *, ' ERROR: ', info_product, ' on PvmProduct call '

c*  do partial sum on each proc 
      do i=1, count
        PSum = PSum + myrow(i)
      end do

c*  gather partial sums to the rootginst 
      msgtag = 21
      call PvmfGather(PartSums, PSum, 1, INTEGER4, msgtag, 
     &                GROUP, rootginst, info)

c*  do a global sum over myrow, the result goes to rootginst 
      msgtag = 23
      call PvmfReduce(PvmSum, myrow, count, INTEGER4, msgtag, 
     &                GROUP, rootginst, info)

c*  init values and include location information on each processor 
      midpoint = nprocs/2
      values(1) = -(myginst - midpoint)*(myginst-midpoint) + count
      values(2) = myginst    

c*  use a user-defined function in reduce, send answer to rootginst 
      msgtag = 25
      call PvmfReduce(MaxWithLoc, values, 2, REAL8, 
     &                msgtag, GROUP, rootginst, info_user)

      bigN = nprocs*count
      if (myginst.eq.rootginst) then
c*  Complete the Sum of Squares using different methods 
        do i=1, nprocs
          Sum1 = Sum1 + PartSums(i) 
        end do
        do i=1, count 
          Sum2 = Sum2 + myrow(i)    
        end do
        SumSquares = bigN*(bigN+1)*(2*bigN+1)/6
        if ( (Sum1.eq.SumSquares) .and. (Sum2.eq.SumSquares)) then
          print *, '/ Test OK: Sum of Squares of first ', bigN, 
     &             ' integers is ', Sum1
        else
          print *, '/ ERROR: The Sum of Squares of the first ', bigN, 
     &             ' integers / was calculated by Sum1 as ', Sum1,
     &             ' and by Sum2 as ', Sum2,
     &             ' for both it should have been ', SumSquares
        end if

        if (info_user.lt.0) 
     &    print *, ' ERROR: ', info_user, ' on User Reduce Function'

        if ((values(1).ne.count) .or. (values(2).ne.midpoint)) then
          print *, ' ERROR: Should have (', count, ', ', midpoint,
     &             '), but have (', values(1), ',', values(2)
        else
          print *, ' Test Ok: Received (', values(1), ',', 
     &              values(2), ') '
        end if

      end if
       

c*  sync up again, leave group, exit pvm 
      call PvmfBarrier(GROUP, nprocs, info)   
      call PvmfLvgroup(GROUP, info)
      call PvmfExit(info)

      stop
      end


c*
c* This function returns the elementwise maximum of two vectors 
c* along with location information.
c*
c* The first num/2 values of x and y are the data values to compare.
c* The second num/2 values of x and y are location information
c* which is kept with the maximum value determined.
c*
c* In the case of a tie in data values, the smaller location 
c* is kept to insure the associativity of the operation.
c*

      subroutine MaxWithLoc(datatype, x, y, num, info)
      implicit none
      integer datatype, num, info
      double precision x(*), y(*)

      include '../include/fpvm3.h'

      integer i, count
      count = num/2

      if (datatype.ne.REAL8) then
        info = PvmBadParam 
        return
      end if

      do i=1, count
        if (y(i).gt.x(i)) then
          x(i)       = y(i)
          x(i+count) = y(i+count)
        elseif (y(i).eq.x(i)) then
          x(i+count) = min(x(i+count), y(i+count))
        end if
      end do

      info = PvmOk
      return
      end


