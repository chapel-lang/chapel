//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//

use Random;

/* START_EXAMPLE_0 */
var rs = new randomStream(int),
    A: [1..1000] int;

forall (a, r) in zip(A, rs.next(A.domain)) do
  a = r;
/* STOP_EXAMPLE_0 */
