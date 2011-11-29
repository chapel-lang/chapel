proc foo(x : ?t) {
  var y : t;
  writeln("foo of ", x, " with y = ", y);
}

foo(2);
foo(2.0);
