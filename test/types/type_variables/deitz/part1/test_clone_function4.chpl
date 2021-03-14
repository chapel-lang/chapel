proc filler(x : int) {
  writeln(x);
}

proc foo(x) {
  writeln(x);
}

var i : int = 2;

filler(i);

foo(i);

var f : real = 2.3;

foo(f);
