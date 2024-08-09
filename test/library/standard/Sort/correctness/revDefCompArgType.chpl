use Sort;

config param useCorrectType = true;

proc doSort(ref x: [] int, comp: reverseDefaultComparator) {
  sort(x, comp);
  writeln(x);
}

var comp1: reverseDefaultComparator = new ReverseComparator();
var arr: [0..6] int = [-2, 8, -3, -1, 0, 10, 17];

if useCorrectType {
  doSort(arr, comp1);
} else {
  doSort(arr, new DefaultComparator());
}

