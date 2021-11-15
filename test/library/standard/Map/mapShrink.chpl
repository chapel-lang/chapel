use Map;

var m = new map(int,int);
m[-1] = 0;

var initialSize = m.table.tableSize;

for i in 0..#initialSize/2 do
  m[i] = i;

assert(m.table.tableSize > initialSize);

for i in 0..#2+initialSize/4 {
  assert(m.table.tableSize > initialSize);
  m.remove(i);
}

assert(m.table.tableSize == initialSize);
