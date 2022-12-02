use IO;

var maxCalories = 0,
    currentCalories = 0,
    nextCalories = 0;

while readln(nextCalories) {
    currentCalories += nextCalories;

    if stdin.matchNewline() {
        if currentCalories > maxCalories then
            maxCalories = currentCalories;
        currentCalories = 0;
    }
}

writeln(maxCalories);
