use Set;

var s = new set(int, initialCapacity=50);

// fill map with a few elements
for i in 0..5 do
  s.add(i);

assert(s._htb.tableSize >= 50);

// ensure that removing the elements does not shrink the _htb
for i in 0..5 do
  s.remove(i);

assert(s._htb.tableSize >= 50);

var s2 = new set(int, initialCapacity=500);

for i in 0..5 do
  s2.add(i);

assert(s2._htb.tableSize >= 500);

for i in 0..5 do
  s2.remove(i);

assert(s2._htb.tableSize >= 500);
