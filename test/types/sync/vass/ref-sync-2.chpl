// Passing a sync variable to a non-sync 'ref' formal, same underlying type.

var SVAR: sync int;

MYPROC(SVAR);

proc MYPROC(ref FORMAL: int) { }
