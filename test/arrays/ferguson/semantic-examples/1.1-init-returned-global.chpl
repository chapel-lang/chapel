use samples;
// one defined in samples as new R(1)

var globalArray: [1..2] R;
proc returnGlobalArray() {
 return globalArray;
}

var A = returnGlobalArray();

A[1] = one;

writeln(globalArray[1].x);
// Does this print out 1 or 0?
// In other words, does A refer to different elements from globalArray?
