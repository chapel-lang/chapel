use OrderedMap;

config param impl: mapImpl;

var m = new orderedMap(int, int, false, defaultComparator, impl);

for i in 1..10 {
  m.add(i, i);
}

for i in 1..20 by -1 {
  m.remove(i);
}

writeln(m);
