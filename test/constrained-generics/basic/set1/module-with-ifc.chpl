/*
This is basic-arg-query.chpl
with the interface declaration and the CG function
are wrapped in one module and "user" code in another.

This way reqFun() implementation is not visible
at call site within the CG function.
TODO: when this works, switch the other similar tests to using modules.
*/
module Library {
  interface IFC(T) {
    proc reqFun(cgFormal: T, formal2: int);
    proc dfltImpl(cgFormal: T, formal2: int) {
      writeln("in IFC.dfltImpl()");
    }
  }

  proc cgFun(cgArg: ?Q, arg2: int) where implements IFC(Q) {
    writeln("in cgFun()");
    reqFun(cgArg, arg2);
    dfltImpl(cgArg, arg2);
  }
}

module User {
  proc reqFun(arg1: real, arg2: int) {
    writeln("in reqFun/real*int", (arg1, arg2));
  }

  implements IFC(real); // relies on the above reqFun(real)

  proc main {
    cgFun(2.3, 4);
  }
}
