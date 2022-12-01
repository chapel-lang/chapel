use IO;

var line: string;
var totCalories, maxCalories = 0;

while readLine(line) {
//  writeln(line, ":", line.size);
  if line.size > 1 {
    const calories = line:int;
//    writeln(calories, "...");
    totCalories += calories;
//    writeln("tot = ", totCalories);
  } else {
    if totCalories > maxCalories {
      maxCalories = totCalories;
//      writeln("max = ", maxCalories);
    }
    totCalories = 0;
  }
  line = "";
}
writeln(maxCalories);
