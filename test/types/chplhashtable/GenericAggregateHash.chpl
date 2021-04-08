class C1 {
  type T;
  var x: T;
}

class C2: C1 {
  type T;
  var y: T;
}

record r1 {
	type T;
  var x: T;
}

record r2 {
  type T;
  var a: [0..3] T;
}

record r3 {
  type T;
  param flag: bool;
}

record r4 {}

operator r1.==(lhs: r1, rhs: r1) {
  return lhs.x == rhs.x;
}

operator r2.==(lhs: r2, rhs: r2) {
  return && reduce (lhs.a == rhs.a);
}

class C { var x = 0; }

proc test1() {
  var d: domain(r1(int));
  d += new r1(int);
  writeln(d);
}
test1();

proc test2() {
  var d: domain(r2(int));
  d += new r2(int);
  writeln(d);
}
test2();

// Tests for classes are actually irrelevant here, but...
proc test3() {
  var d: domain(shared C1(int));
  d += new shared C1(int);
  writeln(d);
}
test3();

proc test4() {
  var d: domain(shared C2(int));
  d += new shared C2(int, 0, int, 0);
  writeln(d);
}
test4();

proc test5() {
  var d: domain(shared C1(int));
  d += new shared C2(int, 0, int, 0);
  writeln(d);
}
test5();

// Test record with only type/param fields (hash should return 0).
proc test6() {
  var d: domain(r3(int, flag=false));
  d += new r3(int, flag=false);
  writeln(d);
}
test6();

// Test empty record (hash should return 0).
proc test7() {
  var d: domain(r4);
  d += new r4();
  writeln(d);
}
test7();

