def foo() {
  return (1, 2.3, "hi");
}

var (x, y, z) = foo();

writeln("x is: ", x);
writeln("y is: ", y);
writeln("z is: ", z);

var (a, , ) = foo();
var (, b, ) = foo();
var (, , c) = foo();
var (d, e, ) = foo();
var (f, , g) = foo();
var (, h, i) = foo();

writeln("a is: ", a);
writeln("b is: ", b);
writeln("c is: ", c);
writeln("d is: ", d);
writeln("e is: ", e);
writeln("f is: ", f);
writeln("g is: ", g);
writeln("h is: ", h);
writeln("i is: ", i);
