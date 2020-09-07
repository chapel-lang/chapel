/*
This is basic-arg-query.chpl
with a call within cgFun() to a required interface function
that passes an incorrect actual to a CG formal.
*/

module Example {
  interface IFC(T) {
    proc reqFun(cgFormal: T, formal2: int);
    proc dfltImpl(cgFormal: T, formal2: int) {
      writeln("in IFC.dfltImpl()");
    }
  }

  proc cgFun(cgArg: ?Q, arg2: int) where implements IFC(Q) {
    writeln("in cgFun()");
    reqFun(5, arg2);    // error: 1st argument needs to be a Q
    dfltImpl(cgArg, arg2);
  }

  module Main {
    proc reqFun(arg1: real, arg2: int) {
      writeln("in reqFun/real*int", (arg1, arg2));
    }

    implements IFC(real); // relies on the above reqFun(real)

    cgFun(2.3, 4);
  }
}
