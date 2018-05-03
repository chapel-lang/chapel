class C {
  type t;
  var x : t;
}

proc foo(c : unmanaged C(?u)) {
  var y : u;
  writeln("foo of ", c, " with y = ", y);
  delete c;
}

foo(unmanaged new C(int, 2));
