use Sort;

var comp1: reverseDefaultComparator = new ReverseComparator();

var arr1: [0..4] int = [3, 21, 1, 2, 9];

sort(arr1, comp1);
writeln(arr1);
