
/*
 * ambig.c
 *
 * $Revision$
 */

#include "d.h"
#undef DPN_TO_PN
#define DPN_TO_PN(_dpn) \
  ((PNode *)(((char*)(_dpn))-(int)(&((PNode*)0)->parse_node)))

extern void print_paren(PNode *);

struct D_ParseNode *
my_ambiguity_fn(struct D_Parser *pp, int n, struct D_ParseNode **v )
{
	int i;
	char *s;
	printf( "my_ambiguity_fn: %d possibilities:\n", n );

	s = v[0]->start_loc.s;
	while (s != v[0]->end)
		putchar( s[0] ), s++;

	for (i=0; i<n; i++) {

		printf( "\n\t" );
		print_paren( DPN_TO_PN( v[i] ) );
		printf( "\n" );

	}

	exit(0);
}

