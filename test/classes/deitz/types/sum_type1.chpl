class C {
  var x : int = 1;
}

class D {
  var y : real = 2.0;
}

proc foo(c : C) {
  writeln(c.x);
}

proc foo(d : D) {
  writeln(d.y);
}

proc bar(e) {
  foo(e);
}

var c : C = new C(), d : D = new D();

foo(c);
foo(d);

bar(c);
bar(d);

delete c;
delete d;
