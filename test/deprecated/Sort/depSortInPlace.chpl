use Sort;

var arr: [0..15] int =
  [3, 2, 15, 2, 23, 5, 8, -2, 9, 3, 34, 100000, 26, 61, -1, 0];
sort(arr, inPlaceAlgorithm=true);
writeln(arr);

var arr2: [0..15] int =
  [3, 2, 15, 2, 23, 5, 8, -2, 9, 3, 34, 100000, 26, 61, -1, 0];
sort(arr2, new DefaultComparator(), false, true);
writeln(arr2);

var arr3: [0..15] int =
  [3, 2, 15, 2, 23, 5, 8, -2, 9, 3, 34, 100000, 26, 61, -1, 0];
sort(Data=arr3);
writeln(arr3);
