use IO;

config const days = 80;

var fishCount: [0..8] int;

do {
  var age: int;
  var success = read(age);
  if success {
    fishCount[age] += 1;
    try { readf(","); } catch { success = false; }
  }
} while success;

writeln(fishCount);

for i in 1..days {
  var newCounts: [0..8] int;
  newCounts[0..7] = fishCount[1..8];
  newCounts[6] += fishCount[0];
  newCounts[8] += fishCount[0];
  fishCount = newCounts;
//  writeln("After ", i, " day(s), school is ", school);
  writeln("After ", i, " day(s), school size is ", + reduce fishCount);
}
