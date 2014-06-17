class C {
  param p : int;
  var x : real;
}

class D : C {
  var y : int;
}

var d = new D(p=2);

writeln(d);

proc foo(c : C) {
  for param i in 1..c.p do
    writeln(i);
}

foo(d);
