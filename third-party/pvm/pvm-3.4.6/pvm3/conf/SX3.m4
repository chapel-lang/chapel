divert(-1)
undefine(`len')
#
# append an underscore to FORTRAN function names
#
define(`FUNCTION',`$1_')
#
# FORTRAN character strings are passed as follows:
# a pointer to the base of the string is passed in the normal
# argument list, and the length is passed by value as an extra
# argument, after all of the other arguments.
#
define(`ARGS',`($1`'undivert(1))')
define(`SAVE',`divert(1)$1`'divert(0)')
define(`STRING_ARG',`$1_ptr`'SAVE(`, $1_len')')
define(`STRING_ARG_DECL',`char * $1_ptr; int $1_len')
define(`INT_ARG_DECL',`int * $1')
define(`STRING_LEN',`$1_len')
define(`STRING_PTR',`$1_ptr')
divert(0)
