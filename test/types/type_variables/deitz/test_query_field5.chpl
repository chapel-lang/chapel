proc foo(x: ?t ...?k) {
  writeln(x);
  var y: t;
  writeln(y);
}

foo(1, 2, 3);
foo(1.0, 2.0, 3.0);
