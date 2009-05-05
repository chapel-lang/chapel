
/* $Id: Pvmfmstat.c,v 1.2 1998/09/21 20:26:49 pvmsrc Exp $ */

#ifdef WIN32
#include "..\..\include\pvm3.h"
#include "..\..\src\pvmwin.h"
#else 
#include "pvm3.h"
#endif

#include "pvm_consts.h"

#ifdef IMA_WIN32_WATCOM
#include "watforstr.h"
void pvmfstat (host_str, mstat)
WatcomFortranStr* host_str;
int *mstat;
{
   char* host_ptr = host_str->strP;
   int   host_len = host_str->len;
#else
void __stdcall
PVMFMSTAT (host_ptr,host_len, mstat)
char * host_ptr; int host_len;
int *mstat;
{
#endif

   char thost[MAX_HOST_NAME + 1];

   /*
    * Copy the host name to make sure there's
    * a NUL at the end.
    */
   if (ftocstr(thost, sizeof(thost), host_ptr, host_len)) {
      *mstat = PvmBadParam;
      return;
   }

   *mstat = pvm_mstat(thost);
}

