// Passing a sync variable to a single 'ref' formal.

var SVAR: sync int;

MYPROC(SVAR);

proc MYPROC(ref FORMAL: single int) { }
