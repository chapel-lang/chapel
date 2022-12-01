use IO;

var line: string;
var totCalories, maxCalories = 0;

do {
  const more = readLine(line),
        foundItem = (line.size > 1);

  if foundItem then
    totCalories += line: int;
    
  // Are we at the end of an elf's item list?  If so, tabulate and set
  // up for the next elf.
  if !more || !foundItem {
    if totCalories > maxCalories {
      maxCalories = totCalories;
    }
    totCalories = 0;
  }
} while more;

writeln(maxCalories);
