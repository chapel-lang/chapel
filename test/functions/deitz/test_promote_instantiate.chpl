proc foo(i: integral, j: int) {
  writeln((i,j));
}

serial true {
  foo(1..3, 4..6);
}
