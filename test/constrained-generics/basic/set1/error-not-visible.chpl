/*
This is module-with-ifc.chpl
with the "import" declaration removed.

While there, also check an incorrect symbol in an 'implements'.
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

module Confused {
  class MyClass { }
  int implements MyClass;

  record MyRecord { }
  proc cgFun(cgArg: ?Q) where Q implements MyRecord { }
}

module User {
  use Confused;

  proc reqFun(arg1: real, arg2: int) {
    writeln("in reqFun/real*int", (arg1, arg2));
  }

  implements IFC(real); // relies on the above reqFun(real)

  proc main {
    cgFun(2.3, 4);
  }
}
