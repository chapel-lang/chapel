//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//
use LinearAlgebra;

/* START_EXAMPLE */
var A = Matrix(4, 4, eltType=int);
A = 1;

triu(A);
/* Returns:

    1    1    1    1
    0    1    1    1
    0    0    1    1
    0    0    0    1
*/

triu(A, 1);
/* Returns:

    0    1    1    1
    0    0    1    1
    0    0    0    1
    0    0    0    0
*/

triu(A, -1);
/* Returns:

    1    1    1    1
    1    1    1    1
    0    1    1    1
    0    0    1    1
*/
/* STOP_EXAMPLE */

writeln(triu(A), "\n");
writeln(triu(A, 1), "\n");
writeln(triu(A, -1));
