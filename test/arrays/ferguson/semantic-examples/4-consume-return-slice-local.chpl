use samples;
proc f() {
  var A:[1..4] R;
  return A[2..3];
}

proc consumesArray(A:[] R) {
  writeln(A[2].x);
}
consumesArray(f());

