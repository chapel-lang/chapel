
/* $Id: Pvmfsetopt.c,v 1.2 1998/09/21 20:27:37 pvmsrc Exp $ */

#ifdef WIN32
#include "..\..\include\pvm3.h"
#include "..\..\src\pvmwin.h"
#else 
#include "pvm3.h"
#endif

#include "pvm_consts.h"

void 
#ifdef IMA_WIN32_WATCOM
pvmfsetopt (what, val, info)
#else
__fortran PVMFSETOPT (what, val, info)
#endif

int *what, *val, *info;
{
   *info = pvm_setopt(*what, *val);
}

