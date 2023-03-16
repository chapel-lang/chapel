class C {
  type t;
  var x: t;
}

var ownC1 = new owned C(int);
var c1 = ownC1.borrow();
var c2 = new unmanaged C(int);

proc foo(c1: borrowed C, c2: borrowed C) {
  writeln(c1, c2);
}

foo(c1, c2);

delete c2;
