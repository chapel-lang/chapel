class C {
  type t;
  var x : t;
}

def foo(c : C(?u)) {
  var y : u;
  writeln("foo of ", c, " with y = ", y);
}

foo(new C(int, 2));
