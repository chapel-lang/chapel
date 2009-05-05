
/* $Id: Pvmfgettid.c,v 1.2 1998/09/21 20:26:17 pvmsrc Exp $ */

#ifdef WIN32
#include "..\..\include\pvm3.h"
#include "..\..\src\pvmwin.h"
#else 
#include "pvm3.h"
#endif

#include "pvm_consts.h"

#ifdef IMA_WIN32_WATCOM
#include "watforstr.h"
void pvmfgettid (group_str, inum, tid)
WatcomFortranStr* group_str;
int *inum, *tid;
{
   char* group_ptr = group_str->strP;
   int   group_len = group_str->len;
#else
void __stdcall
PVMFGETTID (group_ptr,group_len, inum, tid)
char * group_ptr; int group_len;
int *inum, *tid;
{
#endif
   
   char tgroup[MAX_GRP_NAME + 1];

   /*
    * Copy the group name to make sure there's
    * a NUL at the end.
    */
   if (ftocstr(tgroup, sizeof(tgroup), group_ptr, group_len)){
      *tid = PvmBadParam;
      return;
   }

   *tid = pvm_gettid(tgroup, *inum);
}

