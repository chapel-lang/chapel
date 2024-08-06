use SortedSet;

var oset = new sortedSet(int, false, new DefaultComparator());
for i in 1..3 {
  oset.add(i);
  writeln(oset);
}
