
proc main() {
  var D = {1..10};
  var A : [D] int = D;

  writeln(A);

  var RD = D by 1;
  ref B = A[RD];

  for b in B do writeln(b);
}
