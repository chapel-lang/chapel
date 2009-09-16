class C {
  type t;
  var x : t;
}

def foo(c : C(?u)) {
  var y : u;
  writeln("foo of ", c, " with y = ", y);
  delete c;
}

foo(new C(int, 2));
