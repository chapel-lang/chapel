class C {
  type t;
  var x: t;
}

var c1 = new borrowed C(int);
var c2 = new unmanaged C(int);

proc foo(c1: borrowed C, c2: borrowed C) {
  writeln(c1, c2);
}

foo(c1, c2);

delete c2;
