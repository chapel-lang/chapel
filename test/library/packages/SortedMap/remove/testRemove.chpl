use SortedMap;



var m = new sortedMap(int, int, false, new DefaultComparator());

for i in 1..10 {
  m.add(i, i);
}

for i in 1..20 by -1 {
  m.remove(i);
}

writeln(m);
