proc foo(x: ?t ...?k) {
  writeln(x);
  var y: t(1);
  writeln(y);
  var yy: t(2);
  writeln(yy);
  var yyy: t(3);
  writeln(yyy);
}

foo(1.0, 2, 3);
