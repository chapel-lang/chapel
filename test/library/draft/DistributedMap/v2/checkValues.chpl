use DistributedMap;

var m = new distributedMap(int, int);

for i in 1..20 {
  m.add(i, -i);
}

var valSum: int = 0;
// Since the keys can be stored in any order, don't depend on the output being
// in a particular order.  Instead, sum the values to make sure they are all
// there.
for v in m.values() {
  valSum += v;
}
writeln(valSum == -210);
