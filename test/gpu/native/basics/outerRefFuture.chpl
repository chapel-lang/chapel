config const n = 100;
on here.gpus[0] {
  var x = 10;
  var A: [1..n] int = 2;

  ref xRef = x;

  // we're not processing 'ref' intents of scalars on foreach loops for GPUs
  // properly.
  foreach a in A with (ref xRef) do xRef = a;

  writeln(x);
}
