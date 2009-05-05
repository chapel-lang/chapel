c
c $Id: fpvm3_watcom.h,v 1.2 1999/03/15 19:25:55 pvmsrc Exp $
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
c     fpvm3_watcom.h
c
c
c	For the WIN32 WATCOM fortran compiler...
c
c	To activate throughout your code - uncomment this line
c	of code in the ../pvm3.4/include/fpvm3.h file
c
c		include '../include/fpvm3_watcom.h'
c
c     ----------------------------------

*$pragma aux pvmfmytid "!_"
*$pragma aux pvmfaddhost "!_"
*$pragma aux pvmfaddmhf "!_"
*$pragma aux pvmfarchcode "!_"
*$pragma aux pvmfbarrier "!_"
*$pragma aux pvmfbcast "!_"
*$pragma aux pvmfbufinfo "!_"
*$pragma aux pvmfcatchout "!_"
*$pragma aux pvmfconfig "!_"
*$pragma aux pvmfdelhost "!_"
*$pragma aux pvmfdelinfo "!_"
*$pragma aux pvmfdelmhf "!_"
*$pragma aux pvmfgather "!_"
*$pragma aux pvmffreebuf "!_"
*$pragma aux pvmfgetcontext "!_"
*$pragma aux pvmffreecontext "!_"
*$pragma aux pvmffreezegroup "!_"
*$pragma aux pvmfgetmboxinfo "!_"
*$pragma aux pvmfgetinst "!_"
*$pragma aux pvmfgetopt "!_"
*$pragma aux pvmfgetrbuf "!_"
*$pragma aux pvmfgetsbuf "!_"
*$pragma aux pvmfgettid "!_"
*$pragma aux pvmfgsize "!_"
*$pragma aux pvmfhalt "!_"
*$pragma aux pvmfhostsync "!_"
*$pragma aux pvmfinitsend "!_"
*$pragma aux pvmfjoingroup "!_"
*$pragma aux pvmfkill "!_"
*$pragma aux pvmflvgroup "!_"
*$pragma aux pvmfmcast "!_"
*$pragma aux pvmfmkbuf "!_"
*$pragma aux pvmfstat "!_"
*$pragma aux pvmfmytid "!_"
*$pragma aux pvmfnewcontext "!_"
*$pragma aux pvmfnotify "!_"
*$pragma aux pvmfnrecv "!_"
*$pragma aux pvmfpack "!_"
*$pragma aux pvmfperror "!_"
*$pragma aux pvmfparent "!_"
*$pragma aux pvmfprecv "!_"
*$pragma aux pvmfprobe "!_"
*$pragma aux pvmfpsend "!_"
*$pragma aux pvmfpstat "!_"
*$pragma aux pvmfputinfo "!_"
*$pragma aux pvmfrecv "!_"
*$pragma aux pvmfexit "!_"
*$pragma aux pvmfrecvinfo "!_"
*$pragma aux pvmfreduce "!_"
*$pragma aux pvmfscatter "!_"
*$pragma aux pvmfsend "!_"
*$pragma aux pvmfsendsig "!_"
*$pragma aux pvmfsetopt "!_"
*$pragma aux pvmfsetcontext "!_"
*$pragma aux pvmfsetrbuf "!_"
*$pragma aux pvmfsetsbuf "!_"
*$pragma aux pvmfsiblings "!_"
*$pragma aux pvmfsleep "!_"
*$pragma aux pvmfspawn "!_"
*$pragma aux pvmfstartpvmd "!_"
*$pragma aux pvmftasks "!_"
*$pragma aux pvmftrecv "!_"
*$pragma aux pvmfunpack "!_"
