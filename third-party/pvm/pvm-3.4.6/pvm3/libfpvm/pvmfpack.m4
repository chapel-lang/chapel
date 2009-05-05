
/* $Id: pvmfpack.m4,v 1.3 1999/12/10 15:21:46 pvmsrc Exp $ */

#include "pvm3.h"
#include "pvm_consts.h"

void
FUNCTION(pvmfpack) ARGS(`what, vp, cnt, std, info')
	int *what, *cnt, *std, *info;

#ifdef __STDC__
    void *vp;
#else
    char *vp;
#endif
{
	switch( *what )
	{
	case BYTE1:
		*info = pvm_pkbyte((char*)vp, *cnt, *std);
		break;
	case INTEGER2:
		*info = pvm_pkshort((short*)vp, *cnt, *std);
		break;
	case INTEGER4:
		*info = pvm_pkint((int*)vp, *cnt, *std);
		break;
	case INTEGER8:
		*info = pvm_pklong((long*)vp, *cnt, *std);
		break;
	case REAL4:
		*info = pvm_pkfloat((float*)vp, *cnt, *std);
		break;
	case COMPLEX8:
		*info = pvm_pkcplx((float*)vp, *cnt, *std);
		break;
	case REAL8:
		*info = pvm_pkdouble((double*)vp, *cnt, *std);
		break;
	case COMPLEX16:
		*info = pvm_pkdcplx((double*)vp, *cnt, *std);
		break;
	case STRING:
		*info = pvm_pkbyte((char*)vp, *cnt, *std);
		break;
	default:
		*info = PvmBadParam;
		break;
	}
}

