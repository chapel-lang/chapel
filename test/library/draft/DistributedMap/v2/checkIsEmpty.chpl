use DistributedMap;

var m = new distributedMap(int, int);
writeln(m.isEmpty()); // Should be true

// Would be interesting to do this in parallel, but am not for now
for i in 1..20 {
  m.add(i, -i);
}

writeln(m.isEmpty()); // Should be false
