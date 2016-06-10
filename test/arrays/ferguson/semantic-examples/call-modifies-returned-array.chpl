use samples;
// var ArrTwo = [new R(2)]

var globalArray:[1..1] R;

proc returnGlobalArray() {
  return globalArray;
}

proc setToTwo(A) {
  A = ArrTwo;
}

writeln("globalArray is ", globalArray);
writeln("running setToTwo(returnGlobalArray())");

setToTwo(returnGlobalArray());

writeln("globalArray is now ", globalArray);
writeln(globalArray);

