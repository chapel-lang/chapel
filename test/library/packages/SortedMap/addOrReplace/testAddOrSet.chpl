use SortedMap;



var m = new sortedMap(int, int, false, new DefaultComparator());
for i in 1..10 {
  m.addOrReplace(i, i);
}

for i in 1..10 {
  assert(m[i] == i);
  m.addOrReplace(i, i+1);
  assert(m[i] == i+1);
}
