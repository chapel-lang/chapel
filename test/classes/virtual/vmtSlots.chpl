// Exercises per-class virtual method tables across a 4-level hierarchy where
// methods are introduced at every level and some are overridden two levels
// down without being overridden in between. Every dynamic call is made
// through each ancestor's static type.

class L0 {
  proc m0() { writeln("L0.m0"); }
  proc shared0() { writeln("L0.shared0"); }
}

class L1 : L0 {
  override proc m0() { writeln("L1.m0"); }
  proc m1() { writeln("L1.m1"); }
  proc shared1() { writeln("L1.shared1"); }
}

class L2 : L1 {
  // does not override m0, shared0, shared1
  proc m2() { writeln("L2.m2"); }
  override proc m1() { writeln("L2.m1"); }
}

class L3 : L2 {
  override proc shared0() { writeln("L3.shared0"); } // skips L1, L2
  override proc shared1() { writeln("L3.shared1"); } // skips L2
  override proc m2() { writeln("L3.m2"); }
  proc m3() { writeln("L3.m3"); }
}

proc callAsL0(x: borrowed L0) {
  writeln("  as L0:");
  x.m0(); x.shared0();
}
proc callAsL1(x: borrowed L1) {
  writeln("  as L1:");
  x.m0(); x.shared0(); x.m1(); x.shared1();
}
proc callAsL2(x: borrowed L2) {
  writeln("  as L2:");
  x.m0(); x.shared0(); x.m1(); x.shared1(); x.m2();
}
proc callAsL3(x: borrowed L3) {
  writeln("  as L3:");
  x.m0(); x.shared0(); x.m1(); x.shared1(); x.m2(); x.m3();
}

var objs: [0..3] owned L0?;
objs[0] = new L0();
objs[1] = new L1();
objs[2] = new L2();
objs[3] = new L3();

for (o, i) in zip(objs, 0..) {
  writeln("object ", i);
  var x = o!.borrow();
  callAsL0(x);
  if var x1 = x : borrowed L1? then callAsL1(x1);
  if var x2 = x : borrowed L2? then callAsL2(x2);
  if var x3 = x : borrowed L3? then callAsL3(x3);
}
