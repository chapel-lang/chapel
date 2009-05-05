
/* $Id: Pvmfgetrbuf.c,v 1.2 1998/09/21 20:26:14 pvmsrc Exp $ */

#ifdef WIN32
#include "..\..\include\pvm3.h"
#include "..\..\src\pvmwin.h"
#else 
#include "pvm3.h"
#endif

#include "pvm_consts.h"

void 
#ifdef IMA_WIN32_WATCOM
pvmfgetrbuf (mbuf)
#else
__fortran PVMFGETRBUF (mbuf)
#endif
int *mbuf;
{
	*mbuf = pvm_getrbuf();
}

