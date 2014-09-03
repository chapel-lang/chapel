// Passing a single variable to a sync 'ref' formal.

var SVAR: single int;

MYPROC(SVAR);

proc MYPROC(ref FORMAL: sync int) { }
