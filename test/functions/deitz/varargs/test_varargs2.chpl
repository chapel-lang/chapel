proc foo(ys : string ...2) {
  writeln(ys(0));
  writeln(ys(1));
}

foo("hello", "world!");
