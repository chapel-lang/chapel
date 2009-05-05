
/* $Id: Pvmfdelmhf.c,v 1.2 1998/09/21 20:25:51 pvmsrc Exp $ */

#ifdef WIN32
#include "..\..\src\pvmwin.h"
#endif
#include "pvm3.h"
#include "pvm_consts.h"

void 
#ifdef IMA_WIN32_WATCOM
pvmfdelmhf (mhid, info)
#else
__fortran PVMFDELMHF (mhid, info)
#endif

int *mhid, *info;
{
	*info = pvm_delmhf(*mhid);
}

