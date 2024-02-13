use List;

// this works, creates a list of arrays
var a: list(?) = [[1,2,3],[4,5,6]];
compilerWarning(a.type:string);

// this segfaults at runtime, no matter the domain used for the array
var b: list([1..3] int) = [[1,2,3],[4,5,6]];
compilerWarning(b.type:string);
