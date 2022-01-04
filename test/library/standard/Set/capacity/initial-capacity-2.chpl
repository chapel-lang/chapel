use Set;

const initialCapacity = 100;

var s = new set(int,
                initialCapacity=initialCapacity,
                resizeThreshold=0.3);
var v = -1;
s.add(v);
s.remove(v);

var startingSize = s._htb.tableSize;
writeln(s._htb.tableSize);

// ensure that adding initialCapacity elements does
// not resize the map
for i in 0..#initialCapacity do
  s.add(i);
writeln(s._htb.tableSize);

assert(startingSize == s._htb.tableSize);

var s2 = new set(int,
                 initialCapacity=initialCapacity,
                 resizeThreshold=0.75);
s2.add(v);
s2.remove(v);

startingSize = s2._htb.tableSize;
writeln(s2._htb.tableSize);

// ensure that adding initialCapacity elements does
// not resize the map
for i in 0..#initialCapacity do
  s2.add(i);
writeln(s2._htb.tableSize);

assert(startingSize == s2._htb.tableSize);
