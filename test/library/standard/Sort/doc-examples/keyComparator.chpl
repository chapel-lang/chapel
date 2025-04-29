//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//
use Sort;

/* START_EXAMPLE */
var Array = [-1, -4, 2, 3];

// Empty record serves as comparator, implements the keyComparator interface
record absComparator : keyComparator { }

// key method maps an element to the value to be used for comparison
proc absComparator.key(elt) { return abs(elt); }

var absoluteComparator: absComparator;

sort(Array, comparator=absoluteComparator);

// This will output: -1 2 3 -4
writeln(Array);
/* STOP_EXAMPLE */
