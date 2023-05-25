use DistributedMap;

var m = new distributedMap(int, int);

m.add(3, -14);

writeln(m.contains(3));  // Should be true
var val = m(3);
m.remove(3);
writeln(val);            // Should be -14
writeln(m.contains(3));  // Should be false
