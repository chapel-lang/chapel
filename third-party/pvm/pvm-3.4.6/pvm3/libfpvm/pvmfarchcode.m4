
/* $Id: pvmfarchcode.m4,v 1.2 1996/10/04 15:26:54 pvmsrc Exp $ */

#include "pvm3.h"
#include "pvm_consts.h"

void
FUNCTION(pvmfarchcode) ARGS(`STRING_ARG(arch), code')
STRING_ARG_DECL(arch);
int *code;
{
	char tarch[MAX_HOST_NAME + 1];

	/*
	 * Copy the arch name to make sure there's
	 * a NUL at the end.
	 */
	if (ftocstr(tarch, sizeof(tarch), STRING_PTR(arch), STRING_LEN(arch))) {
		*code = PvmBadParam;
		return;
	}

	*code = pvm_archcode(tarch);
}

