use samples;
// ArrTwo defined in samples as [new R(2)]

var globalArray:[1..1] R;

proc returnGlobalArray() {
  return globalArray;
}

writeln("globalArray is ", globalArray);
writeln("setting returnGlobalArray() = ArrTwo");

returnGlobalArray() = ArrTwo;

writeln("globalArray is now ", globalArray);
writeln(globalArray);

// Can a function returning an array be used as an lvalue?
// If so, does the line
//    returnGlobalArray() = ArrTwo;
// change globalArray or an expression temporary?

// Compare with 0-init-callexpr-record.chpl, which results in a compilation
// error for a similar pattern with records.
