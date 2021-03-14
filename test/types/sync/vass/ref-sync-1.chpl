// Passing a sync variable to a generic 'ref' formal.

var SVAR: sync int = 333;

MYPROC(SVAR);
writeln("SVAR isFull: ", SVAR.isFull);

proc MYPROC(ref FORMAL) {
  compilerWarning(FORMAL.type:string, 0);
  writeln("started MYPROC");
  FORMAL.readFE();
  writeln("read the FORMAL");
  FORMAL.writeEF(444);
}
