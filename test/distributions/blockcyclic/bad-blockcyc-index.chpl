use BlockCycDist;

// works
//var Space = { 1..8, 1..4, 1..9};
//var D_bs = Space dmapped BlockCyclic(startIdx=Space.low, blocksize=(2,2,3));

// doesn't work with -nl 1, -nl 2, -nl 8.  works with -nl 4.
var Space2 = { 1..8, 1..9, 1..4 };
var D_bs2 = Space2 dmapped BlockCyclic(startIdx=Space2.low, blocksize=(2,3,2));

proc show_array_locality(ref A: [?D]) {
  var L: [D] int;
  for ij in D {
    L[ij] = A[ij].locale.id;
  }
  writeln(L);
}

var A_bs2: [D_bs2] int;
show_array_locality(A_bs2);


