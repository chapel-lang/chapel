// This test checks whether the following warnings are generated:
// - "using the old slicing rule"
// - "creating arrays/slices with negative strides"
//
// See also: test/arrays/sungeun/warn_reindex1.chpl

config const verbose = false;

var A: [1..3] int;
var A2: [1..3, 1..3] int;
if verbose then writeln(A, A2);

ref B = A[2..3 by -1];        // warning
var BB = B;                   // warning
if verbose then writeln(B, BB);

var CD = {2..3 by -1};
ref C = A[CD];                // warning; note that ranges are not sliced here
var CC = C;                   // warning
if verbose then writeln(C, CC);

var CD2 = {2..3, 2..3 by -1};
ref C2 = A2[CD2];             // warning; note that ranges are not sliced here
var CC2 = C2;                 // warning
if verbose then writeln(C2, CC2);

var D2: [1..5, 2..5] int;
ref D1 = D2[2, 3..4 by -1];   // warning
var DD1 = D1;                 // warning
if verbose then writeln(D1, DD1);

writeln("done");
