
/* $Id: Pvmftrecv.c,v 1.2 1998/09/21 20:27:56 pvmsrc Exp $ */

#include <sys/types.h>

#ifdef WIN32
#include "..\..\src\pvmwin.h"
#include <time.h>
#else 
#include <sys/time.h>
#endif
#include "pvm3.h"

#include "pvm_consts.h"

void  
#ifdef IMA_WIN32_WATCOM
pvmftrecv (tid, msgtag, sec, usec, info)
#else
__fortran PVMFTRECV (tid, msgtag, sec, usec, info)
#endif

int *tid, *msgtag, *sec, *usec, *info;
{
   struct timeval t;

   t.tv_sec = *sec;
   t.tv_usec = *usec;
   *info = pvm_trecv(*tid, *msgtag, (*sec == -1 ? (struct timeval *)0 : &t));
}

