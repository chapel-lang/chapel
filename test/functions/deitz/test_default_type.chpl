fun bar(x) return 12;

fun foo(x, y = bar(x)) {
  writeln(x, " ", y);
}

foo(12.0, 12.0);
