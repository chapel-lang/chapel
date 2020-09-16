// Test ref to local value tuple and ref to global value tuple.
record r { var x: int = 0; }

var g1 = (0, new r());

proc test1() {
  var tup = (0, new r());

  writeln(tup);

  ref foo = tup;
  foo[0] = 128;
  foo[1] = new r(128);

  writeln(tup);
}
test1();

proc test2() {
  writeln(g1);

  ref foo = g1;
  foo[0] = 128;
  foo[1] = new r(128);

  writeln(g1);
}
test2();

