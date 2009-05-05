
/* $Id: Pvmffreebuf.c,v 1.2 1998/09/21 20:25:54 pvmsrc Exp $ */

#ifdef WIN32
#include "..\..\include\pvm3.h"
#include "..\..\src\pvmwin.h"
#else 
#include "pvm3.h"
#endif

#include "pvm_consts.h"

void 
#ifdef IMA_WIN32_WATCOM
pvmffreebuf (mbuf, info)
#else
__fortran PVMFFREEBUF (mbuf, info)
#endif
int *mbuf, *info;
{
	*info = pvm_freebuf(*mbuf);
}

