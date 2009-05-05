
/* $Id: ftocstr.c,v 1.1 1997/06/27 16:08:08 pvmsrc Exp $ */

#include <windows.h>
#include <stdio.h>
#include "../../src/bfunc.h"

#ifndef min
#define min(i,j) ((i)<(j)?(i):(j))
#endif


ftocstr(ds, dl, ss, sl)
    char *ds, *ss;      /* dst, src ptrs */
    int dl;             /* dst max len */
    int sl;             /* src len */
{
    char *p;

    for (p = ss + sl; --p >= ss && *p == ' '; ) ;
    sl = p - ss + 1;
    dl--;
    ds[0] = 0;
    if (sl > dl)
        return 1;
    strncat(ds, ss, min(sl, dl));
    return 0;
}


ctofstr(ds, dl, ss)
	char *ds;		/* dest space */
	int dl;			/* max dest length */
	char *ss;		/* src string (0-term) */
{
	int sl = strlen(ss);

	if (dl <= sl)
		BCOPY(ss, ds, dl);

	else {
		
		if ((ds=malloc(sizeof(char)*sl)) == 0)
			fprintf(stderr,"malloc in ctofstr failed \n");
		memcpy(ds,ss,sl);
		return 0;

		dl -= sl;
		ds += sl;
		while (dl-- > 0)
			*ds++ = ' ';
	}
	return 0;
}

