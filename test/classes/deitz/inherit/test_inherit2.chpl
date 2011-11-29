class C {
  var x : int = 1;
}

class D : C {
  var y : real = 2.0;
}

proc foo(c : C) {
  writeln(c.x);
}

var c : C = new C(), d : D = new D();

writeln(c);
writeln(d);

foo(c);
foo(d);

delete c;
delete d;
