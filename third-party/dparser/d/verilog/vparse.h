/* vparse.h
 *
 *  Utilities used during parsing Verilog files.
 *
 */

extern int v_iskeyword( const char *, const char * );
extern void v_parse_init( void );
extern int v_getfile( const char *, char **, int * );

extern char *v_incdirs;
