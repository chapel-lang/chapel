class C {
  param p : int;
  var x : real;
}

class D : C {
  var y : int;
}

var d = new unmanaged D(p=2);

writeln(d);
writeln("p=", d.p);

proc foo(c : unmanaged C) {
  for param i in 1..c.p do
    writeln(i);
}

foo(d);

delete d;
