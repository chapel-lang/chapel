proc foo(type t, a : t) where isSubtype(t,A) {
  writeln("foo1 ", a.x);
}

proc foo(type t, c : t) where isSubtype(t,C) {
  writeln("foo2 ", c.x);
}

class A {
  var x : int;
}

class B : A {
  var y : int;
}

class C {
  var x : real;
}

class D : C {
  var y : real;
}

var b = new unmanaged B(x = 1, y = 2);
var d = new unmanaged D(x = 3.0, y = 4.0);

foo(B, b);
foo(D, d);

delete d;
delete b;
