
/* $Id: Pvmfgetsbuf.c,v 1.2 1998/09/21 20:26:15 pvmsrc Exp $ */

#ifdef WIN32
#include "..\..\include\pvm3.h"
#include "..\..\src\pvmwin.h"
#else 
#include "pvm3.h"
#endif

#include "pvm_consts.h"

void 
#ifdef IMA_WIN32_WATCOM
pvmfgetsbuf (mbuf)
#else
__fortran PVMFGETSBUF (mbuf)
#endif

int *mbuf;
{
	*mbuf = pvm_getsbuf();
}

