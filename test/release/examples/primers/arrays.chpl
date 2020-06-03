// Arrays

//
// This primer is a tutorial on Chapel rectangular arrays and domains.
//

//
// Arrays in Chapel are specified using a square-bracketed expression
// that specifies the array's index set, followed by the array's
// element type.  Rectangular arrays are those whose indices are
// integers or tuples of integers, supporting standard
// multidimensional, rectilinear index sets.
//

config const n = 5;

//
// Declare an array of five 64-bit real values:
//
var A: [1..n] real;

//
// Like other variable types in Chapel, arrays are initialized so that
// each element stores its default value.  So our array of real values
// above will default to an array of the value 0.0.
//

writeln("Initially, A is: ", A);

//
// Arrays can also be declared using the the array literal syntax.
// Array literals are specified by enclosing a comma separated list of
// expressions in square brackets.  The domain of the array will be
// 0-based, and the type of the array's element is the type of the
// first element listed.
//
var A2 = [-1.1, -2.2, -3.3, -4.4, -5.5];

writeln("Initially, A2 is: ", A2);

//
// Arrays can be accessed using scalar index values of the appropriate
// type, using either parentheses or square brackets:
//

A[1] = 1.1;
A(2) = 2.2;

writeln("After assigning two elements, A is: ", A);

//
// Arrays can also be accessed using ranges to refer to subsets of
// array elements, or sub-arrays, using a technique called slicing:
//

A[2..4] = 3.3;

writeln("After assigning its interior values, A is: ", A);
writeln();

//
// Just like with array indexing, either square brackets or parentheses
// can be used for array slicing:
//

writeln("A(2..4) is: ", A(2..4), "\n");

//
// Note: further information on slicing can be found in the
// :ref:`Slices Primer <primers-slices>`
//

//
// Arrays can be multidimensional as well:
//

var B: [1..n, 1..n] real;

forall (i,j) in {1..n, 1..n} do
  B(i,j) = i + j/10.0;

writeln("Initially, B is:\n", B, "\n");

//
// An array's elements can be iterated over in a for or forall loop,
// which causes the index variable to store references to an array's
// elements:
//

forall b in B do
  b += 1;

writeln("After incrementing B's elements, B is:\n", B, "\n");

//
// An array's index set is referred to as a domain -- a first-class
// language concept that stores the set of indices used to access the
// array.  The arrays A and B above are respectively declared with the
// anonymous but explicit domains ``{1..n}`` and ``{1..n, 1..n}``.
// Array A2 above is declared with the implicit domain ``{0..4}``.  An
// array's domain can be accessed using the ``.domain`` method:
//

forall (i,j) in B.domain do
  B(i,j) -= 1;

writeln("After decrementing B's elements, B is:\n", B, "\n");

//
// Domains can also be queried when arrays are passed to functions
// in order to refer to the domain via a new identifier within the
// context of that function:
//

printArr(B);

proc printArr(X: [?D] real) {
  writeln("within printArr, D is: ", D, "\n");
  forall (i,j) in D do
    X(i,j) = -X(i,j);
  writeln("after negating X within printArr, X is:\n", X, "\n");
}

//
// Arrays are passed to functions by reference by default, so the
// modifications to X in function printArr are reflected on B as
// well:
//

writeln("After calling printArr, B is:\n", B, "\n");

//
// Domains can also be declared and named.  This has the advantages
// of allowing them to be reused for multiple arrays, for associating
// names with different index spaces, and for amortizing the overhead
// of storing distributed arrays across multiple similar array
// variables.
//
// The following domain declaration defines a 2D rectangular domain
// called ``ProbSpace`` which is the same size and shape as ``B`` was above.
//

var ProbSpace: domain(2) = {1..n, 1..n};

//
// We then use that domain to declare some arrays...
//

var C, D, E: [ProbSpace] bool;

// ...and to iterate over their index spaces...

for (i,j) in ProbSpace do
  C(i,j) = (i+j) % 3 == 0;

writeln("After initializing C, its value is:\n", C, "\n");

//
// When indexing over a multidimensional domain, the indices
// can be captured into a single tuple variable rather than
// destructuring the tuple into its integer components.
// Similarly, multidimensional array accesses can be expressed using
// tuple indices rather than multiple integer arguments.  In the
// following example, the index variable ij stores a 2-tuple of
// integers (``2*int`` in Chapel).  This is a really inefficient way
// to assign the diagonal values "true" -- note the use of tuple
// indexing to tease the individual components out of the 2-tuple.
//

for ij in ProbSpace do
  D(ij) = ij(0) == ij(1);

writeln("After initializing D, its value is:\n", D, "\n");


//
// Arrays of similar size and shape support whole-array assignment.
//

E = C;

writeln("After assigning C to E, E's value is:\n", E, "\n");

//
// Whole array assignment also allows scalar values to be promoted
// and assigned to every element of an array.
//

B = 0.0;

writeln("After being reset, B is:\n", B, "\n");

//
// An array need not be indexed using the domain used to declare it,
// though doing so presents the compiler with opportunities to
// optimize bounds checks away.  In the following loop, there is
// no known relation between ``B`` and ``ProbSpace``, so bounds checks are
// harder to prove away (requires symbolic analysis of the definitions
// of the two domains and the invariance of their bounds).
//

for (i,j) in ProbSpace do
  B(i,j) = i + j/10.0;

writeln("B has been re-initialized to:\n", B, "\n");

//
// Whole-array assignment can also be used for arrays or sub-arrays
// whose index spaces differ. Their shapes must still match.
//

var F, G: [ProbSpace] real;

F[2..n-1, 2..n-1] = B[1..n-2, 3..n];

writeln("After assigning a slice of B to F, F's value is:\n", F, "\n");

//
// Arrays can also be sliced using unbounded ranges in which either
// the low and/or high bounds are omitted.  In this case, they will be
// inherited from the array's bounds.
//

G[2.., ..] = B[..n-1, ..];

writeln("After assigning a slice of B to G, G's value is:\n", G, "\n");

//
// Array slicing supports rank-change semantics when sliced using
// a scalar value rather than a range.  In the following assignment,
// recall that ``A`` was our initial 1-dimensional array.
//

A = B[n/2, ..];

writeln("After being assigned a slice of B, A is:\n", A, "\n");

//
// Domains can also be sliced.  However, rather than having indexing
// semantics, domain slicing results in intersection of the domain's
// index set and the specified slice.
//
// Domain slicing, like array indexing and slicing, can be written
// with either square brackets or parentheses.
//

writeln("ProbSpace[1..n-2, 3..] is: ", ProbSpace[1..n-2, 3..], "\n");

// Ranges also support slicing in this way, though we don't
// demonstrate that here.

//
// Domain variables and expressions can also be used to specify an
// array slice rather than using lists of ranges.  For example:
//

const ProbSpaceSlice = ProbSpace[0..n+1, 3..];

writeln("B[ProbSpaceSlice] is:\n", B[ProbSpaceSlice], "\n");

//
// Forall loops over domains and arrays can be written using the
// syntax ``[<ind> in <Dom>] ...`` which is shorthand for
// ``forall <ind> in <Dom> do ...``
//

const offset = (1,1); // a 2-tuple offset

[ij in ProbSpace[2..n-1, 2..n-1]] F(ij) = B(ij + offset);

writeln("After assigning F a shifted slice of B, it is:\n", F, "\n");

[b in B] b = -b;

writeln("After negating B, it is:\n", B, "\n");

//
// Note that this shorthand resembles the array type definition in
// a variable declaration.

//
// Another advantage to declaring named domain variables is that their
// index sets can be reassigned.  This results in a logical
// re-allocation of the array variable in question, preserving array
// values for indices that existed in both the old and new domain
// values:
//

var VarDom = {1..n};

var VarArr: [VarDom] real = [i in VarDom] i;

writeln("Initially, VarArr = ", VarArr, "\n");

//
// Now, if we reassign ``VarDom``, ``VarArr`` will be reallocated with the
// old values preserved and the new values initialized to the element
// type's default value.
//

VarDom = {1..2*n};

writeln("After doubling VarDom, VarArr = ", VarArr, "\n");

//
// As mentioned before, this reallocation preserves values according
// to index, so if we extend the lower bound of the domain, the
// non-zero values will still logically be associated with indices
// ``1..n``:
//

VarDom = {-n+1..2*n};

writeln("After lowering VarDom's lower bound, VarArr = ", VarArr, "\n");

//
// If the domain shrinks, values will be thrown away
//

VarDom = {2..n-1};

writeln("After shrinking VarDom, VarArr = ", VarArr, "\n");

//
// One trick to reallocate an array without preserving any values is
// to assign its domain variable a degenerate domain, e.g. ``{1..0}``,
// and then assign it the new value:
//

VarDom = {1..0};

writeln("VarArr should now be empty: ", VarArr, "\n");

VarDom = {1..n};

writeln("VarArr should now be reset: ", VarArr, "\n");

//
// Note that querying an array's domain via the ``.domain`` method or
// the function argument query syntax does not result in a domain
// expression that can be reassigned.  In particular, we cannot do:
//
//   ``VarArr.domain = {1..2*n};``
//
// nor:
//
//   ``proc foo(X: [?D]) {  D = {1..2*n};  }``
//
// Only a domain variable or formal argument can be
// reassigned to reallocate arrays.  This is to avoid confusion
// since assigning one domain variable can cause a number of
// arrays to be reallocated.  It also implies that arrays declared
// using an anonymous domain cannot be reallocated.  So for our
// original array declarations ``A`` and ``B``, we have no way of reallocating
// them.  Arrays with constant domains provide the compiler with
// optimization benefits, so this supports a common case efficiently.
//

//
// As some of our examples have shown, arrays in Chapel can have
// arbitrary element types -- numeric values, classes, or records.
// Arrays can also support array elements, and initial support for
// this is implemented in our compiler.  For example:
//

var Y: [ProbSpace] [1..3] real;

forall (i,j) in ProbSpace do
  for k in 1..3 do
    Y(i,j)(k) = i*10 + j + k/10.0;

writeln("Y is:\n", Y);

/*
   Our current implementation requires that array elements must
   all be of uniform size. We would also like to support jagged arrays,
   where the inner array size is a function of the outer.
   In particular, it is our intention to support arrays like these:

   .. code-block:: chapel

       var Triangle: [row in 1..n] [1..row] real;
       var HierArr: [lvl in 1..n] [1..2**lvl, 1..2**lvl] real;
*/

//
// For further information, see the :ref:`Domain Primer <primers-domains>`
// and other array primers: :ref:`Sparse <primers-sparse>`,
// :ref:`Associative <primers-associative>`.
//
