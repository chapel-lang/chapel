use List;

// this segfaults at runtime, no matter the domain used for the array.
// TODO: But only on CHPL_COMM != none...
var b: list([1..3] int) = [[1,2,3],[4,5,6]];
writeln(b.type:string, " = ", b);
