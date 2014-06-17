proc bar(x) return 12;

proc foo(x, y = bar(x)) {
  writeln(x, " ", y);
}

foo(12.0, 12.0);
