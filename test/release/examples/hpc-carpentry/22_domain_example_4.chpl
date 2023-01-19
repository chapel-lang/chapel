config const n = 8;
const mesh: domain(2) = {1..n, 1..n}; // a 2D domain defined in shared memory
                                      // on a single locale
var T: [mesh] real;  // a 2D array of reals defined in shared memory on a
                     // single locale (mapped onto this domain)
forall idx in T.domain {     // go in parallel through all n^2 elements of T
  writeln(idx, ' ', T(idx)); // idx is a tuple (i,j); also print corresponding
                             // array element
}
