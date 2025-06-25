//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//

/* START_EXAMPLE */
use BlockDist;

const Space = {1..8, 1..8};
const Dist = new blockDist(boundingBox=Space);
const D = Dist.createDomain(Space);
var A: [D] int;

forall a in A do
  a = here.id;

writeln(A);
/* STOP_EXAMPLE */
