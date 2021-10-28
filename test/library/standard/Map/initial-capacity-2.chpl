use Map;

const initialCapacity = 100;

var m = new map(int, int,
                initialCapacity=initialCapacity);
m[-1] = 0;
m.remove(-1);

const startingSize = m.table.tableSize;
writeln(m.table.tableSize);

// ensure that adding initialCapacity elements does
// not resize the map
for i in 0..#initialCapacity do
  m[i] = i;
writeln(m.table.tableSize);

assert(startingSize == m.table.tableSize);
