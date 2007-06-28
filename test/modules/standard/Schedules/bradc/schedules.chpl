use Schedules;

config const lo = 1;
config const hi = 100;

config const numBlocks = 3;
config const blockSize = 15;

var Dom = [lo..hi];

// If these schedules are really meant to be used at the user level,
// should make them take a domain wrapper rather than a domain class.
// Unifying the name of dim_type and _dim_index_type would allow it
// to work for either
for b in BlockSchedule(Dom._value, numBlocks) {
  writeln("b is: ", b);
}
writeln();

for b in FixedBlockSchedule(Dom._value, blockSize) {
  writeln("b is: ", b);
}
