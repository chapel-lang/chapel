class C {
  var x : integer = 1;
}

class D {
  var y : float = 2.0;
}

function foo(c : C) {
  writeln(c.x);
}

function foo(d : D) {
  writeln(d.y);
}

function bar(e) {
  foo(e);
}

var c : C = C(), d : D = D();

foo(c);
foo(d);

bar(c);
bar(d);
