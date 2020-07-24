use OrderedMap;

config param impl: mapImpl;

var m = new orderedMap(int, int, false, defaultComparator, impl);
for i in 1..10 {
  m.addOrSet(i, i);
}

for i in 1..10 {
  assert(m[i] == i);
  m.addOrSet(i, i+1);
  assert(m[i] == i+1);
}
