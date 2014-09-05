// Passing a sync variable to a generic 'ref' formal.

var SVAR: sync int = 333;

MYPROC(SVAR);
writeln("SVAR isFull: ", SVAR.isFull);

proc MYPROC(ref FORMAL) {
  compilerWarning(typeToString(FORMAL.type), 0);
  writeln("started MYPROC");
  FORMAL;
  writeln("read the FORMAL");
  FORMAL = 444;
}
