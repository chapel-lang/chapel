class C {
  var x : int;
}

class D : C {
  param p : int;
  var y : real;
}

var d = new D(p=2);

writeln(d);

def foo(d : D) {
  for param i in 1..d.p do
    writeln(i);
}

foo(d);
