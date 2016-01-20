proc foo(type t) {
  writeln(t:string);
  var X: t;
  writeln(X);
}

foo([1..3] [1..3] int);
