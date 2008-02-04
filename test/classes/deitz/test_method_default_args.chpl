class C {
  var x: int;
  def foo(y: int = 2) return x + y;
}

var c = new C();
var x = c.foo();
writeln(x);
