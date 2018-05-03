class C {
  param k: int;
  var x: k*int;
}

proc foo(y: unmanaged C, x: unmanaged C ...y.k) {
  writeln(x);
  delete y;
  delete x(1);
  delete x(2);
}

foo(new unmanaged C(2), new unmanaged C(1), new unmanaged C(3));
