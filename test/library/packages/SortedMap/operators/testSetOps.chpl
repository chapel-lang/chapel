use SortedMap;



var m1 = new sortedMap(int, int, false, new DefaultComparator());
var m2 = new sortedMap(int, int, false, new DefaultComparator());

m1[1] = 1;
m1[2] = 2;
m1[3] = 3;
m2[1] = -1;
m2[3] = -3;
m2[5] = -5;

writeln((m1 + m2).toArray());
writeln((m1 | m2).toArray());
writeln((m1 & m2).toArray());
writeln((m1 - m2).toArray());
writeln((m1 ^ m2).toArray());
