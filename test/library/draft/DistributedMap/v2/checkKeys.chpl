use DistributedMap;

var m = new distributedMap(int, int);

for i in 1..20 {
  m.add(i, -i);
}

var keySum: int = 0;
// Since the keys can be stored in any order, don't depend on the output being
// in a particular order.  Instead, sum them to make sure they are all there.
for k in m.keys() {
  keySum += k;
}
writeln(keySum == 210);
