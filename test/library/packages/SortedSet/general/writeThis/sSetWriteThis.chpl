use SortedSet;

var oset = new sortedSet(int, false, new defaultComparator());
for i in 1..3 {
  oset.add(i);
  writeln(oset);
}
