/*
Ensure an error when implements arguments mismatched.
*/

interface IFC2(T1, T2) {
  proc reqFun(cgFormal1: T1, cgFormal2: T2);
}

proc cgFun(cgArg1: ?Q1, cgArg2: ?Q2) where implements IFC2(Q1, Q2) {
  writeln("IFC2.cgFun");
  reqFun(cgArg1, cgArg2);
}

record MyRec1 {
  type AT;
}

record MyRec2 {
  param PP;
}

proc reqFun(cgFormal1: MyRec1, cgFormal2: MyRec2): void {
  writeln("in reqFun(MyRec1,MyRec2)");
}

implements IFC2(MyRec1, MyRec2);  // ok

implements IFC2(MyRec2, MyRec1);  // error: no reqFun(MyRec2,MyRec1)

cgFun(new MyRec2("hello"), new MyRec1(locale));  // report the above error
