
proc f(out arg: int) {
  arg = 12;
}

proc main() {
  var x;
  f(x);
  writeln(x);
}
