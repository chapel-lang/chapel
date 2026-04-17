//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//

/* START_EXAMPLE_0 */
config const n = 8;
var D = {1..n};
var A: [D] real = noinit;  // A's elements won't be initialized here

// Reading one of A's elements here is a memory error -
// it will read uninitialized memory.

// A's elements can be initialized using the assignment operator
forall a in A do
  a = 0.0;

// Now it is safe to read from A.
writeln(A);
/* STOP_EXAMPLE_0 */
