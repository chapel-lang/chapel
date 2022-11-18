use DistributedMap;

var m = new distributedMap(int, int);
var res1 = m.remove(5); // Should be false but finish anyways
writeln(res1);
writeln(m.contains(5)); // Should also be false
m.add(5, 14);
writeln(m.contains(5)); // Should be true now
var res2 = m.remove(5); // Should be true now
writeln(res2);
writeln(m.contains(5)); // Should be false again
