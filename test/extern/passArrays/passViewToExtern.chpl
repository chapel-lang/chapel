require "passViewToExtern.h";

var A: [1..9] real = [1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0];
var B: [1..3, 1..3] real;

forall (i,j) in {1..3, 1..3} do
  B[i,j] = i + j / 10.0;

extern proc printThree(ptr: [] real);

printThree(A);
printThree(B);
printThree(A[3..]);
printThree(A.reindex(0..8));
printThree(B[3, ..]);
printThree(B[2..3, ..]);
printThree(B.reindex(0..2, 1..6 by 2));

// The following do _something_... but should they?  (they currently
// wrap around in memory rather than sticking to the pseudo-array's
// index set)
// Are the results right / appropriate / explainable?
// Should we be generating execution-time errors for these cases?
// Note that they behave the same as they did on main pre-array views.
printThree(B[2..3, 2..3]);
printThree(B[.., 2]);
