//
// This examples demonstrates uses of some of Chapel's standard
// distributions.  To use these distributions in a Chapel program,
// the respective module must be used:
//
use BlockDist, CyclicDist, BlockCycDist;

//
// For each distribution, we'll create a distributed domain and array
// and then initialize it just to give a brief flavor of how the
// distribution maps across locales.  Running this example on 6
// locales does a nice job of illustrating the distribution
// characteristics.
//
// All of these distributions support options to map to a different
// virtual locale grid than the one used by default (a
// multidimensional factoring of the built-in Locales array), as well
// as to control the amount of parallelism used in data parallel
// loops.  See the Standard Distributions chapter of the language spec
// for more details.
//

//
// Make the program size configurable from the command line.
//
config const n = 8;

//
// Declare a 2-dimensional domain Space that we will later use to
// initialize the distributed domains.
//
const Space = [1..n, 1..n];

//
// The Block distribution distributes a bounding box from
// n-dimensional space across the target locale array viewed as an
// n-dimensional virtual locale grid.  The bounding box is blocked
// into roughly equal portions across the locales.  Note that domains
// declared over a Block distribution can also store indices outside
// of the bounding box; the bounding box is merely used to compute
// the blocking of space.
//
// In this example, we declare a 2-dimensional Block-distributed
// domain BlockSpace and a Block-distributed array BA declared over
// the domain.
//
const BlockSpace = Space dmapped Block(boundingBox=Space);
var BA: [BlockSpace] int;

//
// To illustrate how the index set is distributed across locales,
// we'll use a forall loop to initialize each array element to the
// locale ID that stores that index/element/iteration.
//
forall ba in BA do
  ba = here.id;

//
// Output the Block-distributed array to visually see how the elements
// are partitioned across the locales.
//
writeln("Block Array Index Map");
writeln(BA);
writeln();

//
// Next, we'll do the same thing with a Cyclic-distributed array.
// Cyclic distributions start at a designated n-dimensional index and
// distribute the n-dimensional space across an n-dimensional array
// of locales in a round-robin fashion (in each dimension).  As with
// the Block distribution, domains may be declared using the
// distribution who have lower indices that the starting index; that
// value should just be considered a parameterization of how the
// distribution is defined.
//
const CyclicSpace = Space dmapped Cyclic(startIdx=Space.low);
var CA: [CyclicSpace] int;

forall ca in CA do
  ca = here.id;

writeln("Cyclic Array Index Map");
writeln(CA);
writeln();


//
// Next, we'll declare a Block-Cyclic distribution.  These
// distributions also deal out indices in a round-robin fashion,
// but rather than dealing out singleton indices, they deal out blocks
// of indices.  Thus, the BlockCyclic distribution is parameterized
// by a starting index (as with Cyclic) and a block size (per
// dimension) specifying how large the chunks to be dealt out are.
// 
const BlkCycSpace = Space dmapped BlockCyclic(startIdx=Space.low, 
                                              blocksize=(2, 3));
var BCA: [BlkCycSpace] int;

forall bca in BCA do
  bca = here.id;

writeln("Block-Cyclic Array Index Map");
writeln(BCA);
writeln();
