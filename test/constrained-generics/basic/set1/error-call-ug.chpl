/*
This is basic-arg-query.chpl
with added call from a CG function cgFun()
to an UG function ugFun()
which is an error.
*/


interface IFC(T) {
  proc reqFun(cgFormal: T, formal2: int);
  proc dfltImpl(cgFormal: T, formal2: int) {
    writeln("in IFC.dfltImpl()");
  }
}

proc cgFun(cgArg: ?Q, arg2: int) where implements IFC(Q) {
  writeln("in cgFun()");
  ugFun(cgArg);  // error
}

proc ugFun(arg) { }

proc reqFun(arg1: real, arg2: int) {
  writeln("in reqFun/real*int", (arg1, arg2));
}

implements IFC(real); // relies on the above reqFun(real)

cgFun(2.3, 4);
