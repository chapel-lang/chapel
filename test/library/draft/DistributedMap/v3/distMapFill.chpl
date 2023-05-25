use DistributedMap;

var dm = new distributedMap(string, int);

for s in "abcdefghijklmnopqrstuvwxyz".items() {
    dm.add(s, s.toByte());
}

writeln(dm);

writeln(dm["b"]);
dm["b"] += 1;
writeln(dm["b"]);
