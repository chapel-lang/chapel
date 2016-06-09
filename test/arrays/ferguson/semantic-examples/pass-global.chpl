var globalArray: [1..10000] real;
proc returnGlobalArray() {
 return globalArray;
}
proc consumesArray(A:[] real) {
 writeln(A[1]);
}
consumesArray(returnExistingArray());
