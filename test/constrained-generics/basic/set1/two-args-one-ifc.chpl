/*
This is self-arg-query.chpl
with two CG formals for cgFun()
constrained with the same interface.

One call to cgFun has the two actuals of the same type,
the other call of different types.
*/

interface IFC {
  proc reqFun(cgFormal: Self, formal2: int);
  proc dfltImpl(cgFormal: Self, formal2: int) {
    writeln("in IFC.dfltImpl()");
  }
}

proc cgFun(cgArg1: ?Q1, cgArg2: ?Q2)
  where Q1 implements IFC
     && Q2 implements IFC
{
  writeln("in cgFun()");

  reqFun(cgArg1,   55);
  dfltImpl(cgArg1, 66);

  reqFun(cgArg2,   77);
  dfltImpl(cgArg2, 88);
}

proc reqFun(arg1: real, arg2: int) {
  writeln("in reqFun/real*int", (arg1, arg2));
}

real implements IFC; // relies on the above reqFun(real)

cgFun(2.3, 4.5);

proc reqFun(arg1: int, arg2: int) {
  writeln("in reqFun/int*int", (arg1, arg2));
}

int implements IFC; // relies on the above reqFun(int)

cgFun(67, 8.9);
