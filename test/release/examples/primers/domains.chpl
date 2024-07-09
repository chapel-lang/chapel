// Domains

/* This primer showcases Chapel domains as abstract concepts, primarily
   within the context of rectangular domains.  For other uses of domains
   see the following primers:

    - Arrays primer (:ref:`arrays.chpl <primers-arrays>`)
    - Sparse arrays primer (:ref:`sparse.chpl <primers-sparse>`)
    - Associative arrays primer (:ref:`associative.chpl <primers-associative>`)

*/

//
// A domain is a first-class representation of an index set used to
// specify iteration spaces, define arrays, and aggregate operations
// such as slicing.
//

//
// Rectangular domains are used to represent rectangular index sets.
// Each dimension of a rectangular domain is specified by a range and
// thus can take on the shape of any range.  See the Ranges primer
// (:ref:`ranges.chpl <primers-ranges>`) for more information.
//
// Rectangular domains support a literal syntax that is a
// comma-separated list of range expressions enclosed in curly braces.
//
// ``RD`` is an n by n by n domain.
//
config var n = 10;

var RD: domain(3) = {1..n, 1..n, 1..n};
writeln(RD); // {1..10, 1..10, 1..10}

//
// Rectangular domains have a set of methods that enable convenient
// reuse of existing domains.
//
// The :proc:`~ChapelDomain.expand` method returns a new domain that is expanded or
// contracted in each dimension depending on the value and sign of the offset
// argument.  The expansion/contraction in each dimension happens symmetrically to
// the lower and upper bound.
//
var RDexp = RD.expand((1,2,-2));
writeln(RDexp); // {0..11, -1..12, 3..8}

//
// The :proc:`~ChapelDomain.exterior` method returns a new domain that is an
// exterior portion of the current domain.  This is not the "border" of the
// domain (which is not a rectangular domain), in particular the new domain
// will share no indices with the original domain unless an offset is 0, in which
// case the respective range will be that of the original domain.
// Inputs specify the width of the new domain in the respective dimension.
// A positive value specifies that the exterior should be taken from the high
// bound; a negative offset, the low bound.
//
var RDext1 = RD.exterior((1,4,-4));
writeln(RDext1); // {11..11, 11..14, -3..0}
var RDext2 = RD.exterior((0,4,0));
writeln(RDext2); // {1..10, 11..14, 1..10}

//
// The :proc:`~ChapelDomain.interior` method returns a new domain that is the
// interior portion of the current domain.  Inputs specify the width
// of the new domain in the respective dimension.  The sign of the offset implies
// using the high or low bound as in the exterior case.
//
var RDint = RD.interior((2,-5,1));
writeln(RDint); // {9..10, 1..5, 10..10}

//
// The :proc:`~ChapelDomain.translate` method returns a new domain that is the
// current domain translated by the offset.
//
var RDtrans1 = RD.translate((0,4,-4));
writeln(RDtrans1); // {1..10, 5..14, -3..6}

//
// All four functions above
// (:proc:`~ChapelDomain.expand`, :proc:`~ChapelDomain.exterior`,
// :proc:`~ChapelDomain.interior`, :proc:`~ChapelDomain.translate`)
// may also be called with a single offset, in which case the offset will
// apply to all dimensions.
//
var RDtrans2 = RD.translate(4);
writeln(RDtrans2); // {5..14, 5..14, 5..14}

//
// A subdomain is a domain that is declared in terms of a parent
// domain, causing it to have the same type as their parent.  A
// subdomain represents a subset of its parent domain's index set,
// though this constraint is not currently enforced by the
// implementation.

//
// Create rectangular subdomains.
//
/*
  .. warning::

    Sparse domains and arrays are currently unstable.
    Their functionality is likely to change in the future.

*/
var RSD1, RSD2 : subdomain(RD);

// A subdomain is initially empty.
writeln("RSD1:", RSD1); // RSD1:{1..0, 1..0, 1..0}
writeln("RSD2:", RSD2); // RSD2:{1..0, 1..0, 1..0}

// We can select parts of the original rectangular domain using slicing.
RSD1 = RD[..n/2, .., ..];   // This gives half of the domain,
RSD2 = RD[n/2+1.., .., ..]; // and this gives the other half.

writeln("RSD1:", RSD1); // RSD1:{1..5, 1..10, 1..10}
writeln("RSD2:", RSD2); // RSD2:{6..10, 1..10, 1..10}

/*
 .. note::
    - Subdomains of rectangular domains are regular unless they are explicitly
      declared to be sparse.
    - At present, range checking to ensure that a subdomain fits within its
      parent domain is unimplemented.
*/


//
// Create a sparse subdomain of a regular domain.
//

var SSD: sparse subdomain(RD);

writeln("SSD:", SSD); // Initially empty.

// Add some indices to the sparse subdomain.

SSD += (1,2,3);
SSD += (4,5,6);
SSD += (7,8,9);
SSD += (9,10,1);

writeln("SSD:", SSD); // Now contains an unordered set of indices.

/*
  .. note::
     Checks to ensure that sparse subdomain indices lie within the parent domain
     have not been implemented.

For more information on domains, see the Domains chapter of the
:ref:`Chapel Language Specification <chapel-spec>`.
*/
