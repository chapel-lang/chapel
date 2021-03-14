// a simple test of two forall loops with reduce intents

const D = {1..10};

var A: [D] int = 1;

var tot: int;
forall i in D with (+ reduce tot) do
  tot += i;

var tot2: int;
forall a in A with (+ reduce tot2) do
  tot2 += a;

writeln((tot,tot2));
