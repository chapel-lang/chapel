class C {
  var x : int = 1;
}

class D : C {
  var y : float = 2.0;
}

def foo(c : C) {
  writeln(c.x);
}

var c : C = C(), d : D = D();

writeln(c);
writeln(d);

foo(c);
foo(d);
