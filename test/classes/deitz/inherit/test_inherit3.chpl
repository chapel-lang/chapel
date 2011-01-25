class C {
  var x : int = 1;
}

class D : C {
  var y : real = 2.0;
}

proc foo(c : C) {
  writeln(c.x);
}

var c : C;

c = new C();
foo(c);
delete c;
c = new D();
foo(c);
delete c;
