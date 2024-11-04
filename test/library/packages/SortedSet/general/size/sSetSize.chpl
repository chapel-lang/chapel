use SortedSet;

var oset = new sortedSet(int, false, new defaultComparator());
for i in 1..10 {
  oset.add(i);
  writeln(oset.size);
}
for i in 1..10 {
  oset.remove(i);
  oset.remove(i + 10);
  writeln(oset.size);
}
