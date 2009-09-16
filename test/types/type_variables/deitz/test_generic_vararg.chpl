class C {
  param k: int;
  var x: k*int;
}

def foo(y: C, x: C ...y.k) {
  writeln(x);
  delete y;
  delete x(1);
  delete x(2);
}

foo(new C(2), new C(1), new C(3));
