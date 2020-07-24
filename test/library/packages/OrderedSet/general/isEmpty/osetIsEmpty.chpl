use OrderedSet;
config param impl: setImpl;
var oset = new orderedSet(int, false, defaultComparator, impl);
writeln(oset.isEmpty());
oset.add(1);
writeln(oset.isEmpty());
oset.clear();
writeln(oset.isEmpty());
