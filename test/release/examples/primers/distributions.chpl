// Distributions

/*
  This primer demonstrates uses of some of Chapel's standard
  distributions.  To use these distributions in a Chapel program,
  the respective module must be used:
*/
use BlockDist, CyclicDist, BlockCycDist;
use DimensionalDist2D, ReplicatedDim, BlockCycDim;

//
// ``ReplicatedDist`` is covered in the :ref:`primers-replicated` primer.
//
// For each distribution, we'll create a distributed domain and array
// and then initialize it just to give a brief flavor of how the
// distribution maps across locales.  Running this example on 6
// locales does a nice job of illustrating the distribution
// characteristics.
//
// All of these distributions support options to map to a different
// virtual locale grid than the one used by default (a multidimensional
// factoring of the built-in ``Locales`` array), as well as
// to control the amount of parallelism used in data parallel
// loops.  For more details, see the documentation on
// :ref:`Standard Distributions <layouts_and_distributions>`.
//

//
// Make the program size configurable from the command line.
//
config const n = 8;

//
// Declare a 2-dimensional domain ``Space`` that we will later use to
// initialize the distributed domains.
//
const Space = {1..n, 1..n};

// blockDist (and distribution basics)
// -----------------------------------
//
// The ``blockDist`` distribution partitions an n-dimensional bounding
// box between a set of target locales arranged into a conceptual
// n-dimensional grid.  The bounding box is divided into roughly
// equal-size block sections, where each locale owns one of them.
// Note that domains declared using a ``blockDist`` distribution can
// also store indices outside of the bounding box; the bounding box is
// merely used to compute a partitioning of n-dimensional space, not
// to constrain legal domains.
//
// In this example, we declare a 2-dimensional block distribution
// whose bounding box is defined by ``Space``, a domain over that
// distribution using the same index set, and an array over the
// domain.
//
const BlkDist = new blockDist(boundingBox=Space);
const BlockSpace = Space dmapped BlkDist;
var BA: [BlockSpace] int;

//
// To illustrate how the index set is distributed across locales,
// we'll use a forall loop to initialize each array element to the
// locale ID that stores that index/element/iteration.
//
forall ba in BA do
  ba = here.id;

//
// The ``hasSingleLocalSubdomain`` method on arrays will return true if the
// index set for a locale can be represented by a single domain.
//
if !BA.hasSingleLocalSubdomain() then
  halt("For a blockDist distribution, the index set per locale should be \
      represented by a single domain");

//
// If the distribution's subdomains can be represented as single subdomain,
// we can use ``localSubdomain()`` to get the index set for the
// current locale.
//
// Below, we'll use the index set to confirm that the array elements have the
// correct locale ID.
//

for L in Locales {
  on L {
    const indices = BA.localSubdomain();
    for i in indices {
      if BA[i] != L.id then
        halt("Error: incorrect locale id");
    }
  }
}


//
// Output the Block-distributed array to visually see how the elements
// are partitioned across the locales.
//
writeln("Block Array Index Map");
writeln(BA);
writeln();

//
// Most of Chapel's standard distributions support an optional
// ``targetLocales`` argument that permits you to pass in your own
// array of locales to be targeted.  In general, the targetLocales
// argument should match the rank of the distribution.  So for
// example, to block-distribute a domain over a 2D ``numLocales * 1``
// view of the locale set, one could do something like the following.

//
// We start by creating our own ``MyLocales`` array of the locale values.
// Here we use the standard array reshape function for convenience.
// Generally, this array could be accessed/assigned like any other.
//

var MyLocaleView = {0..#numLocales, 1..1};
var MyLocales: [MyLocaleView] locale = reshape(Locales, MyLocaleView);

//
// Then we declare a distributed domain/array that targets
// this view of the locales:
//

const BlkDist2 = new blockDist(boundingBox=Space, targetLocales=MyLocales);
const BlockSpace2 = Space dmapped BlkDist2;
var BA2: [BlockSpace2] int;

//
// Now we can do a similar computation as before to verify where
// each array element ended up:
//
forall ba in BA2 do
  ba = here.id;

writeln("Block Array Index Map");
writeln(BA2);
writeln();

//
// We can use the ``targetLocales`` method available on an array to get
// the locales array used as targets:
//
for (L, ML) in zip(BA2.targetLocales(), MyLocales) do
  if L != ML then
    halt("Error: BA2.targetLocales() should equal MyLocales");



// cyclicDist
// ----------
//
// Next, we'll perform a similar computation for the ``cyclicDist``
// distribution.  This distribution deals indices of n-dimensional
// space out to a set of target locales arranged in a conceptual
// n-dimensional grid.  A designated ``startIdx`` is given to the
// initial locale, and others are dealt out in a round-robin fashion
// in each dimension from there.  Similar to the ``blockDist``
// distribution, domains declared using the ``cyclicDist`` may have
// indices that precede the distribution's starting index.
//
const CycDist = new cyclicDist(startIdx=Space.low);
const CyclicSpace = Space dmapped CycDist;
var CA: [CyclicSpace] int;

forall ca in CA do
  ca = here.id;

writeln("Cyclic Array Index Map");
writeln(CA);
writeln();

//
// When using the ``localSubdomain`` query with ``cyclicDist``, the
// result will be a strided set of indices for any dimension that has
// more than one target locale.
//
on Locales[0] {
  const indices = CA.localSubdomain();
  for i in indices {
    if CA[i] != 0 then
      halt("Error: Cyclic array values on Locale 0 should be zero");
  }
}

// Block-Cyclic
// ------------
//
// Next, we'll use a ``blockCycDist`` distribution.  Block-Cyclic
// distributions also deal out indices in a round-robin fashion,
// but rather than dealing out singleton indices, they deal out blocks
// of indices.  Thus, the ``blockCycDist`` distribution is parameterized
// by a starting index (as with ``Cyclic``) and a block size (per
// dimension) specifying how large the chunks to be dealt out are.
//
const BlkCycSpace = Space dmapped blockCycDist(startIdx=Space.low,
                                              blocksize=(2, 3));
var BCA: [BlkCycSpace] int;

forall bca in BCA do
  bca = here.id;

writeln("Block-Cyclic Array Index Map");
writeln(BCA);
writeln();

//
// A locale's index set for a Block-Cyclic distribution cannot be represented
// by a single subdomain.
//
if BCA.hasSingleLocalSubdomain() then
  halt("A Block-Cyclic index set cannot be represented by a single subdomain");

//
// If the local index set cannot be represented by a single subdomain,
// we can use the ``localSubdomains`` iterator to yield a number of domains
// that represent the whole index set.
//
// Let's write a function that uses ``localSubdomains`` to verify the
// correctness of the array values.
//

proc verifyID(Data: []) {
  for L in Locales {
    on L {
      for indices in Data.localSubdomains() {
        for i in indices {
          if Data[i] != L.id then
            halt("Error: incorrect locale id");
        }
      }
    }
  }
}
verifyID(BCA);

//
// The ``localSubdomains`` iterator is also available on distributions that
// can represent a locale's index set with a single domain. This allows us to
// write more general code that will work for all distributions.
//
// This means that we can call the 'verifyID' function on any array, like the
// 'BA' array from earlier.
//
verifyID(BA);


// 2D Dimensional
// --------------
//
// The ``DimensionalDist2D`` distribution lets us build a 2D distribution
// as a composition of specifiers for individual dimensions.
// Under such a "dimensional" distribution each dimension is handled
// independently of the other.
//
// The dimension specifiers are similar to the corresponding multi-dimensional
// distributions in constructor arguments and index-to-locale mapping rules.
// However, instead of an array of locales, a specifier constructor
// accepts just the number of locales that the indices in the corresponding
// dimension will be distributed across.
//
// The ``DimensionalDist2D`` constructor requires:
//   an ``[0..nl1-1, 0..nl2-1]`` array of locales, where
//   ``nl1`` and ``nl2`` are the number of locales in each dimension, and
//   two dimension specifiers, created for ``nl1`` and ``nl2`` locale counts,
//   resp.
//
// Presently, the following dimension specifiers are available
// (shown here with their constructor arguments):
//
//   * ``ReplicatedDim(numLocales)``
//   * ``BlockDim(numLocales, boundingBoxLow, boundingBoxHigh)``
//   * ``BlockCyclicDim(lowIdx, blockSize, numLocales)``
//

//
// The following example creates a dimensional distribution that
// replicates over 2 locales (when available) in the first dimension
// and distributes using block-cyclic distribution in the second dimension.
// The example computes ``nl1`` and ``nl2`` and reshapes ``MyLocales``
// correspondingly.
//

var (nl1, nl2) = if numLocales == 1 then (1, 1) else (2, numLocales/2);
MyLocaleView = {0..#nl1, 0..#nl2};
MyLocales = reshape(Locales[0..#nl1*nl2], MyLocaleView);

const DimReplicatedBlockcyclicSpace = Space
  dmapped DimensionalDist2D(MyLocales,
                            new ReplicatedDim(numLocales = nl1),
                            new BlockCyclicDim(numLocales = nl2,
                                               lowIdx = 1, blockSize = 2));

var DRBA: [DimReplicatedBlockcyclicSpace] int;

// The ``ReplicatedDim`` specifier always accesses the local replicand.
// (This differs from how the ``Replicated`` distribution works.)
//
// This example visits each replicand. The behavior is the same
// regardless of the second index into ``MyLocales`` below.

for locId1 in 0..#nl1 do on MyLocales[locId1, 0] {

  forall drba in DRBA do
    drba = here.id;

  writeln("Dimensional2D(Replicated,BlockCyclic) Array Index Map",
          " from ", here);

  // Technicality: 'writeln(DRBA)' would read DRBA always on Locale 0.
  // Since we want to see what DRBA contains on the current locale,
  // we use 'Helper' that is mapped using the default distribution.
  // 'Helper = DRBA' captures the view of DRBA on the current locale,
  // which we then print out.

  const Helper: [Space] int = DRBA;
  writeln(Helper);
  writeln();

}
