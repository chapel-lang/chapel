/*
This is basic-arg-query.chpl
where the 'implements' statement is removed
and the call to cgFun is modified so that
the interface constraint in proc cgFun is not satisfied.
*/

interface IFC(T) {
  proc reqFun(cgFormal: T, formal2: int);
  proc dfltImpl(cgFormal: T, formal2: int) {
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

cgFun("hello", 4); //error: the constraint on cgFun is not satisfied

//string implements IFC;  // does not hold
