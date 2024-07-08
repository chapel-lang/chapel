use DistributedMap;

var m = new distributedMap(int, int);
writeln(m.size); // Should be 0

// Would be interesting to do this in parallel, but am not for now
for i in 1..20 {
  m.add(i, -i);
}

writeln(m.size); // Should be 20
