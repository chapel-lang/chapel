/* varpse.c
 *
 *  Utilities used during parsing Verilog files.
 *
 * $Revision$
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>

#include "vparse.h"

char *v_incdirs = NULL;

/* Definition table for preprocessor. */
#define DEFTABHASHSZ 127
struct deftab_e_s {
	char *name;
	char *substitution;
	struct deftab_e_s *next; /* next bucket. */
};

struct deftab_s {
	struct deftab_e_s *bucket[ DEFTABHASHSZ ];
};

/* A fileinfo_s struct holds all the current info about the source
 * and destination buffers of the file we are processing.
 */
struct fileinfo_s {

	char *srcbuf;
	int srclen;
	char *sp;
	int srcleft;

	char *newbuf;
	int newspace;
	char *dp;
	int newlen;

	struct deftab_s *deftab;
	int ifdefd_out;
};

/* Forward declarations. */
static int unimpl_directive( struct fileinfo_s * );
static int define_directive( struct fileinfo_s * );
static int undef_directive( struct fileinfo_s * );
static int ifdef_directive( struct fileinfo_s * );
static int endif_directive( struct fileinfo_s * );
static int else_directive( struct fileinfo_s * );
static int timescale_directive( struct fileinfo_s * );
static int include_directive( struct fileinfo_s * );

static int v_getfile2( const char *, struct fileinfo_s * );

static void def_add( struct deftab_s *, const char *, int, char * );
static char *def_find( struct deftab_s *, const char *, int );
static int def_delete( struct deftab_s *, const char *, int );
static void def_free( struct deftab_s * );

/* Preprocessor directive table. */
static struct ppdirtab_s {
	char *name;
	int (*handler)( struct fileinfo_s * );
} ppdirectives[] = {
	"accelerate",			unimpl_directive,
	"autoexpand_vectornets", 	unimpl_directive,
	"celldefine", 			unimpl_directive,
	"define", 			define_directive,
	"default_nettype", 		unimpl_directive,
	"else", 			else_directive,
	"endcelldefine", 		unimpl_directive,
	"endif", 			endif_directive,
	"endprotect",			unimpl_directive,
	"endprotected",  		unimpl_directive,
	"expand_vectornet", 		unimpl_directive,
	"ifdef", 			ifdef_directive,
	"ifndef",			ifdef_directive,
	"include",			include_directive,
	"noaccelerate",			unimpl_directive,
	"noexpand_vectornets", 		unimpl_directive,
	"noremove_gatenames",		unimpl_directive,
	"nounconnected_drive",		unimpl_directive,
	"protect", 			unimpl_directive,
	"protected",			unimpl_directive,
	"remove_gatenames",		unimpl_directive,
	"remove_netnames", 		unimpl_directive,
	"unconnected_drive",		unimpl_directive,
	"resetall",			unimpl_directive,
	"timescale",			timescale_directive,
	"unconnected_drive",		unimpl_directive,
	"undef",			undef_directive,

	NULL, NULL };

static void
kill_eol( struct fileinfo_s *pfi )
{
	while ( *pfi->sp != '\n' && pfi->srcleft > 0 ) {
		pfi->srcleft--;
		pfi->sp++;
	}
}

/* unimpl_directive:
 *
 * For any directive we can't handle, we just eat up the rest of the line.
 */
static int
unimpl_directive( struct fileinfo_s *pfi )
{
	char *cp;
	printf( "unimpl_directive: unimplemented directive: " );
	cp = pfi->sp;
	while ( *pfi->sp != '\n' && pfi->srcleft > 0 ) {
		pfi->srcleft--;
		putchar( *pfi->sp++ );
	}
	putchar( '\n' );
	return 0;
}



/* Find the length of the a def starting at s */
static int
getdeflen( const char *s, int srcleft )
{
	int deflen;

	if ( ! ( s[0] == '_' ||
		 ( s[0] >= 'a' && s[0] <= 'z' ) ||
		 ( s[0] >= 'A' && s[0] <= 'Z' ) ) || srcleft < 1 )
		return 0;

	deflen = 1;

	while ( srcleft > deflen &&
		( ( s[deflen] >= 'a' && s[deflen] <= 'z' ) ||
		  ( s[deflen] >= 'A' && s[deflen] <= 'Z' ) ||
		  ( s[deflen] >= '0' && s[deflen] <= '9' ) ||
		  s[deflen] == '_' ) )
		deflen++;

	return deflen;
}

/* define_directive:
 *
 */
static int
define_directive( struct fileinfo_s *pfi )
{
	const char *pname;
	int deflen;
	char *psub;

	if ( pfi->ifdefd_out > 0 ) {
		kill_eol( pfi );
		return 0;
	}

	while ( pfi->sp[0] && pfi->srcleft > 0 && ! isspace( pfi->sp[0] ) ) {
		pfi->sp++;
		pfi->srcleft--;
	}

	if ( pfi->srcleft > 0 && isspace( pfi->sp[0] ) ) {
		pfi->sp++;
		pfi->srcleft--;
	}

	pname = pfi->sp;
	deflen = getdeflen( pfi->sp, pfi->srcleft );
	if ( deflen < 1 ||
	     (pfi->sp[deflen] != '\n' && ! isspace( pfi->sp[deflen])) ) {
		fprintf( stderr, "bad `define directive.\n" );
		return -1;
	}

	pfi->sp += deflen;
	pfi->srcleft -= deflen;

	if ( pfi->srcleft > 0 && pfi->sp[0] == ' ' ) {

		const char *psubs;
		char *psubd;
		int sublen;

		pfi->sp++;
		pfi->srcleft--;

		psubs = pfi->sp;
		sublen = 0;

		/* Find length. */
		while ( pfi->srcleft > 0 && pfi->sp[0] != '\n' ) {
			if ( pfi->srcleft > 1 && pfi->sp[0] == '\\' &&
			     pfi->sp[1] == '\n' ) {
				pfi->sp += 2;
				pfi->srcleft -= 2;
				sublen++;
			}
			else {
				pfi->sp++;
				pfi->srcleft--;
				sublen++;
			}
		}

		/* Copy into new buffer. */
		psub = psubd = malloc( sublen+1 );
		while ( sublen-- > 0 ) {
			if ( psubs[0] == '\\' && psubs[1] == '\n' ) {
				*psubd++ = '\n';
				psubs += 2;
			}
			else
				*psubd++ = *psubs++;
		}
		*psubd++ = '\0';
	}
	else {
		psub = malloc( 1 );
		psub[0] = '\0';
	}

	def_add( pfi->deftab, pname, deflen, psub );

	return 0;
	
}

static int
undef_directive( struct fileinfo_s *pfi )
{
	const char *pname;
	int deflen;

	if ( pfi->ifdefd_out > 0 ) {
		kill_eol( pfi );
		return 0;
	}

	while ( pfi->sp[0] && pfi->srcleft > 0 && ! isspace( pfi->sp[0] ) ) {
		pfi->sp++;
		pfi->srcleft--;
	}

	if ( pfi->srcleft > 0 && isspace( pfi->sp[0] ) ) {
		pfi->sp++;
		pfi->srcleft--;
	}

	pname = pfi->sp;
	deflen = getdeflen( pfi->sp, pfi->srcleft );
	if ( deflen < 1 ||
	     (pfi->sp[deflen] != '\n' && ! isspace( pfi->sp[deflen])) ) {
		fprintf( stderr, "bad `undef directive.\n" );
		return -1;
	}

	pfi->sp += deflen;
	pfi->srcleft -= deflen;

	kill_eol( pfi );

	return def_delete( pfi->deftab, pname, deflen );
}


/* Handle ifdef and ifndef directives. */
static int
ifdef_directive( struct fileinfo_s *pfi )
{
	int ifndef, defined;
	const char *pname;
	int deflen;

	ifndef = ( pfi->srcleft > 6 && strncmp( pfi->sp, "ifndef", 6 ) == 0 );

	while ( pfi->sp[0] && pfi->srcleft > 0 && ! isspace( pfi->sp[0] ) ) {
		pfi->sp++;
		pfi->srcleft--;
	}

	if ( pfi->srcleft > 0 && isspace( pfi->sp[0] ) ) {
		pfi->sp++;
		pfi->srcleft--;
	}

	pname = pfi->sp;
	deflen = getdeflen( pfi->sp, pfi->srcleft );
	if ( deflen < 1 ||
	     (pfi->sp[deflen] != '\n' && ! isspace( pfi->sp[deflen])) ) {
		fprintf( stderr, "bad `ifdef directive.\n" );
		return -1;
	}

	pfi->sp += deflen;
	pfi->srcleft -= deflen;

	defined = def_find( pfi->deftab, pname, deflen ) != NULL;
	pfi->ifdefd_out <<= 1;
	if ( ifndef && defined || ! ifndef && ! defined )
		pfi->ifdefd_out |= 1;

	kill_eol( pfi );
	return 0;
}

static int
endif_directive( struct fileinfo_s *pfi )
{
	pfi->ifdefd_out >>= 1;
	kill_eol( pfi );
	return 0;
}

static int
else_directive( struct fileinfo_s *pfi )
{
	pfi->ifdefd_out ^= 1;
	kill_eol( pfi );
	return 0;
}

static int
timescale_directive( struct fileinfo_s *pfi )
{
	if ( pfi->ifdefd_out > 0 ) {
		kill_eol( pfi );
		return 0;
	}

	kill_eol( pfi );
	return 0;
}

/* find_include: go through directories looking for include file. */
static char *
find_include( char *filename )
{
	char *cp1, *cp2;
	static char file2[4096];

	if ( access( filename, R_OK ) == 0 )
		return filename;

	if ( ! v_incdirs )
		return NULL;

	cp1 = v_incdirs;
	while ( cp1[0] ) {

		cp2 = file2;
		while ( cp1[0] && cp1[0] != ':' &&
			(cp2 - file2) < sizeof(file2) )
			*cp2++ = *cp1++;
		if ( cp1[0] == ':' )
			cp1++;
		/* Check for buffer overflows. */
		if ( (cp2 - file2) + strlen(filename) + 2 > sizeof(file2) ) {
			fprintf( stderr, "incidr + filename too long!\n" );
			exit(1);
		}

		/* Ignore null directories. */
		if ( cp2 == file2 )
			continue;

		*cp2++ = '/';
		strcpy( cp2, filename );

		if ( access( file2, R_OK ) == 0 )
			return file2;

	}

	return NULL;
}

static int
include_directive( struct fileinfo_s *pfi )
{
	char *save_srcbuf;
	int save_srclen;
	char *save_sp;
	int save_srcleft;
	char basename[2048], *cp, *filename;

	if ( pfi->ifdefd_out > 0 ) {
		kill_eol( pfi );
		return 0;
	}

	pfi->sp += 7; /* strlen( "include" ) */
	pfi->srcleft -= 7;

	while ( pfi->srcleft > 0 && isspace(pfi->sp[0]) )
		pfi->sp++,pfi->srcleft--;
	if ( pfi->srcleft > 0 && pfi->sp[0] == '"' )
		pfi->sp++,pfi->srcleft--;

	cp = basename;
	while ( pfi->srcleft > 0 && !isspace(pfi->sp[0]) &&
		pfi->sp[0] != '\n' && pfi->sp[0] != '"' &&
		cp - basename < sizeof(basename) ) {
		*cp++ = *pfi->sp++;
		pfi->srcleft--;
	}
	*cp++ = '\0';

	kill_eol( pfi );

	filename =  find_include( basename );
	if ( ! filename ) {
		fprintf( stderr, "include_directive: can't find %s\n", basename );
		return -1;
	}

	save_srcbuf = 	pfi->srcbuf;
	save_srclen = 	pfi->srclen;
	save_sp = 	pfi->sp;
	save_srcleft = 	pfi->srcleft;

	pfi->srcbuf = NULL; /* not really necessary -- might help debug */
	pfi->srclen = 0;
	pfi->sp = NULL;
	pfi->srcleft = 0;

	if ( v_getfile2( filename, pfi ) < 0 )
		return -1;

	pfi->srcbuf = 	save_srcbuf;
	pfi->srclen =	save_srclen;
	pfi->sp =	save_sp;
	pfi->srcleft =	save_srcleft;

	return 0;
}

/* f_appendc:
 *
 * Append a character to the destination file, allocating space if
 * necessary.
 */
static __inline void
f_appendc( struct fileinfo_s *pfi, char c )
{
	if ( ! pfi->ifdefd_out || c == '\n' ) {
		if ( pfi->newlen >= pfi->newspace-64 ) {
			pfi->newspace += 1024;
			pfi->newbuf = realloc( pfi->newbuf, pfi->newspace );
			if ( ! pfi->newbuf )
				fprintf( stderr, "f_appendc: out of memory "
					 "(newspace=%d)\n",
					 pfi->newspace ),exit(1);
			pfi->dp = pfi->newbuf + pfi->newlen;
		}
  
		*pfi->dp++ = c;
		pfi->newlen++;
	}
}

/* f_appends:
 *
 * Append the string to the destination file, allocating space if
 * necessary.  XXX: untested.
 */
static void
f_appends( struct fileinfo_s *pfi, const char *s )
{
	int l = strlen(s);

	if ( ! pfi->ifdefd_out ) {
		if ( pfi->newlen + l >= pfi->newspace-64 ) {

			pfi->newspace += l;
			pfi->newspace = ( pfi->newspace+1023 ) % 1024;

			pfi->newbuf = realloc( pfi->newbuf, pfi->newspace );
			if ( ! pfi->newbuf )
				fprintf( stderr, "f_appends: out of memory "
					 "(newspace=%d)\n",
					 pfi->newspace ),exit(1);

			pfi->dp = pfi->newbuf + pfi->newlen;
		}
  
		pfi->newlen += l;
		while ( l-- > 0 )
			*pfi->dp++ = *s++;
	}
}


/* v_preprocess:
 *
 * Take input file <filebuf,oldlen> and create a new buffer and copy
 * preprocessed file to that buffer.  Does not free or disturb source
 * file.
 */
static int
v_preprocess( struct fileinfo_s *pfi )
{
	int i;
	int is_start_of_line = 1;

	while (pfi->srcleft > 0) {

		/* Verilog pre-processor stuff. */
		if ( pfi->sp[0] == '`' ) {

			int deflen;
			char *subs;

			/* Munch tick-mark */
			pfi->srcleft--;
			pfi->sp++;

			if ( is_start_of_line ) {

				/* Directive? */

				int i, l, isdirective=0;

				for (i=0; ppdirectives[i].name; i++) {

					l = strlen( ppdirectives[i].name );
					if ( pfi->srcleft >= l && ( isspace(pfi->sp[l]) || pfi->sp[l] == '\n' ) &&
					     strncmp( pfi->sp, ppdirectives[i].name, l ) == 0 ) {

						if ( (*ppdirectives[i].handler)( pfi ) < 0 )
							return -1;

						isdirective=1;
						break;
					}
				}

				if ( isdirective )
					continue;
			}

			/* Find end of legal substitution */
			deflen = getdeflen( pfi->sp, pfi->srcleft );
			if ( deflen == 0 ) {
				fprintf( stderr, "v_preprocess: illegal "
					 "definition name.\n" );
				return -1;
			}

			/* Substitute defined thingy */
			subs = def_find( pfi->deftab, pfi->sp, deflen );
			if ( subs )
				f_appends( pfi, subs );
			else {
				char name[1024];
				if ( deflen > 1023 )
					deflen = 1023;
				strncpy( name, pfi->sp, deflen );
				name[deflen] = '\0';
				fprintf( stderr, "v_preprocess: "
					 "undefined: %s\n", name );
				return -1;
			}
      
			pfi->sp += deflen;
			pfi->srcleft -= deflen;

			continue;
		}

#define HANDLE_C_COMMENTS
#ifdef HANDLE_C_COMMENTS
		if ( pfi->sp[0] == '/' && pfi->srcleft > 1 && pfi->sp[1] == '/' ) {
			/* Handle to-end-of-line comment */
			while ( pfi->sp[0] != '\n' && pfi->srcleft > 0 ) {
				pfi->srcleft--;
				pfi->sp++;
			}

			continue;
		}

		if ( pfi->sp[0] == '/' && pfi->srcleft > 1 && pfi->sp[1] == '*' ) {

			/* Handle C-like comments. */
			pfi->sp += 2;
			pfi->srcleft -= 2;

			while ( pfi->srcleft > 1 &&
				(pfi->sp[0] != '*' || pfi->sp[1] != '/') ) {

				if ( *pfi->sp == '\n' )
					f_appendc( pfi, '\n' );

				pfi->srcleft--;
				pfi->sp++;

			}

			if ( pfi->srcleft > 1 ) {
				pfi->srcleft -= 2;
				pfi->sp += 2;
			}

			continue;
		}
#endif

		if ( pfi->sp[0] == '\n' )
			is_start_of_line = 1;
		else if ( ! isspace( pfi->sp[0] ) )
			is_start_of_line = 0;

		f_appendc( pfi, *pfi->sp );
		pfi->sp++;
		pfi->srcleft--;

	}

	/* XXX */
	for (i=0; i<8; i++)
		f_appendc( pfi, '\n' );

	return 0;
}

/* Read in a file and preprocess it before returning it. */
static int
v_getfile2( const char *filename, struct fileinfo_s *pfi )
{
	int fd;
	struct stat statbuf;

	fd = open( filename, O_RDONLY );
	if ( fd < 0 ) {
		fprintf( stderr, "Trouble reading '%s': %s\n",
			 filename, strerror(errno) );
		return -1;
	}

	if ( fstat( fd, &statbuf ) < 0 ) {
		fprintf( stderr, "Trouble stat'ing '%s': %s\n",
			 filename, strerror(errno) );
		return -1;
	}

	pfi->srclen = pfi->srcleft = statbuf.st_size;
	if ( pfi->srclen < 0 )
		abort();

	pfi->srcbuf = pfi->sp = malloc( pfi->srclen + 16 );
	if ( ! pfi->srcbuf ) {
		fprintf( stderr, "Couldn't allocate %d bytes for %s\n",
			 pfi->srclen, filename );
		close( fd );
		return -1;
	}

	if ( read( fd, pfi->srcbuf, pfi->srclen ) != pfi->srclen ) {
		fprintf( stderr, "Trouble reading %d bytes for %s\n",
			 pfi->srclen, filename );
		close( fd );
		free( pfi->srcbuf );
		return -1;
	}

	close( fd );

	if ( ! pfi->newspace ) {
		pfi->newspace = (pfi->srclen+1023) & ~1023;
		pfi->newbuf = pfi->dp = malloc( pfi->newspace );
		if ( ! pfi->newbuf )
			fprintf( stderr, "v_getfile2: couldn't malloc "
				 "newbuf.\n" ),exit(1);
		pfi->newlen = 0;
		pfi->ifdefd_out = 0;
	}
	if ( v_preprocess( pfi ) < 0 ) {
		fprintf( stderr, "trouble pre-processing %s\n", filename );
		free( pfi->srcbuf );
		return -1;
	}

	free( pfi->srcbuf );
	pfi->srcbuf = NULL; /* DEBUG */
	pfi->sp = NULL;

	/* Sentinals.  v_preprocess guarantees we'll have extra room for
	 * inserting these.
	 */
	pfi->newbuf[ pfi->newlen ] = '\0';
	pfi->newbuf[ pfi->newlen+1 ] = '\0';

	return 0;
}

int
v_getfile( const char *filename, char **pbuf, int *plen )
{
	struct fileinfo_s fi;

	bzero( & fi, sizeof(fi) );

	fi.deftab = malloc( sizeof(struct deftab_s) );
	if ( ! fi.deftab )
		fprintf( stderr, "v_getfile: couldn't malloc deftab.\n" ),
			exit(1);
	bzero( fi.deftab, sizeof(struct deftab_s) );

	if ( v_getfile2( filename, & fi ) < 0 ) {
		*pbuf = NULL;
		*plen = 0;
		free( fi.newbuf );
		def_free( fi.deftab );
		return -1;
	}
	else {
		*pbuf = fi.newbuf;
		*plen = fi.newlen;
		def_free( fi.deftab );
		return 0;
	}
}

/************************** definition tables ********************************/
/* hey, reinvent the wheel with me... */

static int
deftab_hash(const char *s, int len)
{
	int j=0;
	while (len-- > 0)
		j += *s++;
	return j % DEFTABHASHSZ;
}

/* def_add:
 *
 * I use lengths instead of zero-terminated strings so we can grab the
 * definitions right out of the input buffer.
 */
static void
def_add( struct deftab_s *dt, const char *name, int namelen,
	      char *substitution )
{
	int h = deftab_hash( name, namelen );
	struct deftab_e_s **ppde = & dt->bucket[ h ];
	struct deftab_e_s *pde;

	while ( (pde = *ppde) ) {
    
		if ( strncmp( name, pde->name, namelen ) == 0 &&
		     strlen( pde->name ) == namelen ) {
			/* Already defined.  Replace substitution. */
			free( pde->substitution );
			pde->substitution = substitution;
			return;
		}
      
		ppde = & pde->next;
	}

	pde = (struct deftab_e_s *) malloc( sizeof(struct deftab_e_s) );
	if ( ! pde )
		fprintf( stderr, "adddef: out of memory." ),exit(1);
	*ppde = pde;

	pde->name = malloc( namelen+1 );
	if ( ! pde->name )
		fprintf( stderr, "addef: out of memory.\n" ),exit(1);
	memcpy( pde->name, name, namelen );
	pde->name[namelen] = '\0';

	pde->substitution = substitution;
}

static char *
def_find( struct deftab_s *dt, const char *name, int namelen )
{
	int h = deftab_hash( name, namelen );
	struct deftab_e_s *pde = dt->bucket[ h ];

	while ( pde ) {
		if ( strncmp( name, pde->name, namelen ) == 0 &&
		     strlen( pde->name ) == namelen )
			return pde->substitution;
		pde = pde->next;
	}
	return NULL;
}

static int
def_delete( struct deftab_s *dt, const char *name, int namelen )
{
	int h = deftab_hash( name, namelen );
	struct deftab_e_s **ppde = & dt->bucket[ h ];
	struct deftab_e_s *pde;

	while ( (pde = *ppde) ) {

		if ( strncmp( name, pde->name, namelen ) == 0 &&
		     strlen( pde->name ) == namelen ) {
      
			*ppde = pde->next;

			free( pde->name );
			free( pde->substitution );
			free( pde );

			return 0;
		}

		ppde = & pde->next;
	}

	return -1;
}

static void
def_free( struct deftab_s *dt )
{
	int i;
	struct deftab_e_s *pde, *pde_tmp;

	for (i=0; i<DEFTABHASHSZ; i++) {
		pde = dt->bucket[ i ];
		while ( pde ) {
			pde_tmp = pde->next;
			free( pde );
			pde = pde_tmp;
		}
	}
	free( dt );
}

/***************************** other helper functions ********************/

static char *keyword_table[] = {
	"always",
	"and",
	"assign",
	"attribute",
	"begin",
	"buf",
	"bufif0",
	"bufif1",
	"case",
	"casex",
	"casez",
	"cmos",
	"deassign",
	"default",
	"defparam",
	"disable",
	"edge",
	"else",
	"end",
	"endattribute",
	"endcase",
	"endfunction",
	"endmodule",
	"endprimitive",
	"endspecify",
	"endtable",
	"endtask",
	"event",
	"for",
	"force",
	"forever",
	"fork",
	"function",
	"highz0",
	"highz1",
	"if",
	"initial",
	"inout",
	"input",
	"integer",
	"join",
	"large",
	"macromodule",
	"medium",
	"module",
	"nand",
	"negedge",
	"nmos",
	"nor",
	"not",
	"notif0",
	"notif1",
	"or",
	"output",
	"parameter",
	"pmos",
	"posedge",
	"primitive",
	"pull0",
	"pull1",
	"pulldown",
	"pullup",
	"rcmos",
	"real",
	"realtime",
	"reg",
	"release",
	"repeat",
	"rnmos",
	"rpmos",
	"rtran",
	"rtranif0",
	"rtranif1",
	"scalared",
	"signed",
	"small",
	"specify",
	"specparam",
	"strength",
	"strong0",
	"strong1",
	"supply0",
	"supply1",
	"table",
	"task",
	"time",
	"tran",
	"tranif0",
	"tranif1",
	"tri",
	"tri0",
	"tri1",
	"triand",
	"trior",
	"trireg",
	"unsigned",
	"vectored",
	"wait",
	"wand",
	"weak0",
	"weak1",
	"while",
	"wire",
	"wor",
	"xnor",
	"xor",
	NULL };

static short keyword_length[ 128 ];

int
v_iskeyword( const char *s, const char *e )
{
	int i;
	int slen = (e - s);

	if ( s[0] < 'a' || s[0] > 'z' )
		return 0;

	/* This could be faster. */
	for (i=0; keyword_table[i]; i++)
		if ( slen == keyword_length[i] &&
		     strncmp( s, keyword_table[i], slen ) == 0 )
			return 1;

	return 0;
}

void
v_parse_init(void)
{
	int i;

	for (i=0; keyword_table[i]; i++)
		keyword_length[i] = strlen( keyword_table[i] );
}
