
proc main() {
  var A : [1..10] int = 1;
  var B => A;
  B[1] = 42;

  writeln("A = ", A);
  writeln("B = ", B);
}
