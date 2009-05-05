
/* $Id: Pvmfmkbuf.c,v 1.2 1998/09/21 20:26:47 pvmsrc Exp $ */

#ifdef WIN32
#include "..\..\include\pvm3.h"
#include "..\..\src\pvmwin.h"
#else 
#include "pvm3.h"
#endif

#include "pvm_consts.h"

void 
#ifdef IMA_WIN32_WATCOM
pvmfmkbuf (encoding, mbuf)
#else
__fortran PVMFMKBUF (encoding, mbuf)
#endif
int *encoding, *mbuf;
{
	*mbuf = pvm_mkbuf(*encoding);
}

