use DistributedMap, Map;

var m = new distributedMap(int, int);

for i in 1..20 {
  m.add(i, -i);
}

try {
  writeln(m.getValue(23));
} catch e: KeyNotFoundError {
  writeln(e.message());
} catch e {
  writeln("uh oh, caught different error: " + e.message());
}

writeln(m.getValue(15)); // Should be -15
writeln(m.getValue(2)); // Should be -2
writeln(m.getValue(20)); // Should be -20
writeln(m.getValue(1)); // Should be -1
writeln(m.getValue(15)); // Should still be -15
