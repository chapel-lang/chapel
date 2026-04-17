//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//

/* START_EXAMPLE */
use CyclicDist;

const Dist = new cyclicDist(startIdx=(1,1));
const D = Dist.createDomain({1..8, 1..8});
var A: [D] int;

forall a in A do
  a = here.id;

writeln(A);
/* STOP_EXAMPLE */
