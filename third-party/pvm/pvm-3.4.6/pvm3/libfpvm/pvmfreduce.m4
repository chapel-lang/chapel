
/* $Id: pvmfreduce.m4,v 1.4 1997/07/23 14:36:48 pvmsrc Exp $ */

#include "pvm3.h"
#include "pvm_consts.h"

void
FUNCTION(pvmfreduce) ARGS(`func, data,count, datatype, msgtag, STRING_ARG(gname), rootinst, info')
#if defined(IMA_UXPM) || defined(IMA_UXPV)
void (**func)(); /* correction for Fujitsu FORTRAN77 EX */
#else
void (*func)();
#endif
void *data;
int *count, *datatype, *msgtag, *rootinst, *info;
STRING_ARG_DECL(gname);
{
  char tgroup[MAX_GRP_NAME + 1];

  /*
   * Copy the group name to make sure there's a NUL at the end.
   */
  if (ftocstr(tgroup, sizeof(tgroup), STRING_PTR(gname), STRING_LEN(gname))){
    *info = PvmBadParam;
    return;
    }

#if defined(IMA_UXPM) || defined(IMA_UXPV)
  *info = pvm_reduce(*func, data, *count, *datatype, *msgtag, tgroup, 
                     *rootinst);
#else
  *info = pvm_reduce(func, data, *count, *datatype, *msgtag, tgroup, 
                     *rootinst);
#endif

}

/* various reduction functions are defined here */

void
FUNCTION(pvmmax) ARGS(`datatype, data, work, count, info')
int *datatype;
void *data, *work;
int *count, *info;
{
  PvmMax(datatype, data, work, count, info);
}



void
FUNCTION(pvmmin) ARGS(`datatype, data, work, count, info')
int *datatype;
void *data, *work;
int *count, *info;
{
  PvmMin(datatype, data, work, count, info);
}

void
FUNCTION(pvmsum) ARGS(`datatype, data, work, count, info')
int *datatype;
void *data, *work;
int *count, *info;
{
  PvmSum(datatype, data, work, count, info);
}

void
FUNCTION(pvmproduct) ARGS(`datatype, data, work, count, info')
int *datatype;
void *data, *work;
int *count, *info;
{
  PvmProduct(datatype, data, work, count, info);
}
