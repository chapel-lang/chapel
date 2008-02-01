class C {
  var x: int;
  def foo(y: int = 2) type return x + y;
}

var c = C();
var x: c.foo();
writeln(x);
