private proc foo(r: [0..1] range) {
  writeln("first");
  writeln(r);
}
private proc foo(r: int) {
  writeln("second");
  writeln(r);
}

foo([0..1, 2..3]);


private proc bar(r: [0..1] range(?)) {
  writeln("first");
  writeln(r);
}
private proc bar(r: int) {
  writeln("second");
  writeln(r);
}

bar([4..5, 6..8]);
