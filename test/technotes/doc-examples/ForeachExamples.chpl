//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//

use BlockDist;

config const N = 8;

proc test0() {
  /* START_EXAMPLE_0 */
  var A: [1..N] int;
  foreach i in A.domain do
    A[i] += 1;
  /* STOP_EXAMPLE_0 */
  writeln(A);
}
test0();

proc test1() {
  /* START_EXAMPLE_1 */
  var D = {1..N} dmapped new blockDist({1..N});
  var A: [D] int;
  forall i in A.domain do
    A[i] += 1;
  /* STOP_EXAMPLE_1 */
  writeln(A);
}
test1();
