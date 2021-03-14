       program Stream

       integer 		n
c      parameter	(n = 174458581)
       integer 		nTrials
c      parameter	(nTrials = 10)

       double precision, dimension(:), allocatable:: A, B, C
       double precision, dimension(:), allocatable:: Timing

       double precision alpha
       parameter 	(alpha = 3.0)

       double precision Time1
       double precision	getTime
       external 	getTime

       logical 		failure

       call getInputData(n, nTrials, alpha)

       allocate (A(n))
       allocate (B(n))
       allocate (C(n))
       allocate (Timing(nTrials))

       do 5 i = 1, n
          A(i) = 1.0
          B(i) = 2.0
          C(i) = 0.0
    5  continue
       do 20 j = 1, nTrials
          Time1 = getTime()
         do 10 i = 1, n
           A(i) = B(i) + alpha*C(i) 
   10    continue
          Timing(j) = getTime() - Time1
   20  continue   

       call validateResults(n, A, B, C, alpha, failure)
       
       write(*,*) "N       = ", n
       write(*,*) "NTrials = ", nTrials
       write(*,*) "Validation:"
       if (failure) then
         write(*,*) "  FAILURE"
       else
         write(*,*) "  SUCCESS"
       endif
       call printResults(n, nTrials, Timing)

       end

       subroutine getInputData(n, nTrials)
       integer 		n, nTrials

       write(*, *) "Enter size of benchmark"
       read(*,*) n
       write(*,*) "Enter number of trials"
       read(*,*) nTrials
       
       return
       end
       subroutine validateResults(n, A, B, C, alpha, failure)

       integer 		n
       double precision	A(n), B(n), C(n), alpha
       logical		failure

       double precision infNorm, eps
       parameter 	(eps = 1.0e-16)

       infNorm = 0.0
       do 10 i = 1, n
         infNorm = max(infNorm, dabs(A(i) - (B(i) + alpha*C(i))))
   10  continue
       if (infNorm .gt. eps) then
         failure = .true.
       else
         failure = .false.
       endif  

       return
       end

       subroutine printResults(n, nTrials, Timing)

       integer		n, nTrials
       double precision	Timing(nTrials)
       double precision totalTime, avgTime, minTime
    
       totalTime = 0.0
       minTime = Timing(1) 
       do 10 i = 1, nTrials
         totalTime = totalTime + Timing(i)
         minTime = min(minTime, Timing(i))
   10  continue
       avgTime = totalTime/real(nTrials)

       write(*,*) "Execution time:"
       write(*,*) "  tot = ", totalTime
       write(*,*) "  avg = ", avgTime
       write(*,*) "  min = ", minTime
       write(*,*) " "

       if (avgTime .ne. 0.0e1) then
         write(*,*) "GB/s = ", 3.0*8.0*(dble(n)/avgTime)*1.0e-9
       else
         write(*,*) "Average time = 0 seconds. Cannot compute GB/s"
       endif
       return
       end
       
       double precision function getTime()
c      This routine calls a C timing routine, curtime, which is
c      defined in timers.c.  This is the same timer that is used
c      to time the C version (which is from the Chapel runtime).
         external curtime
         double precision timeVal
  
         call curtime(timeVal)
         getTime = timeVal
       return
       end
