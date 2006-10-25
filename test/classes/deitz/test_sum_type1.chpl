class C {
  var x : int = 1;
}

class D {
  var y : real = 2.0;
}

def foo(c : C) {
  writeln(c.x);
}

def foo(d : D) {
  writeln(d.y);
}

def bar(e) {
  foo(e);
}

var c : C = C(), d : D = D();

foo(c);
foo(d);

bar(c);
bar(d);
