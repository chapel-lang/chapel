// Ranges

/*
   This primer illustrates Chapel ranges and range operations.
*/

/*
  .. _primers-ranges-general:

  Ranges (general info)
  ---------------------
*/
//
// Ranges represent sequences of integral numbers, or "indices".
// A range literal is defined using the ``..`` operator.
// The lower bound is always on the left;
// the upper bound is always on the right.
//
writeln("Basic range r");
const r = 1..10;
writeRange(r);

// Ranges can also be defined using an `open-interval` form using the
// ``..<`` operator which causes the upper bound to be omitted.  For
// example, the range ``3..<10`` is equivalent to ``3..9``.
writeln("Open interval range rOpen");
const rOpen = 3..<10;
writeRange(rOpen);

// This form can be convenient when iterating over a 0-based
// data structure using ranges like ``lo..<data.size``.

//
// Ranges are a basic building block for iteration.  Here we use a
// for-loop to iterate over the sequence represented by ``r`` and
// compute the sum of the indices in the sequence.
//
var sum: int = 0;
for i in r do
  sum += i;
writeln("The sum of the values in ", r, " is ", sum);

//
// We can compute this same sum with a reduction over the range.
//
sum = + reduce r;
writeln("The sum of the values in ", r, " as computed by reduce is also ", sum);
writeln();

//
// Ranges are also the building blocks for domains.  Here
// we use the range ``r`` to build up a 2-D domain and an array.
//
writeln("Domains and arrays");
const D: domain(2) = {r, r};
var A: [D] real;
writeln("D = ", D);
writeln("Array A");
writeln(A);
writeln();

//
// Ranges don't have to be fully bounded.  A range without a low bound
// represents an infinite sequence of numbers that are less than or equal to
// the high bound.  Similarly a range without a high bound represents
// an infinitude of numbers greater or equal to the low bound.  A range
// with no bounds is allowed as well.
//
writeln("Unbounded ranges");
writeRange(1..);
writeRange(..5);
writeRange(..);

//
// When an unbounded range is used in a zippered iteration,
// only the necessary number of indices are taken from its sequence.
//
// Only ranges whose sequences have a starting point may be used in
// an iteration. This includes all the above ranges except ``..`` and ``..5``.
//
writeln("Iterating over (312..315, 1..) generates");
for (i, j) in zip(312..315, 1..) {
  write(" ", (i, j));
}
writeln();
writeln();

/*
  .. _primers-ranges-operators:

  Operators
  ---------
*/

//
// There are several operators for working with ranges.  The ``by`` and
// ``align`` operators create strided and aligned range values. The ``#``
// operator counts a number of elements from the range.  The ``+`` and
// ``-`` operators shift the sequence the range represents.  The ``==``
// operator compares ranges for equality.
//

//
// The ``by`` operator applies a stride to a range, selecting a subsequence
// from its sequence.  If the range was already strided, the effect is
// multiplicative.  If the stride is negative, the direction of the sequence
// is reversed.
//
writeln("Range stride and the by operator");
writeRange(r by 2);
writeRange(r by 2 by 2);
writeRange(r by -1);
writeRange(5.. by 2);
writeln();

//
// The ``align`` operator specifies the alignment of a strided range.  The
// indices in the aligned range are all equivalent to the specified
// alignment modulo the absolute value of the stride.  For example,
// alignment can differentiate the sequence of all odd numbers from all
// even numbers.
//
const allOdds = .. by 2 align 1;
const allEvens = .. by 2 align 2;
//
// The alignment is always taken modulo the stride, so one could also say:
//  ``const allEvens = .. by 2 align 0;``
//  ``const allEvens = .. by 2 align -2;``
// etc.
//
writeln("Range alignment and the align operator");
const oddsBetween1and10 = r by 2 align 1;
writeRange(oddsBetween1and10);
const evensBetween1and10 = r by 2 align 0;
writeRange(evensBetween1and10);

//
// If a range's stride is ``1`` or ``-1``, its sequence is always defined.
// Otherwise, the sequence is defined only when the range's alignment
// is defined, or "unambiguous". Without alignment, for example, it is
// unknown whether the sequence contains all odds or all evens.
// A defined sequence is required when using the range in an iteration
// or in many other cases.
//
// The ``align`` operator always defines the alignment, overriding
// the existing alignment, if any.
// When creating a strided range from a range literal, ``by`` will define
// the alignment to be the literal's low bound for positive stride,
// or the literal's high bound for negative stride.
// The corresponding bound must exist.
//
writeln("Implicit alignment set using 'by'");
writeRange(1..10 by -2);
writeRange(..5 by -3);
const rangeWithAmbiguousAlignment = 1.. by -3;
writeln();

//
// The count operator ``#`` counts off a number of elements from the start
// of a range. If the count is negative, the elements are taken from the end
// of the range, instead. It is an error to take a positive count of indices
// from a range with no starting index, or a negative count of indices
// from a range with no ending index.
//
writeln("The count operator");
const numElements = 5;
writeRange(0..#numElements);
writeRange(r # 4);
writeRange(r by -1 # 4);
writeRange(..5 # -3);
writeln();

//
// The ``+`` and ``-`` operators are used to shift a range's sequence
// higher or lower.
//
writeln("Operators + and -");
writeRange(r + 2);
writeRange(1 + ..5);
writeRange((r by 2) - 1);
writeln();

//
// The ``==`` operator can be used to test if two ranges are equal.
// Equality means they represent the same sequence of indices.
//
writeln("Range equality");
writeln(r(allOdds) == oddsBetween1and10);          // true
writeln(r(allEvens) == evensBetween1and10);        // true
writeln();

/*
  .. _primers-ranges-slicing:

  Slicing
  -------
*/

//
// Range slicing produces an intersection of the sequences defined
// by two given ranges. Slicing is commutative in most respects.
// It is written as indexing or application of one range to another.
//
writeln("Range slicing");
writeln("A slice of ", r, " with ", 2..7);
writeRange(r(2..7));
const r1 = 5..15;
writeln("A slice of ", r, " with ", r1);
writeRange(r(r1));
writeln("A slice of ", r1, " with ", r, " is the same");
writeRange(r1(r));

//
// The odds between 1 and 10 (using range slicing):
//
writeRange(r(allOdds));

//
// The evens between 1 and 10 (using range slicing):
//
writeRange(r(allEvens));

//
// Either or both of the ranges in a range slicing operation can be strided
//
const r2 = 1..20 by 3;
writeln("A slice of ", r2, " with ", 1..20 by 2);
writeRange(r2(1..20 by 2));
writeln("A slice of ", r2, " with ", 1..20 by -2);
writeRange(r2(1..20 by -2));
writeln();

//
// When an unbounded range is used in slicing a bounded range,
// the corresponding infinite sequence participates in the intersection.
//
writeln("A slice of ", r, " with ", 5..);
writeRange(r(5..));
writeln("A slice of ", r, " with ", 5.. by 2);
writeRange(r(5.. by 2));
writeln("A slice of ", 1.., " with ", ..5);
writeRange((1..)(..5));

/*
  .. _primers-ranges-writerange:

  Definition of writeRange
  ------------------------
*/

//
// The procedure that has been used throughout this primer to print
// ranges is defined below. It adjusts to the specifics of the range.
//
proc writeRange(r: range(?)) {
  write("Range ", r);
  if r.boundedType == BoundedRangeType.bounded {
    // The range is fully bounded - print its entire sequence.
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
