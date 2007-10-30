       program Stream

       integer 		n, nMax
       parameter	(n = 1000000, nMax = 1000000)
       double precision A(nMax), B(nMax), C(nMax)

       integer 		nTrials, ntMax
       parameter	(nTrials = 20, ntMax = 1000)
       double precision Timing(ntMax)

       double precision alpha
       parameter 	(alpha = 3.0)

       real 		TArray(2), Tresult, Time1
       real 		etime
       external		etime

       logical 		failure

       do 5 i = 1, n
          A(i) = 1.0
          B(i) = 2.0
          C(i) = 0.0
    5  continue
       do 20 j = 1, nTrials
         Tresult = etime(TArray)
         Time1 = TArray(1)
         do 10 i = 1, n
           A(i) = B(i) + alpha*C(i) 
   10    continue
         Tresult = etime(TArray)
         Timing(j) = TArray(1) - Time1
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

       end

       subroutine printResults(n, nTrials, Timing)

       integer		n, nTrials
       real		Timing(nTrials)
       real		totalTime, avgTime, minTime
    
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

       if (avgTime .gt. 0.0) then
         write(6,*) "GB/s = ", 3.0*8.0*(dble(n)/avgTime)
       else
         write(6,*) "Average time = 0 seconds. Cannot compute GB/s"
       endif
       end
       
       
