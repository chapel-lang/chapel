
/* $Id: Pvmfbufinfo.c,v 1.2 1998/09/21 20:25:35 pvmsrc Exp $ */

#ifdef WIN32
#include "..\..\include\pvm3.h"
#include "..\..\src\pvmwin.h"
#else 
#include "pvm3.h"
#endif

#include "pvm_consts.h"

#ifdef IMA_WIN32_WATCOM
void 
pvmfbufinfo(mid, len, type, tid, info)
#else
__stdcall PVMFBUFINFO (mid, len, type, tid, info)
#endif
int *mid, *len, *type, *tid, *info;
{
	*info = pvm_bufinfo(*mid, len, type, tid);
}

