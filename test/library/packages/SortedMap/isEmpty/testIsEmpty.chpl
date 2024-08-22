use SortedMap;



var m = new sortedMap(int, int, false, new DefaultComparator());
assert(m.isEmpty());

for i in 1..3 do
  m.add(i,i);
assert(!m.isEmpty());

for i in 1..3 do
  m.remove(i);
assert(m.isEmpty());
