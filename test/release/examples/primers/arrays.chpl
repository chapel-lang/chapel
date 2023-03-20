// Arrays

//
// This primer is a tutorial on Chapel's rectangular arrays and domains.
//

//
// Arrays in Chapel are specified using a square-bracketed expression
// that specifies the array's index set, followed by the array's
// element type.  Rectangular arrays are those whose indices are
// integers or tuples of integers, bounded by a range in each
// dimension, supporting multidimensional, rectilinear index sets.
//

config const n = 5;

//
// Declare an 'n'-element array of 64-bit real values (where 'n' is 5
// by default):
//
var A: [1..n] real;

//
// Like other variable types in Chapel, arrays are initialized so that
// each element stores its default value.  So our array of real values
// above will default to an array whose elements each store the value
// 0.0.
//

writeln("Initially, A is: ", A);

//
// Arrays can also be declared using array literals.  These are
// specified by enclosing a comma-separated list of expressions in
// square brackets.  The domain of the array will be 0-based, and the
// type of the array's elements will be that of the expressions if
// they all have the same type.  If they do not, the array's element
// type will be a type that can hold all of the values, if the
// compiler can determine that one exists.
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
// array elements, or *sub-arrays*, using a technique called slicing:
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
// Note: further information on slicing can be found in the
// :ref:`Slices Primer <primers-slices>`
//

//
// Arrays can be multidimensional as well.  For example, the following
// declaration creates a 2D 'n' x 'n' array of ``real`` floating point
// values.
//

var B: [1..n, 1..n] real;

forall (i,j) in {1..n, 1..n} do
  B[i,j] = i + j/10.0;

writeln("Initially, B is:\n", B, "\n");

//
// An array's elements can be iterated over using Chapel's loop forms
// (e.g., ``for``, ``foreach``, or ``forall``), which causes the index
// variable to refer to an array element in each iteration.  For
// example, the following loop increments each of 'B's elements by 1,
// in parallel:
//

forall b in B do
  b += 1;

writeln("After incrementing B's elements, B is:\n", B, "\n");

//
// An array's index set is referred to as a *domain* — a first-class
// language concept that stores the set of indices used to access the
// array.  The arrays 'A' and 'B' above are respectively declared with
// the anonymous domains ``{1..n}`` and ``{1..n, 1..n}``.  Array 'A2'
// above is declared with the implicit domain ``{0..4}``.

// An array's domain can be queried using the ``.domain`` method,
// which returns a ``const ref`` to the domain in question.  For
// example, here's a loop that iterates over B's indices in parallel
// by querying the domain:
//

forall (i,j) in B.domain do
  B[i,j] -= 1;

writeln("After decrementing B's elements, B is:\n", B, "\n");

//
// Domains can also be queried when arrays are passed to routines, as
// a means of associating a new ``const ref`` identifier with the
// domain for the routine's duration.  For example, the following
// procedure queries the domain of its array argument 'X', naming it
// 'D':
//

proc negateAndPrintArr(X: [?D] real) {
  writeln("within negateAndPrintArr, D is: ", D, "\n");
  forall (i,j) in D do
    X[i,j] = -X[i,j];
  writeln("after negating X within negateAndPrintArr, X is:\n", X, "\n");
}

negateAndPrintArr(B);

//
// Arrays are passed to routines by reference (``ref``) by default, so
// the modifications to 'X' in procedure 'negateAndPrintArr()' are
// reflected back in the actual argument 'B' as well:
//

writeln("After calling negateAndPrintArr, B is:\n", B, "\n");

//
// Domains can also be declared and named.  This has several
// advantages, including: allowing multiple arrays to share a single
// domain; associating a logical name with an index set; amortizing
// the overheads associated with storing distributed arrays across
// multiple array variables of the same size and shape; and enabling
// compiler optimizations.
//
// The following domain declaration defines a 2D rectangular domain
// called 'ProbSpace', which is the same size and shape as 'B' was
// above.
//

const ProbSpace = {1..n, 1..n};

// Note that we declare the domain as being ``const``, indicating that
// we will never change the set of indices it represents.  Besides
// indicating the programmer's intent, this can enable key compiler
// optimizations, so is recommended whenever a domain's index set is
// known to be unchanging.
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
// When indexing over a multidimensional domain, the indices can be
// expressed using a single tuple variable rather than destructuring
// the tuple into its integer components.  Similarly, multidimensional
// array accesses can be expressed using tuple indices rather than
// multiple integer arguments.  In the following example, the index
// variable 'ij' stores a 2-tuple of integers (``2*int`` in Chapel).
// Note the use of tuple indexing to tease the individual components
// of out of the 2-tuple 'ij'.
//

for ij in ProbSpace do
  D[ij] = ij(0) == ij(1);

writeln("After assigning D, its value is:\n", D, "\n");


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

B = 0.0;

writeln("After being reset, B is:\n", B, "\n");

//
// An array need not be indexed using the domain that was used to
// declare it, though doing so presents the compiler with
// opportunities to optimize bounds checks away.  In the following
// loop, there is no known relation between 'B' and 'ProbSpace', so
// bounds checks are harder to prove away (since it requires symbolic
// analysis of the definitions of the two domains and the invariance
// of their bounds).
//

for (i,j) in ProbSpace do
  B[i,j] = i + j/10.0;

writeln("B has been re-initialized to:\n", B, "\n");

//
// Whole-array assignment can also be used for arrays or sub-arrays
// whose index sets differ as long as they have the same number of
// dimensions and shape (number of elements per dimension).
//

var F, G: [ProbSpace] real;

F[2..n-1, 2..n-1] = B[1..n-2, 3..n];

writeln("After assigning a slice of B to a slice of F, F's value is:\n", F, "\n");

//
// Arrays can also be sliced using unbounded ranges in which either
// the low and/or high bounds are omitted.  In this case, the missing
// bounds are defined by the array's bounds.  For example, the
// following statement assigns all rows of 'G' starting from row 2
// using all rows of 'B' up to number 'n-1'.  It assigns all columns
// since no bounds are provided in the second dimension.
//

G[2.., ..] = B[..n-1, ..];

writeln("After assigning a slice of B to G, G's value is:\n", G, "\n");

//
// Array slicing supports rank-change semantics when sliced using a
// scalar value rather than a range.  In the following assignment,
// recall that 'A' was our initial 1-dimensional array.  The slice
// of 'B' takes all columns of row 'n/2' and treats it as a 1D array.
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

//
// Forall loops over domains and arrays can be written using the
// syntax ``[<ind> in <Dom>] ...`` which is shorthand for
// ``forall <ind> in <Dom> do ...`` Forall loops are discussed
// in the :ref:`foralls primer (forallLoops.chpl)<primers-forallLoops>`.
//

const offset = (1,1);  // a 2-tuple offset

[ij in ProbSpace[2..n-1, 2..n-1]] F[ij] = B[ij + offset];

writeln("After assigning F a shifted slice of B, it is:\n", F, "\n");

[b in B] b = -b;

writeln("After negating B, it is:\n", B, "\n");

//
// Note that this loop shorthand resembles the array type definition
// in a variable declaration.
//

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
// Now, if we reassign 'VarDom', 'VarArr' will be reallocated with the
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
// to assign its domain variable a degenerate domain (e.g. ``{1..0}``)
// and then assign it the new value:
//

VarDom = {1..0};  // empty the array such that no values need to be preserved

writeln("VarArr is now empty: ", VarArr, "\n");

VarDom = {1..n};  // assign the new size

writeln("VarArr should now be reset: ", VarArr, "\n");

// Note that querying an array's domain via the ``.domain`` method or
// the function argument query syntax does not result in a domain
// expression that can be reassigned since those forms return a
// ``const ref`` to the domain.  In particular, we cannot do:
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
// declarations 'A' and 'B', we have no way of reallocating them.
// Arrays with constant domains provide the compiler with optimization
// benefits, so this design supports a common case efficiently.

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
// Note: further information on records can be found in the
// :ref:`Records Primer <primers-records>`
//

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

//
// For further information, see the :ref:`Domain Primer <primers-domains>`
// and other array primers: :ref:`Sparse <primers-sparse>`,
// :ref:`Associative <primers-associative>`.
//
