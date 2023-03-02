use DistributedMap;

var m = new distributedMap(int, int);

for i in 1..20 {
  m.add(i, -i);
}

var keySum: int = 0;
var valSum: int = 0;
// Since the keys can be stored in any order, don't depend on the output being
// in a particular order.  Instead, sum them to make sure they are all there.
for (k, v) in zip(m.keys(), m.values()) {
  if (k != -v) {
    writeln("Error!  Stored the wrong value for the key!");
  }
  keySum += k;
  valSum += v;
}
writeln(keySum == 210);
writeln(valSum == -210);
