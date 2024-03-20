// Arrays

//
// This primer is a tutorial on Chapel's rectangular arrays and
// domains.  Other primers cover Chapel's :ref:`associative
// <primers-associative>` and :ref:`sparse <primers-sparse>` arrays,
// building on concepts introduced here.
//

//
// Arrays in Chapel are specified using a square-bracketed expression
// that specifies the array's index set or *domain*, followed by the
// array's element type.  Rectangular arrays are those whose indices
// are integers or tuples of integers, bounded by a range in each
// dimension, supporting multidimensional, rectilinear index sets.
//

// Declaring Arrays
// ----------------

// Let's start by declaring an ``n``-element array of 64-bit ``real``
// values (where ``n`` defaults to 5):

config const n = 5;

var A: [1..n] real;

//
// Like other variable types in Chapel, arrays are initialized so that
// each element stores its default value.  So our array of ``real`` values
// above will default to an array whose elements each store the value
// 0.0.
//

writeln("Initially, A is: ", A);  // prints 0.0 for each array element

//
// Arrays can also be declared using array literals.  These are
// specified by enclosing a comma-separated list of expressions in
// square brackets.  Unless otherwise specified, the domain of the new
// array variable will be 0-based, and the type of its elements will
// be that of the expressions if they all have the same type.  If they
// do not, the array's element type will be a type that can hold all
// of the values, if the compiler can determine that one exists.
//
var A2 = [-1.1, -2.2, -3.3, -4.4, -5.5];

writeln("Initially, A2 is: ", A2);


// Basic Array Indexing and Slicing
// --------------------------------

//
// Arrays can be accessed using scalar index values of the appropriate
// type, using either parentheses or square brackets:
//

A[1] = 1.1;
A(2) = 2.2;

writeln("After assigning two elements, A is: ", A);

//
// Arrays can also be accessed using ranges to refer to subsets of
// array elements, or *sub-arrays*, using a technique called *slicing*:
//

A[2..4] = 3.3;

writeln("After assigning its interior values, A is: ", A);
writeln();

//
// As with array indexing, either square brackets or parentheses can
// be used for array slicing:
//

writeln("A(2..4) is: ", A(2..4), "\n");

//
// Further information on slicing can be found in the :ref:`Slices
// Primer <primers-slices>`
//

// Multidimensional Arrays
// -----------------------

//
// Arrays can be multidimensional as well.  For example, the following
// declaration creates a 2D ``n`` x ``n`` array of ``real``
// values.
//

var B: [1..n, 1..n] real;

forall (i,j) in {1..n, 1..n} do
  B[i,j] = i + j/10.0;

writeln("Initially, B is:\n", B, "\n");

// Loops over Arrays
// -----------------

//
// An array's elements can be iterated over using Chapel's standard
// loop forms like ``for``, ``foreach``, or ``forall`` (see the
// :ref:`Loops Primer <primers-loops>` for details).  These cause the
// index variable to refer to an array element in each iteration.  For
// example, the following loop increments each of ``B``'s elements by
// 1, in parallel:
//

forall b in B do
  b += 1;

writeln("After incrementing B's elements, B is:\n", B, "\n");


// Domains and Domain Queries
// --------------------------

//
// An array's index set is referred to as a domain — a first-class
// language concept that stores the set of indices used to access the
// array.  The arrays ``A`` and ``B`` above are respectively declared
// over the anonymous domains ``{1..n}`` and ``{1..n, 1..n}``, created
// from the ranges specified within the array type's square brackets.
// Array ``A2`` above will have the implicit domain ``{0..4}`` to
// represent the five values in its initializing expression.
//
// An array's domain can be queried using the ``.domain`` method,
// which returns a ``const ref`` to the domain in question.  For
// example, here's a loop that iterates over B's indices in parallel
// by querying its domain:
//

forall (i,j) in B.domain do
  B[i,j] -= 1;

writeln("After decrementing B's elements, B is:\n", B, "\n");

//
// Domains can also be queried when arrays are passed to routines, as
// a means of associating a new ``const ref`` identifier with the
// domain for the routine's duration.  For example, the following
// procedure queries the domain of its array argument ``X``, naming it
// ``D``:
//

proc negateAndPrintArr(ref X: [?D] real) {
  writeln("within negateAndPrintArr, D is: ", D, "\n");
  forall (i,j) in D do
    X[i,j] = -X[i,j];
  writeln("after negating X within negateAndPrintArr, X is:\n", X, "\n");
}

negateAndPrintArr(B);

//
// Arrays are passed to routines by constant (``const``) by
// default, which does not allow them to be modified within the routine.
// The above procedure ``negateAndPrintArr()`` must use a non-constant
// reference intent (``ref``) explicitly, so that its modifications of ``X``
// are both allowed and reflected in the actual argument ``B``:
//

writeln("After calling negateAndPrintArr, B is:\n", B, "\n");

//
// Domains can also be declared and named.  This has several
// advantages, including:
//
// * allowing multiple arrays to share a single domain
//
// * associating a logical name with an index set
//
// * amortizing overheads when storing multiple distributed arrays with the same indices
//
// * enabling compiler optimizations.
//
//
// The following domain declaration defines a 2D rectangular domain
// called ``ProbSpace``, which has the same size, shape, and index set
// as ``B`` above.
//

const ProbSpace = {1..n, 1..n};

// Note that we declare the domain as being ``const``, indicating that
// we will never change the set of indices it represents.  Besides
// indicating the programmer's intent, this can enable key compiler
// optimizations, and is therefore recommended whenever a domain's
// index set is known to be invariant.
//

//
// We can then use that domain to declare some arrays...
//

var C, D, E: [ProbSpace] bool;

// ...and to iterate over their shared index set...

forall (i,j) in ProbSpace do
  C[i,j] = (i+j) % 3 == 0;

writeln("After assigning C, its value is:\n", C, "\n");

//
// An array need not be accessed using indices from the domain that
// was used to declare it.  For example, the following loop indexes
// into ``B`` using indices from ``ProbSpace`` even though there is no
// direct relationship between ``B`` and ``ProbSpace``.
//

for (i,j) in ProbSpace do
  B[i,j] = i + j/10.0;

writeln("B has been re-assigned to:\n", B, "\n");

//
// When iterating over a multidimensional domain, the indices can be
// expressed using a single tuple variable rather than destructuring
// the tuple into its integer components.  Similarly, multidimensional
// array accesses can be written using tuple indices rather than
// multiple integer arguments.  In the following example, the index
// variable ``ij`` stores a 2-tuple of integers (``2*int`` or ``(int,
// int)``).  Note the use of tuple indexing to access the individual
// components of the 2-tuple ``ij``.
//

for ij in ProbSpace do
  D[ij] = ij(0) == ij(1);

writeln("After assigning D, its value is:\n", D, "\n");

//
// For further information on domains, see the :ref:`Domain Primer
// <primers-domains>`.
//


// Whole-Array Assignment
// ----------------------

//
// Arrays of similar size and shape support whole-array assignment.
//

E = C;

writeln("After assigning C to E, E's value is:\n", E, "\n");

//
// Whole-array assignment also permits a scalar value that is
// compatible with the array's element type to be assigned to each
// element of the array:
//

E = true;

writeln("After being assigned 'true', E is:\n", E, "\n");

//
// Whole-array assignment can also be used for arrays or sub-arrays
// whose index sets differ as long as they have the same number of
// dimensions and the same shape (number of elements per dimension).
//

var F, G: [ProbSpace] real;

F[2..n-1, 2..n-1] = B[1..n-2, 3..n];

writeln("After assigning a slice of B to a slice of F, F's value is:\n", F, "\n");


// More on Slicing
// ---------------

//
// Arrays can also be sliced using unbounded ranges in which either
// the low and/or high bounds are omitted.  In this case, the missing
// bounds are defined by the array's bounds.  For example, the
// following statement assigns all rows of ``G`` starting from row 2
// using all rows of ``B`` up to number ``n-1``.  It assigns all columns
// since no bounds are provided in the second dimension.
//

G[2.., ..] = B[..n-1, ..];

writeln("After assigning a slice of B to G, G's value is:\n", G, "\n");

//
// Array slicing supports rank-change semantics when sliced using a
// scalar value rather than a range.  In the following assignment,
// recall that ``A`` was our initial 1-dimensional array.  The slice
// of ``B`` takes all columns of row ``n/2``, treating it as a 1D array.
//

A = B[n/2, ..];

writeln("After being assigned a slice of B, A is:\n", A, "\n");

//
// Domains can also be sliced.  However, rather than having indexing
// semantics, domain slicing computes the intersection between the
// domain's index set and the specified slice.  Like array indexing
// and slicing, domain slicing can be written with either square
// brackets or parentheses.
//

writeln("ProbSpace[1..n-2, 3..] is: ", ProbSpace[1..n-2, 3..], "\n");

//
// Domain variables and expressions can also be used to specify an
// array slice rather than using lists of ranges.  For example:
//

const ProbSpaceSlice = ProbSpace[0..n+1, 3..];

writeln("B[ProbSpaceSlice] is:\n", B[ProbSpaceSlice], "\n");


// Resizing Arrays
// ---------------

//
// Another advantage to declaring named domain variables is that their
// index sets can be reassigned.  This results in a logical
// re-allocation of all arrays declared over that domain, preserving
// the array values for any indices that are preserved by the new
// domain's value:
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
// As mentioned, this reallocation preserves values according
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
// to assign its domain variable a degenerate domain (e.g. ``{1..0}``)
// and then assign it the new value:
//

VarDom = {1..0};  // empty the array such that no values need to be preserved

writeln("VarArr is now empty: ", VarArr, "\n");

VarDom = {1..n};  // re-assign the domain to establish the new indices

writeln("VarArr should now be reset: ", VarArr, "\n");

// Note that querying an array's domain via the ``.domain`` method or
// the function argument query syntax does not result in a domain that
// can be reassigned since those forms return a ``const ref``.  In
// particular, we cannot do:
//
//   ``B.domain = {1..2*n, 1..2*n};``
//
// nor:
//
//   ``proc foo(X: [?D]) {  D = {1..2*n};  }``
//
// Instead, to resize such arrays, their domains would need to be
// named variables, and would need to be passed by ``ref`` to any
// subroutines wanting to resize the arrays.
//
// An implication of this is that arrays declared using an anonymous
// domain cannot be reallocated.  So for our original array
// declarations ``A`` and ``B``, we have no way of reallocating them.
// Arrays with constant domains provide the compiler with optimization
// benefits, so this design supports a common case efficiently.

// Array Fields / Storing Arrays in Objects
// ----------------------------------------

//
// A record with an array field whose size is not known until
// initialization time can be declared as follows:
//

record wrapFixedArr {
  const size: int;
  var Arr: [1..size] real;
}

var RSmall = new wrapFixedArr(size=10);
var RLarge = new wrapFixedArr(size=1000);

writeln("Size of RSmall's FieldArr: ", RSmall.Arr.size);
writeln("Size of RLarge's FieldArr: ", RLarge.Arr.size);

// Note that such a record does not support resizing the array since
// its domain is not a named variable.  However, we could create such
// a record as follows:

record wrapDynArr {
  var Inds = {1..0};
  var Arr: [Inds] real;
}

var r: wrapDynArr;

writeln("Initial size of r: ", r.Arr.size);

r.Inds = {1..100};

writeln("New size of r: ", r.Arr.size);

// Either of these approaches can be used to create records (or
// classes) with array fields where each instance of the type has a
// different array size.

//
// Further information on records can be found in the :ref:`Records
// Primer <primers-records>`
//


// Arrays of Arrays
// ----------------

//
// Arrays in Chapel can have arbitrary element types, such as numeric
// values, classes, or records.  Arrays can also have array elements,
// and initial support for this is implemented in our compiler.  For
// example:
//

var Y: [ProbSpace] [1..3] real;

forall (i,j) in ProbSpace do
  for k in 1..3 do
    Y[i,j][k] = i*10 + j + k/10.0;

writeln("Y is:\n", Y);

/*

   In such array-of-array cases, our current implementation requires
   that the array elements all have the same index set.  In the
   future, we expect to support skyline/jagged arrays, in which the
   inner array sizes can be a function of their indices.  In
   particular, it is our intention to support arrays like these:

   .. code-block:: chapel

       var Triangle: [row in 1..n] [1..row] real;
       var HierArr: [lvl in 1..n] [1..2**lvl, 1..2**lvl] real;

   For the time being, such cases must be implemented by wrapping the
   array elements in records, as shown above.

*/
