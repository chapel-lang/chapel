
/* $Id: Pvmfsleep.c,v 1.1 1999/03/15 19:03:19 pvmsrc Exp $ */

#ifdef WIN32
#include "..\..\include\pvm3.h"
#include "..\..\src\pvmwin.h"
#else 
#include "pvm3.h"
#endif

#include "pvm_consts.h"

void 
#ifdef IMA_WIN32_WATCOM
pvmfsleep (secs)
#else
__fortran PVMFSLEEP (secs)
#endif
int *secs;
{
	pvmsleep(*secs);
}

