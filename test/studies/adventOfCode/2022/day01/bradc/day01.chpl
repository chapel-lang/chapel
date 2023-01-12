use IO;

var line: string;
var currentCalories, maxCalories = 0;

do {
  const more = readLine(line),
        foundItem = (line.size > 1);

  if foundItem then
    currentCalories += (line: int);
    
  // If we are at the end of an elf's item list, update our maximum
  // value if appropriate and reset our tally for the next elf.
  if !more || !foundItem {
    if currentCalories > maxCalories {
      maxCalories = currentCalories;
    }
    currentCalories = 0;
  }
} while more;

writeln(maxCalories);
