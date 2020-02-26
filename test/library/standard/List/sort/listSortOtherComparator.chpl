//
//
//
use List;
use Sort;

// Verify that the default comparator still works.
var lst1: list(int) = [8, 7, 6, 5, 1, 2, 3, 4];
lst1.sort();
writeln(lst1);
lst1.clear();

var lst2: list(int) = [8, 7, 6, 5, 1, 2, 3, 4];
lst2.sort(Sort.reverseComparator);
writeln(lst2);
lst2.clear();

