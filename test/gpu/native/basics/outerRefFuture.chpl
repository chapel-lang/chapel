config const n = 100;
on here.gpus[0] {
  var x = 10;
  var A: [1..n] int = 2;

  ref xRef = x;

  // as of today, we don't have a good implementation for shadow
  // variables/intents for foreach. But this test exposes a case where the
  // behavior is wrong. We'd expect this code to change x to 2. But as we
  // offload x and use a GPU-only ref in the kernel, you don't see any changes
  // made to x.
  foreach a in A do xRef = a;

  writeln(x);
}
