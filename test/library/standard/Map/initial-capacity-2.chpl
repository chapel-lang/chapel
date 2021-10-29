use Map;

const initialCapacity = 100;

var m = new map(int, int,
                initialCapacity=initialCapacity,
                resizeThreshold=0.3);
m[-1] = 0;
m.remove(-1);

var startingSize = m.table.tableSize;
writeln(m.table.tableSize);

// ensure that adding initialCapacity elements does
// not resize the map
for i in 0..#initialCapacity do
  m[i] = i;
writeln(m.table.tableSize);

assert(startingSize == m.table.tableSize);

var m2 = new map(int, int,
                 initialCapacity=initialCapacity,
                 resizeThreshold=0.75);
m2[-1] = 0;
m2.remove(-1);

startingSize = m2.table.tableSize;
writeln(m2.table.tableSize);

// ensure that adding initialCapacity elements does
// not resize the map
for i in 0..#initialCapacity do
  m2[i] = i;
writeln(m2.table.tableSize);

assert(startingSize == m2.table.tableSize);

