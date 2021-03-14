// Domains

/*
   This primer showcases Chapel domains as abstract concepts, primarily
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
writeln(RD);

//
// Rectangular domains have a set of methods that enable convenient
// reuse of existing domains.
//
// The :proc:`~ChapelArray.expand` method returns a new domain that is expanded or
// contracted depending on the sign of the offset argument.
//
var RDbigger = RD.expand((1,1,1));
writeln(RDbigger);
var RDsmaller = RD.expand((-1,-1,-1));
writeln(RDsmaller);

//
// The :proc:`~ChapelArray.exterior` method returns a new domain that is the
// exterior portion of the current domain.  A positive offset specifies that
// the exterior should be taken from the high bound; a negative offset, the low
// bound.
//
var RDext_p = RD.exterior((1,1,1));
writeln(RDext_p);
var RDext_n = RD.exterior((-1,-1,-1));
writeln(RDext_n);

//
// The :proc:`~ChapelArray.interior` method returns a new domain that is the
// interior portion of the current domain.  The sign of the offset implies
// using the high or low bound as in the exterior case.
//
var RDint_p = RD.interior((1,1,1));
writeln(RDint_p);
var RDint_n = RD.interior((-1,-1,-1));
writeln(RDint_n);

//
// The :proc:`~ChapelArray.translate` method returns a new domain that is the
// current domain translated by the offset.
//
var RDtrans_p = RD.translate((1,1,1));
writeln(RDtrans_p);
var RDtrans_n = RD.translate((-1,-1,-1));
writeln(RDtrans_n);

//
// A subdomain is a domain that is declared in terms of a parent
// domain, causing it to have the same type as their parent.  A
// subdomain represents a subset of its parent domain's index set,
// though this constraint is not currently enforced by the
// implementation.

//
// Create rectangular subdomains.
//
var RSD1, RSD2 : subdomain(RD);

// A subdomain is initially empty.
writeln("RSD1:", RSD1);
writeln("RSD2:", RSD2);

// We can select parts of the original rectangular domain using slicing.
RSD1 = RD[..n/2, .., ..]; // This gives half of the domain
RSD2 = RD[n/2+1.., .., ..]; // And this the other half.

writeln("RSD1:", RSD1);
writeln("RSD2:", RSD2);

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
