use samples;
// var one = new R(1);

var globalArray: [1..2] R;
proc returnGlobalArray() const {
 return globalArray;
}
proc setToOne(ref A:[] R) {
  A[1] = one;
}
setToOne(returnGlobalArray());
writeln(globalArray[1]);

// Does this program output 0 or 1?
// In other words, does the act of returning globalArray
// inside returnGlobalArray make a copy?
