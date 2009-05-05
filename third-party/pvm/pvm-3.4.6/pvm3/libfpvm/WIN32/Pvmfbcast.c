
/* $Id: Pvmfbcast.c,v 1.2 1998/09/21 20:25:34 pvmsrc Exp $ */

#ifdef WIN32
#include "..\..\include\pvm3.h"
#include "..\..\src\pvmwin.h"
#else 
#include "pvm3.h"
#endif

#include "pvm_consts.h"

#ifdef IMA_WIN32_WATCOM
#include "watforstr.h"
void
pvmfbcast (group_str, msgtag, info)
WatcomFortranStr* group_str;
int *msgtag, *info;
{
   char* group_ptr = group_str->strP;
   int   group_len = group_str->len;
#else
void __stdcall
PVMFBCAST (group_ptr,group_len, msgtag, info)
char * group_ptr; int group_len;
int *msgtag, *info;
{
#endif

   char tgroup[MAX_GRP_NAME + 1];

   /*
    * Copy the group name to make sure there's
    * a NUL at the end.
    */
   if (ftocstr(tgroup, sizeof(tgroup), group_ptr, group_len)){
      *info = PvmBadParam;
      return;
   }

   *info = pvm_bcast(tgroup, *msgtag);
}

