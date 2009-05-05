
/* $Id: Pvmfbarrier.c,v 1.2 1998/09/21 20:25:32 pvmsrc Exp $ */

#ifdef WIN32
#include "..\..\src\pvmwin.h"
#endif
#include "pvm3.h"
#include "pvm_consts.h"

#ifdef IMA_WIN32_WATCOM
#include "watforstr.h"
void
pvmfbarrier (group_str, count, info)
WatcomFortranStr* group_str;
int *count, *info;
{
   char* group_ptr = group_str->strP;
   int   group_len = group_str->len;
#else
void __stdcall
PVMFBARRIER (group_ptr,group_len, count, info)
char * group_ptr; int group_len;
int *count, *info;
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

   *info = pvm_barrier(tgroup, *count);
}

