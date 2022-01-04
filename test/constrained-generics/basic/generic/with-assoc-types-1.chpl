/*
This is IFC1 from generic-implements-1.chpl
with added testing of associated types.
*/

interface IFC1 {
  type AT;
  proc reqFun(cgArg: Self);
  proc reqFun(cgArg: Self, atArg: AT);
}

proc cgFun(cgArg: ?Q) where Q implements IFC1 {
  writeln("cgFun.IFC1");
  reqFun(cgArg);
}

proc cgFun(cgArg: ?Q, atArg: cgArg.AT) where Q implements IFC1 {
  writeln("cgFun.IFC1.AT");
  reqFun(cgArg, atArg);
}

// nb unconstrained generic
proc show(cgName: string, cgArg) {
  writeln("  reqFun.", cgName);
  writeln("    cgArg   = ", cgArg, " : ", cgArg.type:string);
}
proc show(cgName: string, cgArg, atName: string, atArg) {
  writeln("  reqFun.", cgName, ".", atName);
  writeln("    cgArg   = ", cgArg, " : ", cgArg.type:string);
  writeln("    atArg   = ", atArg, " : ", atArg.type:string);
}


//--- RT* series have a generic type field ---//

// RT1: generic type field is not an assoc type
// assoc type is concrete
record RT1 {
  var  xx: int;
  type GT;
  proc AT type return int;
}
proc reqFun(cgArg: RT1)            : void { show("RT1", cgArg); }
proc reqFun(cgArg: RT1, atArg: int): void { show("RT1", cgArg,
                                                 "int", atArg); }
//inferred: RT1 implements IFC1
var rt1 = new RT1(101, real);
cgFun(rt1);
cgFun(rt1, 11);


// RT2: assoc type == generic type field
record RT2 {
  var  xx: int;
  type AT;
}
proc reqFun(cgArg: RT2)                 : void { show("RT2", cgArg); }
proc reqFun(cgArg: RT2, atArg: cgArg.AT): void { show("RT2", cgArg,
                                                      "AT",  atArg); }
//inferred: RT2 implements IFC1;
var rt2 = new RT2(102, bool);
cgFun(rt2);
cgFun(rt2, true);


// RT3: like RT1, assoc type is computed to be the generic type field
record RT3 {
  var  xx: int;
  type GT;
  proc AT type return GT;
}
proc reqFun(cgArg: RT3)                 : void { show("RT3", cgArg); }
proc reqFun(cgArg: RT3, atArg: cgArg.GT): void { show("RT3", cgArg,
                                                      "GT",  atArg); }
//inferred: RT3 implements IFC1;
var rt3 = new RT3(103, string);
cgFun(rt3);
cgFun(rt3, "rt03");


// RT4: like RT1, assoc type is this.type
record RT4 {
  var  xx: int;
  type GT;
  proc AT type return this.type;
}
proc reqFun(cgArg: RT4)                   : void { show("RT4", cgArg); }
proc reqFun(cgArg: RT4, atArg: cgArg.type): void { show("RT4", cgArg,
                                                        "self",atArg); }
//inferred: RT4 implements IFC1;
var rt4 = new RT4(104, locale);
cgFun(rt4);
cgFun(rt4, rt4);


//--- RV* series have generic var field ---//

// RV1: associated type is concrete
record RV1 {
  var  xx;
  proc AT type return int;
}
proc reqFun(cgArg: RV1)            : void { show("RV1", cgArg); }
proc reqFun(cgArg: RV1, atArg: int): void { show("RV1", cgArg,
                                                 "int", atArg); }
//inferred: RV1 implements IFC1
var rv1 = new RV1(2.01);
cgFun(rv1);
cgFun(rv1, 21);


// RV3: like RV1, assoc type is computed to be the type of the generic var field
record RV3 {
  var  xx;
  proc AT type return xx.type;
}
proc reqFun(cgArg: RV3)                 : void { show("RV3", cgArg); }
proc reqFun(cgArg: RV3, atArg: cgArg.AT): void { show("RV3", cgArg,
                                                      "AT",  atArg); }
//inferred: RV3 implements IFC1;
var rv3 = new RV3("rv03");
cgFun(rv3);
cgFun(rv3, "bis03");


// RV4: like RV1, assoc type is this.type
record RV4 {
  var  xx;
  proc AT type return this.type;
}
proc reqFun(cgArg: RV4)                   : void { show("RV4", cgArg); }
proc reqFun(cgArg: RV4, atArg: cgArg.type): void { show("RV4", cgArg,
                                                        "self",atArg); }
//inferred: RV4 implements IFC1;
var rv4 = new RV4(here);
cgFun(rv4);
cgFun(rv4, rv4);


//--- R*P* series include param fields ---//

// RTX1: generic type field is not an assoc type
// assoc type is concrete
record RTX1 {
  var  xx: int;
  type GT;
  proc AT type return int;
  param PP;
}
proc reqFun(cgArg: RTX1)            : void { show("RTX1", cgArg); }
proc reqFun(cgArg: RTX1, atArg: int): void { show("RTX1", cgArg,
                                                 "int", atArg); }
//inferred: RTX1 implements IFC1
var rtx1 = new RTX1(301, real, "PP31");
cgFun(rtx1);
cgFun(rtx1, 31);


// RTX2: assoc type == generic type field
record RTX2 {
  var  xx: int;
  type AT;
  param PP;
}
proc reqFun(cgArg: RTX2)                 : void { show("RTX2", cgArg); }
proc reqFun(cgArg: RTX2, atArg: cgArg.AT): void { show("RTX2", cgArg,
                                                      "AT",  atArg); }
//inferred: RTX2 implements IFC1;
var rtx2 = new RTX2(302, bool, 3.02);
cgFun(rtx2);
cgFun(rtx2, true);


// RTX3: like RTX1, assoc type is computed to be the generic type field
record RTX3 {
  var  xx: int;
  type GT;
  proc AT type return GT;
  param PP;
}
proc reqFun(cgArg: RTX3)                 : void { show("RTX3", cgArg); }
proc reqFun(cgArg: RTX3, atArg: cgArg.GT): void { show("RTX3", cgArg,
                                                      "GT",  atArg); }
//inferred: RTX3 implements IFC1;
var rtx3 = new RTX3(303, string, false);
cgFun(rtx3);
cgFun(rtx3, "rtx03");


// RTX4: like RTX1, assoc type is this.type
record RTX4 {
  var  xx: int;
  type GT;
  proc AT type return this.type;
  param PP;
}
proc reqFun(cgArg: RTX4)                   : void { show("RTX4", cgArg); }
proc reqFun(cgArg: RTX4, atArg: cgArg.type): void { show("RTX4", cgArg,
                                                        "self",atArg); }
//inferred: RTX4 implements IFC1;
var rtx4 = new RTX4(304, locale, 3.04);
cgFun(rtx4);
cgFun(rtx4, rtx4);


// RVX1: associated type is concrete
record RVX1 {
  var  xx;
  proc AT type return int;
  param PP;
}
proc reqFun(cgArg: RVX1)            : void { show("RVX1", cgArg); }
proc reqFun(cgArg: RVX1, atArg: int): void { show("RVX1", cgArg,
                                                 "int", atArg); }
//inferred: RVX1 implements IFC1
var rvx1 = new RVX1(4.01, true);
cgFun(rvx1);
cgFun(rvx1, 41);


// RVX3: like RVX1, assoc type is computed to be the type of the generic var field
record RVX3 {
  var  xx;
  proc AT type return xx.type;
  param PP;
}
proc reqFun(cgArg: RVX3)                 : void { show("RVX3", cgArg); }
proc reqFun(cgArg: RVX3, atArg: cgArg.AT): void { show("RVX3", cgArg,
                                                      "AT",  atArg); }
//inferred: RVX3 implements IFC1;
var rvx3 = new RVX3("rvx03", 4.03);
cgFun(rvx3);
cgFun(rvx3, "bis03");

// RVX4: like RVX1, assoc type is this.type
record RVX4 {
  var  xx;
  proc AT type return this.type;
  param PP;
}
proc reqFun(cgArg: RVX4)                   : void { show("RVX4", cgArg); }
proc reqFun(cgArg: RVX4, atArg: cgArg.type): void { show("RVX4", cgArg,
                                                        "self",atArg); }
//inferred: RVX4 implements IFC1;
var rvx4 = new RVX4(here, "there");
cgFun(rvx4);
cgFun(rvx4, rvx4);

