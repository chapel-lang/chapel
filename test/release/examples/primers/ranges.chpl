// Ranges

/*
   This primer illustrates Chapel ranges and range operations.
*/

/*
  .. _primers-ranges-general:

  Range Basics
  ------------
*/
//
// Ranges represent regular sequences of values, such as integers, and
// are typically defined in terms of a low and high bound.  Range
// values can be specified using the ``..`` operator, where the low
// bound is specified on the left side of the ``..`` and the high
// bound is on the right.  These bounds are inclusive for ranges
// created with ``..``.
//
const n = 5,
      lo = -3,
      hi = 3;
writeln("Basic ranges");
const r = 1..10,    // 1, 2, 3, ..., 10
      r2 = 0..n,    // 0, 1, 2, ..., n
      r3 = lo..hi;  // -3, -2, -1, ..., 3
writeRange(r);
writeRange(r2);
writeRange(r3);
writeln();

// Ranges can also be defined using an `open-interval` form using the
// ``..<`` operator, causing the upper bound to be excluded from the
// values represented by this range.  For example, the range
// ``3..<10`` is equivalent to ``3..9``.  This form is particularly
// useful for 0-based indexing, to avoid bounds like ``n-1``.
//
writeln("Open interval ranges");
const rOpen = 1..<10,  // 1, 2, 3, ..., 9
      rOpen2 = 0..<n;  // 0, 1, 2, ..., n-1
writeRange(rOpen);
writeRange(rOpen2);
writeln();


// Note that whenever a range's low bound exceeds its high bound, the
// range is considered to be empty.
writeln("Empty ranges");
const empty1 = 1..0,
      empty2 = 0..-1,
      empty3 = 10..1;
writeRange(empty1);
writeRange(empty2);
writeRange(empty3);
writeln();

// To create a range representing a decreasing series of values, the
// ``by`` operator can be used with a negative stride.  As a simple
// example, the following range represents the integers counting from
// 10 down to 1.

writeln("Decreasing range");
const countDown = 1..10 by -1;  // 10, 9, 8, ..., 1
writeRange(countDown);
writeln();

// See the `Range Operators` section below for more information about
// strided ranges and the ``by`` operator.

/*
   Uses of Ranges
   --------------
*/

//
// Ranges are a basic building block for iteration.  The following
// for-loop iterates over the sequence represented by ``r``, computing
// the sum of the values in the sequence.
//
var sum = 0;
for i in 1..10 do       // compute 1 + 2 + 3 + ... + 10
  sum += i;
writeln("The sum of the values in '1..10' is ", sum);
writeln();

// Ranges can also drive parallel loop forms such as
// :ref:`forall<primers-forallLoops>` and
// :ref:`coforall<primers-taskparallel-coforall>` loops.

//
// Ranges also serve as the building block for rectangular
// :ref:`domains <primers-domains>` and :ref:`arrays
// <primers-arrays>`.  Here we use ranges to declare a 2D
// 10x10 domain and an array over it.
//
writeln("Domains and arrays");
const D = {1..10, 1..10};  // could also use '= {r, r};'
var A: [D] real;           // a 10x10 array of real floating point values
writeln("D = ", D);
writeln("Array A");
writeln(A);
writeln();

// Arrays can also be declared using ranges directly, which
// effectively creates an anonymous domain for the array:

var A1: [1..n] int,
    A2: [1..n, 1..n] string;

//
// Ranges can be used in general contexts that expect an iterable
// collection of values, such as reductions:
//
sum = + reduce r;  // compute 1 + 2 + 3 + ... + 10
writeln("The sum of the values in ", r, ", computed by reduce, is also ", sum);

// Or whole-array operations:
A1 = r;      // assign the elements of A1 their corresponding values of r
writeln(A1);
writeln();


/*
  Variations on Basic Ranges
  --------------------------
*/

//
// Ranges can be `unbounded` on one or both sides, when the
// respective bound is omitted.  For example, a range without a high
// bound represents values greater than or equal to the low bound.
//
writeln("Unbounded ranges");
writeRange(1..);   // 1, 2, 3, ...
writeRange(..5);   // ..., 3, 4, 5
writeRange(..);    // ...
writeln();

//
// Loops may iterate over an unbounded range as long as it has a
// well-defined first value to start from.  When an unbounded range of
// integers is zipped with another iterator expression, its size
// automatically conforms to the required size.
//
writeln("Iterating over zip(312..315, 1..) generates");
for (i, j) in zip(312..315, 1..) {
  write(" ", (i, j));
}
writeln();
writeln();

// Ranges can also be defined over boolean values and enumerated
// types.  In the case of enumerations, the range's sequence of values
// is based on the declaration order of the enum's constants rather
// than their (optional) numerical values.
//
writeln("Ranges over bools and enums:");
enum dir {north, south, east, west};
enum color {red=4, orange=2, yellow=1, green=3, blue=6, indigo=7, violet=5};
const boolRange = false..true,                 // false, true
      enumRange = dir.north..dir.west,         // north, south, east, west
      colorRange = color.orange..color.green;  // orange, yellow, green
writeRange(boolRange);
writeRange(enumRange);
writeRange(colorRange);
writeln();

// For boolean and enum ranges, the lack of a bound implies that the
// type's low/high value should be used in place of the missing bound.
//
writeRange(false..);         // like 'false..true'
writeRange(dir.south..);     // like 'dir.south..dir.west'
writeRange(..color.indigo by -1);  // like 'color.red..color.indigo by -1'
writeln();

/*
  .. _primers-ranges-operators:

  Range Operators
  ---------------
*/

// New ranges can be constructed from existing ones using the count,
// stride, and/or alignment operators: ``#``, ``by``, and ``align``.

//
// The count operator ``#`` applies an integer count, `c`, to a range
// and generates a new range representing `c` of the range's values.
// If the count is positive, the new range will represent the first
// `c` values; if it is negative, it represents the last `c` values in
// the range.  It is an error to apply a positive count of to a range
// with no first value (e.g., ``..10``), or a negative count to a
// range with no last value (e.g., ``1..``).
//
writeln("The count operator");
const numElements = 5;
writeRange(0..#numElements);  // 0, 1, 2, 3, 4
writeRange(r # 4);            // 1, 2, 3, 4
writeRange(..5 # -3);         // 3, 4, 5
writeln();

//
// The ``by`` and ``align`` operators are used to create `strided
// ranges` representing non-consecutive, evenly spaced values.
//

//
// The ``by`` operator applies a stride to a range, selecting a
// subsequence of its original values.  If the range was already
// strided, the effect is multiplicative.  If the stride is negative,
// the direction of the sequence is reversed.
//
writeln("Strided ranges using the by operator");
writeRange(r by 2);       // 1, 3, 5, 7, 9
writeRange(r by 2 by 2);  // 1, 5, 9
writeRange(r by -1);      // 10, 9, 8, ..., 1
writeRange(5.. by 2);     // 5, 7, 9, 11, ...
writeln();

writeln("Examples mixing # and by");
writeRange(r by 2 # 4);    // 1, 3, 5, 7
writeRange(r # 4 by 2);    // 1, 3
writeRange(r by -2 # 4);   // 10, 8, 6, 4
writeRange(r # 4 by -2);   // 4, 2
writeRange(r by 2 # -4);   // 3, 5, 7, 9
writeRange(r # -4 by 2);   // 7, 9
writeRange(r by -2 # -4);  // 8, 6, 4, 2
writeRange(r # -4 by -2);  // 10, 8
writeln();

// By default, a strided range's values are aligned with
// its first value.  Thus, for a positive stride, the values are
// aligned with its low bound; while for a negative stride, the
// values are aligned to its high bound.
//
writeln("Implicit alignment set using 'by'");
writeRange(1..10 by 2);  // values are aligned with 1: 1, 3, 5, 7, 9
writeRange(1..10 by -2); // values are aligned with 10: 10, 8, 6, 4, 2
writeRange(..5 by -3);   // values are aligned with 5: 5, 2, -1, -4, ...
const rangeWithAmbiguousAlignment = 1.. by -3;  // alignment undefined
writeln();

//
// The ``align`` operator can be used to explicitly specify the
// alignment of a strided range.  The indices in the aligned range are
// all equivalent to the specified alignment modulo the stride's
// absolute value.  For example, alignment can be used to define
// sequences representing odd numbers versus even numbers.
//
writeln("Range alignment and the align operator");
const oddsBetween1and10  = r by 2 align 1,  // 1, 3, 5, 7, 9
      evensBetween1and10 = r by 2 align 2;  // 2, 4, 6, 8, 10
writeRange(oddsBetween1and10);
writeRange(evensBetween1and10);

const allOdds = .. by 2 align 1,
      allEvens = .. by 2 align 2;
writeRange(allOdds);
writeRange(allEvens);
writeln();
//
// The alignment is always taken modulo the stride, so one could also
// declare 'allEvens' using `.. by 2 align 0`, `.. by 2 align 12`, or
// any other even number as the alignment.:
//



//
// The ``+`` and ``-`` operators can be used to create a new range
// from an existing one, representing a shift in the range's sequence
// of values.
//
writeln("Operators + and -");
writeRange(r + 5);          // 6..15
writeRange(r - 3);          // -2..7
writeRange((r by 2) - 1);   // 0..9  by 2
writeRange(1 + ..5);        // ..6
writeln();

//
// The ``==`` operator can be used to test whether two ranges are
// equal.  Equality means they represent the same sequence of indices.
//
writeln("Range equality");
writeln(r == 1..10);                               // true
writeln((1..10 by 2) == (1..9 by 2));              // true
writeln(r == (r by 2));                            // false
writeln(oddsBetween1and10 != evensBetween1and10);  // true
writeln();

/*
  .. _primers-ranges-slicing:

  Range Slicing (Intersection)
  ----------------------------
*/

//
// `Range slicing` is when one range is indexed using another range.
// This computes the intersection of the two ranges values.  Slicing
// is commutative in most respects.
//
writeln("Range slicing");
writeln("A slice of ", r, " with ", 2..7);
writeRange(r[2..7]);  // 2..7
const r1 = 5..15;
writeln("A slice of ", r, " with ", r1);
writeRange(r[r1]);    // 5..10
writeln("A slice of ", r1, " with ", r, " is the same");
writeRange(r1[r]);    // 5..10

//
// Computing with the odds and evens between 1 and 10 (using range
// slicing):
//
writeRange(r[allOdds]);   // 1, 3, 5, 7, 9
writeRange(r[allEvens]);  // 2, 4, 6, 8, 10

writeln(r[allOdds] == oddsBetween1and10);    // true
writeln(r[allEvens] == evensBetween1and10);  // true

//
// Either or both of the ranges in a range slicing operation can be
// strided and/or unbounded
// When the slicing range has a negative stride, the direction of
// the original range is reversed.
// As of Chapel Release 1.30, this feature requires compiling
// with ``-snewSliceRule``. In a subsequent release this behavior
// will be standard and the option ``-snewSliceRule`` will be removed.
//
const rs = 1..20 by 3;
writeln("A slice of ", rs, " with ", 1..20 by 2);
writeRange(rs[1..20 by 2]);  // 1, 7, 13, 19
writeln("A slice of ", rs, " with ", 1..20 by -2);
writeRange(rs[1..20 by -2]); // 16, 10, 4
writeln();

writeln("A slice of ", r, " with ", 5..);
writeRange(r[5..]);       // 5, 6, 7, 8, 9, 10
writeln("A slice of ", r, " with ", 5.. by 2);
writeRange(r[5.. by 2]);  // 5, 7, 9
writeln("A slice of ", 1.., " with ", ..5);
writeRange((1..)[..5]);   // 1, 2, 3, 4, 5
writeln();

// Range Types
// -----------
//
// Range types are generic with respect to three fields:
//
// * ``idxType``: The type of the range's values (defaults to ``int``)
// * ``bounds``: A :enum:`boundKind` value indicating which bounds the range stores (defaults to ``boundKind.both``)
// * ``stridable``: A ``bool`` indicating whether or not the range can have a stride other than 1 (defaults to ``false``)
//
// Like other variables, range types can be inferred by the compiler
// from the initializing expression for simplicity, as in the previous
// examples.  However, they can also be specified if desired.  Here
// are some examples that are equivalent to ones we've seen before:
//
const rt: range(int) = 1..10,
      rt2: range(int, bounds=boundKind.both, strides=strideKind.one)
         = 1..10,
      rte: range(color) = color.orange..color.green,
      rts: range(strides=strideKind.any) = 1..10 by 2,
      rtub: range(bounds=boundKind.low) = 1..;

// More importantly, range types can be used to make a range variable
// more flexible than its initializer permits.  For example, this
// range's initializer has a unit stride, but because its type is
// declared with ``stridable=true``, it can later be assigned a range
// value with a stride.

var rangeVar: range(int, strides=strideKind.any) = 1..10;

// Range types are also valuable in declaring formal arguments of a
// procedure in which you want to leave certain aspects of the range
// constrained or unconstrained.

proc acceptsNonStridedIntRangesOnly(r: range(int)) { }
proc acceptsAnyRange(r: range(?)) { }

acceptsNonStridedIntRangesOnly(1..10);
acceptsAnyRange(1..10);
acceptsAnyRange(1..10 by 2);
acceptsAnyRange(color.orange..color.green);

/*
  .. _primers-ranges-writerange:

  Helper function for this primer: writeRange()
  ---------------------------------------------
*/

//
// The procedure that has been used throughout this primer to print
// ranges is defined below. It adjusts to the specifics of the range.
//
proc writeRange(r: range(?)) {
  write("Range ", r);
  if r.bounds == boundKind.both ||
    ((isBoolType(r.idxType) || isEnumType(r.idxType)) && r.hasFirst()) {
      // The range is fully bounded, either because it is a bounded
      // range or because it is unbounded but defined on bools or an
      // enum, so - print its entire sequence.
    write(" = ");
    var first: bool = true;;
    for i in r {
      if !first then write(", ");
      write(i);
      first = false;
    }
  } else if r.hasFirst() {
    // The range is not fully bounded, but its sequence has a starting point
    // - print the first three indices.  Note that in this and the next
    // case the sequence can be either increasing or decreasing.
    write(" = ");
    for i in r # 3 do
      write(i, ", ");
    write("...");
  } else if r.hasLast() {
    // The range is not fully bounded, but its sequence has an ending point.
    // Print the last three indices.
    write(" = ...");
    for i in r # -3 do
      write(", ", i);
  } else if r.stride == 1 || r.stride == -1 {
    // If we are here, the range is fully unbounded.
    write(" = all integers, ",
          if r.stride > 0 then "increasing" else "decreasing");
  } else {
    // We got a more complex range, do not elaborate.
  }
  writeln();
}
