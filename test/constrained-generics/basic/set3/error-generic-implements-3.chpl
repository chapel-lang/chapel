/*
This is generic-implements-1.chpl
that adds or assumes an incorrect implements-statement.

Within the error-generic-implements-* family:
error upon: implements IFC2(MyRec, NotherRec);
because the needed implemtation of reqFun is commented out
*/

//---------------------------------------------------------------------------

record MyRec {
  var   myVar;
  type  myType;
  param myParam;
  proc  reqMethod(): void {
    writeln("  MyRec.reqMethod");
    writeln("    this    = ", this, " : ", this.type:string);
  }
}

proc reqFun(reqArg: MyRec): void {
  writeln("  reqFun.MyRec");
  writeln("    reqArg  = ", reqArg, " : ", reqArg.type:string);
}

//-----------

record NotherRec {
  param myParam;
  type  myType;
  var   myVar;
  proc  reqMethod(): void {
    writeln("  NotherRec.reqMethod");
    writeln("    this    = ", this, " : ", this.type:string);
  }
  // see also reqFun/1 and reqFun/2 below
}

// no proc reqFun(reqArg: NotherRec)

//-----------

proc reqFun(reqArg1: MyRec, reqArg2: MyRec): void {
  writeln("  reqFun.MyRec.MyRec");
  writeln("    reqArg1 = ", reqArg1, " : ", reqArg1.type:string);
  writeln("    reqArg2 = ", reqArg2, " : ", reqArg2.type:string);
}

/*
proc reqFun(reqArg1: MyRec, reqArg2: NotherRec): void {
  writeln("  reqFun.MyRec.NotherRec");
  writeln("    reqArg1 = ", reqArg1, " : ", reqArg1.type:string);
  writeln("    reqArg2 = ", reqArg2, " : ", reqArg2.type:string);
}
*/

proc reqFun(reqArg1: NotherRec, reqArg2: MyRec): void {
  writeln("  reqFun.NotherRec.MyRec");
  writeln("    reqArg1 = ", reqArg1, " : ", reqArg1.type:string);
  writeln("    reqArg2 = ", reqArg2, " : ", reqArg2.type:string);
}

//-----------

var myInst1 = new MyRec(11, real, 111);
var myInst2 = new MyRec("hi", int, false);
var myInst3 = new NotherRec(1.78, string, "by");

writeln("myInst1 = ", myInst1);
writeln("myInst2 = ", myInst2);
writeln("myInst3 = ", myInst3);

//---------------------------------------------------------------------------

interface IFC1 {
  proc reqFun(reqFormal: Self);
  proc Self.reqMethod();
  // associated types are not tested here
}

proc cgFun(cgArg: ?Q) where Q implements IFC1 {
  writeln();
  writeln("cgFun.IFC1");
  reqFun(cgArg);
  cgArg.reqMethod();
}

//inferred: MyRec implements IFC1;
cgFun(myInst1);
cgFun(myInst2);

//would be incorrect:
// NotherRec implements IFC1;
// cgFun(myInst3);

//---------------------------------------------------------------------------

interface IFC2(T1, T2) {
  proc reqFun(reqFormal1: T1, reqFormal2: T2);
  proc T1.reqMethod();
  proc T2.reqMethod();
  // associated types are currently disallowed for multi-arg interfaces
}

proc cgFun(cgArg1: ?Q1, cgArg2: ?Q2) where implements IFC2(Q1, Q2) {
  writeln();
  writeln("cgFun.IFC2");
  reqFun(cgArg1, cgArg2);
  cgArg1.reqMethod();
  cgArg2.reqMethod();
}

//inferred: implements IFC2(MyRec, MyRec);
cgFun(myInst1, myInst2);

implements IFC2(MyRec, NotherRec);  // error: reqFun is not implemented
cgFun(myInst2, myInst3);   // error; compilation aborts before checking this

//inferred: implements IFC2(NotherRec, MyRec);
cgFun(myInst3, myInst1);

//---------------------------------------------------------------------------

writeln();
writeln("done");
