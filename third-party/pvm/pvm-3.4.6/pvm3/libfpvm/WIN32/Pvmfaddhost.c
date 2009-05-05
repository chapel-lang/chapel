
/* $Id: Pvmfaddhost.c,v 1.2 1998/09/21 20:25:27 pvmsrc Exp $ */

#ifdef WIN32
#include "..\..\src\pvmwin.h"
#endif
#include "pvm3.h"
#include "pvm_consts.h"

#ifdef IMA_WIN32_WATCOM
#include "watforstr.h"

pvmfaddhost (host_str,info)
WatcomFortranStr* host_str;
int *info;
{
char *host_ptr=host_str->strP;
int host_len=host_str->len;
#else
void __fortran
PVMFADDHOST (host_ptr, info, host_len)
char * host_ptr; int host_len;
int *info;
{
#endif

	int  dtid;
	char thost[MAX_HOST_NAME + 1];
	char *ptr = thost;

	/*
	 * Copy the host name to make sure there's
	 * a NUL at the end.
	 */
	if (ftocstr(thost, sizeof(thost), host_ptr, host_len)) {
		*info = PvmBadParam;
		return;
	}

	*info = pvm_addhosts(&ptr, 1, &dtid);
	if (*info >= 0)
		*info = dtid;
}

