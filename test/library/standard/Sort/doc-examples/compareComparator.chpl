//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//
use Sort;

/* START_EXAMPLE */
var Array = [-1, -4, 2, 3];

// Empty record serves as comparator
record absComparator : relativeComparator { }

// compare method defines how 2 elements are compared
proc absComparator.compare(x, y) {
  return abs(x) - abs(y);
}

var absoluteComparator: absComparator;

sort(Array, comparator=absoluteComparator);

// This will output: -1 2 3 -4
writeln(Array);
/* STOP_EXAMPLE */
