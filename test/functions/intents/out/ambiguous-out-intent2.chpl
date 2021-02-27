proc f(out arg: int) {
  writeln("in f(int)");
}

proc f(out arg: real) {
  writeln("in f(real)");
}

proc main() {
  var x: int;
  f(x);
}
