/*
This is basic-arg-query.chpl
with the implicit interface formal Self.
*/

interface IFC {
  proc reqFun(cgFormal: Self, formal2: int);
  proc dfltImpl(cgFormal: Self, formal2: int) {
    writeln("in IFC.dfltImpl()");
  }
}

proc cgFun(cgArg: ?Q, arg2: int) where Q implements IFC {
  writeln("in cgFun()");
  reqFun(cgArg, arg2);
  dfltImpl(cgArg, arg2);
}

proc reqFun(arg1: real, arg2: int) {
  writeln("in reqFun/real*int", (arg1, arg2));
}

real implements IFC; // relies on the above reqFun(real)

cgFun(2.3, 4);
