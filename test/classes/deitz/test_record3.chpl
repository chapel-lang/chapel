record foo {
  var i : integer;
  var f : float;
}

var x : foo = foo();
var y : foo = foo();

x.f = 2.2;

writeln("x: (", x.f, ")");

y.f = 2.3;

writeln("y: (", y.f, ")");

if x.f != y.f then
  writeln("x.f and y.f are not equal");
