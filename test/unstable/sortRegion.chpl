use Sort;

var A:[1..10] int = 1..10 by -1;

sort(A, new defaultComparator(), 2..5);
assert(&& reduce (A == [10, 6, 7, 8, 9, 5, 4, 3, 2, 1]));
