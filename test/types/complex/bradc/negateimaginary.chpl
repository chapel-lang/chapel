proc foo(x: complex) {
  writeln("x is: ", x);
}


foo(-1.0);
foo(-1.0i);
foo(-1.0 + -1.0i);
foo(-1.0 - 1.0i);
foo(1.0i);
foo(1.0 + 1.0i);

var i = -1.0i;

writeln("i is: ", i);


