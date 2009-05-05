
/* $Id: Pvmfscatter.c,v 1.2 1998/09/21 20:27:23 pvmsrc Exp $ */

#ifdef WIN32
#include "..\..\include\pvm3.h"
#include "..\..\src\pvmwin.h"
#else 
#include "pvm3.h"
#endif

#include "pvm_consts.h"

#ifdef IMA_WIN32_WATCOM
#include "watforstr.h"
void pvmfscatter (result, data, count, datatype, msgtag, gname_str, rootinst, info)
void *result, *data;
int *count, *datatype, *msgtag, *rootinst, *info;
WatcomFortranStr* gname_str;
{
   char* gname_ptr = gname_str->strP;
   int   gname_len = gname_str->len;
#else
void __fortran
PVMFSCATTER (result, data, count, datatype, msgtag, gname_ptr,gname_len, rootinst, info)
void *result, *data;
int *count, *datatype, *msgtag, *rootinst, *info;
char * gname_ptr; int gname_len;
{
#endif
  char tgroup[MAX_GRP_NAME + 1];

  /*
   * Copy the group name to make sure there's a NUL at the end.
   */

  if (ftocstr(tgroup,sizeof(tgroup),gname_ptr,gname_len))
    {
    *info = PvmBadParam;
    return;
    }

  *info = pvm_scatter(result, data, *count, *datatype, *msgtag, tgroup, 
                      *rootinst);

}

