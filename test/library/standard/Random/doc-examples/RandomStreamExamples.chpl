//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//

use Random;

const n = 3;

{
/* START_EXAMPLE_0 */
var A: [1..n] int;
var rs = new randomStream(int);

forall (r, a) in zip(rs.next(A.domain), A) do a = r;
/* STOP_EXAMPLE_0 */
}

{
/* START_EXAMPLE_1 */
var A: [1..n] int;

forall a in A with (var rs = new randomStream(int)) do a = rs.next();
/* STOP_EXAMPLE_1 */
}
