use DistributedMap, IO;

var m = new distributedMap(int, int);

var ch = openReader("checkRead.txt");
ch.read(m);

writeln(m.size); // Should be 4
writeln(m.contains(3)); // Should be true
writeln(m.contains(-7)); // Should be true
