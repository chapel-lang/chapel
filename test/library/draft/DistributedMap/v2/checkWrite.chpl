use DistributedMap;

var m = new distributedMap(int, int);

for i in 1..20 {
  m.add(i, -i);
}
// Uses script to check and sort output, since map can store in any order
writeln(m);
