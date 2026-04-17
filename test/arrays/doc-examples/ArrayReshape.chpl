//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//

/* START_EXAMPLE */
var A = [1, 2, 3, 4];
ref B = reshape(A, 1..2, 1..2);
B[1,1] = 5;   // this will change the initial element of A
/* STOP_EXAMPLE */
writeln(A);
writeln(B);
assert(A[0] == 5);
