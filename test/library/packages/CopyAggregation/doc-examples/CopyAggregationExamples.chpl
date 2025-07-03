//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//

/* START_EXAMPLE_0 */
use BlockDist, CopyAggregation;

const size = 10000;
const space = {0..size};
const D = space dmapped new blockDist(space);
var A, reversedA: [D] int = D;

forall (rA, i) in zip(reversedA, D) with (var agg = new SrcAggregator(int)) do
 agg.copy(rA, A[size-i]);

// no flush required, flushed when aggregator is deinitialized

forall (rA, i) in zip(reversedA, D) do
  assert(rA == size-i);
/* STOP_EXAMPLE_0 */

/* START_EXAMPLE_1 */
use CopyAggregation;

var a = 0;
on Locales[numLocales-1] {
 var agg = new DstAggregator(int);
 var b = 1;
 agg.copy(a, b);
 writeln(a);   // can print 0 or 1
 agg.flush();
 writeln(a);   // must print 1
}
/* STOP_EXAMPLE_1 */
