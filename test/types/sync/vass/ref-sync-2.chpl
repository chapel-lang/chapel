// Passing a sync variable to a non-sync 'ref' formal, same underlying type.

var SVAR: sync int;

MYPROC(SVAR);
compilerError("calling MYPROC was allowed"); // no need to execute

proc MYPROC(ref FORMAL: int) { }
