use Random;

config const seed = 31415, size = 5;

var A: [1..size, 1..size] real;

fillRandom(A, seed);

var (maxVal, maxLoc) = maxloc reduce (A, A.domain);
var (minVal, minLoc) = minloc reduce (A, A.domain);

writeln("A is:"); writeln(A);
writeln("The maximum value in A is: A", maxLoc, " = ", maxVal);
writeln("The minimum value in A is: A", minLoc, " = ", minVal);
writeln("The difference is: ", maxVal - minVal);
