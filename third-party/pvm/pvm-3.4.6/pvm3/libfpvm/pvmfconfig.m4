
/* $Id: pvmfconfig.m4,v 1.4 1997/05/07 18:07:30 pvmsrc Exp $ */

#include "pvm3.h"
#include "pvm_consts.h"
#include "../src/bfunc.h"

void
FUNCTION(pvmfconfig)
	ARGS(`nhostp,
		narchp,
		tidp,
		STRING_ARG(name),
		STRING_ARG(arch),
		speedp,
		infop')

	int  *nhostp, *narchp, *tidp, *speedp, *infop;
	STRING_ARG_DECL(name);
	STRING_ARG_DECL(arch);
{
	static struct pvmhostinfo *hip = 0;
	static int nhost = 0;
	static int narch = 0;
	static int next = 0;

	int nh;
	int cc;

/* if user sets nhostp to -1 then pvmfconfig() will be reset */

    if ( nhost && nhostp && (*nhostp == -1) )
        nhost = 0;

	if ( !nhost ) {
		if ( (cc = pvm_config( &nh, &narch, &hip )) < 0 ) {
			*infop = cc;
			return;
		}
		nhost = nh;
		next = 0;
	}

	if ( next < nhost ) {
		if ( nhostp ) *nhostp = nhost;
		if ( narchp ) *narchp = narch;
		if ( tidp ) *tidp = hip[ next ].hi_tid;
		if ( STRING_PTR(name) )
			ctofstr( STRING_PTR(name), STRING_LEN(name),
					hip[ next ].hi_name );
		if ( STRING_PTR(arch) )
			ctofstr( STRING_PTR(arch), STRING_LEN(arch),
					hip[ next ].hi_arch );
		if ( speedp ) *speedp = hip[ next ].hi_speed;
		*infop = 1;
		next++;
	}

	if ( next == nhost )
		nhost = 0;
}

