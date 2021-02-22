proc initViaOut(out arg: ?T) {
  // due to the type query, the type of arg is
  // inferred from the call site, and as a result
  // arg can be default initialized.
  writeln(arg);
}

proc main() {
  var x = 1;
  initViaOut(x);
}
