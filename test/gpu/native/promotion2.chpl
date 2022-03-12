config const n = 10;

config const alpha = 10;

on here.getChild(1) {
  var A: [1..n] int;
  var B: [1..n] int;
  var C: [1..n] int;

  A = 1;
  B = 2;
  C = 3;

  A = B + alpha * C;

  // normally we can just do writeln(A), but we don't have a good way of having
  // verbose GPU execution. So, if we do that the output is just too messy with
  // verbose. If we had verbosity for only launches, things would have been much
  // easier.
  for a in A do
    write(a, " ");
  writeln();
}
