use DistributedMap;

var m = new distributedMap(int, int);

m.addOrSet(3, 17);
m.add(11, 2);
writeln(m.contains(3)); // should be true
writeln(m.contains(11)); // should be true
m.addOrSet(11, -4);
writeln(m.contains(11)); // should still be true
writeln(m.contains(3)); // should be true

var valSum: int = 0;
for v in m.values() {
  valSum += v;
}
writeln(valSum == 13); // Because values should be 17 and -4 only
