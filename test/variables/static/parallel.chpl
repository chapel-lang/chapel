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

coforall i in 0..3 {
    writeln("From task ", i, ": ", getNthElement(0));
    writeln("From task ", i, ": ", getNthElement(1));
    writeln("From task ", i, ": ", getNthElement(2));
    writeln("From task ", i, ": ", getNthElement(3));
    writeln("From task ", i, ": ", getNthElement(4));
    writeln("From task ", i, ": ", getNthElement(5));
}
