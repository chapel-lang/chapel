
/* $Id: Pvmfspawn.c,v 1.2 1998/09/21 20:27:44 pvmsrc Exp $ */

#ifdef WIN32
#include "..\..\include\pvm3.h"
#include "..\..\src\pvmwin.h"
#else 
#include "pvm3.h"
#endif

#include "pvm_consts.h"

#ifdef IMA_WIN32_WATCOM
  #include "watforstr.h"
void pvmfspawn (WatcomFortranStr *aout_str, int *flag, WatcomFortranStr *where_str, int *count, int *tids, int *info)
{
   char* aout_ptr  = aout_str->strP;
   int   aout_len  = aout_str->len;
   char* where_ptr = where_str->strP;
   int   where_len = where_str->len;
#else
void __stdcall
PVMFSPAWN (char *aout_ptr,int aout_len, int *flag, char *where_ptr,int where_len, int *count, int *tids, int *info)
{
#endif

   char taout[MAX_PROC_NAME + 1];
   char twhere[MAX_HOST_NAME + 1];
   /*
    * Copy the program and where names to make sure there's
    * a NUL at the end.
    */
   
   if (ftocstr(taout, sizeof(taout), aout_ptr, aout_len)) {
      *info = PvmBadParam;
      return;
   }
   
   /*
    * if the where parameter is '*', pass a NULL pointer to
    * initiate which will allow execution on any machine.
    */

   if (*(where_ptr) == '*') {
      *info = pvm_spawn(taout, (char**)0, *flag, "", *count, tids);

   } else {
      if (ftocstr(twhere, sizeof(twhere), where_ptr, where_len)) {
         *info = PvmBadParam;
         return;
      }
   
      *info = pvm_spawn(taout, (char**)0, *flag, twhere, *count, tids);

   }
}

