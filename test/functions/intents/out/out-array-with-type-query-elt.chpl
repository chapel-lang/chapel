proc initViaOut(out arg: [] ?eltType) {
  // due to the type query, the type of arg is
  // inferred from the call site, and as a result
  // arg can be default initialized.
  writeln(arg);
}

proc main() {
  var A:[1..10] int;
  initViaOut(A);
}
