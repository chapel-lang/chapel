
/* $Id: Pvmfpsend.c,v 1.2 1998/09/21 20:27:13 pvmsrc Exp $ */

#ifdef WIN32
#include "..\..\include\pvm3.h"
#include "..\..\src\pvmwin.h"
#else 
#include "pvm3.h"
#endif

#include "pvm_consts.h"

void 
#ifdef IMA_WIN32_WATCOM
pvmfpsend (tid, msgtag, buf,buf_len, len, dt, info)
#else
__fortran PVMFPSEND (tid, msgtag, buf,buf_len, len, dt, info)
#endif

   int *tid, *msgtag, *len, *dt, *info;
   char *buf;int buf_len;
{
   *info = pvm_psend(*tid, *msgtag, buf, *len, *dt);
}

