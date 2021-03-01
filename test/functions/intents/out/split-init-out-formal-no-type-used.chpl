
proc f(out arg) {
  usearg(arg);
}

proc usearg(arg) { }

proc main() {
  var x;
  f(x);
}
