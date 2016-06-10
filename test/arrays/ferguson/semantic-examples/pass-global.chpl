use myrecord;
var globalArray: [1..2] R;
proc returnGlobalArray() {
 return globalArray;
}
proc consumesArray(A:[] R) {
 writeln(A[1]);
}
consumesArray(returnGlobalArray());
