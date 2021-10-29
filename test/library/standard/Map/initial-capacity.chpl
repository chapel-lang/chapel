use Map;

var m = new map(int, int, initialCapacity=50);

// fill map with a few elements
for i in 0..5 do
  m[i] = i;

assert(m.table.tableSize >= 50);

// ensure that removing the elements does not shrink the table
for i in 0..5 do
  m.remove(i);

assert(m.table.tableSize >= 50);

var m2 = new map(int, int, initialCapacity=500);

for i in 0..5 do
  m2[i] = i;

assert(m2.table.tableSize >= 500);

for i in 0..5 do
  m2.remove(i);

assert(m2.table.tableSize >= 500);
