record foo {
  var i : int;
  var f : real;
}

var x : foo = new foo();
var y : foo = new foo();

x.f = 2.2;

writeln("x: (", x.f, ")");

y.f = 2.3;

writeln("y: (", y.f, ")");

if x.f != y.f then
  writeln("x.f and y.f are not equal");
