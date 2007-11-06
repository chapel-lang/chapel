       program Stream

       integer 		n, nMax
       parameter	(n = 5503189, nMax = 60000000)
       double precision A(nMax), B(nMax), C(nMax)

       integer 		nTrials, ntMax
       parameter	(nTrials = 100, ntMax = 1000)
       double precision Timing(ntMax)

       double precision alpha
       parameter 	(alpha = 3.0)

       double precision Time1
       double precision	getTime
       external 	getTime

       logical 		failure

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
       
       write(6,*) "N       = ", n
       write(6,*) "NTrials = ", nTrials
       write(6,*) "Validation:"
       if (failure) then
         write(6,*) "  FAILURE"
       else
         write(6,*) "  SUCCESS"
       endif
       call printResults(n, nTrials, Timing)

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

       write(6,*) "Execution time:"
       write(6,*) "  tot = ", totalTime
       write(6,*) "  avg = ", avgTime
       write(6,*) "  min = ", minTime
       write(6,*) " "

       if (avgTime .ne. 0.0e1) then
         write(6,*) "GB/s = ", 3.0*8.0*(dble(n)/avgTime)*1.0e-9
       else
         write(6,*) "Average time = 0 seconds. Cannot compute GB/s"
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
