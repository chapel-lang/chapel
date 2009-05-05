
/* $Id: Pvmfunpack.c,v 1.3 1999/12/10 15:22:14 pvmsrc Exp $ */

#ifdef WIN32
#include "..\..\src\pvmwin.h"
#include <time.h>
#endif 
#include "pvm3.h"
#include "pvm_consts.h"



void
#ifdef IMA_WIN32_WATCOM
pvmfunpack (what, vp, cnt, std, info)
#else
__fortran PVMFUNPACK (what, vp, cnt, std, info)
#endif
   int *what, *cnt, *std, *info;

#ifdef __STDC__
    void *vp;
#else
    char *vp;
#endif
{
   switch( *what ) {

   case BYTE1:
      *info = pvm_upkbyte((char*)vp, *cnt, *std);
      break;
   case INTEGER2:
      *info = pvm_upkshort((short*)vp, *cnt, *std);
      break;
   case INTEGER4:
      *info = pvm_upkint((int*)vp, *cnt, *std);
      break;
   case INTEGER8:
      *info = pvm_upklong((long*)vp, *cnt, *std);
      break;
   case REAL4:
      *info = pvm_upkfloat((float*)vp, *cnt, *std);
      break;
   case COMPLEX8:
      *info = pvm_upkcplx((float*)vp, *cnt, *std);
      break;
   case REAL8:
      *info = pvm_upkdouble((double*)vp, *cnt, *std);
      break;
   case COMPLEX16:
      *info = pvm_upkdcplx((double*)vp, *cnt, *std);
      break;
   case STRING:
      *info = pvm_upkbyte((char*)vp, *cnt, *std);
      break;
   default:
      *info = PvmBadParam;
      break;
   }
}

