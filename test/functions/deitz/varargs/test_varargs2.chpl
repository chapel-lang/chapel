proc foo(ys : string ...2) {
  writeln(ys(1));
  writeln(ys(2));
}

foo("hello", "world!");
