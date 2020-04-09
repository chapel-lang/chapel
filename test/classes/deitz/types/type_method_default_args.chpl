class C {
  var x: int;
  proc foo(y: int = 2) type return (x + y).type;
}

var c = new unmanaged C();
var x: c.foo();
writeln(x);
delete c;
