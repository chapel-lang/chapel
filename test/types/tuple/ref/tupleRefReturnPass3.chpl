// Return a tuple of references to object fields and destructure it.

record r { var x: int = 0; var y: int = 0; }

proc r.refTupleOfFieldsInfer() ref {
  return (x, y);
}

proc r.refTupleOfFieldsSpecify() ref: (int, int) {
  return (x, y);
}

// Return type is inferred.
proc test1() {
  var foo = new r();
  writeln(foo);
  ref (f1, f2) = foo.refTupleOfFieldsInfer();
  f1 = 128;
  f2 = 256;
  writeln(foo);
}
test1();

// Return type is specified.
proc test2() {
  var foo = new r();
  writeln(foo);
  ref (f1, f2) = foo.refTupleOfFieldsSpecify();
  f1 = 128;
  f2 = 256;
  writeln(foo);
}
test2();

