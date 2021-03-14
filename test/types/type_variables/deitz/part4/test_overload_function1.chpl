proc foo(x : int) {
  writeln(x);
}

proc foo(x : real) {
  writeln(x);
}

var i : int = 2;

foo(i);

var f : real = 2.3;

foo(f);
