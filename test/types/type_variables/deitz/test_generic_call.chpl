class C {
  type t;
  var x: t;
}

var c1 = new C(int);
var c2 = new C(int);

proc foo(c1: C, c2: C) {
  writeln(c1, c2);
}

foo(c1, c2);

delete c1;
delete c2;
