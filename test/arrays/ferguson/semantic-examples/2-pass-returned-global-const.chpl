use samples;
// one defined in samples as new R(1)

var globalArray: [1..2] R;
proc returnGlobalArray() const {
 return globalArray;
}
proc setToOne(const ref A:[] R) {
  globalArray[1] = one;
  writeln(A[1].x);
}
setToOne(returnGlobalArray());

// Does this program output 0 or 1?
// In other words, does the act of returning globalArray
// inside returnGlobalArray make a copy?
