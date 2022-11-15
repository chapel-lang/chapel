use DistributedMap;

var m = new distributedMap(int, int);

// We're maybe going to rename this method
var wasPresent = m.set(11, 14);
writeln(wasPresent);      // Should be false
writeln(m.contains(11));  // Should be false

m.add(11, 14);
writeln(m.contains(11));  // Should now be true
writeln(m.getValue(11));  // Should be 14
wasPresent = m.set(11, -2);
writeln(wasPresent);      // Should be true
writeln(m.getValue(11));  // Should be -2, not 14
