//
// Make sure `list.sort` is generic over multiple comparator types,
// not just "defaultComparator".
//
use List;
use Sort;

// Verify that the default comparator still works.
var lst1: list(int) = [8, 7, 6, 5, 1, 2, 3, 4];
lst1.sort();
writeln(lst1);
lst1.clear();

// Try sorting the list in reverse.
var lst2: list(int) = [8, 7, 6, 5, 1, 2, 3, 4];
lst2.sort(Sort.reverseComparator);
writeln(lst2);
lst2.clear();

//
// Follow along with the Sort documentation and make "our own"
// comparator (this is just absval).
//
record myComparator {}
proc myComparator.compare(a, b) {
  return abs(a) - abs(b);
}

var absComparator: myComparator;

// Let's sort a list of negative values with absval.
var lst3: list(int) = [-8, -7, -6, -5, -4, -3, -2, -1];
lst3.sort(absComparator);
writeln(lst3);

