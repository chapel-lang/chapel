class C {
  type t;
  var x : t;
}

fun foo(c : C(?u)) {
  var y : u;
  writeln("foo of ", c, " with y = ", y);
}

foo(C(int, 2));
