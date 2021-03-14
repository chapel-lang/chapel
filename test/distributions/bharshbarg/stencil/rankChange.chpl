use StencilDist;

config const n = 10;

proc main() {
  var Dom = {1..n,1..n};
  var Space = Dom dmapped Stencil(Dom, fluff=(2,3));

  for (a, b) in zip(Dom[1,..], Space[1,..]) {
    assert(a == b);
  }
  var A : [Space] int;
  A = 9;
  A.updateFluff();

  ref first = A[1,..];
  first = 0;
  A.updateFluff();

  for a in A[1,..] do assert(a == 0);
  for a in A[n,..] do assert(a == 9);
  writeln("SUCCESS!");
}
