
/* $Id: pvmfperror.m4,v 1.3 2007/04/19 23:15:00 pvmsrc Exp $ */

#include <stdio.h>
#include <stdlib.h>
#include "pvm3.h"
#include "pvm_consts.h"

#ifdef  SYSVSTR
#include <string.h>
#else
#include <strings.h>
#endif

#define MAX(a,b) ((a) > (b) ? (a) : (b))

void
FUNCTION(pvmfperror) ARGS(`STRING_ARG(p), info')
STRING_ARG_DECL(p);
	int *info;
{
	static char *buf = 0;
	static int buflen = 0;

	/*
	 * Have to have a NUL at the end of the string, and
	 * the only way to do this portably is to copy the whole string
	 * into a malloc'ed buffer.  We keep the buffer around for
	 * future use rather than free'ing it each time we're done.
	 */
	if (!buf)
		buf = malloc(buflen = STRING_LEN(p) + 1);
	else
		if (STRING_LEN(p) + 1 > buflen) {
			buflen = MAX(STRING_LEN(p) + 1, buflen * 2);
			/* don't use realloc; it might cause old data to be copied */
			free(buf);
			buf = malloc(buflen);
		}
	if (!buf) {
		fprintf(stderr, "pvmfperror PvmNoMem");
		*info = PvmNoMem;
		return;
	}
	strncpy(buf, STRING_PTR(p), STRING_LEN(p));
	buf[STRING_LEN(p)] = '\0';

	*info = pvm_perror(buf);
}

