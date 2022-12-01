use IO;

var line: string;
var totCalories, maxCalories = 0;

while readLine(line) {
  if line.size > 1 {
    totCalories += line:int;
  } else {
    if totCalories > maxCalories {
      maxCalories = totCalories;
    }
    totCalories = 0;
  }
}
writeln(maxCalories);
