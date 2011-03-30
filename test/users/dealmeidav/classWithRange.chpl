class A {
  var rng: range;
}

proc main() {
  var a = new A(0..4);
  writeln(a.rng);
}
