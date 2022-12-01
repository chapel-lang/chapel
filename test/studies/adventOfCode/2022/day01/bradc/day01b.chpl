use IO;

var line: string;
var totCalories = 0;
var maxCalories: [1..3] int = 0;

while readLine(line) {
//  writeln(line, ":", line.size);
  if line.size > 1 {
    const calories = line:int;
//    writeln(calories, "...");
    totCalories += calories;
  } else {
//    writeln("tot = ", totCalories);
    var replace = 0;
//    writeln("Before: ", maxCalories);
    for i in 1..3 {
      if totCalories > maxCalories[i] {
        replace = i;
      }
    }
    if replace {
      for i in 1..<replace {
        maxCalories[i] = maxCalories[i+1];
      }
      maxCalories[replace] = totCalories;
    }
//    writeln("After: ", maxCalories);
    totCalories = 0;
  }
  line = "";
}
//    writeln("tot = ", totCalories);
    var replace = 0;
//    writeln("Before: ", maxCalories);
    for i in 1..3 {
      if totCalories > maxCalories[i] {
        replace = i;
      }
    }
    if replace {
      for i in 1..<replace {
        maxCalories[i] = maxCalories[i+1];
      }
      maxCalories[replace] = totCalories;
    }
//    writeln("After: ", maxCalories);
    totCalories = 0;
//writeln(maxCalories);
writeln(+ reduce maxCalories);

