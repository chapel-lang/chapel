use SortedSet;

var oset = new sortedSet(int, false, new DefaultComparator());
writeln(oset.isEmpty());
oset.add(1);
writeln(oset.isEmpty());
oset.clear();
writeln(oset.isEmpty());
