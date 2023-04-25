use DistributedMap, Map;

var m = new distributedMap(int, int);

for i in 1..20 {
  m.add(i, -i);
}

try {
  writeln(m(23));
} catch e: KeyNotFoundError {
  writeln(e.message());
} catch e {
  writeln("uh oh, caught different error: " + e.message());
}

writeln(m(15)); // Should be -15
writeln(m(2)); // Should be -2
writeln(m(20)); // Should be -20
writeln(m(1)); // Should be -1
writeln(m(15)); // Should still be -15
