use OrderedSet;
config param impl: setImpl;
var oset = new orderedSet(int, false, defaultComparator, impl);
for i in 1..10 {
  oset.add(i);
  writeln(oset.size);
}
for i in 1..10 {
  oset.remove(i);
  oset.remove(i + 10);
  writeln(oset.size);
}
