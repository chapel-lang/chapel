class C {
  type t;
  var x : t;
}

function foo(c : C(?u)) {
  var y : u;
  writeln("foo of ", c, " with y = ", y);
}

foo(C(integer, 2));
