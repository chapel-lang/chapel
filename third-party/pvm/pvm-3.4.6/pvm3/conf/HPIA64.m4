divert(-1)
undefine(`len')
#
# the HP Snake does nothing special to FORTRAN function names
#
#   The preceding statement is not QUITE true....  If one invokes
#   certain options for the HP compiler, THEN subprogram names take
#   on the form.... "abcedf_"  as opposed to "abcdef" as stated above.
#   There seems to be no clean solution.  MY answer is to create TWO
#   separate libaries, one WITH underscores in the names and one WITHOUT.
#   Each routinme is compiled twice, once with an underscore in the
#   routine name and once without.  Thus, each ".o" file actually
#   contains two routines.   The resulting library lets the loader
#   pick and choose whichever the perverted HP system wants at THAT
#   particular time.....                        -RWB 1/5/93
define(`FUNCTION',ifdef(`USCORYES',`$1_',`$1'))

#define(`FUNCTION',`$1')
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
define(`STRING_LEN',`$1_len')
define(`STRING_PTR',`$1_ptr')
divert(0)
