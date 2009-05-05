
/* $Id: Pvmfdelhost.c,v 1.2 1998/09/21 20:25:40 pvmsrc Exp $ */

#ifdef WIN32
#include "..\..\include\pvm3.h"
#include "..\..\src\pvmwin.h"
#else 
#include "pvm3.h"
#endif

#include "pvm_consts.h"

#ifdef IMA_WIN32_WATCOM
#include "watforstr.h"
#endif
#ifdef IMA_WIN32_WATCOM
void 
pvmfdelhost (host_str,info)
WatcomFortranStr* host_str;
int *info;
{
   char* host_ptr = host_str->strP;
   int   host_len = host_str->len;
#else
void  __fortran
PVMDELHOST (host_ptr,host_len, info)
char * host_ptr; int host_len;
int *info;
{
#endif
   
   char thost[MAX_HOST_NAME + 1];
   char *ptr = thost;
   int cc;

   /*
    * Copy the host name to make sure there's
    * a NUL at the end.
    */
   if (ftocstr(thost, sizeof(thost), host_ptr, host_len)) {
      *info = PvmBadParam;
      return;
   }

   *info = pvm_delhosts(&ptr, 1, &cc);
   if (*info >= 0)
      *info = cc;
}

