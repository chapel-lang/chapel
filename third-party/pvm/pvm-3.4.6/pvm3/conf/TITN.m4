divert(-1)
undefine(`len')
#
# FORTRAN function names are in upper case
#
define(`FUNCTION',`translit($1,abcdefghijklmnopqrstuvwxyz,ABCDEFGHIJKLMNOPQRSTUVWXYZ)')
#
# The Titan uses a special string descriptor to pass strings in FORTRAN.
# The descriptor is passed by reference.
#
divert(0)
typedef struct { char *ptr; int length; } FSD;
divert(-1)
define(`STRING_ARG',`$1_fsd')
define(`STRING_ARG_DECL',`FSD * $1_fsd')
define(`STRING_LEN',`($1_fsd)->length')
define(`STRING_PTR',`($1_fsd)->ptr')
define(`ARGS',`($1)')
divert(0)
