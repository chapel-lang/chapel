// Passing a sync variable to a sync 'ref' formal, different underlying type.

var SVAR: sync int;

MYPROC(SVAR);

proc MYPROC(ref FORMAL: sync real) { }
