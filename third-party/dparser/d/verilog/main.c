
/*
 * main.c
 *
 * 	Main for Verilog parser.
 *
 * $Revision$
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "vparse.h"
#include "dparse.h"

extern D_ParserTables parser_tables_gram;
extern int verbose_level;
extern struct D_ParseNode *my_ambiguity_fn(struct D_Parser *pp, int n,
                                           struct D_ParseNode **v );

static char *usage = "%s [-vpI] file ...\n";

int
main(int argc, char *argv[]) {

	char 	*filebuf;
	int 	size;
	int	ch, arg;
	char	incdirsbuf[2048];

	int	dump_pp = 0;

	D_Parser *p = new_D_Parser(&parser_tables_gram,
				   sizeof(D_ParseNode_User));
	p->ambiguity_fn = my_ambiguity_fn;

	while ((ch = getopt(argc, argv, "vphI:")) != -1)
		switch (ch) {
		case 'v': verbose_level++; 	break;
		case 'p': dump_pp++;		break;
		case 'I':
			if ( ! v_incdirs ) {
				v_incdirs = incdirsbuf;
				v_incdirs[0] = '\0';
			}
			if ( strlen( v_incdirs ) + strlen( optarg ) + 2 >
			     sizeof(incdirsbuf) )
				exit(1);
			strcat( v_incdirs, optarg );
			strcat( v_incdirs, ":" );
			break;
		case '?':
		case 'h':
		default:
			fprintf( stderr, usage, argv[0] );
			exit(1);
		}

	for (arg = optind; arg < argc; arg++) {

		if ( v_getfile( argv[ arg ], &filebuf, &size ) < 0 ) {
			fprintf( stderr, "Trouble getting '%s', skipping.\n",
				 argv[ arg ] );
			continue;
		}

		p->loc.pathname = argv[ arg ];
		p->loc.line = 1;
		p->loc.col = p->loc.previous_col = 0;

		if ( dump_pp ) {
			int i;
			for (i=0; i<size; i++)
				putchar( filebuf[i] );
		}

		v_parse_init();
		if ( dparse( p, filebuf, size ) && !p->syntax_errors )
			fprintf( stderr, "%s succeeded\n", argv[ arg ] );
		else
			fprintf( stderr, "%s failed\n", argv[ arg ] );

		free( filebuf );

		arg++;
	}

	return 0;
}
