proc bar(x1 : int, x2 : int, x3 : int) {
  writeln(x1, ", ", x2, ", ", x3);
}

proc foo(x : int ...3) {
  bar(x(0), x(1), x(2));
  bar((...x));
}

foo(1, 2, 3);
