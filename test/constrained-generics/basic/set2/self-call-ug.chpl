/*
This is self-arg-query.chpl
with a check for proper scope-resolution of Self.
*/

type Self = int;
type Self2 = string;

interface IFC {
  proc reqFun(cgFormal: Self, formal2: int);
  proc dfltImpl(cgFormal: Self, formal2: int) {
    ugFun(Self2);
    ugFun(Self);
    writeln("in IFC.dfltImpl()");
  }
}

proc cgFun(cgArg: ?Q, arg2: int) where implements IFC(Q) {
  writeln("in cgFun()");
  reqFun(cgArg, arg2);
  dfltImpl(cgArg, arg2);
}

proc ugFun(type arg) { compilerWarning(arg:string); }

proc reqFun(arg1: real, arg2: int) {
  writeln("in reqFun/real*int", (arg1, arg2));
}

implements IFC(real); // relies on the above reqFun(real)

cgFun(2.3, 4);
