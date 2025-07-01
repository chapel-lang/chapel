//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//
use Sort;

/* START_EXAMPLE */
var Array = [-1, -4, 2, 3];

// Using module-defined 'reverseComparator'
sort(Array, comparator = new reverseComparator());

// This will output: 3 2 -1 -4
writeln(Array);
/* STOP_EXAMPLE */
