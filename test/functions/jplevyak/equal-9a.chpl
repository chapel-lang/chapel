record foo { var a : int;  }
proc =(ref a : foo, b) {
  a.a = b.a + 10;
}
var x : foo;
var y : foo;
var z : foo;
x = new foo();
z = x;
y.a = 1;
x = y;
writeln(x.a);
writeln(z.a);
