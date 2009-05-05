
/* $Id: pvmfgather.m4,v 1.2 1996/10/04 15:27:04 pvmsrc Exp $ */

#include "pvm3.h"
#include "pvm_consts.h"

void
FUNCTION(pvmfgather) ARGS(`result, data, count, datatype, msgtag, STRING_ARG(gname), rootinst, info')
void *result, *data;
int *count, *datatype, *msgtag, *rootinst, *info;
STRING_ARG_DECL(gname);
{
  char tgroup[MAX_GRP_NAME + 1];

  /*
   * Copy the group name to make sure there's a NUL at the end.
   */

  if (ftocstr(tgroup,sizeof(tgroup),STRING_PTR(gname),STRING_LEN(gname)))
    {
    *info = PvmBadParam;
    return;
    }

  *info = pvm_gather(result, data, *count, *datatype, *msgtag, tgroup, 
                     *rootinst);

}

