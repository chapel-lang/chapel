//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//

use StencilDist;

// These variables mimic the formals in the 'stencilDist.init(...)'.
const boundingBox: domain(?) = {0..7};
param rank = boundingBox.rank;
type idxType = boundingBox.idxType;
const fluff: rank*idxType;
const i = 0;
/* START_EXAMPLE_0 */
boundingBox.dim(i).expand(fluff(i));
/* STOP_EXAMPLE_0 */

/* START_EXAMPLE_1 */
const bb = boundingBox.dim(i);
bb.expand(fluff(i) * abs(bb.stride));
/* STOP_EXAMPLE_1 */

/* START_EXAMPLE_2 */
use StencilDist;

var BlockDom1 = stencilDist.createDomain({1..5, 1..5});
var BlockArr1 = stencilDist.createArray({1..5, 1..5}, real);
var BlockDom2 = stencilDist.createDomain(1..5, 1..5);
var BlockArr2 = stencilDist.createArray(1..5, 1..5, real);
/* STOP_EXAMPLE_2 */
assert(BlockDom1 == BlockDom2);
assert(BlockArr1 == BlockArr2);

/* START_EXAMPLE_3 */
use StencilDist;

const Space = {1..10, 1..10};
const Dist = new stencilDist(boundingBox=Space, fluff=(1,1));
const D = Dist.createDomain(Space);
var A : [D] int;

forall (i,j) in D with (ref A) do
  A[i,j] = i*10 + j;

// At this point, the ghost cell caches are out of date

A.updateFluff();

// ghost cell caches are now up-to-date
/* STOP_EXAMPLE_3 */

writeln(A);
