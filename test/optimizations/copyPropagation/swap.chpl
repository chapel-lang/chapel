
config const n = 5;

proc main() {
  var A = n * 2;
  var B = -n;

  A <=> B;
  // Swap turned into:
  // var temp = A;
  // A = B;
  // B = temp;
  //
  // copyPropagation should not propagate 'temp' such that 'B = A'.

  writeln("A = ", A);
  writeln("B = ", B);
}
