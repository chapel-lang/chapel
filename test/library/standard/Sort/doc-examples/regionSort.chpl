//
// THIS TEST CASE IS AUTO-INCLUDED IN THE DOCUMENTATION
//
use Sort;

/* START_EXAMPLE */
var Array = [3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5];

// Sort only the elements in the range 1..5
// Same as sort(Array[1..5]);
sort(Array, comparator=new defaultComparator(), region=1..5);

// This will output: 3 1 1 4 5 9 2 6 5 3 5
writeln(Array);
/* STOP_EXAMPLE */
