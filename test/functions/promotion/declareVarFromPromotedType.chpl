proc foo(type t) {
  var x: t;
  writeln(x);
}

proc bar(x) {
  foo(x.type);
}

bar([i in 1..10] i);
