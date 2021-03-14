/*
This is self-arg-query.chpl
with two CG formals for cgFun()
constrained with different interfaces.

"sd" variant: between the two interfaces...
- Same name for reqFun
 - one version for the same type for the same second formal
 - another version with different types
- Different name for dfltImpl
 - same type for the second formal
*/

interface IFC1 {
  proc reqFunS(cgFormal: Self, formal2: string);
  proc reqFunD(cgFormal: Self, formal2: real);
  proc dfltImpl1(cgFormal: Self, formal2: int) {
    writeln("in IFC1.dfltImpl1()");
  }
}

interface IFC2 {
  proc reqFunS(cgFormal: Self, formal2: string);
  proc reqFunD(cgFormal: Self, formal2: int);
  proc dfltImpl2(cgFormal: Self, formal2: int) {
    writeln("in IFC2.dfltImpl2()");
  }
}

proc cgFun(cgArg1: ?Q1, cgArg2: ?Q2)
  where Q1 implements IFC1
     && Q2 implements IFC2
{
  writeln("in cgFun()");

  reqFunS(cgArg1,   "ifc1");
  reqFunD(cgArg1,   1.02);
  dfltImpl1(cgArg1, 103);

  reqFunS(cgArg2,   "ifc2");
  reqFunD(cgArg2,   202);
  dfltImpl2(cgArg2, 203);
}

proc reqFunS(arg1: real, arg2: string) {
  writeln("in reqFunS/real*string", (arg1, arg2));
}

proc reqFunD(arg1: real, arg2: real) {
  writeln("in reqFunD/real*real", (arg1, arg2));
}

real implements IFC1; // relies on the above reqFunS,D

proc reqFunS(arg1: int, arg2: string) {
  writeln("in reqFunS/int*string", (arg1, arg2));
}

proc reqFunD(arg1: int, arg2: int) {
  writeln("in reqFunD/int*int", (arg1, arg2));
}

int implements IFC2; // relies on the above reqFunS,D

cgFun(2.3, 4);
