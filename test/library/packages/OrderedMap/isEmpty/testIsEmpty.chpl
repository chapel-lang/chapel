use OrderedMap;



var m = new orderedMap(int, int, false, defaultComparator);
assert(m.isEmpty());

for i in 1..3 do
  m.add(i,i);
assert(!m.isEmpty());

for i in 1..3 do
  m.remove(i);
assert(m.isEmpty());
