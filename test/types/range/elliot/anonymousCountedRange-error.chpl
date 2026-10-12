proc testAnonRanges(type lowT, type countT) {
  var zero = 0:countT;
  // Applying #0 to a 0.. uint range would result in wraparound, but we
  // exempt zero count ranges and have them early return. however, there are other
  // ways to get overflow (e.g., starting at INT_MAX and trying to iterate)
  for i in 0:lowT..#(0:countT)               do write(i, ' '); writeln();
  for i in 0:lowT..#(zero)                   do write(i, ' '); writeln();
  for i in 0:lowT..#(1:countT)               do write(i, ' '); writeln();
  for i in 0:lowT..#(10:countT) by 2:lowT    do write(i, ' '); writeln();
  for i in (0:lowT.. by 2:lowT) #(10:countT) do write(i, ' '); writeln();
  for i in 10:lowT..#10:countT               do write(i, ' '); writeln();
  for i in (max(lowT)..by 4:lowT) # 2:countT   do write(i, " "); writeln();
}

testAnonRanges(uint(64), int(64));
testAnonRanges(int(64), int(64));
