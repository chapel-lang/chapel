proc foo(x : real, ys : string ...2) {
  writeln(x);
  writeln(ys(0));
  writeln(ys(1));
}

foo(2.0, "hello", "world!");
