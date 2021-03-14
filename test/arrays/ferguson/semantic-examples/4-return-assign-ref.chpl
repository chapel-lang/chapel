use myrecord;
var globalArray: [1..2] int;
proc returnGlobalArray() ref {
 return globalArray;
}
var A = returnGlobalArray();
A[1] = 1;
writeln(globalArray[1]);
// does this print out 0 or 1 ?
