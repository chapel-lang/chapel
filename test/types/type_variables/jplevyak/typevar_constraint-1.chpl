fun foo(type t, a : t) where t:A {
  writeln("foo1 ", a.x);
}

fun foo(type t, c : t) where t:C {
  writeln("foo2 ", c.x);
}

class A { 
  var x : int;
}

class B : A { 
  var y : int;
}

class C { 
  var x : float;
}

class D : C { 
  var y : float;
}

var b = B(x = 1, y = 2);
var d = D(x = 3.0, y = 4.0);

foo(B, b);
foo(D, d);
