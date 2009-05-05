
/* $Id: Pvmfarchcode.c,v 1.2 1998/09/21 20:25:30 pvmsrc Exp $ */

#ifdef WIN32
#include "..\..\src\pvmwin.h"
#endif

#include "pvm3.h"
#include "pvm_consts.h"
#ifdef IMA_WIN32_WATCOM
void 
pvmfarchcode (arch_ptr, code, arch_len)
#else
void __fortran
PVMFARCHCODE (arch_ptr, code, arch_len)
#endif
char * arch_ptr; int arch_len;
int *code;
{
	char tarch[MAX_HOST_NAME + 1];

	/*
	 * Copy the arch name to make sure there's
	 * a NUL at the end.
	 */
	if (ftocstr(tarch, sizeof(tarch), arch_ptr, arch_len)) {
		*code = PvmBadParam;
		return;
	}

	*code = pvm_archcode(tarch);
}

