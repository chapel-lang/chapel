/*
 * Range Example
 *
 * This example demonstrates uses of the range type and operations on
 * ranges.
 *
 */

//
// Ranges represent sequences of integral numbers.  A range literal is
// defined using the '..' operator.  The lower bound is always on the left;
// the upper bound is always on the right.
//
writeln("Basic range");
var r = 1..10;
writeRange(r);

//
// Ranges are useful building blocks for iteration.  Here we use a
// for-loop to iterate over the values in range 'r' and compute the
// sum of the values.
//
var sum: int;
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
// Ranges are also the building blocks for domains and arrays.  Here
// we use the range r to build up a 2D domain and an array.
//
writeln("Domains and arrays");
const D: domain(2) = [r, r];
var A: [D] real;
writeln("D = ", D);
writeln("Array A");
writeln(A);
writeln();

//
// Ranges don't have to be fully bounded.  A range without a low bound
// represents an infinite number of values that are less than or equal to
// the high bound.  Similarly a range without a high bound represents
// an infinitude of values greater or equal to the low bound.  A range
// with no bounds is allowed as well.  Unbounded ranges are useful for
// zippered iteration and range slicing.
//
writeln("Unbounded ranges");
writeRange(1..);
writeRange(..5);
writeRange(..);
write("Iterating over (312..315, 1..) = ");
for (v, i) in (312..315, 1..) {
  write((v, i), " ");
}
writeln();
writeln();

//
// There are several operators for working with ranges:  
//   =  by  #  align  +  -  ==
//
// Operators such as '#' and 'by' can be
// combined to make more complex range values.
//

//
// The 'by' operator applies a stride to a range.  If the range was already
// strided, the effect is multiplicative.  If the stride is negative,
// the range starts at the high bound and counts backward to the low
// bound.  It is an error to apply a positive stride to a range with
// no lower bound or a negative stride to a range with no upper bound.
//
writeln("The by operator");
writeRange(r by 2);
writeRange(r by 2 by 2);
writeRange(r by -1);
writeRange(5.. by 2);
writeln();

//
// Another range operator is the count operator '#'.  It is used to
// count off a number of elements from the start of a range.  
// If the count is negative, the elements are taken from the end of the
// range, instead.  It is an error to take a positive count of indices
// from a range with no starting index, or a negative count of indices
// from a range with no ending index.
//
writeln("The count operator");
var numElements = 5;
writeRange(0..#numElements);
writeRange(r # 4);
writeRange(r by -1 # 4);
writeRange(..5 # -3);
writeln();

// 
// When defining a strided range, it is useful to specify the alignment.
// The align operator makes it easy to do so.
// These two ranges specify all odd and even numbers, respectively.
var odds = .. by 2 align 1;
var evens = .. by 2 align 2;
// The alignment is always taken modulo the stride, so one could also say:
//  var evens = .. by 2 align 0;
//  var evens = .. by 2 align -2;
//  etc.
//
writeln("The align operator");
var oddsBetween1and10 = r by 2 align 1;
writeRange(oddsBetween1and10);
var evensBetween1and10 = r by 2 align 0;
writeRange(evensBetween1and10);
writeln();

//
// The '+' and '-' operators are used to add or subtract a scalar from
// the high and low bounds of a range, shifting it higher or lower.
//
writeln("Arithmetic operators");
writeRange(r + 2);
writeRange(1 + ..5);
writeRange((r by 2) - 1);
writeln();

//
// A range can be "sliced" by another range to create a range that
// represents the intersection of the original two.  Slicing a range
// is accomplished by treating the range as a function and passing it
// a second range.
//
var r2 = 1..20 by 3;
writeln("Range Slicing");
writeRange(r(2..9));
writeRange(r(2.. by 2));
writeRange(r2(1..20 by 2));
// The odds between 1 and 10 (using range slicing):
writeRange(r(odds));
// The evens between 1 and 10 (using range slicing):
writeRange(r(evens));
writeln();

//
// The == operator can be used to test if two ranges are equivalent.
// Equivalence means they produce the same sequence when iterated.
//
writeln("Range equivalence");
writeln(r(odds) == oddsBetween1and10);          // true
writeln(r(evens) == evensBetween1and10);        // true
writeln();

//
// The function that has been used throughout this example to print
// ranges is defined below.  It checks the boundedType of the range
// argument and if the range is fully bounded, all of the values are
// printed.  If the range has only one bound, it prints the three
// values nearest to the bound.  If it is fully unbounded, it just
// prints "all integers".
//
proc writeRange(r: range(?)) {
  write("Range ", r, " = ");
  select r.boundedType {
    when BoundedRangeType.bounded {
      var first: bool = true;;
      for i in r {
        if !first then write(", ");
        write(i);
        first = false;
      }
      writeln();
    }
    when BoundedRangeType.boundedLow {
      for i in r # 3 do
        write(i, ", ");
      writeln("...");
    }
    when BoundedRangeType.boundedHigh {
      write("...");
      for i in r # -3 do
        write(", ", i);
      writeln();
    }
    when BoundedRangeType.boundedNone {
      writeln("all integers");
    }
  }
}
