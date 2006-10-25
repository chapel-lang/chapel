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
c = D1();
foo(c);
c = D2();
foo(c);
