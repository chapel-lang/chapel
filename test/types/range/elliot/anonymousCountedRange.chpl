proc testAnonRanges(type lowT, type countT) {
  var zero = 0:countT;
  for i in 0:lowT..#(0:countT)               do write(i, ' '); writeln();
  for i in 0:lowT..#(zero)                   do write(i, ' '); writeln();
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
