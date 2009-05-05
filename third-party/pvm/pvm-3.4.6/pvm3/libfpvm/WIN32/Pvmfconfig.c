
/* $Id: Pvmfconfig.c,v 1.4 2002/04/03 16:13:32 pvmsrc Exp $ */

#ifdef WIN32
#include "..\..\include\pvm3.h"
#include "..\..\src\pvmwin.h"
#else
#include "pvm3.h"
#endif

#include "pvm_consts.h"
#include "../../src/bfunc.h"
void
#ifdef IMA_WIN32_WATCOM
pvmfconfig(nhostp,
		narchp,
		tidp,
		name_ptr,name_len,
		arch_ptr,arch_len
		speedp,
		infop)

	int  *nhostp, *narchp, *tidp, *speedp, *infop;
	char * name_ptr; int name_len;
	char * arch_ptr; int arch_len;
#else
__fortran PVMFCONFIG
	(nhostp,
		narchp,
/* rd		narchp,arch_len, */
		tidp,
		name_ptr,name_len,
/* rd		arch_ptr, */
		arch_ptr,arch_len,
		speedp,
		infop)

	int  *nhostp, *narchp, *tidp, *speedp, *infop;
	char * name_ptr; int name_len;
	char * arch_ptr; int arch_len;
#endif
{
	static struct pvmhostinfo *hip = 0;
	static int nhost = 0;
	static int narch = 0;
	static int next = 0;

	int cc;
	struct pvmhostinfo *hip2;

#ifndef WIN32
	char *malloc();
#endif
/* if user sets nhostp to -1 then pvmfconfig() will be reset */

    if (hip && (*nhostp == -1)) {
        free((char*)hip);
        hip = 0;
    }

	if (!hip) {
		if ((cc = pvm_config(&nhost, &narch, &hip2)) < 0) {
			*infop = cc;
			return;
		}
		hip = (struct pvmhostinfo*)malloc(nhost * sizeof(struct pvmhostinfo));
		BCOPY((char*)hip2, (char*)hip, nhost * sizeof(struct pvmhostinfo));
		next = 0;
	}

	if (next < nhost) {
		*nhostp = nhost;
		*narchp = narch;
		*tidp = hip[next].hi_tid;
		ctofstr(name_ptr, name_len, hip[next].hi_name);
		ctofstr(arch_ptr, arch_len, hip[next].hi_arch);
		*speedp = hip[next].hi_speed;
		*infop = 1;
		next++;

	}

	if (next == nhost) {
		free((char*)hip);
		hip = 0;
	}
}

