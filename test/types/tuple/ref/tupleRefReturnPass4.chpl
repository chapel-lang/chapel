// Return a tuple of references to object fields and store it in a ref.

record r { var x: int = 0; var y: int = 0; }

proc r.refToFields1() ref {
  return (x, y);
}

proc r.refToFields2() ref: (int, int) {
  return (x, y);
}

var g1 = new r();

// Return type is inferred.
proc test1() {
  var foo = new r();
  writeln(foo);
  ref tup = foo.refToFields1();
  tup[0] = 128;
  tup[1] = 256;
  writeln(foo);
}
test1();

// Return type is specified.
proc test2() {
  var foo = new r();
  writeln(foo);
  ref tup = foo.refToFields2();
  tup[0] = 128;
  tup[1] = 256;
  writeln(foo);
}
test2();

// Return type is inferred, using global.
proc test3() {
  writeln(g1);
  ref tup = g1.refToFields1();
  tup[0] = 128;
  tup[1] = 256;
  writeln(g1);
}
test3();

g1 = new r();

proc test4() {
  writeln(g1);
  ref tup = g1.refToFields2();
  tup[0] = 128;
  tup[1] = 256;
  writeln(g1);
}
test4();

