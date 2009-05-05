
/* $Id: Pvmfjoingrp.c,v 1.3 1998/11/20 19:48:37 pvmsrc Exp $ */

#ifdef WIN32
#include "..\..\src\pvmwin.h"
#endif 
#include "pvm3.h"
#include "pvm_consts.h"


#ifdef IMA_WIN32_WATCOM
#include "watforstr.h"
pvmfjoingroup (group_str, inum)
WatcomFortranStr* group_str;
int *inum;
{
char* group_ptr = group_str->strP;
int   group_len = group_str->len;
#else

void __fortran
PVMFJOINGROUP (group_ptr,group_len, inum)
char * group_ptr; int group_len;
int *inum;
{
#endif

	char tgroup[MAX_GRP_NAME + 1];

	/*
	 * Copy the group name to make sure there's
	 * a NUL at the end.
	 */
	if (ftocstr(tgroup, sizeof(tgroup), group_ptr, group_len)){
		*inum = PvmBadParam;
		return;
	}

	*inum = pvm_joingroup(tgroup);
}

