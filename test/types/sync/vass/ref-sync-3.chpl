// Passing sync variable to non-sync 'ref' formal, different underlying type.

var SVAR: sync int;

MYPROC(SVAR);

proc MYPROC(ref FORMAL: real) { }
