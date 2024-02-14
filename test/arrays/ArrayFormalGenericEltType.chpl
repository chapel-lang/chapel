record R {
  type T;
  var x: T;
}

proc jvp(x: [] R) {
  writeln("in jvp");
  writeln(x);
}

proc testReproducer() {
  var A: [1..10] R(real);
  jvp(A);
}

proc main() {
  testReproducer();
}
