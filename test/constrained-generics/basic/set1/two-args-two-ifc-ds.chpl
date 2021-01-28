/*
This is self-arg-query.chpl
with two CG formals for cgFun()
constrained with different interfaces.

"ds" variant: between the two interfaces...
- Different name for reqFun
 - same type for the second formal
- Same name for dfltImpl
 - one version for the same type for the same second formal
 - another version with different types
*/

interface IFC1 {
  proc reqFun1(cgFormal: Self, formal2: int);
  proc dfltImplS(cgFormal: Self, formal2: string) {
    writeln("in IFC1.dfltImplS()");
  }
  proc dfltImplD(cgFormal: Self, formal2: int) {
    writeln("in IFC1.dfltImplD()");
  }
}

interface IFC2 {
  proc reqFun2(cgFormal: Self, formal2: int);
  proc dfltImplS(cgFormal: Self, formal2: string) {
    writeln("in IFC2.dfltImplS()");
  }
  proc dfltImplD(cgFormal: Self, formal2: real) {
    writeln("in IFC2.dfltImplD()");
  }
}

proc cgFun(cgArg1: ?Q1, cgArg2: ?Q2)
  where Q1 implements IFC1
     && Q2 implements IFC2
{
  writeln("in cgFun()");

  reqFun1(cgArg1,   101);
  dfltImplS(cgArg1, "ifc-1");
  dfltImplD(cgArg1, 103);

  reqFun2(cgArg2,   201);
  dfltImplS(cgArg2, "ifc-2");
  dfltImplD(cgArg2, 203);
}

proc reqFun1(arg1: real, arg2: int) {
  writeln("in reqFun1/real*int", (arg1, arg2));
}

real implements IFC1; // relies on the above reqFun(real)

proc reqFun2(arg1: int, arg2: int) {
  writeln("in reqFun2/int*int", (arg1, arg2));
}

int implements IFC2; // relies on the above reqFun(real)

cgFun(2.3, 4);
