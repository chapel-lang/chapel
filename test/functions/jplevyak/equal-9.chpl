record foo { var a : integer;  }
function =(a : foo, b : foo) {
  a.a = b.a + 10;
}
var x : foo = foo();
var y : foo = foo();
var z : foo = x;
y.a = 1;
x = y;
writeln(x.a);
writeln(z.a);
