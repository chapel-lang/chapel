/*
This is basic-arg-query.chpl
with a call within cgFun() to a required interface function
that passes an incorrect actual to a non-CG formal.
*/

interface IFC(T) {
  proc reqFun(cgFormal: T, formal2: int);
  proc dfltImpl(cgFormal: T, formal2: int) {
    writeln("in IFC.dfltImpl()");
  }
}

proc cgFun(cgArg: ?Q, arg2: int) where implements IFC(Q) {
  writeln("in cgFun()");
  reqFun(cgArg, 1.5);    // error: 2nd argument needs to be an int
  dfltImpl(cgArg, arg2);
}

proc reqFun(arg1: real, arg2: int) {
  writeln("in reqFun/real*int", (arg1, arg2));
}

implements IFC(real); // relies on the above reqFun(real)

cgFun(2.3, 4);
