//
// This examples demonstrates uses of the Block and Cyclic
// distributions.  To use these distributions in a Chapel program, we
// first must use their modules.
//
use BlockDist, CyclicDist;

//
// Make the program size configurable from the command line.
//
config const n = 8;

//
// Declare a 2-dimensional domain Space that we will later use to
// initialize the Block and Cyclic domains.
//
const Space = [1..n, 1..n];

//
// Declare a 2-dimensional Block-distributed domain BlockSpace and a
// Block-distributed array BA declared over BlockSpace.  The Block
// distribution is defined by the boundingBox specification of Space.
//
const BlockSpace: domain(2) dmapped Block(boundingBox=Space) = Space;
var BA: [BlockSpace] int;

//
// Iterate in parallel over the Block-distributed array and assign the
// ID of the locale on which each element exists to that element.
//
forall ba in BA do
  ba = ba.locale.id;

//
// Output the Block-distributed array to visually see how the elements
// are partitioned across the locales.
//
writeln("Block Array Index Map");
writeln(BA);
writeln();

//
// Repeat the above code with a Cyclic-distributed array to visually
// see how the elements in a Cyclic-distributed array are partitioned
// across the locales.
//
const CyclicSpace: domain(2) dmapped Cyclic(startIdx=Space.low) = Space;
var CA: [CyclicSpace] int;

forall ca in CA do
  ca = ca.locale.id;

writeln("Cyclic Array Index Map");
writeln(CA);
writeln();
