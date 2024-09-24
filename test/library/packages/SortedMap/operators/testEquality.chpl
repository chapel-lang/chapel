use SortedMap;



var m1 = new sortedMap(int, int, false, new DefaultComparator()),
    m2 = new sortedMap(int, int, false, new DefaultComparator());

for i in 1..10 {
  m1[i] = -i;
  m2[i] = -i;
}


writeln(m1 == m2);
writeln(m1 != m2);

m2[3] = 3;

writeln(m1 == m2);
writeln(m1 != m2);
