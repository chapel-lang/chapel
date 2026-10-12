
proc double(x: int) {
  return x*2;
}

proc generate(rng, fn) {
  var A : [1..10] int = 1..10;
  return fn(A);
}

proc main() {
  writeln(generate(1..10, double));
}
