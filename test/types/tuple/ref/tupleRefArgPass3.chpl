// Modify ref tuple arguments. Actual elements are reads of object fields.

record r { var x: int = 0; }
class C { var x: int = 0; }
record foo { var x: int = 0; var y: r; var z: shared C?; }

var g1 = new foo();

proc modifyRefArgConcrete(ref tup: (int, r, shared C?)) {
  tup[0] = 128;
  tup[1] = new r(128);
  tup[2] = new shared C(128);
}

proc modifyRefArgGeneric(ref tup) {
  tup[0] = 256;
  tup[1] = new r(256);
  tup[2] = new shared C(256);
}

// Actual elements are reads of object fields. Object is local. Modifying
// function is not generic.
proc test1() {
  writeln("test1");
  var f = new foo();
  writeln(f);
  modifyRefArgConcrete((f.x, f.y, f.z));
  writeln(f);
}
test1();

// Actual elements are reads of object fields. Object is local. Modifying
// function is generic.
proc test2() {
  writeln("test2");
  var f = new foo();
  writeln(f);
  modifyRefArgGeneric((f.x, f.y, f.z));
  writeln(f);
}
test2();

// Actual elements are reads of object fields. Object is global. Modifying
// function is concrete.
proc test3() {
  writeln("test3");
  writeln(g1);
  modifyRefArgConcrete((g1.x, g1.y, g1.z));
  writeln(g1);
}
test3();

g1 = new foo();

// Actual elements are reads of object fields. Object is global. Modifying
// function is generic.
proc test4() {
  writeln("test4");
  writeln(g1);
  modifyRefArgGeneric((g1.x, g1.y, g1.z));
  writeln(g1);
}
test4();

g1 = new foo();

// Actual elements are reads of object fields. Object is local. Modifying
// function is concrete. Actuals are read into a ref first.
proc test5() {
  writeln("test5");
  var f = new foo();
  writeln(f);
  ref tup = (f.x, f.y, f.z);
  modifyRefArgConcrete(tup);
  writeln(f);
}
test5();

// Actual elements are reads of object fields. Object is local. Modifying
// function is generic. Actuals are read into refs first.
proc test6() {
  writeln("test6");
  var f = new foo();
  writeln(f);
  ref tup = (f.x, f.y, f.z);
  modifyRefArgGeneric(tup);
  writeln(f);
}
test6();

