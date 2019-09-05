proc testAnonRanges(type lowT, type countT) {
  var zero = 0:countT;
  // Applying #0 to a 0.. uint range results in wraparound leading to
  // an error when trying to iterate over it when bounds checks are
  // on.
  for i in 0:lowT..#(0:countT)               do write(i, ' '); writeln();
  for i in 0:lowT..#(zero)                   do write(i, ' '); writeln();
  for i in 0:lowT..#(1:countT)               do write(i, ' '); writeln();
  for i in 0:lowT..#(10:countT) by 2:lowT    do write(i, ' '); writeln();
  for i in (0:lowT.. by 2:lowT) #(10:countT) do write(i, ' '); writeln();
  for i in 10:lowT..#10:countT               do write(i, ' '); writeln();
}








testAnonRanges(uint(64), int(64));
