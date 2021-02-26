/*
This is basic-arg-query.chpl
where dfltImpl() is overriden at the implements statement.
*/

interface IFC(T) {
  proc reqFun(cgFormal: T, formal2: int);
  proc dfltImpl(cgFormal: T, formal2: int) {
    writeln("in IFC.dfltImpl#1()");
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

proc dfltImpl(arg1: real, arg2: int) {
  writeln("in dfltImpl#2/real*int", (arg1, arg2));
}

real implements IFC; // relies on the above reqFun(real), overriding dfltImpl

cgFun(2.3, 4);
