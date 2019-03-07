class C {
  type t;
  var x : t;
}

proc foo(c : borrowed C(?u)) {
  var y : u;
  writeln("foo of ", c, " with y = ", y);
}

foo(new borrowed C(int, 2));
