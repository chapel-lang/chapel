use IO;

config const days = 80, debug = false;

var fishCount: [0..8] int;

var age: int;
while read(age) {
  fishCount[age] += 1;
  try {
    readf(",");
  } catch {
    break;
  }
}

writeln(fishCount);

for i in 1..days {
  var newCounts: [0..8] int;
  newCounts[0..7] = fishCount[1..8];
  newCounts[6] += fishCount[0];
  newCounts[8] += fishCount[0];
  fishCount = newCounts;
  writeln("After ", i, " day(s), school size is ", + reduce fishCount);
}
