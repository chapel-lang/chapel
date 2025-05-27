//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//
use Sort;

/* START_EXAMPLE */
var Array = [-1, -4, 2, 3];

// Empty record serves as comparator
record absComparator : relativeComparator{ }

// compare method defines how 2 elements are compared
proc absComparator.compare(x, y) {
  return abs(x) - abs(y); // ascending order
}

var absReverseComparator = new reverseComparator(new absComparator()); // reverse order

sort(Array, comparator=absReverseComparator);

// This will output: -4 3 2 -1
writeln(Array);
/* STOP_EXAMPLE */
