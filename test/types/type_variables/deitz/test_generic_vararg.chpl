class C {
  param k: int;
  var x: k*int;
}

proc foo(y: borrowed C, x: borrowed C ...y.k) {
  writeln(x);
}

foo((new owned C(2), new borrowed C(1), new borrowed C(3))).borrow();
