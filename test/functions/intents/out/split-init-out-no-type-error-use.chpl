
proc f(out arg: int) {
  arg = 12;
}

proc usearg(arg) { }

proc main() {
  var x;
  usearg(x);
  f(x);
}
