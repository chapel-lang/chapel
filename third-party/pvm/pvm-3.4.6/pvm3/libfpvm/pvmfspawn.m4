
/* $Id: pvmfspawn.m4,v 1.2 1996/10/04 15:27:40 pvmsrc Exp $ */

#include "pvm3.h"
#include "pvm_consts.h"

void
FUNCTION(pvmfspawn) ARGS(`STRING_ARG(aout), flag, STRING_ARG(where), count, tids, info')
STRING_ARG_DECL(aout);
STRING_ARG_DECL(where);
	int *flag, *count, *tids, *info;
{
	char taout[MAX_PROC_NAME + 1];
	char twhere[MAX_HOST_NAME + 1];

	/*
	 * Copy the program and where names to make sure there's
	 * a NUL at the end.
	 */
	if (ftocstr(taout, sizeof(taout), STRING_PTR(aout), STRING_LEN(aout))) {
		*info = PvmBadParam;
		return;
	}

	/*
	 * if the where parameter is '*', pass a NULL pointer to
	 * initiate which will allow execution on any machine.
	 */
	if (*(STRING_PTR(where)) == '*') {
		*info = pvm_spawn(taout, (char**)0, *flag, "", *count, tids);

	} else {
		if (ftocstr(twhere, sizeof(twhere), STRING_PTR(where), STRING_LEN(where))) {
			*info = PvmBadParam;
			return;
		}

		*info = pvm_spawn(taout, (char**)0, *flag, twhere, *count, tids);
	}
}

