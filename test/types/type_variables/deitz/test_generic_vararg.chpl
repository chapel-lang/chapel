class C {
  param k: int;
  var x: k*int;
}

def foo(y: C, x: C ...y.k) {
  writeln(x);
}

foo(C(2), C(1), C(3));
