//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//

/* START_EXAMPLE */
use BlockCycDist;

const Space = {1..8, 1..8};
const D: domain(2)
  dmapped new blockCycDist(startIdx=Space.lowBound,blocksize=(2,3))
  = Space;
var A: [D] int;

forall a in A do
  a = a.locale.id;

writeln(A);
/* STOP_EXAMPLE */
