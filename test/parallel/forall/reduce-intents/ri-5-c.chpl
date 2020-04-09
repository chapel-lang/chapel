// a simple test of a forall loop with reduce intents

config const n = 10;

var tot = 0;
forall a in 1..n with (+ reduce tot) do
  tot += a;

writeln(tot);

// more variations on the topic

var D = {1..n};
var A: [D] int = D;
var tot1, tot2, tot3: int;
proc reset() { tot1 = 0; tot2 = 1; tot3 = 0; }
reset();

forall (a1,a2) in zip(1..n,1..n) with (+ reduce tot1, * reduce tot2) {
  tot1 += a1;
  tot2 *= a2;
}
writeln((tot1,tot2)); reset();

forall (a1,a2) in zip(1..n,D) with (+ reduce tot1, * reduce tot2) {
  tot1 += a1;
  tot2 *= a2;
}
writeln((tot1,tot2)); reset();

forall (a1,a2) in zip(1..n,A) with (+ reduce tot1, * reduce tot2) {
  tot1 += a1;
  tot2 *= a2;
}
writeln((tot1,tot2)); reset();

forall (a1,a2) in zip(D,1..n) with (+ reduce tot1, * reduce tot2) {
  tot1 += a1;
  tot2 *= a2;
}
writeln((tot1,tot2)); reset();

forall (a1,a2) in zip(A,1..n) with (+ reduce tot1, * reduce tot2) {
  tot1 += a1;
  tot2 *= a2;
}
writeln((tot1,tot2)); reset();

forall (a1,a2) in zip(A,D) with (+ reduce tot1, * reduce tot2) {
  tot1 += a1;
  tot2 *= a2;
}
writeln((tot1,tot2)); reset();

forall (a1,a2) in zip(D,A) with (+ reduce tot1, * reduce tot2) {
  tot1 += a1;
  tot2 *= a2;
}
writeln((tot1,tot2)); reset();

forall (a1,a2,a3) in zip(A,D,1..n)
  with (+ reduce tot1, * reduce tot2, | reduce tot3)
{
  tot1 += a1;
  tot2 *= a2;
  tot3 |= a3;
}
writeln((tot1,tot2,tot3)); reset();

forall (a1,a2,a3) in zip(D,1..n,A)
  with (+ reduce tot1, * reduce tot2, | reduce tot3)
{
  tot1 += a1;
  tot2 *= a2;
  tot3 |= a3;
}
writeln((tot1,tot2,tot3)); reset();

forall (a1,a2,a3) in zip(1..n,A,D)
  with (+ reduce tot1, * reduce tot2, | reduce tot3)
{
  tot1 += a1;
  tot2 *= a2;
  tot3 |= a3;
}
writeln((tot1,tot2,tot3)); reset();
