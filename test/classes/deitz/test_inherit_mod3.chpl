class C { }

class D1 : C {
  var x : int;
}

class D2 : C {
  var x : real;
}

def foo(c : C) {
  writeln(c.x);
}

var c : C;
c = new D1();
foo(c);
c = new D2();
foo(c);
