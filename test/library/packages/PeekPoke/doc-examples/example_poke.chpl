//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//
/* START_EXAMPLE */
use BlockDist, PeekPoke;

const space = {1..1000};
const D = space dmapped new blockDist(space);
var A: [D] atomic int;

forall i in D do
  A[i].poke(i);

const sum = + reduce A.peek();
writeln(sum); // 500500
/* STOP_EXAMPLE */
