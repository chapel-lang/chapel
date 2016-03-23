proc testSlice(R, RU) {
  writeln("Tests of slicing ", R, " and ", RU, ":");
  writeln("R[RU] = ", R[RU], ": ", (R[RU]).type:string);
  writeln("RU[R] = ", RU[R], ": ", (RU[R]).type:string);
  writeln();
}

testSlice(-2..10, 2..11:uint);
testSlice(-5..-2, 2..11:uint);
testSlice(-5..-2, max(uint)-10..max(uint)-5);
testSlice(-2..10, max(uint)-10..max(uint)-5);
testSlice(2..11, 4..max(uint)-5);
testSlice(2..11, 13..max(uint)-5);
