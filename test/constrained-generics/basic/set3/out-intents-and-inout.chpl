// elaboration on w.outs-1.chpl
// to highlight the difference of an out intent initializing vs. assigning
// into the actual

interface IFC(Self) {
  type AT;
  proc chpl__initCopy(rhs: Self, definedConst: bool): Self;
  proc chpl__initCopy(rhs: AT, definedConst: bool): AT;
  operator =(ref lhs: Self, rhs: Self);
  operator =(ref lhs: AT, rhs: AT);
  proc computeSelf(arg: Self): Self;
  proc computeAT(arg: AT): AT;
}

proc icInner(valSelf: ?QQ, inout inoutSelf: QQ,    out outSelf: QQ,
             valAT: QQ.AT, inout inoutAT: QQ.AT,   out outAT: QQ.AT)
  where QQ implements IFC
{
  inoutSelf = computeSelf(valSelf);
  outSelf   = computeSelf(valSelf);
  inoutAT   = computeAT(valAT);
  outAT     = computeAT(valAT);
}

proc icOuter(valSelf: ?RR, inout inoutSelf: RR,    out outSelf: RR,
             valAT: RR.AT, inout inoutAT: RR.AT,   out outAT: RR.AT)
  where RR implements IFC
{
  var localSelf = computeSelf(valSelf);
  var localAT   = computeAT(valAT);

  // (a) the actuals for out-formals are initialized local vars

  var initedSelf = computeSelf(valSelf);
  var initedAT   = computeAT(valAT);

  icInner(valSelf, localSelf, initedSelf,
          valAT,   localAT,   initedAT);

  // (b) the actuals for out-formals are split-initialized by the call

  var splitSelf, splitAT;

  icInner(valSelf, localSelf, splitSelf,
          valAT,   localAT,   splitAT);

  // (c) the actuals for out- and inout-formals are our own formals

  icInner(valSelf, inoutSelf, outSelf,
          valAT,   inoutAT,   outAT);
}

string implements IFC;
proc string.AT type return string;
proc computeSelf(arg: string): string return "self+" + arg;
proc computeAT(arg: string): string return "at+" + arg;

proc main {
  var inout1 = "inout1";
  var inout2 = "inout2";
  var out1;
  var out2 = "out2";

  icOuter("val1", inout1, out1,
          "val2", inout2, out2);

  writeln("| ", inout1, " | ", out1, " | ", inout2, " | ", out2, " |");
}
