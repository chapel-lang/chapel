use samples;
// ArrTwo defined in samples as [new R(2)]

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
// Does this output 0 or 2 ?
// In other words, did returnGlobalArray return the elements by
// value or reference?

// Compare with the record version, call-modifies-returned-record.chpl, which
// results in a compilation error.
