proc foo(type t) {
  writeln(typeToString(t));
  var X: t;
  writeln(X);
}

foo([1..3] [1..3] int);
