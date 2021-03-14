/* I like that Chapel, like C#, does not require literal suffixes to
   specify data type sizes */

var i1 = 1:int(8);
var i2 = 2:int(16);
var i3 = 3;
var i4 = 3000000000;
var i5 = 10000000000000000000;
var i6 = (-1):uint(32);

printVarInfo(i1, 1);
printVarInfo(i2, 2);
printVarInfo(i3, 3);
printVarInfo(i4, 4);
printVarInfo(i5, 5);
printVarInfo(i6, 6);

proc printVarInfo(val, num) {
  writeln("for i", num, "...");
  writeln("  value is: ", val);
  writeln("  #bits is: ", numBits(val.type));
  writeln("  maxvl is: ", max(val.type));
  writeln();
}
