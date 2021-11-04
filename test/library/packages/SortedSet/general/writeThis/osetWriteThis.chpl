use OrderedSet;

var oset = new orderedSet(int, false, defaultComparator);
for i in 1..3 {
  oset.add(i);
  writeln(oset);
}
