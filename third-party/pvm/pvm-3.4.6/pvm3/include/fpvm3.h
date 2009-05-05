c
c $Id: fpvm3.h,v 1.15 2000/02/10 23:53:08 pvmsrc Exp $
c

c  -------------------------------------------------------------------
c          PVM version 3.4:  Parallel Virtual Machine System
c                University of Tennessee, Knoxville TN.
c            Oak Ridge National Laboratory, Oak Ridge TN.
c                    Emory University, Atlanta GA.
c       Authors:  J. J. Dongarra, G. E. Fagg, M. Fischer
c           G. A. Geist, J. A. Kohl, R. J. Manchek, P. Mucci,
c          P. M. Papadopoulos, S. L. Scott, and V. S. Sunderam
c                    (C) 1997 All Rights Reserved
c
c                               NOTICE
c
c  Permission to use, copy, modify, and distribute this software and
c  its documentation for any purpose and without fee is hereby granted
c  provided that the above copyright notice appear in all copies and
c  that both the copyright notice and this permission notice appear in
c  supporting documentation.
c
c  Neither the Institutions (Emory University, Oak Ridge National
c  Laboratory, and University of Tennessee) nor the Authors make any
c  representations about the suitability of this software for any
c  purpose.  This software is provided ``as is'' without express or
c  implied warranty.
c
c  PVM version 3 was funded in part by the U.S. Department of Energy,
c  the National Science Foundation and the State of Tennessee.
c  -------------------------------------------------------------------

c     ----------------------------------
c     fpvm3.h
c
c     Definitions to be included with
c     User Fortran application
c     ----------------------------------

      integer PVMTASKDEFAULT, PVMTASKHOST, PVMTASKARCH, PVMTASKDEBUG
      integer PVMTASKTRACE, PVMMPPFRONT, PVMHOSTCOMPL, PVMNOSPAWNPARENT
      integer PVMHOST, PVMARCH, PVMDEBUG, PVMTRACE
      integer PVMDATADEFAULT, PVMDATARAW, PVMDATAINPLACE
      integer PVMDATATRACE
      integer PVMDEFAULT, PVMRAW, PVMINPLACE
      integer PVMTASKEXIT, PVMHOSTDELETE, PVMHOSTADD, PVMROUTEADD
      integer PVMROUTEDELETE, PVMNOTIFYCANCEL
      integer PVMROUTE, PVMDEBUGMASK, PVMAUTOERR
      integer PVMOUTPUTTID, PVMOUTPUTCODE, PVMRESVTIDS
      integer PVMTRACETID, PVMTRACECODE, PVMTRACEBUFFER
      integer PVMTRACEOPTIONS, PVMFRAGSIZE, PVMSHOWTIDS, PVMNORESET
      integer PVMTRACEFULL, PVMTRACETIME, PVMTRACECOUNT
      integer PVMSOUTPUTTID, PVMSOUTPUTCODE, PVMSTRACETID
      integer PVMSTRACECODE, PVMSTRACEBUFFER, PVMSTRACEOPTIONS
      integer PVMOUTPUTCTX, PVMTRACECTX, PVMSOUTPUTCTX, PVMSTRACECTX
      integer PVMDONTROUTE, PVMALLOWDIRECT, PVMROUTEDIRECT
      integer PVMPOLLTYPE, PVMPOLLTIME, PVMPOLLCONSTANT, PVMPOLLSLEEP
      integer PVMMBOXDEFAULT, PVMMBOXPERSISTENT, PVMMBOXMULTIINSTANCE
      integer PVMMBOXOVERWRITABLE, PVMMBOXFIRSTAVAIL
      integer PVMMBOXREADANDDELETE, PVMMBOXWAITFORINFO
      integer STRING, BYTE1, INTEGER2, INTEGER4, INTEGER8
      integer REAL4, COMPLEX8, REAL8, COMPLEX16

      integer PvmOk, PvmSysErr, PvmBadParam, PvmMismatch
      integer PvmNoData, PvmNoHost, PvmNoFile, PvmNoMem
      integer PvmBadMsg, PvmNoBuf, PvmNoSuchBuf
      integer PvmNullGroup, PvmDupGroup, PvmNoGroup
      integer PvmNotInGroup, PvmNoInst, PvmHostFail, PvmNoParent
      integer PvmNotImpl, PvmDSysErr, PvmBadVersion, PvmOutOfRes
      integer PvmDupHost, PvmCantStart, PvmAlready, PvmNoTask
      integer PvmNoEntry, PvmDupEntry, PvmOverflow, PvmDenied
      integer PvmNotFound, PvmExists, PvmHostrNMstr, PvmParentNotSet
      integer PvmIPLoopback

c     --------------------
c     spawn 'flag' options
c     --------------------
      parameter( PVMTASKDEFAULT    =  0)
      parameter( PVMTASKHOST       =  1)
      parameter( PVMTASKARCH       =  2)
      parameter( PVMTASKDEBUG      =  4)
      parameter( PVMTASKTRACE      =  8)
      parameter( PVMMPPFRONT       = 16)
      parameter( PVMHOSTCOMPL      = 32)
      parameter( PVMNOSPAWNPARENT  = 64)

c     --------------------------------
c     old option names still supported
c     --------------------------------
      parameter( PVMHOST  =  1)
      parameter( PVMARCH  =  2)
      parameter( PVMDEBUG =  4)
      parameter( PVMTRACE =  8)

c     -------------------------
c     buffer 'encoding' options
c     -------------------------
      parameter( PVMDATADEFAULT = 0)
      parameter( PVMDATARAW     = 1)
      parameter( PVMDATAINPLACE = 2)
      parameter( PVMDATATRACE   = 4)

c     --------------------------------
c     old option names still supported
c     --------------------------------
      parameter( PVMDEFAULT = 0)
      parameter( PVMRAW     = 1)
      parameter( PVMINPLACE = 2)

c     ----------------------
c     notify 'about' options
c     ----------------------
      parameter( PVMTASKEXIT     = 1 )
      parameter( PVMHOSTDELETE   = 2 )
      parameter( PVMHOSTADD      = 3 )
      parameter( PVMROUTEADD     = 4 )
      parameter( PVMROUTEDELETE  = 5 )
      parameter( PVMNOTIFYCANCEL = 256 )

c     --------------------------------
c     packing/unpacking 'what' options
c     --------------------------------
      parameter( STRING   = 0)
      parameter( BYTE1    = 1)
      parameter( INTEGER2 = 2)
      parameter( INTEGER4 = 3)
      parameter( REAL4    = 4)
      parameter( COMPLEX8 = 5)
      parameter( REAL8    = 6)
      parameter( COMPLEX16= 7)
      parameter( INTEGER8 = 8)

c     --------------------------------
c     setopt/getopt options for 'what'
c     --------------------------------
      parameter( PVMROUTE         = 1)
      parameter( PVMDEBUGMASK     = 2)
      parameter( PVMAUTOERR       = 3)
      parameter( PVMOUTPUTTID     = 4)
      parameter( PVMOUTPUTCODE    = 5)
      parameter( PVMTRACETID      = 6)
      parameter( PVMTRACECODE     = 7)
      parameter( PVMTRACEBUFFER   = 8)
      parameter( PVMTRACEOPTIONS  = 9)
      parameter( PVMFRAGSIZE      = 10)
      parameter( PVMRESVTIDS      = 11)
      parameter( PVMSOUTPUTTID    = 12)
      parameter( PVMSOUTPUTCODE   = 13)
      parameter( PVMSTRACETID     = 14)
      parameter( PVMSTRACECODE    = 15)
      parameter( PVMSTRACEBUFFER  = 16)
      parameter( PVMSTRACEOPTIONS = 17)
      parameter( PVMSHOWTIDS      = 18)
      parameter( PVMPOLLTYPE      = 19)
      parameter( PVMPOLLTIME      = 20)
      parameter( PVMOUTPUTCTX     = 21)
      parameter( PVMTRACECTX      = 22)
      parameter( PVMSOUTPUTCTX    = 23)
      parameter( PVMSTRACECTX     = 24)
      parameter( PVMNORESET       = 25)

c     --------------------------------------------
c     tracing option values for setopt function
c     --------------------------------------------
      parameter( PVMTRACEFULL     = 1)
      parameter( PVMTRACETIME     = 2)
      parameter( PVMTRACECOUNT    = 3)

c     --------------------------------------------
c     poll type options for 'how' in setopt function
c     --------------------------------------------
      parameter( PVMPOLLCONSTANT = 1)
      parameter( PVMPOLLSLEEP    = 2)

c     --------------------------------------------
c     for message mailbox operations
c     --------------------------------------------
      parameter( PVMMBOXDEFAULT        =  0)
      parameter( PVMMBOXPERSISTENT     =  1)
      parameter( PVMMBOXMULTIINSTANCE  =  2)
      parameter( PVMMBOXOVERWRITABLE   =  4)
      parameter( PVMMBOXFIRSTAVAIL     =  8)
      parameter( PVMMBOXREADANDDELETE  = 16)
      parameter( PVMMBOXWAITFORINFO    = 32)

c     --------------------------------------------
c     routing options for 'how' in setopt function
c     --------------------------------------------
      parameter( PVMDONTROUTE  = 1)
      parameter( PVMALLOWDIRECT= 2)
      parameter( PVMROUTEDIRECT= 3)

c     --------------------------
c     error 'info' return values
c     --------------------------
      parameter( PvmOk           =   0)
      parameter( PvmBadParam     =  -2)
      parameter( PvmMismatch     =  -3)
      parameter( PvmOverflow     =  -4)
      parameter( PvmNoData       =  -5)
      parameter( PvmNoHost       =  -6)
      parameter( PvmNoFile       =  -7)
      parameter( PvmDenied       =  -8)
      parameter( PvmNoMem        = -10)
      parameter( PvmBadMsg       = -12)
      parameter( PvmSysErr       = -14)
      parameter( PvmNoBuf        = -15)
      parameter( PvmNoSuchBuf    = -16)
      parameter( PvmNullGroup    = -17)
      parameter( PvmDupGroup     = -18)
      parameter( PvmNoGroup      = -19)
      parameter( PvmNotInGroup   = -20)
      parameter( PvmNoInst       = -21)
      parameter( PvmHostFail     = -22)
      parameter( PvmNoParent     = -23)
      parameter( PvmNotImpl      = -24)
      parameter( PvmDSysErr      = -25)
      parameter( PvmBadVersion   = -26)
      parameter( PvmOutOfRes     = -27)
      parameter( PvmDupHost      = -28)
      parameter( PvmCantStart    = -29)
      parameter( PvmAlready      = -30)
      parameter( PvmNoTask       = -31)
      parameter( PvmNotFound     = -32)
      parameter( PvmExists       = -33)
      parameter( PvmHostrNMstr   = -34)
      parameter( PvmParentNotSet = -35)
      parameter( PvmIPLoopback   = -36)

c     --------------------------
c     these are going away in the next version.
c     use the replacements
c     --------------------------
      parameter( PvmNoEntry    = -32)
      parameter( PvmDupEntry   = -33)


c Uncomment this include for use with the WIN32 WATCOM fortran compiler
c
c     include '../include/fpvm3_watcom.h'
c
