proc foo(x: ?t ...?k) {
  writeln(x);
  var y: t;
  writeln(y);
  var yy: t;
  writeln(yy);
  var yyy: t;
  writeln(yyy);
}

foo(1.0, 2, 3);
