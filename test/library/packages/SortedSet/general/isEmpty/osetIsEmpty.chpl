use OrderedSet;

var oset = new orderedSet(int, false, defaultComparator);
writeln(oset.isEmpty());
oset.add(1);
writeln(oset.isEmpty());
oset.clear();
writeln(oset.isEmpty());
