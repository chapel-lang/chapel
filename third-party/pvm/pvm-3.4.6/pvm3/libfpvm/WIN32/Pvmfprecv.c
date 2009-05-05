
/* $Id: Pvmfprecv.c,v 1.2 1998/09/21 20:27:09 pvmsrc Exp $ */

#ifdef WIN32
#include "..\..\include\pvm3.h"
#include "..\..\src\pvmwin.h"
#else 
#include "pvm3.h"
#endif

#include "pvm_consts.h"

void
#ifdef IMA_WIN32_WATCOM
pvmfprecv (tid, msgtag, buf,buf_len, len, dt, atid, atag, alen, info)
#else
__fortran PVMFPRECV (tid, msgtag, buf,buf_len, len, dt, atid, atag, alen, info)
#endif

   int *tid, *msgtag, *len, *dt, *atid, *atag, *alen, *info;
   char *buf;int buf_len;
{
   *info = pvm_precv(*tid, *msgtag, buf, *len, *dt, atid, atag, alen);
}

