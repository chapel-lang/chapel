class C {
  var x : int = 1;
}

class D : C {
  var y : real = 2.0;
}

def foo(c : C) {
  writeln(c);
}

var c : C;

c = new C();
foo(c);
c = new D();
foo(c);
