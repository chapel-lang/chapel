/*
This is basic-arg-query.chpl
where the interface has two formals.

There are two interface constraints and two implements statements.
One each uses the general syntax 'implements IFC(type1,type2)'
and the other uses the convenience 'type1 implements IFC(type2)'.

The two overloads of cgFun(), their interface constraints,
and the implements statements are built so that each of
the two calls to cgFun() is unambiguous.
*/

interface IFC(T1,T2) {
  proc reqFun1(cgFormal: T1, formal2: int);
  proc reqFun2(cgFormal1: T1, cgFormal2: T2);
  proc dfltImpl1(cgFormal: T1, formal2: int) {
    writeln("in IFC.dfltImpl1()");
  }
  proc dfltImpl2(cgFormal1: T1, cgFormal2: T2) {
    writeln("in IFC.dfltImpl2()");
  }
}


// matches calls cgFun(R1, int or real) because implements IFC(R1,real)
proc cgFun(cgArg: ?Q, arg2: real) where Q implements IFC(real) {
  writeln("in cgFun#1()");
  reqFun1(cgArg, 23);
  reqFun2(cgArg, arg2);
  dfltImpl1(cgArg, 45);
  dfltImpl2(cgArg, arg2);
}

// matches calls cgFun(R2, int) because implements IFC(R2,int)
proc cgFun(cgArg1: ?Q1, cgArg2: ?Q2) where implements IFC(Q1, Q2) {
  writeln("in cgFun#2()");
  reqFun1(cgArg1, 67);
  reqFun2(cgArg1, cgArg2);
  dfltImpl1(cgArg1, 89);
  dfltImpl2(cgArg1, cgArg2);
}


record R1 { const x: int; }

proc reqFun1(arg1: R1, arg2: int) {
  writeln("in reqFun1/R1*int", (arg1.x, arg2));
}
proc reqFun2(arg1: R1, arg2: real) {
  writeln("in reqFun2/R1*real", (arg1.x, arg2));
}

implements IFC(R1, real); // relies on the above implementations

cgFun(new R1(67), 89);


record R2 { const x: int; }

proc reqFun1(arg1: R2, arg2: int) {
  writeln("in reqFun1/R2*int", (arg1.x, arg2));
}
proc reqFun2(arg1: R2, arg2: int) {
  writeln("in reqFun2/R2*int", (arg1.x, arg2));
}

R2 implements IFC(int); // relies on the above implementations

cgFun(new R2(23), 45);
