proc testit(type elemType, numArrays, returnPow2 = true, memRatio = 4) {
  writeln("numArrays = ", numArrays);
  writeln("returnPow2 = ", returnPow2);
  writeln("memRatio = ", memRatio);
  writeln();
}

testit(real(64), 3);
testit(real(64), 3, false);
testit(real(64), 3, true);
testit(real(64), 3, false, 8);
