proc foo(a, (_, b, _), c, (d, (e, _))) {
  writeln((a, b, c, d, e));
}

proc main() {
  foo(1, (2, 3, 4), 5, (6, (7, 8)));
}
