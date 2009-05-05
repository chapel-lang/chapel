c
c $Id: spmd.f,v 1.4 1997/08/28 20:11:44 pvmsrc Exp $
c
c---------------------------------------- 
c   SPMD Fortran example using PVM 3
c   Illustrates use of new pvm3.4 call pvmfsiblings
c---------------------------------------- 
       program spmd
       include '../include/fpvm3.h'
       PARAMETER( MAXNPROC=32 )

       integer mytid, me, info
       integer tids(0:MAXNPROC -1)
       integer ntids 

c      -------------
c      Enroll in pvm
c      -------------
       call pvmfmytid( mytid )

c      --------------------------------------------
c      Call pvmfsiblings to determine how many tasks were 
c      spawned with me. 
c      --------------------------------------------
       me = -1 
       call pvmfsiblings(ntids, 0, tids(0))
       if (ntids .gt. MAXNPROC) ntids = MAXNPROC
	 
       do i = 0, ntids - 1
           call pvmfsiblings(ntids, i, tids(i))
           if (tids(i) .eq. mytid) me = i
       end do
	

       if (me .eq. -1) then
           call pvmfexit(info)
           stop
       endif
      
       if (me .eq. 0) then
          write (6,*) 'Pass a token through the', ntids, ' tid ring:'
		  write (6,6000) (tids(i), i=0, ntids-1), tids(0)
 6000     format( 6(I7:, ' -> '))
       end if
		
       call dowork( me, ntids )

c      -------------------------
c      program finished exit pvm
c      -------------------------
       call pvmfexit(info)
       stop
       end


       subroutine dowork( me, nproc )
       include '../include/fpvm3.h'
c-------------------------------------------------
c Simple subroutine to pass a token around a ring
c-------------------------------------------------
       integer me, nproc
 
       integer token, src, dest, count, stride, msgtag 
       integer ndum
c      -------------------------------
c      Determine neighbors in the ring
c      -------------------------------
       call pvmfsiblings(ndum, me-1, src )
       call pvmfsiblings(ndum, me+1, dest )
       if( me .eq. 0 ) call pvmfsiblings( ndum, nproc-1, src )
       if( me .eq. nproc - 1 ) call pvmfsiblings( ndum, 0, dest)

       count  = 1
       stride = 1
       msgtag = 4


       if( me .eq. 0 ) then
          token = dest
          call pvmfinitsend( PVMDEFAULT, info )
          call pvmfpack( INTEGER4, token, count, stride, info )
          call pvmfsend( dest, msgtag, info )
          call pvmfrecv( src, msgtag, info )
          print*, 'token ring done'
       else
          call pvmfrecv( src, msgtag, info )
          call pvmfunpack( INTEGER4, token, count, stride, info )
          call pvmfinitsend( PVMDEFAULT, info )
          call pvmfpack( INTEGER4, token, count, stride, info )
          call pvmfsend( dest, msgtag, info )
       endif
      
       return
       end
