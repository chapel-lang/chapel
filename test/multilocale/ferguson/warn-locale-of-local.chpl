use BlockDist;

var space = {0..#8};
var dom = space dmapped new blockDist(boundingBox=space);
var A: [dom] int;

config const verbose = false;

if verbose {
  writeln(A[5].locale.id);
  writeln(A[2..3].locale.id);
}

on A[2..3] {
  // User might expect 1 (for numLocales = 4)
  if verbose {
    writeln(here.id); // prints 0
  }
}
