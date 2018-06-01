
config const n = 8;
const R = 1..n;
var A: [R] int = R;

proc main {
  writeln("before: ", A);
  forall idx in R with (
    ref tpv = A
  ) {
    tpv[idx] = n - idx;
  }
  writeln("after: ", A);
}
