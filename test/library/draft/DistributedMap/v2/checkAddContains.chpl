use DistributedMap;

var m = new distributedMap(int, int);

m.add(3, 17);
writeln(m.contains(3)); // should be true
writeln(m.contains(11)); // should be false
m.add(11, 2);
writeln(m.contains(11)); // should be true
writeln(m.contains(3)); // should be true
