use Sort;

var arr1: [0..5] int = [1, 3, 2, 4, 6, 5];
sort(arr1, defaultComparator);
writeln(arr1);

var arr2: [0..5] int = [1, 3, 2, 4, 6, 5];
sort(arr2, reverseComparator);
writeln(arr2);
