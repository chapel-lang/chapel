
use StencilDist;

config const n = 10;

proc main() {
  var D = {1..n, 1..n};
  var Space = D dmapped Stencil(D, fluff=(1,1,));
  var A : [Space] int;

  var c = 1;
  for a in A {
    a = c;
    c += 1;
  }


  // Incorrectly prints out cached values.
  writeln(A[D]);
}
