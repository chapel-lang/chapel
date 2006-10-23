def testit(type elemType, numArrays, returnPow2 = true, memRatio = 4) {
  writeln("numArrays = ", numArrays);
  writeln("returnPow2 = ", returnPow2);
  writeln("memRatio = ", memRatio);
  writeln();
}

testit(float(64), 3);
testit(float(64), 3, false);
testit(float(64), 3, true);
testit(float(64), 3, false, 8);
