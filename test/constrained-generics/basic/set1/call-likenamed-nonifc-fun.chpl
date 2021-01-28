/*
This is basic-arg-query.chpl
with the CG function also calling like-named functions
directly, not through the interface.
*/


interface IFC(T) {
  proc reqFun(cgFormal: T, formal2: int);
  proc dfltImpl(cgFormal: T, formal2: int) {
    writeln("in IFC.dfltImpl()");
  }
}

proc cgFun(cgArg: ?Q, arg2: int) where implements IFC(Q) {
  writeln("in cgFun()");
  // calling interface functions
  reqFun(cgArg, arg2);
  dfltImpl(cgArg, arg2);
  // calling non-interface functions directly
  reqFun(6.5, arg2);
  dfltImpl(3.2, "one");
}

proc reqFun(arg1: real, arg2: int) {
  writeln("in reqFun/real*int", (arg1, arg2));
}

proc dfltImpl(arg1: real, arg2: string) {
  writeln("in dfltImpl/real*string", (arg1, arg2));
}

implements IFC(real); // relies on the above reqFun(real), not dfltImpl()

cgFun(2.3, 4);
