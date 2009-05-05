
/* $Id: Pvmfpack.c,v 1.3 1999/12/10 15:22:14 pvmsrc Exp $ */

#ifdef WIN32
#include "..\..\include\pvm3.h"
#include "..\..\src\pvmwin.h"
#else 
#include "pvm3.h"
#endif

#include "pvm_consts.h"

void 
#ifdef IMA_WIN32_WATCOM
pvmfpack (what, vp, cnt, std, info)
	int *what, *cnt, *std, *info;
#else
__fortran PVMFPACK (what, vp, cnt, std, info)
	int *what, *cnt, *std, *info;
#endif
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

