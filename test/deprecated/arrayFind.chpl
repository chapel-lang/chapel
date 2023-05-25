// This checks that the old array.find() interface generates a deprecation
// warning to help people transition to the new interface.

var A = [1, 2, 1, 3, 1, 4, 1, 5];
writeln("Finding 3 in A = ", A.find(3));
