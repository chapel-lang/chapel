class A {
  var rng: range;
}

proc main() {
  var a = new unmanaged A(0..4);

  writeln(a.rng);

  delete a;
}
