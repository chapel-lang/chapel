proc foo(x) where (x.type == int) | (x.type == real) {
  writeln("int or real = ", x);
}

proc foo(x) where (x.type == complex) | (x.type == bool) {
  writeln("complex or bool = ", x);
}

foo(1);
foo(2.0);
foo(3.0+2.0i);
foo(false);
