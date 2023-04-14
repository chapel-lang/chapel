class A {
  var rng: simpleRange; /*autofix*/
}

proc main() {
  var a = new unmanaged A(0..4);

  writeln(a.rng);

  delete a;
}
