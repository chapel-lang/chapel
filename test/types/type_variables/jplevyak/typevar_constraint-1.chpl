function foo(type t : A, a : t) {
  writeln("foo1 ", a.x);
}

function foo(type t : C, c : t) {
  writeln("foo2 ", c.x);
}

class A { 
  var x : integer;
}

class B : A { 
  var y : integer;
}

class C { 
  var x : float;
}

class D : C { 
  var y : float;
}

var b : B(x = 1, y = 2);
var d : D(x = 3.0, y = 4.0);

foo(B, b);
foo(D, d);
