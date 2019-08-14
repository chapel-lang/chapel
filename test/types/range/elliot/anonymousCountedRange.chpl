proc testAnonRanges(type lowT, type countT, param testErrors = false) {
  var zero = 0:countT;
  // Applying #0 to a 0.. uint range results in wraparound leading to an
  // error when trying to iterate over it, so skip those cases unless we
  // want to make sure the error is generated
  if (isIntType(lowT) || testErrors) {
    for i in 0:lowT..#(0:countT)               do write(i, ' '); writeln();
    for i in 0:lowT..#(zero)                   do write(i, ' '); writeln();
  } else {
    writeln();
    writeln();
  }
  for i in 0:lowT..#(1:countT)               do write(i, ' '); writeln();
  for i in 0:lowT..#(10:countT) by 2:lowT    do write(i, ' '); writeln();
  for i in (0:lowT.. by 2:lowT) #(10:countT) do write(i, ' '); writeln();
  for i in 10:lowT..#10:countT               do write(i, ' '); writeln();
}

proc testAnonRanges(param bits) {
  testAnonRanges(int(bits),  int(bits));
  testAnonRanges(int(bits),  uint(bits));
  testAnonRanges(uint(bits), int(bits));
  testAnonRanges(uint(bits), uint(bits));
}

testAnonRanges(8);
testAnonRanges(16);
testAnonRanges(32);
testAnonRanges(64);
testAnonRanges(uint(64), int(64), true);
