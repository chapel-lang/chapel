c
c $Id: slave1.f,v 1.1 1996/09/23 20:55:35 pvmsrc Exp $
c
      program slave1 
      include '../include/fpvm3.h'
c ------------------------------------------------------
c Example fortran program illustrating use of PVM 3
c ------------------------------------------------------
      integer  info, mytid, mtid, msgtype, me
      integer  tids(0:32)
      double precision result, data(100)
      double precision work

c  Enroll this program in PVM 
      call pvmfmytid( mytid )
c  Get the master's task id
      call pvmfparent( mtid )

c ------- Begin user program -------- 

c     Receive data from host 
      msgtype  = 1 
      call pvmfrecv( mtid, msgtype, info ) 
      call pvmfunpack( INTEGER4, nproc, 1, 1, info )
      call pvmfunpack( INTEGER4, tids, nproc, 1, info )
      call pvmfunpack( INTEGER4, n, 1, 1, info )
      call pvmfunpack( REAL8,    data, n, 1, info ) 
 
c     Determine which slave I am (0 -- nproc-1)
      do 5 i=0, nproc
         if( tids(i) .eq. mytid ) me = i
  5   continue
      
c     Do calculations with data
      result = work( me, n, data, tids, nproc ) 
                    
c     Send result to host 
      call pvmfinitsend( PVMDEFAULT, info )
      call pvmfpack( INTEGER4, me, 1, 1, info )
      call pvmfpack( REAL8,    result, 1, 1, info )
      msgtype  = 2 
      call pvmfsend( mtid, msgtype, info ) 

c --------- End user program -------- 

c     Program finished. Leave PVM before exiting 
      call pvmfexit(info) 
      stop
      end

      double precision function work( me, n, data, tids, nproc )
      include '../include/fpvm3.h'
c    --------------------------------------
c     Just a simple routine for illustration
c    --------------------------------------
      double precision data(*), sum, psum
      integer i, n, me, inum
      integer tids(0:*)

      sum = 0.0
      do 10 i=1,n
         sum = sum + me * data(i)
 10   continue
c     ----------------------------------------
c     Pass partial result to neighboring node
c     to illustrate node-to-node communication
c     ----------------------------------------
      call pvmfinitsend( PVMDEFAULT, info )
      call pvmfpack( REAL8, sum, 1, 1, info)
      inum = me+1
      if( inum .eq. nproc ) inum = 0
      call pvmfsend( tids(inum), 77, info )
      call pvmfrecv(  -1, 77, info )
      call pvmfunpack( REAL8, psum, 1, 1, info)

      work = sum + psum
      return
      end
