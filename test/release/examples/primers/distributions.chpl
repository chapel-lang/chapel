// Distributions

/*
  This primer introduces the concept of the *distribution* in Chapel
  and then illustrates the use of a few of Chapel's standard
  distributions.

  In Chapel, *distributions* are recipes for implementing arrays and
  their index sets (*domains*).  Each distribution indicates how a
  domain's indices and array's elements should be mapped to locales,
  stored in each locale's memory, accessed, iterated over, etc.  This
  primer will assume you already have some familiarity with the
  concepts of :ref:`arrays <primers-arrays>`, :ref:`domains
  <primers-domains>`, and :ref:`locales <primers-locales>`; refer to
  their respective primers for more information.

  By default, a task declaring a new domain or array will map all the
  domain's indices and array's elements to the locale on which it's
  running.  This can be thought of as a _degenerate_ distribution, or
  a _layout_.  However, as this primer will demonstrate, domains and
  arrays can also be declared in terms of a specific distribution
  instance to select alternative implementations.

  Crucially, the operations supported by domains and arrays are
  independent of whether they are stored on a single locale or
  distributed across multiple.  The only difference is how and where
  the operations are implemented, which can have impacts on
  performance and scalability (positive or negative).  This property
  makes it easy to implement and debug an algorithm on a single node,
  and then deploy it on a large-scale supercomputer.  It also permits
  you to change the distributions used by your program to alter its
  implementation.
  
  Distributions for rectangular arrays can be thought of as
  distributing a *d*-dimensional space of possible indices to a
  *d*-dimensional array of locales.  Most distribution types are
  characterized by this *rank*, *d*, as well as the index type
  (``idxType``) used to represent indices in each dimension.  For
  example, a 3D distribution might map ``(int, int, int)`` indices to
  a 3D array of locales, defining which one owns any given index ``(i,
  j, k)``.  It would also specify how the array elements corresponding
  a locale's local indices are stored in memory.

  Typical Chapel distributions use an "owner computes" model in which
  each locale owns a subset of a domain's indices as well as the array
  elements corresponding to those indices.  When ``forall`` loops or
  other data-parallel operations are executed across such distributed
  domains and arrays, each locale executes the iterations
  corresponding to its indices and array elements in parallel, using
  its locally available processor cores.

  Multiple domains may share a single distribution, even if their
  index sets are different.  For typical distributions, this ensures
  that a given index ``(i, j, k)`` is owned by the same locale for all
  of the domains, implying that computations on matching indices will
  not require communication.  Domains with this property are said to
  be *aligned*.  When multiple arrays are declared in terms of a
  single distributed domain, they are guaranteed to be aligned, since
  they all share the same distribution.

  In this primer, we'll introduce two common distributions: the first
  maps indices to locales using contiguous rectilinear blocks; the
  other deals indices out to locales in a round-robin fashion.  To use
  these distributions in a Chapel program, their respective modules
  must be used or imported:
*/
use BlockDist, CyclicDist;

//
// A third and somewhat atypical distribution, ``replicatedDist``, is
// covered in the :ref:`primers-replicated` primer.  It does not
// adhere to some of the characterizations described as "typical"
// above.
//
// For each distribution in this primer, we'll create a distributed
// domain and array using that distribution.  Then we'll initialize
// the array using the IDs of the locales owning the elements to
// illustrate how indices are mapped to locales.  Running this example
// on 6 locales does a nice job of illustrating the distribution's
// characteristics.
//
// By default, these distributions will target the entire array of
// ``Locales`` on which the program is running, heuristically
// reshaping them into a *d*-dimensional grid whenever *d* is greater
// than 1.  When a user wants to exert more control over how the
// locales are targeted—for example to specify the number of locales
// in each dimension, or to target just a subset of the locales—they
// may create their own array of locales and pass that in as an
// argument, as we'll see below.
//

//
// First, we'll declare the problem size we'll use for our 2D domains
// and arrays, configurable from the command-line:
//
config const n = 8;

//
// Next, we'll declare a 2-dimensional domain ``Space``, which will
// form the basis of our distributed domains.
//
const Space = {1..n, 1..n};

// blockDist (and distribution basics)
// -----------------------------------
//
// The ``blockDist`` distribution partitions a *d*-dimensional
// bounding box across the *d*-dimensional target locale array.  The
// bounding box is divided into roughly equal-size block sections,
// where each locale owns one of them.

//
// In this example, we declare a 2-dimensional block distribution
// whose bounding box is defined by ``Space``.  We then use that
// distribution instance to create a distributed domain and an array
// over the domain:
//
const BlkDist = new blockDist(boundingBox=Space);
const BlockSpace = BlkDist.createDomain(Space);
var BA: [BlockSpace] int;

/*

  Since the code above only uses a single domain and array, it could
  also be written in one of the following ways for convenience:

  .. code-block:: chapel

	  const BlockSpace = blockDist.createDomain(Space);  
	  const BA: [BlockSpace] int;

  or:

  .. code-block:: chapel

  	  const BA = blockDist.createArray(Space);


  However, since we'll be declaring multiple domains using the same
  distribution, it's useful to declare the distribution instance,
  domain, and array as distinct variables.
*/

//
// To illustrate how the distributed domain and array are mapped to
// locales, let's use a forall loop that assigns each array element
// using the locale ID that stores that index/element/iteration.
//
forall ba in BA do
  ba = here.id;

// As mentioned above, Chapel's owner-computes model means that each
// locale will execute the iterations of this forall loop that it owns
// using its local processor cores.  As a result, each value of ``BA``
// will end up indicating which locale owns it.  We can see this
// ownership map by printing the array out:

writeln("Block Array Ownership Map");
writeln(BA);
writeln();

// We can also determine which indices a given locale owns using the
// ``.localSubdomain()`` query.  This method returns a non-distributed
// domain representing the indices that are local to that locale.  For
// example, we can use it to determine which indices a given locale,
// like locale 0, owns:

writeln("Locale 0 owns the following indices of BA: ", BA.localSubdomain());
writeln();

// As another example, the following code iterates over all the
// locales, querying the local indices and checking that the
// corresponding array elements have the expected value:

coforall L in Locales {
  on L {
    const myIndices = BA.localSubdomain();
    for i in myIndices {
      if BA[i] != L.id then
        halt("Error: incorrect locale id");
    }
  }
}


// Domains declared in terms of a ``blockDist`` distribution can also
// include indices outside of the bounding box. That is, the bounding
// box is only used to compute a partitioning of the *d*-dimensional
// space, **not** to constrain legal domain indices.  Any indices
// located outside the bounding box will be owned by the locale owning
// the nearest index within it.  For example, we can declare a larger
// distribution than ``BlockSpace`` as follows:

const BigBlockSpace = blockDist.createDomain({0..n+1, 0..n+1});

// In this case, the rows and columns numbered ``0`` and ``n+1`` fall
// outside of the bounding box, and such indices, like ``(0, i)`` will
// be owned by the same locale that owns ``(1, i)`` within the box.
// Because ``BigBlockSpace`` shares a distribution with ``BlockSpace``
// we know that any indices that they both contain will be owned by
// the same locale, and that the domains are *aligned*.

//
// As mentioned above, most Chapel distributions support an optional
// ``targetLocales`` argument that permits you to pass in your own
// array of locales to be targeted.  In general, the targetLocales
// argument will match the rank of the distribution.  For example, to
// block-distribute a domain such that each locale owns a block of
// rows but all of the columns, we can create a ``numLocales x 1``
// view of the locale set as follows:

//
// We start by creating our own ``MyLocales`` array of locale values.
// Here, we use the standard array reshape function for convenience.
// More generally, this array can be declared and created like any
// other.
//

const MyLocaleView = {0..<numLocales, 0..0};
var MyLocales: [MyLocaleView] locale = reshape(Locales, MyLocaleView);

//
// Then we declare a distributed domain/array that targets
// this view of the locales:
//

const BlkDist2 = new blockDist(boundingBox=Space, targetLocales=MyLocales);
const BlockSpace2 = BlkDist2.createDomain(Space);
var BA2: [BlockSpace2] int;

//
// Now we can do a similar computation as before to verify where
// each array element ended up:
//
forall ba2 in BA2 do
  ba2 = here.id;

writeln("Block Array Ownership Map");
writeln(BA2);
writeln();

//
// We can use the ``targetLocales`` method supported by an array to
// get the locales array that it's mapped to:
//
forall (l, ml) in zip(BA2.targetLocales(), MyLocales) do
  if l != ml then
    halt("Error: BA2.targetLocales() should equal MyLocales");



// cyclicDist
// ----------
//
// Next, we'll perform a similar computation for the ``cyclicDist``
// distribution.  This distribution also maps indices of
// *d*-dimensional space out to a set of target locales arranged in a
// conceptual *d*-dimensional grid.  However, where ``blockDist``
// distributes contiguous blocks of indices, ``cyclicDist`` deals
// indices to locales in a round-robin fashion.  A designated
// ``startIdx`` is given to the initial locale, and others are dealt
// out cyclically in each dimension from there in all directions.  As
// with ``blockDist``, domains declared using ``cyclicDist`` may have
// indices that precede the distribution's starting index.
//
const CycDist = new cyclicDist(startIdx=Space.low);
const CyclicSpace = CycDist.createDomain(Space);
var CA: [CyclicSpace] int;

/*
As with ``blockDist``, these declarations could be shortened to one of
the following expressions for convenience:

.. code-block:: chapel

	const CyclicSpace = cyclicDist.createDomain(Space);  
	const CA: [CyclicSpace] int;

or:

.. code-block:: chapel

	const CA = cyclicDist.createArray(Space);

*/

forall ca in CA do
  ca = here.id;

writeln("Cyclic Array Ownership Map");
writeln(CA);
writeln();

//
// When using the ``localSubdomain`` query with ``cyclicDist``, the
// result will be a strided set of indices for any dimension that has
// more than one target locale.  For example, locale 0's ownership
// of CA is:
//
writeln("Locale 0 owns the following indices of CA: ", CA.localSubdomain());


// That wraps up this brief introduction to distributions in Chapel
// and their use in declaring distributed domains and arrays.  Keep in
// mind that while we demonstrated only very trivial computations in
// this example, all of Chapel's rich operations on domains and arrays
// are available whether they are local or distributed.
