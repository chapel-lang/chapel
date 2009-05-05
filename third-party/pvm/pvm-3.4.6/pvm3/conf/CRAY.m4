#include <fortran.h>

undefine(`len')
define(`FUNCTION',`translit($1,abcdefghijklmnopqrstuvwxyz,ABCDEFGHIJKLMNOPQRSTUVWXYZ)')
define(`STRING_ARG',`$1_fcd')
define(`STRING_ARG_DECL',`_fcd $1_fcd')
define(`STRING_LEN',`_fcdlen($1_fcd)')
define(`STRING_PTR',`_fcdtocp($1_fcd)')
define(`ARGS',`($1)')
