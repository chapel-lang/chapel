
/* $Id: pvmfunpack.m4,v 1.4 1999/12/10 21:38:34 pvmsrc Exp $ */

#include "pvm3.h"
#include "pvm_consts.h"

void
FUNCTION(pvmfunpack) ARGS(`what, vp, cnt, std, info')
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

