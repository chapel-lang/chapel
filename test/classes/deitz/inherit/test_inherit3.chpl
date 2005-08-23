class C {
  var x : integer = 1;
}

class D : C {
  var y : float = 2.0;
}

function foo(c : C) {
  writeln(c.x);
}

var c : C;

c = C();
foo(c);
c = D();
foo(c);
