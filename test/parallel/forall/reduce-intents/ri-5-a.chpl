// a simple test of a forall loop with reduce intents

var A: [1..10] int = 1;

var tot: int;
forall a in A with (+ reduce tot) do
  tot += a;

writeln(tot);
