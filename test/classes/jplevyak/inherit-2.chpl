class C {
  var c;
  function f() { return c * 2; }
}
function ff(a : C) { return a.c * 2; }

class D : C {
  var d;
  function g() { return c * d; }
  function f() { return c * 3; }
}
function ff(a : D) { return ff(a:C) * 2; }

var x = D(c = 1, d = 2.0);
var y = D(c = 3.0, d = 4);
writeln(x.c + y.d);
writeln(x.d + y.c);
writeln(x.f());
writeln(y.g());
writeln(ff(x));
writeln(ff(x:C));
