
/* $Id: Pvmfinitsend.c,v 1.2 1998/09/21 20:26:24 pvmsrc Exp $ */

#ifdef WIN32
#include "..\..\include\pvm3.h"
#include "..\..\src\pvmwin.h"
#else 
#include "pvm3.h"
#endif

#include "pvm_consts.h"

void
#ifdef IMA_WIN32_WATCOM
pvmfinitsend (tid, info)
#else
__fortran PVMFINITSEND (tid, info)
#endif

int *tid, *info;
{
	*info = pvm_initsend(*tid);
}

