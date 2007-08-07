class C {
  type t;
  var x: t;
}

var c1 = C(int);
var c2 = C(int);

def foo(c1: C, c2: C) {
  writeln(c1, c2);
}

foo(c1, c2);
