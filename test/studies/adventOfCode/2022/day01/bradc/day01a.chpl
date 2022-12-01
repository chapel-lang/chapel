use IO;

var line: string;
var totCalories = 0;
var maxCalories: [1..3] int = 0;

do {
  const more = readLine(line),
        foundItem = (line.size > 1);

  if foundItem then
    totCalories += line: int;
    
  // Are we at the end of an elf's item list?  If so, tabulate and set
  // up for the next elf.
  if !more || !foundItem {

    // Check whether the current total is greater than any/all of our
    // previous three maxes
    var insertIdx = 0;
    for i in 1..3 {
      if totCalories > maxCalories[i] {
        insertIdx = i;
      }
    }

    // If so, shift those values down and insert our new total
    if insertIdx {
      for i in 1..<insertIdx {
        maxCalories[i] = maxCalories[i+1];
      }
      maxCalories[insertIdx] = totCalories;
    }

    totCalories = 0;
  }
} while more;

writeln(+ reduce maxCalories);
