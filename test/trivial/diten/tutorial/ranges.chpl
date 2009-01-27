/*
 * Range Example
 *
 * An example showing operations on ranges.
 *
 */

//
// Ranges represent sequences of integral numbers. A range literal is defined
// using the '..' operator.
//
writeln("Defining r");
var r = 1..10;
writeRange(r);

//
// The values in a range can be iterated over using a for loop.
//
var sum: int;
for i in r do
  sum += i;
writeln("The sum from ", r, " is ", sum);

//
// Ranges don't have to be fully bounded. A range without a low bound
// represents an infinite number of values that are lesser or equal to the
// high bound. Similarly a range without a high bound represents an infinity
// of values greater or equal to the low bound. A range with no bounds is
// allowed as well.
//
writeln();
writeln("Unbounded ranges");
writeRange(1..);
writeRange(..5);
writeRange(..);

//
// There are several operators for working with ranges. The 'by' operator
// applies a stride to a range. If the range was already strided, the effect
// is multiplicative. If the stride is negative, the range starts at the
// high bound and counts backward to the low bound. It is an error to apply
// a stride to a range with no bounds.
//
writeln();
writeln("The by operator");
writeRange(r by 2);
writeRange(r by 2 by 2);
writeRange(r by -1);
writeRange(5.. by 2);


//
// Another range operator is the count operator '#'. It is used to count a
// number of elements from a range. It is an error to count a range with no
// bounds. The primary intended use of the count operator is to avoid having
// subtract one from the number of elements when creating a zero based range.
// Operators such as '#' and 'by' can be combined to make more complex
// range values.
//
writeln();
writeln("The count operator");
var numElements = 5;
writeRange(0..#numElements);
writeRange(r # 4);
writeRange(r by -1 # 4);
writeRange(..5 # 3);
writeRange(..5 by 2 # 3);

//
// The '+' and '-' operators are used to add or subtract a scalar from the
// high and low bounds of a range, shifting it higher or lower.
//
writeln();
writeln("Arithmetic operators");
writeRange(r + 2);
writeRange(1 + ..5);
writeRange((r by 2) - 1);

//
// A range can be "sliced" by another range to create a range that represents
// the intersection of the original two. Slicing a range is accomplished by
// calling it like a function with the range to slice it with as an argument.
//
var r2 = 1..20 by 3;
writeln();
writeln("Range Slicing");
writeRange(r(2..9));
writeRange(r(2.. by 2));
writeRange(r2(1..20 by 2));

//
// The function that has been used throughout this example to print ranges
// is defined below. It checks the boundedType of the range argument and if
// the range is fully bounded, all of the values are printed. If the range
// has only one bound, it prints the three values nearest to the bound.
// If it is fully unbounded, it just prints "..".
//
def writeRange(r: range(?)) {
  select r.boundedType {
    when BoundedRangeType.bounded {
      for i in r do
        write(i, " ");
      writeln();
    }
    when BoundedRangeType.boundedLow {
      for i in r # 3 do
        write(i, " ");
      writeln("..");
    }
    when BoundedRangeType.boundedHigh {
      write(".. ");
      for i in r # 3 do
        write(i, " ");
      writeln();
    }
    when BoundedRangeType.boundedNone {
      writeln("..");
    }
  }
}
