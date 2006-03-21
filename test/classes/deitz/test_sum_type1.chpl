class C {
  var x : int = 1;
}

class D {
  var y : float = 2.0;
}

fun foo(c : C) {
  writeln(c.x);
}

fun foo(d : D) {
  writeln(d.y);
}

fun bar(e) {
  foo(e);
}

var c : C = C(), d : D = D();

foo(c);
foo(d);

bar(c);
bar(d);
