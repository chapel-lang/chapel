c
c $Id: master1.f,v 1.2 1997/06/26 19:42:55 pvmsrc Exp $
c
      program master1 
      include '../include/fpvm3.h'
c ---------------------------------------------------------
c Example fortran program illustrating the use of PVM 3
c ---------------------------------------------------------
      integer i, info, nproc, nhost, msgtype
      integer mytid, iptid, dtid, tids(0:32)
      integer who, speed
      double precision result(32), data(100)
      character*18 nodename, host
      character*8 arch

c ------------ Starting up all the tasks ---------------------------

c     Enroll this program in PVM 
      call pvmfmytid( mytid )

c     Set number of slaves to spawn. 
c     Note standard input cannot be read if spawned from console, so just
c     derive nprocs from the VM config
      call pvmfconfig( nhost, narch, dtid, host, arch, speed, info )
      nproc = nhost * 3
      if( nproc .gt. 32 ) nproc = 32
c
c     Initiate nproc instances of slave1 program 
c     If arch is set to '*' then ANY configured machine is acceptable
      write(6,6000) nproc
6000  format(' ','Spawning ', I4, ' tasks ...')
6001  format(' ',25X, 'SUCCESSFUL')
      nodename = 'fslave1'
      arch = '*'
      call pvmfspawn( nodename, PVMDEFAULT, arch, nproc, tids, numt )

c     Check for problems
100   continue
      if( numt .lt. nproc ) then
         print *, 'trouble spawning ',nodename
         print *, ' Check tids for error code'
         call shutdown( numt, tids )
      endif

      write(6,6001)

c ------- Begin user program -------- 

      n = 100
c     Initiate data array
      do 20 i=1,n
         data(i) = 1
 20   continue

c     broadcast data to all node programs 
      call pvmfinitsend( PVMDEFAULT, info )
      call pvmfpack( INTEGER4, nproc, 1, 1, info )
      call pvmfpack( INTEGER4, tids, nproc, 1, info )
      call pvmfpack( INTEGER4, n, 1, 1, info )
      call pvmfpack( REAL8,    data, n, 1, info ) 
      msgtype  = 1 
      call pvmfmcast( nproc, tids, msgtype, info )
 
c     wait for results from nodes 
      msgtype  = 2 
      do 30 i=1,nproc 
         call pvmfrecv( -1, msgtype, info ) 
         call pvmfunpack( INTEGER4, who, 1, 1, info )
         call pvmfunpack( REAL8, result(who+1), 1, 1, info )
	     if (who .eq. 0) then
            write(6,6002)  result(who+1), who, (nproc - 1) * 100.0
         else
			write(6,6002) result(who+1), who,  (2 * who - 1) * 100.0
         endif
 6002    format(' ', 'I got ', F15.7, ' from', I4,
     +                ' (expected ',F15.7,' )')
	
 30   continue 

c --------- End user program -------- 

c     program finished leave PVM before exiting 
      call pvmfexit(info) 
      stop
      end

      subroutine shutdown( nproc, tids )
      integer nproc, tids(*)
c
c     Kill all tasks I spawned and then myself
c
      do 10 i=0, nproc
         call pvmfkill( tids(i), info )
  10  continue
      call pvmfexit( info )
	  stop
      return
      end
