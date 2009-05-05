
/* $Id: pvmftasks.m4,v 1.4 1997/05/07 18:07:32 pvmsrc Exp $ */

#include "pvm3.h"
#include "pvm_consts.h"
#include "../src/bfunc.h"

void
FUNCTION(pvmftasks)
	ARGS(`where,
		ntaskp,
		tidp,
		ptidp,
		hostp,
		flagp,
		STRING_ARG(a_out),
		infop')

	int *where, *ntaskp, *tidp, *ptidp, *hostp, *flagp, *infop;
	STRING_ARG_DECL(a_out);
{
	static struct pvmtaskinfo *tip = 0;
	static int ntask = 0;
	static int next = 0;

	int nt;
	int cc;

/* if user sets ntaskp to -1 then pvmftasks() will be reset */

    if ( ntask && ntaskp && (*ntaskp == -1) )
        ntask = 0;

	if ( !ntask ) {
		if ( (cc = pvm_tasks( *where, &nt, &tip )) < 0 ) {
			*infop = cc;
			return;
		}
		ntask = nt;
		next = 0;
	}

	if ( next < ntask ) {
		if ( ntaskp ) *ntaskp = ntask;
		if ( tidp ) *tidp = tip[ next ].ti_tid;
		if ( ptidp ) *ptidp = tip[ next ].ti_ptid;
		if ( hostp ) *hostp = tip[ next ].ti_host;
		if ( flagp ) *flagp = tip[ next ].ti_flag;
		if ( STRING_PTR(a_out) )
			ctofstr( STRING_PTR(a_out), STRING_LEN(a_out),
					tip[ next ].ti_a_out );
		*infop = 1;
		next++;
	}

/* cannot return ti_pid without changing interface */
/* users needing this will have to use C           */

	if ( next == ntask )
		ntask = 0;
}

