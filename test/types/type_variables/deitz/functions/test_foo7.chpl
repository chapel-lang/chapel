class C {
  type t;
  var x : t;
}

proc foo(c : borrowed C(?u)) {
  var y : u;
  writeln("foo of ", c, " with y = ", y);
}

foo((new owned C(int, 2)).borrow());
