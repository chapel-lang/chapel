class C {
  var x: int;
  proc foo(y: int = 2) do return x + y;
}

var c = new unmanaged C();
var x = c.foo();
writeln(x);
delete c;
