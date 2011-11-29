proc foo(x : real, ys : string ...2) {
  writeln(x);
  writeln(ys(1));
  writeln(ys(2));
}

foo(2.0, "hello", "world!");
