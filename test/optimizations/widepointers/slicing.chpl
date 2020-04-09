proc main() {
  const n = 20;
  var A : [1..n] int;
  for a in A do a = 1; // for loops to keep generated C code easy to follow

  // Prior to 91638c0c slicing an array would cause all arrays with the same
  // type to be a wide pointer.
  for a in A[1..n/2] do a = 2;

  writeln("A is wide pointer? ", __primitive("is wide pointer", A._value));

  writeln("SUCCESS");
}
