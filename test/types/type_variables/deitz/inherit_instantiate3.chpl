class C {
  var x : int;
}

class D : C {
  param p : int;
  var y : float;
}

var d = D(p=2);

writeln(d);

fun foo(d : D) {
  for param i in 1..d.p do
    writeln(i);
}

foo(d);
