//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//
use LinearAlgebra;

/* START_EXAMPLE */
var A = Matrix(4, 4, eltType=int);
A = 1;

tril(A);
/* Returns:

    1    0    0    0
    1    1    0    0
    1    1    1    0
    1    1    1    1
*/

tril(A, 1);
/* Returns:

    1    1    0    0
    1    1    1    0
    1    1    1    1
    1    1    1    1
*/

tril(A, -1);
/* Returns:

    0    0    0    0
    1    0    0    0
    1    1    0    0
    1    1    1    0
*/
/* STOP_EXAMPLE */

writeln(tril(A), "\n");
writeln(tril(A, 1), "\n");
writeln(tril(A, -1));
