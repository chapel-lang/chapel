
/* $Id: Pvmfnotify.c,v 1.2 1998/09/21 20:26:54 pvmsrc Exp $ */

#ifdef WIN32
#include "..\..\include\pvm3.h"
#include "..\..\src\pvmwin.h"
#else 
#include "pvm3.h"
#endif

#include "pvm_consts.h"

void 
#ifdef IMA_WIN32_WATCOM
pvmfnotify (about, msgtag, ntid, tids, info)
#else
__fortran PVMFNOTIFY (about, msgtag, ntid, tids, info)
#endif

int *about, *ntid, *tids, *msgtag, *info;
{
	*info = pvm_notify(*about, *msgtag, *ntid, tids);
}

