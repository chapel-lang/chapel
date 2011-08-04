//
// Distributions Primer
//
// This primer demonstrates uses of some of Chapel's standard
// distributions.  To use these distributions in a Chapel program,
// the respective module must be used:
//
use BlockDist, CyclicDist, BlockCycDist, ReplicatedDist;

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
// Most of Chapel's standard distributions support an optional
// targetLocales argument that permits you to pass in your own
// array of locales to be targeted.  In general, the targetLocales
// argument should match the rank of the distribution.  So for
// example, to map a Block to a [numLocales x 1] view of the
// locale set, one could do something like this:

//
// We start by creating our own array of the locale values.  Here
// we use the standard array reshape function for convenience,
// but more generally, this array could be accessed/assigned like any
// other.
//

const MyLocaleView = [0..#numLocales, 1..1];
const MyLocales: [MyLocaleView] locale = reshape(Locales, MyLocaleView);

//
// Then we'll declare a distributed domain/array that targets
// this view of the locales:
// 

const BlockSpace2 = Space dmapped Block(boundingBox=Space,
                                        targetLocales=MyLocales);
var BA2: [BlockSpace2] int;

//
// Then we'll do a similar computation as before to verify where
// everything ended up:
//
forall ba in BA2 do
  ba = here.id;

writeln("Block Array Index Map");
writeln(BA2);
writeln();



//
// Next, we'll perform a similar computation for the Cyclic distribution.
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


//
// The ReplicatedDist distribution is different: each of the
// original domain's indices - and the corresponding array elements -
// is replicated onto each locale. (Note: consistency among these
// array replicands is NOT maintained automatically.)
//
// This replication is observable in some cases but not others,
// as shown below. Note: this behavior may change in the future.
//
const ReplicatedSpace = Space dmapped ReplicatedDist();
var RA: [ReplicatedSpace] int;

// The replication is observable - this visits each replicand.
forall ra in RA do
  ra = here.id;

writeln("Replicated Array Index Map, ", RA.numElements, " elements total");
writeln(RA);
writeln();

//
// The replication is observable when the replicated array is
// on the left-hand side. If the right-hand side is not replicated,
// it is copied into each replicand.
// We illustrate this using a non-distributed array.
//
var A: [Space] int = [(i,j) in Space] i*100 + j;
RA = A;
writeln("Replicated Array after being array-assigned into");
writeln(RA);
writeln();

//
// Analogously, each replicand will be visited and
// other participated expressions will be computed on each locale
// (a) when the replicated array is assigned a scalar:
//       RA = 5;
// (b) when it appears first in a zippered forall loop:
//       forall (ra, a) in (RA, A) do ...;
// (c) when it appears in a for loop:
//       for ra in RA do ...;
//
// Zippering (RA,A) or (A,RA) in a 'for' loop will generate
// an error due to their different number of elements.

// Let RA store the Index Map again, for the examples below.
forall ra in RA do
  ra = here.id;

//
// Only the local replicand is accessed - replication is NOT observable
// and consistency is NOT maintained - when:
// (a) the replicated array is indexed - an individual element is read...
//
on Locales(0) do
  writeln("on ", here, ": ", RA(Space.low));
on Locales(LocaleSpace.high) do
  writeln("on ", here, ": ", RA(Space.low));
writeln();

// ...or an individual element is written...
on Locales(LocaleSpace.high) do
  RA(Space.low) = 7777;

writeln("Replicated Array after being indexed into");
writeln(RA);
writeln();

//
// (b) the replicated array is on the right-hand side of an assignment...
//
on Locales(LocaleSpace.high) do
  A = RA + 4;
writeln("Non-Replicated Array after assignment from Replicated Array + 4");
writeln(A);
writeln();

//
// (c) does not appear first in a zippered forall loop:
//       forall (a, ra) in (A, RA) do ...;
//
