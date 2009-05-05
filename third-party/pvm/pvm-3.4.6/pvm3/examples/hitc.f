c
c $Id: hitc.f,v 1.2 1998/10/30 16:40:24 pvmsrc Exp $
c
      program hitc_master
      include '../include/fpvm3.h'
c ---------------------------------------------------------
c HiTc master program. Gets user input. spawns tasks.
c Uses pool-of-tasks dynamic load balancing
c ---------------------------------------------------------
      integer i, info, nhost, narch, msgtype, n, negr
      integer mytid, tids(0:32)
      integer who, idummy
      double precision random
      character*12 hostname
      character*8 arch

c ------------ Starting up all the tasks ---------------------------

c     Enroll this program in PVM 
      call pvmfmytid( mytid )

c     Find out how many hosts are in virtual machine
c     Start one hitc_slave task on each one.
      call pvmfconfig( nhost, narch, idummy, hostname, arch,
     >                 idummy, info )
c
      print *, nhost,' hosts detected in configuration'
      call pvmfspawn( 'hitc_slave', PVMDEFAULT, '*', nhost, tids, info )
      if( info .lt. nhost ) then
        print *,'Error in spawn. info=', info
        do 100 i=0, nhost-1
           print *,'tid',i,tids(i)
100     continue
      endif

c     Set Problem size relative to virtual machine size
c     Number of energies per task
      negr = 10
c     Initial problem size
      n = 140

c     Broadcast data to all node programs 
      call pvmfinitsend( PVMDEFAULT, info )
      call pvmfpack( INTEGER4, n, 1, 1, info )
      msgtype  = 1 
      call pvmfmcast( nhost, tids, msgtype, info )

c     Size of the job pool
      numleft = nhost*negr-nhost 

c     wait for results from nodes 
      do 30 i=1,nhost*negr
         msgtype  = 2 
         call pvmfrecv( -1, msgtype, info ) 
         call pvmfunpack( INTEGER4, who, 1, 1, info )
         print *,'I got result from tid:',who
c        Send more work to this idle task
         if( numleft .gt. 0 ) then
           n = 130 + 10*random()
         else
           n = -1
         endif
         call pvmfinitsend( PVMDEFAULT, info )
         call pvmfpack( INTEGER4, n, 1, 1, info )
         msgtype  = 1
         call pvmfsend( who, msgtype, info )
         print *,'sent ',who,' task of size:',n
         numleft = numleft - 1
 30   continue 

c     program finished leave PVM before exiting 
      print *,'Program Finished'
      call pvmfexit(info) 
      stop
      end

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
