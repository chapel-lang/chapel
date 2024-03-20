record CR { var x: int; }
record GR { type t; var x: t; }
record GRD { type t=int; var x: t; }
class CC { var x: int; }
class GC { type t; var x: t; }

proc crNoQ(): CR { // OK
  return new CR(1);
}
writeln(crNoQ());


proc grNoQ(): GR { // expect a warning
  return new GR(int, 1);
}
writeln(grNoQ());

proc grQ(): GR(?) { // OK
  return new GR(int, 1);
}
writeln(grQ());


proc grdNoQ(): GRD { // OK
  return new GRD(int, 1);
}
writeln(grdNoQ());

proc grdQ(): GRD(?) { // OK
  return new GRD(int, 1);
}
writeln(grdQ());


proc ccNoQ(): CC { // OK
  return new CC(1);
}
writeln(ccNoQ());


proc gcNoQ(): GC { // expect a warning
  return new GC(int, 1);
}
writeln(gcNoQ());

proc gcQ(): GC(?) { // OK
  return new GC(int, 1);
}
writeln(gcQ());

