var globalArray: [1..10000] real;
proc returnGlobalArray() {
 return globalArray;
}
var A = returnGlobalArray();

