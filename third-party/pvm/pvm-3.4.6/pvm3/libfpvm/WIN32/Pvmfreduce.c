
/* $Id: Pvmfreduce.c,v 1.3 1999/03/15 21:44:14 pvmsrc Exp $ */

#ifdef WIN32
#include "..\..\include\pvm3.h"
#include "..\..\src\pvmwin.h"
#else 
#include "pvm3.h"
#endif

#include "pvm_consts.h"

#ifdef IMA_WIN32_WATCOM
#include "watforstr.h"
void pvmfreduce (func, data,count, datatype, msgtag, gname_str, rootinst, info)
void (*func)();
void *data;
int *count, *datatype, *msgtag, *rootinst, *info;
WatcomFortranStr* gname_str;
{
   char* gname_ptr = gname_str->strP;
   int   gname_len = gname_str->len;
#else
void __stdcall
PVMFREDUCE (func, data,count, datatype, msgtag, gname_ptr,gname_len, rootinst, info)
#ifdef UXPM
void (**func)(); /* correction for Fujitsu FORTRAN77 EX */
#else
void (*func)();
#endif
void *data;
int *count, *datatype, *msgtag, *rootinst, *info;
char * gname_ptr; int gname_len;
{
#endif
   
  char tgroup[MAX_GRP_NAME + 1];

  /*
   * Copy the group name to make sure there's a NUL at the end.
   */
  if (ftocstr(tgroup, sizeof(tgroup), gname_ptr, gname_len)){
    *info = PvmBadParam;
    return;
    }

#ifdef UXPM
  *info = pvm_reduce(*func, data, *count, *datatype, *msgtag, tgroup, 
                     *rootinst);
#else
  *info = pvm_reduce(func, data, *count, *datatype, *msgtag, tgroup, 
                     *rootinst);
#endif

}

/* various reduction functions are defined here */

void
#ifdef IMA_WIN32_WATCOM
pvmmax (datatype, data, work, count, info)
#else
__fortran PVMMAX (datatype, data, work, count, info)
#endif
int *datatype;
void *data, *work;
int *count, *info;
{
  PvmMax(datatype, data, work, count, info);
}



void
#ifdef IMA_WIN32_WATCOM
pvmmin (datatype, data, work, count, info)
#else
__fortran PVMMIN (datatype, data, work, count, info)
#endif
int *datatype;
void *data, *work;
int *count, *info;
{
  PvmMin(datatype, data, work, count, info);
}

void
#ifdef IMA_WIN32_WATCOM
pvmsum (datatype, data, work, count, info)
#else
__fortran PVMSUM (datatype, data, work, count, info)
#endif
int *datatype;
void *data, *work;
int *count, *info;
{
  PvmSum(datatype, data, work, count, info);
}

void
#ifdef IMA_WIN32_WATCOM
pvmproduct (datatype, data, work, count, info)
#else
__fortran PVMPRODUCT (datatype, data, work, count, info)
#endif
int *datatype;
void *data, *work;
int *count, *info;
{
  PvmProduct(datatype, data, work, count, info);
}

