/*
This is self-arg-query.chpl
with on CG formals for cgFun()
constrained with different interfaces.

Each of reqFun and dfltImpl has
one version that is implemented in both interfaces
and another version implemented in only one interface.
*/

interface IFC1 {
  proc reqFunB(cgFormal: Self, formal2: real);
  proc reqFunO1(cgFormal: Self, formal2: string);
  proc dfltImplB(cgFormal: Self, formal2: int) {
    writeln("in IFC1.dfltImplB()");
  }
  proc dfltImplO1(cgFormal: Self, formal2: string) {
    writeln("in IFC1.dfltImplO1()");
  }
}

interface IFC2 {
  proc reqFunB(cgFormal: Self, formal2: int);
  proc reqFunO2(cgFormal: Self, formal2: string);
  proc dfltImplB(cgFormal: Self, formal2: real) {
    writeln("in IFC2.dfltImplB()");
  }
  proc dfltImplO2(cgFormal: Self, formal2: string) {
    writeln("in IFC2.dfltImplO2()");
  }
}

proc cgFun(cgArg: ?Q, argI: int, argR: real)
  where Q implements IFC1
     && Q implements IFC2
{
  writeln("in cgFun()");

  reqFunB(cgArg,   argR);
  reqFunO1(cgArg,  "ifc1");
  dfltImplB(cgArg, argI);
  dfltImplO1(cgArg, "ifc1");

  reqFunB(cgArg,   argI);
  reqFunO2(cgArg,  "ifc2");
  dfltImplB(cgArg, argR);
  dfltImplO2(cgArg, "ifc2");
}

proc reqFunB(arg1: real, arg2: real) {
  writeln("in reqFunB/real*real", (arg1, arg2));
}

proc reqFunO1(arg1: real, arg2: string) {
  writeln("in reqFunO1/real*string", (arg1, arg2));
}

real implements IFC1; // relies on the above reqFuns

proc reqFunB(arg1: real, arg2: int) {
  writeln("in reqFunB/real*int", (arg1, arg2));
}

proc reqFunO2(arg1: real, arg2: string) {
  writeln("in reqFunO2/real*string", (arg1, arg2));
}

real implements IFC2; // relies on the above reqFuns

cgFun(11.11, 23, 4.5);
