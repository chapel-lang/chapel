class C {
  var x: int;
  proc foo(y: int = 2) return x + y;
}

var c = new C();
var x = c.foo();
writeln(x);
delete c;
