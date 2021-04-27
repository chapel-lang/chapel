/*
A variation of with-assoc-types-*.chpl
to test multiple associated types and multi-arg interfaces.
*/

module Lib {

interface IFC1 {
  type AT1;
  type AT2;
  type AT3;
  type AT4;
  proc reqFun(cgFormal: Self, atFormal1: AT1, atFormal2: AT2,
                              atFormal3: AT3, atFormal4: AT4);
}

proc cgFun(cgArg: ?Q, atArg1: cgArg.AT1, atArg2: cgArg.AT2,
                      atArg3: cgArg.AT3, atArg4: cgArg.AT4)
  where Q implements IFC1
{
  writeln("IFC1.cgFun");
  reqFun(cgArg, atArg1, atArg2, atArg3, atArg4);
}

//-----------

interface IFC2(T1, T2) {
  // associated types are currently disallowed for multi-arg interfaces
  proc reqFun(cgFormal1: T1, cgFormal2: T2);
}

proc cgFun(cgArg1: ?Q1, cgArg2: ?Q2) where implements IFC2(Q1, Q2) {
  writeln("IFC2.cgFun");
  reqFun(cgArg1, cgArg2);
}

//-----------

record MyRec1 {
  var   xx;
  proc  AT1 type return xx.type;
  proc  AT2 type return this.type;
  proc  AT3 type return uint;
  type  AT4;
  param PP;
}

proc reqFun(cgFormal: MyRec1,
            atFormal1: cgFormal.AT1,
            atFormal2: cgFormal.AT2,
            atFormal3: cgFormal.AT3,
            atFormal4: cgFormal.AT4): void
{
  writeln("  reqFun.MyRec1.AT1.AT2.AT3.AT4");
  writeln("    cgFormal  = ", cgFormal,  " : ", cgFormal.type: string);
  writeln("    atFormal1 = ", atFormal1, " : ", atFormal1.type: string);
  writeln("    atFormal2 = ", atFormal2, " : ", atFormal2.type: string);
  writeln("    atFormal3 = ", atFormal3, " : ", atFormal3.type: string);
  writeln("    atFormal4 = ", atFormal4, " : ", atFormal4.type: string);
}

MyRec1 implements IFC1;

//-----------

record MyRec2 {
  type AT1, AT2, AT3, AT4;
  var xx: int;
}

// would be incorrect: MyRec2 implements IFC1;

proc reqFun(cgFormal1: MyRec1, cgFormal2: MyRec2): void {
  writeln("  reqFun.MyRec1.MyRec2");
  writeln("    cgFormal1  = ", cgFormal1,  " : ", cgFormal1.type: string);
  writeln("    cgFormal2  = ", cgFormal2,  " : ", cgFormal2.type: string);
}

proc reqFun(cgFormal2: MyRec2, cgFormal1: MyRec1): void {
  writeln("  reqFun.MyRec2.MyRec1");
  writeln("    cgFormal2  = ", cgFormal2,  " : ", cgFormal2.type: string);
  writeln("    cgFormal1  = ", cgFormal1,  " : ", cgFormal1.type: string);
}

implements IFC2(MyRec1, MyRec2);
implements IFC2(MyRec2, MyRec1);

} // module Lib

//-----------

module App {
use Lib;
proc main {

var myrec1a = new MyRec1(112, real, 115);  // ATs: int, This, uint, real
cgFun(myrec1a, -116, myrec1a, 117:uint, 1.18);

var myrec2a = new MyRec2(uint, int, string, real, 121);
// would be incorrect: cgFun(myrec2a, 123:uint, -124, "s125", 1.26);

cgFun(myrec1a, myrec2a);
cgFun(myrec2a, myrec1a);

} // main
} // module App
