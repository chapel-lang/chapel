use Map;

var m1 = new map(int, int);

var m2 = new map(int, int);

// Map can be up to 75% full
m2.setMaxCapacity(.75);

// Fill tables to 50% capacity
for i in 0..15 {
  m1[i] = i;
  m2[i] = i;
}

writeln("Map 1 capacity: ", m1.table.tableNumFullSlots:real/m1.table.tableSize);
writeln("Map 2 capacity: ", m2.table.tableNumFullSlots:real/m2.table.tableSize);

// Now over default 50% max capacity
m1[16] = 16;
m2[16] = 16;

writeln("Map 1 capacity: ", m1.table.tableNumFullSlots:real/m1.table.tableSize);
writeln("Map 2 capacity: ", m2.table.tableNumFullSlots:real/m2.table.tableSize);

// Fill second map to max capacity
for i in 17..23 {
  m1[i] = i;
  m2[i] = i;
}

writeln("Map 1 capacity: ", m1.table.tableNumFullSlots:real/m1.table.tableSize);
writeln("Map 2 capacity: ", m2.table.tableNumFullSlots:real/m2.table.tableSize);

// Now over 75% max capacity of map 2
m1[24] = 24;
m2[24] = 24;

writeln("Map 1 capacity: ", m1.table.tableNumFullSlots:real/m1.table.tableSize);
writeln("Map 2 capacity: ", m2.table.tableNumFullSlots:real/m2.table.tableSize);
