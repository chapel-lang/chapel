// a simple test of a forall loop with reduce intents

config const n = 10;

var tot: int;
forall a in 1..n with (+ reduce tot) do
  tot += a;

writeln(tot);
