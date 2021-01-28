/*
Testing these features:

interface declaration
  required function 'reqFun'
  required function 'dfltImpl' with a default implementation

CG function with (cgArg: ?Q)
  invokes both required functions on cgArg

implements declaration
  uses an adjacent proc declaration for 'reqFun'
  uses the default implementation for 'dfltImpl'

invocation of CG function
  the argument type is legal because of the implements declaration
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

real implements IFC; // relies on the above reqFun(real)

cgFun(2.3, 4);
