//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//

{
/* START_EXAMPLE_0 */
var A: [1..10] int;
const D = {6..15};
ref reA = A.reindex(D);
reA[6] = 1; // updates A[1]
/* STOP_EXAMPLE_0 */

assert(A[1] == 1);
}



{
/* START_EXAMPLE_1 */
var A: [3..4, 5..6] int;
ref reA = A.reindex(13..14, 15..16);
reA[13,15] = 1; // updates A[3,5]
/* STOP_EXAMPLE_1 */

assert(A[3,5] == 1);
}
