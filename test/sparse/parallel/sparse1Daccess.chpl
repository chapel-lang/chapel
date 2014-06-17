/*
 * This test shows off some of Chapel's initial support for sparse
 * domains and arrays.  These features are still under development,
 * so your feedback on them is particularly valuable.
 */

// Problem size;
config var n = 10;

// Dense bounding domains
const D1 = {1..n},
      D3 = {1..n, 1..n, 1..n};


var sparseVectSpace: sparse subdomain(D1);  // 1D sparse domain
var sparseVect: [sparseVectSpace] real;     // 1D sparse array (vector)

//
// we can do I/O more explicitly by iterating over the dense domain
//
forall i in D1 do
  write(" ", sparseVect(i));
writeln();
