// In order for this test to succeed, default destructors for records must not
// destroy inherited fields more than once; i.e., the default destructor for
// a record must not destroy inherited fields.

const D = {1..4};

record R1 { var A: [D] int; }

record R2: R1 { var s: string; }

var A: [D] int = 13;

proc foo(x) {
  var r = new R2(A, x);
  writeln(r.s);
}

foo("Bonjour tout le monde!");
writeln(A);
