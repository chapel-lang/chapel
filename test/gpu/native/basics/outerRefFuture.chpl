config const n = 100;
on here.gpus[0] {
  var x = 10;
  var A: [1..n] int = 2;

  ref xRef = x;

  // we're not processing 'ref' intents of scalars for GPUs properly.  We'd
  // expect this code to change x to 2. But as we offload x and use a GPU-only
  // ref in the kernel, you don't see any changes made to x.
  foreach a in A with (ref xRef) do xRef = a;

  writeln(x);
}
