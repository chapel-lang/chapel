use OrderedSet;
config param impl: setImpl;
var oset = new orderedSet(int, false, defaultComparator, impl);
for i in 1..3 {
  oset.add(i);
  writeln(oset);
}
