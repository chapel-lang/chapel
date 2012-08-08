proc foo() {
  return (1, 2.3, "hi");
}

var (x, y, z) = foo();

writeln("x is: ", x);
writeln("y is: ", y);
writeln("z is: ", z);

var (a, _, _) = foo();
var (_, b, _) = foo();
var (_, _, c) = foo();
var (d, e, _) = foo();
var (f, _, g) = foo();
var (_, h, i) = foo();

writeln("a is: ", a);
writeln("b is: ", b);
writeln("c is: ", c);
writeln("d is: ", d);
writeln("e is: ", e);
writeln("f is: ", f);
writeln("g is: ", g);
writeln("h is: ", h);
writeln("i is: ", i);
