var globalArray: [1..10000] real;
proc returnGlobalArray() ref {
 return globalArray;
}
var A = returnGlobalArray();
