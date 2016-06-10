use samples;
// var ArrTwo = [new R(2)];

var globalArray:[1..1] R;

proc returnGlobalArray() {
  return globalArray;
}

writeln("globalArray is ", globalArray);
writeln("setting returnGlobalArray() = ArrTwo");

returnGlobalArray() = ArrTwo;

writeln("globalArray is now ", globalArray);
writeln(globalArray);

