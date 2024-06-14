use CTypes;
use Time;

config param tableSize = 1000;

proc computeExpensiveTable(seed1: real, seed2: real) : c_array(real, tableSize) {
    writeln("Computing an expensive table!");
    var toReturn: c_array(real, tableSize);

    toReturn[0] = seed1;
    toReturn[1] = seed2;
    for i in 2..<tableSize {
        toReturn[i] = toReturn[i-1] + toReturn[i-2];
    }

    return toReturn;
}

proc getNthElement(x: int): real {
    @functionStatic
    ref table = computeExpensiveTable(1, 1);
    // var table = computeExpensiveTable(1, 1);
    return table[x];
}

writeln(getNthElement(0));
writeln(getNthElement(1));
writeln(getNthElement(2));
writeln(getNthElement(3));
writeln(getNthElement(4));
writeln(getNthElement(5));
